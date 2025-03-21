#include "param.h"
#include "types.h"
#include "memlayout.h"
#include "elf.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"

// Metadata of shared/CoW pages will be stored in array of struct uvm_meta
struct uvm_meta {
  // Key of associated array
  uint16 physical_page_number;

  // Values of associated array
  uint8 reference_count;
  enum uvm_type type : 8;
};
// No padding should be inserted
_Static_assert(sizeof(struct uvm_meta) == 4, "sizeof(struct uvm_meta) should be 4");

// Maximum length of meta_list
//
// NOTE: Limit is rather small
enum { META_LIST_MAX = 1000 };

// Length of meta[] array
static uint64 meta_list_length;
// NOTE: meta[] array is sorted by uvm_meta::physical_page_number
static struct uvm_meta meta_list[META_LIST_MAX];

struct meta_bisect_result {
  uint64 index;
  int exist;
};

// Return a right place for pa in meta_list[]
//
// If pa was already in meta_list[], exist = 1
// Otherwise, exist = 0
static struct meta_bisect_result
meta_bisect(const void *pa)
{
  if ((uint64)pa >= PHYSTOP)
    panic("meta_bisect: pa too large");
  if ((uint64)pa < KERNBASE)
    panic("meta_bisect: pa too small");
  uint16 query = (uint16)(((uint64)pa - KERNBASE)/PGSIZE);

  // Bisection range: [lo, hi)
  uint64 lo = 0;
  uint64 hi = meta_list_length;
  while (lo < hi) {
    uint64 mid = lo + (hi - lo)/2;
    uint16 result = meta_list[mid].physical_page_number;
    if (result == query) {
      return (struct meta_bisect_result) { .index = mid, .exist = 1 };
    } else if (result < query) {
      lo = mid + 1;
    } else { // result > query
      hi = mid;
    }
  }

  return (struct meta_bisect_result) { .index = lo, .exist = 0 };
}

// Start reference counting of given page
static void
meta_start_share(void *pa, enum uvm_type type)
{
  // Find a right place to insert new metadata
  const struct meta_bisect_result res = meta_bisect(pa);
  if (res.exist) {
    panic("meta_start_share: Given pa was already shared");
  }

  // Move existing entries
  if (meta_list_length == META_LIST_MAX - 1)
    panic("meta_start_share: Too many managed pages");
  memmove(&meta_list[res.index + 1], &meta_list[res.index], sizeof(struct uvm_meta)*(meta_list_length - res.index));
  // Increment length
  ++meta_list_length;

  // Insert new entry
  meta_list[res.index] = (struct uvm_meta) {
    .physical_page_number = (uint16)(((uint64)pa - KERNBASE)/PGSIZE),
    .reference_count = 1,
    .type = type,
  };
}

static struct uvm_meta *
meta_of(void *pa)
{
  const struct meta_bisect_result res = meta_bisect((void *)pa);
  if (!res.exist) {
    return 0;
  }
  struct uvm_meta *meta = &meta_list[res.index];
  if ((meta->type != UVM_SHARED && meta->type != UVM_COW) || meta->reference_count == 0) {
    panic("usertrap: invalid metadata");
  }
  return meta;
}

// Increase reference count
// If given pa is not reference counter, panic
// If integer overflow occurs, panic
static void
meta_incr(struct uvm_meta *meta)
{
  // Increment RC, check overflow
  if (meta->reference_count == 0xFF) {
    panic("meta_incr: reference counter integer overflow");
  }
  ++meta->reference_count;
}

// Decrease RC, free the page if RC become 0, panic on overflow
static void
meta_decr_of(void *pa)
{
  // Find metadata
  struct meta_bisect_result res = meta_bisect(pa);
  if (!res.exist) {
    panic("meta_decr_of: given pa was not shared");
  }

  // Decrement RC, check overflow
  uint8 * const p_counter = &meta_list[res.index].reference_count;
  if (*p_counter == 0) {
    panic("meta_decr_of: reference counter integer overflow");
  }

  --*p_counter;

  // If reference counter reached zero, delete the metadata and free the page
  if (*p_counter == 0) {
    // Move existing entries
    memmove(&meta_list[res.index], &meta_list[res.index + 1], sizeof(struct uvm_meta)*(meta_list_length - res.index - 1));
    // Increment length
    --meta_list_length;

    kfree(pa);
  }
}



/*
 * the kernel's page table.
 */
pagetable_t kernel_pagetable;

extern char etext[];  // kernel.ld sets this to end of kernel code.

extern char trampoline[]; // trampoline.S

/*
 * create a direct-map page table for the kernel and
 * turn on paging. called early, in supervisor mode.
 * the page allocator is already initialized.
 */
void
kvminit()
{
  kernel_pagetable = (pagetable_t) kalloc();
  memset(kernel_pagetable, 0, PGSIZE);

  // uart registers
  kvmmap(UART0, UART0, PGSIZE, PTE_R | PTE_W);

  // virtio mmio disk interface
  kvmmap(VIRTIO0, VIRTIO0, PGSIZE, PTE_R | PTE_W);

  // CLINT
  kvmmap(CLINT, CLINT, 0x10000, PTE_R | PTE_W);

  // PLIC
  kvmmap(PLIC, PLIC, 0x400000, PTE_R | PTE_W);

  // map kernel text executable and read-only.
  kvmmap(KERNBASE, KERNBASE, (uint64)etext-KERNBASE, PTE_R | PTE_X);

  // map kernel data and the physical RAM we'll make use of.
  kvmmap((uint64)etext, (uint64)etext, PHYSTOP-(uint64)etext, PTE_R | PTE_W);

  // map the trampoline for trap entry/exit to
  // the highest virtual address in the kernel.
  kvmmap(TRAMPOLINE, (uint64)trampoline, PGSIZE, PTE_R | PTE_X);
}

// Switch h/w page table register to the kernel's page table,
// and enable paging.
void
kvminithart()
{
  w_satp(MAKE_SATP(kernel_pagetable));
  sfence_vma();
}

// Return the address of the PTE in page table pagetable
// that corresponds to virtual address va.  If alloc!=0,
// create any required page-table pages.
//
// The risc-v Sv39 scheme has three levels of page-table
// pages. A page-table page contains 512 64-bit PTEs.
// A 64-bit virtual address is split into five fields:
//   39..63 -- must be zero.
//   30..38 -- 9 bits of level-2 index.
//   21..39 -- 9 bits of level-1 index.
//   12..20 -- 9 bits of level-0 index.
//    0..12 -- 12 bits of byte offset within the page.
static pte_t *
walk(pagetable_t pagetable, uint64 va, int alloc)
{
  if(va >= MAXVA)
    panic("walk");

  for(int level = 2; level > 0; level--) {
    pte_t *pte = &pagetable[PX(level, va)];
    if(*pte & PTE_V) {
      pagetable = (pagetable_t)PTE2PA(*pte);
    } else {
      if(!alloc || (pagetable = (pde_t*)kalloc()) == 0)
        return 0;
      memset(pagetable, 0, PGSIZE);
      *pte = PA2PTE(pagetable) | PTE_V;
    }
  }
  return &pagetable[PX(0, va)];
}

static pte_t *
walk_uvm_checked(pagetable_t pagetable, uint64 va)
{
  pte_t *pte;

  if(va >= MAXVA)
    return 0;

  pte = walk(pagetable, va, 0);
  if(pte == 0)
    return 0;
  if((*pte & PTE_V) == 0)
    return 0;
  if((*pte & PTE_U) == 0)
    return 0;
  return pte;
}

// Look up a virtual address, return the physical address,
// or 0 if not mapped.
// Can only be used to look up user pages.
uint64
walkaddr(pagetable_t pagetable, uint64 va)
{
  pte_t *pte = walk_uvm_checked(pagetable, va);
  return PTE2PA(*pte);
}

// add a mapping to the kernel page table.
// only used when booting.
// does not flush TLB or enable paging.
void
kvmmap(uint64 va, uint64 pa, uint64 sz, int perm)
{
  if(mappages(kernel_pagetable, va, sz, pa, perm) != 0)
    panic("kvmmap");
}

// translate a kernel virtual address to
// a physical address. only needed for
// addresses on the stack.
// assumes va is page aligned.
uint64
kvmpa(uint64 va)
{
  uint64 off = va % PGSIZE;
  pte_t *pte;
  uint64 pa;
  
  pte = walk(kernel_pagetable, va, 0);
  if(pte == 0)
    panic("kvmpa");
  if((*pte & PTE_V) == 0)
    panic("kvmpa");
  pa = PTE2PA(*pte);
  return pa+off;
}

// Create PTEs for virtual addresses starting at va that refer to
// physical addresses starting at pa. va and size might not
// be page-aligned. Returns 0 on success, -1 if walk() couldn't
// allocate a needed page-table page.
int
mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm)
{
  uint64 a, last;
  pte_t *pte;

  a = PGROUNDDOWN(va);
  last = PGROUNDDOWN(va + size - 1);
  for(;;){
    if((pte = walk(pagetable, a, 1)) == 0)
      return -1;
    if(*pte & PTE_V)
      panic("remap");
    *pte = PA2PTE(pa) | perm | PTE_V;
    if(a == last)
      break;
    a += PGSIZE;
    pa += PGSIZE;
  }
  return 0;
}

// Remove mappings from a page table. The mappings in
// the given range must exist. Optionally free the
// physical memory.
void
uvmunmap(pagetable_t pagetable, uint64 va, uint64 size, int do_free)
{
  uint64 a, last;
  pte_t *pte;
  uint64 pa;

  a = PGROUNDDOWN(va);
  last = PGROUNDDOWN(va + size - 1);
  for(;;){
    if((pte = walk(pagetable, a, 0)) == 0)
      panic("uvmunmap: walk");
    if((*pte & PTE_V) == 0){
      printf("va=%p pte=%p\n", a, *pte);
      panic("uvmunmap: not mapped");
    }
    if(PTE_FLAGS(*pte) == PTE_V)
      panic("uvmunmap: not a leaf");
    if(do_free){
      pa = PTE2PA(*pte);
      meta_decr_of((void*)pa);
    }
    *pte = 0;
    if(a == last)
      break;
    a += PGSIZE;
    pa += PGSIZE;
  }
}

// create an empty user page table.
pagetable_t
uvmcreate()
{
  pagetable_t pagetable;
  pagetable = (pagetable_t) kalloc();
  if(pagetable == 0)
    panic("uvmcreate: out of memory");
  memset(pagetable, 0, PGSIZE);
  return pagetable;
}

// Load the user initcode into address 0 of pagetable,
// for the very first process.
// sz must be less than a page.
void
uvminit(pagetable_t pagetable, uchar *src, uint sz)
{
  char *mem;

  if(sz >= PGSIZE)
    panic("inituvm: more than a page");
  mem = kalloc();
  memset(mem, 0, PGSIZE);
  mappages(pagetable, 0, PGSIZE, (uint64)mem, PTE_W|PTE_R|PTE_X|PTE_U);
  meta_start_share(mem, UVM_COW);
  memmove(mem, src, sz);
}

// Allocate PTEs and physical memory to grow process from oldsz to
// newsz, which need not be page aligned.  Returns new size or 0 on error.
//
// Map new pages with given permission. Perform reference counting.
uint64
uvmalloc(pagetable_t pagetable, uint64 oldsz, uint64 newsz, int perm, enum uvm_type type)
{
  if (type != UVM_SHARED && type != UVM_COW)
    panic("uvmalloc: Invalid type");

  char *mem;
  uint64 a;

  if(newsz < oldsz)
    return oldsz;

  oldsz = PGROUNDUP(oldsz);
  a = oldsz;
  for(; a < newsz; a += PGSIZE){
    mem = kalloc();
    if(mem == 0){
      uvmdealloc(pagetable, a, oldsz);
      return 0;
    }
    memset(mem, 0, PGSIZE);
    if(mappages(pagetable, a, PGSIZE, (uint64)mem, perm) != 0){
      kfree(mem);
      uvmdealloc(pagetable, a, oldsz);
      return 0;
    }
    meta_start_share(mem, type);
  }
  return newsz;
}

// Deallocate user pages to bring the process size from oldsz to
// newsz.  oldsz and newsz need not be page-aligned, nor does newsz
// need to be less than oldsz.  oldsz can be larger than the actual
// process size.  Returns the new process size.
uint64
uvmdealloc(pagetable_t pagetable, uint64 oldsz, uint64 newsz)
{
  if(newsz >= oldsz)
    return oldsz;

  uint64 newup = PGROUNDUP(newsz);
  if(newup < PGROUNDUP(oldsz))
    uvmunmap(pagetable, newup, oldsz - newup, 1);

  return newsz;
}

// Recursively free page-table pages.
// All leaf mappings must already have been removed.
#ifdef SNU
void
#else
static void
#endif
freewalk(pagetable_t pagetable)
{
  // there are 2^9 = 512 PTEs in a page table.
  for(int i = 0; i < 512; i++){
    pte_t pte = pagetable[i];
    if((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0){
      // this PTE points to a lower-level page table.
      uint64 child = PTE2PA(pte);
      freewalk((pagetable_t)child);
      pagetable[i] = 0;
    } else if(pte & PTE_V){
      panic("freewalk: leaf");
    }
  }
  kfree((void*)pagetable);
}

// Free user memory pages,
// then free page-table pages.
void
uvmfree(pagetable_t pagetable, uint64 sz)
{
  uvmunmap(pagetable, 0, sz, 1);
  freewalk(pagetable);
}

// Given a parent process's page table, share
// its memory into a child's page table.
// Share both the page table and the
// physical memory.
//
// returns 0 on success, -1 on failure.
// frees any allocated pages on failure.
int
uvmcopy(pagetable_t old, pagetable_t new, uint64 sz)
{
  pte_t *pte;
  uint64 pa, i;
  uint flags;
  char *mem;

  for(i = 0; i < sz; i += PGSIZE){
    if((pte = walk(old, i, 0)) == 0)
      panic("uvmcopy: pte should exist");
    if((*pte & PTE_V) == 0)
      panic("uvmcopy: page not present");
    pa = PTE2PA(*pte);
    flags = PTE_FLAGS(*pte);

    mem = (char*)pa;
    struct uvm_meta *meta = meta_of((void*)pa);
    meta_incr(meta);
    // Set as unwritable if CoW borrow occured
    if (meta->type == UVM_COW) {
      *pte &= ~PTE_W;
      flags &= ~PTE_W;
    }

    if(mappages(new, i, PGSIZE, (uint64)mem, flags) != 0){
      kfree(mem);
      goto err;
    }
  }
  return 0;

 err:
  uvmunmap(new, 0, i, 1);
  return -1;
}

// mark a PTE invalid for user access.
// used by exec for the user stack guard page.
void
uvmclear(pagetable_t pagetable, uint64 va)
{
  pte_t *pte;
  
  pte = walk(pagetable, va, 0);
  if(pte == 0)
    panic("uvmclear");
  *pte &= ~PTE_U;
}

// Copy from kernel to user.
// Copy len bytes from src to virtual address dstva in a given page table.
// Return 0 on success, -1 on error.
int
copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len)
{
  while(len > 0){
    uint64 va0 = PGROUNDDOWN(dstva);
    pte_t *pte = walk_uvm_checked(pagetable, va0);
    uint64 pa0 = PTE2PA(*pte);
    if(pa0 == 0)
      return -1;
    uint64 n = PGSIZE - (dstva - va0);
    if(n > len)
      n = len;

    struct uvm_meta *meta = meta_of((void *)pa0);
    if (meta == 0) {
      panic("copyout: All UVMs should be shared");
    }
    if (meta->type == UVM_SHARED) {
      panic("copyout: Tried to write on shared readonly page");
    }

    if (meta->reference_count == 1) {
      // This CoW page is not shared, OK to write it
      *pte |= PTE_W;
    } else {
      // This CoW page is being shared, copy and write it
      --meta->reference_count;

      // TODO: No error handling
      void *mem = kalloc();
      memmove(mem, (void *)pa0, PGSIZE);
      int flags = PTE_FLAGS(*pte);
      *pte = PA2PTE(mem) | flags | PTE_W | PTE_V;
      meta_start_share(mem, UVM_COW);
    }
    uint64 pa1 = PTE2PA(*pte);
    memmove((void *)(pa1 + (dstva - va0)), src, n);

    len -= n;
    src += n;
    dstva = va0 + PGSIZE;
  }
  return 0;
}

// Copy from user to kernel.
// Copy len bytes to dst from virtual address srcva in a given page table.
// Return 0 on success, -1 on error.
int
copyin(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len)
{
  uint64 n, va0, pa0;

  while(len > 0){
    va0 = PGROUNDDOWN(srcva);
    pa0 = walkaddr(pagetable, va0);
    if(pa0 == 0)
      return -1;
    n = PGSIZE - (srcva - va0);
    if(n > len)
      n = len;
    memmove(dst, (void *)(pa0 + (srcva - va0)), n);

    len -= n;
    dst += n;
    srcva = va0 + PGSIZE;
  }
  return 0;
}

// Copy a null-terminated string from user to kernel.
// Copy bytes to dst from virtual address srcva in a given page table,
// until a '\0', or max.
// Return 0 on success, -1 on error.
int
copyinstr(pagetable_t pagetable, char *dst, uint64 srcva, uint64 max)
{
  uint64 n, va0, pa0;
  int got_null = 0;

  while(got_null == 0 && max > 0){
    va0 = PGROUNDDOWN(srcva);
    pa0 = walkaddr(pagetable, va0);
    if(pa0 == 0)
      return -1;
    n = PGSIZE - (srcva - va0);
    if(n > max)
      n = max;

    char *p = (char *) (pa0 + (srcva - va0));
    while(n > 0){
      if(*p == '\0'){
        *dst = '\0';
        got_null = 1;
        break;
      } else {
        *dst = *p;
      }
      --n;
      --max;
      p++;
      dst++;
    }

    srcva = va0 + PGSIZE;
  }
  if(got_null){
    return 0;
  } else {
    return -1;
  }
}

#ifdef SNU
uint64
v2p(pagetable_t pagetable, uint64 va)
{
  uint64 pa;
  pte_t *pte;

  if((pte = walk(pagetable, va, 0)) == 0)
    return -2;      /* pte not found */
  if((*pte & PTE_V) == 0)
    return -3;      /* not valid */
  pa = PTE2PA(*pte);
  return pa;  
}
#endif

// Return 1 if user performed invalid access
int
handle_cow_write(uint64 va, pagetable_t pagetable)
{
  if (va >= MAXVA) { return 1; }
  pte_t *pte = walk(pagetable, va, 0);
  if (pte == 0) { return 1; }
  if ((*pte & PTE_V) == 0) { return 1; }
  if ((*pte & PTE_U) == 0) { return 1; }
  uint64 pa = PTE2PA(*pte);

  struct uvm_meta *meta = meta_of((void *)pa);
  if (meta == 0) {
    panic("handle_cow_write: All UVMs should be shared");
  }

  if (meta->type == UVM_SHARED) {
    return 1;
  }
  if (meta->reference_count == 1) {
    // This CoW page is not shared, OK to write it
    *pte |= PTE_W;
  } else {
    // This CoW page is being shared, copy and write it
    --meta->reference_count;

    // TODO: No error handling
    void *mem = kalloc();
    memmove(mem, (void *)pa, PGSIZE);
    int flags = PTE_FLAGS(*pte);
    *pte = PA2PTE(mem) | flags | PTE_W | PTE_V;
    meta_start_share(mem, UVM_COW);
  }
  return 0;
}

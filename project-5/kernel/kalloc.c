// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

#ifdef SNU
uint64 freemem = 0;
#endif

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
  uint8 reference_counter[(PHYSTOP - KERNBASE)/PGSIZE];
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
#ifdef SNU
  freemem++;
#endif

  // TODO: Remove assertion
  if (kmem.reference_counter[((uint64)r - KERNBASE)/PGSIZE]) {
    kmem.reference_counter[((uint64)r - KERNBASE)/PGSIZE] = 0;
    panic("kfree: Reference counter is not zero");
  }

  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
#ifdef SNU
  {
    kmem.freelist = r->next;
    freemem--;
  }
#else
    kmem.freelist = r->next;
#endif
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}

// Increase RC, panic on overflow
void
krc_incr(void *pa)
{
  uint8 * const p_rc = &kmem.reference_counter[((uint64)pa - KERNBASE)/PGSIZE];

  if (*p_rc == 0xFF) {
    panic("kcr_incr: reference counter integer overflow");
  }

  ++*p_rc;
}

// Decrease RC, free the page if RC become 0, panic on overflow
void
krc_decr(void *pa)
{
  uint8 * const p_rc = &kmem.reference_counter[((uint64)pa - KERNBASE)/PGSIZE];

  // TODO: Remove assertion
  if (*p_rc == 0) {
    panic("kcr_decr: reference counter integer overflow");
    return;
  }

  --*p_rc;

  if (*p_rc == 0) {
    kfree(pa);
  }
}

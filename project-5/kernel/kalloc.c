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

// Metadata of shared/CoW pages will be stored in array of struct page_metadata
struct page_metadata {
  // key
  uint16 physical_page_number;
  // values
  uint8 reference_count;
  uint8 __reserved;
};
// No padding should be inserted in struct page_metadata
_Static_assert(sizeof(struct page_metadata) == 4, "sizeof(struct page_metadata) should be 4");
// Maximum length of page_metadata
// TODO: Limit is rather small
enum { MAX_PAGE_METADATA = 1000 };

struct {
  struct spinlock lock;
  struct run *freelist;

  // Length of meta[] array
  uint64 meta_length;
  // NOTE: meta[] array is sorted by page_metadata::physical_page_number
  struct page_metadata meta[MAX_PAGE_METADATA];
} kmem;

struct meta_bisect_result {
  uint64 index;
  int exist;
};

// Return a right place for pa in kmem.meta[]환
//
// If pa was already in kmem.meta[], exist = 1
// Otherwise, exist = 0
static struct meta_bisect_result
meta_bisect(const void *pa)
{
  uint16 query = (uint16)(((uint64)pa - KERNBASE)/PGSIZE);

  // Bisection range: [lo, hi)
  uint64 lo = 0;
  uint64 hi = kmem.meta_length;
  while (lo < hi) {
    uint64 mid = lo + (hi - lo)/2;
    uint16 result = kmem.meta[mid].physical_page_number;
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

  if (krc_is_shared(pa)) {
    panic("kfree: kfree called before removing metadata");
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

// Start reference counting of given page
void
krc_start_share(void *pa)
{
  // Find a right place to insert new metadata
  const struct meta_bisect_result res = meta_bisect(pa);
  if (res.exist) {
    panic("krc_start_share: Given pa was already shared");
  }

  // Move existing entries
  memmove(&kmem.meta[res.index + 1], &kmem.meta[res.index], sizeof(struct page_metadata)*(kmem.meta_length - res.index));
  // Increment length
  ++kmem.meta_length;

  // Insert new entry
  kmem.meta[res.index] = (struct page_metadata) {
    .physical_page_number = (uint16)(((uint64)pa - KERNBASE)/PGSIZE),
    .reference_count = 1,
  };
}

// If given pa is reference counted, return 1
// Otherwise, return 0
//
// NOTE: 전체적으로 meta[] 에 접근할때에 락도 안하고있고, 이렇게 is_shared 같은
// API를 만들면 TOCTOU 문제가 생길 수 있음. 이 과제에선 싱글코어 CPU로 제한했기
// 때문에 그냥 이렇게 구현한다.
int
krc_is_shared(const void *pa)
{
  // TODO: Change required at CoW implementation
  struct meta_bisect_result res = meta_bisect(pa);
  return res.exist;
}

// Increase reference count
// If given pa is not reference counter, panic
// If integer overflow occurs, panic
void
krc_incr(void *pa)
{
  // Find metadata
  struct meta_bisect_result res = meta_bisect(pa);
  if (!res.exist) {
    panic("kcr_incr: given pa was not reference counter");
  }

  // Increment RC, check overflow
  uint8 * const p_counter = &kmem.meta[res.index].reference_count;
  if (*p_counter == 0xFF) {
    panic("kcr_incr: reference counter integer overflow");
  }
  ++*p_counter;
}

// Decrease RC, free the page if RC become 0, panic on overflow
void
krc_decr(void *pa)
{
  // Find metadata
  struct meta_bisect_result res = meta_bisect(pa);
  if (!res.exist) {
    panic("kcr_incr: given pa was not reference counter");
  }

  // Decrement RC, check overflow
  uint8 * const p_counter = &kmem.meta[res.index].reference_count;
  if (*p_counter == 0) {
    panic("kcr_decr: reference counter integer overflow");
  }

  --*p_counter;

  // If reference counter reached zero, delete the metadata and free the page
  if (*p_counter == 0) {
    // Move existing entries
    memmove(&kmem.meta[res.index], &kmem.meta[res.index + 1], sizeof(struct page_metadata)*(kmem.meta_length - res.index - 1));
    // Increment length
    --kmem.meta_length;

    kfree(pa);
  }
}

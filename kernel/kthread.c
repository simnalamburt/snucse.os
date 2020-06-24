//--------------------------------------------------------------------
//
//  4190.307: Operating Systems (Spring 2020)
//
//  PA#6: Kernel Threads
//
//  June 2, 2020
//
//  Jin-Soo Kim
//  Systems Software and Architecture Laboratory
//  Department of Computer Science and Engineering
//  Seoul National University
//
//--------------------------------------------------------------------

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "proc.h"
#include "defs.h"


// Steal variables and functions from proc.c & vm.c
extern struct proc proc[NPROC];
extern int allocpid();
extern pagetable_t kernel_pagetable;

static void
kthread_entrypoint(void)
{
  // 맨 처음 스케줄링되면, 프로세스가 락된 상태임
  struct proc *p = myproc();
  release(&p->lock);

  // Jump into the entrypoint, never to return.
  p->entry(p->entry_arg);
  panic("Kernel thread finished without calling kthread_exit()");
}


#ifdef SNU
int
kthread_create(const char *name, int prio, void (*fn)(void *), void *arg)
{
  // Check prio
  if (prio < 0 || 100 <= prio) { return -1; }

  struct proc *p;

  for(p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if(p->state == UNUSED) {
      goto found;
    } else {
      release(&p->lock);
    }
  }
  return -1;

found:
  p->parent = 0;
  p->chan = 0;
  p->killed = 0;
  p->xstate = 0;
  p->pid = allocpid();

  p->sz = PGSIZE;
  p->pagetable = kernel_pagetable; // Use kernel address space
  p->tf = 0; // No trapframe, trying to access tf of kernel thread will cause panic

  // Initialize context
  memset(&p->context, 0, sizeof p->context);
  p->context.ra = (uint64)kthread_entrypoint;
  p->context.sp = p->kstack + PGSIZE;

  // Leave p->ofile untouched
  p->cwd = 0; // No cwd, trying to access cwd of kernel thread will cause panic
  safestrcpy(p->name, name, sizeof(p->name));

  // Save kernel thread specific informations
  p->is_kernel_thread = 1;
  p->base_prio = prio;
  p->entry = fn;
  p->entry_arg = arg;

  p->state = RUNNABLE;

  release(&p->lock);

  // If new thread's priority is higher than current thread, yield
  if (prio < kthread_get_prio()) {
    kthread_yield();
  }

  return p->pid;
}

// This function must be called only in kernel thread
void
kthread_exit(void)
{
  struct proc *p = myproc();
  acquire(&p->lock);

  p->parent = 0;
  p->chan = 0;
  p->killed = 0;
  p->xstate = 0;
  p->pid = 0;
  p->is_kernel_thread = 0;
  p->base_prio = 0;

  p->sz = 0;
  p->pagetable = 0;
  p->tf = 0;

  p->cwd = 0;
  p->name[0] = 0;
  p->entry = 0;
  p->entry_arg = 0;

  p->state = UNUSED;

  // Jump into the scheduler, never to return.
  sched();
  panic("Tried to run finished kernel thread");
}

void
kthread_yield(void)
{
  yield();
}

int
kthread_get_prio_of_locked(struct proc *p)
{
  // TODO: base prio가 아니라 effective prio 반환하기
  return p->base_prio;
}

void
kthread_set_prio(int newprio)
{
  struct proc *p = myproc();
  acquire(&p->lock);

  // TODO: Possible optimization?
  int before = kthread_get_prio_of_locked(p);
  p->base_prio = newprio;
  int after = kthread_get_prio_of_locked(p);

  release(&p->lock);

  if (after > before) {
    kthread_yield();
  }
}

int
kthread_get_prio(void)
{
  struct proc *p = myproc();
  acquire(&p->lock);

  int prio = kthread_get_prio_of_locked(p);

  release(&p->lock);
  return prio;
}
#endif

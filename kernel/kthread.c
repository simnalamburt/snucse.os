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
_kthread_entrypoint(void)
{
  void (*entry)(void *);
  void *entry_arg;

  struct proc *p = myproc();
  entry = p->entry;
  entry_arg = p->entry_arg;
  release(&p->lock);

  // Jump into the entrypoint, never to return.
  entry(entry_arg);
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
  p->context.ra = (uint64)_kthread_entrypoint;
  p->context.sp = p->kstack + PGSIZE;

  // Leave p->ofile untouched
  p->cwd = 0; // No cwd, trying to access cwd of kernel thread will cause panic
  safestrcpy(p->name, name, sizeof(p->name));

  // Save kernel thread specific informations
  p->is_kernel_thread = 1;
  p->base_prio = prio;
  p->entry = fn;
  p->entry_arg = arg;
  // TODO: 프로그램 카운터와 switch.S가 보존시키지 않는 각종 레지스터들 보존할 장소 필요함

  p->state = RUNNABLE;

  release(&p->lock);
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

  p->sz = 0;
  p->pagetable = 0;
  p->tf = 0;

  p->cwd = 0;
  p->name[0] = 0;

  p->state = UNUSED;

  // Jump into the scheduler, never to return.
  sched();
  panic("Tried to run finished kernel thread");
}

void
kthread_yield(void)
{
  // TODO: yield하즈아

  return;
}

void
kthread_set_prio(int newprio)
{
  // TODO: 구현하기




  return;
}

int
kthread_get_prio(void)
{
  // TODO: 구현하기



  return 0;
}
#endif

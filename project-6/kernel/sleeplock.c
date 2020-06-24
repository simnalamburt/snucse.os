// Sleeping locks

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"

void
initsleeplock(struct sleeplock *lk, char *name)
{
  initlock(&lk->lk, "sleep lock");
  lk->name = name;
  lk->locked = 0;
  lk->pid = 0;
}

void
acquiresleep(struct sleeplock *lk)
{
  acquire(&lk->lk);
  while (lk->locked) {
    sleep(lk, &lk->lk);
  }
  lk->locked = 1;
  struct proc *p = myproc();
  lk->pid = p->pid;

  // Add to the acquired list
  p->acquired_sleeplock[p->acquired_sleeplock_num] = lk;
  p->acquired_sleeplock_num += 1;
  // TODO: 길이제한 32 넘을경우 패닉해야함

  release(&lk->lk);
}

void
releasesleep(struct sleeplock *lk)
{
  acquire(&lk->lk);

  // Remove from acquired list
  struct proc *p = myproc();
  // TODO: 리스트 길이가 이미 0일경우 패닉해야함
  for (int i = 0; i < p->acquired_sleeplock_num; ++i) {
    if (p->acquired_sleeplock[i] == lk) {
      p->acquired_sleeplock_num -= 1;
      p->acquired_sleeplock[i] = p->acquired_sleeplock[p->acquired_sleeplock_num];
      goto removed;
    }
  }
  panic("lk was not found in acqruied list");
removed:

  lk->locked = 0;
  lk->pid = 0;
  wakeup(lk);
  release(&lk->lk);

  // 프라이오리티 도네이션이 있었을경우, 바로 yield해야한다.
  kthread_yield();
}

int
holdingsleep(struct sleeplock *lk)
{
  int r;
  
  acquire(&lk->lk);
  r = lk->locked && (lk->pid == myproc()->pid);
  release(&lk->lk);
  return r;
}




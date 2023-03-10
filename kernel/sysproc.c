#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  struct proc *p;
  uint64 va;
  uint64 ra;
  int ln, ct;
  unsigned char mk;
  unsigned int pto0, pto1, pto2;
  pte_t pte0, pte1, pte2;

  argaddr(0, &va);
  argint(1, &ln);
  argaddr(2, &ra);
  
  mk = 0;
  pto2 = PX(2, va);
  pto1 = PX(1, va);
  pto0 = PX(0, va);
  p = myproc();

  for(ct=0; ct<ln; ct++) {
    pte2 = p->pagetable[pto2];
    pte1 = ((pagetable_t)PTE2PA(pte2))[pto1];
    pte0 = ((pagetable_t)PTE2PA(pte1))[pto0];
    if((pte0&PTE_V) && (pte0&PTE_A)) {
      mk |= (1<<(ct%8));
      ((pagetable_t)PTE2PA(pte1))[pto0] &= (~PTE_A);
    }
    if(ct%8==7) {
      copyout(p->pagetable, ra+(ct/8), (char*)&mk, 1);
      mk = 0;
    }
    if(pto0<511) { //wrap-around search range
      pto0++;
    } else {
      pto0=0;
      if(pto1<511) {
        pto1++;
      } else {
        pto1=0;
        if(pto2<511) {
          pto2++;
        } else {
          pto2=0;
        }
      }
    }
  }

  if(ct%8!=0) {
      copyout(p->pagetable, ra+(ct/8), (char*)&mk, 1);
      mk = 0;
  }

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

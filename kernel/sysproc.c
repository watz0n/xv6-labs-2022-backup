#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
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
  if(n < 0)
    n = 0;
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

  backtrace();

  return 0;
}

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


uint64
sys_sigalarm(void)
{
  struct proc *p;
  int ticks;
  uint64 funcptr;

  p = myproc();
  argint(0, &ticks);
  argaddr(1, &funcptr);

  if(p->alarm_epc == 0) {
    p->tick_lim = ticks;
    p->tick_cnt = 0;
    p->alarm_handler = (void (*)(void))funcptr;
  }
  else if((ticks==0) && (funcptr==0)) {
    p->tick_cnt = p->tick_lim;
  }

  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc *p;
  p = myproc();

  p->trapframe->ra  = p->alarm_ra ;
  p->trapframe->s0  = p->alarm_s0 ;
  p->trapframe->sp  = p->alarm_sp ;
  p->trapframe->a0  = p->alarm_a0 ;
  p->trapframe->a1  = p->alarm_a1 ;
  p->trapframe->a2  = p->alarm_a2 ;
  p->trapframe->a3  = p->alarm_a3 ;
  p->trapframe->a4  = p->alarm_a4 ;
  p->trapframe->a5  = p->alarm_a5 ;
  p->trapframe->a6  = p->alarm_a6 ;
  p->trapframe->a7  = p->alarm_a7 ;
  p->trapframe->epc = p->alarm_epc;
  p->alarm_epc = 0;

  return p->alarm_a0;
}
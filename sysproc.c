#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "pstat.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Give the current date from CMOS clock
// Should be passed a RTCDate struct to give data back in
int
sys_date(void)
{
	struct rtcdate *r;

	if(argptr(0, (void*)&r, sizeof(*r)) < 0) {
		return -1;
	}
	cmostime(r);
	return 0;
}

// Sets number of tickets for a process
// number is number of tickets for a process
int
sys_settickets(void)
{
	int number;
	argint(0, &number);
	if(number > 0 && number < 100000) {
		struct proc *curproc = myproc();
		curproc->tickets = number;
		return 0;
	} else {
		return -1;
	}
}

// Get information about current running process
// Should be given a struct to give the info back in
int
sys_getpinfo(void)
{
	struct pstat *pinfo;
	if(argptr(0, (void*)&pinfo, sizeof(*pinfo)) < 0) {
		return -1;
	}
	return getpinfo(pinfo);
}

int
sys_yield(void)
{
	yield();
	return 0;
}

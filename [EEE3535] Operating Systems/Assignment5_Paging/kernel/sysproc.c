#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
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
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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

uint64
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

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
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








// Return the physical address of a virtual address.
uint64
sys_phyaddr(void* addr)
{ 
  uint64 va;		//to get virtual address
  uint64 pa;		//to get physical address
  pte_t *pte;		//pte
  uint64 offset;	//to get offset value from va
 
  //get into pagetable(L2)  
  pagetable_t pagetable= myproc()->pagetable;
  
  if(argaddr(0, &va)==-1){return -1;}	//bring va value
  else{
    offset=((uint64)(va<<52))>>52;	//get offset value
    
    //go to pagetable and bring page
    for (int level = 2; level >0; level--){
      pte = &pagetable[PX(level,va)];
      pagetable = (pagetable_t)PTE2PA(*pte);
    }

    //bring pte at L0
    pte=&pagetable[PX(0,va)];
    
    //get physical address value
    pa = PTE2PA(*pte)+offset;
    return pa;
  }
}

// Return the page table (or directory) index of a virtual address
// at the specified page table level.
uint64
sys_ptidx(void *addr, int l)
{ 
  uint64 va;	//to bring virtual address

  if(argaddr(0, &va)==-1){return -1;}
  else{
    return PX(l,va);}	//return index for the given level
}

// Count the total number pages allocated by a process.
uint64
sys_pgcnt(void)
{
  //pagetable for each level
  pagetable_t L2;
  pagetable_t L1;
  pagetable_t L0;

  //to get pte
  pte_t *pte;

  //bring L2 pagetable
  L2 = myproc()->pagetable;

  int cnt=0;	//counter for page

  //count every valid page
  for(int ent2 = 0; ent2 < 512; ent2++){	//check every index of L2
    pte = &L2[ent2];
    if ((*pte & PTE_V)){			//if pte is valid
      L1 = (pagetable_t)PTE2PA(*pte);		//get L1 pagetable
      for(int ent1 = 0; ent1 <512; ent1++){	//check every index of L1
	pte = &L1[ent1];
	if (*pte & PTE_V){			//if pte is valid
	  L0 = (pagetable_t)PTE2PA(L1[ent1]);	//get L0 pagetable
	  for (int ent0 = 0; ent0 < 512; ent0++){//check every index of L0
	    pte = &L0[ent0];
	    if(*pte&PTE_V){			//if pte is valid(=there is a page)
	      cnt++;				//count
	    }
	  }
	}
      }
    }
  }
  
  return cnt;	//return number of pages
}


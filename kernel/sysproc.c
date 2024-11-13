#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sha256.h"


uint64 sys_sha256(void) {
    char input[1024];           // Declare a character array to store input
    char output[SHA256_BLOCK_SIZE];

    // Fetch arguments from the user space
    if (argstr(0, input, sizeof(input)) < 0) {
        return -1; // Argument fetching failed
    }
    printf("%s \n", input);

    // Perform SHA-256 hash on the input
    SHA256_answer((BYTE *)input, (BYTE *)output);

    printf("SHA-256 hash with syscall!: ");

    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        char byte = output[i]; // Access each BYTE
        // Print each nibble of the BYTE
        printf("%x",byte);
    }


    // Copy the result back to user space
    if (copyout(myproc()->pagetable, (uint64)output, output, SHA256_BLOCK_SIZE) < 0) {
        return -1; // Copying back to user space failed
    }

    return 0; // Success
}


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
sys_get_syscall(void)
{
    struct proc *p = myproc();
    return p->syscall_count;
}


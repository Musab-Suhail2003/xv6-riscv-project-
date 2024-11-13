#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "sha256.h"


#define PASSWORD "2460e780bb13fe306d83a66748365b7bd55315f5ac7a485b971ae9a62a84a9f2"
#define PASSWORD_LEN 32

int strcmp(const char *s1, const char *s2) {
    int i = PASSWORD_LEN;
    while (i > 0) {
        s1++;
        s2++;
        if(*s1 && (*s1 == *s2)) 
            break;
        i--;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}



void kernel_password_check(void) {
    BYTE input[] = "xv6pass";
    BYTE output[SHA256_BLOCK_SIZE];

    int attempts = 0;

    while (attempts < 3) {
        printf("Enter password: ");
        int start = ticks;
        SHA256_answer(input, output);
        int end = ticks;
        if (strcmp((char *)output, (char *)PASSWORD) == 0) {
            printf("\nAccess granted.\n");
            int diff = end - start;
            printf("\nCompleted with %d ticks\n", diff);
            return;  // Correct password, proceed to shell
        } else {
            printf("\nIncorrect password.\n");
            attempts++;
        }

    }

    // Failed login, halt the system
    printf("Too many failed attempts. System locked.\n");
    for (;;) {
        // Infinite loop to prevent further access
        asm volatile("wfi");
    }
}

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    kernel_password_check();
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode table
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
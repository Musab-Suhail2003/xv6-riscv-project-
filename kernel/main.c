#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "sha256.h"


#define PASSWORD "69e833a8d352c4bec68964a5d4c9a792e0fee180d9c10ce56e5bc8e83e8e8f4b"
#define PASSWORD_LEN 64

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
    BYTE input[] = "HELLO THIS IS MUSAB SUHAIL. TODAY WE WILL BE TESTING THE KERNEL IMPLEMENTAION FOR SHA256 IN XV6 RISCV. RECENTLY XV6 SWITCHED TO RISCV PRESUMABLY BECAUSE OF ITS FOS NATURE";
    BYTE pass[] = PASSWORD;
    BYTE output[SHA256_BLOCK_SIZE];

    int attempts = 0;

    while (attempts < 3) {
        printf("Enter password: ");
        uint64 start = r_time();
        SHA256_answer(input, output);

        if (strcmp((char *)output, (char *)pass) == 0) {
             for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
                  unsigned char byte = output[i];  // Access each byte

                  // Print the high nibble
                  printf("%x", byte >> 4);  // Shift right by 4 bits and print the upper nibble

                  // Print the low nibble
                  printf("%x", byte & 0xF);  // Mask with 0xF to get the lower nibble
            }
            uint64 end = r_time();

            printf("\nAccess granted.\n");
            int diff = (int)((end - start)/100);
            printf("\nCompleted with microseconds: %d\n", diff);
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
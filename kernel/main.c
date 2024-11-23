#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "sha256.h"

void kernel_password_check(void) {
    BYTE input[] = "HELLO THIS IS MUSAB SUHAIL. TODAY WE WILL BE TESTING THE KERNEL IMPLEMENTAION FOR SHA256 IN XV6 RISCV. RECENTLY XV6 SWITCHED TO RISCV PRESUMABLY BECAUSE OF ITS FOS NATURE HELLO THIS IS MUSAB SUHAIL. TODAY WE WILL BE TESTING THE KERNEL IMPLEMENTAION FOR SHA256 IN XV6 RISCV. MUSAB SUHAIL.";
    BYTE output[SHA256_BLOCK_SIZE];

    printf("Hashing buffer of size: %d\n", strlen((char *)input));

    uint64 start = r_time();
    SHA256_answer(input, strlen((char *)input), output);
    uint64 end = r_time();


    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
      unsigned char byte = output[i];  // Access each byte
                  // Print the high nibble
      printf("%x", byte >> 4);  // Shift right by 4 bits and print the upper nibble

                  // Print the low nibble
      printf("%x", byte & 0xF);  // Mask with 0xF to get the lower nibble
    }

    printf("\nhash completed.\n");
    int diff = (int)((end - start)/100);
    printf("Completed with microseconds: %d\n", diff);
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
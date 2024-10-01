#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/syscall.h"


int main(int argc, char *argv[]) {
  printf("Starting Syscall\n");

  // Run any command to count system calls
  int pid = fork();
  if (pid == 0) {
    exec(argv[1], argv);
  } else {
    wait(0);
    // After the command runs, retrieve the system call count
    int count = get_syscall();
    printf("System calls count: %d\n", count);
  }

  exit(0);
}


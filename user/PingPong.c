#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int p1[2], p2[2];
  char buf[1];
  
  pipe(p1);  // Create pipe 1 (parent -> child)
  pipe(p2);  // Create pipe 2 (child -> parent)

  if (fork() == 0) {
    // Child process
    read(p1[0], buf, 1);   // Read byte from parent
    printf("%d: received ping\n", getpid());
    write(p2[1], buf, 1);  // Send byte back to parent
    exit(0);
  } else {
    // Parent process
    write(p1[1], "A", 1);  // Send byte to child
    read(p2[0], buf, 1);   // Read byte back from child
    printf("%d: received pong\n", getpid());
    wait(0);               // Wait for child to finish
  }
  exit(0);
}


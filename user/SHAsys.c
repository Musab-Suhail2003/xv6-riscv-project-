#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/syscall.h"
#include "kernel/sha256.h"

int main(int argc, char argv[]) {
    // if (argc < 2) {
    //     printf("Usage: sha256 <string>\n");
    //     exit(1);
    // }

    char *hash[SHA256_BLOCK_SIZE];
    //syscall version of sha256
    sha256(argv[1], strlen(argv[1]), hash);

    printf("SHA-256 hash: ");

	int x = 0;
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        char byte = hash[i]; // Access each BYTE
        // Print each nibble of the BYTE
        printf("%x",byte);
		x = i;
    }

    printf("\n %d character lenght \n", x+1);

    exit(0);
}
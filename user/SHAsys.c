#include "kernel/types.h"
#include "user/user.h"

#include <time.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Enter string or filename after SHAsys to hash!\n");
        exit(1);
    }
    
    char *filename = argv[1];
    char *buffer[1024];  // Adjust the buffer size as needed
    int fd = open(filename, 0);
	int length = read(fd, *buffer, 1024);

	int start = uptime();

    if (fd < 0 && length < 0) {
        printf("\nHashing input String \n");
    	sha256((char **)argv[1]);
    }else{
		printf("\nHashing input file \n");
    	sha256(buffer);

	}
	int end = uptime();
	
	close(fd);


    int diff = end - start;
    printf("\nNumber of Clock interrupts: %d ticks\n", diff);

    exit(0);
}
#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Enter string or file after SHA256 to hash! \n");
        exit(1);
    }

	int start, diff;

    if (strcmp(argv[1], "-f") == 0) {
		char *filename = argv[2];
		char buffer[1024];  // Adjust the buffer size as needed
		int fd = open(filename, 0);
		if(fd < 0){
			printf("File not found\n");
			exit(1);
		}
		int length = read(fd, buffer, 1024);

		printf("\nHashing input file of length %d \n", length);
		close(fd);
        start = uptime();
        sha256(buffer);
        diff = uptime() - start;
    }else{
		char input[1024];  // Ensure the array is large enough
		for (int i = 1; i < argc; i++) {
        	strcat(input, argv[i]);
        if (i < argc - 1) strcat(input, " ");  // Add a space between arguments
    	}
		int len = strlen(input);

		printf("\nHashing input String %d\n", len);
        start = uptime();
    	sha256(input);
        diff = uptime() - start;

	}
    printf("\nNumber of Clock interrupts: %d ticks\n", diff);
	
    exit(0);
}
#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Enter string or file after SHA256 to hash! \n");
        exit(1);
    }

	int start;

    if (strcmp(argv[1], "-f") == 0) {
		char *filename = argv[2];
		char buffer[2300];  // Adjust the buffer size as needed
		int fd = open(filename, 0);
		if(fd < 0){
			printf("File not found\n");
			exit(1);
		}
		int length = read(fd, buffer, sizeof(buffer));

        if (length < 0) {
            printf("Error reading file\n");
            exit(1);
        }

		printf("\nHashing input file of length %d \n", length);

		close(fd);
		
        start =rtime();
        sha256(buffer, start);
    }else{
		char input[1024];  // Ensure the array is large enough
		gets(input, sizeof(input));
		int len = strlen(input);
        if(input[len-1]=='\n'){input[len-1] = '\0';}


		printf("Hashing input String %d\n", len);

        start = rtime();
    	sha256(input, start);

	}
    printf("\n");
	
	
    exit(0);
}
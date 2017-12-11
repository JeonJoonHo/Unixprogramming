#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFSIZE 512

int main(int argc, char *argv[]) {
	char buffer[BUFSIZE];
	int filedes;
	ssize_t nread;
	int i;
	long total = 0, word = 0, line = 0;

	if( (filedes = open(argv[1], O_RDONLY) ) == -1 ) {
		printf("error in opening");
		exit(1);
	}

	while((nread = read(filedes, buffer, BUFSIZE)) > 0) {
		i = 0;
		printf("nread : %ld\n", nread);
		while(i < nread) {
            if(!((buffer[i] >= 0x21 && buffer[i] <= 0x39) || (buffer[i] >= 0x41 && buffer[i] <= 0x5a) || (buffer[i] >= 0x61 && buffer[i] <= 0x7a))) {
                printf("word : %d\n", buffer[i]);
                word++;
                if(buffer[i] == '\n') line++;
            }
			i++;
		}				
		total += nread;
	}
	
	printf("Total : %ld\n", total);
    printf("Total word : %ld\n", word);
	printf("Total line : %ld\n", line);
	exit(0);
}

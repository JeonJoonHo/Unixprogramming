#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 512

main(int argc, char *argv[]) {
    int filedes, i;
    char *filename;
    char buf[BUFSIZE];
    ssize_t readSize;
    
    if(argc <= 1) {
        printf("How to Use : 2-14.out fileName\n");
        exit(2);
    }
    
    for(i = 1; i < argc; i++) {
        filename = argv[i];
        if((filedes = open(filename, O_RDONLY)) == -1) {
            printf("%s File Open Error!!\n", filename);
            exit(3);
        }
        printf("%s File...\n", filename);
        while((readSize = read(filedes, buf, BUFSIZE)) > 0) {
            if(write(1, buf, readSize) < readSize) {
                close(filedes);
                printf("Standard Output Write Error!!\n");
                exit(4);
            }
        }
        close(filedes);
        printf("\n");
    }
    
    exit(0);
}

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 512
#define PERM 0644

int copyfile(const int infile, const int outfile) {
    ssize_t nread;
    char buffer[BUFSIZE];
    
    while((nread = read(infile, buffer, BUFSIZE)) > 0) {
        if(write(outfile, buffer, nread) < nread)
            (-3);
    }
    
    if(nread == -1) return (-4);
    
    return 0;
}

main(int argc, char *argv[]) {
    int infile, outfile, copyresult;
    char *name1, *name2;
    if(argc != 3) {
        printf("How to Use : mycp origFilename copyFilename\n");
        exit(2);
    }
    
    name1 = argv[1];
    name2 = argv[2];
    
    if((infile = open(name1, O_RDONLY)) == -1) {
        printf("Orignal File Open Error!!\n");
        exit(3);
    }
    else {
        printf("Success File Open\n");
    }
    
    if((outfile = open(name2, O_WRONLY | O_CREAT | O_TRUNC, PERM)) == -1) {
        printf("Copy File Create Error!!\n");
        close(infile);
        exit(4);
    }
    else {
        printf("Success File Write\n");
    }
    
    copyresult = copyfile(infile, outfile);
    
    close(infile);
    close(outfile);
    
    switch(copyresult) {
        case 0:
            printf("Copy Success!!\n");
            break;
        case -3:
            printf("Error while Writing!!\n");
            exit(5);
        case -4:
            printf("Error while Last Read!!\n");
            exit(6);
    }
    
    exit(0);
}

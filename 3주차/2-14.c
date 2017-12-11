#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int fileopen(char *filename, char *option) {
    int filedes;
    
    if(!strcmp(option[0], 'r')) {
        if(!strcmp(option[1], 'w')) {
            filedes = open(filename, O_RDWR);
        }
        else {
            filedes = open(filename, O_RDONLY);
        }
    }
    else if(!strcmp(option[0], 'w')) {
        filedes = open(filename, O_WRONLY);
    }
    else if(!strcmp(option[0], 'a')) {
        filedes = open(filename, O_WRONLY | O_APPEND);
    }
    else {
        return -1;
    }
    
    if(filedes == -1) return -1;
    else return filedes;
}

main(int argc, char *argv[]) {
    char *name, *option;
    
    name = argv[1];
    option = argv[2];
    
    fileopen(name, option);
    
    exit(0);
}

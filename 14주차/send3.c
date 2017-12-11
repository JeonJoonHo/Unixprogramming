//
// Created by 전준호 on 2017. 11. 22..
//
//
// Created by 전준호 on 2017. 11. 22..
//
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#define FIFO "/tmp/fifo"
#define BUFSIZE 512
int fd;
void sigproc() {
    close(fd);
    unlink(FIFO);
    exit(0);
}

int main() {
    int fd;
    char buffer[BUFSIZE];
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = sigproc;
    act.sa_flags = SA_RESTART;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGTSTP, &act, NULL);

    if(-1 == mkfifo(FIFO, 0600))
        return -1;
    fd = open(FIFO, O_WRONLY);
    if(fd == -1)
        return -2;
    while(1) {
        printf("> ");
        if(fgets(buffer, BUFSIZE-1, stdin))
            break;
        write(fd, buffer, BUFSIZE);
    }
    sigproc();
    return 0;
}


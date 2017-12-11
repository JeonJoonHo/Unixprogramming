//
// Created by 전준호 on 2017. 11. 18..
//
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_CMD_ARG	10
#define MAX_CMD_GRP	10
#define MAX_DIR_LEN	200

const char *prompt = "myshell> ";
char* cmdgrps[MAX_CMD_GRP];
char* cmdvector[MAX_CMD_ARG];
char cmdline[BUFSIZ];
char dirPath[MAX_DIR_LEN];
char* cmdpipe[MAX_CMD_ARG][MAX_CMD_ARG];
int cmdpipenum[MAX_CMD_ARG];

void fatal(char*);
int makelist(char*, const char*, char**, int);
int makeargv(char*, const char*, char**, int);

void execute_cmdline(char *cmd);
void execute_cmdgrp(char **cmdgrp, int isRedirect, int count_v, int isBackgnd);
void execute_chdir(char * cmdvector);
void executr_exit();
void execute_cmd(char** cmd, int isBackground, int i, int count_v, int isRedirect);
void execute_pipe(char* cmd[MAX_CMD_ARG][MAX_CMD_ARG], int pipeNum[MAX_CMD_ARG], int isPipe, int isBackgnd);
void execute_redirect(char **, int isRed, int count_v, int isBackgnd);

void handle_sigchld(int sig);
void sigint_handler(int sig);

int main(int argc, char**argv){
    int i=0;
    pid_t pid;

    while (1) {

        fputs(prompt, stdout);

        signal (SIGINT, SIG_IGN);
        signal (SIGQUIT, SIG_IGN);

        fgets(cmdline, BUFSIZ, stdin);

        cmdline[ strlen(cmdline) -1] ='\0';

        execute_cmdline(cmdline);

    }
    return 0;
}

void handle_sigchld(int sig) {
    int saved_errno = errno;
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
    errno = saved_errno;
}

void sigint_handler(int sig) {
    kill(getppid(), kill);
    printf("\n");
}

void fatal(char *str){
    perror(str);

    exit(1);
}

int makelist(char *s, const char *delimiters, char** list, int MAX_LIST){
    int numtokens = 0;
    char *snew = NULL;

    if( (s==NULL) || (delimiters==NULL) ) return -1;

    snew = s + strspn(s, delimiters);	/* delimiters skip */
    if( (list[numtokens]=strtok(snew, delimiters)) == NULL )
        return numtokens;

    numtokens = 1;

    while(1){
        if( (list[numtokens]=strtok(NULL, delimiters)) == NULL)
            break;
        if(numtokens == (MAX_LIST-1)) return -1;
        numtokens++;
    }
    return numtokens;
}

int makeargv(char *s, const char *delimiters, char** list, int MAX_LIST){
    int numtokens = 0;
    char *snew = NULL;

    if( (s==NULL) || (delimiters==NULL) ) return -1;

    snew = s + strspn(s, delimiters);	/* delimiters skip */
    if( (list[numtokens]=strtok(snew, delimiters)) == NULL )
        return numtokens;

    numtokens = 1;

    while(1){
        if( (list[numtokens]=strtok(NULL, delimiters)) == NULL)
            break;
        if(numtokens == (MAX_LIST-1)) return -1;
        numtokens++;
    }
    return numtokens;
}

void execute_cmdline(char *cmdline){
    int count = 0;
    int count_v =0;
    int i = 0, j = 0;
    int isBackgnd = 0;
    int isPipe = 0;
    int pipeIndex[3];
    count = makelist(cmdline, ";", cmdgrps,MAX_CMD_GRP);

    for(i=0; i<count; ++i){
        count_v = makeargv(cmdgrps[i], " ", cmdvector, MAX_CMD_ARG);

        struct sigaction ss;
        ss.sa_handler = &sigint_handler;
        sigemptyset(&ss.sa_mask);
        ss.sa_flags = SA_RESTART;
        sigaction(SIGINT, &ss, NULL);

        for(j = 0; j < count_v; ++j) {
            if(!strcmp(cmdvector[j],"|")) {
                pipeIndex[isPipe] = j;
                isPipe++;
            }
        }
        pipeIndex[isPipe] = count_v;

        int k = 0;
        for(j = 0; j <= isPipe; ++j) {
            int index = 0;
            for(k; k < pipeIndex[j]; k++) {
                cmdpipe[j][index] = cmdvector[k];
                index++;
                if(!strcmp(cmdvector[k], ">") || !strcmp(cmdvector[k], "<"))
                    index--;
                else
                    cmdpipenum[j] = index;
            }
            k = pipeIndex[j] + 1;
        }

        // If the last string of cmdvector is "&", then replace it as '\0' and set isBacgnd bit
        if(isPipe == 0) {
            int isRedirect = 0;

            for(j = 0; j < count_v; j++) {
                if(!strcmp(cmdvector[j], ">"))
                    isRedirect = 1;
                else if(!strcmp(cmdvector[j], "<"))
                    isRedirect = 2;
            }
            // pipe 없이 커맨드
            execute_cmd(cmdvector, isBackgnd, i, count_v, isRedirect);
        }
        else {
            execute_pipe(cmdpipe, cmdpipenum, isPipe, isBackgnd);
        }
    }
}

void execute_cmd(char **cmd, int isBackgnd, int i, int count_v, int isRedirect) {
    if(!strcmp(cmdvector[count_v-1],"&")){
        cmdvector[count_v -1] = '\0';
        isBackgnd = 1;
    }
    // run Change Directiry if cmdvector[0] is "cd"
    if(!strcmp(cmdvector[0],"cd"))
    {
        execute_chdir(cmdvector[1]);
    }
        // Exit if cmdvector[0] is "exit"
    else if (!strcmp(cmdvector[0],"exit"))
    {
        executr_exit();
    }
    else {
        pid_t pid = fork();
        switch (pid) {
            case -1:
                fatal("fork error\n");
            case 0:
                execute_cmdgrp(cmdvector, isRedirect, count_v, isBackgnd);
            default :
                if (isBackgnd == 0) {
                    waitpid(pid, NULL, NULL);
                } else {
                    if(isRedirect != 0) {
                        printf("%d done ", getppid());
                        int j;
                        for (j = 0; j < count_v - 1; j++)
                            printf("%s ", cmdvector[j]);
                        printf("\n");
                    }

                    struct sigaction sa;
                    sa.sa_handler = &handle_sigchld;
                    sigemptyset(&sa.sa_mask);
                    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
                    if (sigaction(SIGCHLD, &sa, 0) == -1) {
                        perror(0);
                        exit(1);
                    }
                }
                fflush(stdout);
        }
    }
}

void execute_redirect(char** cmd, int isRed, int count_v, int isBackgnd) {
    char *arg;
    int fd;
    int i;

    if(isBackgnd == 0) {
        arg = cmd[count_v - 1];

        if (isRed == 1) { // >
            if ((fd = open(arg, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
                fatal("file open error");
            dup2(fd, STDOUT_FILENO);
            close(fd);
            cmd[count_v - 2] = '\0';
        } else if (isRed == 2) { // <
            if ((fd = open(arg, O_RDONLY | O_CREAT, 0644)) < 0)
                fatal("file open error");
            dup2(fd, STDIN_FILENO);
            close(fd);
            cmd[count_v - 2] = '\0';
        }
    }
    else if(isBackgnd == 1){
        if (isRed == 1) { // >
            arg = cmd[count_v - 2];
            if ((fd = open(arg, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
                fatal("file open error");
            dup2(fd, STDOUT_FILENO);
            close(fd);
            cmd[count_v - 3] = '\0';
        } else if (isRed == 2) { // <
            if ((fd = open(arg, O_RDONLY | O_CREAT, 0644)) < 0)
                fatal("file open error");
            dup2(fd, STDIN_FILENO);
            close(fd);
            cmd[count_v - 3] = '\0';
        }
    }
}

void execute_pipe(char* cmd[MAX_CMD_ARG][MAX_CMD_ARG], int pipeNum[MAX_CMD_ARG], int isPipe, int isBackgnd) {
    pid_t pid[isPipe+1];
    int i = 0,j;
    int p[isPipe+1][2], status;

    for(i = 0; i <= isPipe; i++) {
        if(pipe(p[i]) == -1)
            fatal("pipe call in join");
    }

    for(i = 0; i <= isPipe; i++) {
        switch(pid[i] = fork()){
            case -1:
                fatal("fork error");
            case 0:
                if(i == 0) {
                    dup2(p[i][1], STDOUT_FILENO);

                    for(j = 0; j < isPipe; j++) {
                        close(p[j][1]);
                        close(p[j][0]);
                    }
                }

                else if (i == isPipe) {
                    dup2(p[i - 1][0], STDIN_FILENO);
                    for(j = 0; j < isPipe; j++) {
                        close(p[j][0]);
                        close(p[j][1]);
                    }
                }

                else {
                    dup2(p[i - 1][0], STDIN_FILENO);
                    dup2(p[i][1], STDOUT_FILENO);

                    for(j = 0; j < isPipe; j++) {
                        close(p[j][0]);
                        close(p[j][1]);
                    }
                }
                int count_v = 0;
                int isRedirect = 0;

                for(j = 0; cmd[i][j] != NULL; j++) {
                    count_v++;
                    if(!strcmp(cmd[i][j], ">"))
                        isRedirect = 1;
                    else if(!strcmp(cmd[i][j], "<"))
                        isRedirect = 2;
                }

                if(isRedirect == 0)
                    execute_cmdgrp(cmd[i], 0, pipeNum[i], 0);
                else
                    execute_cmdgrp(cmd[i], isRedirect, pipeNum[i], 0);
        }
    }

    for(j = 0; j < isPipe; j++) {
        close(p[j][0]);
        close(p[j][1]);
    }

    if (isBackgnd == 1) {

        for(j = 0; j <= isPipe; j++) {
            if (pid[j] > 0)
                printf("[Process ID %d]\n", pid[j]);
            else
                sleep(1);
        }
        return;
    }

    while(waitpid(pid[isPipe], &status, WNOHANG)==0)
        sleep(1);
}

void execute_cmdgrp(char **cmdgrp, int isRedirect, int count_v, int isBackgnd){
    int i=0;
    int count = 0;

    if(isRedirect != 0)
        execute_redirect(cmdgrp, isRedirect, count_v, isBackgnd);

    execvp(cmdgrp[0], cmdgrp);

    fatal("exec error\n");
}

// Handles ChangeDirectory Command
void execute_chdir(char * cmdvector){
    chdir(cmdvector);
    printf("%s\n",getcwd(dirPath,MAX_DIR_LEN));
    return;
}

//Handles Exit command
void executr_exit(){
    exit(0);
}

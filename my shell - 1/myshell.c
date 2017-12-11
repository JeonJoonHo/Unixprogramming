//
// Created by 전준호 on 2017. 11. 4..
//
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_CMD_ARG 10

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];

void fatal(char *str){
    perror(str);
    exit(1);
}

int makelist(char *s, const char *delimiters, char** list, int MAX_LIST){
    int i = 0;
    int numtokens = 0;
    char *snew = NULL;

    if( (s==NULL) || (delimiters==NULL) ) return -1;

    snew = s + strspn(s, delimiters);	/* delimiters를 skip */
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

int main(int argc, char**argv){
    int i=0;
    pid_t pid;
    int status;
    int arg_number;
    int is_background;
    char cwd[1024];

    while (1) {
        fputs(prompt, stdout);
        fgets(cmdline, BUFSIZ, stdin);
        cmdline[ strlen(cmdline) -1] ='\0';

        arg_number = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

        is_background = (strcmp(cmdvector[arg_number-1] , "&") == 0 ? 1 : 0);

        if(strcmp(cmdvector[0], "cd") == 0) {
            chdir(cmdvector[1]);
            continue;
        } else if (strcmp(cmdvector[0], "pwd") == 0) {
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
            continue;
        } else if (strcmp(cmdvector[0], "exit") == 0){
            break;
        }

        switch(pid=fork()){
            case 0:
                if(is_background == 1)
                    cmdvector[arg_number-1] = NULL;
                execvp(cmdvector[0], cmdvector);
                fatal("main()");
            case -1:
                fatal("main()");
            default:
                if(is_background == 0)
                    waitpid(pid, &status, 0);
                else
                    sleep(1);
        }
    }
    return 0;
}

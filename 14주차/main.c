#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_CMD_ARG	10
#define MAX_CMD_GRP	10
#define MAX_DIR_LEN	200

const char *prompt = "myshell> ";
char* cmdgrps[MAX_CMD_GRP];
char* cmdvector[MAX_CMD_ARG];
char cmdline[BUFSIZ];
char dirPath[MAX_DIR_LEN];

void fatal(char*);
int makelist(char*, const char*, char**, int);
int makeargv(char*, const char*, char**, int);

void execute_cmdline(char *cmd);
void execute_cmdgrp(char *cmdgrp);
void execute_chdir(char * cmdvector);
void executr_exit();
void catchSignal();

int main(int argc, char**argv){
    int i=0;
    pid_t pid;

    while (1) {

        fputs(prompt, stdout);
        fgets(cmdline, BUFSIZ, stdin);

        cmdline[ strlen(cmdline) -1] ='\0';

        execute_cmdline(cmdline);

    }
    return 0;
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
    int i = 0;
    int isBackgnd = 0;
    count = makelist(cmdline, ";", cmdgrps,MAX_CMD_GRP);

    for(i=0; i<count; ++i){
        count_v = makeargv(cmdgrps[i], " ", cmdvector, MAX_CMD_ARG);

        // If the last string of cmdvector is "&", then replace it as '\0' and set isBacgnd bit
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
        else{
            pid_t pid = fork();
            switch(pid){
                case -1:	fatal("fork error\n");
                case 0:	execute_cmdgrp(cmdgrps[i]);
                default :
                    if (isBackgnd == 0)
                        waitpid(pid,NULL,NULL);
                    fflush(stdout);
            }
        }
    }
}

void execute_cmdgrp(char *cmdgrp){
    int i=0;
    int count = 0;

    execvp(cmdvector[0], cmdvector);

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
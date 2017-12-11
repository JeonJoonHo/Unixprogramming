#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char** argv) {
    char instruction[15], filename[15];
    char cwd[1024];
    
    system("clear");
    
    while(1) {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            
            printf("%s JeonJH Shell  $ ", cwd);
        
        scanf("%s", instruction);
        
        if(strcmp(instruction, "exit") == 0) {
            printf("쉘이 종료됩니다. \n");
            exit(0);
        }
        else if(strcmp(instruction, "ls") == 0) {
            DIR           *d;
            struct dirent *dir;
            d = opendir(".");
            if (d)
            {
                while ((dir = readdir(d)) != NULL)
                {
                    printf("%s\n", dir->d_name);
                }
                
                closedir(d);
            }
            continue;
        }
        
        scanf("%s", filename);
        
        if(strcmp(instruction, "touch") == 0) {
            open(filename, O_WRONLY | O_CREAT);
        }
        else if(strcmp(instruction, "mkdir") == 0) {
            mkdir(filename, 0700);
        }
        else if(strcmp(instruction, "rmdir") == 0) {
            rmdir(filename);
        }
        else if(strcmp(instruction, "rm") == 0) {
            unlink( filename );
        }
    }
}

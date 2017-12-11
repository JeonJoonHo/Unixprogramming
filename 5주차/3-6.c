#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    char *filename = "junk";
    if (access(filename, R_OK) == -1)
        perror("파일 읽기 가능\n");
    else
        printf("파읽 읽기 불가능 %s\n", filename);
    
    if (access(filename, W_OK) == -1)
        perror("파일 쓰기 가능\n");
    else
        printf("파일 쓰기 불가능%s\n", filename);
    
    if (access(filename, X_OK) == -1)
        perror("파일 실행 가능\n");
    else
        printf("파일 실행 불가능%s\n", filename);
    
    return 0;
}

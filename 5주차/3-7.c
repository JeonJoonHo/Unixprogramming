#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int lsoct(char* perm)
{
    int i, j, k,oct=0;
    if(perm[0] == 'r') oct += 400;
    if(perm[1] == 'w') oct += 200;
    if(perm[2] == 'x') oct += 100;
    if(perm[3] == 'r') oct += 40;
    if(perm[4] == 'w') oct += 20;
    if(perm[5] == 'x') oct += 10;
    if(perm[6] == 'r') oct += 4;
    if(perm[7] == 'w') oct += 2;
    if(perm[8] == 'x') oct += 1;
    i = oct % 10;
    oct /= 10;
    j = oct % 10;
    oct /= 10;
    k = oct % 10;
    return(k*64 + j*8 + i);
}

int main(int argc, char* argv[])
{
    mode_t i, j, k, temp, perm;
    
    if( open(argv[1], O_RDONLY) == -1) {
        perror("파일 실행 불가능");
        exit(1);
    }
    
    if( argv[2][0] >= '0' && argv[2][0] <'9' ) {
        temp = atoi(argv[2]);
        i = temp % 10;
        temp /= 10;
        j = temp % 10;
        temp /= 10;
        k = temp % 10;
        perm = k*64 + j*8 + i;
    }
    else
        perm = lsoct(argv[2]);
    
    if( chmod(argv[1], perm ) == -1) {
        perror("chmod 실행 실패");
        exit(1);
    }
    else {
        perror("chmod 실행");
        exit(1);
    }
}

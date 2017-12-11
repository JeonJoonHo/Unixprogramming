#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
    int i;
    
    if( access( argv[1], W_OK ) == -1 ) {
        printf("파일을 삭제하시겠습니까? yes -> 1");
        scanf("%d", &i);
        if( i != 1 ) exit(0);
    }
    
    if( unlink( argv[1] ) == -1 ) {
        perror("unlink 실패");
        exit(1);
    }
    else {
        printf("%s unlink success \n", argv[1]);
    }
    
    printf("Why check write permission about file before calling unlink system call : unlink는 이들 허가에 의해 영향을 받지 않기 때문. ");
    exit(0);
}

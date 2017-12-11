#include <stdio.h>

int oct_result = 0;

char* result = "";

int lsoct(char* perm)
{
    int i, j, k, oct=0;
    if(perm[0] == 'r') oct += 400;
    if(perm[1] == 'w') oct += 200;
    if(perm[2] == 'x') oct += 100;
    if(perm[3] == 'r') oct += 40;
    if(perm[4] == 'w') oct += 20;
    if(perm[5] == 'x') oct += 10;
    if(perm[6] == 'r') oct += 4;
    if(perm[7] == 'w') oct += 2;
    if(perm[8] == 'x') oct += 1;
    
    return(oct);
}

char* octls(int oct)
{
    char perm[11]="";
    int i=6, remainder;
    while(i >= 0){
        remainder = oct % 10;
        oct /= 10;
        switch(remainder){
            case 0: perm[i] = perm[i+1] = perm[i+2] = '-'; break;
            case 1: perm[i+2] = 'x';
                perm[i+1] = perm[i] = '-'; break;
            case 2: perm[i] = perm[i+2] = '-';
                perm[i+1] = 'w';
                break;
            case 3: perm[i+2] = 'x';
                perm[i+1] = 'w';
                perm[i] = '-';
                break;
            case 4: perm[i] = 'r';
                perm[i+1] = perm[i+2] = '-';
                break;
            case 5: perm[i] = 'r';
                perm[i+1] = '-';
                perm[i+2] = 'x';
                break;
            case 6: perm[i] = 'r';
                perm[i+1] = 'w';
                perm[i+2] = '-';
                break;
            case 7: perm[i] = 'r';
                perm[i+1] = 'w';
                perm[i+2] = 'x';
                break;
            default : perror("wrong oct");
        }
        i -= 3;
    }
    printf("Octal permission : %s \n", perm);
    return(perm);
}

int main(int argc, char* argv[]) {
    char* permission = argv[1];
    
    printf("Original permission : %s \n", permission);
    
    if(argv[1][0] >= '0' && argv[1][0] < '9') {
        int number = atoi(permission);
        char* permission_octls = octls(number);
    }
    else {
        int permission_lsoct = lsoct(permission);
        
        printf("Decimal permission : %d \n", permission_lsoct);
    }
}

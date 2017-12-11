#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MFILE 10
#define BUFSIZE 512

int cmp(const char *, time_t);
struct stat sb;

int main(int argc, char **argv){
    
    int j;
    
    time_t last_time;
    
    for(j=1;j<1;j++){
        if(stat(argv[j], &sb) == -1){
            fprintf(stderr, "lookout: couldn't stat %s\n", argv[j]);
            exit(1);
            
        }
        
        last_time = sb.st_mtime;
        
    }
    
    int check = 0;
    
    while(1){
        
        check = cmp(argv[1], last_time);
        if(check == 1) {
            last_time = sb.st_mtime;
        }
        
        sleep(10);
    }
    
}

int cmp(const char *name, time_t last){
    int result = 0;
    char buf[BUFSIZE];
    int fd;
    ssize_t nread;

    if(stat(name, &sb) ==- -1 || sb.st_mtime != last){
        if( (fd = open(name, O_RDONLY)) == -1 ) {
            printf("%s fail to read\n", name);
            exit(0);
        }
        else
            fprintf(stderr, "lookout: %s changed\n", name);
        
        while( (nread = read(fd, buf, BUFSIZ)) > 0)
            write(1, buf, nread);
        
        result = 1;
        return result;
    }
    
    return result;
}



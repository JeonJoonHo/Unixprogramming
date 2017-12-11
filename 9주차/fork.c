#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv){
	pid_t pid;
	printf("Calling fork()\n");
	pid = fork();
	if(pid > 0)
		printf("Parent PIDs : %d, %d, %d\n", pid, getpid(), getppid());
	else if(pid == 0)
		printf("Child PIDs : %d, %d, %d\n", pid, getpid(), getppid());
	else 
		printf("fork() error.\n");
	return 0;
}

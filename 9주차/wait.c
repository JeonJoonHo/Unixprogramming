#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
int main() {
	pid_t pid;
	pid = fork();
	if(pid == 0) {
		sleep(10);
	}
	else {
		printf("Childe PID : %d\n", pid);
		pid = wait(NULL);
		printf("Child Waited : %d\n", pid);
	}
	return 0;
}


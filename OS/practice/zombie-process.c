#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int pid = fork();

	if (pid == 0) {
		printf("Child process PID: %d\n", getpid());
		exit(0);
	} else {
		printf("Parent process PID: %d\n", getpid());
		sleep(10);
	}
	return 0;
}


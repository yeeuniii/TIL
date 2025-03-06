#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int pid = fork();

	if (pid == 0) {
		char* commands[2] = {"/bin/sh", 0};

		printf("Child process PID: %d\n", getpid());
		execve(commands[0], commands, NULL);
	} else {
		printf("Parent process PID: %d\n", getpid());
	}
	return 0;
}


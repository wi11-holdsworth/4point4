#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char** argv) {
	// initialilse pipe structure
	int fd[2];
	pipe(fd);

	// fork to create child process
	int pid = fork();

	if (pid == 0) {
		// child execs wc -l and prints output with pipe in as input
		close(fd[1]); // close write

		// read from pipe to standard input
		dup2(fd[0], STDIN_FILENO);

		// close read
		close(fd[0]);

		// exec wc with stdin
		char *args[] = {"wc", "-l", NULL};
		execvp("wc", args);
	} else {
		// parent execs ls *.c and pipes output to child
		close(fd[0]); // close read
		
		// write to standard output
		dup2(fd[1], STDOUT_FILENO);

		// close write
		close(fd[0]);

		char *args[] = {"sh", "-c", "ls *.c", NULL};
		execvp("sh", args);
	}

	return EXIT_SUCCESS;
}

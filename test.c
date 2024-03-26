#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glob.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
	// initialilse pipe structure
	int fd[2];
	pipe(fd);

	// fork to create child process
	int pid = fork();

	if (pid > 0) {
		// parent execs wc -l and prints output with pipe in as input
		close(fd[1]); // close write 

		// read from pipe to standard input
		dup2(fd[0], STDIN_FILENO);

		// close read
		close(fd[0]);

		// exec wc with stdin
		char *args[] = {"wc", "-l", NULL};
		execvp("wc", args);
	} else {
		// child execs ls *.c and pipes output to parent 
		close(fd[0]); // close read
		
		// write to standard output
		dup2(fd[1], STDOUT_FILENO);

		// close write
		close(fd[1]);
	
		// initialise glob buffer
		glob_t globbuf;

		// number of arguments
		globbuf.gl_offs = 1;

		glob("*.c", GLOB_DOOFFS, NULL, &globbuf);
		globbuf.gl_pathv[0] = "ls";
		execvp("ls", &globbuf.gl_pathv[0]);

		int status;
		waitpid(pid, &status, 0);
		printf("\n");
	}

	return EXIT_SUCCESS;
}

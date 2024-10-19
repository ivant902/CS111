#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int pipe2(int argc, char *argv[])
{
	
	int index = 1;
	int fd [2];
	int error = 0;
	//handles two or more arguments
	for (; index < argc-1; index++)
	{
		error = pipe(fd);
		if (error == -1)
		{
			perror("pipe failed");
			exit(errno);
		}
		int rc = fork();
		if (rc < 0)
		{
			// error for failed fork
			perror("fork failed");
			exit(errno);
		}
		else if (rc == 0)
		{
			// child process
			int error1 = 0;
			close(fd[0]); 
			error1 = dup2(fd[1], 1); // changes stdout to write end of pipe
			if (error1 == -1)
			{
				perror("dup2 failed");
				exit(1);
			}
			close(fd[1]);
			execlp(argv[index], argv[index], NULL);
		}
		else
		{
			// parent process
			int error2 = 0;
			close(fd[1]); 
			dup2(fd[0], 0); // changes stdin to read in pipe
			if (error2 == -1)
			{
				perror("dup2 failed");
				exit(errno);
			}
			close(fd[0]);
			wait(NULL);
		}
	}
	// handles one argument
	execlp(argv[index], argv[index], NULL);
	return 0;
}

int main(int argc, char *argv[])
{
	// handles no arguments
	if (argc == 1)
	{
		perror("No arguments. Input one or more arguemnts.");
	}
	else
	{
		pipe2(argc, argv);
	}
	return 0;

}

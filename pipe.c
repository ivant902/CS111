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
				exit(errno);
			}
			close(fd[1]);
			error = execlp(argv[index], argv[index], NULL);
			if (error == -1)
			{
				perror("child execlp failed");
				exit(errno);
			}
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
	error = execlp(argv[index], argv[index], NULL);
	if (error == -1)
	{
		perror("outside execlp failed");
		exit(errno);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	// handles no arguments
	if (argc == 1)
	{
		errno = EINVAL;
		perror("No arguments. Input one or more arguemnts.");
		exit(errno);
	}
	else
	{
		pipe2(argc, argv);
	}
	return 0;

}

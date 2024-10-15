#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int pipe2(int argc, char *argv[])
{
	
	int index = 1;
	int fd [2];
	int error = 0;
	for (; index < argc-1; index++)
	{
		error = pipe(fd);
		if (error == -1)
		{
			perror("pipe failed");
		}
		int rc = fork();
		if (rc < 0)
		{
			// error for failed fork
			perror("fork failed");
			exit(1);
		}
		else if (rc == 0)
		{
			// child process
			int error1 = 0;
			close(fd[0]); 
			error1 = dup2(fd[1], 1); // changes stdout to write end of pipe
			if (error1 == -1)
			{
				perror("execlp failed");
				return 0;
			}
			close(fd[1]);
			execlp(argv[index], argv[index], NULL);
		}
		else
		{
			// parent process
			int error2 = 0;
			close(fd[1]); 
			error2 = dup2(fd[0], 0); // changes stdin to read in pipe
			close(fd[0]);
			wait(NULL);
			if (error2 == -1)
			{
				perror("execlp failed");
				return 0;
			}
			// execlp(argv[index+1], argv[index+1], NULL);
		}
	}
	execlp(argv[index], argv[index], NULL);
	return 0;
}

int main(int argc, char *argv[])
{
	// TODO: it's all yours
	if (argv[1] == NULL)
	{
		perror("No arguments. Input one or more arguemnts.");
	}
	else
	{
		pipe2(argc, argv);
	}
	return 0;

}

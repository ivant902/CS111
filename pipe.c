#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	// TODO: it's all yours
	int index = 1;
	int fd [2];
	int error = 0;
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
		error1 = dup2(fd[1], 1);
		if (error1 == -1)
		{
			perror("execlp failed");
			return 0;
		}
		execlp(argv[index], argv[index], NULL);
	}
	else
	{
		// parent process
		int error2 = 0;
		close(fd[1]); 
		error2 = dup2(fd[0], 0);
		if (error2 == -1)
		{
			perror("execlp failed");
			return 0;
		}
		execlp(argv[index+1], argv[index+1], NULL);
	}
	return 0;

}

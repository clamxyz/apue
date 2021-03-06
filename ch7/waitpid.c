#include "../public.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	
	if ((pid = fork()) < 0)
	{
		perror("fork error.");
		exit(0);
	}
	else if (pid == 0)
	{
		if ((pid = fork()) < 0)
		{
			perror("fork error.");
			exit(0);
		}
		else if (pid > 0)
		{
			exit(0);
		}
		sleep(2);
		printf("second child, parent pid = %d\n", getppid());
		exit(0);
	}
	if (waitpid(pid, NULL, 0) != pid)
	{
		perror("waitpid");
	}

	return 0;
}


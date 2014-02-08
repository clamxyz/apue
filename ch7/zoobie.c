#include "../public.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	char buf[1024] = {0};

	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		return 0;
	}
	else if (pid ==0)
	{
		printf("child process, pid = %d\n", getpid());
		exit(0);
	}

	sprintf(buf, "ps S -p %d", pid);
	if (system(buf) < 0)
	{
		perror("system");
	}
	waitpid(pid, NULL, 0);	

	return 0;
}


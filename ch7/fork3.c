#include "../public.h"

#include <stdio.h>
#include <unistd.h>

int glob = 6;
char buf[] = "write to stdout";

int main(int argc, char *argv[])
{
	int var = 7;
	pid_t pid = -1;

	printf("parent pid = %d\n", getpid());
	printf("before fork\n");
	if ((pid = vfork()) < 0)
	{
		printf("fork error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	else if (pid == 0)
	{
		glob++;
		var++;
		exit(0);
	}	

	printf("pid = %d,glob = %d, var = %d\n", getpid(), glob, var);

	return 0;
}


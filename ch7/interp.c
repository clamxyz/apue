#include "../public.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	pid_t pid;

	if ((pid = fork()) < 0)
	{
		perror("fork error");
		return -1;
	}	
	else if (pid == 0)
	{
		execlp("/home/study/APUE/ch7/testinterp", "testinterp", "arg1", "arg2", (char *)0);
		perror("execlp error");
	}
	if (waitpid(pid, NULL, 0) != pid)
	{
		perror("waitpid error.");
	}

	return 0;
}


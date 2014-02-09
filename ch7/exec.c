#include "../public.h"
#include <sys/wait.h>

char *env_init[] = {"USER=unknown", "TMPDIR=/tmp", "HOME=/tmp", (char *)0};

int main(int argc, char *argv[])
{
	pid_t pid;
	
	pid = fork();
	if (pid < 0)
	{
		perror("fork error.");
		return -1;
	}
	else if (pid == 0)
	{
		execle("/home/study/APUE/ch7/echoall", "echoall", "arg1", "arg2", (char *)0, env_init);
		perror("execlp /home/study/APUE/ch7/echoall failed.");
	}
	if ((waitpid(pid, NULL, 0) == pid) < 0)
	{
		printf("waitpid error.\n");	
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork error 2.");
		return -2;
	}
	if (pid == 0)
	{
		execl("echoall", "echoall", "arg", (char *)0);
		perror("execl echoall failed");
	}	
	if ((waitpid(pid, NULL, 0) == pid) < 0)
	{
		perror("waitpid error.");
	}
	return 0;
}


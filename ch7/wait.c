#include "../public.h"
#include <sys/wait.h>

static void pr_exit(int status);

int main(int argc, char *argv[])
{
	pid_t pid;
	int status;
	if ( (pid = fork()) < 0)
	{
		printf("fork error[%d][%s].\n", errno, strerror(errno));
		exit(-1);
	}
	else if (pid == 0) exit(7);
	if (wait(&status) != pid)
	{perror("wait error");exit(-1);}
	pr_exit(status);

	if ((pid = fork()) < 0){
		perror("fork error"); exit(-1);	
	}
	else if (pid == 0) abort();
	if (wait(&status) != pid)
	{
		perror("wait error."); exit(-1);
	}
	pr_exit(status);

	if ((pid = fork()) < 0)
	{
		perror("fork error.");
		exit(-1);
	}
	else if (pid == 0) status /= 0;
	if (wait(&status) != pid)
	{
		perror("fork error.");
		exit(-1);
	}
	pr_exit(status);

	exit(0);
}

static void pr_exit(int status)
{
	if (WIFEXITED(status))
	{
		printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d%s\n",
					WTERMSIG(status),
	#ifdef WCOREDUMP
			WCOREDUMP(status) ? " (core file generated) " : " ");
#endif
	else if (WIFSTOPPED(status))
		printf("child stopped, signal number = %d\n", WSTOPSIG(status));
}


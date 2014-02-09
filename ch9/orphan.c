#include "../public.h"
#include <signal.h>

static void pr_ids(char *pszStr);
static void sig_hup(int);

int main(int argc, char *argv[])
{
	char c;
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		exit(-1);
	}
	else if (pid == 0)
	{
		pr_ids("child");
		signal(SIGHUP, sig_hup);
		kill(getpid(), SIGTSTP);
		pr_ids("child");
		if (read(STDIN_FILENO, &c, 1) != 1)
		{
			printf("read error[%d][%s].\n", errno, strerror(errno));
		}
	}
	else 
	{
		pr_ids("parent");
		sleep(5);
	}

	return 0;	
}

static void pr_ids(char *pszStr)
{
	printf("%s:parent pid = %d, child pid = %d, group pid = %d.\n", pszStr, getppid(), getpid(), getpgrp());
	fflush(stdout);
}

static void sig_hup(int signo)
{
	printf("recive SIGHUP signal.signo = %d\n", signo);
}


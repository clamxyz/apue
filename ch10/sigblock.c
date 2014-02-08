#include "../public.h"

#include <signal.h>

static void sig_quit(int signo);

int main(int argc, char *argv[])
{
	sigset_t pendmask, oldmask, newmask;

	if (signal(SIGQUIT, sig_quit) == SIG_ERR)
	{
		printf("install signal SIGQUIT to sig_quit failed.\n");
		exit(-1);
	}
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
	{
		perror("sigprocmask failed");
		exit(-1);
	}
	sleep(5);
	if (sigpending(&pendmask) < 0)
	{
		perror("sigpending failed");
		exit(-1);
	}
	if (sigismember(&pendmask, SIGQUIT))
	{
		printf("pending SIGQUIT\n");
	}
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
	{
		perror("sigprocmask failed");
		exit(-1);
	}
	printf("unblock SIGQUIT\n");
	sleep(5);
	return 0;
}

static void sig_quit(int signo)
{
	printf("caughted signal SIGQUIT\n");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
	{
		printf("install signal SIGQUIT TO SIG_DFL failed.\n");
	}
}


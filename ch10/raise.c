#include "../public.h"
#include <signal.h>
#include <errno.h>

static void sig_intr(int signo);

static int l_raise(int signo);

int main(int argc, char *argv[])
{
	printf("Start Main:\n");
	if (signal(SIGINT, sig_intr) == SIG_ERR)
	{
		perror("signal SIGINT failed");
		exit(-1);
	}
	l_raise(SIGINT);
	printf("End Main:\n");
	
	return 0;
}

static int l_raise(int signo)
{
	return kill(getpid(), signo);
}

static void sig_intr(int signo)
{
	printf("caughted sig_intr\n");
}

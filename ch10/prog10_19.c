#include "../public.h"
#include <signal.h>
#include <sys/wait.h>

static void sig_intr(int signo);
static void sig_quit(int signo);
static void sig_child(int signo);

int main(int argc, char *argv[])
{
	if (signal(SIGINT, sig_intr) == SIG_ERR)
	{
		perror("signal");
		return -1;
	}
	if (signal(SIGQUIT, sig_quit) == SIG_ERR)
	{
		perror("signal SIGQUIT");
		return -1;
	}
	if (signal(SIGCHLD, sig_child) == SIG_ERR)
	{
		perror("signal SIGICHLD");
		return -1;
	}
	printf("parent pid = %d\n", getpid());
	system("/bin/ed");

	return 0;
}

static void sig_intr(int signo)
{
	printf("in sig_intr:\n");
}

static void sig_child(int signo)
{
	int status;

	wait(&status);

	printf("in sig_child:pid = %d\n", getpid());
}

static void sig_quit(int signo)
{
	printf("in sig_quit:\n");
}


#include "../public.h"

#include <signal.h>

typedef void (*signal_handler)(int);

signal_handler signal_intr(int sig_no, signal_handler sig_handler)
{
	struct sigaction oldact;
	struct sigaction newact;
	int status;

	newact.sa_flags = 0;
	newact.sa_flags = SA_INTERRUPT;
	sigemptyset(&newact.sa_mask);
	newact.sa_handler = sig_handler;
	status = sigaction(sig_no, &newact, &oldact);
	
	if (status == -1)
	{
		return SIG_ERR;
	}
	return oldact.sa_handler;
}

static void sig_alarm(int signo)
{
	printf("caughted alarm signal\n");
	alarm(3);
}

int main(int argc, char *argv[])
{
	if (signal_intr(SIGALRM, sig_alarm) == SIG_ERR)
	{
		perror("signal_intr failed");
		exit(-1);
	}	
	alarm(3);
	while(1)
	{
	}
	return 0;
}


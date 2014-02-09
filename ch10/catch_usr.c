#include "../public.h"

#include <signal.h>

static void sig_usr(int signo);

int main(int argc, char *argv[])
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
	{
		perror("signal error");
		exit(-1);
	}
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
	{
		perror("signal error");
		exit(-1);
	}
	
	for( ; ; )
		pause();	

	return 0;
}

static void sig_usr(int signo)
{
	if (signo == SIGUSR1)
		printf("catch signal SIGUSR1\n");
	if (signo == SIGUSR2)
		printf("catch signal SIGUSR2\n");
}


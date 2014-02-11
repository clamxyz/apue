#include "../public.h"

#include <signal.h>

static void sig_usr(int signo)
{
	if (signo == SIGUSR1)
	{
		printf("caughted SIGUSR1\n");
	}
	if (signo == SIGUSR2)
	{
		printf("CAUGHTED SIGUSR2\n");
	}

}

int main(int argc, char *argv[])
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
	{
		perror("signal SIGUSR1 failed");
		abort();
	}
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
	{
		perror("signal SIGUSR2 failed");
		abort();
	}	

	for( ; ; )
		pause();

	return 0;
}


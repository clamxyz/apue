#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void sig_alrm(int signo)
{
	printf("catched sigalrm\n");
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_flags = 0;
	act.sa_handler = sig_alrm;
	sigemptyset(&act.sa_mask);
	sigaction(SIGALRM, &act, NULL);
	printf("Begin Main:\n");
	alarm(3);
	pause();
	printf("Sleep 3s:\n");
	alarm(5);
	pause();
	printf("End Main:\n");

	return 0;
}


#include "timer.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler1(int signo)
{
	printf("in handler 1\n");
	fflush(NULL);
}
void handler2(int signo)
{
	printf("in handler2\n");
	fflush(NULL);
}

int main(int argc, char *argv[])
{
	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask, SIGALRM);
	sigprocmask(SIG_BLOCK, &mask, NULL);
	timer_init();
	timer_declare(1000000 * 1, handler1);
	timer_declare(1000000 * 2, handler2);
	pause();
	pause();
	return 0;
}


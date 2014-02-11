#include "../public.h"
#include <sys/time.h>
#include <signal.h>
/*
 *未解决alarm和其他时间函数交互的问题 
 */
static void sig_alrm(int signo);

static int l_usleep(int micro_secs);

int main(int argc, char *argv[])
{
	int timer = -1;
	printf("Start Main:\n");
	timer = l_usleep(1000 * 3);
	printf("timer remaining %d usec.\n", timer);
	printf("End Main:\n");
	return 0;
}

static void sig_alrm(int signo)
{
	return;
}

static int l_usleep(int micro_secs)
{
	struct itimerval newval, oldval, leftval;
	struct sigaction newact, oldact;
	sigset_t newmask, oldmask, suspendmask;
	
	newval.it_interval.tv_sec = 0;
	newval.it_interval.tv_usec = 0;
	newval.it_value.tv_sec = micro_secs / 1000;
	newval.it_value.tv_usec = micro_secs % 1000;
	
	newact.sa_handler = sig_alrm;
	newact.sa_flags = 0;
	sigemptyset(&newact.sa_mask);
	sigaction(SIGALRM, &newact, &oldact);

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	setitimer(ITIMER_REAL, &newval, &oldval);

	suspendmask = oldmask;
	sigdelset(&suspendmask, SIGALRM);
	sigsuspend(&suspendmask);

	newval.it_interval.tv_sec = 0;
	newval.it_interval.tv_usec = 0;
	getitimer(ITIMER_REAL, &leftval);
	
	sigaction(SIGALRM, &oldact, NULL);
	sigprocmask(SIG_SETMASK, &oldmask, NULL);

	return leftval.it_interval.tv_sec * 1000 + leftval.it_interval.tv_usec;
}


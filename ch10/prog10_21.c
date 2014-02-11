#include "../public.h"
#include <signal.h>

static void sig_alrm(int signo);
static int l_sleep(int nsecs);

int main(int argc, char *argv[])
{
	printf("Main Started:\n");
	l_sleep(5);
	printf("Main End:\n");
	return 0;
}

static void sig_alrm(int signo)
{
	return;
}

static int l_sleep(int nsecs)
{
	struct sigaction oldact, newact;
	sigset_t oldmask, newmask;
	int unslept = -1;	
	
	newact.sa_flags = 0;
	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	sigaction(SIGALRM, &newact, &oldact);
	
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask); 

	alarm(nsecs);
	newmask = oldmask;
	sigdelset(&newmask, SIGALRM);
	
	sigsuspend(&newmask);
	
	unslept = alarm(0);
	sigaction(SIGALRM, &oldact, NULL);
	sigprocmask(SIG_SETMASK, &oldmask, NULL);

	return unslept;
}



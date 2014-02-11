/**
 * abort的POSIX.1实现
 *
 */
#include "../public.h"
#include <signal.h>

#ifdef HANDLER
static void sig_abrt(int signo)
{
	printf("in sig_abrt\n");
}
#endif

static void l_abort(void)
{
	struct sigaction action;
	sigset_t mask;

	sigaction(SIGABRT, NULL, &action);
	if (action.sa_handler == SIG_IGN)
	{
		action.sa_handler = SIG_DFL;
		sigaction(SIGABRT, &action, NULL);
	}
	if (action.sa_handler == SIG_DFL)
		fflush(NULL);

	sigfillset(&mask);
	sigdelset(&mask, SIGABRT);
	sigprocmask(SIG_SETMASK, &mask, NULL); 
	
	kill(getpid(), SIGABRT);
	write(STDOUT_FILENO, "L_ABORT\n", 8);
	/*
	 * if we arrive here, the abort handler must execute and return.
	 */
	fflush(NULL);  /*flush stdio */
	action.sa_handler = SIG_DFL;
	sigaction(SIGABRT, &action, NULL);
	sigprocmask(SIG_SETMASK, &mask, NULL); /* just in case ..*/
	kill(getpid(), SIGABRT);

	exit(1);

}

int main(int argc, char *argv[])
{
#ifdef HANDLER
	signal(SIGABRT, sig_abrt);
#endif
	printf("in main:\n");
	l_abort();
	printf("after abort:\n"); /*never arrive */
	return 0;
}


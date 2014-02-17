#include "public.h"
#include <pthread.h>
#include <signal.h>
#include <syslog.h>

extern int already_running();

extern void daemonize(const char *cmd);

sigset_t mask;

void reload()
{
	/* reload config file */
	syslog(LOG_INFO, "RELOAD CONFIG FILE");
}

void *thr_fn(void * arg)
{
	int signo;

	while(sigwait(&mask, &signo) == 0)
	{
		switch(signo)
		{
			case SIGHUP:
					reload();
					break;
			case SIGTERM:
					syslog(LOG_INFO, "DAEMONIZE EXIT");
					exit(0);
			default:
					syslog(LOG_INFO, "CAUGHTED A SIGNAL %d", signo);
					break;
		}
	}

	return (void *)1;
}

int main(int argc, char *argv[])
{
	char *pcmd;
	struct sigaction act;	
	pthread_t tid;

	pcmd = strrchr(argv[0], '/');
	if (pcmd == NULL)
		pcmd = argv[0];
	else 
		pcmd++;
	
	daemonize(pcmd);
	
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigfillset(&mask);
	if (pthread_sigmask(SIG_BLOCK, &mask, NULL) < 0)
	{
		syslog(LOG_ERR, "pthread_sigmask failed.[%m]");
		exit(-1);
	}
	pthread_create(&tid, NULL, thr_fn, NULL);

	pthread_join(tid, (void**)0);

	return 0;	
}



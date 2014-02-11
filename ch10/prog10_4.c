#include "../public.h"

#include <signal.h>

static void sig_alarm(int signo);
static int sleep1(int nsecs);

int main(int argc, char *argv[])
{
	printf("start...\n");
	sleep1(5);
	printf("end...\n");
	return 0;
}

static void sig_alarm(int signo)
{
	return;
}

static int sleep1(int nsecs)
{
	int errno_save;

	if (signal(SIGALRM, sig_alarm) == SIG_ERR)
	{
		errno_save = errno;
		perror("signal SIGALARM failed");
		errno = errno_save;
		return -1;
	}
	alarm(nsecs);
	pause();
	return (alarm(0));
}


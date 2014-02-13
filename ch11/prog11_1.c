#include "../public.h"
#include <stdio.h>
#include <pthread.h>

static void printids(const char *pszStr)
{
	pthread_t tid;
	pid_t	pid;
	
	pid = getpid();
	tid = pthread_self();

	printf("%s pid = %08d tid = %08d tid(hex) = %08x\n", pszStr, pid, (int)tid, (int)tid);

	return;
}

void *thr_fn(void * arg)
{
	printids("new thread:");
	
	return (void *)0;
}

int main(int argc, char *argv[])
{
	int err;
	pthread_t tid;

	err = pthread_create(&tid, NULL, thr_fn, NULL);
	if (err < 0)
	{
		printf("pthread_create error[%d][%s]\n", err, strerror(err));
		return -1;
	}
	printids("main thread:");
	sleep(1);

	return 0;
}



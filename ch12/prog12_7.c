#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void prepare(void)
{
	printf("prepare locks...\n");
	pthread_mutex_lock(&lock1);
	pthread_mutex_lock(&lock2);
}

void parent(void)
{
	printf("parent locks ...\n");
	pthread_mutex_unlock(&lock1);
	pthread_mutex_unlock(&lock2);
}

void child(void)
{
	printf("child locks ...\n");
	pthread_mutex_unlock(&lock1);
	pthread_mutex_unlock(&lock2);
}

void *thr_fn(void *arg)
{
	printf("thread started...\n");
	pause();
	return 0;
}
int main(void)
{
	int err;
	pid_t pid;
	pthread_t tid;
	
#if defined(BSD) || defined(MACOS)
	printf("pthread_atfork is unsupported.\n");
#else
	if ((err = pthread_atfork(prepare, parent, child)) != 0)
	{
		printf("pthread_atfork failed[%s]\n", strerror(err));
		return -1;
	}
	err = pthread_create(&tid, NULL, thr_fn, NULL);
	if (err)
	{
		printf("pthread_create failed[%s]\n", strerror(err));
		return -1;
	}
	sleep(2);
	printf("parent about to fork...\n");
	if ((pid = fork()) < 0)
	{
		perror("fork error");
		return -1;
	}
	else if (pid == 0)
	{
		printf("child return from fork\n");
	}
	else 
		printf("parent return from fork\n");
#endif
	return 0;	
}



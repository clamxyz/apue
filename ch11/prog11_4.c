#include <pthread.h>
#include <stdio.h>
#include <string.h>

void cleanup(void *arg)
{
	printf("cleanup: arg = %s\n", (char *)arg);	
}
void *thr_fn1(void *arg)
{
	printf("in thr_fn1:\n");
	pthread_cleanup_push(cleanup, "thread 1 first handler");
	pthread_cleanup_push(cleanup, "thread 1 second handler");
	if (arg != NULL)
		return (void *)1;
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	return (void *)1;	
}

void *thr_fn2(void *arg)
{
	printf("in thr_fn2:\n");
	pthread_cleanup_push(cleanup, "thread 2 first handler");
	pthread_cleanup_push(cleanup, "thread 2 second handler");
	if (arg != NULL)
		pthread_exit((void *)2);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_exit((void *)2);
}

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;
	int err;
	void *ptr;
	
	err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
	if (err < 0)
	{
		printf("pthread_create 1 failed [%s]", strerror(err));
		return -1;
	}
	err = pthread_join(tid1, (void **)&ptr);	
	if (err != 0)
	{
		printf("pthread_join 1 failed.[%s]\n", strerror(err));
		return -1;
	}
	err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
	if (err < 0)
	{
		printf("pthread_create 2 failed [%s]", strerror(err));
		return -1;
	}
	pthread_join(tid2, (void **)&ptr);
	if (err != 0)
	{
		printf("pthread_join 2 failed [%s]\n", strerror(err));
		return -1;
	}
	return 0;
}



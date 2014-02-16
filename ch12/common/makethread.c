#include <stdio.h>
#include <pthread.h>

/*
 * 以分离状态创建线程
 */
int makethread(void *(*thr_fn)(void *), void *arg)
{
	pthread_attr_t attr;
	pthread_t tid;
	int err;

	err = pthread_attr_init(&attr);
	if (err)
		return err;
	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (err)
		return err;
	err = pthread_create(&tid, &attr, thr_fn, arg);
	if (err)
		return err;
	pthread_attr_destroy(&attr);
	
	return 0;
}



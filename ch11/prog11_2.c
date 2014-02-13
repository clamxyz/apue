#include <pthread.h>
#include <stdio.h>

void* thr_fn1(void *arg)
{
	printf("in thr_fn1:\n");
	pthread_exit((void *)1);
}

void* thr_fn2(void *arg)
{
	printf("in thr_fn2:\n");
	pthread_exit((void *)2);
}

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;
	int err1, err2;
	void *rtval;

	err1 = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err1 < 0)
	{
		printf("pthread1 create failed.\n");
		return -1;
	}
	err2 = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err2 < 0)
	{
		printf("pthread2 create failed.\n");
		return -1;
	}

	pthread_join(tid1, (void **)&rtval);
	printf("tid1 exit, exit code %p\n", rtval);
	pthread_join(tid2, (void **)&rtval);
	printf("tid2 exit, exit code %p\n", rtval);

	return 0;
}




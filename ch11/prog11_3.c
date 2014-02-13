#include <pthread.h>
#include <stdio.h>
#include <string.h>

struct foo
{
	int a, b, c, d;
};

void print_foo(const char *pszStr, struct foo *pf)
{
	printf("%s", pszStr);
	printf("address = %p", pf);
	printf("foo.a = %d, foo.b = %d, foo.c = %d, foo.d = %d\n", pf->a, pf->b, pf->c, pf->d);
}

void *thr_fn1(void *arg)
{
	struct foo foo;
	foo.a = 1;
	foo.b = 2;
	foo.c = 3;
	foo.d = 4;
	
	printf("in thr_fn1:\n");
	
	/*return &foo;	*/
	pthread_exit(&foo);
}

void *thr_fn2(void *arg)
{
	printf("in thr_fn2:\n");
/*	return (void *)0; */
	pthread_exit((void *)1);
}

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;
	int err;
	struct foo *ptr;

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err < 0)
	{
		printf("pthread_create 1 failed [%s]", strerror(err));
		return -1;
	}
	pthread_join(tid1,(void *)&ptr);	
	print_foo("after thread1", (struct foo *)ptr);
	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err < 0)
	{
		printf("pthread_create 2 failed [%s]", strerror(err));
		return -1;
	}
	print_foo("after thread2", (struct foo *)ptr);

	return 0;
}



#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "mkt.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

struct to_info{
	void (*to_fn)(void *);
	void *to_arg;
	struct timespec to_wait;
};

void *timedout_helper(void *arg)
{
	struct to_info *pto_info = (struct to_info *)arg;

	nanosleep(&pto_info->to_wait, NULL);
	(*pto_info->to_fn)(pto_info->to_arg);
	
	return (void *)0;	
}

void timeout(const  struct timespec *when, void (*func)(void *), void *arg)
{
	struct timespec now;
	struct timeval tv;
	struct to_info *tip;
	int err;
	
	gettimeofday(&tv, NULL);
	now.tv_sec = tv.tv_sec;
	now.tv_nsec = tv.tv_usec * 1000;
	
	if ( (when->tv_sec > now.tv_sec) || (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec) )
	{
		if (tip = (struct to_info *)malloc(sizeof(struct to_info)))
		{
			tip->to_fn = func;
			tip->to_arg = arg;
			tip->to_wait.tv_sec = when->tv_nsec - now.tv_nsec;
			if (when->tv_nsec >= now.tv_nsec)
			{
				tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
			}	
			else
			{
				tip->to_wait.tv_sec--;
				tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec + 100000000;
			}
			err = makethread(timedout_helper, (void *)arg);
			if (err == 0)
				return ;
		}
	}
	(*func)(arg);	
}

pthread_mutex_t mutex;
pthread_mutexattr_t mutexattr;

void retry(void *arg)
{
	pthread_mutex_lock(&mutex);
	/* do something */
	printf("in retry\n");
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
	int err, condition, arg;
	struct timespec when;

	if ((err = pthread_mutexattr_init(&mutexattr)))
	{
		printf("error:%s\n", strerror(err));
	}
	if ((err = pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE)))
	{
		printf("error:%s\n", strerror(err));
	}	
	if ((err = pthread_mutex_init(&mutex, &mutexattr)))
	{
		printf("error:%s\n", strerror(err));
	}	
		
	pthread_mutex_lock(&mutex);
	if (condition)
	{
		timeout(&when, retry, (void *)arg);
	}
	pthread_mutex_unlock(&mutex);
	
	sleep(2);	
	return 0;
}


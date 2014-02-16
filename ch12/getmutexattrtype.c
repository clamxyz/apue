#include <pthread.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	pthread_mutexattr_t mutexattr;
	int type;
	pthread_mutexattr_init(&mutexattr);
	pthread_mutexattr_gettype(&mutexattr, &type);
	if (type == PTHREAD_MUTEX_NORMAL)
		printf("PTHREAD_MUTEX_NORMAL");
	else if (type == PTHREAD_MUTEX_ERRORCHECK)
		printf("PTHREAD_MUTEX_ERRORCHECK");
	else if (type == PTHREAD_MUTEX_RECURSIVE)
		printf("PTHREAD_MUTEX_RECURSIVE");
	else 
		printf("PTHREAD_MUTEX_DEFAULT");	
	puts("\n");
	return 0;
}


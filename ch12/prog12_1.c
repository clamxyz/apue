#include "mkt.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void *thr_fn(void *arg)
{
	printf("in thr_fn:%s\n", (char *)arg);
	
	return (void *)0;
}

int main(int argc, char *argv[])
{
	int err;
	
	err = makethread(thr_fn, (void *)"make a thread with detached state.");
	if (err)
		printf("makethread error[%s]\n", strerror(err));
	
	sleep(5);	
	return 0;
}



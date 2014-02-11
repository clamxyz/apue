#include "../public.h"
#include <signal.h>
#include <stdio.h>

/* 5MB */
#define SIZE  1024

static char *random_buffer(char buf[], size_t size);
static void sig_alrm(int signo);

int main()
{
	char buf[SIZE];
	random_buffer(buf, SIZE);
	signal(SIGALRM, sig_alrm);
	alarm(1);
	fwrite(buf, 1, SIZE, stdout);

	return 0;
}

static void sig_alrm(int signo)
{
	printf("in sig_alrm:\n");
}

static char *random_buffer(char buf[], size_t size)
{
	char *ptr = buf;
	int i = 0;

	if (buf == NULL)
		return ptr;
	srand(0);
	for(i = 0;i < size; i++)
	{
		buf[i] = 0x65 + random() % 10;
	}
	return ptr;
}


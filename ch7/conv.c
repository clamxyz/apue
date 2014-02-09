#include "../public.h"

static void charactertime(char *pszStr);

int main(int argc, char *argv[])
{
	pid_t pid;

	if ((pid = fork()) < 0)
	{
		perror("fork error");
		exit(0);
	}
	else if (pid == 0)
	{
		charactertime("output from stdout\n");
	}
	else charactertime("output from stdout\n");

	return 0;
}

static void charactertime(char *pszStr)
{
	int c;
	char *ptr;
	
	setbuf(stdout, NULL);
	for(ptr = pszStr; c = *ptr++;)
	{
		putc(c, stdout);
	}	
}


#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	#define BUFFSIZE	8192
	int n;
	char buf[BUFFSIZE];
	
	while((n=read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
	{
		if (n != (write(STDOUT_FILENO, buf, n)))
			printf("write error[%d][%s].\n", errno, strerror(errno));
	}
	if (n < 0)
	{
		printf("read error[%d][%s]\n", errno, strerror(errno));
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}




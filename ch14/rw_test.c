#include "fl.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

extern int errno;

int main()
{
	int nread;
	int ntoread;
	int nwrite;
	int ntowrite;
	char buf[1024];

	ntoread = 10;
	if ((nread = readn(STDIN_FILENO, buf, ntoread)) < 0) 	
	{
		printf("readn error[%d][%s]\n", errno, strerror(errno));
		return -1;	
	}
	ntowrite = ntoread;	
	if ((nwrite = write(STDOUT_FILENO, buf, ntowrite)) < 0)
	{
		printf("readn error[%d][%s]\n", errno, strerror(errno));
		return -1;	
	}

	return 0;
}


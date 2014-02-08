#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int c;
	
	while( (c = getc(stdin)) != EOF)
	{
		if ( EOF == putc(c, stdout))
		{
			printf("putc error[%d][%s]\n", errno, strerror(errno));
			return EXIT_FAILURE;
		}
	}
	
	return EXIT_SUCCESS;
}


#include "public.h"

int main(int argc, char *argv[])
{
	printf("EACESS: %s\n", strerror(EACCES));
	errno = ENOENT;
	perror(argv[0]);
	return 0;
}


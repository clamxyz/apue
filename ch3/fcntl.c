#include "../public.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	int flag;
	int mode;

	if (argc != 2)
	{
		printf("need one argument.\n");
		return EXIT_FAILURE;
	}
	fd =atoi(argv[1]);
	flag = fcntl(fd, F_GETFL, 0);
	mode = flag & O_ACCMODE;
	if (mode == O_RDONLY)
		printf("read only");
	else if (mode == O_WRONLY)
		printf("write only");
	else printf("read write");

	if (flag & O_APPEND == O_APPEND)
		printf(",append");
	else if (flag & O_NONBLOCK)
		printf(", nonblock");
	printf("\n");

	return 0;
}



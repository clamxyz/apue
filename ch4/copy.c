#include "../public.h"

#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int srcfd, dstfd;
	int c;

	if (argc != 3)
	{
		printf("usage:./copy <src> <dest>\n");
		return 0;
	}

	srcfd = open(argv[1], O_RDONLY);
	if (srcfd < 0)
	{
		printf("open failed[%d][%s]\n", errno, strerror(errno));
		return -1;
	}	
	dstfd = creat(argv[2], S_IRWXU | S_IRWXG | S_IRWXO);
	if (dstfd < 0)
	{
		printf("create failed[%d][%s]\n", errno, strerror(errno));
		return -1;
	}

	while(read(srcfd, &c, 1) == 1)
	{
		if (c == 0)
			continue;
		if (write(dstfd, &c, 1) < 1)
		{
			printf("write failed[%d][%s]\n", errno, strerror(errno));
			break;
		}	
	}
	
	close(srcfd);
	close(dstfd);

	return 0;
}


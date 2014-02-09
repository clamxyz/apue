#include "../public.h"

#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd = -1;
	if (argc < 2)
	{
		printf("1 arguments needed!\n");
		return -1;
	}
	fd = open(argv[1], O_RDONLY);
	if (lseek(fd, 0, SEEK_CUR) == -1)
	{
		printf("%s:Cannot seek!\n", argv[1]);
	}
	else 
	{
		printf("%s:Can seek!\n", argv[1]);
	}	
	close(fd);
	return 0;
}


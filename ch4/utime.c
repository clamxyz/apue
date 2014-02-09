#include "../public.h"

#include <sys/stat.h>
#include <utime.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	struct utimbuf times;
	struct stat buf;
	int i;
	int fd;
	
	if (argc < 2)
	{
		printf("useage: utime <file1> <file2> ..\n");
		return EXIT_FAILURE;
	}
	for (i = 1; i < argc; i++)
	{
		if (stat(argv[i], &buf) != 0)
		{
			perror(argv[i]);
			continue;
		}
		if ((fd = open(argv[i], O_WRONLY | O_TRUNC)) < 0)
		{
			perror(argv[i]);
			continue;
		}
		times.actime = buf.st_atime;
		times.modtime = buf.st_mtime;
		if (utime(argv[i], &times) < 0)
		{
			perror(argv[i]);
			close(fd);
			continue;
		}
		close(fd);
	}	
	return EXIT_SUCCESS;
}


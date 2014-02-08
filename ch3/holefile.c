#include "../public.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd = -1;
	char buf[] = "abcdefghijkl";
	char buf1[] = "ABCDEFGHIJKL";

	if (argc < 2)
	{
		printf("need a argument.\n");
		return EXIT_FAILURE;
	}
	fd = creat(argv[1], S_IRWXU | S_IRWXG | S_IXOTH);
	if (fd == -1)
	{
		printf("creat file %s[%d][%s]\n", argv[1], errno, strerror(errno));
		return EXIT_FAILURE;
	}
	if (write(fd, buf, sizeof buf) != sizeof(buf))
	{
		printf("write error[%d][%s]\n", errno, strerror(errno));
		return EXIT_FAILURE;
	}
	if (lseek(fd, 10, SEEK_CUR) == -1)
	{
		printf("seek error[%d][%s]\n", errno, strerror(errno));
		return EXIT_FAILURE;
	}
	if (write(fd, buf, sizeof buf1) != sizeof(buf1))
	{
		printf("creat file %s[%d][%s]\n", argv[1], errno, strerror(errno));
		return EXIT_FAILURE;
	}
	close(fd);
	return EXIT_SUCCESS;
}


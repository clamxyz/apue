#include "fl.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern int errno;
/*
 * 同一个进程可以对同一个文件可以一直加锁
 */
int main()
{
	int fd;

	if ( (fd = open("temp.foo", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG)) < 0)
	{
		printf("open failed.[%d][%s]\n", errno, strerror(errno));
		return -1;
	}

	if (lockfile(fd) < 0)
	{
		printf("lockfile failed[%d][%s]\n", errno, strerror(errno));
		close(fd);
		return -1;
	}
	write(fd, "ww", 3);
	sleep(100);
	if (lockfile(fd) < 0)
	{
		printf("lockfile failed[%d][%s]\n", errno, strerror(errno));
		close(fd);
		return -1;
	}

	close(fd);
	return -1;
}


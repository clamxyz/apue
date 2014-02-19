#include "fl.h"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int lock_reg(int fd, int cmd, int type, off_t start, int whence, off_t len)
{
	struct flock lock;

	lock.l_type = type;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;

	return (fcntl(fd, cmd, &lock));
}

int lock_test(int fd, int cmd, int type, off_t start, int whence, off_t len)
{
	struct flock lock;

	lock.l_type = type;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;

	if (fcntl(fd, cmd, &lock) < 0)
		return -1;
	if (lock.l_type == F_UNLCK)
		return 0;
	return (lock.l_pid);
}

int lockfile(int fd)
{
	struct flock lock;

	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_whence = SEEK_SET;
	
	return fcntl(fd, F_SETLK, &lock);
}



#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

#define LOCKFILE	"/var/run/daemon.pid"
#define LOCKMODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int already_running(void)
{
	int fd;
	char buf[100];

	fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);

	if (fd < 0)
	{
		syslog(LOG_ERR, "open LOCK failed[%ld][%m]", (long)getpid());
		exit(1);
	}

	if (flock(fd, LOCK_EX | LOCK_NB) < 0)
	{
		if (errno == EWOULDBLOCK)
		{
			syslog(LOG_ERR, "system is already running");
			return 1;
		}
	}

	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf));
	return 0;	
}


#include "mysock.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#define MAXSLEEP 128

int connect_retry(int fd, struct sockaddr *paddr, socklen_t len)
{	
	
	int nsec;

	for(nsec = 1; nsec < MAXSLEEP; nsec <<= 1)
	{
		if (connect(fd, paddr, len) == 0)
			return 0;
		if (nsec < MAXSLEEP / 2)
			sleep(nsec);
	}
	if (nsec >= MAXSLEEP)
		return -1;
	return 0;
}

int initserv(int type, struct sockaddr *paddr, socklen_t len)
{
	int fd;
	int err;
	int reuse = 1;
	if ( (fd = socket(paddr->sa_family, type, 0)) < 0)
		return -1;
	if ( setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
		return -1;	
	if ( bind(fd, paddr, len) < 0)
	{
		err = errno;
		goto errout;
	}
	if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
	{
			if (listen(fd, SOMAXCONN) < 0)
			{
					err = errno;
					goto errout;
			}
		return fd;
	}
errout:
	close(fd);
	errno = err;
	return -1;
}


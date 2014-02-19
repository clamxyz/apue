#include "fl.h"
#include <sys/types.h>
#include <unistd.h>

ssize_t readn(int fd, void *buf, size_t n)
{
	int nleft = n;
	int nread = 0;

	while(nleft > 0)
	{
		if ((nread = read(fd, buf, nleft)) < 0)
		{
			if (n == nleft) return (-1); /* error, return -1 */
			else
				break;				/* error, return amount read so far */
		}
		else if (nread == 0)
			break;
		
		nleft -= nread;
		buf += nread;
	}

	return n - nleft;
}

ssize_t writen(int fd, void *buf, size_t n)
{
	int nleft = n;
	int nwritten = 0;
	
	while (nleft > 0)
	{
		if ( (nwritten = write(fd, buf, nleft)) < 0)
		{
			if (nleft == n) return -1;
			else  break;
		}
		else if (nwritten == 0)
		{
			break;
		}
		nleft -= nwritten;
		buf += nwritten;
	}

	return n - nleft;
}


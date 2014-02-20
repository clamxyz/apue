#include "mysock.h"
#include "public.h"
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

static void print_recv(int fd);

int main()
{
	int fd;
	char host[] = "clamxyz";
	struct addrinfo *ailist;
	struct addrinfo *aip;
	struct addrinfo hit;
	int err;

	memset(&hit, 0, sizeof hit);
	hit.ai_socktype = SOCK_STREAM;
	
	if ( (err = getaddrinfo(host, "ruptimed", &hit, &ailist) < 0))
	{
		printf("getaddrinfo error[%d][%s]\n", err, gai_strerror(err));
		return -1;
	}	
	for(aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		if ((fd = socket(aip->ai_family, aip->ai_socktype, 0)) < 0) 
		{
			printf("socket error[%d][%s]\n", errno, strerror(errno));
			return -1;
		}
		if ( connect_retry(fd, aip->ai_addr, aip->ai_addrlen) < 0)
		{
			printf("connect_retry error[%d][%s]\n", errno, strerror(errno));
			return -1;
		}
		print_recv(fd);
	}

	return 0;
}

static void print_recv(int fd)
{
	#define BUFLEN 128
	char buf[BUFLEN];

	while(recv(fd, buf, BUFLEN, 0) > 0)
	{
		printf("%s", buf);
	}
	close(fd);
	return;
}

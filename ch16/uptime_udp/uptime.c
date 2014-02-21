#include "public.h"
#include "mysock.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFLEN 128

int print_buf(int fd, struct addrinfo *aip)
{
	char buf[BUFLEN];

	buf[0] = 0;
	if (sendto(fd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen) < 0)
	{
		printf("sendto error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	if (recvfrom(fd, buf, BUFLEN, 0, NULL, NULL) < 0)
	{
		printf("recvfrom error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	puts(buf);
  
	return 0;
}
int main(int argc, char *argv[])
{
	int fd;
	int err;
	struct addrinfo hit;
	struct addrinfo *aip;
	struct addrinfo *ailist;
	char host[INET_ADDRSTRLEN];
	char server[] = "ruptimed";	

	memset(&hit, 0, sizeof hit);
	hit.ai_socktype = SOCK_DGRAM;
	if (gethostname(host, INET_ADDRSTRLEN) < 0)
	{
		printf("gethostname error[%d][%s]\n", errno, strerror(errno));
		exit(1);
	}	
	if ( (err = getaddrinfo(host, server, &hit, &ailist)) < 0)
	{
		printf("getaddrinfo error[%d][%s]\n", err,gai_strerror(err));
		return -1;
	}
	for(aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		if ( (fd = socket(aip->ai_family, aip->ai_socktype, 0)) < 0)
		{
			printf("socket error[%d][%s]\n", errno, strerror(errno));
			return -1;
		}
		print_buf(fd, aip);
		close(fd);
		exit(0);
	}
	printf("not found aip\n");
	return 0;
}


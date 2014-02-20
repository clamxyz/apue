#include "public.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

static void print_af(struct addrinfo *aip)
{
	if (aip == (struct addrinfo *)0)
		return ;
	printf("family ");
	switch(aip->ai_family)
	{
		case AF_INET:
				printf("inet ");
				break;
		case AF_INET6:
				printf("inet6 ");
				break;
		case AF_UNSPEC:
				printf("unspecific");
				break;
		case AF_UNIX:
				printf("unix");
				break;
	}
	return;
}

static void print_flags(struct addrinfo *aip)
{
	if (aip == (struct addrinfo *)0)
		return ;
	printf("flag ");
	switch(aip->ai_flags)
	{
		case AI_ADDRCONFIG:
			printf("addrconfig ");
			break;
		case AI_ALL:
			printf("all ");
			break;
		case AI_CANONNAME:
			printf("canonname ");
			break;
		case AI_NUMERICHOST:
			printf("numerichost ");
			break;
		case AI_PASSIVE:
			printf("passive ");	
			break;
		case AI_V4MAPPED:
			printf("v4mapped ");
			break;
		default:
			printf("unknown ");
			break;
	}
	return;
}

static void print_protocal(struct addrinfo *aip)
{
	if (aip == (struct addrinfo *)0)
		return ;
	printf("protocal ");
	switch(aip->ai_protocol)
	{
		case 0:
			printf("default ");
			break;
		case IPPROTO_TCP:
			printf("tcp ");
			break;
		case IPPROTO_UDP:
			printf("udp ");
			break;
		case IPPROTO_RAW:
			printf("raw ");
			break;
		default:
			printf("unknown ");
			break;
	}

}

static void print_type(struct addrinfo *aip)
{
	if (aip == (struct addrinfo *)0)
		return;
	printf("type ");
	switch(aip->ai_socktype)
	{
		case SOCK_STREAM:
			printf("stream ");
			break;
		case SOCK_DGRAM:
			printf("datagram ");
			break;
		case SOCK_SEQPACKET:
			printf("seqpacket ");
			break;
		case SOCK_RAW:
			printf("raw ");
			break;
		default:
			printf("unknown ");
			break;
	}

	return;
}

int main(int argc, char *argv[])
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hit;
	struct sockaddr_in *sinp;
	const char *addr;
	int err;
	char abuf[INET_ADDRSTRLEN];

	if (argc != 3)
	{
		printf("Usage:%s <nodename> <service>\n", argv[0]);
		return -1;
	}

	hit.ai_flags = AI_CANONNAME;
	hit.ai_family = 0;
	hit.ai_socktype = 0;
	hit.ai_protocol = 0;
	hit.ai_addrlen = 0;
	hit.ai_canonname = NULL;
	hit.ai_addr = NULL;
	hit.ai_next = NULL;
	if ( (err = getaddrinfo(argv[1], argv[2], &hit, &ailist)) != 0)
	{
		printf("getaddrinfo error[%d][%s]\n", err, gai_strerror(err));
		return -1;
	}
	
	for (aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		print_flags(aip);
		print_af(aip);
		print_type(aip);
		print_protocal(aip);

		if (aip->ai_family == AF_INET)
		{
			sinp = (struct sockaddr_in *)aip->ai_addr;
			printf("\n\thost %s address %s port %d\n", argv[2], inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN), ntohs(sinp->sin_port));
		}
	}
	return 0;
}


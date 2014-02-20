#include "public.h"
#include "mysock.h"

#include <syslog.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFLEN 128

extern void daemonize();

int serv(int fd)
{
	char buf[BUFLEN];
	char host[INET_ADDRSTRLEN];
	struct sockaddr sa;
	socklen_t len;	
	struct sockaddr_in *sinp;
	FILE *fp;
	
	while(recvfrom(fd, buf, BUFLEN, 0, &sa, &len) > 0)
	{
		sinp = (struct sockaddr_in *)&sa;
		syslog(LOG_INFO, "RECV FROM client[%s]", inet_ntop(sinp->sin_family, &sinp->sin_addr.s_addr, host, INET_ADDRSTRLEN));
		fp = popen("/usr/bin/uptime", "r");
		if (fp == NULL)
		{
			sprintf(buf, "popen error[%s]\n", strerror(errno));
			sendto(fd, buf, strlen(buf), 0, &sa, len);
			exit(1);
		}
		while(fgets(buf, BUFLEN, fp) > 0)
		{
			sendto(fd, buf, strlen(buf), 0, &sa, len);
		}			
	}
	syslog(LOG_ERR, "recvfrom error[%d][%s]", errno, strerror(errno));	
	return 0;
}

int main(int argc, char *argv[])
{
	int fd;
	int err;
	char host[INET_ADDRSTRLEN];
	struct addrinfo hit;
	struct addrinfo *aip;
	struct addrinfo *ailist;

	if (gethostname(host, INET_ADDRSTRLEN) < 0)
	{
		printf("gethostname error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	memset(&hit, 0, sizeof hit);
	hit.ai_socktype = SOCK_DGRAM; 
	daemonize();
	
	if ((err = getaddrinfo(host, "ruptimed", &hit, &ailist)) < 0)
	{
		syslog(LOG_ERR, "getaddrinfo error[%d][%s]", err, gai_strerror(err));
		return -1;
	}
	
	for (aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		syslog(LOG_ERR, "socket type = %s\n", aip->ai_socktype == SOCK_DGRAM ? "SOCK_DGRAM" : "Default");
		if ( (fd = socket(aip->ai_family, aip->ai_socktype, 0)) < 0)
		{
			syslog(LOG_ERR, "socket error[%d][%m]", errno);
		return -1;
		}
		if ( initserv(fd, aip->ai_addr, aip->ai_addrlen) < 0)
		{
			syslog(LOG_ERR, "initserv error[%d][%m]", errno);
			return -1;
		}

		serv(fd);	
		close(fd);
		exit(0);
	}		
	return 0;
}


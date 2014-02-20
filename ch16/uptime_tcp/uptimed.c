#include "mysock.h"
#include "public.h"

#include <syslog.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>

extern int daemonize(const char *cmd);

#define BUF_LEN 128

int serv(int fd)
{
	int clfd = -1;
	struct sockaddr addr;
	socklen_t len;
	struct sockaddr_in *psin;
	char buf[INET_ADDRSTRLEN];
	char context[BUF_LEN];
	FILE *fpout;
	int slen;

	while(1)
	{
		clfd = accept(fd, &addr, &len);	
		if (clfd < 0)
		{
			syslog(LOG_ERR, "accept error[%d][%m]", errno);
			exit(1);
		}
		psin = (struct sockaddr_in *)&addr;
		syslog(LOG_INFO, "accept client[%s]\n", inet_ntop(AF_INET, &psin->sin_addr.s_addr, buf, INET_ADDRSTRLEN));
	
		if ( (fpout = popen("/usr/bin/uptime", "r")) == NULL)
		{
			syslog(LOG_ERR, "popen error[%d][%m]", errno);
			continue;
		}
		syslog(LOG_INFO, "popen success");
		while ( fgets(context, BUF_LEN, fpout) != NULL)
		{
			slen = strlen(context);
			if ( 0 > send(clfd, context, slen, 0))
			{
				syslog(LOG_ERR, "send error[%d][%m]", errno);
				break;
			}	
		}
		pclose(fpout);
		close(clfd);
		syslog(LOG_INFO, "close client[%s]\n", buf);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	char *phost;
	long n;
	int fd;
	int err;
	struct addrinfo *aip;
	struct addrinfo *ailist;
	struct addrinfo hit;
	
#ifdef _SC_HOST_NAME_MAX 
	n = sysconf(_SC_HOST_NAME_MAX);
	if (n < 0)
	{
		printf("sysconf _SC_HOST_NAME_MAX[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
#elif defined(HOST_NAME_MAX)
	n = HOST_NAME_MAX;
#else
	n = 256;
#endif
	
	if ( (phost = (char *)malloc(n)) < 0)
	{
		printf("malloc failed[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	
	if (gethostname(phost, n) < 0)
	{
		printf("gethostname failed[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	memset(&hit, 0, sizeof hit);
	hit.ai_flags = AI_CANONNAME;
	daemonize("ruptimed");

	if ( (err = getaddrinfo(phost, "ruptimed", &hit, &ailist)) < 0)
	{
		syslog(LOG_ERR, "getaddrinfo failed[%d][%s]", err, gai_strerror(err));
		return -1;	
	}
	for (aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		if (aip->ai_socktype == SOCK_STREAM)
		{
			if ( (fd = initserv(aip->ai_socktype, aip->ai_addr, aip->ai_addrlen)) < 0)
			{
				syslog(LOG_ERR, "initserv failed[%d][%m]", errno);
				return -1;
			}
			else
				break;
		}	
	}	
	
	serv(fd);
	close(fd);	

	return 0;
}


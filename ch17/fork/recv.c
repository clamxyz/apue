#include "public.h"
#include "fl.h"
#include <sys/socket.h>
#include <sys/uio.h>
#include <stddef.h>

static struct cmsghdr *cmptr = NULL;
#define CONTROLLEN	(sizeof(struct cmsghdr) + sizeof(int))

int recv_fd(int servfd, ssize_t (*userfunc)(int, const void *, size_t))
{
	int newfd, nread, status;
	char *ptr, buf[MAXLINE];
	struct iovec iov[1];
	struct msghdr msg;

	status = -1;
	for ( ; ; )
	{
		iov[0].iov_base = buf;
		iov[0].iov_len = sizeof(buf);
		msg.msg_iov = iov;
		msg.msg_iovlen = 1;
		msg.msg_name = NULL;
		msg.msg_namelen = 0;

		if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return -1;
		msg.msg_control = (caddr_t) cmptr;
		msg.msg_controllen = CONTROLLEN;
		if ( (nread = recvmsg(servfd, &msg, 0)) < 0)
		{
			err_sys("recvmsg error", errno);
		}
		else if (nread == 0)
		{
			err_ret("connection close by server", 0);
			return -1;
		}	
		
		for (ptr = buf; ptr < &buf[nread];){
			if (*ptr++ == 0)
			{
				if (ptr != &buf[nread-1])
				{
					printf("message format error\n");
				}
				status = *ptr & 255;
				if (status == 0)
				{
					if (msg.msg_controllen != CONTROLLEN)
					{
						printf("status = 0 but no fd");
					}
					newfd = *(int *)CMSG_DATA(cmptr);
				} else newfd = -status;

				nread -= 2;
			}
		}	
		if (nread > 0)
			if ((*userfunc)(STDERR_FILENO, buf, nread) != nread)
				return -1;
		if (status >= 0)
			return newfd;
	}	
}


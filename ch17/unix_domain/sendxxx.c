#include "public.h"
#include "fl.h"
#include <sys/socket.h>
#include <sys/uio.h>
#include <stddef.h>

int send_err(int clifd, int errcode, char *errmsg)
{
	int n;

	if ( (n = strlen(errmsg)) > 0 )
	{
		if (writen(clifd, errmsg, n) != n)
		{
			return (-1);
		}	
	}	
	if (errcode >= 0)
	{
		errcode = -1;
	}
	if (send_fd(clifd, errcode) < 0)
		return -1;
	
	return 0;
}

static struct cmsghdr *cmptr = NULL;
#define CONTROLLEN	(sizeof(struct cmsghdr) + sizeof(int))

int send_fd(int clifd, int fd)
{
	struct iovec	iov[1];
	struct msghdr	msg;
	char		buf[2];
	iov[0].iov_base = buf;
	iov[0].iov_len = 2;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	if (fd < 0)
	{
		msg.msg_control = NULL;
		msg.msg_controllen = 0;
		buf[1] = -fd;
		if (buf[1] == 0)
			buf[1] = 1;
	}
	else {
		if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return -1;
		cmptr->cmsg_level = SOL_SOCKET;
		cmptr->cmsg_type = SCM_RIGHTS;
		cmptr->cmsg_len = CONTROLLEN;
		msg.msg_control = (caddr_t)cmptr;
		msg.msg_controllen = CONTROLLEN;
		*(int *)CMSG_DATA(cmptr) = fd;
		buf[1] = 0;
	}
	buf[0] = 0;
	if (sendmsg(clifd, &msg, 0) != 2)
		return (-1);
	return (0);
}



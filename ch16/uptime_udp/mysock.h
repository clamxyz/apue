#ifndef MYSOCK_H__
#define MYSOCK_H__

#include <sys/socket.h>
#include <sys/types.h>

int connect_retry(int fd, struct sockaddr *paddr, socklen_t len);

int initserv(int type, struct sockaddr *paddr, socklen_t len);
#endif /*MYSOCK_H__*/


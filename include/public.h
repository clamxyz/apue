#ifndef PUBLIC_H__
#define PUBLIC_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <unistd.h>
#include "debug.h"

#define MAXLINE 1024

int send_err(int clifd, int errcode, char *errmsg);
int send_fd(int clifd, int fd);

#endif


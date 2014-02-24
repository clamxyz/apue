#ifndef OPEN_H__
#define OPEN_H__

#include <sys/types.h>
#include <errno.h>
#include "public.h"
#include "fl.h"
#include <sys/uio.h>
#include <sys/socket.h>

#define CL_OPEN "open"

int csopen(char *, int);

#endif /*OPEN_H__*/


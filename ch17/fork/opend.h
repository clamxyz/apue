#include <sys/types.h>
#include <errno.h>
#include "public.h"
#include "fl.h"

#define CL_OPEN  "open"

extern char errmsg[];
extern int oflag;
extern char *pathname;

int cli_args(int, char **);
void request(char *, int, int);


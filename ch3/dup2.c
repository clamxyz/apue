#include "../public.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int open_max(void);

int main(int argc, char *argv[])
{
	int maxfd = open_max() - 1;
	int i;
	/*
	 *待实现
	 */

	return EXIT_SUCCESS;
}

extern int errno;
static int open_max(void)
{
#define OPEN_MAX
	int openmax = OPEN_MAX;
#else
	int openmax = 0;
#endif
#define OPEN_MAX_GUESS 1024

	if (openmax == 0)
	{
		errno = 0;
		openmax = sysconf(_SC_OPEN_MAX);
		if (openmax == -1)
		{
			if (errno == 0)
				openmax = OPEN_MAX_GUESS;
			else return -1;
		}
	}
	return openmax;
}


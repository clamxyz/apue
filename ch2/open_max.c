#include "../public.h"
#include <limits.h>

static int open_max(void);

int main(int argc, char *argv[])
{
	printf("open_max :%d\n", open_max());
	return 0;
}

static int open_max(void)
{
#define OPEN_MAX_GUESS	256
#ifdef OPEN_MAX
	int openmax = OPEN_MAX;
#else
	int openmax = 0;
#endif
	
	if (openmax == 0)
	{
		errno = 0;
		openmax = sysconf(_SC_OPEN_MAX);
		if (openmax < 0 && errno == 0)
		{
			openmax = OPEN_MAX_GUESS;
		}
		else if (openmax < 0) return -1;
	}
	
	return openmax;
}


#include "fl.h"
#include "public.h"
#include <fcntl.h>

char buf[500000];

int main(void)
{
	int ntowrite;
	int nwrited;
	char *ptr;

	if (500000 != read(STDIN_FILENO, buf, 500000))
	{
		fprintf(stderr, "read error[%s]", strerror(errno));
		return -1;
	}

	set_fl(STDOUT_FILENO, O_NONBLOCK);

	ntowrite = 500000;
	ptr = buf;
	while(ntowrite > 0)
	{
		nwrited = write(STDOUT_FILENO, buf, ntowrite);
		fprintf(stderr, "nwrited=%d, errno = %d\n", nwrited, errno);
		if (ntowrite > 0)
		{
			ptr += nwrited;
			ntowrite -= nwrited;
		}
	}

	return 0;
}


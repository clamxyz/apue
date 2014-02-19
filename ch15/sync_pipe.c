#include "public.h"

static int pfds[2] = {-1, -1};
static int cfds[2] = {-1, -1};

void TELL_WAIT()
{
	if (pipe(pfds) < 0)
	{
		pfds[0] = pfds[1] = -1;
		err_sys("pipe error", errno);
	}
	if (pipe(cfds) < 0)
	{
		cfds[0] = cfds[1] = -1;
		err_sys("pipe error", errno);
	}
}

void TELL_PARENT()
{
	if (pfds[0] < 0 || pfds[1] < 0 || cfds[0] < 0 || cfds[1] < 0)
		err_sys("invalid pipe fd", EINVAL);	
	if (write(pfds[1], "c", 1) < 0)
		err_sys("write c error", errno);
}

void WAIT_PARENT()
{
	char c;
	if (pfds[0] < 0 || pfds[1] < 0 || cfds[0] < 0 || cfds[1] < 0)
		err_sys("invalid pipe fd", EINVAL);
	if (read(pfds[0], &c, 1) != 1)
		err_sys("read p error", errno);
}

void WAIT_CHILD()
{
	char c;
	
	if (pfds[0] < 0 || pfds[1] < 0 || cfds[0] < 0 || cfds[1] < 0)
		err_sys("invalid pipe fd", EINVAL);

	if (read(cfds[0], &c, 1) != 1)
		err_sys("read c error", errno);
}

void TELL_CHILD()
{
	if (pfds[0] < 0 || pfds[1] < 0 || cfds[0] < 0 || cfds[1] < 0)
		err_sys("invalid pipe fd", EINVAL);
	if (write(cfds[1], "p", 1) != 1)
		err_sys("read error", errno);
}

int main()
{
		return 0;
}


#include <stdio.h>
#include <unistd.h>

extern void daemonize(const char *cmd);

int main()
{
	daemonize("daemon");
	sleep(100);

	return 0;
}


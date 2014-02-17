#include "public.h"
#include <string.h>
#include <syslog.h>

extern void daemonize(char *cmd);

int main(int argc, char *argv[])
{
	char *pcmd;

	pcmd = strrchr(argv[0], '/');
	if (pcmd == NULL)
		pcmd = argv[0];
	pcmd++;

	daemonize(pcmd);

	syslog(LOG_INFO, "getlogin=%s\n", getlogin());

	return 0;
}


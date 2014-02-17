#include "public.h"
#include <syslog.h>

int main()
{
	openlog("just for test", LOG_CONS, LOG_USER);
	syslog(LOG_ERR, "how to use syslog");
	syslog(LOG_INFO, "THIS IS INFO MESSAGE");
	closelog();	
	return 0;
}


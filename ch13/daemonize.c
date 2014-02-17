#include "public.h"
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

void daemonize(const char *cmd)
{
	pid_t pid;
	struct rlimit rlim;
	int i = 0;
	int fd0, fd1, fd2;
	struct sigaction sa;

	/*
	 * Clear file creation mode
	 */
	umask(0);

	/*
	 * Get the maxinum number of file descriptors
	 */

	if (getrlimit(RLIMIT_NOFILE, &rlim) < 0)
		err_sys("getrlimit failed", errno);
	/*
	 *	Become a session leader and lose controling of TTY  
	 */
	if ( (pid = fork()) < 0)
	{
		err_sys("fork error", errno);
	}
	else if (pid > 0)
		exit(0);
	setsid();

	/*
	 * Ensure future opens won't allocate controlling TTY
	 */	
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		err_sys("sigaction SIGHUP failed", errno);
	if ( (pid = fork()) < 0)
	{
		err_sys("fork error", errno);
	}
	else if (pid > 0) 
		exit(0);
	/*
	 * Change current work directory to "/"
	 */	
	if (chdir("/") < 0)
		err_sys("fork error", errno);

	if (rlim.rlim_max == RLIM_INFINITY)
		rlim.rlim_max = 1024;
	for (i = 0;i < rlim.rlim_max; i++)
		close(i);

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(fd0);
	fd2 = dup(fd0);	

	openlog(cmd, LOG_CONS, LOG_DAEMON);	
	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "fd0, fd1, fd2 has unexecpted value. fd0 = %d, fd1 = %d, fd2 = %d", fd0, fd1, fd2);
		exit(-1);
	}

	return ;
}


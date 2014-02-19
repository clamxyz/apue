#include "public.h"
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int pfds[2];
	pid_t pid;
	char buf[1024];
	char wrbuf[] = "Hello, World!\n";
	int ntowrite = sizeof wrbuf;
	int ntoread = ntowrite;

	if (pipe(pfds) < 0)
	{
		printf("pipe error.[%d][%s]\n", errno, strerror(errno));
		return -1;
	}

	if ( (pid = fork()) < 0)
	{
		printf("fork error.[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	else if (pid == 0)
	{
		/*child */
		close(pfds[1]);
		if (read(pfds[0], buf, ntoread) < 0)
		{
			perror("read error");
			_exit(0);
		}
		if (write(STDOUT_FILENO, buf, ntoread) < 0)
		{
			perror("write error");
			_exit(0);
		}
		close(pfds[0]);
		_exit(0);
	}
	else
	{
		close(pfds[0]);
		if (write(pfds[1], wrbuf, ntowrite) < 0)
		{
			perror("write error");
			return -1;
		}

		waitpid(pid, NULL, 0);
		close(pfds[1]);
		exit(0);
	}

	return 0;
}


#include "public.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int fds1[2], fds2[2];
	pid_t pid;

	if (pipe(fds1) < 0 || pipe(fds2) < 0)
		err_sys("pipe failed", errno);

	if ( (pid = fork()) < 0)
	{
		err_sys("fork error", errno);
	}
	else if (pid == 0)
	{
		close(fds1[1]);
		close(fds2[0]);

		if (fds1[0] != STDIN_FILENO)
		{
			dup2(fds1[0], STDIN_FILENO);
			close(fds1[0]);
		}
		if (fds2[1] != STDOUT_FILENO)
		{
			dup2(fds2[1], STDOUT_FILENO);
			close(fds2[1]);
		}

		execl("./add2", "add2", (void *)0);
		_exit(127);
	}
	else
	{
		int n;
		int len;
		char buf[MAXLINE + 1];

		close(fds1[0]);
		close(fds2[1]);
		while( (n = read(STDIN_FILENO, buf, MAXLINE)) > 0)
		{
			buf[n] = 0;
			len = strlen(buf);
			if ( (n = write(fds1[1], buf, len)) < 0)
			{
				err_sys("write error", errno);
			}
			if ( (n = read(fds2[0], buf, MAXLINE)) < 0)
			{
				err_sys("read error", errno);
			}
			buf[n] = '\0';
			len = strlen(buf);
			if ( (n = write(STDOUT_FILENO, buf, len)) < 0 )
			{
				err_sys("write error", errno);
			}
		}
		close(fds1[1]);
		close(fds2[0]);
		waitpid(pid, NULL, 0);
	}	

	return 0;
}


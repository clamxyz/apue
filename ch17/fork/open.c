#include "open.h"
#include <fcntl.h>

#define BUFFSIZE 8192

extern int recv_fd(int servfd, ssize_t (*userfunc)(int, const void *, size_t));

int main(int argc, char *argv[])
{
	int n, fd;
	char buf[BUFFSIZE], line[MAXLINE];

	while(fgets(line, MAXLINE, stdin) != NULL)
	{
		line[strlen(line) - 1] = '\0';

		if ( (fd = csopen(line, O_RDONLY)) < 0)
			continue;
		printf("open fd success\n");
		while ((n = read(fd, buf, BUFFSIZE)) > 0)
			if (write(STDOUT_FILENO, buf, n) != n)
				err_sys("write error", errno);
		if (n < 0)
			err_sys("write error", errno);
		close(fd);
	}

	return 0;
}

int csopen(char *name, int oflag)
{
	pid_t pid;
	int len;
	char buf[10];
	struct iovec iov[3];
	static int fd[2] = {-1, -1};

	if (fd[0] < 0)
	{
		if (pipe(fd) < 0)
			err_sys("pipe error", errno);
		if ( (pid = fork()) < 0)
			err_sys("fork error", errno);
		else if (pid == 0)
		{
			close(fd[0]);
			if (fd[1] != STDIN_FILENO)
				if (dup2(fd[1], STDIN_FILENO) < 0)
					err_sys("dup2 error to stdin", errno);
			if (fd[1] != STDOUT_FILENO)
				if (dup2(fd[1], STDOUT_FILENO) < 0)
					err_sys("dup2 error to stdout", errno);
			close(fd[1]);
			if ( execl("./opend", "opend", (void *)0) < 0)
				err_sys("execl error", errno);
		}
		close(fd[1]);
	}
	sprintf(buf, " %d", oflag);
	iov[0].iov_base = CL_OPEN " ";
	iov[0].iov_len = strlen(CL_OPEN) + 1;
	iov[1].iov_base = name;
	iov[1].iov_len = strlen(name) + 1;
	iov[2].iov_base = buf;
	iov[2].iov_len = strlen(buf) + 1;
	len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
	if (writev(fd[0], iov, 3) < 0)
		err_sys("writev error", errno);

	return (recv_fd(fd[0], write));
}


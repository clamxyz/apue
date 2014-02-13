#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

extern int errno;

static void TELL_WAIT();
static void TELL_CHILD(pid_t pid);
static void TELL_PARENT(pid_t pid);
static void WAIT_CHILD();
static void WAIT_PARENT();

static sigset_t newmask, oldmask, zeromask;
volatile sig_atomic_t sigflag;

int main(int argc, char *argv[])
{
	char filename[] = "ipc.txt";
	int num = 0;
	int fd = -1;
	pid_t pid;

	fd = open(filename, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd == -1)
	{
		printf("open %s failed[%d][%s]\n", filename, errno, strerror(errno));
		return -1;
	}
	if (sizeof(num) != write(fd, &num, sizeof(int)))
	{
		printf("write error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
#ifdef TEST
	if (fsync(fd) < 0)
	{
		perror("fsysnc");
		return -1;
	}
#endif 
	TELL_WAIT();
	pid = fork();
	if (pid < 0)
	{
		printf("fork error[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	else if (pid == 0)
	{
			/*child process*/
		int i = 0;
			while(i++ < 10)
			{

					WAIT_PARENT();
					lseek(fd, 0, SEEK_SET);
					if (read(fd, &num, sizeof(int)) <= 0)
					{
							printf("child: read error[%d][%s]\n", errno, strerror(errno));
							return -1;
					}	
					num += 1;
					lseek(fd, 0, SEEK_SET);
					if (write(fd, &num, sizeof(int)) <= 0)
					{
						printf("child:write error[%d][%s]\n", errno, strerror(errno));
						return -1;
					}
					printf("child:write num = %08d success.\n", num);
					TELL_PARENT(getppid());
			}
	}
	else 
	{
		/*parent process*/
		int i = 0;
		int status;
		
		while(i++ < 10)
		{
			lseek(fd, 0, SEEK_SET);
			if (read(fd, &num, sizeof(int)) <= 0)
			{
				printf("parent:read error[%d][%s]\n", errno, strerror(errno));
				return -1;
			}
			num += 1;
			lseek(fd, 0, SEEK_SET);
			if (write(fd, &num, sizeof(int)) <= 0)
			{
				printf("parent:write error[%d][%s]\n", errno, strerror(errno));
				return -1;
			}
			printf("parent:write num = %08d success.\n", num);
			TELL_CHILD(pid);
			WAIT_CHILD();	
		}

		waitpid(pid, &status, 0);
	}

	if (fd != -1)
		close(fd);
	
	return 0;
}

void sig_usr(int signo)
{
	sigflag = 1;
}

static void TELL_CHILD(pid_t pid)
{
	kill(pid, SIGUSR1);	
}

static void TELL_PARENT(pid_t pid)
{
	kill(pid, SIGUSR2);
}

static void TELL_WAIT()
{
	struct sigaction act;
	
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	act.sa_handler = sig_usr;

	if (sigaction(SIGUSR1, &act, NULL) < 0)
	{
		perror("SIGUSR1");
		exit(-1);
	}
	if (sigaction(SIGUSR2, &act, NULL) < 0)
	{
		perror("SIGUSR1");
		exit(-1);
	}
	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	sigaddset(&newmask, SIGUSR2);
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
	{
		perror("sigprocmask SIG_BLOCK");
		exit(-1);
	}
}

static void WAIT_PARENT()
{
	while(sigflag == 0)
	{
		sigsuspend(&zeromask);
	}
	sigflag = 0;
	
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

static void WAIT_CHILD()
{
	while(sigflag == 0)
		sigsuspend(&zeromask);
	sigflag = 0;

	sigprocmask(SIG_SETMASK, &oldmask, NULL);
}


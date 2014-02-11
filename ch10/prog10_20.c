#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int l_system(const char *pszCmd);
static void sig_intr(int signo);
static void sig_quit(int signo);
static void sig_child(int signo);

int main(int argc, char *argv[])
{
	if (signal(SIGINT, sig_intr) == SIG_ERR)
	{
		perror("signal");
		return -1;
	}
	if (signal(SIGQUIT, sig_quit) == SIG_ERR)
	{
		perror("signal SIGQUIT");
		return -1;
	}
	if (signal(SIGCHLD, sig_child) == SIG_ERR)
	{
		perror("signal SIGICHLD");
		return -1;
	}
	printf("parent pid = %d\n", getpid());
	l_system("/bin/ed");

	return 0;
}

static void sig_intr(int signo)
{
	printf("in sig_intr:\n");
}

static void sig_child(int signo)
{
	int status;

	wait(&status);

	printf("in sig_child:pid = %d\n", getpid());
}

static void sig_quit(int signo)
{
	printf("in sig_quit:\n");
}

int l_system(const char *pszCmd)
{
	pid_t pid;
	int status;
	struct sigaction ignore, saveintr, savequit;
	sigset_t chldmask, savemask;

	if (pszCmd == NULL)
		return -1;
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	if (sigaction(SIGINT, &ignore, &saveintr) < 0)
		return -1;
	if (sigaction(SIGQUIT, &ignore, &savequit) < 0)
		return -1;
	
	sigemptyset(&chldmask);
	sigaddset(&chldmask, SIGCHLD);
	if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0)
		return -1;
	if ((pid = fork()) < 0)
	{
		status = -1;
	}else if (pid == 0)
	{
		sigaction(SIGINT, &saveintr, NULL);
		sigaction(SIGQUIT, &savequit, NULL);
		sigprocmask(SIG_SETMASK, &savemask, NULL);

		execl("/bin/sh", "sh", "-c", pszCmd, (char *)0);
		_exit(127);
	}	
	else 
	{
		while(waitpid(pid, &status, 0) < 0)
		{
			if (errno != EINTR){
				status = -1;
				break;
			}
		}
	}
	if (sigaction(SIGINT, &saveintr, NULL) < 0)
		return -1;
	if (sigaction(SIGQUIT, &savequit, NULL) < 0)
		return -1;
	if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0)
		return -1;
	return status;
}


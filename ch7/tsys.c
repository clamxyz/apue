#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

extern int errno;

int _system(char *pszCmd)
{
	pid_t pid;
	int status;	

	if (pszCmd == NULL)
		return 1;
	if ((pid = fork()) < 0)
	{
		perror("fork error.");
		return -1;
	}		
	else if (pid == 0)
	{
		execlp("/bin/sh", "sh", "-c", pszCmd, (char *)0);
		perror("execlp error");
		_exit(127);
	}
	else 
	{
		while(waitpid(pid, &status, 0) < 0)
		{
			if(errno == EINTR)
			{
				status = -1;
				break;
			}
		}
	}

	return status;
}

int main(int argc, char *argv[])
{
	int status;

	if ((status = _system("date")) < 0)
	{
		perror("_system(\"date\")");
		exit(-1);
	}	
	if ((status = _system("nosuchcommand")) < 0)
	{
		perror("_system(nosuchcommand)");
		exit(-1);
	}
	if ((status = _system("who; exit 44")) < 0)
	{
		perror("_system(who;exit 44)");
		exit(-1);
	}

	return 0;	
}


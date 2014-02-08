#include "../public.h"
#include <pwd.h>
#include <signal.h>

static void sig_alarm(int signo);

int main(int argc, char *argv[])
{
	struct passwd *ptr;

	if (signal(SIGALRM, sig_alarm) == SIG_ERR)
	{
		perror("signal error");
		exit(-1);
	}
	alarm(1);
	while(1)
	{
		ptr = getpwnam("study");
		if (ptr == NULL)
		{
			perror("getpwnam study error");
			exit(-1);
		}
		if (strcmp(ptr->pw_name, "study") != 0)
		{
			printf("return value interrupt.name = %s\n", ptr->pw_name);
			exit(-1);	
		}
	}	
	return 0;		
}

static void sig_alarm(int signo)
{
	struct passwd *rootptr;
	printf("In sig_alarm\n");
	rootptr = getpwnam("root");
	if (rootptr == NULL)
	{
		perror("getpwnam root error");		
		exit(-1);
	}
	alarm(1);
}


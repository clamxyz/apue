#include "public.h"

#include <sys/wait.h>

int main()
{
	pid_t pid;
	#define BUFFSIZE	1024
	char buf[BUFFSIZE];
	int status;
	
	printf("%%");
	while(NULL != fgets(buf, BUFFSIZE, stdin))
	{
		buf[strlen(buf) - 1] = 0;
		pid = fork();
		if (pid < 0)
		{
			printf("fork error.[%d][%s]\n", errno, strerror(errno));
			return EXIT_FAILURE;
		}
		
		if (pid == 0)
		{
			/*child process*/
			execlp(buf, buf, (char *)0);
			printf("cannot execute %s.errno[%d][%s].\n", buf, errno, strerror(errno));
		}
		waitpid(pid, &status, 0);
		printf("process %d exit with %d\n", pid, status);
		printf("%%");
	}
	return EXIT_SUCCESS;	
}


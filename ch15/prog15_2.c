#include "public.h"

#include <sys/wait.h>

#define PAGER_DFL "/bin/more"

int main(int argc, char *argv[])
{
	int pfds[2];
	pid_t pid;
	FILE *fp;

	if (argc != 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return -1;
	}

	if (pipe(pfds) < 0)
	{
		err_sys("fork error", errno);
	}
	
	if ( (fp = fopen(argv[1], "r")) == NULL)
		err_sys("fopen error", errno);

	if ( (pid = fork()) < 0)
	{
		err_sys("fork error", errno);
	}
	else if (pid == 0)
	{
		char *ppager;
		char *pagv;

		close(pfds[1]);
		if (pfds[0] != STDIN_FILENO)
		{
			if (dup2(pfds[0], STDIN_FILENO) != STDIN_FILENO)
			{
				err_sys("dup2 error", errno);
			}
			close(pfds[0]);
		}
		if ( (ppager = getenv("PAGER")) == NULL)
		{
			ppager = PAGER_DFL;
		}
		if ( (pagv = strrchr(ppager, '/')) != NULL)
		{
			pagv++;	
		}	
		execl(ppager, pagv, (void *)0);
		printf("execl ppager error\n");
		_exit(127);
	}
	else 
	{
		char buf[1024 + 1];
		int len;	
		close(pfds[0]);
		while(fgets(buf,sizeof buf, fp) != NULL)
		{
			len = strlen(buf); 
			write(pfds[1], buf, len);
		}
		close(pfds[1]);
		waitpid(pid, NULL, 0);
	}	
		
	return 0;
}


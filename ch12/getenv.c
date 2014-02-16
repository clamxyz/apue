#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>

extern char **environ;
#define ARG_MAX 10240
static char envbuf[ARG_MAX];

char *l_getenv(const char *name)
{
	char **envp = environ;
	int len;

	len = strlen(name);
	while(envp++ && len)
	{
		if (strncmp(*envp, name, len) == 0 && (*envp)[len] == '=')
		{
			strcpy(envbuf, *envp);
			return envbuf;
		}
	}
	return NULL;			
}

int main(int argc, char *argv[])
{
	char *buf;
	
	buf = l_getenv("HOME");
	printf("env is %s\n", buf == NULL ? "null" : buf);
	return 0;
}


#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main(void)
{
	void *p;
	printf("environ[0] = %s\n", environ[0]);
	p = realloc(environ[0], 10240);
	return 0;
}

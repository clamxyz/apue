#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[])
{
	char **ptr = environ;

	while(*ptr != NULL)
	{
		printf("%s\n", *ptr++);
	}
	return 0;
}


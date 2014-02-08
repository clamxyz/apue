#include "../public.h"

int main(int argc, char *argv[])
{
	char **ptr;
	extern char **environ;
	int i;

	for (i = 0;i < argc; i++)
	{
		printf("argv[%d]=%s\n", i, argv[i]);
	}
	for (ptr = environ; *ptr != (char *)0; ptr++)
	{
		printf("%s\n", *ptr);
	}

	return 0;
}


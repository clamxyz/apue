#include <stdio.h>

#define MAXLINE	1024

int main(int argc, char *argv[])
{
	char file[L_tmpnam];
	char line[MAXLINE];
	FILE *fp;

	printf("%s\n", tmpnam(NULL));
	tmpnam(file);
	printf("%s\n", file);
	if ((fp = tmpfile()) == NULL)
	{
		perror("tmpfile");
		return -1;
	}
	fclose(fp);
	return 0;
}


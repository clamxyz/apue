#include <stdio.h>
/**
 * 打印标准ＩＯ流的状态
 */
static void pr_stdio(char *name, FILE *fp);
int main(int argc, char *argv[])
{
	FILE *fp;

	printf("enter any character:");
	if (getchar() == EOF)
	{
		perror("getchar");
		return -1;
	}
	fputs("line from stderr.\n", stderr);

	pr_stdio("stdin", stdin);
	pr_stdio("stdout", stdout);
	pr_stdio("stderr", stderr);

	fp = fopen("stdio.c", "r");
	if (fp == NULL)
	{
		perror("fopen");
	}
	pr_stdio("fp", fp);
	fclose(fp);
	return 0;
}


static void pr_stdio(char *name, FILE *fp)
{
	printf("stream=%s, ", name);
	if (fp->_flags & _IOLBUF) printf("line buffer,");
	else if (fp->_flags & _IONBUF) printf("no buffer,");
	else printf("all buffer,");
	printf("buffer size=%d\n", fp->_bufsize);
}


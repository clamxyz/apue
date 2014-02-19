#include "public.h"

int main()
{
	char buf[MAXLINE];
	int n;
	int a, b;
	int len;

	while( (n = read(STDIN_FILENO, buf, MAXLINE)) > 0)
	{
		buf[n] = '\0';
		sscanf(buf, "%d%d", &a, &b);
		sprintf(buf, "%d\n", a + b);
		len = strlen(buf);
		if ( (n = write(STDOUT_FILENO, buf, len)) != len)
		{
			err_sys("write error", errno);
		}
	}
	return 0;
}


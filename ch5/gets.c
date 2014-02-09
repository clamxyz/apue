#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/**
 * fgets当MAX_LINE不足够存储一行时，会分多次返回。因此如果要确保fgets每次都返回一行，那么
 * 需要给出　足够的缓冲区　或者判断　最后一个字符是否为'\n'
 */
#define MAX_LINE	4

int main(int argc, char *argv[])
{
	char line[MAX_LINE + 1];
	char *ptr ;
	int len = 0;

	while(fgets(line, MAX_LINE,stdin) != NULL)
	{
		if (fputs(line,stdout) == EOF)
		{
			perror("fputs");
			return -1;
		}
	}
	if (ferror(stdin))
		printf("input error\n");
	return 0;
}


#include "../public.h"
#include <sys/types.h>
#include <sys/stat.h>

static void usage(void)
{
	printf("Usage: lstat <file1> <file2> ...\n");
}

int main(int argc, char *argv[])
{
	int i;
	struct stat buf;
	char *ptr;

	if (argc < 2)
	{
		usage();
		return EXIT_SUCCESS;
	}
	for(i = 1; i < argc; i++)
	{
		if (-1 == lstat(argv[i], &buf))
		{
			perror(argv[i]);
			continue;
		}
		if (S_ISREG(buf.st_mode)) ptr = "regular file";
		else if (S_ISBLK(buf.st_mode)) ptr = "block special file";
		else if (S_ISCHR(buf.st_mode)) ptr = "char special file";
		else if (S_ISLNK(buf.st_mode)) ptr = "symbol link file";
		else if (S_ISSOCK(buf.st_mode)) ptr = "socket file";
		else if (S_ISFIFO(buf.st_mode)) ptr = "fifo";
		else if (S_ISDIR(buf.st_mode)) ptr = "directory";
		else ptr = "unknow file type";

		printf("%s:%s", argv[i], ptr);
		if ((S_ISUID & buf.st_mode) == S_ISUID) printf(" uid-setter");
		if ((S_ISGID & buf.st_mode) == S_ISGID) printf(" gid-setter");
		printf("\n");
	}

	return EXIT_SUCCESS;	
}


#include "../public.h"
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int i;
	struct stat statbuf;

	if (argc < 2)
	{
		printf("usage:version <file1> <file2>\n");
		return 0;
	}
	for (i = 1;i < argc; i++)
	{
		if (lstat(argv[i], &statbuf) < 0)
		{
			perror(argv[i]);
			continue;
		}
		printf("%s:%d/%d\n", "dev", major(statbuf.st_dev), minor(statbuf.st_dev));
		if (S_ISBLK(statbuf.st_mode) || S_ISCHR(statbuf.st_mode))
		{
			printf("%s:%d/%d\n", S_ISBLK(statbuf.st_mode) ? "block" : "character", major(statbuf.st_rdev), minor(statbuf.st_rdev));
		}	
	}
	
	return 0;	
}

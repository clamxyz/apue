#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
	DIR *dp;
	struct dirent *pDirEntry;
	if (argc == 1)
	{
		printf("Usage: myls <dir1>\n");
	}
	dp = opendir(argv[1]);
	if (dp == NULL)
	{
		printf("Cannot open directory[%s].[%d][%s]\n", argv[1], errno, strerror(errno));
		return EXIT_FAILURE;
	}
	while (NULL != (pDirEntry = readdir(dp)))
	{
		printf("%s\n", pDirEntry->d_name);
	}		
	closedir(dp);

	return EXIT_SUCCESS;	
}


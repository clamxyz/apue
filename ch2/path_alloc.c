#include "../public.h"
#include <limits.h>
#include <assert.h>

static char *path_alloc(void);

int main(int argc, char *argv[])
{
	free(path_alloc());		
	return 0;
}

static char *path_alloc(void)
{
#ifdef PATH_MAX
	int path_max = PATH_MAX;
#else
	int path_max = 0;
#endif
#define PATH_MAX_GUESS	1024
	char *pszPath;
	
	if (path_max == 0)
	{
		errno = 0;
		path_max = pathconf("/", _PC_PATH_MAX);
		if (path_max < 0)
		{
			if (errno == 0) path_max = PATH_MAX_GUESS;
			else printf("pathconf error[%d][%s]\n", errno, strerror(errno));
		}
		else path_max++; /*for "/" */
	}	
	pszPath = (char *) malloc(path_max + 1);
	if (pszPath == NULL)
	{
		printf("%s:malloc failed[%d][%s].\n", __FILE__,errno, strerror(errno));
		return NULL;
	}
	printf("path_max:%d\n", path_max);
	return pszPath;
}

static void path_free(char *pszPath)
{
	assert(pszPath);
	free(pszPath);
}


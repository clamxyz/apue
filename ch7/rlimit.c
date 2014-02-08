/*
 * 打印资源限制
 */
#include "../public.h"
#include <sys/time.h>
#include <sys/resource.h>

#define doit(name) pr_limits(#name, name);
static void pr_limits(char *name, int id);

int main(int argc, char *argv[])
{
	doit(RLIMIT_AS);
	doit(RLIMIT_CORE);
	doit(RLIMIT_CPU);
	return 0;
}

static void pr_limits(char *name, int id)
{
	struct rlimit rlim;
	if (getrlimit(id, &rlim) < 0)
	{
		printf("getrlimit error[%d][%s]\n", errno, strerror(errno));
		return;
	}		
	printf("%s\t", name);
	if (rlim.rlim_cur == RLIM_INFINITY)
	{
		printf("soft(INFINITY)\t");	
	}
	else printf("soft(%ld)\t", rlim.rlim_cur);

	if (rlim.rlim_max == RLIM_INFINITY)
		printf("hard(INFINITY)\n");
	else
		printf("hard(%ld)\n", rlim.rlim_max);
	
}


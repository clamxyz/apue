#include "../public.h"

int main(int argc, char *argv[])
{
	if (chdir("/tmp") != 0)
	{
		printf("chdir failed[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
	printf("chdir successed.\n");
	return 0;
}


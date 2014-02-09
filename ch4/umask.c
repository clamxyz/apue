#include "../public.h"

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	umask(0);
	if (creat("foo", S_IRWXU | S_IRWXG | S_IRWXO))
		printf("create foo failed.\n");
	umask(S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH);
	if (creat("bar", S_IRWXU | S_IRWXG | S_IRWXO))
		printf("creat bar failed.\n");
	return 0;	
}


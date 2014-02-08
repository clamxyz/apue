#include "../public.h"

int main(int argc, char *argv[])
{
	printf("uid = %d, euid = %d\n", getuid(), geteuid());
	return 0;
}


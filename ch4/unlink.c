#include "../public.h"

#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	if (open("tempfile", O_RDWR) < 0)
			perror("open tempfile error");
	if (unlink("tempfile"))
			perror("unlink tempfile error");
	return 0;
}


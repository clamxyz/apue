#include "../public.h"

/**
  * access:按实际用户组id和用户Id来判断文件存取权限
  * open:按有效组Id和有效用户Id来判断文件存取权限
  */
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	char *ptr;
	if (argc != 2)
	{
		printf("usage: ./access <file>\n");
		return EXIT_SUCCESS;
	}
	if (access(argv[1], F_OK) == 0) printf("exists ");
	if (access(argv[1], R_OK) == 0) printf("read only ");
	if (access(argv[1], W_OK) == 0) printf("write only ");
	if (access(argv[1], X_OK) == 0) printf("execute ");
	printf("\n");

	return EXIT_SUCCESS;
}


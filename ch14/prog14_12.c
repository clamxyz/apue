#include "public.h"
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	void *paddr;
	char buf[] = "Hello, World!\n";
	int fd;
	struct stat statbuf;

	fd = open("map.foo", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd < 0)
	{
		perror("open map.foo failed.");
		return -1;
	}
	
	if (fstat(fd, &statbuf) < 0)
	{
		perror("fstat error");	
		return -1;
	}

	printf("file length is %d\n", statbuf.st_size);
	ftruncate(fd, sizeof buf );

	paddr = mmap(0, sizeof buf , PROT_WRITE, MAP_SHARED, fd, 0);
	memcpy(paddr, buf, sizeof buf + 1);
	msync(paddr, sizeof buf , MS_SYNC); 
	munmap(paddr, sizeof buf);
	close(fd);

	return 0;
}


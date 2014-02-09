#include "../public.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

typedef int pfnVistor(const char *, const struct stat *, int );
static pfnVistor vistor;
static long nreg, ndir, nblk, nchr, nfifo, nsock, nslink, ntot;

static int myftw(const char *pathname, pfnVistor vistor);
static int dopath(pfnVistor vistor);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("usage: ftw <path>\n");
		return 0;
	}
	if (myftw(argv[1], vistor))
		printf("myftw failed.\n");
	ntot = nreg + ndir + nblk + nchr + nfifo + nsock + nslink;
	if (ntot == 0)
		ntot = 1;
	printf("regulare file	:%7ld %5.2f %%\n", nreg, nreg * 100.0 / ntot);
	printf("direcotries	:%7ld %5.2f %%\n", ndir, ndir * 100.0 / ntot);
	printf("block specail	:%7ld %5.2f %%\n", nblk, nblk * 100.0 / ntot);
	printf("char specail	:%7ld %5.2f %%\n", nchr, nchr * 100.0 / ntot);
	printf("fifo		:%7ld %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
	printf("sock		:%7ld %5.2f %%\n", nsock, nsock * 100.0 / ntot);
	printf("link file	:%7ld %5.2f %%\n", nslink, nslink * 100.0 / ntot);

	return 0;
}
static char *fullpath;

static int myftw(const char *pathname, pfnVistor vistor)
{
	fullpath = (char *)malloc(4096);

	strcpy(fullpath, pathname);

	return dopath(vistor);
}

#define FTW_F	1		/*regulare file*/
#define FTW_D	2		/*direcotry*/
#define FTW_DNR	3		/*directory cannot read*/
#define FTW_NS	4		/*cannot not stat*/
static int dopath(pfnVistor vistor)
{
	int ret;
	struct stat buf;
	char *ptr;
	struct dirent *pdir;
	DIR	*dp;

	if ((ret = lstat(fullpath, &buf)) < 0)
	{
		return vistor(fullpath, &buf, FTW_NS);	
	}
	if (S_ISDIR(buf.st_mode) == 0)
		return vistor(fullpath, &buf, FTW_F);
	
	if ((ret = vistor(fullpath, &buf, FTW_D)) != 0)
		return ret;
	/*it's direcotry*/
	ptr = fullpath + strlen(fullpath);
	*ptr++ = '/';
	*ptr= 0;	
	dp = opendir(fullpath);
	if (dp == NULL)
		return vistor(fullpath, &buf, FTW_DNR);
	while ((pdir = readdir(dp)) != NULL)
	{
		if (strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0)
		{
			continue;
		}
		strcpy(ptr, pdir->d_name);
		if (dopath(vistor) != 0)
			break;
	}
/*ptr[-1] = 0;*/
	closedir(dp);
	return ret;
}

static int vistor(const char *pathname, const struct stat *statbuf, int level)
{
	/*	printf("%s\n", pathname); */
	switch(level)
	{
		case FTW_F:
				if (S_ISREG(statbuf->st_mode)) nreg++;
				else if (S_ISBLK(statbuf->st_mode)) nblk++;
				else if (S_ISCHR(statbuf->st_mode)) nchr++;
				else if (S_ISSOCK(statbuf->st_mode)) nsock++;
				else if (S_ISFIFO(statbuf->st_mode)) nfifo++;
				else if (S_ISLNK(statbuf->st_mode)) nslink++;
				else printf("unknown file type.\n");
				break;
		case FTW_D:
				ndir++;
				break;
		case FTW_NS:
				printf("%s:stat failed.\n", pathname);
				break;
		case FTW_DNR:
				printf("%s:direcotry read failed.\n", pathname);
				break;
		default:
				printf("unknown file.\n");
	}
	return 0;
}


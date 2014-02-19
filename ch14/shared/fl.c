#include <unistd.h>
#include <fcntl.h>
#ifdef DEBUG
#include <string.h>
#endif

#include "fl.h"

int set_fl(int fd, int flag)
{
	int oflag;

	oflag = fcntl(fd, F_GETFL, 0);
	if (oflag < 0)
	{
#ifdef DEBUG
		perror("fcntl F_GETFL failed");
#endif
		return -1;
	}
	oflag |= flag;

	oflag = fcntl(fd, F_SETFL, oflag);

	if (oflag != 0)
	{
#ifdef DEBUG
		perror("fcntl F_SETFL failed.");
#endif
		return -1;
	}

	return 0;
}

int clr_fl(int fd, int flag)
{
	int oflag;

	oflag = fcntl(fd, F_GETFL, 0);
	if (oflag < 0)
	{
#ifdef DEBUG
		perror("fcntl F_GETFL failed");
#endif
		return -1;
	}

	oflag &= ~flag;
   if (fcntl(fd, F_SETFL, oflag) < 0)
   {
#ifdef DEBUG
		perror("fcntl F_SETFL failed");
#endif
		return -1;
   }

	return 0;
}



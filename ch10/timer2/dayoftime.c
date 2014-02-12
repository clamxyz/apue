#include <sys/time.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct timeval tv;
	struct timezone tz;
	
	gettimeofday(&tv, &tz);
	printf("tv{sec=%08ld, usec=%08ld\n", tv.tv_sec, tv.tv_usec);
	printf("tz{tz_minuteswest = %08d, tz_dsttime = %08d\n", tz.tz_minuteswest, tz.tz_dsttime);
	return 0;
}


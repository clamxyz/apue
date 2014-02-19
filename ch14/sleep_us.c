#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>

#define USECTOSEC	1000000

int sleep_us(int us)
{
	struct timeval val;
	val.tv_sec = us / USECTOSEC;
	val.tv_usec = us % USECTOSEC;

	return select(0, NULL, NULL, NULL, &val);
}

unsigned long caltv(struct timeval *pstv, struct timeval *petv)
{
	struct timeval result;
	result.tv_sec = petv->tv_sec - pstv->tv_sec;
	result.tv_usec = petv->tv_usec - pstv->tv_usec;
	
	return result.tv_sec * USECTOSEC + result.tv_usec;
}

void pr_tv(struct timeval *ptv)
{
	printf("tv_sec = %ld, tv_usec = %ld\n", ptv->tv_sec, ptv->tv_usec);
}

void cal_avg()
{	
	struct timeval stv, etv;
	int i ;
	long sum;
	double avg;

	for (i = 0, sum = 0;i < 100000;i++)
	{
	gettimeofday(&stv, NULL);
	sleep_us(1000 * 10);
	gettimeofday(&etv, NULL);
	sum += caltv(&stv, &etv);
	}
	avg = (double)sum/i;
	printf("AVG is %lf\n", avg);	
}

int main()
{
	struct timeval stv, etv;

	cal_avg();
	printf("Start Main:\n");
	gettimeofday(&stv, NULL);
	pr_tv(&stv);
	sleep_us(1000 * 10);
	gettimeofday(&etv, NULL);
	pr_tv(&etv);
	printf("Elapsed Time:%ld\n", caltv(&stv, &etv));
	printf("End Main:\n");

	return 0;
}


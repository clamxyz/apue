#include <sys/times.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct tms starttms, endtms;
	clock_t clocks;
	int clk_per_sec;
	int i;	
	clk_per_sec = sysconf(_SC_CLK_TCK);
	if (clk_per_sec < 0)
	{
		perror("sysconf error.");
		return -1;
	}
	clocks = times(&starttms);
	if (clocks == (clock_t)-1)
	{
		perror("times error.");
		return -1;
	}
	for(i = 0; i < 10; i++)
	system("date");
	clocks = times(&endtms);
	if (clocks == (clock_t)-1)
	{
		perror("times error.");
		return -1;
	}
	printf("user time:		%f\n", (endtms.tms_utime - starttms.tms_utime) * 1.0 / clk_per_sec);
	printf("system time:		%f\n", (endtms.tms_stime - starttms.tms_stime) * 1.0 / clk_per_sec);
	printf("child user time:	%f\n", (endtms.tms_cutime - starttms.tms_cutime) * 1.0 / clk_per_sec);
	printf("child system time:	%f\n", (endtms.tms_cstime - starttms.tms_cstime) * 1.0 / clk_per_sec);

	return 0;
}


/**
 * 带时间限制的read
 * 经测试发现，signal对于read使用的是sa_restart标志。
 * 执行结果如下：
 * ./prog10_7
 * >>read..
 * >>(after 10 secs)in sig_alarm
 * >>read from console continue
 * >>
 */

#include "../public.h"
#include <signal.h>

static void sig_alarm(int signo)
{
	printf("in sig_alarm\n");
	return;
}

int main(int argc, char *argv[])
{
	char buf[10] = {0};
	if (signal(SIGALRM, sig_alarm) == SIG_ERR)
	{
		perror("signal SIGALRM failed");
		return -1;
	}

	printf("read..\n");
	alarm(10);
	if (read(STDIN_FILENO, buf, 10) < 0)
	{
		puts("read timeout");
	}
	alarm(0);
	printf("read:%s\n", buf);
	return 0;
}


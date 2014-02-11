#include "../public.h"

#include <signal.h>
#include <setjmp.h>

static jmp_buf jmpbuf;

static void sig_alarm(int signo);
static int sleep2(int nsecs);

int main(int argc, char *argv[])
{
	printf("start..\n");
	sleep2(2);
	puts("end...");	
	return 0;
}

static void sig_alarm(int signo)
{
	longjmp(jmpbuf, 1);
}

static int sleep2(int nsecs)
{
	if (signal(SIGALRM, sig_alarm) == SIG_ERR)
	{
		return nsecs;
	}
	/*
	 * 竞态条件并没有解决，如果在signal和setjmp之间发生了SIGALRM信号，
	 * 那么系统就会崩溃，产生段错误。
	alarm(1);
	pause();
	*/
	if (setjmp(jmpbuf) == 0)
	{
		alarm(nsecs);
		pause();
	}
	return alarm(0);
}



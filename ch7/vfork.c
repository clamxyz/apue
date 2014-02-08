#include "../public.h"

static void f1(void)
{
	if(vfork() < 0)
		perror("vfork error");
}

static void f2(void)
{
	char buf[1000];
	int i =0;
	for (i = 0; i < 1000; i++)
			buf[i] = 0;
}

int main()
{
	f1();
	f2();
	_exit(0);
}


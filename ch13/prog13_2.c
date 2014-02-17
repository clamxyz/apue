#include <stdio.h>
#include <unistd.h>

extern int already_running();

int main()
{
	if (already_running())
	{
		printf("already running\n");
		return -1;
	}
	else
		printf("does't running");
	sleep(100);
	return 0;
}


#include "public.h"

int main()
{
	FILE *fpin;
	char line[1024];
		
	fpin = popen("./toupper", "r");
	
	if (fpin == NULL)
	{
		err_sys("popen failed", errno);
	}
	setlinebuf(fpin);
	setlinebuf(stdout);
	while(fgets(line, 1024, fpin) != NULL)
	{
		fputs(line, stdout);
	}
		
	pclose(fpin);
	return 0;
}


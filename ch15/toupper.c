#include "public.h"
#include <ctype.h>

int main()
{
	int c;

	while( (c = getchar()) != EOF)
	{
		if (isalpha(c))
		{
			c = toupper(c);	
		}
		putchar(c);
		if (c == '\n')
			fflush(stdout);
	}

	return 0;
}


#include "../public.h"

static void atexit1(void);
static void atexit2(void);
static void atexit3(void);

int main(int argc, char *argv[])
{
	atexit(atexit1);
	atexit(atexit2);
	atexit(atexit3);

	return 0;
}

static void atexit1(void)
{
	printf("atexit1\n");
}

static void atexit2(void)
{
	printf("atexit2\n");
}

static void atexit3(void)
{
	printf("atexit3\n");
}


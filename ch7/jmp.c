/*
 * setjmp/longjmp函数不保证保存自动变量和寄存器变量
 * 调用longjmp时不保证回滚所有自动变量和寄存器变量的值
 * 但是volatile和全局变量、静态变量的值会被修改，
 * 不回滚
 */
#include "../public.h"

#include <setjmp.h>

static void f1(int i, int j, int k);
static void f2(void);

static jmp_buf env;

int main(int argc, char *argv[])
{
	int count;
	register int val;
	volatile int sum;

	count = 2;
	val = 3;
	sum = 4;

	if (setjmp(env) != 0)
	{
		printf("after longjmp:count = %d, val = %d, sum = %d\n", count, val, sum);
		return 0;
	}
	count = 97;
	val = 98;
	sum = 99;
	f1(count, val, sum);

	return 0;
}

static void f1(int i, int j, int k)
{
	printf("IN F1:count = %d, val = %d, sum = %d\n", i, j, k);
	f2();
}

static void f2(void)
{
	longjmp(env, 1);
}


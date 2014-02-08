/**
 * argv参数要求被实现为以NULL为结尾的字符串数组
 * 本程序就是要测试该功能是否实现
 * 测试结果：
 * 以下程序可以打印出程序的所有参数
 */
#include "../public.h"

int main(int argc, char *argv[])
{
	while(*argv != NULL)
	{
		printf("%s\n", *argv++);
	}
	return 0;
}


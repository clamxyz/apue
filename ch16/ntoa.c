#include "public.h"

#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	struct in_addr addr;
	char buf[INET_ADDRSTRLEN];

	inet_pton(AF_INET, "192.168.8.192", &addr);
	printf("192.168.8.192 = %s\n", inet_ntop(AF_INET, &addr, buf, INET_ADDRSTRLEN));

	return 0;
}


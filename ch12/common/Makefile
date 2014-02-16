all:
	gcc -Wall -fPIC -c makethread.c mkt.h
	gcc -Wall -fPIC -lpthread -shared -o libmkt.so makethread.o
	mv libmkt.so ../

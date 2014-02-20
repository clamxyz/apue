all:
	gcc -Wall -c -fPIC mysock.c mysock.h
	gcc -Wall -fPIC -shared -o libmysock.so mysock.o
	cp libmysock.so ../
	cp mysock.h ../


all:
	gcc -Wall -fPIC -c fl.c lock.c rw.c
	gcc -Wall -fPIC -shared -o libfl.so fl.o lock.o rw.o
	cp libfl.so ../.
	cp fl.h ../.



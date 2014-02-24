SRCS=$(wildcard *.c)
BIN=open

all:$(BIN)

$(BIN):open.c open.h 
	gcc -Wall -DDEBUG -I${APUE_HOME}/include/ -g -o $@ $(filter $@.c, $^)  ./libfl.so recv.o

install:
	./mvbin.sh
clean:
	rm -rf $(BIN)

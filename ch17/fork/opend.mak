SRCS=$(wildcard *.c)
BIN=opend

all:$(BIN)

$(BIN):opend.c opend.h 
	gcc -Wall -DDEBUG -I${APUE_HOME}/include/ -g -o $@ $(filter $@.c, $^)  ./libfl.so sendxxx.o

install:
	./mvbin.sh
clean:
	rm -rf $(BIN)

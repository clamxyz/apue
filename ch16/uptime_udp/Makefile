SRCS=$(wildcard *.c)
BIN=$(SRCS:.c=)

all:$(BIN)

$(BIN):$(SRCS)
	gcc -Wall -DDEBUG -Iinclude/ -I../include/ -L../lib/  -ldaemon -g -o $@ $(filter $@.c, $^)  ./libmysock.so

install:
	./mvbin.sh
clean:
	rm -rf $(BIN)

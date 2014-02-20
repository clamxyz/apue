SRCS=$(wildcard *.c)
BIN=$(SRCS:.c=)

all:$(BIN)

$(BIN):$(SRCS)
	gcc -Wall -DDEBUG -Iinclude/ -I../../include/ -g -o $@ $(filter $@.c, $^)  ./libmysock.so ./libdaemon.so

install:
	./mvbin.sh
clean:
	rm -rf $(BIN)

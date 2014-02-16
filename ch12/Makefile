SRCS=$(wildcard *.c)
BIN=$(SRCS:.c=)
all:$(BIN)

$(BIN):$(SRCS)
	gcc -Wall -DDEBUG -g -L. -pthread -lmkt -pthread -o $@ $(filter $@.c, $^)  mkt.h ./libmkt.so

install:
	./mvbin.sh
clean:
	rm -rf $(BIN)

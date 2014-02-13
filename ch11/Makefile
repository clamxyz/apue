SRCS=$(wildcard *.c)
BIN=$(SRCS:.c=)

all:$(BIN)

$(BIN):$(SRCS)
	gcc -Wall -DDEBUG -g -lpthread -o $@ $(filter $@.c, $^) 

install:
	./mvbin.sh
clean:
	rm -rf $(BIN)

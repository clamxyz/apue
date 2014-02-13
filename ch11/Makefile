SRCS=$(wildcard *.c)
BIN=$(SRCS:.c=)

all:$(BIN)

$(BIN):$(SRCS)
	gcc -Wall -DDEBUG -g -o $@ $(filter $@.c, $^) 

install:
	./mvbin.sh
clean:
	rm -rf $(BIN)

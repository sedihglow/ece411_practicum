BIN = ./bin/user.out
CC=gcc
CCFLAGS = -I ./include -c -Wall -Wextra
CCFLAGS+= -m64 -O0 -g -pedantic -std=c99
LDFLAGS = -m64

SRCS=$(wildcard ./src/*.c)
OBJS=$(SRCS:.c=.o)

.PHONY: all
all: $(BIN)

$(BIN):$(OBJS)
						$(CC) $(OBJS) -o $@ $(LDFLAGS)
						
%.o:%.c
						$(CC) $(CCFLAGS) -o $@ -c $<

.PHONY: valgrind 
valgrind:				# check for memory leak
						$(info -- Checking For Memory Leaks --)
						make
						valgrind --leak-check=full --show-leak-kinds=all ./bin/user.out

.PHONY: debug
debug:					# GNU debugger
						$(info -- Debugging --)
						gdb ./bin/user.out

.PHONY: clean
clean:					# clean the directory
						$(info -- Cleaning The Directory --)
						rm -rf ./src/*.o ./bin/user.out
.PHONY: run
run:					# run the program as follows
						$(info -- Running Program --)
						make
						./bin/user.out

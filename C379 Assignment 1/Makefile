CC=gcc
# todo: delete -g
CFLAGS=-Wall -Wextra -std=gnu99 -g -m32
DEPS=memlayout.h

.PHONY: all
all: mem_1

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mem_1: memlayout.o mem_1.c
	$(CC) $(CFLAGS) -o mem_1 memlayout.o mem_1.c

.PHONY: clean
clean:
	rm -f *.o mem_1
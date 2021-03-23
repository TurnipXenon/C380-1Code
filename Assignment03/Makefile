CC=gcc -Wall -Wextra -std=gnu99 -g $(CFLAGS)

.PHONY: all
all: valws379

%.o: %.c %.h
	$(CC) -c -o $@ $<

valws379: valws379.c
	$(CC) -o $@ $^

notapp: argparser.o notapp_base.o observer_client.o user_client.o server.o server_data.o notapp.c
	$(CC) -o $@ $^

.PHONY: notapp.time
notapp.time:
	make clean && make CFLAGS='-D NOTAPP_TIME'

.PHONY: run_test
run_test: test
	./test

.PHONY: clean
clean:
	rm -f *.o notapp

.PHONY: argtest1
argtest1: valws379
	./valws379 -s 32
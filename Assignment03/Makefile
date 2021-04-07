CC=gcc -Wall -Wextra -std=c99 -g $(CFLAGS)

.PHONY: all
all: valws379

%.o: %.c %.h
	$(CC) -c -o $@ $<

valws379: valws379.c
	$(CC) -o $@ $^

notapp: argparser.o notapp_base.o observer_client.o user_client.o server.o server_data.o notapp.c
	$(CC) -o $@ $^

lackey_test: lab9/lackey.c
	$(CC) -g -o $@ $^

.PHONY: notapp.time
notapp.time:
	make clean && make CFLAGS='-D NOTAPP_TIME'

.PHONY: run_test
run_test: test
	./test

.PHONY: clean
clean:
	rm -f *.o valws379

.PHONY: argtest1
argtest1: valws379
	./valws379 -s 32

.PHONY: test1
test1: valws379 lackey_test
	 valgrind --tool=lackey --trace-mem=yes ./lackey_test 2> ./valws379 -s 32
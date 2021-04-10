CC=gcc -Wall -Wextra -std=gnu99 -g $(CFLAGS)
EXRANOFF=setarch `uname -m` -R
TEST_LOC=results/test210410_0008.txt

.PHONY: all
all: valws379

%.o: %.c %.h
	$(CC) -c -o $@ $<

valws379: valws379_base.o queue.o windowset.o hashtable.o linkedlist.o valws379.c
	$(CC) -o $@ $^

queue_test: valws379_base.o queue.o queue_test.c
	$(CC) -o $@ $^

# notapp: argparser.o notapp_base.o observer_client.o user_client.o server.o server_data.o notapp.c
# 	$(CC) -o $@ $^

lackey_test: lab9/lackey.c
	$(CC) -g -o $@ $^

.PHONY: clean
clean:
	rm -f *.o valws379

.PHONY: argtest1
argtest1: valws379
	./valws379 -s 32

.PHONY: test1
test1: valws379 lackey_test
	 valgrind --tool=lackey --trace-mem=yes ./lackey_test 2>&1 /dev/null | $(EXRANOFF) ./valws379 -s 3 16 20 > $(TEST_LOC)
	

.PHONY: valgrind1
valgrind1: valws379 lackey_test
	 valgrind --tool=lackey --trace-mem=yes ./lackey_test 2>&1 /dev/null | $(EXRANOFF) valgrind --leak-check=full ./valws379 -s 0 16 100  > $(TEST_LOC)

.PHONY: queue_test1
queue_test1: queue_test
	valgrind --leak-check=full ./queue_test
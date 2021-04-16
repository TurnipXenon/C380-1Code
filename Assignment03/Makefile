CC=gcc -Wall -Wextra -std=gnu99 -g $(CFLAGS)
EXRANOFF=setarch `uname -m` -R
TEST_LOC=results/test210415_1915.txt
VALGRIND_PREFIX=valgrind --tool=lackey --trace-mem=yes
CFLAGS=-D DEBUG_COMMENTS
# todo cleanup

.PHONY: all
all: valws379

%.o: %.c %.h
	$(CC) -c -o $@ $<

# for testing purposes
# from https://stackoverflow.com/questions/3283021/compile-a-standalone-static-executable
%.out: %.c
	$(CC) -static -static-libgcc -static-libstdc++ -o $@ $^

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

# Tests below

.PHONY: test1
test1: valws379 lackey_test
	 valgrind --tool=lackey --trace-mem=yes ./lackey_test 2>&1 /dev/null | $(EXRANOFF) ./valws379 128 1000 > $(TEST_LOC)

.PHONY: valgrind1
valgrind1: valws379 lackey_test
	 valgrind --tool=lackey --trace-mem=yes ./lackey_test 2>&1 /dev/null | $(EXRANOFF) valgrind --leak-check=full ./valws379 16 1000  > $(TEST_LOC)

.PHONY: queue_test1
queue_test1: queue_test
	valgrind --leak-check=full ./queue_test

.PHONY: test_heapsort25
test_heapsort25: results/heapsort.out valws379 lackey_test
	$(VALGRIND_PREFIX) ./results/heapsort.out 100 2>&1 /dev/null | ./valws379 4096 100000 > results/heapsort_result_25.txt
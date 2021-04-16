CC=gcc -Wall -Wextra -std=gnu99 -g $(CFLAGS)
EXRANOFF=setarch `uname -m` -R
TEST_LOC=results/test210415_1915.txt
VALGRIND_PREFIX=valgrind --tool=lackey --trace-mem=yes

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

.PHONY: test_radixsort_full
test_radixsort_full: results/radixsort.out valws379
	$(VALGRIND_PREFIX) ./results/radixsort.out 10000 2>&1 /dev/null | ./valws379 4096 100000 > results/radixsort_full.txt
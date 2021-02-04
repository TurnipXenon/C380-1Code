CC=gcc -Wall -Wextra -std=gnu99 -m32 $(CFLAGS)
EXRANOFF=setarch `uname -m` -R

.PHONY: all
all: mem_1 mem_2 mem_3

%.o: %.c %.h
	$(CC) -c -o $@ $<

mem_1: memlayout.o mem_1.c
	$(CC) -o mem_1 memlayout.o mem_1.c

mem_2: memlayout.o mem_2.c
	$(CC) -o mem_2 memlayout.o mem_2.c

mem_3: memlayout.o mem_3.c
	$(CC) -o mem_3 memlayout.o mem_3.c -ldl

.PHONY: clean
clean:
	rm -f *.o mem_1 mem_2 mem_3

.PHONY: test_1
test_1: mem_1
	$(EXRANOFF) ./mem_1

.PHONY: test_2
test_2: mem_2
	$(EXRANOFF) ./mem_2

.PHONY: test_3
test_3: mem_3
	$(EXRANOFF) ./mem_3

.PHONY: all_tests
all_tests:
	make test_1 && make test_2 && make test_3
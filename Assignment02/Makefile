CC=gcc -Wall -Wextra -std=gnu99 -g -pthread $(CFLAGS)

.PHONY: all
all: notapp

%.o: %.c %.h
	$(CC) -c -o $@ $<

notapp: argparser.o notapp_base.o observer_client.o user_client.o server.o server_data.o notapp.c
	$(CC) -o $@ $^

.PHONY: notapp.time
notapp.time:
	make clean && make CFLAGS='-D NOTAPP_TIME'

.PHONY: clean
clean:
	rm -f *.o notapp

.PHONY: observer1
observer1: notapp
	./notapp -o 127.0.0.1 57993 test_folder1

.PHONY: observer2
observer2: notapp
	./notapp -o 127.0.0.1 57993 test_folder2

.PHONY: observer3
observer3: notapp
	./notapp -o 127.0.0.1 57993 test_folder3

.PHONY: user1
user1: notapp
	./notapp -u 127.0.0.1 57993

.PHONY: server1
server1: notapp
	./notapp -s -t 5.5 -p 57993 # valgrind ./notapp -s -t 2.5 -l server1_test.txt
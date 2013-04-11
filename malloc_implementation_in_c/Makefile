CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Werror

run-test : build-release
	./tltest

run-debug : build-debug
	./tltest

run-gdb : build-debug
	gdb ./tltest

run-valgrind : build-debug
	valgrind --leak-check=yes --show-reachable=yes --tool=memcheck ./tltest

build-release : CFLAGS += -O2
build-release : tllist.o tltest.c tllist.h
	$(CC) $(CFLAGS) tltest.c tllist.o -o tltest

build-debug : CFLAGS += -g -DDEBUG
build-debug : tllist.o tltest.c tllist.h
	$(CC) $(CFLAGS) tltest.c tllist.o -o tltest

tllist.o : tllist.c tllist.h
	$(CC) $(CFLAGS) -c tllist.c

clean :
	rm *.o tltest

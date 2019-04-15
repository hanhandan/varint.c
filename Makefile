CC = gcc
AR = ar

all: lib

lib: 
	$(CC) -c varint.c -o varint.o
	$(AR) rcs varint.a varint.o

clean:
	rm *.o

test:
	$(CC) test/main.c varint.a -o test/main
	./test/main

srv:
	$(CC) srv/server.c varint.a -o server
cli:
	$(CC) cli/client.c varint.a -o client

.PHONY: test

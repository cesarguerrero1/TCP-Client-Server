all: server

server: server.o
	gcc -g -Wall server.o -o server

server.o: server.c
	gcc -g -Wall -c server.c

clean:
	rm -r *.o server
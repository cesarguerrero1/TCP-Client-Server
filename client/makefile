all: client

client: client.o
	gcc -g -Wall client.o -o client

client.o: client.c
	gcc -g -Wall -c client.c

clean:
	rm -r *.o client
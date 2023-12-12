all: client

client: client.o commands.o command-helpers.o config.o
	gcc -g -Wall client.o commands.o command-helpers.o config.o -o client

client.o: client.c
	gcc -g -Wall -c client.c

commands.o: commands.c commands.h
	gcc -g -Wall -c commands.c

command-helpers.o: command-helpers.c command-helpers.h
	gcc -g -Wall -c command-helpers.c

config.o: config.c config.h
	gcc -g -Wall -c config.c

clean:
	rm -rf *.o client
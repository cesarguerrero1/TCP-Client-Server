all: server

server: server.o commands.o command-helpers.o config.o
	gcc -g -Wall server.o commands.o command-helpers.o config.o -o server

server.o: server.c
	gcc -g -Wall -c server.c

commands.o: commands.c commands.h
	gcc -g -Wall -c commands.c

command-helpers.o: command-helpers.c command-helpers.h
	gcc -g -Wall -c command-helpers.c

config.o: config.c config.h
	gcc -g -Wall -c config.c

clean:
	rm -rf *.o server ./server-root
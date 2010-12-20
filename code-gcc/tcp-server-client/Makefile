

CC := gcc
FLAG := -Wall -ansi

all: tcp-server.o main.o

tcp-server.o: tcp-server.c
	$(CC) $(FLAG) -c tcp-server.c

main.o: main.c 
	$(CC) $(FLAG) main.c -o test

clean:
	rm test

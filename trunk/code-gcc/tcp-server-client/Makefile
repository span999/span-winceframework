

CC := gcc
FLAG := -Wall -ansi

all: tcp-client.o tcp-server.o main.o
	$(CC) $(GLAG) -o test tcp-server.o main.o

tcp-server.o: tcp-server.c
	$(CC) $(FLAG) -c tcp-server.c

main.o: main.c tcp-server.h 
	$(CC) $(FLAG) -c main.c

tcp-client.o: tcp-client.c
	$(CC) $(FLAG) -o client tcp-client.c

clean:
	rm test
	rm client
	rm *.o

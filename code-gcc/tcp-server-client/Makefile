

CC := gcc
FLAG := -Wall -ansi

all: toolhelps.o testrun.o tcp-client.o tcp-server.o main.o
	$(CC) $(GLAG) -o test testrun.o tcp-server.o main.o toolhelps.o

toolhelps.o: toolhelps.c
	$(CC) $(FLAG) -c toolhelps.c

tcp-server.o: tcp-server.c
	$(CC) $(FLAG) -c tcp-server.c

testrun.o: testrun.c
	$(CC) $(FLAG) -c testrun.c

main.o: main.c
	$(CC) $(FLAG) -c main.c

tcp-client.o: tcp-client.c
	$(CC) $(FLAG) -o client tcp-client.c toolhelps.o

clean:
	rm test
	rm client
	rm *.o

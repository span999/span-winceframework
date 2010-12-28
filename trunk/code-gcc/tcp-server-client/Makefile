

CC := gcc
CCFLAG := -Wall -ansi

AR := ar
ARFLAG := rcs

all: toolhelps.a toolhelps.o testrun.o tcp-client.o tcp-server.o main.o
	$(CC) $(CCGLAG) -o test testrun.o tcp-server.o main.o toolhelps.o

toolhelps.o: toolhelps.c
	$(CC) $(CCFLAG) -c toolhelps.c

tcp-server.o: tcp-server.c
	$(CC) $(CCFLAG) -c tcp-server.c

testrun.o: testrun.c
	$(CC) $(CCFLAG) -c testrun.c

main.o: main.c
	$(CC) $(CCFLAG) -c main.c

tcp-client.o: tcp-client.c
	$(CC) $(CCFLAG) -o client tcp-client.c toolhelps.o

toolhelps.a: toolhelps.c
	$(AR) $(ARFLAG) toolhelps.a toolhelps.c

clean:
	rm -f test
	rm -f client
	rm -f *.o
	rm -f *.a

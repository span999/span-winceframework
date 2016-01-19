

CC=gcc


receive.o:
	$(CC) -c receive.c
	
send.o:
	$(CC) -c send.c

main.o:
	#$(CC) -c main.c
	
build: main.o send.o receive.o
	#$(CC) -Wall -L$(CURRPATH) -lshared -o myTest main.o staticlib.a libsIO-common.a
	$(CC) -Wall -O3 -o receive receive.o
	$(CC) -Wall -O3 -o send send.o

all: build


clean:
	#rm *.a
	rm *.o
	#rm *.so
	rm receive send



CC=gcc
OUT=uartestor2-x86

serialport.o:
	$(CC) -c serialport.c

main.o:
	$(CC) -c main.c
	
build: clean main.o serialport.o
	#$(CC) -Wall -L$(CURRPATH) -lshared -o myTest main.o staticlib.a libsIO-common.a
	$(CC) -Wall -O3 -o $(OUT) main.o serialport.o
	$(CC) -Wall -O3 -static -o $(OUT)2 main.c serialport.c

all: build


clean:
	#rm *.a
	rm *.o
	#rm *.so
	rm $(OUT)*

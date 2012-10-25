
CC	= gcc
DEFINES =
CFLAGS	= -O2 -I. -L. ${DEFINES}

PROGRAM	= cpuusage
DYNLIB  = libtest.so


all:PROGRAM


DYNLIB:
	@echo 
	@echo ================================================================
	@echo build dynamic library
	@echo ----------------------------------------------------------------
	${CC} so_test.c -fPIC -shared -o ${DYNLIB}

PROGRAM: 
	@echo 
	@echo ================================================================
	@echo build binary.
	@echo ----------------------------------------------------------------
	${CC} main.c -o ${PROGRAM} -lpthread

clean:
	@echo 
	@echo ================================================================
	@echo Erasing objects and backup files.
	@echo ----------------------------------------------------------------
	rm -f ${PROGRAM} *.so *.o

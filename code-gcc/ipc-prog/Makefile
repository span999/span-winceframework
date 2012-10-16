
CC	= gcc
DEFINES =
CFLAGS	= -O2 -I. -L. ${DEFINES}

PROGRAM	= testcode
DYNLIB  = libtest.so


all:PROGRAM


DYNLIB:
	@echo 
	@echo ================================================================
	@echo build dynamic library
	@echo ----------------------------------------------------------------
	${CC} so_test.c -fPIC -shared -o ${DYNLIB}

PROGRAM: DYNLIB
	@echo 
	@echo ================================================================
	@echo build binary.
	@echo use export LD_LIBRARY_PATH=. for linrary path setting!!
	@echo ----------------------------------------------------------------
	${CC} main.c ${DYNLIB} -o ${PROGRAM} -lpthread

clean:
	@echo 
	@echo ================================================================
	@echo Erasing objects and backup files.
	@echo ----------------------------------------------------------------
	rm -f ${PROGRAM} *.so *.o

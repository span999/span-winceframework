
CC	= gcc
DEFINES =
CFLAGS	= -O2 -I. -L. ${DEFINES}

PROGRAM	= testcode
DYNLIB1  = libtest.so
DYNLIB2  = libipc.so


all: clean PROGRAM

#
# export LD_LIBRARY_PATH=.
# if you got error while loading shared linraries
#


DYNLIB1:
	@echo 
	@echo ================================================================
	@echo build dynamic library ${DYNLIB1}
	@echo ----------------------------------------------------------------
	${CC} so_test.c -fPIC -shared -o ${DYNLIB1}

DYNLIB2:
	@echo 
	@echo ================================================================
	@echo build dynamic library ${DYNLIB2}
	@echo ----------------------------------------------------------------
	${CC} so_ipc.c -fPIC -shared -o ${DYNLIB2}


PROGRAM: DYNLIB1 DYNLIB2
	@echo 
	@echo ================================================================
	@echo 
	@echo build binary. ${PROGRAM}
	@echo   use export LD_LIBRARY_PATH=. for linrary path setting!! 
	@echo 
	@echo ----------------------------------------------------------------
	${CC} main.c ${DYNLIB1}  ${DYNLIB2} -o ${PROGRAM} -lpthread

clean:
	@echo 
	@echo ================================================================
	@echo Erasing objects and backup files.
	@echo ----------------------------------------------------------------
	rm -f ${PROGRAM} *.so *.o


CC	= gcc
AR	= ar
DEFINES =
CFLAGS	= -O2 -I. -L. ${DEFINES}

PROGRAM	= cpuusage
DYNLIB  = libtest.so
STATICLIB = libcpuutil.a
STATICNAME = cpuutil


all: clean PROGRAM


STATICLIB:
	@echo 
	@echo ================================================================
	@echo build static library
	@echo ----------------------------------------------------------------
	${CC} -c ${STATICNAME}.c
	${AR} rcs ${STATICLIB} ${STATICNAME}.o

DYNLIB:
	@echo 
	@echo ================================================================
	@echo build dynamic library
	@echo ----------------------------------------------------------------
	${CC} so_test.c -fPIC -shared -o ${DYNLIB}

PROGRAM: STATICLIB
	@echo 
	@echo ================================================================
	@echo build binary.
	@echo ----------------------------------------------------------------
	${CC} main.c -o ${PROGRAM} -lpthread -L. -l${STATICNAME}

clean:
	@echo 
	@echo ================================================================
	@echo Erasing objects and backup files.
	@echo ----------------------------------------------------------------
	rm -f ${PROGRAM} *.so *.o *.a

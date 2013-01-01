#
#
#

CC      = gcc
AR      = ar
DEFINES =
CFLAGS+= -O2 -I. -L. ${DEFINES}
#CFLAGS+="$(pkg-config dbus-1 --cflags)"

OUTBIN1 = dbus.bin
DBUSINC = -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include
#DBUSINC = $(pkg-config dbus-1 --cflags)
DBUSLIB = -l dbus-1
PROGRAM = dbus.c
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

PROGRAM: 
	@echo 
	@echo ================================================================
	@echo build binary = ${OUTBIN1}
	@echo ----------------------------------------------------------------
	${CC} ${PROGRAM} -o ${OUTBIN1} -lpthread ${DBUSINC} ${DBUSLIB} ${CFLAGS}
#	${CC} ${PROGRAM} -o ${OUTBIN1} -lpthread ${CFLAGS}


clean:
	@echo 
	@echo ================================================================
	@echo Erasing objects and backup files.
	@echo ----------------------------------------------------------------
	rm -f ${OUTBIN1} *.so *.o *.a

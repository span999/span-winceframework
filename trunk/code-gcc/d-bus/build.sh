#!/bin/bash
#
#

CC=gcc
BIN=dbus.bin
DBUSINC="-I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include"
DBUSLIB="-l dbus-1"


rm ${BIN}
###${CC} dbus.c -o ${BIN} $(pkg-config dbus-1 --cflags)
${CC} dbus.c -o ${BIN} ${DBUSINC} ${DBUSLIB}



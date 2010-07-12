# MPLAB IDE generated this makefile for use with GNU make.
# Project: ePaper-NMEAparser-Lib.mcp
# Date: Mon Jul 12 23:01:06 2010

COMPILERDIR = tools\compiler

AS = $(COMPILERDIR)\pic30-as.exe
CC = $(COMPILERDIR)\pic30-gcc.exe
LD = $(COMPILERDIR)\pic30-ld.exe
AR = $(COMPILERDIR)\pic30-ar.exe
HX = $(COMPILERDIR)\pic30-bin2hex.exe
# RM = rm
RM = del


./lib/ePaper-NMEAparser-Lib.a : obj/NMEAparser.o
	$(AR) r ".\lib\ePaper-NMEAparser-Lib.a" "obj\NMEAparser.o"

obj/NMEAparser.o : include/GenericTypeDefs.h include/mTypeDef.h src/services/NMEAparser/NMEAparser-def.h c:/program\ files/microchip/mplab\ c30/include/stdio.h c:/program\ files/microchip/mplab\ c30/include/yvals.h c:/program\ files/microchip/mplab\ c30/include/string.h src/services/NMEAparser/NMEAparser.c
	$(CC) -mcpu=24FJ128GB106 -x c -c "src\services\NMEAparser\NMEAparser.c" -o "obj\NMEAparser.o" -I "include\" -I "tools\include\" -g -Wall

clean : 
	$(RM) "obj\NMEAparser.o" ".\lib\ePaper-NMEAparser-Lib.a" ".\lib\ePaper-NMEAparser-Lib.hex"


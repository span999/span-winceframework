
CC	= gcc
DEFINES =
CFLAGS	= -O2 -I/usr/X11R6/include/ -L/usr/X11R6/lib/ ${DEFINES}

PROGRAM	= picGUI

SRCS += $(wildcard *.c)
SRCS += $(wildcard ./Graphics/*.c)
SRCS += $(wildcard ./Demo/*.c)

HDRS += $(wildcard *.h)
HDRS += $(wildcard ./Graphics/*.h)
HDRS += $(wildcard ./Demo/*.h)

LIBS += -lXext -lX11 -lm
OBJS += $(SRCS:.c=.o)
	  

all:PROGRAM

PROGRAM: $(SRCS) $(HDRS) 
	${CC} $(CFLAGS) -o $(PROGRAM) $(SRCS) $(LIBS) 

clean:
	@echo ================================================================
	@echo Erasing objects and backup files.
	@echo ----------------------------------------------------------------
	rm -f $(PROGRAM) $(OBJS)

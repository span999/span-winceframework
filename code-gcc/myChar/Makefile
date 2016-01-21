#
# $ sudo insmod ./example.ko
# $ sudo rmmod example
#

#INC1=/usr/src/linux-headers-3.13.0-76-generic/include
#INC2=/usr/src/linux-headers-3.13.0-76/arch/x86/include

obj-m+=myChar.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	#$(CC) myChar.c -o test -I$(INC1) -I$(INC2)
	gcc myCharTest.c -o myCharTest
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm myCharTest

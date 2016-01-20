/*
 * 
 * 
 * 
 * 
**/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <asm/ioctls.h>
#include "xr17v35x.h"
 

/* ======================================================== */
#define 	BAUDRATE 		B115200 
///#define 	MODEMDEVICE 	"/dev/ttyUSB0"
#define     MODEMDEVICE    	"/dev/ttyXR0"
///#define     MODEMDEVICE    	"/dev/ttyS0"
#define 	_POSIX_SOURCE 	1 /* POSIX compliant source */
#define 	FALSE 			0
#define 	TRUE 			1

#define		IR_PIN			8
#define		IR_PULSE		(562-100)
#define		IR_GROUND		(1675-100)

/* ======================================================== */
void openport(void);
void readport(void);
void sendport(void);
void closeport(void);
static void dumpport(void);
static void IrDAport(void);
static void MPIOportOut(int pin, int level);
static int MPIOportIn(int pin);
static void MPIO_NEC_IR_init(int pin);
static void MPIO_NEC_IR_LeadPulse(int pin);
static void MPIO_NEC_IR_Data(int pin, unsigned char data);
static void MPIO_NEC_IR_LogicONE(int pin);
static void MPIO_NEC_IR_LogicZERO(int pin);
static void MPIO_NEC_IR_EndPulse(int pin);
static void MPIO_NEC_IR_PinCtrl(int pin, int level);
static void MPIO_NEC_IR_deinit(int pin);
static void Normalport(void);
static void setdefault(struct termios *);


/* ======================================================== */
///char *tmpbuffer;
int fd = 0;
struct termios oldtp, newtp;
//char sendcmd1[256]="\0";
///char buffer[512];
 

void main()
{
	openport();
	sleep(1);
	
	dumpport();

	MPIO_NEC_IR_init(IR_PIN);
	///MPIO_NEC_IR_LeadPulse(IR_PIN);
	MPIO_NEC_IR_Data(IR_PIN, 0x55);
	MPIO_NEC_IR_EndPulse(IR_PIN);
	MPIO_NEC_IR_deinit(IR_PIN);

	MPIOportOut(8, 0);
	usleep(562);
	MPIOportOut(8, 1);
	usleep(562);
	MPIOportOut(8, 0);
	usleep(562);
	MPIOportOut(8, 1);
	sleep(1);
	MPIOportOut(8, 0);

	dumpport();
	
	closeport();
}

static void setdefault(struct termios *ptms)
{
	//char msg[80] = "setdefault: set default value!\r\n";
	
	if( ptms ) 
	{
		//fputs(msg,output);
		ptms->c_cflag |= (CLOCAL | CREAD);    // Enable the receiver and set local mode
		ptms->c_cflag &= ~PARENB;             // No parity bit
		ptms->c_cflag &= ~CSTOPB;             // 1 stop bit
		ptms->c_cflag &= ~CSIZE;              // Mask data size
		ptms->c_cflag |=  CS8;                // Select 8 data bits
		ptms->c_cflag &= ~CRTSCTS;            // Disable hardware flow control
	}
	return;
}

void sendport(void)
{
	int n;
	char *tmpbuffer;
	FILE *file;
	int fileLen;
	
	///printf("enter write\n");

	//sem_wait(&len);
	file = fopen("send.txt", "r");
 
	//get file size
	fseek(file, 0, SEEK_END);
	fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);
 
	tmpbuffer = (char *)malloc(fileLen + 1);
 
	//read file contents
	printf("Start send...\n");
	
	///read to buffer once
	fread(tmpbuffer, fileLen, 1, file);
	fclose(file);

	///send buffer to serial port
	n = write(fd, tmpbuffer, fileLen + 1);
 
	if (n < 0)
	{
		fputs("write() of bytes failed!\n", stderr);
	}
	else
	{
		printf("Image sent successfully %d\n",n);
    }
	///close(fd);

}

static void dumpport(void)
{
	struct xrioctl_rw_reg xregs;

	printf("EXAR_READ_REG=[0x%04x], EXAR_WRITE_REG=[0x%04x]\n", EXAR_READ_REG, EXAR_WRITE_REG);

	/* set LCR[7]=1 */
	xregs.reg = XR_CFG_LCR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	xregs.regvalue = (xregs.regvalue | 0x80);
	ioctl(fd, EXAR_WRITE_REG, &xregs);

	/* XR_CFG_DLL */
	xregs.reg = XR_CFG_DLL;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
	
	/* XR_CFG_DLM */
	xregs.reg = XR_CFG_DLM;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
	
	/* XR_CFG_DLD */
	xregs.reg = XR_CFG_DLD;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);

	/* clear LCR[7]=0 */
	xregs.reg = XR_CFG_LCR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	xregs.regvalue = (xregs.regvalue & (~0x80));
	ioctl(fd, EXAR_WRITE_REG, &xregs);

	/* XR_CFG_IER */
	xregs.reg = XR_CFG_IER;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
	
	/* XR_CFG_ISR */
	xregs.reg = XR_CFG_ISR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
	
	/* XR_CFG_LCR */
	xregs.reg = XR_CFG_LCR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);

	/* XR_CFG_MCR */
	xregs.reg = XR_CFG_MCR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
	
	/* XR_CFG_LSR */
	xregs.reg = XR_CFG_LSR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
	
	/* XR_CFG_MSR */
	xregs.reg = XR_CFG_MSR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);

	/* XR_CFG_SPR */
	xregs.reg = XR_CFG_SPR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);

	/* XR_CFG_FCTR */
	xregs.reg = XR_CFG_FCTR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);

	/* XR_CFG_EFR */
	xregs.reg = XR_CFG_EFR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);

}

static void IrDAport(void)
{
	struct xrioctl_rw_reg xregs;
	unsigned char old = 0;

	xregs.reg = XR_CFG_EFR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	old = xregs.regvalue;
	xregs.regvalue = xregs.regvalue|0x10;	/* enable IR function */
	ioctl(fd, EXAR_WRITE_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]->[0x%02x]\n", xregs.reg, old, xregs.regvalue);

	xregs.reg = XR_CFG_EFR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);


	xregs.reg = XR_CFG_MCR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	old = xregs.regvalue;
	xregs.regvalue = xregs.regvalue|0x40;	/* enable IR mode */
	ioctl(fd, EXAR_WRITE_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]->[0x%02x]\n", xregs.reg, old, xregs.regvalue);

	xregs.reg = XR_CFG_MCR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
	
	printf("enable IR mode\n");
}

///#define		IO_DEBUG

static void MPIOportOut(int pin, int level)
{
	struct xrioctl_rw_reg xregs;
	unsigned char old = 0;
	unsigned char regAddroff = 0;
	unsigned char regValoff = 0;

#ifdef IO_DEBUG
	printf("MPIOportOut pin:%d, level:%d\n", pin, level);
#endif

	if (7<pin) {
		regAddroff = 6;
		regValoff = 8;
	}	

	xregs.reg = (XR_CFG_MPIOLVL0_7+regAddroff);
	ioctl(fd, EXAR_READ_REG, &xregs);
	old = xregs.regvalue;
	if (0<level) {
		xregs.regvalue = (xregs.regvalue|(0x01<<(pin-regValoff)));	/* set pin level */	
	} else {
		xregs.regvalue = (xregs.regvalue&(~(0x01<<(pin-regValoff))));	/* set pin level */	
	}
	ioctl(fd, EXAR_WRITE_REG, &xregs);
#ifdef IO_DEBUG
	printf("XRreg[0x%02x]=[0x%02x]->[0x%02x]\n", xregs.reg, old, xregs.regvalue);

	xregs.reg = (XR_CFG_MPIOLVL0_7+regAddroff);
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
#endif
	
	xregs.reg = (XR_CFG_MPIOSEL0_7+regAddroff);
	ioctl(fd, EXAR_READ_REG, &xregs);
	old = xregs.regvalue;
	///xregs.regvalue = (xregs.regvalue|(0x01<<(pin-regValoff)));	/* set pin output */
	xregs.regvalue = (xregs.regvalue&(~(0x01<<(pin-regValoff))));	/* set pin output */
	ioctl(fd, EXAR_WRITE_REG, &xregs);
#ifdef IO_DEBUG
	printf("XRreg[0x%02x]=[0x%02x]->[0x%02x]\n", xregs.reg, old, xregs.regvalue);

	xregs.reg = (XR_CFG_MPIOSEL0_7+regAddroff);
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
#endif
}

static void MPIO_NEC_IR_init(int pin)
{
	struct xrioctl_rw_reg xregs;
	unsigned char old = 0;
	unsigned char regAddroff = 0;
	unsigned char regValoff = 0;

#ifdef IO_DEBUG
	printf("MPIO_NEC_IR_init pin:%d\n", pin);
#endif
	if (7<pin) {
		regAddroff = 6;
		regValoff = 8;
	}

	xregs.reg = (XR_CFG_MPIOSEL0_7+regAddroff);
	ioctl(fd, EXAR_READ_REG, &xregs);
	old = xregs.regvalue;
	///xregs.regvalue = (xregs.regvalue|(0x01<<(pin-regValoff)));	/* set pin input */
	xregs.regvalue = (xregs.regvalue&(~(0x01<<(pin-regValoff))));	/* set pin output */
	ioctl(fd, EXAR_WRITE_REG, &xregs);
#ifdef IO_DEBUG
	printf("XRreg[0x%02x]=[0x%02x]->[0x%02x]\n", xregs.reg, old, xregs.regvalue);

	xregs.reg = (XR_CFG_MPIOSEL0_7+regAddroff);
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
#endif

}

static void MPIO_NEC_IR_LeadPulse(int pin)
{
#ifdef IO_DEBUG
	printf("MPIO_NEC_IR_LeadPulse pin:%d\n", pin);
#endif

	MPIO_NEC_IR_PinCtrl(pin,1);
	usleep(9000);
	MPIO_NEC_IR_PinCtrl(pin,0);
	usleep(4500);
}

static void MPIO_NEC_IR_Data(int pin, unsigned char data)
{
	int iLoop = 0;
	
	for ( iLoop = 0; iLoop < 8; iLoop++) {
		if (0<(data&(0x01<<iLoop)))
			MPIO_NEC_IR_LogicONE(pin);
		else
			MPIO_NEC_IR_LogicZERO(pin);
	}

	///inverse
	for ( iLoop = 0; iLoop < 8; iLoop++) {
		if (0<(data&(0x01<<iLoop)))
			MPIO_NEC_IR_LogicZERO(pin);
		else
			MPIO_NEC_IR_LogicONE(pin);
	}

}

static void MPIO_NEC_IR_LogicONE(int pin)
{
#ifdef IO_DEBUG
	printf("MPIO_NEC_IR_LogicZERO pin:%d\n", pin);
#endif

	MPIO_NEC_IR_PinCtrl(pin,1);
	usleep(IR_PULSE);
	MPIO_NEC_IR_PinCtrl(pin,0);
	usleep(IR_GROUND);
}

static void MPIO_NEC_IR_LogicZERO(int pin)
{
#ifdef IO_DEBUG
	printf("MPIO_NEC_IR_LogicZERO pin:%d\n", pin);
#endif

	MPIO_NEC_IR_PinCtrl(pin,1);
	usleep(IR_PULSE);
	MPIO_NEC_IR_PinCtrl(pin,0);
	usleep(IR_PULSE);
}

static void MPIO_NEC_IR_EndPulse(int pin)
{
#ifdef IO_DEBUG
	printf("MPIO_NEC_IR_EndPulse pin:%d\n", pin);
#endif

	MPIO_NEC_IR_PinCtrl(pin,1);
	usleep(IR_PULSE);
	MPIO_NEC_IR_PinCtrl(pin,0);
}

static void MPIO_NEC_IR_PinCtrl(int pin, int level)
{
	struct xrioctl_rw_reg xregs;
	unsigned char old = 0;
	unsigned char regAddroff = 0;
	unsigned char regValoff = 0;

#ifdef IO_DEBUG
	printf("MPIO_NEC_IR_PinCtrl pin:%d level:%d\n", pin, level);
#endif
	if (7<pin) {
		regAddroff = 6;
		regValoff = 8;
	}

	xregs.reg = (XR_CFG_MPIOLVL0_7+regAddroff);
	ioctl(fd, EXAR_READ_REG, &xregs);
	old = xregs.regvalue;
	if (0<level) {
		xregs.regvalue = (xregs.regvalue|(0x01<<(pin-regValoff)));	/* set pin level */	
	} else {
		xregs.regvalue = (xregs.regvalue&(~(0x01<<(pin-regValoff))));	/* set pin level */	
	}
	ioctl(fd, EXAR_WRITE_REG, &xregs);
#ifdef IO_DEBUG
	printf("XRreg[0x%02x]=[0x%02x]->[0x%02x]\n", xregs.reg, old, xregs.regvalue);

	xregs.reg = (XR_CFG_MPIOLVL0_7+regAddroff);
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
#endif

}

static void MPIO_NEC_IR_deinit(int pin)
{
	struct xrioctl_rw_reg xregs;
	unsigned char old = 0;
	unsigned char regAddroff = 0;
	unsigned char regValoff = 0;

#ifdef IO_DEBUG
	printf("MPIO_NEC_IR_deinit pin:%d\n", pin);
#endif
	if (7<pin) {
		regAddroff = 6;
		regValoff = 8;
	}

	xregs.reg = (XR_CFG_MPIOSEL0_7+regAddroff);
	ioctl(fd, EXAR_READ_REG, &xregs);
	old = xregs.regvalue;
	xregs.regvalue = (xregs.regvalue|(0x01<<(pin-regValoff)));	/* set pin input */
	///xregs.regvalue = (xregs.regvalue&(~(0x01<<(pin-regValoff))));	/* set pin output */
	ioctl(fd, EXAR_WRITE_REG, &xregs);
#ifdef IO_DEBUG
	printf("XRreg[0x%02x]=[0x%02x]->[0x%02x]\n", xregs.reg, old, xregs.regvalue);

	xregs.reg = (XR_CFG_MPIOSEL0_7+regAddroff);
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
#endif

}




static void Normalport(void)
{
	struct xrioctl_rw_reg xregs;
	unsigned char old = 0;

	xregs.reg = XR_CFG_MCR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	old = xregs.regvalue;
	xregs.regvalue = (xregs.regvalue&(~0x40));	/* disable enable IR mode */
	ioctl(fd, EXAR_WRITE_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]->[0x%02x]\n", xregs.reg, old, xregs.regvalue);

	xregs.reg = XR_CFG_MCR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
	
	xregs.reg = XR_CFG_EFR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	old = xregs.regvalue;
	xregs.regvalue = (xregs.regvalue&(~0x10));	/* disable IR function */
	ioctl(fd, EXAR_WRITE_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]->[0x%02x]\n", xregs.reg, old, xregs.regvalue);

	xregs.reg = XR_CFG_EFR;
	ioctl(fd, EXAR_READ_REG, &xregs);
	printf("XRreg[0x%02x]=[0x%02x]\n", xregs.reg, xregs.regvalue);
}

void openport(void)
{
	fd = open(MODEMDEVICE, O_RDWR|O_NOCTTY|O_NDELAY );
	
	printf("Oviya port[%s] open=%d\n", MODEMDEVICE, fd);
	if (fd<0) { perror(MODEMDEVICE); exit(-1); }
                                                                                
	fcntl(fd, F_SETFL, 0);
	
	tcgetattr(fd, &oldtp); /* save current serial port settings */
	//tcgetattr(fd, &newtp); /* save current serial port settings */
	bzero(&newtp, sizeof(newtp)); /* clean */
                                                                                
	newtp.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	
	setdefault( &newtp );	/* set default */
	
	newtp.c_iflag = IGNPAR | ICRNL;
	newtp.c_oflag = 0;
	newtp.c_lflag = ICANON;

	/* 
		initialize all control characters 
		default values can be found in /usr/include/termios.h, and are given
		in the comments, but we don't need them here
	*/
	newtp.c_cc[VINTR]    = 0;     /* Ctrl-c */
	newtp.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	newtp.c_cc[VERASE]   = 0;     /* del */
	newtp.c_cc[VKILL]    = 0;     /* @ */
	//newtp.c_cc[VEOF]     = 4;     /* Ctrl-d */
	newtp.c_cc[VEOF]     = 0;     /* Ctrl-d */
	newtp.c_cc[VTIME]    = 0;     /* inter-character timer unused */
	newtp.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
	newtp.c_cc[VSWTC]    = 0;     /* '\0' */
	newtp.c_cc[VSTART]   = 0;     /* Ctrl-q */
	newtp.c_cc[VSTOP]    = 0;     /* Ctrl-s */
	newtp.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	newtp.c_cc[VEOL]     = 0;     /* '\0' */
	newtp.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	newtp.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	newtp.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	newtp.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	newtp.c_cc[VEOL2]    = 0;     /* '\0' */

	///tcflush(fd, TCIFLUSH);
	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &newtp);
}

void closeport(void)
{
	close(fd);
}

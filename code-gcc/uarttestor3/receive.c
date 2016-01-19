#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>        
#include <stdlib.h> 

#define BAUDRATE B115200 
#define MODEMDEVICE "/dev/ttyUSB0"/*UART NAME IN PROCESSOR*/
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

///#define LOGFILE

void openport(void);
void sendport(void);
void readport(void);

int fd=0, n;
static int cnt, size, s_cnt;
unsigned char *var;
struct termios oldtp, newtp;
char sendcmd1[10]="\0";
FILE *file;
 
void readport(void)
{
	unsigned char buff;
#ifdef LOGFILE
	file = fopen( "zname.txt", "w+" );
#endif

	while (1) { 
		n = read(fd, &buff, 1);
		//fcntl(fd,F_SETFL,0);
		if (n == -1) {
			switch(errno) {
				case EAGAIN: /* sleep() */ 
					continue;
          
				default: 
					goto quit;
			}
		}
        
		if (n ==0) break;

#ifdef LOGFILE		
		fputc(buff, file);
#endif
		printf("%d %c\n", n,buff);
	}

quit:
#ifdef LOGFILE
	fclose (file);
#endif
	;
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

void openport(void)
{
         
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY |O_NDELAY );
	printf("oviya :%d\n",fd);
	
	if (fd < 0)
    {
		perror(MODEMDEVICE);
	}
                                                                                
	fcntl(fd,F_SETFL,0);
	tcgetattr(fd,&oldtp); /* save current serial port settings */
	//tcgetattr(fd,&newtp); /* save current serial port settings */
	bzero(&newtp, sizeof(newtp));
	//bzero(&oldtp, sizeof(oldtp));
                                                                                
	newtp.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

	setdefault( &newtp );

	newtp.c_iflag = IGNPAR | ICRNL;
                                                                                
	newtp.c_oflag = 0;                                                                        
	newtp.c_lflag = ICANON;                                                                    
	newtp.c_cc[VINTR]    = 0;     /* Ctrl-c */
	newtp.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	newtp.c_cc[VERASE]   = 0;     /* del */
	newtp.c_cc[VKILL]    = 0;     /* @ */
	//newtp.c_cc[VEOF]     = 4;     /* Ctrl-d */
	newtp.c_cc[VTIME]    = 0;     /* inter-character timer unused */
	newtp.c_cc[VMIN]     = 0;     /* blocking read until 1 character arrives */
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
	//tcflush(fd, TCIFLUSH);
	//tcsetattr(fd,TCSANOW,&newtp);
}
 
int main(int argc, char** argv)
{
	openport();
	readport();
	
	return 0;
}

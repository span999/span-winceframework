/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* install if libxtst not found */
/* sudo apt-get install libxtst-dev */
#if 1
#include <X11/extensions/XTest.h>
#else  /* new X11 */
#include <Xlib.h>
#endif
#include <X11/keysym.h>
#include <termios.h>

#include "toolhelps.h"

/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
#if 1
	#define	DBGFSET		(dDBG|dINFO|dERR)
#else
	#define	DBGFSET		(dINFO|dERR)
#endif
#define	dF(x)		(DBGFSET&x)





char _getch() {

	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
		
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");

	if (read(0, &buf, 1) < 0)
		perror ("read()");
	
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
    
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror ("tcsetattr ~ICANON");
	
	return (buf);
}


int FakeKeySend( Display* disp, unsigned int keyCode )
{
	int iRet = 0;
#ifdef __ARM_CODE__
	XTestFakeKeyEvent( disp, XKeysymToKeycode( disp, keyCode), True, CurrentTime );
	///spMSG( dF(dINFO), "%s:%s: keyCode=%d ON\n", __FILE__, __FUNCTION__, keyCode );
	XFlush(disp);
	XTestFakeKeyEvent( disp, XKeysymToKeycode( disp, keyCode), False, CurrentTime );
	///spMSG( dF(dINFO), "%s:%s: keyCode=%d OFF\n", __FILE__, __FUNCTION__, keyCode );
	XFlush(disp);
#else
	XTestFakeKeyEvent( disp, XKeysymToKeycode( disp, keyCode), True, CurrentTime );
	///spMSG( dF(dINFO), "%s:%s: keyCode=%d ON\n", __FILE__, __FUNCTION__, keyCode );
	XFlush(disp);
	XTestFakeKeyEvent( disp, XKeysymToKeycode( disp, keyCode), False, CurrentTime );
	///spMSG( dF(dINFO), "%s:%s: keyCode=%d OFF\n", __FILE__, __FUNCTION__, keyCode );
	XFlush(disp);
#endif	
	return iRet;
}


int main(void) {
	
	Display* disp = NULL;
	int iTmp = 0;
	char userKey = 0;
	char prevKey = 0;
	
	
	disp = XOpenDisplay( NULL );
	if( disp == NULL )
	{
		spMSG( dF(dERR), "%s:%s: XOpenDisplay fail !!!\n", __FILE__, __FUNCTION__ );
		return 1;
	}
	
	while( iTmp++ < 10000 )
	{
		///FakeKeySend( disp, XK_Up );

		///userKey = getchar();
		userKey = _getch();
		spMSG( dF(dINFO), "%s:%s: user key = [%d][%c][0x%02x]\n", __FILE__, __FUNCTION__, userKey, userKey, userKey );
		
		/* double 'ESC' for exit */
		if( (userKey == 27) && (prevKey == 27) )
			break;
		else
		if( (userKey == 65) && (prevKey == 91) )
			FakeKeySend( disp, XK_Up );
		else
		if( (userKey == 66) && (prevKey == 91) )
			FakeKeySend( disp, XK_Down );
		else
		if( (userKey == 67) && (prevKey == 91) )
			FakeKeySend( disp, XK_Right );
		else
		if( (userKey == 68) && (prevKey == 91) )
			FakeKeySend( disp, XK_Left );
		else
		if( (userKey == 10) )
			FakeKeySend( disp, XK_Return );
		else
		if( (userKey == 9) )
			FakeKeySend( disp, XK_Tab );
		else
		if( (userKey == 127) )
			FakeKeySend( disp, XK_BackSpace );
		else
		if( (userKey == 51) && (prevKey == 91) )
			FakeKeySend( disp, XK_Delete );		
		else
		if( (userKey == 70) && (prevKey == 79) )
			FakeKeySend( disp, XK_End );		
		else
		if( (userKey == 53) && (prevKey == 91) )
			FakeKeySend( disp, XK_Page_Up );		
		else
		if( (userKey == 54) && (prevKey == 91) )
			FakeKeySend( disp, XK_Page_Down );		
		
		sleep(0);
		prevKey = userKey;
	}
	
	
	XCloseDisplay( disp );
	
	
	return 0;
}

	

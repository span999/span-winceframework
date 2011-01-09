
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <sys/time.h>

#include "xinit.h"
#include "types.h"

#include "Demo/MainDemo.h"

int InfiniteLoop=TRUE;
int TimerCount;

XWindow *xw;

void OnBreak(int Arg)
{
  	InfiniteLoop=FALSE;
}

void ClearKBuf(XWindow *XWnd)
{
  	XEvent ev;
  	long KEY;

  	while(XCheckWindowEvent(XWnd->display,XWnd->window,KeyPressMask,&ev))
    {
      	KEY=XLookupKeysym((XKeyEvent *)&ev,0);  
    }
}

void clear_timer()
{
  	struct itimerval iv;

  	iv.it_value.tv_sec  = iv.it_interval.tv_sec  = 0;
  	iv.it_value.tv_usec = iv.it_interval.tv_usec = 0;

  	signal(SIGALRM, SIG_DFL);
  	setitimer(ITIMER_REAL, &iv, NULL);
}

int main(int argc,char *argv[])
{
  	XSetWindowAttributes xsw;
  	int MyPalette[]= { 255,255,255, 255,0,0, 0,255,0, 0,0,255, 0,0,0,
		     			-1,-1,-1}; /* put this to specify the end of your palette */
	int buttonPressed = 0;	     			
	
  	/* Opening a window with a palette and no virtual screen */
  	xw=(XWindow *)malloc(sizeof(XWindow));
  	if( OpenXWindow(xw, 320, 240, -1, "picGUI...", MyPalette, FALSE, TRUE, TRUE, TRUE) )
    {
      	printf("\nClosing eGUI.\n");
      	return 1;
    }
	
	/* Setting specific properties for the window */
	xsw.border_pixel     = WhitePixel(xw->display,xw->screennum);
	xsw.background_pixel = BlackPixel(xw->display,xw->screennum);
	xsw.win_gravity      = SouthEastGravity;
	
	XChangeWindowAttributes(xw->display, xw->window, CWBackPixel|CWBorderPixel|CWWinGravity, &xsw);
	XClearWindow(xw->display,xw->window);

  	/* Clean exiting on severals breaks */
  	signal(SIGHUP,OnBreak);
  	signal(SIGINT,OnBreak);
  	signal(SIGQUIT,OnBreak);
  	signal(SIGTERM,OnBreak);

	initDevice( xw );
	init_mainDemo();
		
	while(InfiniteLoop)
    {
    	GOL_MSG uiMsg = { -1, -1, 0, 0 };
    	XEvent  ev;
      	long    KEY;

		if( XPending( xw->display ) )
		{	
			XNextEvent( xw->display,&ev );		
	      		      	
	      	switch(ev.type)
			{
				case KeyPress:
				  	KEY=XLookupKeysym((XKeyEvent *)&ev,0);
		  			switch(KEY)
		    		{
		    			case XK_Escape:
		    			case XK_space:
		      				InfiniteLoop=FALSE;
		      				break;
		      				
		      			case XK_Right:
		      				printf( "XK_Right\n");
		      				break;
		      			case XK_Left:
		      				printf( "XK_Left\n");
		      				break;
		      			case XK_Up:
		      				printf( "XK_Up\n");
		      				break;
		      			case XK_Down:
		      				printf( "XK_Down\n");
		      				break;
		      				
		      			default:
		      				break;
		    		}
		  			ClearKBuf(xw);
		  			break;
		  			
				case Expose:	  			
		  			break;
		  			
				case ButtonPress:
					//printf( "Clicked at (%d,%d)\n", ev.xmotion.x, ev.xmotion.y );	  
					uiMsg.type    = TYPE_TOUCHSCREEN;
					uiMsg.uiEvent = EVENT_PRESS;
					uiMsg.param1  = ev.xmotion.x;
    				uiMsg.param2  = ev.xmotion.y;
    				buttonPressed = 1;
		  			break;
		  			
		  		case ButtonRelease:		  		
		  			//printf( "Click Released at (%d,%d)\n", ev.xmotion.x, ev.xmotion.y );
		  			uiMsg.type    = TYPE_TOUCHSCREEN;
					uiMsg.uiEvent = EVENT_RELEASE;
					uiMsg.param1  = ev.xmotion.x;
    				uiMsg.param2  = ev.xmotion.y;
    				buttonPressed = 0;
		  			break;
		  			
		  		case MotionNotify:
		  			//printf( "Notification (%d,%d)\n", ev.xmotion.x, ev.xmotion.y );
		  			if( buttonPressed )
		  			{
			  			uiMsg.type    = TYPE_TOUCHSCREEN;
						uiMsg.uiEvent = EVENT_MOVE;
						uiMsg.param1  = ev.xmotion.x;
	    				uiMsg.param2  = ev.xmotion.y;
		  			}		  									
					break;
		  			
		  		default:
		  			//printf( "default (%d,%d)\n", ev.xmotion.x, ev.xmotion.y );
		  			break;
			}
		}
		    	
		mainDemo( &uiMsg );
		XFlush(xw->display);
		usleep(5000);		
    }
	
  	/* Destroy the created window */
  	TrashXWindow(xw);
  
  	/* kill the timer signal */
  	clear_timer();

  return 0;
}

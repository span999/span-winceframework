/*
*********************************************************************************************************
*                                                uC/GUI
*
----------------------------------------------------------------------
File        : LCDSDL.C
Purpose     : SDL display driver
              This driver does no perform any function. It can be
              used for measurement purpose.
----------------------------------------------------------------------   
Version-Date---Author-Explanation                                        
----------------------------------------------------------------------   

---------------------------END-OF-HEADER------------------------------
*/

#include "LCD_Private.h"      /* private modul definitions & config */
#include "GUI_Private.h"
#include "GUIDebug.h"

///for SDL
#include "SDLdefine.h"
#include <SDL.h>
#include "..\..\System\SDLDislpayEmulator\SDLDisplay.h"
#include "..\..\System\SDLDislpayEmulator\kbkeymap.h"

///#if (LCD_CONTROLLER == -2) \
///    && (!defined(WIN32) | defined(LCD_SIMCONTROLLER))
#ifdef USE_SDL_LCD



void TaskKbdHook( void * pvParameters )
{
	int iKey = 0;
	
	printf("  TaskKbdHook:\n");
	iKey = GUI_WaitKey();
	printf("  TaskKbdHook: keycode=%d\n", iKey);

}



//*****************************************************************************
//
//! Sends a IO message.
//
//*****************************************************************************

///static BOOL ioMsgUpdated = FALSE;
static bool ioMsgUpdated = FALSE;
///static GOL_MSG ioMsg; 

void TransIOMsg(unsigned long ulMessage, long lX, long lY)
{
    //
    // Add the message to the widget message queue.
    //
    //return(WidgetMessageQueueAdd(WIDGET_ROOT, ulMessage, lX, lY, 1, 1));
	
	///printf("\nIn TransIOMsg\n");

#if 1
	if( WIDGET_MSG_PTR_DOWN == ulMessage )
	{
		printf("  mice down: x=%d y=%d\n", lX, lY );
		///ioMsg.type = TYPE_TOUCHSCREEN;
		///ioMsg.uiEvent = EVENT_PRESS;
		///ioMsg.param1 = (SHORT)lX;
		///ioMsg.param2 = (SHORT)lY;
	}
	else
	if( WIDGET_MSG_PTR_UP == ulMessage )
	{
		printf("  mice up: x=%d y=%d\n", lX, lY );
		///ioMsg.type = TYPE_TOUCHSCREEN;
		///ioMsg.uiEvent = EVENT_RELEASE;
		///ioMsg.param1 = (SHORT)lX;
		///ioMsg.param2 = (SHORT)lY;
	}
	else
	if( WIDGET_MSG_PTR_MOVE == ulMessage )
	{
		///printf("  mice move: x=%d y=%d\n", lX, lY );
		///ioMsg.type = TYPE_TOUCHSCREEN;
		///ioMsg.uiEvent = EVENT_MOVE;
		///ioMsg.param1 = (SHORT)lX;
		///ioMsg.param2 = (SHORT)lY;
	}
	else
	if( WIDGET_MSG_KEY_DOWN == ulMessage )
	{
		SHORT	kbscan;
		SHORT	kbstat;
		
		kbscan = (lY&0xFFFF);
		kbstat = ((lY>>16)&0xFFFF);
		///printf("  keybd down: ID=%d kbscan=%d kbstat=0x%x\n", lX, kbscan, kbstat );
	#if 0	
		ioMsg.type = TYPE_KEYBOARD;
		ioMsg.uiEvent = EVENT_KEYSCAN;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)kbscan;

		/*re-mapping key, simulate the key pressed/repeased*/
		if( KB_KEY_1 == ioMsg.param2 )
		{	/*simulate the "BACK" key repeased*/
			ioMsg.param1 = ID_BUTTON_BACK;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			///printf("  keybd down: simulate BACK key\n");
		}
		else
		if( KB_KEY_2 == ioMsg.param2 )
		{	/*simulate the "NEXT" key repeased*/
			ioMsg.param1 = ID_BUTTON_NEXT;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			///printf("  keybd down: simulate NEXT key\n");
		}
		else
		if( KB_KEY_I == ioMsg.param2 )
		{	/*simulate the "UP" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_UP_HOLD;
			else
				ioMsg.param1 = ID_BTN_UP;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			///printf("  keybd down: simulate UP key\n");
		}
		else
		if( KB_KEY_O == ioMsg.param2 )
		{	/*simulate the "UP" key hold*/
			ioMsg.param1 = ID_BTN_UP_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			///printf("  keybd down: simulate UP_HOLD key\n");
		}
		else
		if( KB_KEY_K == ioMsg.param2 )
		{	/*simulate the "DOWN" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_DOWN_HOLD;
			else
				ioMsg.param1 = ID_BTN_DOWN;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			///printf("  keybd down: simulate DOWN key\n");
		}
		else
		if( KB_KEY_L == ioMsg.param2 )
		{	/*simulate the "DOWN" key hold*/
			ioMsg.param1 = ID_BTN_DOWN_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			///printf("  keybd down: simulate DOWN_HOLD key\n");
		}
		else
		if( KB_KEY_U == ioMsg.param2 )
		{	/*simulate the "EXIT" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_EXIT_HOLD;
			else
				ioMsg.param1 = ID_BTN_EXIT;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			///printf("  keybd down: simulate EXIT key\n");
		}
		else
		if( KB_KEY_Y == ioMsg.param2 )
		{	/*simulate the "EXIT" key hold*/
			ioMsg.param1 = ID_BTN_EXIT_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			///printf("  keybd down: simulate EXIT_HOLD key\n");
		}
		else
		if( KB_KEY_J == ioMsg.param2 )
		{	/*simulate the "ENTER" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_ENTER_HOLD;
			else
				ioMsg.param1 = ID_BTN_ENTER;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			///printf("  keybd down: simulate ENTER key\n");
		}
		else
		if( KB_KEY_H == ioMsg.param2 )
		{	/*simulate the "ENTER" key hold*/
			ioMsg.param1 = ID_BTN_ENTER_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			///printf("  keybd down: simulate ENTER_HOLD key\n");
		}
		else
			ioMsg.param1 = 0;

	#endif
		///tell GUI the key message
		///GUI_SendKeyMsg(int Key, int Pressed);
		GUI_SendKeyMsg(kbscan, (WIDGET_MSG_KEY_DOWN == ulMessage)?1:0 );

	}
	else
	if( WIDGET_MSG_KEY_UP == ulMessage )
	{
		SHORT	kbscan;
		SHORT	kbstat;
		
		kbscan = (lY&0xFFFF);
		kbstat = ((lY>>16)&0xFFFF);
		printf("  keybd up: ID=%d kbscan=%d kbstat=0x%x\n", lX, kbscan, kbstat );
		
		///GUI_SendKeyMsg(int Key, int Pressed);
		GUI_SendKeyMsg(kbscan, (WIDGET_MSG_KEY_DOWN == ulMessage)?1:0 );
	}
	else
	{
		printf("  io unknow: x=%d y=%d\n", lX, lY );
		///ioMsg.type = TYPE_UNKNOWN;
		///ioMsg.uiEvent = EVENT_INVALID;
		///ioMsg.param1 = (SHORT)lX;
		///ioMsg.param2 = (SHORT)lY;
	}
		
	ioMsgUpdated = TRUE;

	
#endif

#if 0
	/*
	TODO:
	*/
        //
        // Write this message into the message queue.
        //
        if( errQUEUE_FULL == xQueueSend( grlibQueue,(const void *)&message, 0 ))
        {
            g_ulMQOverflow++;
        }

	//     portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
#endif
        //
        // Success.
        //
        return;
}


/*********************************************************************
*
*      Text output for Logging, warnings and errors

  Logging - required only for higher debug levels
*/
void GUI_X_Log (const char *s)
{
	/*SIM_Log(s);*/ 
}

void GUI_X_Warn (const char *s)     
{ 
	/*SIM_Warn(s);*/
}

void GUI_X_ErrorOut (const char *s)     
{ 
	/*SIM_ErrorOut(s);*/ 
}


/*********************************************************************
*
*       SDL specified functions
*
**********************************************************************
*/

void Lock(SDL_Surface *screen)
{
	if ( SDL_MUSTLOCK(screen) )
	{
		if ( SDL_LockSurface(screen) < 0 )
		{
			//exit(1);
		}
	}
}


void Unlock(SDL_Surface *screen)
{
	if ( SDL_MUSTLOCK(screen) )
	{
		SDL_UnlockSurface(screen);
	}
}

/* LCD resolution set in LCDConf.h */
#define GetMaxX()   (LCD_XSIZE)
#define GetMaxY()   (LCD_YSIZE)


// global surface - screen
SDL_Surface *screen = NULL;

void Init_SDL_ScreenSurface(void)
{
	screen = SDL_SetVideoMode(GetMaxX(), GetMaxY(), 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	///screen = SDL_SetVideoMode(GetMaxX()+1, GetMaxY()+1, 8, SDL_HWSURFACE | SDL_DOUBLEBUF);
}


BOOL IsScreenValid( void )
{
	BOOL bRet = FALSE;

	if( NULL != screen )
		bRet = TRUE;
	else
	{
	#if 0
		///screen = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		screen = SDL_SetVideoMode(GetMaxX()+1, GetMaxY()+1, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		///screen = SDL_SetVideoMode(GetMaxX()+1, GetMaxY()+1, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
	#else
		Init_SDL_ScreenSurface();
	#endif
		if( NULL == screen )
		{
			printf(" Unable to set %dx%d video: %s\n", GetMaxX(), GetMaxY(), SDL_GetError());
			bRet = FALSE;
		}
		else
			printf(" Create to set %dx%d video: %s\n", GetMaxX(), GetMaxY(), SDL_GetError());
	}
	
	return bRet;
}


int TransColors( int PixelIndex )
{
	static int cc = 0x0fff;
	int tt = 0x0;

	///printf("\nTransColors=0x%x", PixelIndex);

	if( (-1) == PixelIndex )
	{
		;///return 0xffffff;
		cc = GUI_Index2Color(LCD_COLORINDEX);
	}
	else
	{
		///cc = GUI_Index2Color(LCD_COLORINDEX);
		cc = GUI_Index2Color(PixelIndex);
		///printf("\nTransColors=0x%x->0x%x", PixelIndex, cc);
	}

	/// reverse the define of RGB565CONVERT
	///cc = cc | ((_color.Val & 0x0000001F) << (0+3)) ;
	///cc = cc | ((_color.Val & 0x000007E0) << (3+2)) ;
	///cc = cc | ((_color.Val & 0x0000F800) << (5+3)) ;

	/// reverse the B,R of RGB888CONVERT
	tt = (cc&0x00ffff)|((cc<<16)&0xff0000);	///R->B
	tt = (tt&0xffff00)|((cc>>16)&0x0000ff);	///B->R
	
	///return cc;
	return tt;
}


void DrawPixel(SDL_Surface *screen, int x, int y, unsigned long val)
{

	Uint8 R;
	Uint8 G;
	Uint8 B;

	Uint32 color;

	// extract Red Green Blue colors
	R = (Uint8) (val >> 16) & 0xFF;
	G = (Uint8) (val >> 8 ) & 0xFF;
	B = (Uint8) (val >> 0 ) & 0xFF;

	color = SDL_MapRGB(screen->format, R, G, B);
///	Uint32 color = SDL_MapRGB(screen->format, R, G, B);
  
 
	switch (screen->format->BytesPerPixel)
	{
		case 1:
		{
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
			*bufp = color;
		}
		break;
		case 2:
		{
			Uint16 *bufp;
			bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
			*bufp = color;
		}
		break;
		case 3:
		{
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
			{
				bufp[0] = color;
				bufp[1] = color >> 8;
				bufp[2] = color >> 16;
			} else {
				bufp[2] = color;
				bufp[1] = color >> 8;
				bufp[0] = color >> 16;
			}
		}
		break;
		case 4:
		{
			Uint32 *bufp;
			bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
			*bufp = color;
		}
		break;
	}
  
}


static void displayRectFill(void *pvDisplayData,
							SHORT left, SHORT top, SHORT right, SHORT bottom,
                            unsigned long ulValue);

static void displayPixelDraw(void *pvDisplayData, long lX, long lY, unsigned long ulValue)
{
	
//	printf("%u-%u-%u ",lX, lY, ulValue);
#if 1
	Lock(screen);
	DrawPixel(screen, lX, lY, ulValue);
	Unlock(screen);
#else
	displayRectFill( NULL, lX, lY, lX, lY, ulValue );
#endif
//	SDL_UpdateRect(screen, lX, lY, 1, 1);

	
}


static void displayRectFill(void *pvDisplayData,
							SHORT left, SHORT top, SHORT right, SHORT bottom,
                            unsigned long ulValue)
{
    SDL_Rect rect;

    rect.x = (left>right)?right:left;
	rect.y = (top>bottom)?bottom:top;
	rect.w = abs(right - left) + 1;
	rect.h = abs(top - bottom) + 1;
	
	if( rect.w > GetMaxX() )
		rect.w = GetMaxX();

	if( rect.h > GetMaxY() )
		rect.h = GetMaxY();

		
	SDL_FillRect(screen, &rect, ulValue);
	
	//SDL_Flip(screen);
	
}





/*********************************************************************
*
*       _DrawBitLine1BPP
*/
static void _DrawBitLine1BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {

  LCD_PIXELINDEX Index0 = *(pTrans+0);
  LCD_PIXELINDEX Index1 = *(pTrans+1);
  x+=Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS|LCD_DRAWMODE_XOR)) {
  case 0:    /* Write mode */
    do {
      ///LCDSIM_SetPixelIndex(x++,y, (*p & (0x80>>Diff)) ? Index1 : Index0, LCD_DISPLAY_INDEX);
	  LCD_L0_SetPixelIndex(x++,y, (*p & (0x80>>Diff)) ? Index1 : Index0);
			if (++Diff==8) {
        Diff=0;
				p++;
			}
		} while (--xsize);
    break;
  case LCD_DRAWMODE_TRANS:
    do {
  		if (*p & (0x80>>Diff))
        ///LCDSIM_SetPixelIndex(x,y, Index1, LCD_DISPLAY_INDEX);
		LCD_L0_SetPixelIndex(x,y, Index1);
      x++;
			if (++Diff==8) {
        Diff=0;
				p++;
			}
		} while (--xsize);
    break;
  case LCD_DRAWMODE_XOR:;
    do {
  		if (*p & (0x80>>Diff)) {
        //int Pixel = LCDSIM_GetPixelIndex(x,y, LCD_DISPLAY_INDEX);
        ///LCDSIM_SetPixelIndex(x,y, LCD_NUM_COLORS-1-Pixel, LCD_DISPLAY_INDEX);
		//LCD_L0_SetPixelIndex(x,y, LCD_NUM_COLORS-1-Pixel);
		printf("dddd");
      }
      x++;
			if (++Diff==8) {
        Diff=0;
				p++;
			}
		} while (--xsize);
    break;
	}

}

/*********************************************************************
*
*       _DrawBitLine2BPP
*/
#if (LCD_MAX_LOG_COLORS > 2)
static void _DrawBitLine2BPP(int x, int y, U8 const * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
#if 0
  LCD_PIXELINDEX Pixels = *p;
  int CurrentPixel = Diff;
  x += Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      if (pTrans) {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
          SETPIXEL(x++, y, PixelIndex);
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      } else {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          SETPIXEL(x++, y, Index);
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      }
      break;
    case LCD_DRAWMODE_TRANS:
      if (pTrans) {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          if (Index) {
            LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
            SETPIXEL(x, y, PixelIndex);
          }
          x++;
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      } else {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          if (Index) {
            SETPIXEL(x, y, Index);
          }
          x++;
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      }
      break;
  }
#endif
}
#endif

/*********************************************************************
*
*       _DrawBitLine4BPP
*/
#if (LCD_MAX_LOG_COLORS > 4)
static void _DrawBitLine4BPP(int x, int y, U8 const * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
#if 0
  LCD_PIXELINDEX Pixels = *p;
  int CurrentPixel = Diff;
  x += Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      if (pTrans) {
        do {
          int Shift = (1 - CurrentPixel) << 2;
          int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
          LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
          SETPIXEL(x++, y, PixelIndex);
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      } else {
        do {
          int Shift = (1 - CurrentPixel) << 2;
          int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
          SETPIXEL(x++, y, Index);
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      }
      break;
    case LCD_DRAWMODE_TRANS:
      if (pTrans) {
        do {
          int Shift = (1 - CurrentPixel) << 2;
          int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
          if (Index) {
            LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
            SETPIXEL(x, y, PixelIndex);
          }
          x++;
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      } else {
        do {
          int Shift = (1 - CurrentPixel) << 2;
          int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
          if (Index) {
            SETPIXEL(x, y, Index);
          }
          x++;
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      }
      break;
  }
#endif
}
#endif

/*********************************************************************
*
*       _DrawBitLine8BPP
*/
#if (LCD_MAX_LOG_COLORS > 16)
static void _DrawBitLine8BPP(int x, int y, U8 const*p, int xsize, const LCD_PIXELINDEX*pTrans) {
#if 1
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    if (pTrans) {
      for (;xsize > 0; xsize--,x++,p++) {
        pixel = *p;
        ///SETPIXEL(x, y, *(pTrans+pixel));
		displayPixelDraw( NULL, x, y, TransColors(*(pTrans+pixel)) );
      }
    } else {
      for (;xsize > 0; xsize--,x++,p++) {
        ///SETPIXEL(x, y, *p);
		displayPixelDraw( NULL, x, y, TransColors(*p) );
      }
    }
  } else {   /* Handle transparent bitmap */
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          ///SETPIXEL(x+0, y, *(pTrans+pixel));
		  displayPixelDraw( NULL, x+0, y, TransColors(*(pTrans+pixel)) );
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          ///SETPIXEL(x+0, y, pixel);
		  displayPixelDraw( NULL, x+0, y, TransColors(pixel) );
        }
      }
    }
  }
#endif
}
#endif

/*********************************************************************
*
*       _DrawBitLine16BPP
*/
#if (LCD_BITSPERPIXEL > 8)
static void _DrawBitLine16BPP(int x, int y, U16 const*p, int xsize, const LCD_PIXELINDEX*pTrans) {
#if 0
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    if (pTrans) {
      for (;xsize > 0; xsize--,x++,p++) {
        pixel = *p;
        SETPIXEL(x, y, *(pTrans+pixel));
      }
    } else {
      for (;xsize > 0; xsize--,x++,p++) {
        SETPIXEL(x, y, *p);
      }
    }
  } else {   /* Handle transparent bitmap */
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SETPIXEL(x+0, y, *(pTrans+pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SETPIXEL(x+0, y, pixel);
        }
      }
    }
  }
#endif
}
#endif



/*********************************************************************
*
*       Exported functions
*
**********************************************************************
*/

/*********************************************************************
*
*       LCD_L0_SetPixelIndex
*/
void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex) {
	///printf("\nLCD_L0_SetPixelIndex");
	if( IsScreenValid() )
	{
		displayPixelDraw( NULL, x, y, TransColors(PixelIndex) );
		///displayPixelDraw( NULL, x, y, TransColors(-1) );
		///displayRectFill( NULL, x, y, x, y, TransColors(-1) );
	}
	GUI_USE_PARA(x);
	GUI_USE_PARA(y);
	GUI_USE_PARA(PixelIndex);
}

/*********************************************************************
*
*       LCD_L0_GetPixelIndex
*/
unsigned int LCD_L0_GetPixelIndex(int x, int y) {
	//printf("\nLCD_L0_GetPixelIndex");
	GUI_USE_PARA(x);
	GUI_USE_PARA(y);
	return 0;
}

/*********************************************************************
*
*       LCD_L0_XorPixel
*/
void LCD_L0_XorPixel(int x, int y) {
	printf("\nLCD_L0_XorPixel");
	GUI_USE_PARA(x);
	GUI_USE_PARA(y);
}

/*********************************************************************
*
*       LCD_L0_DrawHLine
*/
void LCD_L0_DrawHLine(int x0, int y,  int x1) {
	///printf("\nLCD_L0_DrawHLine");
#if 0	
	if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
		for (;x0 <= x1; x0++) {
			;///XORPIXEL(x0, y);
		}
	} else {
		for (;x0 <= x1; x0++) {
			///SETPIXEL(x0, y, LCD_COLORINDEX);
			if( IsScreenValid() )
				///displayPixelDraw( NULL, x0, y, TransColors(-1) );
				displayPixelDraw( NULL, x0, y, GUI_Context.Color );
		}
	}
#else
	displayRectFill( NULL, x0, y, x1, y, TransColors(-1) );
#endif	
	GUI_USE_PARA(x0);
	GUI_USE_PARA(y);
	GUI_USE_PARA(x1);
}

/*********************************************************************
*
*       LCD_L0_DrawVLine
*/
void LCD_L0_DrawVLine(int x, int y0,  int y1) {
	///printf("\nLCD_L0_DrawVLine");
	displayRectFill( NULL, x, y0, x, y1, TransColors(-1) );
	GUI_USE_PARA(x);
	GUI_USE_PARA(y0);
	GUI_USE_PARA(y1);
}

/*********************************************************************
*
*       LCD_L0_FillRect
*/
void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
	///printf("\nLCD_L0_FillRect");
	if( IsScreenValid() ) {
		///displayRectFill( NULL, left, top, right, bottom, _color.Val );
		///displayRectFill( NULL, x0, y0, x1, y1, TransColors(LCD_DISPLAY_INDEX) );
		displayRectFill( NULL, x0, y0, x1, y1, TransColors(-1) );
	}
	GUI_USE_PARA(x0);
	GUI_USE_PARA(y0);
	GUI_USE_PARA(x1);
	GUI_USE_PARA(y1);
}

/*********************************************************************
*
*       LCD_L0_DrawBitmap
*/
void LCD_L0_DrawBitmap(int x0, int y0,
                       int xsize, int ysize,
                       int BitsPerPixel, 
                       int BytesPerLine,
                       const U8 GUI_UNI_PTR * pData, int Diff,
                       const LCD_PIXELINDEX* pTrans)
{

  int i;

///	printf("\nLCD_L0_DrawBitmap, x=%d[%d] y=%d[%d] bpp=%d bpl=%d \n", x0, xsize, y0, ysize, BitsPerPixel, BytesPerLine);

  /*
     Use DrawBitLineXBPP
  */
  for (i=0; i<ysize; i++) {
    switch (BitsPerPixel) {
    case 1:
      _DrawBitLine1BPP(x0, i+y0, pData, Diff, xsize, pTrans);
      break;
    #if (LCD_MAX_LOG_COLORS > 2)
      case 2:
        _DrawBitLine2BPP(x0, i+y0, pData, Diff, xsize, pTrans);
        break;
    #endif
    #if (LCD_MAX_LOG_COLORS > 4)
      case 4:
        _DrawBitLine4BPP(x0, i+y0, pData, Diff, xsize, pTrans);
        break;
    #endif
    #if (LCD_MAX_LOG_COLORS > 16)
      case 8:
        _DrawBitLine8BPP(x0, i+y0, pData, xsize, pTrans);
        break;
    #endif
    #if (LCD_BITSPERPIXEL > 8)
      case 16:
        _DrawBitLine16BPP(x0, i+y0, (const U16 *)pData, xsize, pTrans);
        break;
    #endif
    }
    pData += BytesPerLine;
  }	
	
	GUI_USE_PARA(x0);
	GUI_USE_PARA(y0);
	GUI_USE_PARA(xsize);
	GUI_USE_PARA(ysize);
	GUI_USE_PARA(BitsPerPixel);
	GUI_USE_PARA(BytesPerLine);
	GUI_USE_PARA(pData);
	GUI_USE_PARA(Diff);
	GUI_USE_PARA(pTrans);
}

/*********************************************************************
*
*       LCD_L0_SetOrg
*/
void LCD_L0_SetOrg(int x, int y) {
  GUI_USE_PARA(x);
  GUI_USE_PARA(y);
}

/*********************************************************************
*
*       LCD_On / LCD_Off
*/
void LCD_On (void) {}
void LCD_Off(void) {}

/*********************************************************************
*
*       LCD_L0_Init
*/
int LCD_L0_Init(void) {
	/// init the SDL
	printf("\nLCD_L0_Init");
	return 0;
}

/*********************************************************************
*
*       LCD_L0_SetLUTEntry
*/
void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR Color) {
///	printf("\nLCD_L0_SetLUTEntry");
	GUI_USE_PARA(Pos);
	GUI_USE_PARA(Color);
}

#else

void LCDNull_c(void);
void LCDNull_c(void) {} /* avoid empty object files */

#endif /* (LCD_CONTROLLER == -2) */

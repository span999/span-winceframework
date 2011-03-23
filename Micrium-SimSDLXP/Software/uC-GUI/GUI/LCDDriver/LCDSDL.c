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
#include "..\..\System\SDLDislpayEmulator\SDLDisplay.h"

///#if (LCD_CONTROLLER == -2) \
///    && (!defined(WIN32) | defined(LCD_SIMCONTROLLER))
#ifdef USE_SDL_LCD





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
	
	printf("\nIn TransIOMsg\n");

#if 0
	if( WIDGET_MSG_PTR_DOWN == ulMessage )
	{
		printf("  mice down: x=%d y=%d\n", lX, lY );
		ioMsg.type = TYPE_TOUCHSCREEN;
		ioMsg.uiEvent = EVENT_PRESS;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)lY;
	}
	else
	if( WIDGET_MSG_PTR_UP == ulMessage )
	{
		printf("  mice up: x=%d y=%d\n", lX, lY );
		ioMsg.type = TYPE_TOUCHSCREEN;
		ioMsg.uiEvent = EVENT_RELEASE;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)lY;
	}
	else
	if( WIDGET_MSG_PTR_MOVE == ulMessage )
	{
		///printf("  mice move: x=%d y=%d\n", lX, lY );
		ioMsg.type = TYPE_TOUCHSCREEN;
		ioMsg.uiEvent = EVENT_MOVE;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)lY;
	}
	else
	if( WIDGET_MSG_KEY_DOWN == ulMessage )
	{
		SHORT	kbscan;
		SHORT	kbstat;
		
		kbscan = (lY&0xFFFF);
		kbstat = ((lY>>16)&0xFFFF);
		printf("  keybd down: ID=%d kbscan=%d kbstat=0x%x\n", lX, kbscan, kbstat );
		ioMsg.type = TYPE_KEYBOARD;
		ioMsg.uiEvent = EVENT_KEYSCAN;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)kbscan;
		
		/*re-mapping key, simulate the key pressed/repeased*/
		if( KB_KEY_1 == ioMsg.param2 )
		{	/*simulate the "BACK" key repeased*/
			ioMsg.param1 = ID_BUTTON_BACK;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate BACK key\n");
		}
		else
		if( KB_KEY_2 == ioMsg.param2 )
		{	/*simulate the "NEXT" key repeased*/
			ioMsg.param1 = ID_BUTTON_NEXT;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate NEXT key\n");
		}
		else
		if( KB_KEY_I == ioMsg.param2 )
		{	/*simulate the "UP" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_UP_HOLD;
			else
				ioMsg.param1 = ID_BTN_UP;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate UP key\n");
		}
		else
		if( KB_KEY_O == ioMsg.param2 )
		{	/*simulate the "UP" key hold*/
			ioMsg.param1 = ID_BTN_UP_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate UP_HOLD key\n");
		}
		else
		if( KB_KEY_K == ioMsg.param2 )
		{	/*simulate the "DOWN" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_DOWN_HOLD;
			else
				ioMsg.param1 = ID_BTN_DOWN;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate DOWN key\n");
		}
		else
		if( KB_KEY_L == ioMsg.param2 )
		{	/*simulate the "DOWN" key hold*/
			ioMsg.param1 = ID_BTN_DOWN_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate DOWN_HOLD key\n");
		}
		else
		if( KB_KEY_U == ioMsg.param2 )
		{	/*simulate the "EXIT" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_EXIT_HOLD;
			else
				ioMsg.param1 = ID_BTN_EXIT;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate EXIT key\n");
		}
		else
		if( KB_KEY_Y == ioMsg.param2 )
		{	/*simulate the "EXIT" key hold*/
			ioMsg.param1 = ID_BTN_EXIT_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate EXIT_HOLD key\n");
		}
		else
		if( KB_KEY_J == ioMsg.param2 )
		{	/*simulate the "ENTER" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_ENTER_HOLD;
			else
				ioMsg.param1 = ID_BTN_ENTER;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate ENTER key\n");
		}
		else
		if( KB_KEY_H == ioMsg.param2 )
		{	/*simulate the "ENTER" key hold*/
			ioMsg.param1 = ID_BTN_ENTER_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate ENTER_HOLD key\n");
		}
		else
			ioMsg.param1 = 0;
	}
	else
	{
		printf("  io unknow: x=%d y=%d\n", lX, lY );
		ioMsg.type = TYPE_UNKNOWN;
		ioMsg.uiEvent = EVENT_INVALID;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)lY;
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
*       Exported functions
*
**********************************************************************
*/

/*********************************************************************
*
*       LCD_L0_SetPixelIndex
*/
void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex) {
	printf("\nLCD_L0_SetPixelIndex");
	GUI_USE_PARA(x);
	GUI_USE_PARA(y);
	GUI_USE_PARA(PixelIndex);
}

/*********************************************************************
*
*       LCD_L0_GetPixelIndex
*/
unsigned int LCD_L0_GetPixelIndex(int x, int y) {
	printf("\nLCD_L0_GetPixelIndex");
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
	printf("\nLCD_L0_DrawHLine");
	GUI_USE_PARA(x0);
	GUI_USE_PARA(y);
	GUI_USE_PARA(x1);
}

/*********************************************************************
*
*       LCD_L0_DrawVLine
*/
void LCD_L0_DrawVLine(int x, int y0,  int y1) {
	printf("\nLCD_L0_DrawVLine");
	GUI_USE_PARA(x);
	GUI_USE_PARA(y0);
	GUI_USE_PARA(y1);
}

/*********************************************************************
*
*       LCD_L0_FillRect
*/
void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
	printf("\nLCD_L0_FillRect");
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
	printf("\nLCD_L0_DrawBitmap");
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
	printf("\nLCD_L0_SetLUTEntry");
	GUI_USE_PARA(Pos);
	GUI_USE_PARA(Color);
}

#else

void LCDNull_c(void);
void LCDNull_c(void) {} /* avoid empty object files */

#endif /* (LCD_CONTROLLER == -2) */

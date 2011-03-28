/*
*********************************************************************************************************
*                                                µC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2000, SEGGER Microcontroller Systeme GmbH          
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed 
*              in any way. We appreciate your understanding and fairness.
*
* File        : REnoUImain.c
* Purpose     : Application program in Reno project
*********************************************************************************************************
*/

#if defined(RENOUI_PROJ)

#include "LCD_ConfDefaults.h"  /* valid LCD configuration */
#include "GUI.h"
#if GUI_WINSUPPORT
//  #include "PROGBAR.h"
//  #include "LISTBOX.h"
  #include "FRAMEWIN.h"
  #include "BUTTON.h"
  #include "TEXT.h"
#endif

void FrameCenter( void );
void StartWindow( int iOption );
void SecondWindow( int iOption );
void BootWindow( int iOption );


#ifndef NULL
	#define NULL (0)
#endif


static TEXT_Handle		ghTEXT = 0;



void spSetDefaultEffect ( void )
{
	///WIDGET_SetDefaultEffect(&WIDGET_Effect_3D);
	WIDGET_SetDefaultEffect(&WIDGET_Effect_Simple);
	///WIDGET_SetDefaultEffect(&WIDGET_Effect_None);
	
	FRAMEWIN_SetDefaultBorderSize( 0 );


}



/*
  *******************************************************************
  *
  *              main()
  *
  *******************************************************************
*/

#if 0
void MainTask(void) {
#else
void MainTask_RenoUItest(void) {
#endif
	int Cnt =0;
	int i;
///	int YPos;
	int LCDXSize = LCD_GET_XSIZE();
	int LCDYSize = LCD_GET_YSIZE();
///	const GUI_BITMAP *pBitmap;


	printf("GUI_Init()!!!!!!!!!!!!!!!!!!\n");
	OS_Printf("GUI_Init()!!!!!!!!!!!!!!!!!!\n");
	
	///
	GUI_Init();

	///clear
	GUI_SetBkColor(GUI_RED);
	GUI_Clear();
	///GUI_Delay(1000);
	GUI_Delay(100);
	
	///clear
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	///GUI_Delay(1000);
	GUI_Delay(100);
	
	
	///draw line
	GUI_SetColor(GUI_WHITE);

	for (i=0; i<170; i+=10) {
		GUI_DrawHLine(i,0,100);
		GUI_DispStringAt("Line ",0,i);
		GUI_DispDecMin(i);
	}

	GUI_Delay(100);
	
	for (i=60; i<150; i+=10) {
		GUI_DrawVLine(i,0,168);
	}
	
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE);
  
	for (i=0; i<160; i++) {
		int len = (i<80) ? i : 160-i;
		GUI_DrawHLine(i,20,len+20);
	}

	GUI_Delay(100);
	GUI_Clear();

	FrameCenter();

#if 0	
	if (LCD_GET_YSIZE()>(100+bmMicriumLogo_1bpp.YSize)) {
		pBitmap=&bmMicriumLogo;
	} else {
		GUI_SetColor(GUI_BLUE);
		pBitmap=&bmMicriumLogo_1bpp;
	}

	GUI_DrawBitmap(pBitmap,(LCDXSize-pBitmap->XSize)/2,10);
	YPos=20+pBitmap->YSize;
	GUI_SetFont(&GUI_FontComic24B_1);
	GUI_DispStringHCenterAt("www.micrium.com",LCDXSize/2,YPos);
	GUI_Delay(1000);
	GUI_SetColor(GUI_RED);
	GUI_DispStringHCenterAt("?2004\n", LCDXSize/2,YPos+30);
	GUI_SetFont(&GUI_Font10S_1);
	GUI_DispStringHCenterAt("Micriµm Inc.",LCDXSize/2,YPos+60);;
	GUI_Delay(1000);
#endif

}


///static void _UpdateCmdWin(void) {
static void _UpdateCmdWin(FRAMEWIN_Handle hFrame) {
  #if GUI_WINSUPPORT
    ///WM_InvalidateWindow(_ahInfoWin[1]);
	WM_InvalidateWindow(hFrame);
	WM_DeleteWindow(hFrame);
	///GUI_MEMDEV_Clear(NULL);
  #endif
}


#if (GUI_WINSUPPORT)
static void _cbCmdWin(WM_MESSAGE* pMsg)
{
static int _iTest, _iTestMinor;
	int Key = 0;

	printf("_cbCmdWin() ==> %d \n", pMsg->MsgId);
	switch (pMsg->MsgId) {
		case WM_CREATE:
			printf("_cbCmdWin() WM_CREATE!!!!!!!!!!!!!!!!!\n");
			break;
		case WM_MOVE:
			printf("_cbCmdWin() WM_MOVE!!!!!!!!!!!!!!!!!\n");
			break;
		case WM_SIZE:
			printf("_cbCmdWin() WM_SIZE!!!!!!!!!!!!!!!!!\n");
			break;

		case WM_TOUCH:
			break;
		case WM_NOTIFY_PARENT:
			break;
		case WM_KEY:
			printf("_cbCmdWin() WM_KEY!!!!!!!!!!!!!!!!!\n");
		#if 0
			if (((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt > 0) {
				Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
					if (Key == GUI_KEY_INSERT) {
						WM_HWIN hWin;
						hWin = WM_GetDialogItem(WM_GetParent(pMsg->hWin), ID_OVERWRITE);
						_Overwrite ^= 1;
						_SetButtonState(hWin, _Overwrite);
					}
			}
		#else
			if (((WM_KEY_INFO*)(pMsg->Data.p))->Key > 0) {
				Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
				printf("_cbCmdWin() WM_KEY=%d[%c]\n", Key, Key);
				
				if( 'u' == Key ) 
					GUI_DispStringAt("-exit key-", 0, 80);
				else	
				if( 'y' == Key ) 
					GUI_DispStringAt("-exit key-hold", 0, 80);
				else	
				if( 'i' == Key ) 
					GUI_DispStringAt("-up key-", 0, 80);
				else	
				if( 'o' == Key ) 
					GUI_DispStringAt("-up key-hold", 0, 80);
				else	
				if( 'j' == Key ) 
					GUI_DispStringAt("-enter key-", 0, 80);
				else	
				if( 'h' == Key ) 
					GUI_DispStringAt("-enter key-hold", 0, 80);
				else	
				if( 'k' == Key ) 
					GUI_DispStringAt("-down key-", 0, 80);
				else	
				if( 'l' == Key ) 
					GUI_DispStringAt("-down key-hold", 0, 80);
				
				if( ghTEXT != 0 )
				{
					if( 'u' == Key ) 
						TEXT_SetText( ghTEXT, "-exit key-");
					else	
					if( 'y' == Key ) 
						TEXT_SetText( ghTEXT, "-exit key-hold");
					else	
					if( 'i' == Key ) 
						TEXT_SetText( ghTEXT, "-up key-");
					else	
					if( 'o' == Key ) 
						TEXT_SetText( ghTEXT, "-up key-hold");
					else	
					if( 'j' == Key ) 
						TEXT_SetText( ghTEXT, "-enter key-");
					else	
					if( 'h' == Key ) 
						TEXT_SetText( ghTEXT, "-enter key-hold");
					else	
					if( 'k' == Key ) 
						TEXT_SetText( ghTEXT, "-down key-");
					else	
					if( 'l' == Key ) 
						TEXT_SetText( ghTEXT, "-down key-hold");
				}
				
			}
		#endif
			
			break;
		case WM_GET_ID:
			break;
		case WM_PAINT:
			/* Update info in command window */
			GUI_SetBkColor(GUI_GRAY);
			GUI_Clear();
			GUI_DispStringAt("Demo ", 0, 0);
			GUI_DispDecMin(_iTest + 1);
			GUI_DispString(".");
			GUI_DispDecMin(_iTestMinor);
			GUI_DispString("/");
//    		GUI_DispDecMin(countof(_apfTest));
		default:
			WM_DefaultProc(pMsg);
	}
}
#endif


extern GUI_CONST_STORAGE GUI_BITMAP bmStartup_Screen_1;
void BootWindow( int iOption )
{
	GUI_DispStringAt("BootWindow\n", 10, 65 );
	GUI_Delay(300);
	GUI_DrawBitmap(&bmStartup_Screen_1, 0, 0);
	GUI_Delay(300);
}


void StartWindow( int iOption )
{
static BUTTON_Handle   _ahButton[2];
static FRAMEWIN_Handle _ahFrameWin[3];
static FRAMEWIN_Handle _ahInfoWin[2];
static int _ButtonSizeX, _ButtonSizeY;


	spSetDefaultEffect();

    /* Calculate position and size of FrameWin[1] */
///    _ButtonSizeX = 60;
///    _ButtonSizeY = 40;
    _ButtonSizeX = 25*iOption;
    _ButtonSizeY = 20*iOption;

    /* Create the control window incl. buttons */
	///create frame
    ///_ahFrameWin[1] = FRAMEWIN_Create("Control", &_cbCmdWin, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0, LCD_GetXSize(), LCD_GetYSize());
	_ahFrameWin[1] = FRAMEWIN_CreateEx(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_HWIN_NULL, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0, "Control", &_cbCmdWin);
	
	///adjust the title height
	FRAMEWIN_SetTitleHeight( _ahFrameWin[1], 25);
	
	WM_SetCallback(_ahFrameWin[1], _cbCmdWin);
	WM_SetFocus(_ahFrameWin[1]);
	
	///get window handle of the frame, there is always a window created while frame created.
    _ahInfoWin[1] = WM_GetClientWindow(_ahFrameWin[1]);
	///create button within windows of frame
    _ahButton[0] = BUTTON_CreateAsChild(10, 20, _ButtonSizeX, _ButtonSizeY, _ahInfoWin[1], 'H', WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
    _ahButton[1] = BUTTON_CreateAsChild(10, 90, _ButtonSizeX, _ButtonSizeY, _ahInfoWin[1], 'N', WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
    BUTTON_SetText(_ahButton[0], "Halt");
    BUTTON_SetText(_ahButton[1], "Next");
	
	ghTEXT = TEXT_CreateEx(10, 148, 70, 20, _ahInfoWin[1], WM_CF_SHOW, GUI_TA_HCENTER, 0, "test");
	
	WM_ExecIdle();
	
	GUI_Delay(1000);
    ///_UpdateCmdWin();
	///_UpdateCmdWin(_ahInfoWin[1]);
	_UpdateCmdWin(_ahFrameWin[1]);		///kill frame will kill all window in
	GUI_SetBkColor(GUI_GRAY);
	GUI_Clear();	
    
	///WM_ExecIdle();
}


void SecondWindow( int iOption )
{
static FRAMEWIN_Handle _ahFrameWin[3];
static FRAMEWIN_Handle _ahInfoWin[2];
    int xSize, ySize, xPos, yPos;


	spSetDefaultEffect();

    /* Calculate position and size of FrameWin[0] */
    ///xSize = LCD_GetXSize() / 2;
	xSize = LCD_GetXSize();
    ///ySize = 65;
	ySize = LCD_GetYSize() ;
    ///xPos  = LCD_GetXSize() - xSize;
	xPos = 0;
    yPos  = 0;
    /* Create info window and run the individual demos */
	///create frame
    ///_ahFrameWin[0] = FRAMEWIN_Create("emWin Demo", NULL, WM_CF_STAYONTOP, xPos, yPos, xSize, ySize);
	_ahFrameWin[0] = FRAMEWIN_CreateEx(xPos, yPos, xSize, ySize, WM_HWIN_NULL, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0, "Win Demo", NULL);
//    _ahInfoWin[0] = WM_GetFirstChild(_ahFrameWin[0]);
	///create window within frame
    ///_ahInfoWin[0] = WM_CreateWindowAsChild(0, 0, 0, 0, WM_GetFirstChild(_ahFrameWin[0]), WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0);
	_ahInfoWin[0] = WM_CreateWindowAsChild(xPos, yPos-20, xSize, ySize-20, WM_GetFirstChild(_ahFrameWin[0]), WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0);

    WM_ExecIdle();

	GUI_Delay(300);
    ///_UpdateCmdWin();
	///_UpdateCmdWin(WM_GetFirstChild(_ahFrameWin[0]));
	_UpdateCmdWin(_ahFrameWin[0]);
	GUI_SetBkColor(GUI_GRAY);
	GUI_Clear();
}


void FontWindow( int iOption )
{
  int NumColors = LCD_GetDevCap(LCD_DEVCAP_NUMCOLORS);
  int Lines = (NumColors - 1) / 16 + 1;
  int y0;
  #if (LCD_BITSPERPIXEL > 8)
    int XStep = LCD_XSIZE / 256;
    int YStep = NumColors / 256;
  #endif
  int i;
  #if GUIDEMO_LARGE
    y0 = 70;
  #elif !(GUIDEMO_TINY)
    #if (LCD_YSIZE < 120)
      y0 = 50;
    #else
      y0 = 55;
    #endif
  #else
    y0 = 16;
  #endif
//  GUIDEMO_ShowIntro("Available colors",
//                    "Shows the first of the"
//                    "\navailable colors in the"
//                    "\nsimulated configuration");
	printf("\nFontWindow\n");
	spSetDefaultEffect();

  GUI_SetColor(GUI_WHITE);
  GUI_SetBkColor(GUI_BLACK); 
  GUI_Clear();
	GUI_SetFont(&GUI_Font8x16);
	GUI_DispString("Available colors\n");
  #if !(GUIDEMO_TINY)
	  GUI_SetFont(&GUI_Font8_ASCII);
    #ifdef LCD_CONTROLLER
      GUI_DispString("\nLCD_CONTROLLER: ");
      GUI_DispDecMin(LCD_CONTROLLER);
      GUI_DispString("\n");
    #endif
    GUI_DispDecMin(LCD_BITSPERPIXEL);
    GUI_DispString(" bpp");
    #ifdef LCD_BUSWIDTH
      GUI_DispString(", ");
      GUI_DispDecMin(LCD_BUSWIDTH);
      GUI_DispString(" bit bus");
    #endif
    GUI_DispString(", ");
    GUI_DispDecMin(NumColors);
    GUI_DispString(" colors\n");
    #if (LCD_FIXEDPALETTE) 
      GUI_DispString("Fixed palette: ");
      GUI_DispDecMin(LCD_FIXEDPALETTE);
    #else
      GUI_DispString("(Custom)");
    #endif
  #endif
  #if (LCD_BITSPERPIXEL <= 8)
  {
    int XStep = LCD_XSIZE / 16;
    #if (LCD_YSIZE < 320)
      int YStep = (LCD_YSIZE - y0) / Lines;
    #else
      int YStep = (LCD_YSIZE - y0 - 60) / Lines;
    #endif
    int dy = (YStep <= 3) ? 1 : 2;
	  for (i = 0; i < NumColors; i++) {
      GUI_SetColorIndex(i);
      GUI_FillRect((i%16) * XStep + 2,
                   (i/16) * YStep + y0 + 1,
                   (i%16 + 1) * XStep - 1,
                   (i/16 + 1) * YStep + y0 - dy);
    }
  }
  #else
  {
	  for (i = 0; i < NumColors; i++) {
      GUI_SetColorIndex(i);
      GUI_DrawPoint((U8)(i) + ((LCD_XSIZE - 1 - 256) >> 1), (i >> 8) + y0);
    }
  }
  #endif
  GUI_Delay(300);

}




void spClearScreen( void )
{
	GUI_SetBkColor(GUI_GRAY);
	GUI_Clear();
	GUI_Delay(100);
}


void FrameCenter( void )
{
	int iLoop = 3;
    GUI_CONTEXT ContextOld;


	spClearScreen();

	while( iLoop > 0 )
	{
		spClearScreen();
		
		GUI_SaveContext(&ContextOld);
		BootWindow(0);
		spClearScreen();
		GUI_RestoreContext(&ContextOld);
	#if 1
		GUI_SaveContext(&ContextOld);
		StartWindow( iLoop );
		spClearScreen();
		GUI_RestoreContext(&ContextOld);
	#endif	
		GUI_SaveContext(&ContextOld);
		SecondWindow( iLoop );
		spClearScreen();
		GUI_RestoreContext(&ContextOld);
		
		GUI_SaveContext(&ContextOld);
		FontWindow( iLoop );
		spClearScreen();
		GUI_RestoreContext(&ContextOld);
		
		iLoop--;
	}
	
}




#endif	/* #if defined(TEST_PROJ) */
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
#endif

void FrameCenter( void );
void StartWindow( int iOption );
void SecondWindow( int iOption );


#ifndef NULL
	#define NULL (0)
#endif

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

  switch (pMsg->MsgId) {
  case WM_PAINT:
    /* Update info in command window */
    GUI_SetBkColor(GUI_GRAY);
    GUI_Clear();
    GUI_DispStringAt("Demo ", 0, 0);
    GUI_DispDecMin(_iTest + 1);
    GUI_DispString(".");
    GUI_DispDecMin(_iTestMinor);
    GUI_DispString("/");
//    GUI_DispDecMin(countof(_apfTest));
  default:
    WM_DefaultProc(pMsg);
  }
}
#endif



void StartWindow( int iOption )
{
static BUTTON_Handle   _ahButton[2];
static FRAMEWIN_Handle _ahFrameWin[3];
static FRAMEWIN_Handle _ahInfoWin[2];
static int _ButtonSizeX, _ButtonSizeY;

    /* Calculate position and size of FrameWin[1] */
///    _ButtonSizeX = 60;
///    _ButtonSizeY = 40;
    _ButtonSizeX = 25*iOption;
    _ButtonSizeY = 20*iOption;

    /* Create the control window incl. buttons */
	///create frame
    ///_ahFrameWin[1] = FRAMEWIN_CreateEx("Control", &_cbCmdWin, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0, LCD_GetXSize(), LCD_GetYSize());
	_ahFrameWin[1] = FRAMEWIN_CreateEx(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_HWIN_NULL, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0, "Control", &_cbCmdWin);
	///get frame handle
    _ahInfoWin[1] = WM_GetClientWindow(_ahFrameWin[1]);
	///create button within frame handle
    _ahButton[0] = BUTTON_CreateAsChild(10, 20, _ButtonSizeX, _ButtonSizeY, _ahInfoWin[1], 'H', WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
    _ahButton[1] = BUTTON_CreateAsChild(10, 90, _ButtonSizeX, _ButtonSizeY, _ahInfoWin[1], 'N', WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
    BUTTON_SetText(_ahButton[0], "Halt");
    BUTTON_SetText(_ahButton[1], "Next");
	
	WM_ExecIdle();
	
	GUI_Delay(100);
    ///_UpdateCmdWin();
	_UpdateCmdWin(_ahInfoWin[1]);
	GUI_SetBkColor(GUI_GRAY);
	GUI_Clear();	
    
	///WM_ExecIdle();
}


void SecondWindow( int iOption )
{
static FRAMEWIN_Handle _ahFrameWin[3];
static FRAMEWIN_Handle _ahInfoWin[2];
    int xSize, ySize, xPos, yPos;

    /* Calculate position and size of FrameWin[0] */
    ///xSize = LCD_GetXSize() / 2;
	xSize = LCD_GetXSize();
    ///ySize = 65;
	ySize = LCD_GetYSize() ;
    ///xPos  = LCD_GetXSize() - xSize;
	xPos = 0;
    yPos  = 0;
    /* Create info window and run the individual demos */
    _ahFrameWin[0] = FRAMEWIN_Create("emWin Demo", NULL, WM_CF_STAYONTOP, xPos, yPos, xSize, ySize);
//    _ahInfoWin[0] = WM_GetFirstChild(_ahFrameWin[0]);
    _ahInfoWin[0] = WM_CreateWindowAsChild(0, 0, 0, 0, WM_GetFirstChild(_ahFrameWin[0]), WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0);
	
    WM_ExecIdle();

	GUI_Delay(100);
    ///_UpdateCmdWin();
	_UpdateCmdWin(WM_GetFirstChild(_ahFrameWin[0]));
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
  GUIDEMO_Delay(2500);

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
    
	
	while( iLoop > 0 )
	{
	#if 0
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
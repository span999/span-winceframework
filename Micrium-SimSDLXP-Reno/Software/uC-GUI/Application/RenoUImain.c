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


#include "GUI.h"
#if GUI_WINSUPPORT
//  #include "PROGBAR.h"
//  #include "LISTBOX.h"
  #include "FRAMEWIN.h"
  #include "BUTTON.h"
#endif

void WindowMain( void );



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

	GUI_Delay(1000);
	
	for (i=60; i<150; i+=10) {
		GUI_DrawVLine(i,0,168);
	}
	
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE);
  
	for (i=0; i<160; i++) {
		int len = (i<80) ? i : 160-i;
		GUI_DrawHLine(i,20,len+20);
	}

	GUI_Delay(1000);
	GUI_Clear();

	WindowMain();

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


static void _UpdateCmdWin(void) {
  #if GUI_WINSUPPORT && GUIDEMO_LARGE
    WM_InvalidateWindow(_ahInfoWin[1]);
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


void WindowMain( void )
{
static BUTTON_Handle   _ahButton[2];
static FRAMEWIN_Handle _ahFrameWin[3];
static FRAMEWIN_Handle _ahInfoWin[2];
static int _ButtonSizeX, _ButtonSizeY;

    /* Calculate position and size of FrameWin[1] */
    _ButtonSizeX = 30;
    _ButtonSizeY = 20;

    /* Create the control window incl. buttons */
	///create frame
    _ahFrameWin[1] = FRAMEWIN_Create("Control", &_cbCmdWin, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0, LCD_GetXSize(), LCD_GetYSize());
	///get frame handle
    _ahInfoWin[1] = WM_GetClientWindow(_ahFrameWin[1]);
	///create button with frame handle
    _ahButton[0] = BUTTON_CreateAsChild(4, 20, _ButtonSizeX, _ButtonSizeY, _ahInfoWin[1], 'H', WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
    _ahButton[1] = BUTTON_CreateAsChild(40, 20, _ButtonSizeX, _ButtonSizeY, _ahInfoWin[1], 'N', WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
    BUTTON_SetText(_ahButton[0], "Halt");
    BUTTON_SetText(_ahButton[1], "Next");
	
	GUI_Delay(1000);
    _UpdateCmdWin();
	
    WM_ExecIdle();
}




#endif	/* #if defined(TEST_PROJ) */
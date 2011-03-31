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
  #include "LISTBOX.h"
  #include "FRAMEWIN.h"
  #include "BUTTON.h"
  #include "TEXT.h"
#endif


#ifndef NULL
	#define NULL (0)
#endif



/*
	type define for fframe header and screen
*/
typedef void (*PFNFRAMEPAGEMAIN)(int iOption);
typedef void (*PFNFRAMEPAGEMAINCB)(WM_MESSAGE* pMsg);


typedef struct
{
    PFNFRAMEPAGEMAIN		pfnFramePageMain;     	// 
    PFNFRAMEPAGEMAINCB		pfnFramePageMainCb;    	//
	WM_CALLBACK* 			pfnOldCB;
	int						iWaits;
	int						iNextReady;				///1 for ready
	int						iClearFirst;			///1 for clear before draw
///	PFNBTNHANDLE		pfnBtnHandle;		// handle button event for this frame.
///	WORD				wDrawOption;
///    PFNDRAWCALLBACK   	pfnUpdateCallback;  // 
///    void*				pfnFrameAdvData;    // 
} FRAMEPAGE_HEADER;


typedef struct
{
	FRAMEPAGE_HEADER*		pPrivFramePage;
	FRAMEPAGE_HEADER*		pNextFramePage;
	FRAMEPAGE_HEADER*		pNowFramePage;
///	BOOL 					IsFrameCreate;
} SCREEN_STATUS;




void FrameCenter( void );
void StartWindow( int iOption );
void SecondWindow( int iOption );
void BootWindow( int iOption );
void SimpleWindow( int iOption );
void ListboxWindow( int iOption );

FRAMEPAGE_HEADER headPoweroffWindow;
FRAMEPAGE_HEADER headBootWindow;
FRAMEPAGE_HEADER headDataModeWindow;
FRAMEPAGE_HEADER headPopupWindow;


static SCREEN_STATUS ScrStat;
static SCREEN_STATUS *pScrStat = &ScrStat;

#define		pCurrFramePage				ScrStat.pNowFramePage
#define		pAfterFramePage				ScrStat.pNextFramePage
#define		pBeforeFramePage			ScrStat.pPrivFramePage

#define		pCurrFramePageMain			ScrStat.pNowFramePage->pfnFramePageMain
#define		pCurrFramePageMainCb		ScrStat.pNowFramePage->pfnFramePageMainCb
#define		pCurrFramePageOldCb			ScrStat.pNowFramePage->pfnOldCB
#define		pCurrFramePageWait			ScrStat.pNowFramePage->iWaits
#define		pCurrFramePageNextReady		ScrStat.pNowFramePage->iNextReady
#define		pCurrFramePageClearFirst	ScrStat.pNowFramePage->iClearFirst







static TEXT_Handle		ghTEXT = 0;
static WM_CALLBACK* pfnListCB = NULL;





void spSetDefaultEffect ( void )
{
	
	///WIDGET_SetDefaultEffect(&WIDGET_Effect_3D);
	///WIDGET_SetDefaultEffect(&WIDGET_Effect_Simple);
	WIDGET_SetDefaultEffect(&WIDGET_Effect_None);
	
	FRAMEWIN_SetDefaultBorderSize( 2 );

	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE); 

}


void spClearScreen( void )
{
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_GRAY);
	GUI_Clear();
	///GUI_Delay(100);
}



void spBlankScreen( void )
{
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE); 
	GUI_Clear();
}

void spBlankRect( int x, int y, int xsize, int ysize )
{
	GUI_RECT 	rtTemp;
	GUI_COLOR	colorTemp;
	
	rtTemp.x0 = x;
	rtTemp.y0 = y;
	rtTemp.x1 = x+xsize;
	rtTemp.y1 = y+ysize;

#if 1
	colorTemp = GUI_GetColor();
	GUI_SetColor(GUI_WHITE);
	GUI_FillRect( rtTemp.x0, rtTemp.y0, rtTemp.x1, rtTemp.y1 );
	GUI_SetColor(colorTemp);
#else
	GUI_ClearRect( rtTemp.x0, rtTemp.y0, rtTemp.x1, rtTemp.y1 );
#endif
}


void spcbRoundWinExt( WM_MESSAGE* pMsg, GUI_COLOR color, int radius, int pensize, int iFill )
{
	GUI_RECT rtTemp;
	GUI_COLOR clTemp;
	
	if( !pMsg || (WM_PAINT != pMsg->MsgId && WM_DELETE != pMsg->MsgId) )
		return;
	
	WM_GetClientRectEx( pMsg->hWin, &rtTemp );
	
	if( WM_DELETE == pMsg->MsgId )
	{
		GUI_ClearRect( rtTemp.x0, rtTemp.y0, rtTemp.x1, rtTemp.y1);
		return;
	}
	
	if( iFill > 0 )
	{
		///GUI_ClearRect( rtTemp.x0-radius, rtTemp.y0-radius, rtTemp.x1+radius, rtTemp.y1+radius);
		spBlankRect( rtTemp.x0-radius, rtTemp.y0-radius, (rtTemp.x1-rtTemp.x0)+(2*radius), (rtTemp.y1-rtTemp.y0)+(2*radius) );
	}
	
	clTemp = GUI_GetColor();
	GUI_SetColor( color );
	///GUI_SetPenSize( 1 );
	if( pensize > 1 )
	{
		GUI_FillRect( rtTemp.x0+radius, rtTemp.y0, rtTemp.x1-radius, rtTemp.y0+(pensize-1));
		GUI_FillRect( rtTemp.x0+radius, rtTemp.y1-(pensize-1), rtTemp.x1-radius, rtTemp.y1);
		GUI_FillRect( rtTemp.x0, rtTemp.y0+radius, rtTemp.x0+pensize, rtTemp.y1-radius);
		GUI_FillRect( rtTemp.x1-pensize, rtTemp.y0+radius, rtTemp.x1, rtTemp.y1-radius);
	}
	else
	{
		GUI_DrawHLine( rtTemp.y0, rtTemp.x0+radius, rtTemp.x1-radius );
		GUI_DrawHLine( rtTemp.y1, rtTemp.x0+radius, rtTemp.x1-radius ); 
		GUI_DrawVLine( rtTemp.x0, rtTemp.y0+radius, rtTemp.y1-radius ); 
		GUI_DrawVLine( rtTemp.x1, rtTemp.y0+radius, rtTemp.y1-radius );
	}
	if( radius > 0 )
	{
		///GUI_SetPenSize( 1 );
		GUI_DrawArc( rtTemp.x0+radius, rtTemp.y0+radius, radius, radius, 90, 180 );
		GUI_DrawArc( rtTemp.x0+radius, rtTemp.y1-radius, radius, radius, 180, 270 );
		GUI_DrawArc( rtTemp.x1-radius, rtTemp.y1-radius, radius, radius, -90, 0 );
		GUI_DrawArc( rtTemp.x1-radius, rtTemp.y0+radius, radius, radius, 0, 90 );
	}
	GUI_SetColor( clTemp );
}





void spFramePageWait( void )
{
	///we should replace this with event wait instead of polling.
	if( pCurrFramePageWait == 0 )
	{
		while( 0==pCurrFramePageNextReady )
			GUI_Delay(10);
	}
	else
	{
		int iCnt = 0;
		iCnt = pCurrFramePageWait;
		while( 0==pCurrFramePageNextReady && iCnt > 0)
		{
			GUI_Delay(10);
			iCnt = iCnt - 10;
		}
	}
	pCurrFramePageNextReady = 0;
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
			///WM_DefaultProc(pMsg);
			/* Update info in command window */
			GUI_SetBkColor(GUI_GRAY);
			GUI_Clear();
			GUI_DispStringAt("Demo ", 0, 0);
			GUI_DispDecMin(_iTest + 1);
			GUI_DispString(".");
			GUI_DispDecMin(_iTestMinor);
			GUI_DispString("/");
//    		GUI_DispDecMin(countof(_apfTest));
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
#endif


#if (GUI_WINSUPPORT)
static void _cbFrameWin(WM_MESSAGE* pMsg)
{
static int _iTest, _iTestMinor;
	int Key = 0;

	printf("_cbFrameWin() ==> %d \n", pMsg->MsgId);
	switch (pMsg->MsgId) {
		case WM_CREATE:
			printf("_cbFrameWin() WM_CREATE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;
		case WM_MOVE:
			printf("_cbFrameWin() WM_MOVE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;
		case WM_SIZE:
			printf("_cbFrameWin() WM_SIZE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;

		case WM_TOUCH:
			WM_DefaultProc(pMsg);
			break;
		case WM_NOTIFY_PARENT:
			WM_DefaultProc(pMsg);
			break;
		case WM_KEY:
			printf("_cbFrameWin() WM_KEY!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
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
				printf("_cbFrameWin() WM_KEY=%d[%c]\n", Key, Key);
				
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
			WM_DefaultProc(pMsg);
			break;
	#if 0		
		case WM_PAINT:
			printf("_cbFrameWin() WM_PAINT!!!!!!!!!!!!!!!!!\n");
			///WM_DefaultProc(pMsg);
			/* Update info in command window */
			///GUI_SetBkColor(GUI_GRAY);
			///GUI_Clear();
			GUI_DispStringAt("Frame ", 80, 0);
			WM_DefaultProc(pMsg);
			break;
	#endif		
		default:
			WM_DefaultProc(pMsg);
	}
}
#endif


#if (GUI_WINSUPPORT)
static void cbPoweroffWindow(WM_MESSAGE* pMsg)
{
///static int _iTest, _iTestMinor;
	

	printf("cbPoweroffWindow() ==> %d \n", pMsg->MsgId);

	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;
			
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( 'o' == Key )
		{	/// hold up key
			printf( "UP key hold!!!!!!!\n" );
			///set next framepage
			pAfterFramePage = &headBootWindow;
			///ready for next framepage
			pCurrFramePageNextReady = 1;
		}		
	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		GUI_DispStringAt("PowerOff...", 80, 30);
	}
	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );
	return;

}
#endif



void PoweroffWindow( int iOption )
{
	WM_HWIN hWin1 = 0;
	///WM_CALLBACK* pfnOldCB = NULL;
	
	
	///give me a blank page
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();
	
	///create a window
	///hWin1 = WM_CreateWindow( 0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	hWin1 = WM_CreateWindow( 0, 0, 10, 10, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	
	///set callback
	///pfnOldCB = WM_SetCallback( hWin1, &cbPoweroffWindow );
	pCurrFramePageOldCb = WM_SetCallback( hWin1, pCurrFramePageMainCb );
	///ScrStat.pNowFramePage->pfnOldCB = WM_SetCallback( hWin1, pCurrFramePageMainCb );
	
	WM_BringToTop( hWin1 );
	WM_SetFocus( hWin1 );
	
	GUI_DispStringAt("PowerOff...", 45, 80);
	
    WM_ExecIdle();

	spFramePageWait();
	
	WM_DeleteWindow( hWin1 );
}


extern GUI_CONST_STORAGE GUI_BITMAP bmStartup_Screen_1;
void BootWindow( int iOption )
{
	GUI_DispStringAt("BootWindow\n", 45, 80 );
	GUI_Delay(100);
	GUI_DrawBitmap(&bmStartup_Screen_1, 0, 0);
	spFramePageWait();
	///set next framepage
	pAfterFramePage = &headDataModeWindow;
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
	
	///WM_SetCallback(_ahFrameWin[1], _cbFrameWin);
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
	
	GUI_Delay(300);
    ///_UpdateCmdWin();
	///_UpdateCmdWin(_ahInfoWin[1]);
	_UpdateCmdWin(_ahFrameWin[1]);		///kill frame will kill all window in
	GUI_SetBkColor(GUI_GRAY);
	GUI_Clear();	
    
	///WM_ExecIdle();
}

#define		EDGEOFFSET	20
void SecondWindow( int iOption )
{
static FRAMEWIN_Handle _ahFrameWin[3];
static FRAMEWIN_Handle _ahInfoWin[2];
    int xSize, ySize, xPos, yPos;


	spSetDefaultEffect();

    /* Calculate position and size of FrameWin[0] */
    ///xSize = LCD_GetXSize() / 2;
	xSize = LCD_GetXSize()-(EDGEOFFSET*2);
    ///ySize = 65;
	ySize = LCD_GetYSize()-(EDGEOFFSET*2);
    ///xPos  = LCD_GetXSize() - xSize;
	xPos = EDGEOFFSET;
    yPos  = EDGEOFFSET;
    /* Create info window and run the individual demos */
	///create frame
    ///_ahFrameWin[0] = FRAMEWIN_Create("emWin Demo", NULL, WM_CF_STAYONTOP, xPos, yPos, xSize, ySize);
	_ahFrameWin[0] = FRAMEWIN_CreateEx(xPos, yPos, xSize, ySize, WM_HWIN_NULL, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0, "Win Demo", NULL);
//    _ahInfoWin[0] = WM_GetFirstChild(_ahFrameWin[0]);
	///create window within frame
    _ahInfoWin[0] = WM_CreateWindowAsChild(0, 0, 30, 50, WM_GetFirstChild(_ahFrameWin[0]), WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0);
	///_ahInfoWin[0] = WM_CreateWindowAsChild(xPos, yPos-20, xSize, ySize-20, WM_GetFirstChild(_ahFrameWin[0]), WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0);

	///WM_SetCallback(_ahFrameWin[0], _cbFrameWin);
	
    WM_ExecIdle();

	GUI_Delay(1000);
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


#if (GUI_WINSUPPORT)
static void cbSimpleWindow(WM_MESSAGE* pMsg)
{
static int _iTest, _iTestMinor;
	int Key = 0;

	printf("cbSimpleWindow() ==> %d \n", pMsg->MsgId);
	switch (pMsg->MsgId) {
		case WM_CREATE:
			printf("cbSimpleWindow() WM_CREATE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;
		case WM_MOVE:
			printf("cbSimpleWindow() WM_MOVE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;
		case WM_SIZE:
			printf("cbSimpleWindow() WM_SIZE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;

		case WM_TOUCH:
			WM_DefaultProc(pMsg);
			break;
		case WM_NOTIFY_PARENT:
			WM_DefaultProc(pMsg);
			break;
		case WM_KEY:
			printf("cbSimpleWindow() WM_KEY!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
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
				printf("_cbFrameWin() WM_KEY=%d[%c]\n", Key, Key);
				
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
			WM_DefaultProc(pMsg);
			break;
	#if 1	
		case WM_DELETE:
			printf("cbSimpleWindow() WM_DELETE!!!!!!!!!!!!!!!!!\n");
			spcbRoundWinExt( pMsg, GUI_BLUE, 0, 1, 1 );
			WM_DefaultProc(pMsg);
			break;
		case WM_PAINT:
			printf("cbSimpleWindow() WM_PAINT!!!!!!!!!!!!!!!!!\n");
			///WM_DefaultProc(pMsg);
			/* Update info in command window */
			spcbRoundWinExt( pMsg, GUI_BLUE, 0, 1, 1 );
			WM_DefaultProc(pMsg);
			break;
	#endif		
		default:
			WM_DefaultProc(pMsg);
	}
}



static void cbSimpleRoundWindow(WM_MESSAGE* pMsg)
{
static int _iTest, _iTestMinor;
	int Key = 0;

	printf("cbSimpleWindow() ==> %d \n", pMsg->MsgId);
	switch (pMsg->MsgId) {
		case WM_CREATE:
			printf("cbSimpleWindow() WM_CREATE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;
		case WM_MOVE:
			printf("cbSimpleWindow() WM_MOVE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;
		case WM_SIZE:
			printf("cbSimpleWindow() WM_SIZE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;

		case WM_TOUCH:
			WM_DefaultProc(pMsg);
			break;
		case WM_NOTIFY_PARENT:
			WM_DefaultProc(pMsg);
			break;
		case WM_KEY:
			printf("cbSimpleWindow() WM_KEY!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
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
				printf("_cbFrameWin() WM_KEY=%d[%c]\n", Key, Key);
				
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
			WM_DefaultProc(pMsg);
			break;
	#if 1	
		case WM_DELETE:
			printf("cbSimpleWindow() WM_DELETE!!!!!!!!!!!!!!!!!\n");
			spcbRoundWinExt( pMsg, GUI_BLUE, 13, 2, 1 );
			WM_DefaultProc(pMsg);
			break;
		case WM_PAINT:
			printf("cbSimpleWindow() WM_PAINT!!!!!!!!!!!!!!!!!\n");
			///WM_DefaultProc(pMsg);
			/* Update info in command window */
			spcbRoundWinExt( pMsg, GUI_BLUE, 13, 2, 1 );
			WM_DefaultProc(pMsg);
			break;
	#endif		
		default:
			WM_DefaultProc(pMsg);
	}
}
#endif


static const GUI_ConstString _ListBox[] = {
  "English", "Deutsch", "Français", "Japanese", "Italiano", "English", "Deutsch", "Français", "Japanese", "Italiano", NULL
};


static void cbListBoxWindow(WM_MESSAGE* pMsg)
{
static int _iTest, _iTestMinor;
	int Key = 0;

	printf("cbListBoxWindow() ==> %d \n", pMsg->MsgId);
	
	if( pfnListCB )
	{
		if( WM_KEY == pMsg->MsgId )
		{
			if( ((WM_KEY_INFO*)(pMsg->Data.p))->Key > 0 )
			{
				Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
				///hack the key value
				if( Key == 'i' )
					((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
				else
				if( Key == 'k' )
					((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;

			}
			pfnListCB( pMsg );
			return;
		}
		else
		{
			pfnListCB( pMsg );
			return;
		}
	}	
	
	switch (pMsg->MsgId) {
		case WM_CREATE:
			printf("cbListBoxWindow() WM_CREATE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;
		case WM_MOVE:
			printf("cbListBoxWindow() WM_MOVE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;
		case WM_SIZE:
			printf("cbListBoxWindow() WM_SIZE!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			break;

		case WM_TOUCH:
			WM_DefaultProc(pMsg);
			break;
		case WM_NOTIFY_PARENT:
			WM_DefaultProc(pMsg);
			break;
		case WM_KEY:
			printf("cbListBoxWindow() WM_KEY!!!!!!!!!!!!!!!!!\n");
			WM_DefaultProc(pMsg);
			if (((WM_KEY_INFO*)(pMsg->Data.p))->Key > 0) {
				Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
				printf("cbListBoxWindow() WM_KEY=%d[%c]\n", Key, Key);
				
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

				if( pMsg->hWin != 0 )
				{
					WM_MESSAGE Msg = {0};
					///WM_SendMessage(hDialog, &Msg);
					if( 'u' == Key ) 
						;///TEXT_SetText( ghTEXT, "-exit key-");
					else	
					if( 'y' == Key ) 
						;///TEXT_SetText( ghTEXT, "-exit key-hold");
					else	
					if( 'i' == Key ) 
						LISTBOX_IncSel( pMsg->hWin );///TEXT_SetText( ghTEXT, "-up key-");
					else	
					if( 'o' == Key ) 
						;///TEXT_SetText( ghTEXT, "-up key-hold");
					else	
					if( 'j' == Key ) 
						;///TEXT_SetText( ghTEXT, "-enter key-");
					else	
					if( 'h' == Key ) 
						;///TEXT_SetText( ghTEXT, "-enter key-hold");
					else	
					if( 'k' == Key ) 
						LISTBOX_DecSel( pMsg->hWin );///TEXT_SetText( ghTEXT, "-down key-");
					else	
					if( 'l' == Key ) 
						;///TEXT_SetText( ghTEXT, "-down key-hold");
				}
				
			}
			
			break;
		case WM_GET_ID:
			WM_DefaultProc(pMsg);
			break;
	#if 1	
		case WM_DELETE:
			printf("cbListBoxWindow() WM_DELETE!!!!!!!!!!!!!!!!!\n");
			///cbRoundWinExt( pMsg, GUI_BLUE, 0, 1 );
			WM_DefaultProc(pMsg);
			break;
		case WM_PAINT:
			printf("cbListBoxWindow() WM_PAINT!!!!!!!!!!!!!!!!!\n");
			///WM_DefaultProc(pMsg);
			/* Update info in command window */
			WM_DefaultProc(pMsg);
			break;
	#endif		
		default:
			WM_DefaultProc(pMsg);
	}
}




void SimpleWindow( int iOption )
{

	WM_HWIN hWin1 = 0;
	WM_HWIN hWin2 = 0;
	WM_HWIN hWin3 = 0;
	WM_HWIN hWin4 = 0;
	WM_HWIN hWin5 = 0;
	LISTBOX_Handle	hList;
	WM_CALLBACK* pOldCB = NULL;
	///WM_CALLBACK* pfnListCB = NULL;
	
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE); 
	GUI_Clear();

	///create windows
	hWin1 = WM_CreateWindow( 0+EDGEOFFSET, 0+EDGEOFFSET, LCD_GetXSize()-(EDGEOFFSET*2), LCD_GetYSize()-(EDGEOFFSET*2), WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pOldCB = WM_SetCallback( hWin1, &cbSimpleRoundWindow );

	///create windows
	hWin2 = WM_CreateWindow( 0, 0, LCD_GetXSize(), LCD_GetYSize()/3, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pOldCB = WM_SetCallback( hWin2, &cbSimpleWindow );
	///create windows
	hWin3 = WM_CreateWindow( 0, 0+LCD_GetYSize()/3, LCD_GetXSize(), LCD_GetYSize()/3, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pOldCB = WM_SetCallback( hWin3, &cbSimpleWindow );
	///create windows
	hWin4 = WM_CreateWindow( 0, 0+2*LCD_GetYSize()/3, LCD_GetXSize()/2, LCD_GetYSize()/3, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pOldCB = WM_SetCallback( hWin4, &cbSimpleWindow );
	///create windows
	hWin5 = WM_CreateWindow( LCD_GetXSize()/2, 0+2*LCD_GetYSize()/3, LCD_GetXSize()/2, LCD_GetYSize()/3, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pOldCB = WM_SetCallback( hWin5, &cbSimpleWindow );

	///create list box
	hList = LISTBOX_CreateEx( 0+EDGEOFFSET, 0+EDGEOFFSET+13, LCD_GetXSize()-(EDGEOFFSET*2), LCD_GetYSize()-(EDGEOFFSET*2)-(13*2), WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, _ListBox);
	///add callback
	///pOldCB = WM_SetCallback( hList, &cbListBoxWindow );
	pfnListCB = WM_SetCallback( hList, &cbListBoxWindow );
	
	
	WM_BringToTop( hWin1 );
	WM_SetFocus( hWin1 );
	WM_BringToTop( hList );
	WM_SetFocus( hList );
	
	WM_ExecIdle();
	GUI_Delay(1000);
	
	WM_DeleteWindow( hWin1 );
	WM_DeleteWindow( hWin2 );
	WM_DeleteWindow( hWin3 );
	WM_DeleteWindow( hWin4 );
	WM_DeleteWindow( hWin5 );
	WM_DeleteWindow( hList );
	pfnListCB = NULL;
	
}



#if (GUI_WINSUPPORT)
static void cbDataModeWindow(WM_MESSAGE* pMsg)
{

	printf("cbDataModeWindow() ==> %d \n", pMsg->MsgId);

	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;
			
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( 'o' == Key )
		{	/// hold up key
			printf( "UP key hold!!!!!!!\n" );
			///set next framepage
			pAfterFramePage = &headPopupWindow;
			///ready for next framepage
			pCurrFramePageNextReady = 1;
		}
		else		
		if( 'l' == Key )
		{	/// hold down key
			printf( "DOWN key hold!!!!!!!\n" );
			///set next framepage
			pAfterFramePage = &headPopupWindow;
			///ready for next framepage
			pCurrFramePageNextReady = 1;
		}

	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		spcbRoundWinExt( pMsg, GUI_BLUE, 0, 1, 1 );
	}
	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );
	return;

}
#endif

void DataModeWindow( int iOption )
{
	///WM_HWIN hWin1 = 0;
	WM_HWIN hWin2 = 0;
	WM_HWIN hWin3 = 0;
	WM_HWIN hWin4 = 0;
	WM_HWIN hWin5 = 0;
	///LISTBOX_Handle	hList;
	///WM_CALLBACK* pOldCB = NULL;


	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	///create windows
	hWin2 = WM_CreateWindow( 0, 0, LCD_GetXSize(), LCD_GetYSize()/3, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hWin2, pCurrFramePageMainCb );
	///create windows
	hWin3 = WM_CreateWindow( 0, 0+LCD_GetYSize()/3, LCD_GetXSize(), LCD_GetYSize()/3, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hWin3, pCurrFramePageMainCb );
	///create windows
	hWin4 = WM_CreateWindow( 0, 0+2*LCD_GetYSize()/3, LCD_GetXSize()/2, LCD_GetYSize()/3, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hWin4, pCurrFramePageMainCb );
	///create windows
	hWin5 = WM_CreateWindow( LCD_GetXSize()/2, 0+2*LCD_GetYSize()/3, LCD_GetXSize()/2, LCD_GetYSize()/3, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hWin5, pCurrFramePageMainCb );

	
	WM_BringToTop( hWin2 );
	WM_SetFocus( hWin2 );
	
	WM_ExecIdle();
	spFramePageWait();
	
	WM_DeleteWindow( hWin2 );
	WM_DeleteWindow( hWin3 );
	WM_DeleteWindow( hWin4 );
	WM_DeleteWindow( hWin5 );
	
}



#if (GUI_WINSUPPORT)
static void cbPopupWindow(WM_MESSAGE* pMsg)
{
	printf("cbPopupWindow() ==> %d \n", pMsg->MsgId);

	if( pMsg->MsgId == WM_PAINT )
	{
		/* Update info in command window */
		spcbRoundWinExt( pMsg, GUI_BLACK, 13, 2, 1 );
		WM_DefaultProc(pMsg);
	}
	return;

}

static void cbPopupWindowList(WM_MESSAGE* pMsg)
{
	printf("cbPopupWindowList() ==> %d \n", pMsg->MsgId);
	
	if( pCurrFramePageOldCb )
	{
		if( WM_KEY == pMsg->MsgId )
		{
			int Key = 0;
			if( ((WM_KEY_INFO*)(pMsg->Data.p))->Key > 0 )
			{
				Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
				///hack the key value
				if( Key == 'i' )	///up
					((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
				else
				if( Key == 'k' )	///down
					((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
				else
				if( Key == 'j' )	///enter
				{
					///pBeforeFramePage = pCurrFramePage;
					///pCurrFramePage = pAfterFramePage;
					pAfterFramePage = pBeforeFramePage;
					pCurrFramePageNextReady = 1;
				}
				else
				if( Key == 'u' )	///back
				{
					pAfterFramePage = pBeforeFramePage;
					pCurrFramePageNextReady = 1;
				}
					
			}
			if( pCurrFramePageOldCb )
				pCurrFramePageOldCb( pMsg );
			return;
		}
		else
		{
			if( pCurrFramePageOldCb )
				pCurrFramePageOldCb( pMsg );
			return;
		}
	}	
}
#endif


static const GUI_ConstString _PopupListBox[] = {
  "Activity", "Navigation", "Settings", "History", NULL
};



void PopupWindow( int iOption )
{
	WM_HWIN hWin1 = 0;
	LISTBOX_Handle	hList;
	WM_CALLBACK* pOldCB = NULL;

	
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	spSetDefaultEffect();
	
	///spBlankRect( 0+EDGEOFFSET, 0+EDGEOFFSET, LCD_GetXSize()-(EDGEOFFSET*2), LCD_GetYSize()-(EDGEOFFSET*2) );
	
	///create windows for popup outline
	hWin1 = WM_CreateWindow( 0+EDGEOFFSET, 0+EDGEOFFSET, LCD_GetXSize()-(EDGEOFFSET*2), LCD_GetYSize()-(EDGEOFFSET*2), WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pOldCB = WM_SetCallback( hWin1, &cbPopupWindow );


	///create list box
	hList = LISTBOX_CreateEx( 0+EDGEOFFSET+2, 0+EDGEOFFSET+13, LCD_GetXSize()-(EDGEOFFSET*2)-4, LCD_GetYSize()-(EDGEOFFSET*2)-(13*2), WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, _PopupListBox);
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hList, pCurrFramePageMainCb );
	
	
	WM_BringToTop( hWin1 );
	WM_SetFocus( hWin1 );
	WM_BringToTop( hList );
	WM_SetFocus( hList );
	
	WM_ExecIdle();
	GUI_Delay(1000);
	
	WM_DeleteWindow( hWin1 );
	WM_DeleteWindow( hList );
	///pfnListCB = NULL;
	
}





static const GUI_ConstString _apListBox[] = {
  "English", "Deutsch", "Français", "Japanese", "Italiano", NULL
};


void ListboxWindow( int iOption )
{
	FRAMEWIN_Handle	hFrame;
	LISTBOX_Handle	hList;
	int				iTmp = 0;
	
	spSetDefaultEffect();
	
	hFrame = FRAMEWIN_CreateEx(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_HWIN_NULL, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0, "List Box", &_cbCmdWin);
	iTmp = FRAMEWIN_GetTitleHeight( hFrame );
	
	hList = LISTBOX_CreateEx(0, iTmp, LCD_GetXSize(), LCD_GetYSize()-iTmp, (WM_HWIN)hFrame, WM_CF_SHOW, 0, 0, _apListBox);
	
	
    WM_ExecIdle();

	GUI_Delay(300);
    ///_UpdateCmdWin();
	///_UpdateCmdWin(WM_GetFirstChild(_ahFrameWin[0]));
	_UpdateCmdWin(hFrame);
	GUI_SetBkColor(GUI_GRAY);
	GUI_Clear();
}










FRAMEPAGE_HEADER headPoweroffWindow = {
	PoweroffWindow,
	cbPoweroffWindow,
	NULL,
	0,
	0,
	1,
};

FRAMEPAGE_HEADER headBootWindow = {
	BootWindow,
	NULL,
	NULL,
	100,
	0,
	1,
};

FRAMEPAGE_HEADER headDataModeWindow = {
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
};

FRAMEPAGE_HEADER headPopupWindow = {
	PopupWindow,
	cbPopupWindowList,
	NULL,
	0,
	0,
	0,
};




void FrameCenter( void )
{
	int iLoop = 5;
    GUI_CONTEXT ContextOld;

	///set the first FramePage header
	pCurrFramePage = &headPoweroffWindow;

	spClearScreen();

#if 1
	while( iLoop > 0 )
	{
		GUI_SaveContext(&ContextOld);
		pCurrFramePage->pfnFramePageMain( iLoop );
		GUI_RestoreContext(&ContextOld);
		///spClearScreen();
		iLoop--;

		pBeforeFramePage = pCurrFramePage;
		pCurrFramePage = pAfterFramePage;
		///pAfterFramePage =
	}
#else	
	while( iLoop > 0 )
	{

	#if 1
		GUI_SaveContext(&ContextOld);
		BootWindow(0);
		spClearScreen();
		GUI_RestoreContext(&ContextOld);
	#endif
	#if 0
		GUI_SaveContext(&ContextOld);
		StartWindow( iLoop );
		spClearScreen();
		GUI_RestoreContext(&ContextOld);
	#endif
	#if 0
		GUI_SaveContext(&ContextOld);
		SecondWindow( iLoop );
		spClearScreen();
		GUI_RestoreContext(&ContextOld);
	#endif
	#if 0
		GUI_SaveContext(&ContextOld);
		FontWindow( iLoop );
		spClearScreen();
		GUI_RestoreContext(&ContextOld);
	#endif
	#if 0
		GUI_SaveContext(&ContextOld);
		ListboxWindow( iLoop );
		spClearScreen();
		GUI_RestoreContext(&ContextOld);
	#endif
	#if 1
		GUI_SaveContext(&ContextOld);
		SimpleWindow( iLoop );
		spClearScreen();
		GUI_RestoreContext(&ContextOld);
	#endif
	
		spClearScreen();
		
		iLoop--;
	}
#endif	
}




#endif	/* #if defined(TEST_PROJ) */
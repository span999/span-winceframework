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

#include "RenoGUI.h"
#include "RenoUItoolhelp.h"
#include "RenoUIcommon.h"
#include "RenoUIframe.h"


#include "RenoUICcomm.h"
#include "RenoUICdatamode.h"
#include "RenoUICwatchmode.h"




void FrameCenter( void );
void StartWindow( int iOption );
void SecondWindow( int iOption );
void BootWindow( int iOption );
void SimpleWindow( int iOption );
void ListboxWindow( int iOption );





///static TEXT_Handle		ghTEXT = 0;
///static WM_CALLBACK* pfnListCB = NULL;



/*
	simulate the data from user...
*/
void TaskUserDataHook( void * pvParameters )
{
	int iKey = 0;
	WM_MESSAGE  UserDataMsg;
	
	SPPRINTF("  TaskUserDataHook:\n");
	
	///iKey = GUI_WaitKey();
	GUI_Delay(500);
	
	
	if( pCurrFramePage == &headSGSGSWindow )
	{	///in Settings / GPS Settings / GPS Satellites
		///WM_SendMessage(WM_HWIN hWin, WM_MESSAGE* pMsg)
		int iTmp = 0;
		
		_DE_GPS_SATELLITE sss = { 1, 1, 20 };
		DE_GPS_SATELLITES_INFO	gpsSatellites = {
						12,
						{
							{ 1, 1, 20 },
							{ 2, 1, 22 },
							{ 3, 1, 24 },
							{ 4, 1, 26 },
							{ 5, 1, 28 },
							{ 6, 1, 30 },
							{ 7, 1, 32 },
							{ 8, 1, 34 },
							{ 9, 1, 36 },
							{ 10, 1, 38 },
							{ 11, 1, 40 },
							{ 12, 1, 42 }
						}
		};
		
		///create random data
		for( iTmp=0; iTmp<12; iTmp++ )
			gpsSatellites.satellites[iTmp].iSignal = spGetRandNum( 0, 40 );
		
		///fill message struct
		UserDataMsg.MsgId = WM_GPS_SATELLITE;
		UserDataMsg.hWin = pCurrFramePageHandle;
		UserDataMsg.hWinSrc = NULL;
		UserDataMsg.Data.p = (void*)&gpsSatellites;
		
		///send to current frame page
		if( pCurrFramePageHandle )
			WM_SendMessage(pCurrFramePageHandle, &UserDataMsg);
			
		SPPRINTF("  TaskUserDataHook:  WM_GPS_SATELLITE\n");	
	
	}
	else
	if( 
		(pCurrFramePage == &headDataMode1Window) ||
		(pCurrFramePage == &headDataMode2Window) ||
		(pCurrFramePage == &headDataMode3Window) ||
		(pCurrFramePage == &headDataMode4Window) ||
		(pCurrFramePage == &headDataMode5Window) ||
		(pCurrFramePage == &headDataMode6Window)
	)
	{
		char str1[8];
		char str2[8];
		char str3[8];
		char str4[8];

		DE_ACTIVITY_DATA activitydatas = {
									4,
									{
										{ ACTDATA_ID_DISTANCE, "1.23 " },
										{ ACTDATA_ID_TIME, "12:12" },
										{ ACTDATA_ID_ELEVATION, "234" },
										{ ACTDATA_ID_POWER, "999" },
										{ 0, "0.00" },
										{ 0, "0.00" }
									}
								};
								
		///create random data
		sprintf( str1, "%02d.%02d", spGetRandNum( 1, 99), spGetRandNum( 1, 99) );
		sprintf( str2, "%02d:%02d", spGetRandNum( 1, 24), spGetRandNum( 0, 59) );
		sprintf( str3, "%d", spGetRandNum( 1, 999) );
		sprintf( str4, "%d", spGetRandNum( 1, 999) );
		///activitydatas.activity[0].sDataStr = "3.33 ";
		activitydatas.activity[0].sDataStr = str1;
		activitydatas.activity[1].sDataStr = str2;
		activitydatas.activity[2].sDataStr = str3;
		activitydatas.activity[3].sDataStr = str4;
		
		///fill message struct
		UserDataMsg.MsgId = WM_ACTIVITY_DATA;
		UserDataMsg.hWin = pCurrFramePageHandle;
		UserDataMsg.hWinSrc = NULL;
		UserDataMsg.Data.p = (void*)&activitydatas;
		
		///send to current frame page
		if( pCurrFramePageHandle )
		{
			WM_SendMessage(pCurrFramePageHandle, &UserDataMsg);
			///WM_Paint( pCurrFramePageHandle );
			spUpdateDataModeWindow();
		}

		SPPRINTF("  TaskUserDataHook:  WM_ACTIVITY_DATA\n");
	}
	else
	if( pCurrFramePage == &headWatchWindow )
	{
		static int second = 0;
		static int minute = 0;
		char str1[8];
		char str2[8];
		DE_WATCH_DATA watchdatas = {
									2,
									{
										{ WATCHDATA_ID_SECOND, "00" },
										{ WATCHDATA_ID_TIME, "12:00" },
										{ 0, "0.00" },
										{ 0, "0.00" },
										{ 0, "0.00" }
									}
								};
								
		///create random data
		if( 59 == second )
		{
			second = 0;
			minute++;
		}
		else
			second++;
			
		if( 60 == minute )
			minute = 0;
			
		sprintf( str1, "%02d", second );
		sprintf( str2, "12:%02d", minute );

		watchdatas.watchdata[0].sDataStr = str1;
		watchdatas.watchdata[1].sDataStr = str2;
		
		///fill message struct
		UserDataMsg.MsgId = WM_WATCH_DATA;
		UserDataMsg.hWin = pCurrFramePageHandle;
		UserDataMsg.hWinSrc = NULL;
		UserDataMsg.Data.p = (void*)&watchdatas;
		
		///send to current frame page
		if( pCurrFramePageHandle )
		{
			WM_SendMessage(pCurrFramePageHandle, &UserDataMsg);
			///WM_Paint( pCurrFramePageHandle );
			spUpdateWatchModeWindow();
			///spUpdateDataModeWindow();
		}
		
		SPPRINTF("  TaskUserDataHook:  WM_WATCH_DATA\n");
	}
		
	SPPRINTF("  TaskUserDataHook: keycode=%d\n", iKey);
}



/*
	routin body
*/


static void Logo_TestPattern( void )
{
	int i;

	///clear with red
	GUI_SetBkColor(GUI_RED);
	GUI_Clear();
	GUI_Delay(100);
	
	///clear with blue
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_Delay(100);
		
	///draw H line with white
	GUI_SetColor(GUI_WHITE);

	for (i=0; i<170; i+=10) {
		GUI_DrawHLine(i,0,100);
		GUI_DispStringAt("Line ",0,i);
		GUI_DispDecMin(i);
	}

	GUI_Delay(100);
	
	///draw V line with white
	for (i=60; i<150; i+=10) {
		GUI_DrawVLine(i,0,168);
	}
	
	///set black on white
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE);
  
	for (i=0; i<160; i++) {
		int len = (i<80) ? i : 160-i;
		GUI_DrawHLine(i,20,len+20);
	}
}

/*
  *******************************************************************
  *
  *              main()
  *
  *******************************************************************
*/

void MainTask_RenoUItest(void) {
	int Cnt =0;
	int LCDXSize = LCD_GET_XSIZE();
	int LCDYSize = LCD_GET_YSIZE();


	SPPRINTF("GUI_Init()!!!!!!!!!!!!!!!!!!\n");
	OS_Printf("GUI_Init()!!!!!!!!!!!!!!!!!!\n");
	
	///
	GUI_Init();

#if 0 /* test pattern */
	Logo_TestPattern();
#endif /* test pattern */

	///set black on white
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE);

	GUI_Delay(100);
	GUI_Clear();

	FrameCenter();

}




static FRAMEPAGE_HEADER* SAPAPSDPADSASNSDataSetFrameList[] = {
	&headSAPAPSDPADSASNS1Window,
	&headSAPAPSDPADSASNS2Window,
	&headSAPAPSDPADSASNS3Window,
	&headSAPAPSDPADSASNS4Window,
	&headSAPAPSDPADSASNS5Window,
	&headSAPAPSDPADSASNS6Window
};

static DATASETS_FRAME_LIST		SAPAPSDPADSASNSDataSetsList = {
	6,
	1,
	SAPAPSDPADSASNSDataSetFrameList,
};



void FrameCenter( void )
{
	int iLoop = 55;
    GUI_CONTEXT ContextOld;

	///set the first FramePage header
	pCurrFramePage = &headPoweroffWindow;

	spClearScreen();


	while( iLoop > 0 )
	{
		GUI_SaveContext(&ContextOld);

		pCurrFramePageMain( iLoop );
		GUI_RestoreContext(&ContextOld);

		iLoop--;

		pBeforeFramePage = pCurrFramePage;
		pCurrFramePage = pAfterFramePage;
		///pAfterFramePage =
		
		if( 1 == iLoop)
			pCurrFramePage = &headUnderConstructionWindow;
	}
}





#if 0

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

///#define		EDGEOFFSET	20
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
				if( IsUP_press(Key) )
					((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
				else
				if( IsDOWN_press(Key) )
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

#endif













#endif	/* #if defined(RENOUI_PROJ) */
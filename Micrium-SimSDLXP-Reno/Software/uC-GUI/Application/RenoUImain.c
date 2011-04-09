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
	type define for frame header and screen
*/

/*
	frame page main routin 
*/
typedef void (*PFNFRAMEPAGEMAIN)(int iOption);

/*
	frame page main callback routin
*/
typedef void (*PFNFRAMEPAGEMAINCB)(WM_MESSAGE* pMsg);

/*
	frame page types 
*/
typedef enum
{
	FRAMEPAGE_COMMON = 0,
    FRAMEPAGE_ACTIVITY,
	FRAMEPAGE_DATAMODE,
    FRAMEPAGE_TITLED,
    FRAMEPAGE_LISTMENU,
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	FRAMEPAGE_POPUP_NOTIFY,
	FRAMEPAGE_POPUP_OPTION,
	FRAMEPAGE_POPUP_CONFIRM,
	FRAMEPAGE_POPUP_NUMBERS,
} FRAMEPAGE_TYPE;

/*
	frame page header
*/

struct H_FRAMEPAGE_HEADER
{
	FRAMEPAGE_TYPE				frametype;				///frame page type index
    PFNFRAMEPAGEMAIN			pfnFramePageMain;     	///address of main routine 
    PFNFRAMEPAGEMAINCB			pfnFramePageMainCb;    	///address of user callback
	WM_CALLBACK* 				pfnOldCB;				///old callback address store
	int							iWaits;					///frame page timeout
	int							iNextReady;				///flag for frame page done, 1 for ready
	int							iClearFirst;			///flag for clear before draw, 1 for action
	void*						pFrameData;				///frame data if needed, context info
	struct H_FRAMEPAGE_HEADER*	pTimeoutFrame;			///frame to go if timeout
};

typedef struct H_FRAMEPAGE_HEADER FRAMEPAGE_HEADER;


/*
	screen status record
*/
typedef struct
{
	FRAMEPAGE_HEADER*		pPrivFramePage;
	FRAMEPAGE_HEADER*		pNextFramePage;
	FRAMEPAGE_HEADER*		pNowFramePage;
} SCREEN_STATUS;

/*
	frame page data of menu list
*/
typedef struct
{
	int						iListNum;
	const GUI_ConstString	sListTitle;
	const GUI_ConstString*	sListName;
	FRAMEPAGE_HEADER*		pUplevelFrame;
	FRAMEPAGE_HEADER** 		pListFrame;
	int*					pListParam;
} FP_LISTMENU_HEADER;


/*
	frame page data of popup list
*/
typedef struct
{
	int						iListNum;
	int						iListSel;
	const GUI_ConstString	sListTitle;
	const GUI_ConstString*	sListName;
	FRAMEPAGE_HEADER*		pUplevelFrame;
	FRAMEPAGE_HEADER** 		pListFrame;
	int*					pListParam;
	WM_HWIN					hWinFrame;
} FP_POPUPLIST_HEADER;


/*
	frame page data of activity data set
*/
typedef struct
{
	int						iIndex;
	const GUI_ConstString	sDataName;
	const GUI_ConstString	sDataUnit;
	const GUI_ConstString	sDataValue;
} DATASET_HEADER;

typedef struct
{
	int						iDataSetNum;
	FRAMEPAGE_HEADER*		pUplevelFrame;
	DATASET_HEADER*			pDataSets;
	FRAMEPAGE_HEADER** 		pDataFrame;
	int*					pDataParam;
} FP_DATASETS_HEADER;

typedef struct
{
	int 					iTotal;
	int						iCurrIdx;
	FRAMEPAGE_HEADER**		pDataSetFrame;
} DATASETS_FRAME_LIST;


/*
	elements type define
*/

#define		ELEMENT_FIELD_MASK				0xFFFF0000
#define		ELEMENT_FIELD_OFFSET			16
#define		COMMON_FIELD					0x0000
#define		BOOLEAN_OPTION_FIELD			0x0001

#define		ELEMENT_VALUE_MASK				0x0000FFFF





/*
	toolhelp routin
*/
int IsSamekey( int iSrc, int iTar );
int IsMsgKeyStatus( WM_MESSAGE* pMsg, int iTar );



void FrameCenter( void );
void StartWindow( int iOption );
void SecondWindow( int iOption );
void BootWindow( int iOption );
void SimpleWindow( int iOption );
void ListboxWindow( int iOption );



/*
	frame page declare
*/
FRAMEPAGE_HEADER headPoweroffWindow;
FRAMEPAGE_HEADER headBootWindow;
FRAMEPAGE_HEADER headDataModeWindow;
FRAMEPAGE_HEADER headDataMode1Window;
FRAMEPAGE_HEADER headDataMode2Window;
FRAMEPAGE_HEADER headDataMode3Window;
FRAMEPAGE_HEADER headDataMode4Window;
FRAMEPAGE_HEADER headDataMode5Window;
FRAMEPAGE_HEADER headDataMode6Window;
FRAMEPAGE_HEADER headPopupListWindow;
FRAMEPAGE_HEADER headPopupListWindow_Fitness;
FRAMEPAGE_HEADER headPopupListWindow_DeviceModeFitness;
FRAMEPAGE_HEADER headPopupListWindow_NumberEntry;
FRAMEPAGE_HEADER headNavigationWindow;
FRAMEPAGE_HEADER headSettingsWindow;
FRAMEPAGE_HEADER headSGSWindow;
FRAMEPAGE_HEADER headSGSGSWindow;
FRAMEPAGE_HEADER headSGSNWindow;
FRAMEPAGE_HEADER headSUIWindow;
FRAMEPAGE_HEADER headSUIPIWindow;
FRAMEPAGE_HEADER headSUICIWindow;
FRAMEPAGE_HEADER headSUICIPWindow;
FRAMEPAGE_HEADER headSUICIEWindow;
FRAMEPAGE_HEADER headSUICIAWindow;
FRAMEPAGE_HEADER headSDSWindow;
FRAMEPAGE_HEADER headSDSLWindow;
FRAMEPAGE_HEADER headSDSUMWindow;
FRAMEPAGE_HEADER headSDSUMCWindow;
FRAMEPAGE_HEADER headSDSUMSDWindow;
FRAMEPAGE_HEADER headSDSUMEWindow;
FRAMEPAGE_HEADER headSDSUMHWWindow;
FRAMEPAGE_HEADER headSDSUMTWindow;
FRAMEPAGE_HEADER headSDSUMHRWindow;
FRAMEPAGE_HEADER headSDSUMPWindow;
FRAMEPAGE_HEADER headSAPWindow;
FRAMEPAGE_HEADER headSAPDPWindow;
FRAMEPAGE_HEADER headSAPDPSPWindow;
FRAMEPAGE_HEADER headSAPDPSSWindow;
FRAMEPAGE_HEADER headSAPDPADSWindow;
FRAMEPAGE_HEADER headSAPDPADSASWindow;
FRAMEPAGE_HEADER headSAPDPADSASNS1Window;
FRAMEPAGE_HEADER headSAPDPADSASNS2Window;
FRAMEPAGE_HEADER headSAPDPADSASNS3Window;
FRAMEPAGE_HEADER headSAPDPADSASNS4Window;
FRAMEPAGE_HEADER headSAPDPADSASNS5Window;
FRAMEPAGE_HEADER headSAPDPADSASNS6Window;
FRAMEPAGE_HEADER headSAPDPADSASNSWindow_NumberEntry;
FRAMEPAGE_HEADER headHistoryWindow;
FRAMEPAGE_HEADER headWatchWindow;
FRAMEPAGE_HEADER headUnderConstructionWindow;


/*
	screen status
*/
static SCREEN_STATUS ScrStat;
static SCREEN_STATUS *pScrStat = &ScrStat;




/*
	shortcut declare
*/
#define		pCurrFramePage					ScrStat.pNowFramePage
#define		pAfterFramePage					ScrStat.pNextFramePage
#define		pBeforeFramePage				ScrStat.pPrivFramePage

#define		pCurrFramePageType				ScrStat.pNowFramePage->frametype
#define		pCurrFramePageMain				ScrStat.pNowFramePage->pfnFramePageMain
#define		pCurrFramePageMainCb			ScrStat.pNowFramePage->pfnFramePageMainCb
#define		pCurrFramePageOldCb				ScrStat.pNowFramePage->pfnOldCB
#define		pCurrFramePageWait				ScrStat.pNowFramePage->iWaits
#define		pCurrFramePageNextReady			ScrStat.pNowFramePage->iNextReady
#define		pCurrFramePageClearFirst		ScrStat.pNowFramePage->iClearFirst
#define		pCurrFramePageFrameData			ScrStat.pNowFramePage->pFrameData
#define		pCurrFramePageTimeoutFrame		ScrStat.pNowFramePage->pTimeoutFrame



#define		IsUP_press(x)		IsSamekey( x, 'i' )
#define		IsUP_hold(x)		IsSamekey( x, 'o' )
#define		IsDOWN_press(x)		IsSamekey( x, 'k' )
#define		IsDOWN_hold(x)		IsSamekey( x, 'l' )
#define		IsBACK_press(x)		IsSamekey( x, 'u' )
#define		IsBACK_hold(x)		IsSamekey( x, 'y' )
#define		IsENTER_press(x)	IsSamekey( x, 'j' )
#define		IsENTER_hold(x)		IsSamekey( x, 'h' )

#define		IsMSGUP_press(x)	IsMsgKeyStatus( x, 'i' )
#define		IsMSGUP_hold(x)		IsMsgKeyStatus( x, 'o' )
#define		IsMSGDOWN_press(x)	IsMsgKeyStatus( x, 'k' )
#define		IsMSGDOWN_hold(x)	IsMsgKeyStatus( x, 'l' )
#define		IsMSGBACK_press(x)	IsMsgKeyStatus( x, 'u' )
#define		IsMSGBACK_hold(x)	IsMsgKeyStatus( x, 'y' )
#define		IsMSGENTER_press(x)	IsMsgKeyStatus( x, 'j' )
#define		IsMSGENTER_hold(x)	IsMsgKeyStatus( x, 'h' )


#define IsNotCurrFPListMenuLike		((FRAMEPAGE_LISTMENU != pCurrFramePageType) && (FRAMEPAGE_LISTMENU_BOOLOPTION != pCurrFramePageType))





static TEXT_Handle		ghTEXT = 0;
static WM_CALLBACK* pfnListCB = NULL;




/*
	routin body
*/
void spSetDefaultEffect ( void )
{
	
	///WIDGET_SetDefaultEffect(&WIDGET_Effect_3D);
	///WIDGET_SetDefaultEffect(&WIDGET_Effect_Simple);
	WIDGET_SetDefaultEffect(&WIDGET_Effect_None);
	
	FRAMEWIN_SetDefaultBorderSize( 0 );
	FRAMEWIN_SetDefaultClientColor( GUI_WHITE );
	FRAMEWIN_SetDefaultBarColor( 1, GUI_BLACK );
	FRAMEWIN_SetDefaultTextColor( 1, GUI_WHITE );
	FRAMEWIN_SetDefaultFont( &GUI_Font16B_ASCII );

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

void spGoAfterFramePage( FRAMEPAGE_HEADER *pGoAfterFP )
{
	if( pGoAfterFP )
	{
		///set next framepage
		pAfterFramePage = pGoAfterFP;
		///ready for next framepage
		pCurrFramePageNextReady = 1;
	}
}

int spFrameTimeoutHandling( int iTO )
{	
	int iRet = 0;
	
	if( 
		0 != pCurrFramePageWait &&
		1 == iTO && 
		NULL != pCurrFramePageTimeoutFrame 
	)
	{
		spGoAfterFramePage( pCurrFramePageTimeoutFrame );
		iRet = 1;
	}
	return iRet;
}


int IsSamekey( int iSrc, int iTar )
{
	if( iSrc == 'o' )
		printf("  UP key hold~~\n");
	else
	if( iSrc == 'i' )
		printf("  UP key press~~\n");
	else
	if( iSrc == 'l' )
		printf("  DOWN key hold~~\n");
	else
	if( iSrc == 'k' )
		printf("  DOWN key press~~\n");
	else
	if( iSrc == 'y' )
		printf("  BACK key hold~~\n");
	else
	if( iSrc == 'u' )
		printf("  BACK key press~~\n");
	else
	if( iSrc == 'h' )
		printf("  ENTER key hold~~\n");
	else
	if( iSrc == 'j' )
		printf("  ENTER key press~~\n");
	else
		printf("  unknow key press~~!!\n");
	
	return (iSrc == iTar)?1:0;		
} 

int spGetListParamType( int ListParam )
{
	int iRet = 0;
	
	iRet = (ListParam&ELEMENT_FIELD_MASK)>>ELEMENT_FIELD_OFFSET;
	
	return iRet;
}

int spGetListParamValue( int ListParam )
{
	int iRet = 0;
	
	iRet = (ListParam&ELEMENT_VALUE_MASK);
	
	return iRet;

}

int IsMsgKeyStatus( WM_MESSAGE* pMsg, int iTar )
{
	int iRet = 0;

	if( pMsg )
	{
		int Key = 0;
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;		
		iRet = IsSamekey( Key, iTar );
	}
	
	return iRet;
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
	#if 0
		///GUI_ClearRect( rtTemp.x0-radius, rtTemp.y0-radius, rtTemp.x1+radius, rtTemp.y1+radius);
		spBlankRect( rtTemp.x0-radius, rtTemp.y0-radius, (rtTemp.x1-rtTemp.x0)+(2*radius), (rtTemp.y1-rtTemp.y0)+(2*radius) );
	#else
		spBlankRect( rtTemp.x0+radius, rtTemp.y0, (rtTemp.x1-rtTemp.x0)-(2*radius), (rtTemp.y1-rtTemp.y0) );
		spBlankRect( rtTemp.x0, rtTemp.y0+radius, (rtTemp.x1-rtTemp.x0), (rtTemp.y1-rtTemp.y0)-(2*radius) );
		if( radius > 0 )
		{
			GUI_FillEllipse( rtTemp.x0+radius, rtTemp.y0+radius, radius, radius );
			GUI_FillEllipse( rtTemp.x1-radius, rtTemp.y0+radius, radius, radius );
			GUI_FillEllipse( rtTemp.x1-radius, rtTemp.y1-radius, radius, radius );
			GUI_FillEllipse( rtTemp.x0+radius, rtTemp.y1-radius, radius, radius );
		}
	#endif
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


void spcbPopupNumberWinExt( WM_MESSAGE* pMsg, GUI_COLOR color, int pensize, int iFill )
{
	GUI_RECT rtTemp;
	GUI_COLOR clTemp;
	GUI_POINT pPoint1[] = {
		{ 0, 0},
		{24, 0},
		{12,20},
	};
	GUI_POINT pPoint2[] = {
		{ 0, 0},
		{24, 0},
		{12,-20},
	};
	
	if( !pMsg || (WM_PAINT != pMsg->MsgId && WM_DELETE != pMsg->MsgId && WM_KEY != pMsg->MsgId ) )
		return;
	
	WM_GetClientRectEx( pMsg->hWin, &rtTemp );
	
	if( WM_DELETE == pMsg->MsgId )
	{
		GUI_ClearRect( rtTemp.x0, rtTemp.y0, rtTemp.x1, rtTemp.y1);
		return;
	}
	
	clTemp = GUI_GetColor();
	GUI_SetColor( color );
	///GUI_SetPenSize( 1 );
	if( pensize > 1 )
	{
		GUI_FillRect( (rtTemp.x0+rtTemp.x1)/2, (rtTemp.y0+rtTemp.y1)/2, rtTemp.x1, (rtTemp.y0+rtTemp.y1)/2+(pensize-1));
		///GUI_FillRect( rtTemp.x0+radius, rtTemp.y1-(pensize-1), rtTemp.x1-radius, rtTemp.y1);
		GUI_FillRect( (rtTemp.x0+rtTemp.x1)/2, rtTemp.y0, ((rtTemp.x0+rtTemp.x1)/2)+(pensize-1), rtTemp.y1);
		///GUI_FillRect( rtTemp.x1-pensize, rtTemp.y0+radius, rtTemp.x1, rtTemp.y1-radius);
	}
	else
	{
		GUI_DrawHLine( (rtTemp.y0+rtTemp.y1)/2, (rtTemp.x0+rtTemp.x1)/2, rtTemp.x1 );
		///GUI_DrawHLine( rtTemp.y1, rtTemp.x0+radius, rtTemp.x1-radius ); 
		GUI_DrawVLine( (rtTemp.x0+rtTemp.x1)/2, rtTemp.y0, rtTemp.y1 ); 
		///GUI_DrawVLine( rtTemp.x1, rtTemp.y0+radius, rtTemp.y1-radius );
	}
#if 0
	if( WM_KEY == pMsg->MsgId )
	{
		int Key = 0;
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;

		///hack the key value
		if( IsUP_press(Key) )	///up
		{
			///((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
			GUI_DrawPolygon( pPoint2, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)-4 );
			printf("popup number up!!!!!!!!!\n");
		}
		else
		if( IsDOWN_press(Key) )	///down
		{
			///((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
			GUI_DrawPolygon( pPoint1, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)+4 );
			printf("popup number down!!!!!!!!!\n");
		}
	}
	else
#endif
	if( WM_PAINT == pMsg->MsgId )
	{
		FP_POPUPLIST_HEADER*	pPopList = NULL;

		if( 
			FRAMEPAGE_POPUP_NUMBERS != pCurrFramePageType ||
			NULL == pCurrFramePageFrameData
		)
		{
			printf("!!!!Error, there should popup list data here!! abort!!\n");
			return;
		}
	
		pPopList = pCurrFramePageFrameData;
		///in case we reach the top or bottom
		if( pPopList->iListSel == pPopList->iListNum-1 )
		{	///top
			///down arrow
			GUI_FillPolygon( pPoint1, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)+4 );
			///up arrow
			GUI_DrawPolygon( pPoint2, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)-4 );
		}
		else
		if( pPopList->iListSel == 0 )
		{	///bottom
			///down arrow
			GUI_DrawPolygon( pPoint1, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)+4 );
			///up arrow
			GUI_FillPolygon( pPoint2, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)-4 );
		}
		else
		{
			if( iFill )
			{
				///down arrow
				GUI_FillPolygon( pPoint1, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)+4 );
				///up arrow
				GUI_FillPolygon( pPoint2, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)-4 );
			}
			else
			{
				///down arrow
				GUI_DrawPolygon( pPoint1, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)+4 );
				///up arrow
				GUI_DrawPolygon( pPoint2, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)-4 );
			}
		}
	}
	GUI_SetColor( clTemp );
}


int spFramePageWait( void )
{
	int iRet = 0;

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
		if( iCnt <= 0 )
			iRet = 1;	///it's a timeout
	}
	pCurrFramePageNextReady = 0;
	
	return iRet;
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




#if (GUI_WINSUPPORT)
static void cbPoweroffWindow(WM_MESSAGE* pMsg)
{
	printf("cbPoweroffWindow() ==> %d \n", pMsg->MsgId);

	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		if( IsMSGUP_hold(pMsg) )	/// hold up key
			spGoAfterFramePage( &headBootWindow );
	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		GUI_DispStringAt("PowerOff...", 80, 30);
	}
	
	///go orignal callback
	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );
}
#endif

void PoweroffWindow( int iOption )
{
	WM_HWIN hWin1 = 0;	
	
	///give me a blank page
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();
	
	///create a window
	///hWin1 = WM_CreateWindow( 0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	hWin1 = WM_CreateWindow( 0, 0, 10, 10, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	
	///set callback
	pCurrFramePageOldCb = WM_SetCallback( hWin1, pCurrFramePageMainCb );
	
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
	int iTO = 0;

	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	GUI_DispStringAt("Trial version,\n", 30, 80 );
	GUI_DispStringAt("Disable after \n", 30, 100 );
	GUI_DispStringAt("a few actions!\n", 30, 120 );
	GUI_Delay(300);
	GUI_DrawBitmap(&bmStartup_Screen_1, 0, 0);
	
	iTO = spFramePageWait();

	///go timeout frame if we were timeout
	spFrameTimeoutHandling(iTO);
}



int spGetDataFrameX( int iFrameOlder, int iFrameTotal )
{
	int iRet = 0;
	
	switch( iFrameTotal )
	{
		case 1:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		case 2:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				case 1:
					iRet = 0;
					break;
				default:
					iRet = 0;
					break;
			}

			break;
		case 3:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				case 1:
					iRet = 0;
					break;
				case 2:
					iRet = 0;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		case 4:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				case 1:
					iRet = 0;
					break;
				case 2:
					iRet = 0;
					break;
				case 3:
					iRet = LCD_GetXSize()/2;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		case 5:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				case 1:
					iRet = 0;
					break;
				case 2:
					iRet = 0;
					break;
				case 3:
					iRet = LCD_GetXSize()/2;
					break;
				case 4:
					iRet = LCD_GetXSize()/2;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		case 6:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				case 1:
					iRet = 0;
					break;
				case 2:
					iRet = 0;
					break;
				case 3:
					iRet = LCD_GetXSize()/2;
					break;
				case 4:
					iRet = LCD_GetXSize()/2;
					break;
				case 5:
					iRet = LCD_GetXSize()/2;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		default:
			break;
	}
	
	return iRet;
}

int spGetDataFrameY( int iFrameOlder, int iFrameTotal )
{
	int iRet = 0;
	
	switch( iFrameTotal )
	{
		case 1:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		case 2:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				case 1:
					iRet = LCD_GetYSize()/2;
					break;
				default:
					iRet = 0;
					break;
			}

			break;
		case 3:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				case 1:
					iRet = LCD_GetYSize()/3;
					break;
				case 2:
					iRet = (LCD_GetYSize()*2)/3;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		case 4:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				case 1:
					iRet = LCD_GetYSize()/3;
					break;
				case 2:
					iRet = (LCD_GetYSize()*2)/3;
					break;
				case 3:
					iRet = (LCD_GetYSize()*2)/3;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		case 5:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				case 1:
					iRet = LCD_GetYSize()/3;
					break;
				case 2:
					iRet = (LCD_GetYSize()*2)/3;
					break;
				case 3:
					iRet = (LCD_GetYSize()*2)/3;
					break;
				case 4:
					iRet = LCD_GetYSize()/3;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		case 6:
			switch( iFrameOlder )
			{
				case 0:
					iRet = 0;
					break;
				case 1:
					iRet = LCD_GetYSize()/3;
					break;
				case 2:
					iRet = (LCD_GetYSize()*2)/3;
					break;;
				case 3:
					iRet = (LCD_GetYSize()*2)/3;
					break;
				case 4:
					iRet = LCD_GetYSize()/3;
					break;
				case 5:
					iRet = 0;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		default:
			break;
	}
	
	return iRet;
}

int spGetDataFrameXsize( int iFrameOlder, int iFrameTotal )
{
	int iRet = 0;
	
	switch( iFrameTotal )
	{
		case 1:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetXSize();
					break;
				default:
					iRet = LCD_GetXSize();
					break;
			}
			break;
		case 2:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetXSize();
					break;
				case 1:
					iRet = LCD_GetXSize();
					break;
				default:
					iRet = LCD_GetXSize();
					break;
			}
			break;
		case 3:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetXSize();
					break;
				case 1:
					iRet = LCD_GetXSize();
					break;
				case 2:
					iRet = LCD_GetXSize();
					break;
				default:
					iRet = LCD_GetXSize();
					break;
			}
			break;
		case 4:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetXSize();
					break;
				case 1:
					iRet = LCD_GetXSize();
					break;
				case 2:
					iRet = LCD_GetXSize()/2;
					break;
				case 3:
					iRet = LCD_GetXSize()/2;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		case 5:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetXSize();
					break;
				case 1:
					iRet = LCD_GetXSize()/2;
					break;
				case 2:
					iRet = LCD_GetXSize()/2;
					break;
				case 3:
					iRet = LCD_GetXSize()/2;
					break;
				case 4:
					iRet = LCD_GetXSize()/2;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		case 6:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetXSize()/2;
					break;
				case 1:
					iRet = LCD_GetXSize()/2;
					break;
				case 2:
					iRet = LCD_GetXSize()/2;
					break;
				case 3:
					iRet = LCD_GetXSize()/2;
					break;
				case 4:
					iRet = LCD_GetXSize()/2;
					break;
				case 5:
					iRet = LCD_GetXSize()/2;
					break;
				default:
					iRet = 0;
					break;
			}
			break;
		default:
			break;
	}
	
	return iRet;
}

int spGetDataFrameYsize( int iFrameOlder, int iFrameTotal )
{
	int iRet = 0;

	switch( iFrameTotal )
	{
		case 1:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetYSize();
					break;
				default:
					iRet = LCD_GetYSize();
					break;
			}
			break;
		case 2:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetYSize()/2;
					break;
				case 1:
					iRet = LCD_GetYSize()/2;
					break;
				default:
					iRet = LCD_GetYSize()/2;
					break;
			}

			break;
		case 3:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetYSize()/3;
					break;
				case 1:
					iRet = LCD_GetYSize()/3;
					break;
				case 2:
					iRet = LCD_GetYSize()/3;
					break;
				default:
					iRet = LCD_GetYSize()/3;
					break;
			}
			break;
		case 4:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetYSize()/3;
					break;
				case 1:
					iRet = LCD_GetYSize()/3;
					break;
				case 2:
					iRet = LCD_GetYSize()/3;
					break;
				case 3:
					iRet = LCD_GetYSize()/3;
					break;
				default:
					iRet = LCD_GetYSize()/3;
					break;
			}
			break;
		case 5:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetYSize()/3;
					break;
				case 1:
					iRet = LCD_GetYSize()/3;
					break;
				case 2:
					iRet = LCD_GetYSize()/3;
					break;
				case 3:
					iRet = LCD_GetYSize()/3;
					break;
				case 4:
					iRet = LCD_GetYSize()/3;
					break;
				default:
					iRet = LCD_GetYSize()/3;
					break;
			}
			break;
		case 6:
			switch( iFrameOlder )
			{
				case 0:
					iRet = LCD_GetYSize()/3;
					break;
				case 1:
					iRet = LCD_GetYSize()/3;
					break;
				case 2:
					iRet = LCD_GetYSize()/3;
					break;
				case 3:
					iRet = LCD_GetYSize()/3;
					break;
				case 4:
					iRet = LCD_GetYSize()/3;
					break;
				case 5:
					iRet = LCD_GetYSize()/3;
					break;
				default:
					iRet = LCD_GetYSize()/3;
					break;
			}
			break;
		default:
			break;
	}

	return iRet;
}

void spDrawDataModeContent( int* piTotal, int* piIndex, FP_DATASETS_HEADER* pDataModeData, GUI_RECT* prtTemp )
{
	const GUI_FONT* pFont = NULL;
	
	switch( *piTotal )
	{
		case 1:
		case 2:
			///draw data name
			pFont = GUI_GetFont();
			GUI_SetFont( &GUI_Font16B_ASCII );
			GUI_DispStringAt( pDataModeData->pDataSets[*piIndex].sDataName, prtTemp->x0+3, prtTemp->y0+3 );
			GUI_SetFont( pFont );
			///draw data unit
			pFont = GUI_GetFont();
			GUI_SetFont( &GUI_Font16B_ASCII );
			GUI_DispStringAt( pDataModeData->pDataSets[*piIndex].sDataUnit, prtTemp->x1-23, prtTemp->y1-18 );
			GUI_SetFont( pFont );
			///draw data value
			pFont = GUI_GetFont();
			GUI_SetFont( &GUI_Font32B_ASCII );
			GUI_DispStringAt( pDataModeData->pDataSets[*piIndex].sDataValue, prtTemp->x0+13, (prtTemp->y0+prtTemp->y1)/2-12 );
			GUI_SetFont( pFont );
			break;
		case 3:
		case 4:
		case 5:
		case 6:
		default:
			///draw data name
			GUI_DispStringAt( pDataModeData->pDataSets[*piIndex].sDataName, prtTemp->x0+3, prtTemp->y0+3 );
			///draw data unit
			GUI_DispStringAt( pDataModeData->pDataSets[*piIndex].sDataUnit, prtTemp->x1-23, prtTemp->y1-13 );
			///draw data value
			GUI_DispStringAt( pDataModeData->pDataSets[*piIndex].sDataValue, prtTemp->x0+13, (prtTemp->y0+prtTemp->y1)/2-3 );
			break;
	}
}


void spcbDataModeVal( WM_MESSAGE* pMsg, GUI_COLOR color )
{
	GUI_RECT 	rtTemp;
	GUI_COLOR 	clTemp[2];
	int			iIndex = 0;
	int			iTotal = 0;
	FP_DATASETS_HEADER* pDataModeData = NULL;
	
	
	if( !pMsg || (WM_PAINT != pMsg->MsgId) )
		return;
		
	if( 
		FRAMEPAGE_DATAMODE != pCurrFramePageType ||
		NULL == pCurrFramePageFrameData
	)
	{
		printf("!!!!Error, there should activity data mode data here!! abort!!\n");
		return;
	}
	pDataModeData = pCurrFramePageFrameData;
	
	WM_GetClientRectEx( pMsg->hWin, &rtTemp );
	WM_GetUserData( pMsg->hWin, &iIndex, sizeof(int) );
	iTotal = pDataModeData->iDataSetNum;
		
	clTemp[0] = GUI_GetColor();
	clTemp[1] = GUI_GetBkColor();
	GUI_SetColor( color );
	GUI_SetBkColor( GUI_WHITE );

#if 1
	spDrawDataModeContent( &iTotal, &iIndex, pDataModeData, &rtTemp );
#else	
	///draw data name
	GUI_DispStringAt( pDataModeData->pDataSets[iIndex].sDataName, rtTemp.x0+3, rtTemp.y0+3 );
	///draw data unit
	GUI_DispStringAt( pDataModeData->pDataSets[iIndex].sDataUnit, rtTemp.x1-23, rtTemp.y1-13 );
	///draw data value
	GUI_DispStringAt( pDataModeData->pDataSets[iIndex].sDataValue, rtTemp.x0+13, (rtTemp.y0+rtTemp.y1)/2-3 );
#endif

	GUI_SetColor( clTemp[0] );
	GUI_SetBkColor( clTemp[1] );

}


static FRAMEPAGE_HEADER* DataSetFrameList[] = {
	&headDataMode1Window,
	&headDataMode2Window,
	&headDataMode3Window,
	&headDataMode4Window,
	&headDataMode5Window,
	&headDataMode6Window
};

static DATASETS_FRAME_LIST		DataSetsList = {
	6,
	1,
	DataSetFrameList,
};

static FRAMEPAGE_HEADER* SAPDPADSASNSDataSetFrameList[] = {
	&headSAPDPADSASNS1Window,
	&headSAPDPADSASNS2Window,
	&headSAPDPADSASNS3Window,
	&headSAPDPADSASNS4Window,
	&headSAPDPADSASNS5Window,
	&headSAPDPADSASNS6Window
};

static DATASETS_FRAME_LIST		SAPDPADSASNSDataSetsList = {
	6,
	1,
	SAPDPADSASNSDataSetFrameList,
};


#if (GUI_WINSUPPORT)
static void cbDataModeWindow(WM_MESSAGE* pMsg)
{
	int iTmp = 0;
	
	WM_GetUserData( pMsg->hWin, &iTmp, sizeof(int) );
	printf("cbDataModeWindow() ==> %d user=%d\n", pMsg->MsgId, iTmp);

	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;			
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		
		if( IsUP_hold(Key) )	/// hold up key
			spGoAfterFramePage( &headPopupListWindow_DeviceModeFitness );
		else		
		if( IsDOWN_hold(Key) )	/// hold down key
			spGoAfterFramePage( &headPopupListWindow_Fitness );
		else
		if( IsUP_press(Key) )
		{
			if( DataSetsList.iCurrIdx == DataSetsList.iTotal )
				DataSetsList.iCurrIdx = 1;
			else
				DataSetsList.iCurrIdx++;
			
			spGoAfterFramePage( DataSetsList.pDataSetFrame[DataSetsList.iCurrIdx-1] );
		}
		else
		if( IsDOWN_press(Key) )
		{
			if( DataSetsList.iCurrIdx == 1 )
				DataSetsList.iCurrIdx = DataSetsList.iTotal;
			else
				DataSetsList.iCurrIdx--;
			
			spGoAfterFramePage( DataSetsList.pDataSetFrame[DataSetsList.iCurrIdx-1] );
		}		
///		if( IsUP_press(Key) || IsDOWN_press(Key) )
///			spGoAfterFramePage( &headPopupListWindow_NumberEntry );
	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		///draw outline
		spcbRoundWinExt( pMsg, GUI_BLUE, 0, 1, 1 );
		///draw content
		spcbDataModeVal( pMsg, GUI_BLUE );
	}
	
	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );
}
#endif

void DataModeWindow( int iOption )
{
	int iLoop = 0;
	int iTO = 0;
	WM_HWIN hDataWin[6];
	FP_DATASETS_HEADER* pDataModeData = NULL;
	///LISTBOX_Handle	hList;
	///WM_CALLBACK* pOldCB = NULL;

	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	if( 
		FRAMEPAGE_DATAMODE != pCurrFramePageType ||
		NULL == pCurrFramePageFrameData
	)
	{
		printf("!!!!Error, there should activity data mode data here!! abort!!\n");
		return;
	}
	
	pDataModeData = pCurrFramePageFrameData;
	
	for( iLoop=0; iLoop<pDataModeData->iDataSetNum; iLoop++ )
	{
		///create windows
		hDataWin[iLoop] = WM_CreateWindow(  
							spGetDataFrameX( iLoop, pDataModeData->iDataSetNum ),
							spGetDataFrameY( iLoop, pDataModeData->iDataSetNum ),
							spGetDataFrameXsize( iLoop, pDataModeData->iDataSetNum ),
							spGetDataFrameYsize( iLoop, pDataModeData->iDataSetNum ),
							WM_CF_SHOW|WM_CF_STAYONTOP,
							NULL,
							sizeof(int) );
			
		///add callback
		pCurrFramePageOldCb = WM_SetCallback( hDataWin[iLoop], pCurrFramePageMainCb );
		
		WM_SetUserData( hDataWin[iLoop], &(pDataModeData->pDataSets[iLoop].iIndex), sizeof(int) );
	}

	WM_BringToTop( hDataWin[0] );
	WM_SetFocus( hDataWin[0] );
	
	WM_ExecIdle();
	iTO = spFramePageWait();

	for( iLoop=0; iLoop<pDataModeData->iDataSetNum; iLoop++ )
		WM_DeleteWindow( hDataWin[iLoop] );
	
	///go timeout frame if we were timeout
	if( 0 != pCurrFramePageWait && 1 == iTO && NULL != pCurrFramePageTimeoutFrame )
		spGoAfterFramePage( pCurrFramePageTimeoutFrame );
}


#if (GUI_WINSUPPORT)
static void cbPopupWindow(WM_MESSAGE* pMsg)
{
	printf("cbPopupWindow() ==> %d \n", pMsg->MsgId);

	if( pMsg->MsgId == WM_PAINT )
	{
		/* Update info in command window */
		spcbRoundWinExt( pMsg, GUI_BLACK, 13, 2, 1 );
		if( FRAMEPAGE_POPUP_NUMBERS == pCurrFramePageType )
			spcbPopupNumberWinExt( pMsg, GUI_BLACK, 2, 1 );
		
		WM_DefaultProc(pMsg);
	}
/*	
	else
	if( pMsg->MsgId == WM_KEY )
	{
		if( FRAMEPAGE_POPUP_NUMBERS == pCurrFramePageType )
			spcbPopupNumberWinExt( pMsg, GUI_BLACK, 2, 1 );
		
		WM_DefaultProc(pMsg);
	}
*/
	return;

}

static void cbPopupWindowList(WM_MESSAGE* pMsg)
{
	///printf("cbPopupWindowList() ==> %d \n", pMsg->MsgId);
	
	if( WM_KEY == pMsg->MsgId )
	{
		int Key = 0;
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;

		///hack the key value
		if( IsUP_press(Key) )	///up
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
		else
		if( IsDOWN_press(Key) )	///down
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
		else
		if( IsENTER_press(Key) )	///enter
		{
			FP_POPUPLIST_HEADER*	pPopList = NULL;
			int iSelet = -1;
			///pBeforeFramePage = pCurrFramePage;
			///pCurrFramePage = pAfterFramePage;
		
			if( 
				FRAMEPAGE_POPUP_OPTION != pCurrFramePageType ||
				NULL == pCurrFramePageFrameData
			)
			{
				printf("!!!!Error, there should popup list data here!! abort!!\n");
				return;
			}
			
			pPopList = pCurrFramePageFrameData;
			///check what you selected
			iSelet = LISTBOX_GetSel(pMsg->hWin);
			if( iSelet < pPopList->iListNum )
			{
				FRAMEPAGE_HEADER*		pNextFrame = NULL;
				///jump to what you selected
				pNextFrame = pPopList->pListFrame[iSelet];
				spGoAfterFramePage( pNextFrame );
			}
			else
			{
				printf( "!!!!!Error!! LISTBOX_GetSel=%d\n", iSelet );
				spGoAfterFramePage( pBeforeFramePage );
			}	
		}
		else
		if( IsBACK_press(Key) )	///back
		{
			spGoAfterFramePage( pBeforeFramePage );
		}

		if( pCurrFramePageOldCb )
			pCurrFramePageOldCb( pMsg );
	}
	else
	{
		if( pCurrFramePageOldCb )
			pCurrFramePageOldCb( pMsg );
	}	
}
#endif

#define		EDGEOFFSET	20

void PopupWindowList( int iOption )
{
	WM_HWIN hWin1 = 0;
	LISTBOX_Handle	hList;
	WM_CALLBACK* pOldCB = NULL;
	FP_POPUPLIST_HEADER* pPopList = NULL;

	
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	spSetDefaultEffect();
	
	//need a blank space ???
	///spBlankRect( 0+EDGEOFFSET, 0+EDGEOFFSET, LCD_GetXSize()-(EDGEOFFSET*2), LCD_GetYSize()-(EDGEOFFSET*2) );
	
	if( 
		FRAMEPAGE_POPUP_OPTION != pCurrFramePageType ||
		NULL == pCurrFramePageFrameData
	)
	{
		printf("!!!!Error, there should popup list data here!! abort!!\n");
		return;
	}
	pPopList = pCurrFramePageFrameData;
	
	
	///create windows for popup outline
	hWin1 = WM_CreateWindow( 0+EDGEOFFSET, 0+EDGEOFFSET, LCD_GetXSize()-(EDGEOFFSET*2), LCD_GetYSize()-(EDGEOFFSET*2), WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pOldCB = WM_SetCallback( hWin1, &cbPopupWindow );
	pPopList->hWinFrame = hWin1;

	///create list box
	///hList = LISTBOX_CreateEx( 0+EDGEOFFSET+2, 0+EDGEOFFSET+13, LCD_GetXSize()-(EDGEOFFSET*2)-4, LCD_GetYSize()-(EDGEOFFSET*2)-(13*2), WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, _PopupListBox);
	if( FRAMEPAGE_POPUP_OPTION == pCurrFramePageType )
		hList = LISTBOX_CreateEx( 0+EDGEOFFSET+2, 0+EDGEOFFSET+13, LCD_GetXSize()-(EDGEOFFSET*2)-4, LCD_GetYSize()-(EDGEOFFSET*2)-(13*2), WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, pPopList->sListName);
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hList, pCurrFramePageMainCb );
	
	
	WM_BringToTop( hWin1 );
	WM_SetFocus( hWin1 );
	WM_BringToTop( hList );
	WM_SetFocus( hList );
	
	WM_ExecIdle();
	spFramePageWait();
	
	WM_DeleteWindow( hWin1 );
	WM_DeleteWindow( hList );

}



#if (GUI_WINSUPPORT)
static void spcbPopupWindowNumbersSelect(WM_MESSAGE* pMsg)
{
	FP_POPUPLIST_HEADER*	pPopList = NULL;
	int iSelet = -1;
	///pBeforeFramePage = pCurrFramePage;
	///pCurrFramePage = pAfterFramePage;
		
	if( 
		FRAMEPAGE_POPUP_NUMBERS != pCurrFramePageType ||
		NULL == pCurrFramePageFrameData
	)
	{
		printf("!!!!Error, there should popup list data here!! abort!!\n");
		return;
	}
	
	pPopList = pCurrFramePageFrameData;
	///check what you selected
	iSelet = LISTBOX_GetSel(pMsg->hWin);
	if( iSelet < pPopList->iListNum )
	{
		FRAMEPAGE_HEADER*		pNextFrame = NULL;
		///jump to what you selected
		pNextFrame = pPopList->pListFrame[iSelet];
		///store the selection for next
		pPopList->iListSel = iSelet;
		spGoAfterFramePage( pNextFrame );
	}
	else
	{
		printf( "!!!!!Error!! LISTBOX_GetSel=%d\n", iSelet );
		spGoAfterFramePage( pBeforeFramePage );
	}
}

static void cbPopupWindowNumbers(WM_MESSAGE* pMsg)
{
	printf("cbPopupWindowNumbers() ==> %d \n", pMsg->MsgId);
	
	if( WM_KEY == pMsg->MsgId )
	{
		int Key = 0;
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;

		///hack the key value
		if( IsUP_press(Key) )	///up
		{	///doing trick
			///((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
		}
		else
		if( IsDOWN_press(Key) )	///down
		{	///doing trick
			///((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
		}
		else
		if( IsENTER_press(Key) )	///enter
		{	
			spcbPopupWindowNumbersSelect(pMsg);
		}
		else
		if( IsBACK_press(Key) )	///back
		{
			spGoAfterFramePage( pBeforeFramePage );
		}

		if( pCurrFramePageOldCb )
			pCurrFramePageOldCb( pMsg );
	}
	else
	{
		if( pCurrFramePageOldCb )
			pCurrFramePageOldCb( pMsg );
	}
}

static void cbSAPDPADSASNSPopupWindowNumbers(WM_MESSAGE* pMsg)
{
	printf("cbSAPDPADSASNSPopupWindowNumbers() ==> %d \n", pMsg->MsgId);
	
	if( WM_KEY == pMsg->MsgId )
	{
		int Key = 0;
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;

		///hack the key value
		if( IsUP_press(Key) )	///up
		{	///doing trick
			///((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
			///special handling
			if( pCurrFramePageOldCb )
				pCurrFramePageOldCb( pMsg );
			spcbPopupWindowNumbersSelect(pMsg);
			return;
		}
		else
		if( IsDOWN_press(Key) )	///down
		{	///doing trick
			///((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
			///special handling
			if( pCurrFramePageOldCb )
				pCurrFramePageOldCb( pMsg );
			spcbPopupWindowNumbersSelect(pMsg);
			return;
		}
		else
		if( IsENTER_press(Key) )	///enter
		{
			spcbPopupWindowNumbersSelect(pMsg);
		}
		else
		if( IsBACK_press(Key) )	///back
		{
			spGoAfterFramePage( pBeforeFramePage );
		}

		if( pCurrFramePageOldCb )
			pCurrFramePageOldCb( pMsg );
	}
	else
	{
		if( pCurrFramePageOldCb )
			pCurrFramePageOldCb( pMsg );
	}
}
#endif


void PopupWindowNumbers( int iOption )
{
	WM_HWIN hWin1 = 0;
	LISTBOX_Handle	hList;
	WM_CALLBACK* pOldCB = NULL;
	FP_POPUPLIST_HEADER* pPopList = NULL;
	int iTO = 0;
	const GUI_FONT* pFont = NULL;
	int x, y, xsize, ysize;

	///set default setting
	spSetDefaultEffect();
	
	///set font for list
	///pFont = &GUI_Font16B_ASCII;
	pFont = &GUI_Font32B_ASCII;
	///set popup frame size
	x = 40;
	y = 56-4;
	xsize = 64;
	ysize = 56+8;
	
	//need a blank space ???
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	if( 
		FRAMEPAGE_POPUP_NUMBERS != pCurrFramePageType ||
		NULL == pCurrFramePageFrameData
	)
	{
		printf("!!!!Error, there should popup list data here!! abort!!\n");
		return;
	}
	
	pPopList = pCurrFramePageFrameData;
	///create windows for popup outline
	hWin1 = WM_CreateWindow( x, y, xsize, ysize, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pOldCB = WM_SetCallback( hWin1, &cbPopupWindow );
	///store header
	pPopList->hWinFrame = hWin1;

	///create list box
	///hList = LISTBOX_CreateEx( x+3, y+13+((ysize-(13*2))/4), (xsize/2)-4, (ysize-(13*2))/2, WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, pPopList->sListName);
	hList = LISTBOX_CreateEx( x+8, y+7+((ysize-(13*2))/4), (xsize/2)-12, GUI_GetYSizeOfFont(pFont), WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, pPopList->sListName);

	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hList, pCurrFramePageMainCb );
	
	///LISTBOX_SetTextAlign( hList, GUI_TA_HCENTER|GUI_TA_VCENTER );
	///set font in list
	LISTBOX_SetFont( hList , pFont );
	///set to default selected in list
	LISTBOX_SetSel( hList , pPopList->iListSel );
	
	///pull window to top
	WM_BringToTop( hWin1 );
	WM_SetFocus( hWin1 );
	WM_BringToTop( hList );
	WM_SetFocus( hList );
	
	WM_ExecIdle();
	iTO = spFramePageWait();
	
	///kill all window
	WM_DeleteWindow( hWin1 );
	WM_DeleteWindow( hList );
	
	///go timeout frame if we were timeout
	spFrameTimeoutHandling(iTO);
}


#if (GUI_WINSUPPORT)
static void cbNavigationWindow(WM_MESSAGE* pMsg)
{
	printf("cbNavigationWindow() ==> %d \n", pMsg->MsgId);
	
	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;
			
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( IsUP_hold(Key) )
		{	/// hold up key
			printf( "UP key hold!!!!!!!\n" );
			///set next framepage
			pAfterFramePage = &headPopupListWindow_DeviceModeFitness;
			///ready for next framepage
			pCurrFramePageNextReady = 1;
		}
		else		
		if( IsDOWN_hold(Key) )
		{	/// hold down key
			printf( "DOWN key hold!!!!!!!\n" );
			///set next framepage
			pAfterFramePage = &headPopupListWindow_Fitness;
			///ready for next framepage
			pCurrFramePageNextReady = 1;
		}

	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		///spcbRoundWinExt( pMsg, GUI_BLUE, 0, 1, 1 );
	}
	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );
	return;

}
#endif


void NavigationWindow( int iOption )
{
	FRAMEWIN_Handle	hFrame;
	int iTmp = 0;

	
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	spSetDefaultEffect();

	///create frame title
	hFrame = FRAMEWIN_CreateEx(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, "Navigation", NULL);
	iTmp = FRAMEWIN_GetTitleHeight( hFrame );
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hFrame, pCurrFramePageMainCb );
	
	WM_BringToTop( hFrame );
	WM_SetFocus( hFrame );

	
	WM_ExecIdle();
	spFramePageWait();
	
	WM_DeleteWindow( hFrame );
	
}


#if (GUI_WINSUPPORT)
static int spListBoxOwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
	int iRet = 0;
	
	///printf("spListBoxOwnerDraw x=%d y=%d idx=%d\n", pDrawItemInfo->x0, pDrawItemInfo->x0, pDrawItemInfo->ItemIndex);
	switch (pDrawItemInfo->Cmd) {
		case WIDGET_ITEM_GET_XSIZE:
			///printf("spListBoxOwnerDraw: WIDGET_ITEM_GET_XSIZE\n");
			///iRet = LISTBOX_OwnerDraw(pDrawItemInfo) + 100; /* Returns the default xsize+10 */
			///return iRet;
			break;
		case WIDGET_ITEM_GET_YSIZE:
			///printf("spListBoxOwnerDraw: WIDGET_ITEM_GET_YSIZE\n");
			///iRet = LISTBOX_OwnerDraw(pDrawItemInfo) - 0; /* Returns the default ysize+10 */
			///return iRet;
			break;
		case WIDGET_ITEM_DRAW:
			/* Your code to be added to draw the LISTBOX item */
			///printf("spListBoxOwnerDraw: WIDGET_ITEM_DRAW\n");
			iRet = LISTBOX_OwnerDraw(pDrawItemInfo);
			{
				FP_LISTMENU_HEADER* pListMenu = NULL;
	
				if( 
					IsNotCurrFPListMenuLike ||
					NULL == pCurrFramePageFrameData
				)	
				{
					printf("!!!!Error, there should list menu data here!! abort!!\n");
					return iRet;
				}
				
				pListMenu = pCurrFramePageFrameData;
				/*
				GUI_RECT rtTemp;
				rtTemp.x0 = 110;
				rtTemp.y0 = 0;
				rtTemp.x1 = 140;
				rtTemp.y1 = 160;
				GUI_FillRect(rtTemp.x0, rtTemp.y0, rtTemp.x1, rtTemp.y1);
				*/
				if( FRAMEPAGE_LISTMENU_BOOLOPTION == pCurrFramePageType )
				{	///handle the frame page with all boolean option in it!! 
					GUI_POINT pPoint1[] = {
									{ 0+0, 0+2},
									{ 0+0, 8+2},
									{ 8+0, 8+2},
									{ 8+0, 0+2},
								};
					GUI_POINT pPoint2[] = {
									{ 0+0+2, 0+2+2},
									{ 0+0+5, 0+2+7},
									{ 0+0+12, 0+2},
									{ 0+0+5, 0+2+5},
								};

					if( 1 == spGetListParamValue( pListMenu->pListParam[pDrawItemInfo->ItemIndex] ) )
					{
						///draw the square
						///GUI_DispStringAt("|v|", pDrawItemInfo->x0 + 130, pDrawItemInfo->y0);
						GUI_DrawPolygon( pPoint1, 4, pDrawItemInfo->x0 + 130, pDrawItemInfo->y0 );
						///mark the check
						///GUI_DrawLine(130+2,4,130+5,9);
						///GUI_DrawLine(130+5,9,130+12,2);
						GUI_FillPolygon( pPoint2, 4, pDrawItemInfo->x0 + 130, pDrawItemInfo->y0 );
					}
					else
					{
						///draw the square
						///GUI_DispStringAt("|_|", pDrawItemInfo->x0 + 130, pDrawItemInfo->y0);
						GUI_DrawPolygon( pPoint1, 4, pDrawItemInfo->x0 + 130, pDrawItemInfo->y0 );
					}
				}
				else
				if( BOOLEAN_OPTION_FIELD == spGetListParamType( pListMenu->pListParam[pDrawItemInfo->ItemIndex] ) )
				{	///handle the single element with boolean option
					GUI_POINT pPoint1[] = {
									{ 0+0, 0+2},
									{ 0+0, 8+2},
									{ 8+0, 8+2},
									{ 8+0, 0+2},
								};
					GUI_POINT pPoint2[] = {
									{ 0+0+2, 0+2+2},
									{ 0+0+5, 0+2+7},
									{ 0+0+12, 0+2},
									{ 0+0+5, 0+2+5},
								};
					if( 1 == spGetListParamValue( pListMenu->pListParam[pDrawItemInfo->ItemIndex] ) )
					{
						///draw the square
						///GUI_DispStringAt("|v|", pDrawItemInfo->x0 + 130, pDrawItemInfo->y0);
						GUI_DrawPolygon( pPoint1, 4, pDrawItemInfo->x0 + 130, pDrawItemInfo->y0 );
						///mark the check
						GUI_FillPolygon( pPoint2, 4, pDrawItemInfo->x0 + 130, pDrawItemInfo->y0 );					
					}
					else
					{
						///draw the square
						///GUI_DispStringAt("|_|", pDrawItemInfo->x0 + 130, pDrawItemInfo->y0);
						GUI_DrawPolygon( pPoint1, 4, pDrawItemInfo->x0 + 130, pDrawItemInfo->y0 );					
					}
				}
				else
				if( NULL != pListMenu->pListFrame[pDrawItemInfo->ItemIndex] )
				{	/// show -> if there is next level menu
					GUI_POINT pPoint1[] = {
									{ 0, 0+1},
									{ 0, 10+1},
									{ 7, 5+1},
								};
								
					if( FRAMEPAGE_LISTMENU == pListMenu->pListFrame[pDrawItemInfo->ItemIndex]->frametype )
					{
						///GUI_DispStringAt("->", pDrawItemInfo->x0 + 130, pDrawItemInfo->y0);
						GUI_FillPolygon( pPoint1, 3, pDrawItemInfo->x0 + 130, pDrawItemInfo->y0 );
					}
					else
					if( FRAMEPAGE_LISTMENU_BOOLOPTION == pListMenu->pListFrame[pDrawItemInfo->ItemIndex]->frametype )
					{
						int iLoop = 0;
						FP_LISTMENU_HEADER* pNextList = NULL;
						
						///GUI_DispStringAt("->", pDrawItemInfo->x0 + 130, pDrawItemInfo->y0);
						GUI_FillPolygon( pPoint1, 3, pDrawItemInfo->x0 + 130, pDrawItemInfo->y0 );
						
						
						///find the name of next boolean value for display
						pNextList = pListMenu->pListFrame[pDrawItemInfo->ItemIndex]->pFrameData;
						for( iLoop=0; iLoop<pNextList->iListNum; iLoop++ )
						{
							if( 
								BOOLEAN_OPTION_FIELD == spGetListParamType( pNextList->pListParam[iLoop] ) &&
								1 == spGetListParamValue( pNextList->pListParam[iLoop] )
							)
								break;
						}
						///show option selected in next level menu list
						///GUI_DispStringAt("<value>", pDrawItemInfo->x0 + 130 - 60, pDrawItemInfo->y0);
						///GUI_DispStringAt(pNextList->sListName[iLoop], pDrawItemInfo->x0 + 130 - 60, pDrawItemInfo->y0);
						///GUI_GetStringDistX( pListMenu->sListName[pDrawItemInfo->ItemIndex] );
						///GUI_DispStringLen
						GUI_DispStringAt(	
									pNextList->sListName[iLoop], 
									pDrawItemInfo->x0 + GUI_GetStringDistX( pListMenu->sListName[pDrawItemInfo->ItemIndex] ), 
									pDrawItemInfo->y0);
					}
				}
			}
			///iRet = LISTBOX_OwnerDraw(pDrawItemInfo);
			return iRet;
			break;
	}
	return LISTBOX_OwnerDraw(pDrawItemInfo); /* Def. function for unhandled cmds */
	///return iRet;
}

static void cbListMenuWindow(WM_MESSAGE* pMsg)
{
	///printf("cbListMenuWindow() ==> %d \n", pMsg->MsgId);
	
	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;
			
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( IsUP_press(Key) )	///up
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
		else
		if( IsDOWN_press(Key) )	///down
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
		else
		if( IsUP_hold(Key) )
		{	/// hold up key
			///set next framepage
			pAfterFramePage = &headPopupListWindow_DeviceModeFitness;
			///ready for next framepage
			pCurrFramePageNextReady = 1;
		}
		else		
		if( IsDOWN_hold(Key) )
		{	/// hold down key
			///set next framepage
			pAfterFramePage = &headPopupListWindow_Fitness;
			///ready for next framepage
			pCurrFramePageNextReady = 1;
		}
		else
		if( IsBACK_press(Key) )
		{	/// back key
			///set next framepage
			FP_LISTMENU_HEADER*	pListMenu = NULL;
			FRAMEPAGE_HEADER*	pNextFrame = NULL;
			int iSelet = -1;
			///pBeforeFramePage = pCurrFramePage;
			///pCurrFramePage = pAfterFramePage;
				
			if( 
				IsNotCurrFPListMenuLike ||
				NULL == pCurrFramePageFrameData
			)
			{
				printf("!!!!Error, there should list menu data here!! abort!!\n");
				return;
			}
			
			pListMenu = pCurrFramePageFrameData;
			if( NULL == pListMenu->pUplevelFrame )
			{
				///handle the exception of BACK key in menu list
				if( 
					&headSettingsWindow == pCurrFramePage ||					
					&headHistoryWindow == pCurrFramePage
				)
					pAfterFramePage = &headDataModeWindow;
				else
					pAfterFramePage = pBeforeFramePage;
			}
			else
				pAfterFramePage = pListMenu->pUplevelFrame;
				
			///ready for next framepage
			pCurrFramePageNextReady = 1;
		}
		else		
		if( IsENTER_press(Key) )
		{	/// enter key
			FP_LISTMENU_HEADER*	pListMenu = NULL;
			FRAMEPAGE_HEADER*	pNextFrame = NULL;
			int iSelet = -1;
			///pBeforeFramePage = pCurrFramePage;
			///pCurrFramePage = pAfterFramePage;

			if( 
				IsNotCurrFPListMenuLike ||
				NULL == pCurrFramePageFrameData
			)
			{
				printf("!!!!Error, there should list menu data here!! abort!!\n");
				return;
			}
			
			pListMenu = pCurrFramePageFrameData;
			///check what you selected
			iSelet = LISTBOX_GetSel(pMsg->hWin);

			if( FRAMEPAGE_LISTMENU_BOOLOPTION == pCurrFramePageType )
			{
				///if the selected was NOT enabled...
				if( 0 == spGetListParamValue( pListMenu->pListParam[iSelet] ) )
				{
					int iTmp = 0;
					///clean all
					for( iTmp=0; iTmp<pListMenu->iListNum; iTmp++ )
						pListMenu->pListParam[iTmp] = (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0;
						
					///set selected enable
					pListMenu->pListParam[iSelet] = (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1;
				}
				
				spGoAfterFramePage( pListMenu->pUplevelFrame );
			}
			else
			if( iSelet < pListMenu->iListNum )
			{
				int iPara = 0;
				int iTmp = 0;
				iPara = pListMenu->pListParam[iSelet];
				if( 0 < (iPara & (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0) )
				{	///it's boolean item/field
					if( 0 < (iPara & (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1) )
					{	///enabled boolean
						///invert it
						pListMenu->pListParam[iSelet] = (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0;
					}
					else
					{	///disabled boolean
						///invert it
						pListMenu->pListParam[iSelet] = (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1;
					}
					
					///iPara = iSelet+1;
					iPara = LISTBOX_GetNumItems( pMsg->hWin );
					for( iTmp=iSelet+1; iTmp<iPara; iTmp++ )
						LISTBOX_SetItemDisabled( pMsg->hWin, iTmp, (pListMenu->pListParam[iSelet] == ((BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1)?0:1 ) );
					
				}
				else
				{
					///jump to what you selected
					pNextFrame = pListMenu->pListFrame[iSelet];
					pAfterFramePage = pNextFrame;
					pCurrFramePageNextReady = 1;
				}
			}
			else
			{
				printf( "!!!!!LISTBOX_GetSel=%d\n", iSelet );
				pAfterFramePage = pBeforeFramePage;
				pCurrFramePageNextReady = 1;
			}	
		}

	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		///spcbRoundWinExt( pMsg, GUI_BLUE, 0, 1, 1 );
	}
	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );
	return;

}
#endif



void ListMenuWindow( int iOption )
{
	FRAMEWIN_Handle	hFrame;
	LISTBOX_Handle	hList;
	FP_LISTMENU_HEADER* pListMenu = NULL;
	int iTmp = 0;

	
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	spSetDefaultEffect();

	if( 
		IsNotCurrFPListMenuLike ||
		NULL == pCurrFramePageFrameData
	)
	{
		printf("!!!!Error, there should list menu data here!! abort!!\n");
		return;
	}
	
	pListMenu = pCurrFramePageFrameData;
	///create frame title
	hFrame = FRAMEWIN_CreateEx(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, pListMenu->sListTitle, NULL);
	iTmp = FRAMEWIN_GetTitleHeight( hFrame );

	///create list menu
	hList = LISTBOX_CreateEx(0, iTmp, LCD_GetXSize(), LCD_GetYSize()-iTmp, (WM_HWIN)hFrame, WM_CF_SHOW, 0, 0, pListMenu->sListName);
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hList, pCurrFramePageMainCb );
	
	LISTBOX_SetOwnerDraw( hList, spListBoxOwnerDraw );
	///LISTBOX_InvalidateItem( hList, 2 );
	
	
	WM_BringToTop( hList );
	WM_SetFocus( hList );

	
	WM_ExecIdle();
	spFramePageWait();

	WM_DeleteWindow( hList );	
	WM_DeleteWindow( hFrame );
}




#if (GUI_WINSUPPORT)

/*
	TextEntry list mode types 
*/
typedef enum
{
	TE_LIST_INIT = 0,
    TE_LIST_SUMMARY,
	TE_LIST_SINGLE,
} TE_LIST_TYPE;

static TEXT_Handle hText = NULL;
static unsigned iShowCnt = 0;
static char sShow[] = "               \0";


static void spTextEntrySummary( LISTBOX_Handle hList )
{
	if( hList )
	{
		LISTBOX_AddString( hList, "<-Bak" );
		LISTBOX_AddString( hList, "A/B/C" );
		LISTBOX_AddString( hList, "D/E/F" );
		LISTBOX_AddString( hList, "G/H/I" );
		LISTBOX_AddString( hList, "J/K/L" );
		LISTBOX_AddString( hList, "M/N/O" );
		LISTBOX_AddString( hList, "P/Q/R" );
		LISTBOX_AddString( hList, "S/T/U" );
		LISTBOX_AddString( hList, "V/W/X" );
		LISTBOX_AddString( hList, "Y/Z/ " );
		LISTBOX_AddString( hList, ",/./:" );
		LISTBOX_AddString( hList, "!/@/-" );
		LISTBOX_AddString( hList, "(/)/0" );
		LISTBOX_AddString( hList, "1/2/3" );
		LISTBOX_AddString( hList, "4/5/6" );
		LISTBOX_AddString( hList, "7/8/9" );
	}
}

static void spTextEntrySingle( LISTBOX_Handle hList, int iIndex )
{
	if( hList )
	{
		LISTBOX_AddString( hList, "<-Bak" );
		
		switch( iIndex )
		{
			case 1:
				LISTBOX_AddString( hList, "A" );
				LISTBOX_AddString( hList, "B" );
				LISTBOX_AddString( hList, "C" );
				break;
			case 2:
				LISTBOX_AddString( hList, "D" );
				LISTBOX_AddString( hList, "E" );
				LISTBOX_AddString( hList, "F" );
				break;
			case 3:
				LISTBOX_AddString( hList, "G" );
				LISTBOX_AddString( hList, "H" );
				LISTBOX_AddString( hList, "I" );
				break;
			case 4:
				LISTBOX_AddString( hList, "J" );
				LISTBOX_AddString( hList, "K" );
				LISTBOX_AddString( hList, "L" );
				break;
			case 5:
				LISTBOX_AddString( hList, "M" );
				LISTBOX_AddString( hList, "N" );
				LISTBOX_AddString( hList, "O" );
				break;
			case 6:
				LISTBOX_AddString( hList, "P" );
				LISTBOX_AddString( hList, "Q" );
				LISTBOX_AddString( hList, "R" );
				break;
			case 7:
				LISTBOX_AddString( hList, "S" );
				LISTBOX_AddString( hList, "T" );
				LISTBOX_AddString( hList, "U" );
				break;
			case 8:
				LISTBOX_AddString( hList, "V" );
				LISTBOX_AddString( hList, "W" );
				LISTBOX_AddString( hList, "X" );
				break;
			case 9:
				LISTBOX_AddString( hList, "Y" );
				LISTBOX_AddString( hList, "Z" );
				LISTBOX_AddString( hList, " " );
				break;
			case 10:
				LISTBOX_AddString( hList, "," );
				LISTBOX_AddString( hList, "." );
				LISTBOX_AddString( hList, ":" );
				break;
			case 11:
				LISTBOX_AddString( hList, "!" );
				LISTBOX_AddString( hList, "@" );
				LISTBOX_AddString( hList, "-" );
				break;
			case 12:
				LISTBOX_AddString( hList, "(" );
				LISTBOX_AddString( hList, ")" );
				LISTBOX_AddString( hList, "0" );
				break;
			case 13:
				LISTBOX_AddString( hList, "1" );
				LISTBOX_AddString( hList, "2" );
				LISTBOX_AddString( hList, "3" );
				break;
			case 14:
				LISTBOX_AddString( hList, "4" );
				LISTBOX_AddString( hList, "5" );
				LISTBOX_AddString( hList, "6" );
				break;
			case 15:
				LISTBOX_AddString( hList, "7" );
				LISTBOX_AddString( hList, "8" );
				LISTBOX_AddString( hList, "9" );
				break;
			default:
				break;
		}
	}
}


static void spcbTextEntryWindowTextSelect(WM_MESSAGE* pMsg, int Key)
{
	static TE_LIST_TYPE status = TE_LIST_INIT;
	FP_LISTMENU_HEADER*	pListMenu = NULL;
	int iSelet = -1;
		
	if( 
		IsNotCurrFPListMenuLike ||
		NULL == pCurrFramePageFrameData
	)
	{
		printf("!!!!Error, there should menu list data here!! abort!!\n");
		return;
	}
	
	if( TE_LIST_INIT == status )
		status = TE_LIST_SUMMARY;		///it's in summary mode, by default
	
	pListMenu = pCurrFramePageFrameData;
	///check what you just selected
	iSelet = LISTBOX_GetSel(pMsg->hWin);
	if( iSelet < pListMenu->iListNum )
	{
		unsigned iTmp = 0;
		char sOut[] = "a     \0";

		LISTBOX_GetItemText( pMsg->hWin, iSelet, sOut, 6 );
		
		///clean all
		iTmp = LISTBOX_GetNumItems(pMsg->hWin);
		for( ; 0<iTmp; iTmp-- )
			LISTBOX_DeleteItem( pMsg->hWin, 0 );

		///add new
		switch( iSelet )
		{
			case 0:
				///send backspace message
				if( iShowCnt > 0 )
				{
					iShowCnt--;
					sShow[iShowCnt] = ' ';
				}
				TEXT_SetText(hText, sShow);
				spTextEntrySummary( pMsg->hWin );
				status = TE_LIST_INIT;
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				if( TE_LIST_SUMMARY == status )
				{
					if( IsENTER_press(Key) )	///enter 
					{
						spTextEntrySingle( pMsg->hWin, iSelet );
						status = TE_LIST_SINGLE;
					}
					else
					if( IsBACK_press(Key) )	///back
					{
						spGoAfterFramePage( pListMenu->pUplevelFrame );
						status = TE_LIST_INIT;
					}
				}
				else
				if( TE_LIST_SINGLE == status )
				{
					if( IsENTER_press(Key) )	///enter 
					{
						printf("Get text entry ==> %s\n", sOut);
						///GUI_DispStringAt(sOut, 5, 10);
						if( iShowCnt < 15 )
						{
							sShow[iShowCnt] = sOut[0];
							iShowCnt++;
						}
						///send message to another window
						///TEXT_SetText(hText, sOut);
						TEXT_SetText(hText, sShow);
						
						spTextEntrySummary( pMsg->hWin );
						status = TE_LIST_INIT;
					}
					else
					if( IsBACK_press(Key) )	///back
					{
						spTextEntrySummary( pMsg->hWin );
						status = TE_LIST_SUMMARY;
					}		
				}
				break;
		
			
		}

	}
	else
	{
		printf( "!!!!!Error!! LISTBOX_GetSel=%d\n", iSelet );
		spGoAfterFramePage( pBeforeFramePage );
	}
}

static void cbTextEntryWindow(WM_MESSAGE* pMsg)
{
	///printf("cbTextEntryWindow() ==> %d \n", pMsg->MsgId);
	
	if( WM_KEY == pMsg->MsgId )
	{
		int Key = 0;
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;

		///hack the key value
		if( IsUP_press(Key) )	///up
		{	///doing trick
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
			///((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
		}
		else
		if( IsDOWN_press(Key) )	///down
		{	///doing trick
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
			///((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
		}
		else
		if( IsENTER_press(Key) )	///enter
		{	
			spcbTextEntryWindowTextSelect(pMsg, Key);
		}
		else
		if( IsBACK_press(Key) )	///back
		{
			///spGoAfterFramePage( pBeforeFramePage );
			spcbTextEntryWindowTextSelect(pMsg, Key);
		}

		if( pCurrFramePageOldCb )
			pCurrFramePageOldCb( pMsg );
	}
	else
	{
		if( pCurrFramePageOldCb )
			pCurrFramePageOldCb( pMsg );
	}
}
#endif

#define	 TEXTENTRYLIST_Y_OFFSET		15
#define  TEXTENTRYLIST_XSIZE		40
#define  TEXTENTRYLIST_YSIZE		(LCD_GetYSize()-TEXTENTRYLIST_Y_OFFSET-TEXTENTRYLIST_Y_OFFSET)
#define  TEXTENTRYLIST_X			(LCD_GetXSize()-TEXTENTRYLIST_XSIZE)
#define  TEXTENTRYLIST_Y			(0+TEXTENTRYLIST_Y_OFFSET)


void TextEntryWindow( int iOption )
{
	FRAMEWIN_Handle	hFrame;
	LISTBOX_Handle	hList;
	FP_LISTMENU_HEADER* pListMenu = NULL;
	///TEXT_Handle hText = NULL;
	int iTmp = 0;
	int iTO = 0;

	spSetDefaultEffect();
	WIDGET_SetDefaultEffect(&WIDGET_Effect_Simple);
	
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	if( 
		IsNotCurrFPListMenuLike ||
		NULL == pCurrFramePageFrameData
	)
	{
		printf("!!!!Error, there should list menu data here!! abort!!\n");
		return;
	}
	
	iShowCnt = 0;	///init
	pListMenu = pCurrFramePageFrameData;
	///create frame title
	hFrame = FRAMEWIN_CreateEx(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, pListMenu->sListTitle, NULL);
	iTmp = FRAMEWIN_GetTitleHeight( hFrame );

	hText = TEXT_CreateEx( 10, 40, 60, 20, hFrame, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 9, "Text here");
	
	
	///create list menu
	hList = LISTBOX_CreateEx(TEXTENTRYLIST_X, TEXTENTRYLIST_Y+iTmp, TEXTENTRYLIST_XSIZE, TEXTENTRYLIST_YSIZE-iTmp, (WM_HWIN)hFrame, WM_CF_SHOW, 0, 0, pListMenu->sListName);
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hList, pCurrFramePageMainCb );
	
	///LISTBOX_SetOwnerDraw( hList, spListBoxOwnerDraw );
	///LISTBOX_InvalidateItem( hList, 2 );
	
	///pull window up
	WM_BringToTop( hList );
	WM_SetFocus( hList );

	
	WM_ExecIdle();
	iTO = spFramePageWait();

	WM_DeleteWindow( hText );
	WM_DeleteWindow( hList );	
	WM_DeleteWindow( hFrame );
	
	WIDGET_SetDefaultEffect(&WIDGET_Effect_None);

	///go timeout frame if we were timeout
	spFrameTimeoutHandling(iTO);
}


#if (GUI_WINSUPPORT)
static void cbWatchWindow(WM_MESSAGE* pMsg)
{

	printf("cbPoweroffWindow() ==> %d \n", pMsg->MsgId);

	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;
			
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( IsUP_hold(Key) )
		{	/// hold up key
			printf( "UP key hold!!!!!!!\n" );
			///set next framepage
			pAfterFramePage = &headPopupListWindow_DeviceModeFitness;
			///ready for next framepage
			pCurrFramePageNextReady = 1;
		}
	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		GUI_DispStringAt("Watch Mode...", 80, 30);
	}
	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );
	return;

}
#endif



void WatchWindow( int iOption )
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
	
	GUI_DispStringAt("Watch Mode...", 45, 80);
	
    WM_ExecIdle();

	spFramePageWait();
	
	WM_DeleteWindow( hWin1 );
}


void UnderConstructionWindow( int iOption )
{
	WM_HWIN hWin1 = 0;
	
	///give me a blank page
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();
	
	///create a window
	hWin1 = WM_CreateWindow( 0, 0, 10, 10, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	
	///set callback
	///pCurrFramePageOldCb = WM_SetCallback( hWin1, pCurrFramePageMainCb );
	
	WM_BringToTop( hWin1 );
	WM_SetFocus( hWin1 );
	
	GUI_DispStringAt("Under Construction...", 10, 80);
	
    WM_ExecIdle();
	spFramePageWait();
	
	WM_DeleteWindow( hWin1 );
	pAfterFramePage = pBeforeFramePage;
}





static const GUI_ConstString _apListBox[] = {
  "English", "Deutsch", "Français", "Japanese", "Italiano", NULL
};








/*
*/
FRAMEPAGE_HEADER headPoweroffWindow = {
	FRAMEPAGE_COMMON,
	PoweroffWindow,
	cbPoweroffWindow,
	NULL,
	0,
	0,
	1,
	NULL,
	NULL,
};

/*
*/
FRAMEPAGE_HEADER headBootWindow = {
	FRAMEPAGE_COMMON,
	BootWindow,
	NULL,
	NULL,
	100,
	0,
	1,
	NULL,
	&headDataMode1Window,
};

FRAMEPAGE_HEADER headDataModeWindow = {
	FRAMEPAGE_ACTIVITY,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	NULL,
	NULL,
};


/*
	Data mode with 1 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_1Window[] = {
	{ 0, "distance", "/mi", "10.01" },
	{ 1, "time", "", "21:45" },
	{ 2, "avg pace", "/mi", "7:15" },
	{ 3, "elevation", "/ft", "155" },
	{ 4, "lap avg hr", "bpm", "118" },
	{ 5, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_1Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_1Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_1Window = {
	1,
	NULL,
	_DataModeContent_1Window,
	_DataModeFrame_1Window,
	_DataModeParam_1Window,
};

FRAMEPAGE_HEADER headDataMode1Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_1Window,
	NULL,
};


/*
	Data mode with 2 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_2Window[] = {
	{ 0, "distance", "/mi", "10.01" },
	{ 1, "time", "", "21:45" },
	{ 2, "avg pace", "/mi", "7:15" },
	{ 3, "elevation", "/ft", "155" },
	{ 4, "lap avg hr", "bpm", "118" },
	{ 5, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_2Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_2Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_2Window = {
	2,
	NULL,
	_DataModeContent_2Window,
	_DataModeFrame_2Window,
	_DataModeParam_2Window,
};

FRAMEPAGE_HEADER headDataMode2Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_2Window,
	NULL,
};


/*
	Data mode with 3 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_3Window[] = {
	{ 0, "distance", "/mi", "10.01" },
	{ 1, "time", "", "21:45" },
	{ 2, "avg pace", "/mi", "7:15" },
	{ 3, "elevation", "/ft", "155" },
	{ 4, "lap avg hr", "bpm", "118" },
	{ 5, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_3Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_3Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_3Window = {
	3,
	NULL,
	_DataModeContent_3Window,
	_DataModeFrame_3Window,
	_DataModeParam_3Window,
};

FRAMEPAGE_HEADER headDataMode3Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_3Window,
	NULL,
};


/*
	Data mode with 4 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_4Window[] = {
	{ 0, "distance", "/mi", "10.01" },
	{ 1, "time", "", "21:45" },
	{ 2, "avg pace", "/mi", "7:15" },
	{ 3, "elevation", "/ft", "155" },
	{ 4, "lap avg hr", "bpm", "118" },
	{ 5, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_4Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_4Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_4Window = {
	4,
	NULL,
	_DataModeContent_4Window,
	_DataModeFrame_4Window,
	_DataModeParam_4Window,
};

FRAMEPAGE_HEADER headDataMode4Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_4Window,
	NULL,
};


/*
	Data mode with 5 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_5Window[] = {
	{ 0, "distance", "/mi", "10.01" },
	{ 1, "time", "", "21:45" },
	{ 2, "avg pace", "/mi", "7:15" },
	{ 3, "elevation", "/ft", "155" },
	{ 4, "lap avg hr", "bpm", "118" },
	{ 5, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_5Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_5Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_5Window = {
	5,
	NULL,
	_DataModeContent_5Window,
	_DataModeFrame_5Window,
	_DataModeParam_5Window,
};

FRAMEPAGE_HEADER headDataMode5Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_5Window,
	NULL,
};


/*
	Data mode with 6 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_6Window[] = {
	{ 0, "distance", "/mi", "10.01" },
	{ 1, "time", "", "21:45" },
	{ 2, "avg pace", "/mi", "7:15" },
	{ 3, "elevation", "/ft", "155" },
	{ 4, "lap avg hr", "bpm", "118" },
	{ 5, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_6Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_6Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_6Window = {
	6,
	NULL,
	_DataModeContent_6Window,
	_DataModeFrame_6Window,
	_DataModeParam_6Window,
};

FRAMEPAGE_HEADER headDataMode6Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_6Window,
	NULL,
};


/*
	Popup Fitness
*/
static const GUI_ConstString _PopupListBox_Fitness[] = {
  "Activity", "Navigation", "Settings", "History", NULL
};

static FRAMEPAGE_HEADER* _PopupListFrame_Fitness[] = {
	&headDataMode1Window, &headNavigationWindow, &headSettingsWindow, &headHistoryWindow
};

static int _PopupListParam_Fitness[] = {
	0, 0, 0, 0
};

FP_POPUPLIST_HEADER fpPopupListData_Fitness = {
	4,
	0,
	NULL,
	_PopupListBox_Fitness,
	NULL,
	_PopupListFrame_Fitness,
	_PopupListParam_Fitness,
	0,
};

FRAMEPAGE_HEADER headPopupListWindow_Fitness = {
	FRAMEPAGE_POPUP_OPTION,
	PopupWindowList,
	cbPopupWindowList,
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupListData_Fitness,
	NULL,
};


/*
	Popup Watch
*/
static const GUI_ConstString _PopupListBox_DeviceModeFitness[] = {
  "Watch mode", "Power down", NULL
};

static FRAMEPAGE_HEADER* _PopupListFrame_DeviceModeFitness[] = {
	&headWatchWindow, &headPoweroffWindow
};

static int _PopupListParam_DeviceModeFitness[] = {
	0, 0
};

FP_POPUPLIST_HEADER fpPopupListData_DeviceModeFitness = {
	2,
	0,
	NULL,
	_PopupListBox_DeviceModeFitness,
	NULL,
	_PopupListFrame_DeviceModeFitness,
	_PopupListParam_DeviceModeFitness,
	0,
};

FRAMEPAGE_HEADER headPopupListWindow_DeviceModeFitness = {
	FRAMEPAGE_POPUP_OPTION,
	PopupWindowList,
	cbPopupWindowList,
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupListData_DeviceModeFitness,
	NULL,
};


/*
	Popup Number entry
*/
static const GUI_ConstString _PopupListBox_NumberEntry[] = {
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", NULL
};

static FRAMEPAGE_HEADER* _PopupListFrame_NumberEntry[] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

static int _PopupListParam_NumberEntry[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

FP_POPUPLIST_HEADER fpPopupListData_NumberEntry = {
	10,
	0,
	NULL,
	_PopupListBox_NumberEntry,
	NULL,
	_PopupListFrame_NumberEntry,
	_PopupListParam_NumberEntry,
	0,
};

FRAMEPAGE_HEADER headPopupListWindow_NumberEntry = {
	FRAMEPAGE_POPUP_NUMBERS,
	PopupWindowNumbers,
	cbPopupWindowNumbers,
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupListData_NumberEntry,
	NULL,
};


/*
*/
FRAMEPAGE_HEADER headPopupListWindow = {
	FRAMEPAGE_POPUP_OPTION,
	PopupWindowList,
	cbPopupWindowList,
	NULL,
	0,
	0,
	0,
	NULL,
	NULL,
};


/*
	Navigation
*/
FRAMEPAGE_HEADER headNavigationWindow = {
	FRAMEPAGE_TITLED,
	NavigationWindow,
	cbNavigationWindow,
	NULL,
	0,
	0,
	1,
	NULL,
	NULL,
};


/*
	Settings 
*/
static const GUI_ConstString _SettingListBox[] = {
  "GPS Settings",
  "Device Settings",
  "Activity Profiles",
  "User Information",
  "Device Information",
  NULL
};

static FRAMEPAGE_HEADER* _SettingListFrame[] = {
	&headSGSWindow,
	&headSDSWindow,
	&headSAPWindow,
	&headSUIWindow,
	&headUnderConstructionWindow
};

static int _SettingListParam[] = {
	0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SettingsWindow = {
	5,
	"Settings",
	_SettingListBox,
	NULL,
	_SettingListFrame,
	_SettingListParam,
};

FRAMEPAGE_HEADER headSettingsWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SettingsWindow,
	NULL,
};


/*
	Settings / GPS Settings
*/
static const GUI_ConstString _SGSListBox[] = {
	"GPS Enabled: ",
	"GPS Satellites",
	"Navigation",
	NULL
};

static FRAMEPAGE_HEADER* _SGSListFrame[] = {
	&headUnderConstructionWindow,
	&headSGSGSWindow,
	&headSGSNWindow
};

static int _SGSListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1,
	0, 
	0
};

FP_LISTMENU_HEADER fpListMenuData_SGSWindow = {
	3,
	"GPS Settings",
	_SGSListBox,
	&headSettingsWindow,
	_SGSListFrame,
	_SGSListParam,
};

FRAMEPAGE_HEADER headSGSWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SGSWindow,
	NULL,
};


/*
	Settings / GPS Settings / GPS Satellites
*/
static const GUI_ConstString _SGSGSListBox[] = {
	"GPS Satellites",
	"GPS Satellites",
	"GPS Satellites",
	NULL
};

static FRAMEPAGE_HEADER* _SGSGSListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SGSGSListParam[] = {
	0,
	0, 
	0
};

FP_LISTMENU_HEADER fpListMenuData_SGSGSWindow = {
	3,
	"GPS Satellites",
	_SGSGSListBox,
	&headSGSWindow,
	_SGSGSListFrame,
	_SGSGSListParam,
};

FRAMEPAGE_HEADER headSGSGSWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SGSGSWindow,
	NULL,
};


/*
	Settings / GPS Settings / Navigation
*/
static const GUI_ConstString _SGSNListBox[] = {
	"GPS Navigation",
	"GPS Navigation",
	"GPS Navigation",
	NULL
};

static FRAMEPAGE_HEADER* _SGSNListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SGSNListParam[] = {
	0,
	0, 
	0
};

FP_LISTMENU_HEADER fpListMenuData_SGSNWindow = {
	3,
	"Navigation",
	_SGSNListBox,
	&headSGSWindow,
	_SGSNListFrame,
	_SGSNListParam,
};

FRAMEPAGE_HEADER headSGSNWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SGSNWindow,
	NULL,
};



/*
	Settings / User Information
*/
static const GUI_ConstString _SUIListBox[] = {
	"Physical Information",
	"Contact Information",
	"Emergency Information",
	NULL
};

static FRAMEPAGE_HEADER* _SUIListFrame[] = {
	&headSUIPIWindow,
	&headSUICIWindow,
	&headUnderConstructionWindow
};

static int _SUIListParam[] = {
	0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SUIWindow = {
	3,
	"User Information",
	_SUIListBox,
	&headSettingsWindow,
	_SUIListFrame,
	_SUIListParam,
};

FRAMEPAGE_HEADER headSUIWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUIWindow,
	NULL,
};


/*
	Settings / User Information / Physical Information
*/
static const GUI_ConstString _SUIPIListBox[] = {
	"Age",
	"Gender",
	"Height",
	"Weight",
	"Resting Heart Rate",
	NULL
};

static FRAMEPAGE_HEADER* _SUIPIListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SUIPIListParam[] = {
	0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SUIPIWindow = {
	5,
	"Physical Information",
	_SUIPIListBox,
	&headSUIWindow,
	_SUIPIListFrame,
	_SUIPIListParam,
};

FRAMEPAGE_HEADER headSUIPIWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUIPIWindow,
	NULL,
};


/*
	Settings / User Information / Contact Information
*/
static const GUI_ConstString _SUICIListBox[] = {
	"Phone",
	"E-mail",
	"Address",
	NULL
};

static FRAMEPAGE_HEADER* _SUICIListFrame[] = {
	&headSUICIPWindow,
	&headSUICIEWindow,
	&headSUICIAWindow
};

static int _SUICIListParam[] = {
	0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SUICIWindow = {
	3,
	"Contact Information",
	_SUICIListBox,
	&headSUIWindow,
	_SUICIListFrame,
	_SUICIListParam,
};

FRAMEPAGE_HEADER headSUICIWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUICIWindow,
	NULL,
};


/*
	Settings / User Information / Contact Information / Address
*/
static const GUI_ConstString _SUICIAListBox[] = {
	"<-Bak",
	"A/B/C",
	"D/E/F",
	"G/H/I",
	"J/K/L",
	"M/N/O",
	"P/Q/R",
	"S/T/U",
	"V/W/X",
	"Y/Z/ ",
	",/./:",
	"!/@/-",
	"(/)/0",
	"1/2/3",
	"4/5/6",
	"7/8/9",
	NULL
};

static FRAMEPAGE_HEADER* _SUICIAListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SUICIAListParam[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SUICIAWindow = {
	16,
	"Address",
	_SUICIAListBox,
	&headSUICIWindow,
	_SUICIAListFrame,
	_SUICIAListParam,
};

FRAMEPAGE_HEADER headSUICIAWindow = {
	FRAMEPAGE_LISTMENU,
	TextEntryWindow,
	cbTextEntryWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUICIAWindow,
	NULL,
};


/*
	Settings / User Information / Contact Information / Phone
*/
FP_LISTMENU_HEADER fpListMenuData_SUICIPWindow = {
	16,
	"Phone",
	_SUICIAListBox,
	&headSUICIWindow,
	_SUICIAListFrame,
	_SUICIAListParam,
};

FRAMEPAGE_HEADER headSUICIPWindow = {
	FRAMEPAGE_LISTMENU,
	TextEntryWindow,
	cbTextEntryWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUICIPWindow,
	NULL,
};


/*
	Settings / User Information / Contact Information / Email
*/
FP_LISTMENU_HEADER fpListMenuData_SUICIEWindow = {
	16,
	"Email",
	_SUICIAListBox,
	&headSUICIWindow,
	_SUICIAListFrame,
	_SUICIAListParam,
};

FRAMEPAGE_HEADER headSUICIEWindow = {
	FRAMEPAGE_LISTMENU,
	TextEntryWindow,
	cbTextEntryWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUICIEWindow,
	NULL,
};



/*
	Settings / Device Settings
*/
static const GUI_ConstString _SDSListBox[] = {
  "Language",
  "Date & Time",
  "Units of Measurement",
  "Activity Recording",
  "ANT+ Sensors",
  "Feedback",
  "Equipment",
  NULL
};

static FRAMEPAGE_HEADER* _SDSListFrame[] = {
	&headSDSLWindow,
	&headUnderConstructionWindow,
	&headSDSUMWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SDSListParam[] = {
	0, 0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SDSWindow = {
	7,
	"Device Settings",
	_SDSListBox,
	&headSettingsWindow,
	_SDSListFrame,
	_SDSListParam,
};

FRAMEPAGE_HEADER headSDSWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSWindow,
	NULL,
};


/*
	Settings / Device Settings / Language
*/
static const GUI_ConstString _SDSLListBox[] = {
  "English",
  "Spanish",
  "French",
  "Italian",
  "Portuguese",
  "Dutch",
  "German",
  "Danish",
  "Greek",
  "Nordic",
  NULL
};

static FRAMEPAGE_HEADER* _SDSLListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SDSLListParam[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


FP_LISTMENU_HEADER fpListMenuData_SDSLWindow = {
	10,
	"Language",
	_SDSLListBox,
	&headSDSWindow,
	_SDSLListFrame,
	_SDSLListParam,
};

FRAMEPAGE_HEADER headSDSLWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSLWindow,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement
*/
static const GUI_ConstString _SDSUMListBox[] = {
  "Coordinates: ",
  "Speed/Distance: ",
  "Elevation: ",
  "Height/Weight: ",
  "Temperature: ",
  "Heart Rate: ",
  "Power: ",
  NULL
};

static FRAMEPAGE_HEADER* _SDSUMListFrame[] = {
	&headSDSUMCWindow,
	&headSDSUMSDWindow,
	&headSDSUMEWindow,
	&headSDSUMHWWindow,
	&headSDSUMTWindow,
	&headSDSUMHRWindow,
	&headSDSUMPWindow,
};

static int _SDSUMListParam[] = {
	0, 0, 0, 0, 0, 0, 0
};


FP_LISTMENU_HEADER fpListMenuData_SDSUMWindow = {
	7,
	"Units of Measurement",
	_SDSUMListBox,
	&headSDSWindow,
	_SDSUMListFrame,
	_SDSUMListParam,
};

FRAMEPAGE_HEADER headSDSUMWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMWindow,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Coordinates
*/
static const GUI_ConstString _SDSUMCListBox[] = {
	"Decimal Degrees",
	"Degrees Minutes",
	"Degrees Minutes Seconds",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMCListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMCListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMCWindow = {
	3,
	"Coordinates",
	_SDSUMCListBox,
	&headSDSUMWindow,
	_SDSUMCListFrame,
	_SDSUMCListParam,
};

FRAMEPAGE_HEADER headSDSUMCWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMCWindow,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Speed/Distance
*/
static const GUI_ConstString _SDSUMSDListBox[] = {
	"Metric",
	"Statute",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMSDListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMSDListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMSDWindow = {
	2,
	"Speed/Distance",
	_SDSUMSDListBox,
	&headSDSUMWindow,
	_SDSUMSDListFrame,
	_SDSUMSDListParam,
};

FRAMEPAGE_HEADER headSDSUMSDWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMSDWindow,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Elevation
*/
static const GUI_ConstString _SDSUMEListBox[] = {
	"Metric",
	"Statute",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMEListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMEListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMEWindow = {
	2,
	"Elevation",
	_SDSUMEListBox,
	&headSDSUMWindow,
	_SDSUMEListFrame,
	_SDSUMEListParam,
};

FRAMEPAGE_HEADER headSDSUMEWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMEWindow,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Height/Weight
*/
static const GUI_ConstString _SDSUMHWListBox[] = {
	"Metric",
	"Statute",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMHWListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMHWListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMHWWindow = {
	2,
	"Height/Weight",
	_SDSUMHWListBox,
	&headSDSUMWindow,
	_SDSUMHWListFrame,
	_SDSUMHWListParam,
};

FRAMEPAGE_HEADER headSDSUMHWWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMHWWindow,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Temperature
*/
static const GUI_ConstString _SDSUMTListBox[] = {
	"Metric",
	"Statute",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMTListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMTListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMTWindow = {
	2,
	"Temperature",
	_SDSUMTListBox,
	&headSDSUMWindow,
	_SDSUMTListFrame,
	_SDSUMTListParam,
};

FRAMEPAGE_HEADER headSDSUMTWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMTWindow,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Heart Rate
*/
static const GUI_ConstString _SDSUMHRListBox[] = {
	"BPM",
	"%MHR",
	"%HRR",
	"Zone",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMHRListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMHRListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0,
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0,
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMHRWindow = {
	4,
	"Heart Rate",
	_SDSUMHRListBox,
	&headSDSUMWindow,
	_SDSUMHRListFrame,
	_SDSUMHRListParam,
};

FRAMEPAGE_HEADER headSDSUMHRWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMHRWindow,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Power
*/
static const GUI_ConstString _SDSUMPListBox[] = {
	"Watts",
	"%FTP",
	"kJ",
	"Zone",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMPListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMPListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0,
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0,
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMPWindow = {
	4,
	"Power",
	_SDSUMPListBox,
	&headSDSUMWindow,
	_SDSUMPListFrame,
	_SDSUMPListParam,
};

FRAMEPAGE_HEADER headSDSUMPWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMPWindow,
	NULL,
};


/*
	Settings / Activity Profiles
*/
static const GUI_ConstString _SAPListBox[] = {
  "Enabled:",
  "Name:",
  "Activity Type:",
  "Display Preferences",
  "Recording Preferences",
  "Training Zones",
  "Train Alerts",
  "Equipment",
  "Copy Settings From",
  NULL
};

static FRAMEPAGE_HEADER* _SAPListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headSAPDPWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SAPListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 0, 0, 0, 0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SAPWindow = {
	9,
	"Activity Profile Setting",
	_SAPListBox,
	&headSettingsWindow,
	_SAPListFrame,
	_SAPListParam,
};

FRAMEPAGE_HEADER headSAPWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPWindow,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences 
*/
static const GUI_ConstString _SAPDPListBox[] = {
  "Speed/Pace: ",
  "Activity Data Screen",
  "Activity Pacer",
  "Auto Scroll:",
  "Scroll Speed: ",
  NULL
};

static FRAMEPAGE_HEADER* _SAPDPListFrame[] = {
	&headSAPDPSPWindow,
	&headSAPDPADSWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headSAPDPSSWindow
};

static int _SAPDPListParam[] = {
	0,
	0, 
	0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	0
};

FP_LISTMENU_HEADER fpListMenuData_SAPDPWindow = {
	5,
	"Display Preferences",
	_SAPDPListBox,
	&headSAPWindow,
	_SAPDPListFrame,
	_SAPDPListParam,
};

FRAMEPAGE_HEADER headSAPDPWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPDPWindow,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences / Speed/Pace
*/
static const GUI_ConstString _SAPDPSPListBox[] = {
	"Speed",
	"Pace",
	NULL
};

static FRAMEPAGE_HEADER* _SAPDPSPListFrame[] = {
	NULL,
	NULL
};

static int _SAPDPSPListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SAPDPSPWindow = {
	2,
	"Speed/Pace",
	_SAPDPSPListBox,
	&headSAPDPWindow,
	_SAPDPSPListFrame,
	_SAPDPSPListParam,
};

FRAMEPAGE_HEADER headSAPDPSPWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPDPSPWindow,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences / Scroll Speed
*/
static const GUI_ConstString _SAPDPSSListBox[] = {
	"Slow",
	"Medium",
	"Fast",
	NULL
};

static FRAMEPAGE_HEADER* _SAPDPSSListFrame[] = {
	NULL,
	NULL,
	NULL
};

static int _SAPDPSSListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SAPDPSSWindow = {
	3,
	"Scroll Speed",
	_SAPDPSSListBox,
	&headSAPDPWindow,
	_SAPDPSSListFrame,
	_SAPDPSSListParam,
};

FRAMEPAGE_HEADER headSAPDPSSWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPDPSSWindow,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences / Activity Data Screen
*/
static const GUI_ConstString _SAPDPADSListBox[] = {
  "Add Screen",
  NULL
};

static FRAMEPAGE_HEADER* _SAPDPADSListFrame[] = {
	&headSAPDPADSASWindow
};

static int _SAPDPADSListParam[] = {
	0
};

FP_LISTMENU_HEADER fpListMenuData_SAPDPADSWindow = {
	1,
	"Activity Data Screens",
	_SAPDPADSListBox,
	&headSAPDPWindow,
	_SAPDPADSListFrame,
	_SAPDPADSListParam,
};

FRAMEPAGE_HEADER headSAPDPADSWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPDPADSWindow,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences / Activity Data Screen / Add Screen
*/
static const GUI_ConstString _SAPDPADSASListBox[] = {
  "New Screen",
  "Copy Last Screen",
  NULL
};

static FRAMEPAGE_HEADER* _SAPDPADSASListFrame[] = {
	&headSAPDPADSASNS1Window,
	&headUnderConstructionWindow
};

static int _SAPDPADSASListParam[] = {
	0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SAPDPADSASWindow = {
	2,
	"Add Screen",
	_SAPDPADSASListBox,
	&headSAPDPADSWindow,
	_SAPDPADSASListFrame,
	_SAPDPADSASListParam,
};

FRAMEPAGE_HEADER headSAPDPADSASWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPDPADSASWindow,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences / Activity Data Screen / Add Screen / New Screen
*/
/// we will re-use old data struct from above
FRAMEPAGE_HEADER headSAPDPADSASNS1Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_1Window,
	&headSAPDPADSASNSWindow_NumberEntry,
};

FRAMEPAGE_HEADER headSAPDPADSASNS2Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_2Window,
	&headSAPDPADSASNSWindow_NumberEntry,
};

FRAMEPAGE_HEADER headSAPDPADSASNS3Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_3Window,
	&headSAPDPADSASNSWindow_NumberEntry,
};

FRAMEPAGE_HEADER headSAPDPADSASNS4Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_4Window,
	&headSAPDPADSASNSWindow_NumberEntry,
};

FRAMEPAGE_HEADER headSAPDPADSASNS5Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_5Window,
	&headSAPDPADSASNSWindow_NumberEntry,
};

FRAMEPAGE_HEADER headSAPDPADSASNS6Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_6Window,
	&headSAPDPADSASNSWindow_NumberEntry,
};

/*
	Popup Number entry for
	Settings / Activity Profiles / Display Preferences / Activity Data Screen / Add Screen / New Screen
*/
static const GUI_ConstString _PopupListBox_SAPDPADSASNSNumberEntry[] = {
  "1", "2", "3", "4", "5", "6", NULL
};

/* */
static FRAMEPAGE_HEADER* _PopupListFrame_SAPDPADSASNSNumberEntry[] = {
	&headSAPDPADSASNS1Window, 
	&headSAPDPADSASNS2Window, 
	&headSAPDPADSASNS3Window, 
	&headSAPDPADSASNS4Window, 
	&headSAPDPADSASNS5Window, 
	&headSAPDPADSASNS6Window
};

static int _PopupListParam_SAPDPADSASNSNumberEntry[] = {
	0, 0, 0, 0, 0, 0
};

FP_POPUPLIST_HEADER fpPopupListData_SAPDPADSASNSNumberEntry = {
	6,
	0,
	NULL,
	_PopupListBox_SAPDPADSASNSNumberEntry,
	NULL,
	_PopupListFrame_SAPDPADSASNSNumberEntry,
	_PopupListParam_SAPDPADSASNSNumberEntry,
	0,
};

FRAMEPAGE_HEADER headSAPDPADSASNSWindow_NumberEntry = {
	FRAMEPAGE_POPUP_NUMBERS,
	PopupWindowNumbers,
	cbSAPDPADSASNSPopupWindowNumbers,	///special handling
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupListData_SAPDPADSASNSNumberEntry,
	NULL,
};







/*
	History
*/
static const GUI_ConstString _HistoryListBox[] = {
  "Activity History", "Activity Totals", "Location History", NULL
};

static FRAMEPAGE_HEADER* _HistoryListFrame[] = {
	&headUnderConstructionWindow, &headUnderConstructionWindow, &headUnderConstructionWindow
};

static int _HistoryListParam[] = {
	0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_HistoryWindow = {
	3,
	"History",
	_HistoryListBox,
	NULL,
	_HistoryListFrame,
	_HistoryListParam,
};

FRAMEPAGE_HEADER headHistoryWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_HistoryWindow,
	NULL,
};


/*
	Watch mode
*/
FRAMEPAGE_HEADER headWatchWindow = {
	FRAMEPAGE_COMMON,
	WatchWindow,
	cbWatchWindow,
	NULL,
	0,
	0,
	1,
	NULL,
	NULL,
};


/*
	underconstruction
*/
FRAMEPAGE_HEADER headUnderConstructionWindow = {
	FRAMEPAGE_COMMON,
	UnderConstructionWindow,
	NULL,
	NULL,
	100,
	0,
	1,
	NULL,
	NULL,
};


void FrameCenter( void )
{
	int iLoop = 55;
    GUI_CONTEXT ContextOld;

	///set the first FramePage header
	pCurrFramePage = &headPoweroffWindow;

	spClearScreen();

#if 1
	while( iLoop > 0 )
	{
		GUI_SaveContext(&ContextOld);
		///pCurrFramePage->pfnFramePageMain( iLoop );
		pCurrFramePageMain( iLoop );
		GUI_RestoreContext(&ContextOld);
		///spClearScreen();
		iLoop--;

		pBeforeFramePage = pCurrFramePage;
		pCurrFramePage = pAfterFramePage;
		///pAfterFramePage =
		
		if( 1 == iLoop)
			pCurrFramePage = &headUnderConstructionWindow;
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















#endif	/* #if defined(RENOUI_PROJ) */
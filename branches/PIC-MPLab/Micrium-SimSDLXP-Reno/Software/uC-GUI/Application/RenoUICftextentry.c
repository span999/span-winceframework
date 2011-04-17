
/*
	body of UI frame page list menu functions
*/

#ifdef RENOUI_PROJ
/* ********************************************************* */


#include "RenoGUI.h"
#include "RenoUItoolhelp.h"
#include "RenoUIframe.h"
#include "RenoUIcommon.h"

#include "RenoUICcomm.h"
#include "RenoUICftextentry.h"




#define		FTEXTFRAME_X			20
#define		FTEXTFRAME_Y			20
#define		FTEXTFRAME_XSIZE		20
#define		FTEXTFRAME_YSIZE		50


#define		FTEXTENTRY_X			1
#define		FTEXTENTRY_Y			15
#define		FTEXTENTRY_XSIZE		(FTEXTFRAME_XSIZE-(FTEXTENTRY_X*2))
#define		FTEXTENTRY_YSIZE		30	///??


static unsigned iTextCnt = 0;
static char sTextShow[] = "                   \0";



#if (GUI_WINSUPPORT)
static void cbFloatTextWindow(WM_MESSAGE* pMsg)
{
	GUI_POINT pPoint1[] = {
		{ 0, 0},
		{18, 0},
		{9,9},
	};
	GUI_POINT pPoint2[] = {
		{ 0, 0},
		{18, 0},
		{9,-9},
	};

	SPPRINTF("cbFloatTextWindow() ==> %d \n", pMsg->MsgId);

	if( pMsg->MsgId == WM_PAINT )
	{
		FP_FTEXTENTRY_HEADER* pFTextEntry = NULL;
		GUI_RECT rtTemp;
		
		if( spFramePageValid( FRAMEPAGE_FLOATTEXT_ENTRY ) )
			return;

		GUI_SetColor(GUI_BLACK);
		GUI_SetBkColor(GUI_WHITE); 
	
		pFTextEntry = pCurrFramePageFrameData;
		WM_GetClientRectEx( pFTextEntry->hWinInputFrame, &rtTemp );
		GUI_FillRectEx(&rtTemp);
		spBlankRectEx(&rtTemp);	///clean

		///in case we reach the top or bottom
		if( pFTextEntry->iListSel == pFTextEntry->iListNum-1 )
		{	///top
			///down arrow
			GUI_FillPolygon( pPoint1, 3, ((rtTemp.x0+rtTemp.x1)/2)-9, rtTemp.y1-9 );
			///up arrow
			GUI_DrawPolygon( pPoint2, 3, ((rtTemp.x0+rtTemp.x1)/2)-9, rtTemp.y0+9 );
		}
		else
		if( pFTextEntry->iListSel == 0 )
		{	///bottom
			///down arrow
			GUI_DrawPolygon( pPoint1, 3, ((rtTemp.x0+rtTemp.x1)/2)-9, rtTemp.y1-9 );
			///up arrow
			GUI_FillPolygon( pPoint2, 3, ((rtTemp.x0+rtTemp.x1)/2)-9, rtTemp.y0+9 );
		}
		else
		{
			///down arrow
			GUI_FillPolygon( pPoint1, 3, ((rtTemp.x0+rtTemp.x1)/2)-9, rtTemp.y1-9 );
			///up arrow
			GUI_FillPolygon( pPoint2, 3, ((rtTemp.x0+rtTemp.x1)/2)-9, rtTemp.y0+9 );
		}

		///GUI_DispStringAt("Trial version,\n", 0, 0 );
		///GUI_DispStringAt("Disable after \n", 10, 10 );
		///GUI_DispStringAt("a few actions!\n", 20, 20 );		
		
		///GUI_DispStringAt("Trial version,\n", 30, 30 );
		///GUI_DispStringAt("Disable after \n", 30, 50 );
		///GUI_DispStringAt("a few actions!\n", 30, 70 );
		
		WM_DefaultProc(pMsg);
	}
}

static void spcbFloatTextEntryTextSelect(WM_MESSAGE* pMsg)
{
	FP_FTEXTENTRY_HEADER* pFTextEntry = NULL;
	int iSelet = -1;
		
	if( spFramePageValid( FRAMEPAGE_FLOATTEXT_ENTRY ) )
		return;
	
	pFTextEntry = pCurrFramePageFrameData;
	
	///check what you selected
	iSelet = LISTBOX_GetSel(pMsg->hWin);
	if( iSelet < pFTextEntry->iListNum )
	{
		pFTextEntry->iListSel = iSelet;
		WM_Paint( pFTextEntry->hWinInputFrame );
	}

}

void cbFloatTextEntryWindow(WM_MESSAGE* pMsg)
{
	SPPRINTF("cbFloatTextEntryWindow() ==> %d \n", pMsg->MsgId);
	
	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;
			
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( IsUP_press(Key) )	///up
		{
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
			if( pCurrFramePageOldCb ) pCurrFramePageOldCb( pMsg );
			spcbFloatTextEntryTextSelect(pMsg);
			return;
		}
		else
		if( IsDOWN_press(Key) )	///down
		{
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
			if( pCurrFramePageOldCb ) pCurrFramePageOldCb( pMsg );
			spcbFloatTextEntryTextSelect(pMsg);
			return;
		}
		else
		if( IsUP_hold(Key) )
		{	/// hold up key
			spGoAfterFramePage( &headPopupListWindow_DeviceModeFitness );
			return;
		}
		else		
		if( IsDOWN_hold(Key) )
		{	/// hold down key
			spGoAfterFramePage( &headPopupListWindow_Fitness );
			return;
		}
		else
		if( IsBACK_press(Key) )
		{	/// back key
			spGoAfterFramePage( ((FP_FTEXTENTRY_HEADER*)pCurrFramePageFrameData)->pUplevelFrame );
			return;
		}
		else		
		if( IsENTER_press(Key) )
		{	/// enter key
			FP_FTEXTENTRY_HEADER* pFTextEntry = NULL;
			GUI_RECT rtTemp;
			char sOut[] = "a  \0";
			
			
			pFTextEntry = pCurrFramePageFrameData;
			///get the charactor
			LISTBOX_GetItemText( pFTextEntry->hWinInputList, pFTextEntry->iListSel, sOut, 2 );
			///add to result
			sTextShow[iTextCnt] = sOut[0];
			///pFTextEntry->sResult[pFTextEntry->iResultCnt] = sOut[0];
			iTextCnt++;
			pFTextEntry->iResultCnt++;
			pFTextEntry->sResult = sTextShow;
			///move the cursor
			WM_GetClientRectEx( pFTextEntry->hWinInputFrame, &rtTemp );
			WM_MoveWindow( pFTextEntry->hWinInputFrame, rtTemp.x0+FTEXTFRAME_XSIZE, rtTemp.y0 );
			
			TEXT_SetText( pFTextEntry->hWinTextBoard, pFTextEntry->sResult );
			return;
		}
		else
		{
			return;
		}
	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		///spcbRoundWinExt( pMsg, GUI_BLUE, 0, 1, 1 );
		///cbFloatTextWindow(pMsg);
	}
	
	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );
	
	return;

}
#endif



void FloatTextEntryWindow( int iOption )
{
	int iTO = 0;
	///FRAMEWIN_Handle	hFrame = NULL;
	FP_FTEXTENTRY_HEADER* pFTextEntry = NULL;
	WM_CALLBACK* pOldCB = NULL;
	int iTmp = 0;
	const GUI_FONT* pFont = NULL;
	
	
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	spSetDefaultEffect();

	if( spFramePageValid( FRAMEPAGE_FLOATTEXT_ENTRY ) )
		return;
	
	pFTextEntry = pCurrFramePageFrameData;
	///create frame title
	pCurrFramePageHandle = FRAMEWIN_CreateEx( 
								0, 
								0, 
								LCD_GetXSize(), 
								LCD_GetYSize(), 
								WM_HWIN_NULL, 
								WM_CF_SHOW|WM_CF_STAYONTOP, 
								0, 
								0, 
								pFTextEntry->sTitle, 
								NULL );
	iTmp = FRAMEWIN_GetTitleHeight( pCurrFramePageHandle );

	///create windows for text frame	///GUI_GetYSizeOfFont(pFont)
	pFTextEntry->hWinInputFrame = WM_CreateWindow( 
								FTEXTFRAME_X, 
								FTEXTFRAME_Y+iTmp, 
								FTEXTFRAME_XSIZE, 
								FTEXTFRAME_YSIZE, 
								WM_CF_SHOW|WM_CF_STAYONTOP, 
								NULL, 
								0 );
	///pFTextEntry->hWinInputFrame = WM_CreateWindow( 0, 0+iTmp, LCD_GetXSize(), LCD_GetYSize()-iTmp, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pOldCB = WM_SetCallback( pFTextEntry->hWinInputFrame, &cbFloatTextWindow );
	
	///we don't want to see focus items in reverse
	LISTBOX_SetDefaultBkColor( LISTBOX_CI_SELFOCUS, GUI_WHITE );
	LISTBOX_SetDefaultTextColor( LISTBOX_CI_SELFOCUS, GUI_BLACK );
	
	///pFont = &GUI_Font16B_ASCII;
	pFont = &GUI_Font24B_ASCII;
	///create windows for input list
	pFTextEntry->hWinInputList = LISTBOX_CreateEx( 
								FTEXTENTRY_X, 
								(FTEXTFRAME_YSIZE-GUI_GetYSizeOfFont(pFont))/2, 
								FTEXTENTRY_XSIZE, 
								GUI_GetYSizeOfFont(pFont), 
								pFTextEntry->hWinInputFrame, 
								WM_CF_SHOW|WM_CF_STAYONTOP, 
								0, 
								0, 
								pFTextEntry->sListText );
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( pFTextEntry->hWinInputList, pCurrFramePageMainCb );
	///set font in list
	LISTBOX_SetFont( pFTextEntry->hWinInputList , pFont );

	pFTextEntry->hWinTextBoard = TEXT_CreateEx(
				FTEXTFRAME_X, 
				FTEXTFRAME_Y, 
				100, 
				20,
				pCurrFramePageHandle,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				0,
				30,
				pFTextEntry->sResult
			);

			
	
	WM_BringToTop( pCurrFramePageHandle );
	WM_BringToTop( pFTextEntry->hWinInputFrame );
	WM_BringToTop( pFTextEntry->hWinInputList );
	WM_SetFocus( pCurrFramePageHandle );
	WM_SetFocus( pFTextEntry->hWinInputFrame );
	WM_SetFocus( pFTextEntry->hWinInputList );


	WM_ExecIdle();
	iTO = spFramePageWait();

	WM_DeleteWindow( pFTextEntry->hWinInputList );
	WM_DeleteWindow( pFTextEntry->hWinInputFrame );
	WM_DeleteWindow( pCurrFramePageHandle );

	///we don't want to see focus items in reverse
	LISTBOX_SetDefaultBkColor( LISTBOX_CI_SELFOCUS, GUI_BLUE );
	LISTBOX_SetDefaultTextColor( LISTBOX_CI_SELFOCUS, GUI_WHITE );			
	
	///go timeout frame if we were timeout
	spFrameTimeoutHandling(iTO);
}









/* ********************************************************* */
#endif	/* RENOUI_PROJ */
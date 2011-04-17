
/*
	body of UI frame page  watch mode functions
*/

#ifdef RENOUI_PROJ
/* ********************************************************* */


#include "RenoGUI.h"
#include "RenoUItoolhelp.h"
#include "RenoUIframe.h"
#include "RenoUIcommon.h"

#include "RenoUICcomm.h"
#include "RenoUICwatchmode.h"




#if (GUI_WINSUPPORT)
void cbWatchWindow(WM_MESSAGE* pMsg)
{

	SPPRINTF("cbWatchWindow() ==> %d \n", pMsg->MsgId);

	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;
			
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( IsUP_hold(Key) )
		{	/// hold up key
			SPPRINTF( "UP key hold!!!!!!!\n" );
			spGoAfterFramePage( &headPopupListWindow_DeviceModeFitness );
			return;
		}
	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		///GUI_DispStringAt("Watch Mode...", 80, 30);
	}
	else
	if( pMsg->MsgId == WM_WATCH_DATA )
	{
		int iD;
		DE_WATCH_DATA*	pWatchdatas = NULL;
		_DE_WATCHMODE*		pWatch = NULL;
		FP_WATCH_HEADER* pWatchData = NULL;
		GUI_RECT rtTemp;
		
		pWatchdatas = (DE_WATCH_DATA*)(pMsg->Data.p);
		pWatch = &(pWatchdatas->watchdata[0]);
		pWatchData = pCurrFramePageFrameData;

		for( iD=0; iD<pWatchdatas->iTotal; iD++ )
		{
			if( (pWatch+iD)->iID == WATCHDATA_ID_SECOND )
			{
				SPPRINTF("WM_WATCH_DATA found match %d=>%d\n", iD, WATCHDATA_ID_SECOND);
				WM_GetWindowRectEx( pWatchData->hTextSecond, &rtTemp );
				spBlankRectEx( &rtTemp );
				TEXT_SetText( pWatchData->hTextSecond, (pWatch+iD)->sDataStr );
				//if( pCurrFramePageOldCb ) pCurrFramePageOldCb( pMsg );
			}
			else
			if( (pWatch+iD)->iID == WATCHDATA_ID_TIME )
			{
				SPPRINTF("WM_WATCH_DATA found match %d=>%d\n", iD, WATCHDATA_ID_TIME);
				WM_GetWindowRectEx( pWatchData->hTextTime, &rtTemp );
				spBlankRectEx( &rtTemp );
				TEXT_SetText( pWatchData->hTextTime, (pWatch+iD)->sDataStr );
				//if( pCurrFramePageOldCb ) pCurrFramePageOldCb( pMsg );
			}
		}
		
		return;
	}
	
	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );
	return;

}
#endif



void WatchWindow( int iOption )
{
	FP_WATCH_HEADER* pWatchData = NULL;
	///WM_HWIN hWin1 = 0;
	///WM_CALLBACK* pfnOldCB = NULL;
	
	
	///give me a blank page
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	if( spFramePageValid( FRAMEPAGE_WATCH ) )
		return;

	pWatchData = pCurrFramePageFrameData;
	///create a window
	pCurrFramePageHandle = WM_CreateWindow( 
				0, 
				0, 
				LCD_GetXSize(), 
				LCD_GetYSize(), 
				WM_CF_SHOW|WM_CF_STAYONTOP, 
				NULL, 
				0 );
	///hWin1 = WM_CreateWindow( 0, 0, 10, 10, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	
	///set callback
	///pfnOldCB = WM_SetCallback( hWin1, &cbPoweroffWindow );
	pCurrFramePageOldCb = WM_SetCallback( pCurrFramePageHandle, pCurrFramePageMainCb );
	///pCurrFramePageOldCb = WM_SetCallback( hWin1, pCurrFramePageMainCb );
	///ScrStat.pNowFramePage->pfnOldCB = WM_SetCallback( hWin1, pCurrFramePageMainCb );

	///pFont = &GUI_Font16B_ASCII;
	///pFont = &GUI_Font24B_ASCII;
	///pFont = &GUI_Font32B_ASCII;
	pWatchData->hTextDays = TEXT_CreateEx(
				3, 
				3, 
				120, 
				30,
				pCurrFramePageHandle,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				0,
				50,
				"Monday");
	
	TEXT_SetFont( pWatchData->hTextDays, &GUI_Font24B_ASCII );

	pWatchData->hTextDate = TEXT_CreateEx(
				3, 
				3+30, 
				120, 
				30,
				pCurrFramePageHandle,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				0,
				51,
				"2011-04-07");
	
	TEXT_SetFont( pWatchData->hTextDate, &GUI_Font24B_ASCII );

	pWatchData->hTextTime = TEXT_CreateEx(
				3+30, 
				3+30+30, 
				90, 
				40,
				pCurrFramePageHandle,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				0,
				52,
				"12:00");
	
	TEXT_SetFont( pWatchData->hTextTime, &GUI_Font32B_ASCII );

	pWatchData->hTextSecond = TEXT_CreateEx(
				3+120, 
				3+30+30, 
				30, 
				18,
				pCurrFramePageHandle,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				0,
				53,
				"00");
	
	TEXT_SetFont( pWatchData->hTextSecond, &GUI_Font16B_ASCII );

	pWatchData->hText24H = TEXT_CreateEx(
				3+120, 
				3+30+30+18, 
				30, 
				20,
				pCurrFramePageHandle,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				0,
				54,
				"24h");
	
	TEXT_SetFont( pWatchData->hText24H, &GUI_Font16B_ASCII );

	
	WM_BringToTop( pCurrFramePageHandle );
	WM_SetFocus( pCurrFramePageHandle );
///	WM_BringToTop( hWin1 );
///	WM_SetFocus( hWin1 );
	
	GUI_DispStringAt("Watch Mode...", 45, 80);
	
    WM_ExecIdle();
	spFramePageWait();
	
	WM_DeleteWindow( pCurrFramePageHandle );
///	WM_DeleteWindow( hWin1 );
}


void spUpdateWatchModeWindow( void )
{
	FP_WATCH_HEADER* pWatchData = NULL;
	int iLoop = 0;
		
	if( spFramePageValid( FRAMEPAGE_WATCH ) )
		return;
		
	pWatchData = pCurrFramePageFrameData;

	WM_Paint( pWatchData->hTextSecond );
	WM_Paint( pWatchData->hTextTime );

}






/* ********************************************************* */
#endif	/* RENOUI_PROJ */
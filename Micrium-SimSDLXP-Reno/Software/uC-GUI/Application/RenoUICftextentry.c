
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



#if (GUI_WINSUPPORT)
void cbFloatTextEntryWindow(WM_MESSAGE* pMsg)
{
	///SPPRINTF("cbFloatTextEntryWindow() ==> %d \n", pMsg->MsgId);
	
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
			spGoAfterFramePage( &headPopupListWindow_DeviceModeFitness );
		}
		else		
		if( IsDOWN_hold(Key) )
		{	/// hold down key
			spGoAfterFramePage( &headPopupListWindow_Fitness );
		}
		else
		if( IsBACK_press(Key) )
		{	/// back key
			;
		}
		else		
		if( IsENTER_press(Key) )
		{	/// enter key
			;
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


#define		FTEXTENTRY_X			20
#define		FTEXTENTRY_Y			20
#define		FTEXTENTRY_XSIZE		20
#define		FTEXTENTRY_YSIZE		20



void FloatTextEntryWindow( int iOption )
{

	int iTO = 0;
	FP_FTEXTENTRY_HEADER* pFTextEntry = NULL;
	int iTmp = 0;

	
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	spSetDefaultEffect();

	if( spFramePageValid( FRAMEPAGE_FLOATTEXT_ENTRY ) )
		return;
	
	pFTextEntry = pCurrFramePageFrameData;
	///create frame title
	pCurrFramePageHandle = FRAMEWIN_CreateEx(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, pFTextEntry->sTitle, NULL);
	iTmp = FRAMEWIN_GetTitleHeight( pCurrFramePageHandle );

	///create windows for text frame
	pFTextEntry->hWinTextFrame = WM_CreateWindow( 0, iTmp, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );

	///create windows for input frame
	pFTextEntry->hWinInputFrame = WM_CreateWindow( FTEXTENTRY_X, FTEXTENTRY_Y, FTEXTENTRY_XSIZE, FTEXTENTRY_YSIZE, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );

	///add callback
	pCurrFramePageOldCb = WM_SetCallback( pFTextEntry->hWinInputFrame, pCurrFramePageMainCb );

	WM_BringToTop( pCurrFramePageHandle );
	WM_BringToTop( pFTextEntry->hWinTextFrame );
	WM_BringToTop( pFTextEntry->hWinInputFrame );
	WM_SetFocus( pFTextEntry->hWinInputFrame );

	WM_ExecIdle();
	iTO = spFramePageWait();

	WM_DeleteWindow( pFTextEntry->hWinInputFrame );
	WM_DeleteWindow( pFTextEntry->hWinTextFrame );
	WM_DeleteWindow( pCurrFramePageHandle );
	
	///go timeout frame if we were timeout
	spFrameTimeoutHandling(iTO);
}









/* ********************************************************* */
#endif	/* RENOUI_PROJ */
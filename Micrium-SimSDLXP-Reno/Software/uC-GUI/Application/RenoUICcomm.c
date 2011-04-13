
/*
	body of UI frame page common functions
*/

#ifdef RENOUI_PROJ
/* ********************************************************* */


#include "RenoGUI.h"
#include "RenoUItoolhelp.h"
#include "RenoUIframe.h"
#include "RenoUIcommon.h"

#include "RenoUICcomm.h"





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
	
	///we don't want to see disabled items
	LISTBOX_SetDefaultBkColor( LISTBOX_CI_DISABLED, GUI_WHITE );
	LISTBOX_SetDefaultTextColor( LISTBOX_CI_DISABLED, GUI_WHITE );

	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE); 

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
			SPPRINTF("popup number up!!!!!!!!!\n");
		}
		else
		if( IsDOWN_press(Key) )	///down
		{
			///((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
			GUI_DrawPolygon( pPoint1, 3, ((rtTemp.x0+rtTemp.x1)/2)+4, ((rtTemp.y0+rtTemp.y1)/2)+4 );
			SPPRINTF("popup number down!!!!!!!!!\n");
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
			SPPRINTF("!!!!Error, there should popup list data here!! abort!!\n");
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


#if (GUI_WINSUPPORT)
void cbPoweroffWindow(WM_MESSAGE* pMsg)
{
	SPPRINTF("cbPoweroffWindow() ==> %d \n", pMsg->MsgId);

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


#if (GUI_WINSUPPORT)
static void cbPopupWindow(WM_MESSAGE* pMsg)
{
	SPPRINTF("cbPopupWindow() ==> %d \n", pMsg->MsgId);

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

void cbPopupWindowList(WM_MESSAGE* pMsg)
{
	///SPPRINTF("cbPopupWindowList() ==> %d \n", pMsg->MsgId);
	
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
				SPPRINTF("!!!!Error, there should popup list data here!! abort!!\n");
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
				SPPRINTF( "!!!!!Error!! LISTBOX_GetSel=%d\n", iSelet );
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
		SPPRINTF("!!!!Error, there should popup list data here!! abort!!\n");
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

///extern GUI_CONST_STORAGE GUI_BITMAP bmReno_Icon_20_GPS_Search;
#if (GUI_WINSUPPORT)
void cbPopupWindowNotify(WM_MESSAGE* pMsg)
{
	SPPRINTF("cbPopupWindowNotify() ==> %d \n", pMsg->MsgId);
	
	if( WM_KEY == pMsg->MsgId )
	{
		int Key = 0;
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;

		///hack the key value
		if( IsUP_press(Key) )	///up
		{
			spGoAfterFramePage( pBeforeFramePage );
		}
		else
		if( IsDOWN_press(Key) )	///down
		{
			spGoAfterFramePage( pBeforeFramePage );
		}
		else
		if( IsENTER_press(Key) )	///enter
		{
			spGoAfterFramePage( pBeforeFramePage );
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
	if( WM_PAINT == pMsg->MsgId )
	{
		FP_POPUPNOTIFY_HEADER* pPopNoty = NULL;
		GUI_RECT rtTmp;
		
		pPopNoty = pCurrFramePageFrameData;
		WM_GetClientRectEx( pCurrFramePageHandle, &rtTmp );
		spBlankRectEx( &rtTmp );

		GUI_SetColor(GUI_BLACK);
		GUI_SetBkColor(GUI_WHITE); 
		
		GUI_DrawBitmap( pPopNoty->pBitmap, (rtTmp.x1-rtTmp.x0)/2-10, 15 );
		
		///GUI_DispStringAt( pPopNoty->sTitle, (rtTmp.x1-rtTmp.x0)/2-, 30 );
		rtTmp.y0 = rtTmp.y0 + 40;
		GUI_SetFont( &GUI_Font16B_ASCII );
		GUI_DispStringInRect( pPopNoty->sTitle, &rtTmp, GUI_TA_HCENTER|GUI_TA_VCENTER );
	}
	else
	{
		if( pCurrFramePageOldCb )
			pCurrFramePageOldCb( pMsg );
	}	
}
#endif

///#define		EDGEOFFSET	20

void PopupWindowNotify( int iOption )
{
	WM_CALLBACK* pOldCB = NULL;
	FP_POPUPNOTIFY_HEADER* pPopNoty = NULL;
	int x, y, xsize, ysize;
	int iTO = 0;
	
	x = 0+EDGEOFFSET;
	y = 0+EDGEOFFSET;
	xsize = LCD_GetXSize()-(EDGEOFFSET*2);
	ysize = LCD_GetYSize()-(EDGEOFFSET*2);
	
	spSetDefaultEffect();
	
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();
	
	//need a blank space ???
	///spBlankRect( 0+EDGEOFFSET, 0+EDGEOFFSET, LCD_GetXSize()-(EDGEOFFSET*2), LCD_GetYSize()-(EDGEOFFSET*2) );
	
	if( 
		FRAMEPAGE_POPUP_NOTIFY != pCurrFramePageType ||
		NULL == pCurrFramePageFrameData
	)
	{
		SPPRINTF("!!!!Error, there should popup notify data here!! abort!!\n");
		return;
	}
	pPopNoty = pCurrFramePageFrameData;
	
	///create windows for popup outline
	pPopNoty->hWinFrame = WM_CreateWindow( x, y, xsize, ysize, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback
	pOldCB = WM_SetCallback( pPopNoty->hWinFrame, &cbPopupWindow );

	///create window for notify content
	pCurrFramePageHandle = WM_CreateWindow( x+13, y+13, xsize-26, ysize-26, WM_CF_SHOW|WM_CF_STAYONTOP, NULL, 0 );
	///add callback for main frame
	pCurrFramePageOldCb = WM_SetCallback( pCurrFramePageHandle, pCurrFramePageMainCb );
	
	WM_BringToTop( pPopNoty->hWinFrame );
	WM_SetFocus( pPopNoty->hWinFrame );
	WM_BringToTop( pCurrFramePageHandle );
	WM_SetFocus( pCurrFramePageHandle );
	
	WM_ExecIdle();
	iTO = spFramePageWait();
	
	WM_DeleteWindow( pPopNoty->hWinFrame );
	WM_DeleteWindow( pCurrFramePageHandle );
	
	///go timeout frame if we were timeout
	spFrameTimeoutHandling(iTO);
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
		SPPRINTF("!!!!Error, there should popup list data here!! abort!!\n");
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
		SPPRINTF( "!!!!!Error!! LISTBOX_GetSel=%d\n", iSelet );
		spGoAfterFramePage( pBeforeFramePage );
	}
}

void cbPopupWindowNumbers(WM_MESSAGE* pMsg)
{
	SPPRINTF("cbPopupWindowNumbers() ==> %d \n", pMsg->MsgId);
	
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

void cbSAPDPADSASNSPopupWindowNumbers(WM_MESSAGE* pMsg)
{
	SPPRINTF("cbSAPDPADSASNSPopupWindowNumbers() ==> %d \n", pMsg->MsgId);
	
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
		SPPRINTF("!!!!Error, there should popup list data here!! abort!!\n");
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
void cbNavigationWindow(WM_MESSAGE* pMsg)
{
	SPPRINTF("cbNavigationWindow() ==> %d \n", pMsg->MsgId);
	
	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;
			
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( IsUP_hold(Key) )
		{	/// hold up key
			SPPRINTF( "UP key hold!!!!!!!\n" );
			///set next framepage
			pAfterFramePage = &headPopupListWindow_DeviceModeFitness;
			///ready for next framepage
			pCurrFramePageNextReady = 1;
		}
		else		
		if( IsDOWN_hold(Key) )
		{	/// hold down key
			SPPRINTF( "DOWN key hold!!!!!!!\n" );
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
	
	///SPPRINTF("spListBoxOwnerDraw x=%d y=%d idx=%d\n", pDrawItemInfo->x0, pDrawItemInfo->x0, pDrawItemInfo->ItemIndex);
	switch (pDrawItemInfo->Cmd) {
		case WIDGET_ITEM_GET_XSIZE:
			///SPPRINTF("spListBoxOwnerDraw: WIDGET_ITEM_GET_XSIZE\n");
			///iRet = LISTBOX_OwnerDraw(pDrawItemInfo) + 100; /* Returns the default xsize+10 */
			///return iRet;
			break;
		case WIDGET_ITEM_GET_YSIZE:
			///SPPRINTF("spListBoxOwnerDraw: WIDGET_ITEM_GET_YSIZE\n");
			///iRet = LISTBOX_OwnerDraw(pDrawItemInfo) - 0; /* Returns the default ysize+10 */
			///return iRet;
			break;
		case WIDGET_ITEM_DRAW:
			/* Your code to be added to draw the LISTBOX item */
			///SPPRINTF("spListBoxOwnerDraw: WIDGET_ITEM_DRAW\n");
			iRet = LISTBOX_OwnerDraw(pDrawItemInfo);
			{
				FP_LISTMENU_HEADER* pListMenu = NULL;
	
				if( 
					IsNotCurrFPListMenuLike ||
					NULL == pCurrFramePageFrameData
				)	
				{
					SPPRINTF("!!!!Error, there should list menu data here!! abort!!\n");
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

/*
	check if the item (by Index) in a list is boolean option field, and handle the rest of items
*/
int spListMenuOptionFieldCheck( LISTBOX_Handle hList, int iSelet, int iInit )
{
	int iRet = 0;
	FP_LISTMENU_HEADER*	pListMenu = NULL;
	int iPara = 0;
	int iTmp = 0;
	
	if( 
		IsNotCurrFPListMenuLike ||
		NULL == pCurrFramePageFrameData
	)
	{
		SPPRINTF("!!!!Error, there should list menu data here!! abort!!\n");
		return iRet;
	}
		
	pListMenu = pCurrFramePageFrameData;

	iPara = pListMenu->pListParam[iSelet];
	
	///control the items regarding if there is boolean option item
	///usually, the boolean option item will set the disable flag of rest items
	if( 0 == iInit && 0 < (iPara & (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0) )
	{	///it's boolean item/field
		if( ((BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1) == (iPara & ((BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1)) )
		{	///enabled boolean
			///invert it
			pListMenu->pListParam[iSelet] = (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0;
		}
		else
		{	///disabled boolean
			///invert it
			pListMenu->pListParam[iSelet] = (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1;
		}
		///iPara = pListMenu->pListParam[iSelet];	///flush the variable
		///set the item disabled flag for new 
//		iPara = LISTBOX_GetNumItems( hList );	///till the end ?? no other boolean option field??
//		for( iTmp=iSelet+1; iTmp<iPara; iTmp++ )
//			LISTBOX_SetItemDisabled( hList, iTmp, (pListMenu->pListParam[iSelet] == ((BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1)?0:1 ) );
		
		LISTBOX_SetItemDisabled( hList, iSelet, 1 );	///trigger redraw
		LISTBOX_SetItemDisabled( hList, iSelet, 0 );	///trigger redraw
		iRet = 1;
	}
	
	if( 0 < (iPara & (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0) )
	{
		iPara = LISTBOX_GetNumItems( hList );	///till the end ?? no other boolean option field??
		for( iTmp=iSelet+1; iTmp<iPara; iTmp++ )
			LISTBOX_SetItemDisabled( hList, iTmp, (pListMenu->pListParam[iSelet] == ((BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1)?0:1 ) );
	}
	
	return iRet;
}

void cbListMenuWindow(WM_MESSAGE* pMsg)
{
	///SPPRINTF("cbListMenuWindow() ==> %d \n", pMsg->MsgId);
	
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
				SPPRINTF("!!!!Error, there should list menu data here!! abort!!\n");
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
				SPPRINTF("!!!!Error, there should list menu data here!! abort!!\n");
				return;
			}
			
			pListMenu = pCurrFramePageFrameData;
			///check what you selected
			iSelet = LISTBOX_GetSel(pMsg->hWin);
			
			///if the frame is all boolean option items...
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
			#if 1
				if( spListMenuOptionFieldCheck( pMsg->hWin, iSelet, 0 ) )
				{
					;
				}
			#else	
				int iPara = 0;
				int iTmp = 0;
				iPara = pListMenu->pListParam[iSelet];
				///control the items regarding if there is boolean option item
				///usually, the boolean option item will set the disable flag of rest items
				if( 0 < (iPara & (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0) )
				{	///it's boolean item/field
					if( ((BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1) == (iPara & ((BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1)) )
					{	///enabled boolean
						///invert it
						pListMenu->pListParam[iSelet] = (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0;
					}
					else
					{	///disabled boolean
						///invert it
						pListMenu->pListParam[iSelet] = (BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1;
					}
					///iPara = pListMenu->pListParam[iSelet];	///flush the variable
					///set the item disabled flag for new 
					iPara = LISTBOX_GetNumItems( pMsg->hWin );
					for( iTmp=iSelet+1; iTmp<iPara; iTmp++ )
						LISTBOX_SetItemDisabled( pMsg->hWin, iTmp, (pListMenu->pListParam[iSelet] == ((BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1)?0:1 ) );
					
					LISTBOX_SetItemDisabled( pMsg->hWin, iSelet, 1 );	///trigger redraw
					LISTBOX_SetItemDisabled( pMsg->hWin, iSelet, 0 );	///trigger redraw
				}
			#endif	
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
				SPPRINTF( "!!!!!LISTBOX_GetSel=%d\n", iSelet );
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
		SPPRINTF("!!!!Error, there should list menu data here!! abort!!\n");
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
	
	if( FRAMEPAGE_LISTMENU_BOOLOPTION != pCurrFramePageType )
	{	///in case we have boolean option field in the list
		int iCnt = 0;
		int iTmp = 0;
		iCnt = LISTBOX_GetNumItems( hList );
		
		for( iTmp=0; iTmp<iCnt; iTmp++ )
			spListMenuOptionFieldCheck( hList, iTmp, 1 );
	}
	
	
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
		SPPRINTF("!!!!Error, there should menu list data here!! abort!!\n");
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
						SPPRINTF("Get text entry ==> %s\n", sOut);
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
		SPPRINTF( "!!!!!Error!! LISTBOX_GetSel=%d\n", iSelet );
		spGoAfterFramePage( pBeforeFramePage );
	}
}

void cbTextEntryWindow(WM_MESSAGE* pMsg)
{
	///SPPRINTF("cbTextEntryWindow() ==> %d \n", pMsg->MsgId);
	
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
		SPPRINTF("!!!!Error, there should list menu data here!! abort!!\n");
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
void cbWatchWindow(WM_MESSAGE* pMsg)
{

	SPPRINTF("cbPoweroffWindow() ==> %d \n", pMsg->MsgId);

	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;
			
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( IsUP_hold(Key) )
		{	/// hold up key
			SPPRINTF( "UP key hold!!!!!!!\n" );
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



#ifdef 		ONE_GPS_SATELLITES

#define		SGSGS_TITLE_H			20
#define		SGSGS_LIST_H			20
#define		SGSGS_BAR_N				12
#define 	SGSGS_BAR_W				8
#define		SGSGS_BAR_IV			3
#define		SGSGS_BAR_H_SCALE		10

static void spSGSGSDraw( WM_MESSAGE* pMsg, GUI_RECT* prtDraw )
{
	int iTmp = 0;
	///GUI_RECT rtTemp;
	DE_GPS_SATELLITES_INFO* pgpsSatellites = NULL;
	_DE_GPS_SATELLITE* pgps = NULL;
	///TEXT_Handle hText[2];
	
	if( NULL != pMsg )
	{
		pgpsSatellites = (DE_GPS_SATELLITES_INFO*)(pMsg->Data.p);
		pgps = &(pgpsSatellites->satellites[0]);
		SPPRINTF( "WM_GPS_SATELLITE total=%d\n", pgpsSatellites->iTotal );

		///prtDraw->x0 = 0;
		prtDraw->y0 = prtDraw->y0+SGSGS_TITLE_H;
		///prtDraw->x1 = LCD_GetXSize();
		prtDraw->y1 = prtDraw->y1-SGSGS_LIST_H;
		
		///spBlankScreen();
		spBlankRect( prtDraw->x0, prtDraw->y0, prtDraw->x1-prtDraw->x0, prtDraw->y1-prtDraw->y0 );
		
		//draw the scale
		for( iTmp=0; iTmp<SGSGS_BAR_N; iTmp++ )
			GUI_DrawLine(prtDraw->x0,prtDraw->y1-(iTmp*SGSGS_BAR_H_SCALE),prtDraw->x1,prtDraw->y1-(iTmp*SGSGS_BAR_H_SCALE));
	
		///draw the bar of Satellites.
		for( iTmp=0; iTmp<SGSGS_BAR_N; iTmp++ )
			GUI_FillRect( 3+SGSGS_BAR_IV+(SGSGS_BAR_IV+SGSGS_BAR_W)*iTmp, (prtDraw->y1-(pgps+iTmp)->iSignal), 3+SGSGS_BAR_IV+(SGSGS_BAR_IV+SGSGS_BAR_W)*iTmp+SGSGS_BAR_W, prtDraw->y1 );
	}
	else
	{
		TEXT_Handle hText[2];
		hText[0] = TEXT_CreateEx(
				prtDraw->x0, 
				prtDraw->y0, 
				(prtDraw->x1-prtDraw->x0), 
				SGSGS_TITLE_H,
				NULL,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				0,
				20,
				"Accuracy:   +/-15ft"
			);
	
		hText[1] = TEXT_CreateEx(
				prtDraw->x0, 
				prtDraw->y1-SGSGS_LIST_H, 
				(prtDraw->x1-prtDraw->x0), 
				SGSGS_LIST_H,
				NULL,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				TEXT_CF_HCENTER|TEXT_CF_VCENTER,
				21,
				"01 02 03 04 05 06 07 08 09 10 11 12"
			);	
	
		TEXT_SetFont( hText[1], &GUI_Font8_ASCII );		
		//draw the scale
		for( iTmp=0; iTmp<SGSGS_BAR_N; iTmp++ )
			GUI_DrawLine(prtDraw->x0,prtDraw->y1-SGSGS_LIST_H-(iTmp*SGSGS_BAR_H_SCALE),prtDraw->x1,prtDraw->y1-SGSGS_LIST_H-(iTmp*SGSGS_BAR_H_SCALE));
	
		///draw the bar of Satellites.
		for( iTmp=0; iTmp<SGSGS_BAR_N; iTmp++ )
			GUI_FillRect( 3+SGSGS_BAR_IV+(SGSGS_BAR_IV+SGSGS_BAR_W)*iTmp, 60+iTmp, 3+SGSGS_BAR_IV+(SGSGS_BAR_IV+SGSGS_BAR_W)*iTmp+SGSGS_BAR_W, prtDraw->y1-SGSGS_LIST_H );
			
		WM_BringToTop( hText[0] );
		WM_BringToTop( hText[1] );			
	}	
}

#else	///ONE_GPS_SATELLITES

#define		SGSGS_TITLE_H			20
#define		SGSGS_LIST_H			20
#define		SGSGS_BAR_N				12
#define 	SGSGS_BAR_W				18
#define 	SGSGS_BAR_H_MAX			45
#define		SGSGS_BAR_IV			6
#define		SGSGS_BAR_H_SCALE		10

static void spSGSGSDraw( WM_MESSAGE* pMsg, GUI_RECT* prtDraw )
{
	int iTmp = 0;
	///GUI_RECT rtTemp;
	DE_GPS_SATELLITES_INFO* pgpsSatellites = NULL;
	_DE_GPS_SATELLITE* pgps = NULL;
	///TEXT_Handle hText[2];
	
	if( NULL != pMsg )
	{
		pgpsSatellites = (DE_GPS_SATELLITES_INFO*)(pMsg->Data.p);
		pgps = &(pgpsSatellites->satellites[0]);
		SPPRINTF( "WM_GPS_SATELLITE total=%d\n", pgpsSatellites->iTotal );

		///prtDraw->x0 = 0;
		///prtDraw->y0 = prtDraw->y0+SGSGS_TITLE_H;
		///prtDraw->x1 = LCD_GetXSize();
		///prtDraw->y1 = prtDraw->y1-SGSGS_LIST_H;
		
		///spBlankScreen();
		spBlankRect( prtDraw->x0, prtDraw->y0+SGSGS_TITLE_H, prtDraw->x1-prtDraw->x0, SGSGS_BAR_H_MAX );
		spBlankRect( prtDraw->x0, prtDraw->y1-SGSGS_LIST_H-SGSGS_BAR_H_MAX, prtDraw->x1-prtDraw->x0, SGSGS_BAR_H_MAX );
		
		//draw the scale
		for( iTmp=0; iTmp<5; iTmp++ )
		{
			GUI_DrawLine(
					prtDraw->x0,
					prtDraw->y0+SGSGS_TITLE_H+SGSGS_BAR_H_MAX-(iTmp*SGSGS_BAR_H_SCALE),
					prtDraw->x1,
					prtDraw->y0+SGSGS_TITLE_H+SGSGS_BAR_H_MAX-(iTmp*SGSGS_BAR_H_SCALE)
				);
			GUI_DrawLine(
					prtDraw->x0,
					prtDraw->y1-SGSGS_LIST_H-(iTmp*SGSGS_BAR_H_SCALE),
					prtDraw->x1,
					prtDraw->y1-SGSGS_LIST_H-(iTmp*SGSGS_BAR_H_SCALE)
				);
		}
		
		///draw the bar of Satellites.
		for( iTmp=0; iTmp<(SGSGS_BAR_N/2); iTmp++ )
		{	
			///up
			GUI_FillRect( (SGSGS_BAR_IV/2)+((SGSGS_BAR_W+SGSGS_BAR_IV)*iTmp), prtDraw->y0+SGSGS_TITLE_H+SGSGS_BAR_H_MAX-((pgps+iTmp)->iSignal), (SGSGS_BAR_IV/2)+((SGSGS_BAR_W+SGSGS_BAR_IV)*iTmp)+SGSGS_BAR_W, prtDraw->y0+SGSGS_TITLE_H+SGSGS_BAR_H_MAX );
			
			///down
			GUI_FillRect( (SGSGS_BAR_IV/2)+((SGSGS_BAR_W+SGSGS_BAR_IV)*iTmp), prtDraw->y1-SGSGS_LIST_H-((pgps+iTmp+iTmp)->iSignal), (SGSGS_BAR_IV/2)+((SGSGS_BAR_W+SGSGS_BAR_IV)*iTmp)+SGSGS_BAR_W, prtDraw->y1-SGSGS_LIST_H );
		}	}
	else
	{
		TEXT_Handle hText[3];
		hText[0] = TEXT_CreateEx(
				prtDraw->x0, 
				prtDraw->y0, 
				(prtDraw->x1-prtDraw->x0), 
				SGSGS_TITLE_H,
				NULL,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				0,
				20,
				"Accuracy:   +/-15ft"
			);

		hText[1] = TEXT_CreateEx(
				prtDraw->x0, 
				prtDraw->y0+SGSGS_TITLE_H+SGSGS_BAR_H_MAX, 
				(prtDraw->x1-prtDraw->x0), 
				SGSGS_LIST_H,
				NULL,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				TEXT_CF_HCENTER|TEXT_CF_VCENTER,
				21,
				"01 02 03 04 05 06"
			);
			
		hText[2] = TEXT_CreateEx(
				prtDraw->x0, 
				prtDraw->y1-SGSGS_LIST_H, 
				(prtDraw->x1-prtDraw->x0), 
				SGSGS_LIST_H,
				NULL,
				WM_CF_SHOW|WM_CF_STAYONTOP,
				TEXT_CF_HCENTER|TEXT_CF_VCENTER,
				22,
				"07 08 09 10 11 12"
			);	
	
		///TEXT_SetFont( hText[1], &GUI_Font8_ASCII );
		///TEXT_SetFont( hText[2], &GUI_Font8_ASCII );
		
		
		//draw the scale
		for( iTmp=0; iTmp<5; iTmp++ )
		{
			GUI_DrawLine(
					prtDraw->x0,
					prtDraw->y0+SGSGS_TITLE_H+SGSGS_BAR_H_MAX-(iTmp*SGSGS_BAR_H_SCALE),
					prtDraw->x1,
					prtDraw->y0+SGSGS_TITLE_H+SGSGS_BAR_H_MAX-(iTmp*SGSGS_BAR_H_SCALE)
				);
			GUI_DrawLine(
					prtDraw->x0,
					prtDraw->y1-SGSGS_LIST_H-(iTmp*SGSGS_BAR_H_SCALE),
					prtDraw->x1,
					prtDraw->y1-SGSGS_LIST_H-(iTmp*SGSGS_BAR_H_SCALE)
				);
		}
		
		///draw the bar of Satellites.
		for( iTmp=0; iTmp<(SGSGS_BAR_N/2); iTmp++ )
		{	
			///up
			///SGSGS_BAR_IV
			GUI_FillRect( (SGSGS_BAR_IV/2)+((SGSGS_BAR_W+SGSGS_BAR_IV)*iTmp), prtDraw->y0+SGSGS_TITLE_H+SGSGS_BAR_H_MAX-(20+iTmp), (SGSGS_BAR_IV/2)+((SGSGS_BAR_W+SGSGS_BAR_IV)*iTmp)+SGSGS_BAR_W, prtDraw->y0+SGSGS_TITLE_H+SGSGS_BAR_H_MAX );
			
			///down
			GUI_FillRect( (SGSGS_BAR_IV/2)+((SGSGS_BAR_W+SGSGS_BAR_IV)*iTmp), prtDraw->y1-SGSGS_LIST_H-(20+iTmp), (SGSGS_BAR_IV/2)+((SGSGS_BAR_W+SGSGS_BAR_IV)*iTmp)+SGSGS_BAR_W, prtDraw->y1-SGSGS_LIST_H );
		}
/*		
		//draw the scale
		for( iTmp=0; iTmp<SGSGS_BAR_N; iTmp++ )
			GUI_DrawLine(prtDraw->x0,prtDraw->y1-SGSGS_LIST_H-(iTmp*SGSGS_BAR_H_SCALE),prtDraw->x1,prtDraw->y1-SGSGS_LIST_H-(iTmp*SGSGS_BAR_H_SCALE));
	
		///draw the bar of Satellites.
		for( iTmp=0; iTmp<SGSGS_BAR_N; iTmp++ )
			GUI_FillRect( 3+SGSGS_BAR_IV+(SGSGS_BAR_IV+SGSGS_BAR_W)*iTmp, 60+iTmp, 3+SGSGS_BAR_IV+(SGSGS_BAR_IV+SGSGS_BAR_W)*iTmp+SGSGS_BAR_W, prtDraw->y1-SGSGS_LIST_H );
*/		
		WM_BringToTop( hText[0] );
		WM_BringToTop( hText[1] );		
		WM_BringToTop( hText[2] );				
	}	
}

#endif










#if (GUI_WINSUPPORT)
void cbSGSGSWindow(WM_MESSAGE* pMsg)
{
	SPPRINTF("cbSGSGSWindow() ==> %d \n", pMsg->MsgId);
	
	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;
		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( IsBACK_press(Key) )	///back
		{
			spGoAfterFramePage( pBeforeFramePage );
		}
		else
		if( IsDOWN_press(Key) )	///down
		{	///test
			spGoAfterFramePage( &headPopupNotifyGPSOffWindow );
		}
		else
		if( IsUP_press(Key) )	///up
		{	///test
			spGoAfterFramePage( &headPopupNotifyGPSLockWindow );
		}

	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		;
	}
	else
	if( pMsg->MsgId == WM_GPS_SATELLITE )	///data input
	{
		GUI_RECT rtTemp;
		
		rtTemp.x0 = 0;
		rtTemp.y0 = 0+18;
		rtTemp.x1 = LCD_GetXSize();
		rtTemp.y1 = LCD_GetYSize();
		
		spSGSGSDraw( pMsg, &rtTemp);
	}
	
	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );
	return;

}
#endif


void SGSGSWindow( int iOption )
{
	FRAMEWIN_Handle	hFrame;
	int iTmp = 0;
	GUI_RECT rtTemp;
	WM_HWIN hWin = NULL;
	///TEXT_Handle hText[2];
/*
	rtTemp.x0 = 10;
	rtTemp.y0 = 40;
	rtTemp.x1 = 10+20;
	rtTemp.y1 = LCD_GetYSize();
*/	
	spSetDefaultEffect();
	
	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	///create frame title
	///hFrame = FRAMEWIN_CreateEx(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, "GPS Satellites", NULL);
	hFrame = FRAMEWIN_CreateEx(0, 0, LCD_GetXSize(), 18, WM_HWIN_NULL, WM_CF_SHOW|WM_CF_STAYONTOP, 0, 0, "GPS Satellites", NULL);
	iTmp = FRAMEWIN_GetTitleHeight( hFrame );
	pCurrFramePageHandle = hFrame;	///store the handle of main callback window
	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hFrame, pCurrFramePageMainCb );
	
	///get the client window size
	///WM_GetClientRectEx( hFrame, &rtTemp );

	rtTemp.x0 = 0;
	rtTemp.y0 = 0+18;
	rtTemp.x1 = LCD_GetXSize();
	rtTemp.y1 = LCD_GetYSize();

	spSGSGSDraw( NULL, &rtTemp );

	WM_BringToTop( hFrame );
	WM_SetFocus( hFrame );
///	WM_BringToTop( hText[1] );
///	WM_BringToTop( hText[2] );
	
	WM_ExecIdle();
	spFramePageWait();

	WM_DeleteWindow( hWin );
	WM_DeleteWindow( hFrame );
	
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






/* ********************************************************* */
#endif	/* RENOUI_PROJ */
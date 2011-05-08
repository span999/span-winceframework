
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
#include "RenoUIClistmenu.h"





static GUI_POINT pPoint1[] = {
							{ 0+0, 0+2},
							{ 0+0, 8+2},
							{ 8+0, 8+2},
							{ 8+0, 0+2},
						};
static GUI_POINT pPoint2[] = {
							{ 0+0+2, 0+2+2},
							{ 0+0+5, 0+2+7},
							{ 0+0+12, 0+2},
							{ 0+0+5, 0+2+5},
						};


static GUI_POINT pScrollUp[] = {
							{ 0, 8},
							{ 6, 0},
							{ 12, 8},
						};
static GUI_POINT pScrollDown[] = {
							{ 0, 0},
							{ 12, 0},
							{ 6, 8},
						};



#ifdef 	USE_LIST_SCROLL_ICON
static WM_HWIN 			hListScrollMain = NULL;
static WM_CALLBACK* 	pcbListMainScroll = NULL;
static int				iFirstIdx	= 0;
static int				iLastIdx	= 0;
static int				iIsTop		= 0;
static int				iIsEnd		= 0;


static cbListMainScroll(WM_MESSAGE* pMsg)
{
	int iTmp = 0;
	GUI_RECT rtTemp;
	SPPRINTF( "-->cbListMainScroll=%d\n", 99 );
	if( pcbListMainScroll ) pcbListMainScroll( pMsg );

	WM_GetClientRectEx( pMsg->hWin, &rtTemp );
	iTmp = FRAMEWIN_GetTitleHeight( pMsg->hWin );

	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE);
	if( iIsTop )
		GUI_DrawPolygon( pScrollUp, 3, (LCD_GetXSize()-12)/2, rtTemp.y0+1 );
	else
		GUI_FillPolygon( pScrollUp, 3, (LCD_GetXSize()-12)/2, rtTemp.y0+1 );

	if( iIsEnd )
		GUI_DrawPolygon( pScrollDown, 3, (LCD_GetXSize()-12)/2, rtTemp.y1-10 );
	else
		GUI_FillPolygon( pScrollDown, 3, (LCD_GetXSize()-12)/2, rtTemp.y1-10 );

	iIsTop = 0;
	iIsEnd = 0;
}
#endif


#ifdef 	USE_LIST_SCROLL_BTN
static BUTTON_Handle	hUpScroll = NULL;
static BUTTON_Handle	hDownScroll = NULL;
static int				iFirstIdx	= 0;
static int				iLastIdx	= 0;



static void spListBoxScrollBtnCtl( int iIndex )
{
	static int moreUp = 0;
	static int moreDn = 0;
	
	SPPRINTF("spListBoxScrollBtnCtl idx=%d\n", iIndex);

	if( iLastIdx > LISTMENU_ITEMS_MAX-1 )	///over page
	{
		if( 0 == moreUp && 0 == moreDn )
		{	///first time
			moreUp = 0;
			moreDn = 1;
		}

		if( iFirstIdx == iIndex )	///handling up max
		{
			if( moreUp )
				moreUp = 0;
		}
		else
		if( iLastIdx == iIndex )	///handling down max
		{
			if( moreDn )
				moreDn = 0;
		}
		else
		{
			if( iIndex >= LISTMENU_ITEMS_MAX )
				moreUp = 1;
			if( iIndex <= iLastIdx-LISTMENU_ITEMS_MAX )
				moreDn = 1;
		}

	}
	else	///only one page
	{
		moreUp = 0;
		moreDn = 0;
	}

	if( moreUp )
		BUTTON_SetText( hUpScroll, "more UP" );
	else
		BUTTON_SetText( hUpScroll, "UP" );
	
	if( moreDn )
		BUTTON_SetText( hDownScroll, "more DOWN" );
	else
		BUTTON_SetText( hDownScroll, "DOWN" );
}
#endif



#if (GUI_WINSUPPORT)
static int spListBoxOwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
	int iRet = 0;

#ifdef 	USE_LIST_SCROLL_ICON
	if( WIDGET_ITEM_DRAW == pDrawItemInfo->Cmd )
	{
		if( iFirstIdx == pDrawItemInfo->ItemIndex )
			iIsTop = 1;
		if( iLastIdx == pDrawItemInfo->ItemIndex )
			iIsEnd = 1;
		///WM_Paint( hListScrollMain );
	}
#endif
#ifdef 	USE_LIST_SCROLL_BTN
	if( WIDGET_ITEM_DRAW == pDrawItemInfo->Cmd )
	{
		spListBoxScrollBtnCtl( pDrawItemInfo->ItemIndex );
	}
#endif


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
				
					GUI_POINT pPoint3[] = {
									{ 0, 0+1},
									{ 0, 12+1},
									{ 8, 6+1},
								};

					if( FRAMEPAGE_LISTMENU == pListMenu->pListFrame[pDrawItemInfo->ItemIndex]->frametype )
					{
						///GUI_DispStringAt("->", pDrawItemInfo->x0 + 130, pDrawItemInfo->y0);
						GUI_FillPolygon( pPoint3, 3, pDrawItemInfo->x0 + 130, pDrawItemInfo->y0 );
					}
					else
					if( FRAMEPAGE_LISTMENU_BOOLOPTION == pListMenu->pListFrame[pDrawItemInfo->ItemIndex]->frametype )
					{
						int iLoop = 0;
						FP_LISTMENU_HEADER* pNextList = NULL;

						///GUI_DispStringAt("->", pDrawItemInfo->x0 + 130, pDrawItemInfo->y0);
						GUI_FillPolygon( pPoint3, 3, pDrawItemInfo->x0 + 130, pDrawItemInfo->y0 );


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
									///pNextList->sListName[iLoop],
									///pNextList->sListShortName[iLoop],
									(NULL==pNextList->sListShortName)?(pNextList->sListName[iLoop]):(pNextList->sListShortName[iLoop]),
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

#ifdef 	USE_LIST_SCROLL_ICON
static void spcbScrollIconDraw(WM_MESSAGE* pMsg, GUI_COLOR color )
{
	if( pMsg->MsgId == WM_PAINT )
	{
		GUI_DrawPolygon( pScrollUp, 3, (LCD_GetXSize()+12)/2, -10 );
	}
}
#endif

void cbListMenuWindow(WM_MESSAGE* pMsg)
{
	///SPPRINTF("cbListMenuWindow() ==> %d \n", pMsg->MsgId);

	///hack the WM msg here
	if( pMsg->MsgId == WM_KEY )
	{
		int Key = 0;

		Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if( IsUP_press(Key) )	///up
		{
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_UP;
			if( pCurrFramePageOldCb ) pCurrFramePageOldCb( pMsg );
			return;
		}
		else
		if( IsDOWN_press(Key) )	///down
		{
			((WM_KEY_INFO*)(pMsg->Data.p))->Key = GUI_KEY_DOWN;
			if( pCurrFramePageOldCb ) pCurrFramePageOldCb( pMsg );
			return;
		}
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
			///set next framepage
			FP_LISTMENU_HEADER*	pListMenu = NULL;
			FRAMEPAGE_HEADER*	pNextFrame = NULL;
			int iSelet = -1;

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
					spGoAfterFramePage( &headDataModeWindow );
				else
					spGoAfterFramePage( pBeforeFramePage );
			}
			else
				spGoAfterFramePage( pListMenu->pUplevelFrame );
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
					spGoAfterFramePage( pNextFrame );
				}
			}
			else
			{
				SPPRINTF( "!!!!!LISTBOX_GetSel=%d\n", iSelet );
				spGoAfterFramePage( pBeforeFramePage );
			}
		}
	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		if( pCurrFramePageOldCb ) pCurrFramePageOldCb( pMsg );
	#ifdef 	USE_LIST_SCROLL_ICON
		spcbScrollIconDraw( pMsg, GUI_BLUE );
	#endif
		return;
	}

	if( pCurrFramePageOldCb )
		pCurrFramePageOldCb( pMsg );

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
	///FRAMEWIN_SetDefaultFont( &GUI_FontArial17_11pt );
	FRAMEWIN_SetDefaultFont( &GUI_FontArial22_14pt );
	iTmp = FRAMEWIN_GetTitleHeight( hFrame );
	///FRAMEWIN_SetClientColor( hFrame, GUI_RED );
	GUI_DrawHLine( iTmp+1, 0,LCD_GetXSize() );
	GUI_DrawHLine( iTmp+2, 0,LCD_GetXSize() );
	iTmp = iTmp + 3;	// add Title Line
	
#ifdef 	USE_LIST_SCROLL_ICON
	if( SCROLL_ICON_HEIGHT )
	{
	#if 0	///access with message
		WM_MESSAGE  MsgCmd;

		MsgCmd.MsgId = WM_GET_CLIENT_WINDOW;
		MsgCmd.hWin = hFrame;
		///get handle of client window
		WM_SendMessage( hFrame, &MsgCmd );
		hListScrollMain = MsgCmd.Data.v;
	#else
		hListScrollMain = WM_GetClientWindow( hFrame );
	#endif
		///set callback to client window
		pcbListMainScroll = WM_SetCallback( hListScrollMain, cbListMainScroll );

		iFirstIdx	= 0;
		iLastIdx	= pListMenu->iListNum - 1;
	}
#endif

#ifdef 	USE_LIST_SCROLL_BTN
	if( SCROLL_BTN_HEIGHT )
	{
		hUpScroll = BUTTON_CreateEx( 
					0,
					0+iTmp,
					SCROLL_BTN_WIDTH,
					SCROLL_BTN_HEIGHT,
					hFrame,
					WM_CF_SHOW,
					0,
					88);

		hDownScroll = BUTTON_CreateEx( 
					0,
					LCD_GetYSize()-SCROLL_BTN_HEIGHT-2,
					SCROLL_BTN_WIDTH,
					SCROLL_BTN_HEIGHT,
					hFrame,
					WM_CF_SHOW,
					0,
					89);
					
		BUTTON_SetText( hUpScroll, "UP" );
		BUTTON_SetText( hDownScroll, "DOWN" );
		iFirstIdx	= 0;
		iLastIdx	= pListMenu->iListNum - 1;		
	}
#endif

	///create list menu
	hList = LISTBOX_CreateEx(
					0,
					iTmp+SCROLL_ICON_HEIGHT+SCROLL_BTN_HEIGHT,
					LCD_GetXSize(),
					LCD_GetYSize()-iTmp-SCROLL_ICON_HEIGHT-SCROLL_ICON_HEIGHT-SCROLL_BTN_HEIGHT-SCROLL_BTN_HEIGHT,
					(WM_HWIN)hFrame, 
					WM_CF_SHOW,
					0,
					0,
					pListMenu->sListName);

	///add callback
	pCurrFramePageOldCb = WM_SetCallback( hList, pCurrFramePageMainCb );

	LISTBOX_SetOwnerDraw( hList, spListBoxOwnerDraw );
	///LISTBOX_InvalidateItem( hList, 2 );
	LISTBOX_SetFont( hList, &GUI_FontArial16_10pt );
	///LISTBOX_SetFont( hList, &GUI_FontArial18_12pt );
	
	if( pListMenu->uListItemSpacing > 0 )
		LISTBOX_SetItemSpacing( hList, pListMenu->uListItemSpacing );

	if( FRAMEPAGE_LISTMENU_BOOLOPTION != pCurrFramePageType )
	{	///in case we have boolean option field in the list
		int iCnt = 0;
		///int iTmp = 0;
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
void cbPopupWindow(WM_MESSAGE* pMsg)
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

	if( spFramePageValid( FRAMEPAGE_POPUP_NUMBERS ) )
		return;

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





/* ********************************************************* */
#endif	/* RENOUI_PROJ */

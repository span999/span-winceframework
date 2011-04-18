
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
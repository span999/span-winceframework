
/*
	body of UI frame page data mode functions
*/

#ifdef RENOUI_PROJ
/* ********************************************************* */


#include "RenoGUI.h"
#include "RenoUItoolhelp.h"
#include "RenoUIframe.h"
#include "RenoUIcommon.h"

#include "RenoUICcomm.h"
#include "RenoUICdatamode.h"




static int spGetDataFrameX( int iFrameOlder, int iFrameTotal )
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

static int spGetDataFrameY( int iFrameOlder, int iFrameTotal )
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

static int spGetDataFrameXsize( int iFrameOlder, int iFrameTotal )
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

static int spGetDataFrameYsize( int iFrameOlder, int iFrameTotal )
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

static void spDrawDataModeContent( int* piTotal, int* piIndex, FP_DATASETS_HEADER* pDataModeData, GUI_RECT* prtTemp )
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


static void spcbDataModeVal( WM_MESSAGE* pMsg, GUI_COLOR color )
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
		SPPRINTF("!!!!Error, there should activity data mode data here!! abort!!\n");
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



#if (GUI_WINSUPPORT)
void cbDataModeWindow(WM_MESSAGE* pMsg)
{
	int iTmp = 0;
	
	WM_GetUserData( pMsg->hWin, &iTmp, sizeof(int) );
	SPPRINTF("cbDataModeWindow() ==> %d user=%d\n", pMsg->MsgId, iTmp);

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
		SPPRINTF("!!!!Error, there should activity data mode data here!! abort!!\n");
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





/* ********************************************************* */
#endif	/* RENOUI_PROJ */
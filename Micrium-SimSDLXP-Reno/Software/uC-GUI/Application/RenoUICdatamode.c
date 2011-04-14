
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
	int 			iWidth = 0;

	
	if( 1 == *piTotal || 2 == *piTotal || 3 == *piTotal )
		iWidth = 100;
	else
	{
		if( 4 == *piTotal )
		{
			if( 1 == *piIndex || 2 == *piIndex )
				iWidth = 100;
			else
				iWidth = 60;
		}
		if( 5 == *piTotal )
		{
			if( 1 == *piIndex )
				iWidth = 100;
			else
				iWidth = 60;
		}
		else
			iWidth = 60;
	}
	
	if( NULL == pDataModeData->phText[*piIndex] )
	{
		pDataModeData->phText[*piIndex] = TEXT_CreateEx(
												prtTemp->x0+13, 
												(prtTemp->y0+prtTemp->y1)/2-12, 
												iWidth, ///60, 
												35,
												pDataModeData->phWin[*piIndex],
												WM_CF_SHOW|WM_CF_STAYONTOP,
												0,
												20+*piIndex,
												"Text"
											);
	}

			
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
			//pFont = GUI_GetFont();
			//GUI_SetFont( &GUI_Font32B_ASCII );
			//GUI_DispStringAt( pDataModeData->pDataSets[*piIndex].sDataValue, prtTemp->x0+13, (prtTemp->y0+prtTemp->y1)/2-12 );
			//GUI_SetFont( pFont );
			
			TEXT_SetFont( pDataModeData->phText[*piIndex], &GUI_Font32B_ASCII );
			TEXT_SetText( pDataModeData->phText[*piIndex], pDataModeData->pDataSets[*piIndex].sDataValue );
			
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
			///GUI_DispStringAt( pDataModeData->pDataSets[*piIndex].sDataValue, prtTemp->x0+13, (prtTemp->y0+prtTemp->y1)/2-3 );

			///TEXT_SetFont( pDataModeData->phText[*piIndex], &GUI_Font32B_ASCII );
			TEXT_SetText( pDataModeData->phText[*piIndex], pDataModeData->pDataSets[*piIndex].sDataValue );
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
	
	
	if( !pMsg || ((WM_PAINT != pMsg->MsgId) && (WM_ACTIVITY_DATA != pMsg->MsgId)) )
		return;
		
	if( spFramePageValid( FRAMEPAGE_DATAMODE ) )
		return;

	pDataModeData = pCurrFramePageFrameData;
	
	WM_GetClientRectEx( pMsg->hWin, &rtTemp );
	WM_GetUserData( pMsg->hWin, &iIndex, sizeof(int) );
	iTotal = pDataModeData->iDataSetNum;

	///override data if needed
	if( WM_ACTIVITY_DATA == pMsg->MsgId )
	{
		int iD, iS;
		DE_ACTIVITY_DATA*	pActdatas = NULL;
		_DE_ACTIVITY*		pAct = NULL;
		
		pActdatas = (DE_ACTIVITY_DATA*)(pMsg->Data.p);
		pAct = &(pActdatas->activity[0]);

		for( iD=0; iD<pActdatas->iTotal; iD++ )
		{
			for( iS=0; iS<iTotal; iS++ )
			{
				if( (pAct+iD)->iID == pDataModeData->pDataSets[iS].iActivityType )
				{
					SPPRINTF("spcbDataModeVal() found match %d=%d\n", iD, iS);
					///memcpy( (pAct+iD)->sDataStr, pDataModeData->pDataSets[iS].sDataValue, 4 );
					///pDataModeData->pDataSets[iS].sDataValue = "1.11";
					pDataModeData->pDataSets[iS].sDataValue = (pAct+iD)->sDataStr;
				}
			}
		}
		
	}
	
	///changes color if needed
	clTemp[0] = GUI_GetColor();
	clTemp[1] = GUI_GetBkColor();
	
	GUI_SetColor( color );
	GUI_SetBkColor( GUI_WHITE );

	spDrawDataModeContent( &iTotal, &iIndex, pDataModeData, &rtTemp );

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
	///SPPRINTF("cbDataModeWindow() ==> %d user=%d\n", pMsg->MsgId, iTmp);

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
	}
	else
	if( pMsg->MsgId == WM_PAINT )
	{
		///draw outline
		spcbRoundWinExt( pMsg, GUI_BLUE, 0, 1, 1 );
		///draw content
		spcbDataModeVal( pMsg, GUI_BLUE );
	}
	else
	if( pMsg->MsgId == WM_DELETE )
	{
		FP_DATASETS_HEADER* pDataModeData = NULL;
		int 				iLoop = 0;
		
		if( spFramePageValid( FRAMEPAGE_DATAMODE ) )
			return;
	
		pDataModeData = pCurrFramePageFrameData;
		
		for( iLoop=0; iLoop<pDataModeData->iDataSetNum; iLoop++)
			pDataModeData->phText[iLoop] = NULL;
		
		SPPRINTF("cbDataModeWindow() ===> %d user=%d\n", pMsg->MsgId, iTmp);
	}
	else
	if( pMsg->MsgId == WM_ACTIVITY_DATA )	///data input
	{
		///draw content
		spcbDataModeVal( pMsg, GUI_BLUE );
		SPPRINTF("cbDataModeWindow() ==> %d user=%d\n", pMsg->MsgId, iTmp);
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


	if( pCurrFramePageClearFirst > 0 )
		spBlankScreen();

	if( spFramePageValid( FRAMEPAGE_DATAMODE ) )
		return;
	
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
		///each data set
		WM_SetUserData( hDataWin[iLoop], &(pDataModeData->pDataSets[iLoop].iIndex), sizeof(int) );
		
		///save window handle
		pDataModeData->phWin[iLoop] = hDataWin[iLoop];
	}

	pCurrFramePageHandle = hDataWin[0];
	WM_BringToTop( hDataWin[0] );
	WM_SetFocus( hDataWin[0] );
	
	WM_ExecIdle();
	iTO = spFramePageWait();

	for( iLoop=0; iLoop<pDataModeData->iDataSetNum; iLoop++ )
		WM_DeleteWindow( hDataWin[iLoop] );
	
	///go timeout frame if we were timeout
	spFrameTimeoutHandling( iTO );
}


void spUpdateDataModeWindow( void )
{
	FP_DATASETS_HEADER* pDataModeData = NULL;
	int iLoop = 0;
		
	if( spFramePageValid( FRAMEPAGE_DATAMODE ) )
		return;
		
	pDataModeData = pCurrFramePageFrameData;
		
	for( iLoop=0; iLoop<pDataModeData->iDataSetNum; iLoop++ )
		WM_Paint( pDataModeData->phWin[iLoop] );
}










/* ********************************************************* */
#endif	/* RENOUI_PROJ */
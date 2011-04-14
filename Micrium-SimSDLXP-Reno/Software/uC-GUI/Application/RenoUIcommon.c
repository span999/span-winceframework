
/*
	body of UI common help functions
*/

#ifdef RENOUI_PROJ
/* ********************************************************* */



#include "RenoGUI.h"
#include "RenoUItoolhelp.h"
#include "RenoUIframe.h"


/*
	screen status
*/
SCREEN_STATUS ScrStat;
SCREEN_STATUS *pScrStat = &ScrStat;


/*
	common part
*/

void spClearScreen( void )
{
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_GRAY);
	GUI_Clear();
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



void spBlankRectEx( GUI_RECT* prtTemp )
{
	GUI_COLOR	colorTemp;
	
	if( prtTemp )
	{
		colorTemp = GUI_GetColor();
		GUI_SetColor(GUI_WHITE);
		GUI_FillRect( prtTemp->x0, prtTemp->y0, prtTemp->x1, prtTemp->y1 );
		GUI_SetColor(colorTemp);
	}
}



/*
	frame page part
*/

int spFramePageValid( FRAMEPAGE_TYPE type )
{
	int iRet = 0;
	
	if( 
		(type != pCurrFramePageType) ||
		(NULL == pCurrFramePageFrameData)
	)
	{
		SPPRINTF("!!!!Error, there should be frame page data here type=%d!! abort!!\n", type);
		iRet = 1;
	}
	
	return iRet;
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
		(0 != pCurrFramePageWait) &&
		(1 == iTO) && 
		(NULL != pCurrFramePageTimeoutFrame) 
	)
	{
		spGoAfterFramePage( pCurrFramePageTimeoutFrame );
		iRet = 1;
	}
	return iRet;
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


int spIsMsgKeyStatus( WM_MESSAGE* pMsg, int iTar )
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





/* ********************************************************* */
#endif	/* RENOUI_PROJ */
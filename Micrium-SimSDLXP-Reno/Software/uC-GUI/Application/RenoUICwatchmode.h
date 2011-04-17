
/*
	header of UI frame page watch mode functions
*/

#ifndef _RENOUICWATCHMODE_H_
#define	_RENOUICFTEXTENTRY_H_

#include "RenoGUI.h"
#include "RenoUIframe.h"


/*
	frame page data of Watch mode
*/
typedef struct {
///	const GUI_ConstString	sTitle;
///	const GUI_BITMAP*		pBitmap;
	FRAMEPAGE_HEADER*		pUplevelFrame;
	FRAMEPAGE_HEADER* 		pNextFrame;
///	int*					pListParam;
///	WM_HWIN					hWinFrame;	///outline frame
	TEXT_Handle				hTextDays;
	TEXT_Handle				hTextDate;
	TEXT_Handle				hTextTime;
	TEXT_Handle				hTextSecond;
	TEXT_Handle				hText24H;
} FP_WATCH_HEADER;



void WatchWindow( int iOption );
void cbWatchWindow(WM_MESSAGE* pMsg);
void spUpdateWatchModeWindow( void );


#include "RenoUIframepage.h"


#endif	/* _RENOUICFTEXTENTRY_H_ */
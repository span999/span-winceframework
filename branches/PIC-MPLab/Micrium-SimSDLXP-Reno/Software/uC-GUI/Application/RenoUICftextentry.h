
/*
	header of UI frame page list menu functions
*/

#ifndef _RENOUICFTEXTENTRY_H_
#define	_RENOUICFTEXTENTRY_H_

#include "RenoGUI.h"
#include "RenoUIframe.h"


/*
	frame page data of float text entry
*/
typedef struct {
	int						iListNum;
	int						iListSel;
	const GUI_ConstString	sTitle;
	const GUI_ConstString*	sListText;
	FRAMEPAGE_HEADER*		pUplevelFrame;
	FRAMEPAGE_HEADER* 		pNextFrame;
	WM_HWIN					hWinInputFrame;		///text frame
	LISTBOX_Handle			hWinInputList;		///input list
	char*					sResult;			///text extery result
} FP_FTEXTENTRY_HEADER;



void FloatTextEntryWindow( int iOption );
void cbFloatTextEntryWindow(WM_MESSAGE* pMsg);


#include "RenoUIframepage.h"


#endif	/* _RENOUICFTEXTENTRY_H_ */
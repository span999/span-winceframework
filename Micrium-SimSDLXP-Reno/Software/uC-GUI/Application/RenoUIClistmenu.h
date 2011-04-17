
/*
	header of UI frame page list menu functions
*/

#ifndef _RENOUICLISTMENU_H_
#define	_RENOUICLISTMENU_H_

#include "RenoGUI.h"
#include "RenoUIframe.h"



#define LISTBOX_CI_DISABLED			3	///missing?!!
/*
	list menu elements type define
*/

#define		ELEMENT_FIELD_MASK				0xFFFF0000
#define		ELEMENT_FIELD_OFFSET			16
#define		COMMON_FIELD					0x0000
#define		BOOLEAN_OPTION_FIELD			0x0001

#define		ELEMENT_VALUE_MASK				0x0000FFFF



/*
	frame page data of menu list
*/
typedef struct
{
	int						iListNum;
	const GUI_ConstString	sListTitle;
	const GUI_ConstString*	sListName;
	FRAMEPAGE_HEADER*		pUplevelFrame;
	FRAMEPAGE_HEADER** 		pListFrame;
	int*					pListParam;
} FP_LISTMENU_HEADER;




void cbListMenuWindow(WM_MESSAGE* pMsg);
void ListMenuWindow( int iOption );



#include "RenoUIframepage.h"


#endif	/* _RENOUICLISTMENU_H_ */
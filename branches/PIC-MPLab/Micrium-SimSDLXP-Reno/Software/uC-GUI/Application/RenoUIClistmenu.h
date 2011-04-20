
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
#define		BOOLEAN_OPTION_FIELD			0x0001		///item of list menu in boolean
#define		POPUP_SELECTOR_FIELD			0x0002		///item of popup list in selector

#define		ELEMENT_VALUE_MASK				0x0000FFFF



/*
	frame page data of menu list
*/
typedef struct
{
	int						iListNum;
	const GUI_ConstString	sListTitle;
	const GUI_ConstString*	sListName;
	const GUI_ConstString*	sListShortName;
	FRAMEPAGE_HEADER*		pUplevelFrame;
	FRAMEPAGE_HEADER** 		pListFrame;
	int*					pListParam;
	unsigned int			uListItemSpacing;			
} FP_LISTMENU_HEADER;



/*
	frame page data of popup list
*/
typedef struct
{
	int						iListNum;
	int						iListSel;
	const GUI_ConstString	sListTitle;
	const GUI_ConstString*	sListName;
	FRAMEPAGE_HEADER*		pUplevelFrame;
	FRAMEPAGE_HEADER** 		pListFrame;
	int*					pListParam;
	WM_HWIN					hWinFrame;	///outline frame
} FP_POPUPLIST_HEADER;




void cbListMenuWindow(WM_MESSAGE* pMsg);
void ListMenuWindow( int iOption );

void cbPopupWindow(WM_MESSAGE* pMsg);
void cbPopupWindowList(WM_MESSAGE* pMsg);
void PopupWindowList( int iOption );


void cbPopupWindowNumbers(WM_MESSAGE* pMsg);
void cbSAPDPADSASNSPopupWindowNumbers(WM_MESSAGE* pMsg);
void PopupWindowNumbers( int iOption );


#include "RenoUIframepage.h"


#endif	/* _RENOUICLISTMENU_H_ */
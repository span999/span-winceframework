#ifndef __FIELDHISTORYMENU_H__
#define  __FIELDHISTORYMENU_H__


/*
	type define of common menu
*/
typedef struct
{
    WORD						multilangId;
	XCHAR*						pName;
	void*						pIcon;
	FRAME_HEADER* 				pGoFrame;
} COMMONMENUITEM_HEADER;

typedef struct
{
    WORD						Id;
    SHORT 						left; 
    SHORT 						top; 
    SHORT 						right; 
    SHORT 						bottom; 
    WORD 						state; 
    WORD						multilangId;
	XCHAR*						pTitle; 
    GOL_SCHEME*					pScheme;
	SHORT						focusedItemNum;
	SHORT						totalItemNum;
	COMMONMENUITEM_HEADER		Item1;
	COMMONMENUITEM_HEADER		Item2;
	COMMONMENUITEM_HEADER		Item3;
	COMMONMENUITEM_HEADER		Item4;
	COMMONMENUITEM_HEADER		Item5;
	COMMONMENUITEM_HEADER		Item6;
} COMMONMENU_HEADER;


extern FRAME_HEADER fhHistoryMenu_main;
extern FRAME_HEADER fhHistoryMenu_activityhistory;
extern FRAME_HEADER fhHistoryMenu_AHview;
extern FRAME_HEADER fhHistoryMenu_AHdelete;
extern FRAME_HEADER fhHistoryMenu_activitytotals;
extern FRAME_HEADER fhHistoryMenu_ATview;
extern FRAME_HEADER fhHistoryMenu_ATclear;
extern FRAME_HEADER fhHistoryMenu_locationhistory;
extern FRAME_HEADER fhHistoryMenu_LHview;
extern FRAME_HEADER fhHistoryMenu_LHdelete;



#endif	/*#ifndef __FIELDHISTORYMENU_H__*/

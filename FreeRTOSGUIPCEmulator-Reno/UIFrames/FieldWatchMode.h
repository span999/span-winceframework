#ifndef __FIELDWATCHMODE_H__
#define  __FIELDWATCHMODE_H__


/*
	typr define of popup menu
*/
typedef struct
{
    XCHAR*						pPopMsg;   	/// msg in popup item
	void*						pIcon;		/// icon in popup item
	FRAME_HEADER*				pGoFrame;	/// frame to go if popup item selected
} POPUPITEM_HEADER;

typedef struct
{
    POPUPITEM_HEADER*			pPopItem1;	/// content of popup item 1
    POPUPITEM_HEADER*			pPopItem2;	/// content of popup item 2
    POPUPITEM_HEADER*			pPopItem3;	/// content of popup item 3
    POPUPITEM_HEADER*			pPopItem4;	/// content of popup item 4
    POPUPITEM_HEADER*			pPopItem5;	/// content of popup item 5
    POPUPITEM_HEADER*			pPopItem6;	/// content of popup item 6
} POPUPITEMLIST_HEADER;

typedef struct
{
    SHORT						PopItemNum;		/// item number in popup
	XCHAR*						pPopTitle;  	/// popup title
	FRAME_HEADER*				pPrivFrame;		/// privious frame before popup
    POPUPITEMLIST_HEADER*		pPopItemList;   /// content of all popup items
} POPUPOPTION_HEADER;

#define		POPUP_FRAME_OFFSET_W	10
#define		POPUP_FRAME_OFFSET_H	30

extern void PPMenuSetUp( SHORT	ItemNum, XCHAR *pTitle, FRAME_HEADER *pPrivF );
extern void PPMenuItemsSetUp( SHORT ItemNum, XCHAR *pMsg, void *pIcon, FRAME_HEADER *pFrame2Go );

#define 	PPMenuItem1SetUp(a,b,c) PPMenuItemsSetUp( 1, a, b, c)
#define 	PPMenuItem2SetUp(a,b,c) PPMenuItemsSetUp( 2, a, b, c)
#define 	PPMenuItem3SetUp(a,b,c) PPMenuItemsSetUp( 3, a, b, c)
#define 	PPMenuItem4SetUp(a,b,c) PPMenuItemsSetUp( 4, a, b, c)
#define 	PPMenuItem5SetUp(a,b,c) PPMenuItemsSetUp( 5, a, b, c)
#define 	PPMenuItem6SetUp(a,b,c) PPMenuItemsSetUp( 6, a, b, c)

extern POPUPOPTION_HEADER popupOption;


extern FRAME_HEADER fhWatchMode_watch;
extern FRAME_HEADER fhDeviceMode_poweroff;
extern FRAME_HEADER fhDeviceMode_booting;
extern FRAME_HEADER fhDeviceMode_popup;


#endif	/*#ifndef __FIELDWATCHMODE_H__*/

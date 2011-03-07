#ifndef __FIELDWATCHMODE_H__
#define  __FIELDWATCHMODE_H__


/*
	type define of popup menu
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



/*
	type define of popup ask
*/
typedef struct
{
    POPUPITEM_HEADER*			pPopItem11;	/// content of popup item 1
    POPUPITEM_HEADER*			pPopItem12;	/// content of popup item 2
} POPUPASKLIST_HEADER;

typedef struct
{
    SHORT						PopItemNum;		/// item number in popup
	XCHAR*						pPopTitle;  	/// popup title
	FRAME_HEADER*				pPrivFrame;		/// privious frame before popup
    POPUPASKLIST_HEADER*		pPopAskList;   /// content of all popup items
} POPUPASK_HEADER;

#define		POASK_FRAME_OFFSET_W	15
#define		POASK_FRAME_OFFSET_H	35
#define		POASK_FRAME_W	90
#define		POASK_FRAME_H	25



///popup menu
extern void PPMenuSetUp( SHORT ItemNum, XCHAR *pTitle, FRAME_HEADER *pPrivF );
extern void PPMenuItemsSetUp( SHORT ItemNum, XCHAR *pMsg, void *pIcon, FRAME_HEADER *pFrame2Go );

#define 	PPMenuItem1SetUp(a,b,c) PPMenuItemsSetUp( 1, a, b, c)
#define 	PPMenuItem2SetUp(a,b,c) PPMenuItemsSetUp( 2, a, b, c)
#define 	PPMenuItem3SetUp(a,b,c) PPMenuItemsSetUp( 3, a, b, c)
#define 	PPMenuItem4SetUp(a,b,c) PPMenuItemsSetUp( 4, a, b, c)
#define 	PPMenuItem5SetUp(a,b,c) PPMenuItemsSetUp( 5, a, b, c)
#define 	PPMenuItem6SetUp(a,b,c) PPMenuItemsSetUp( 6, a, b, c)

///popup ask
extern void PPAskSetUp( SHORT ItemNum, XCHAR *pTitle, FRAME_HEADER *pPrivF );
extern void PPAskItemsSetUp( SHORT ItemNum, XCHAR *pMsg, void *pIcon, FRAME_HEADER *pFrame2Go );

#define 	PPAskItem1SetUp(a,b,c) PPAskItemsSetUp( 1, a, b, c)
#define 	PPAskItem2SetUp(a,b,c) PPAskItemsSetUp( 2, a, b, c)



/// entry pad
extern void CreateTextEntryPadHook(WORD wDrawOption);
extern WORD MsgTextEntryPadHook(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
extern WORD MsgTextEntryPadDefaultBtnHook(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg);
extern void CreateNumEntryPadHook(WORD wDrawOption);
extern WORD MsgNumEntryPadHook(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
extern WORD MsgNumEntryPadDefaultBtnHook(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg);







///temp
extern const XCHAR Ask01Str[];
extern const XCHAR Ask02Str[];
extern const XCHAR Ask03Str[];
extern const XCHAR Ask04Str[];




extern POPUPOPTION_HEADER popupOption;
extern POPUPASK_HEADER popupAsk;


extern FRAME_HEADER fhWatchMode_watch;
extern FRAME_HEADER fhDeviceMode_poweroff;
extern FRAME_HEADER fhDeviceMode_booting;
extern FRAME_HEADER fhDeviceMode_popup;
extern FRAME_HEADER fhDeviceMode_popask;



#endif	/*#ifndef __FIELDWATCHMODE_H__*/

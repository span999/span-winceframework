#ifndef __FIELDWATCHMODE_H__
#define  __FIELDWATCHMODE_H__



typedef struct
{
    XCHAR*			pPopMsg;   ///
	void*			pIcon;		///
	FRAME_HEADER*	pGoFrame;
} POPUPITEM_HEADER;


typedef struct
{
    POPUPITEM_HEADER*			pPopItem1;   ///
    POPUPITEM_HEADER*			pPopItem2;   ///
    POPUPITEM_HEADER*			pPopItem3;   ///
    POPUPITEM_HEADER*			pPopItem4;   ///
    POPUPITEM_HEADER*			pPopItem5;   ///
    POPUPITEM_HEADER*			pPopItem6;   ///
} POPUPITEMLIST_HEADER;


typedef struct
{
    SHORT						PopItemNum;   ///
	XCHAR*						pPopTitle;   ///
	FRAME_HEADER*				pPrivFrame;
    POPUPITEMLIST_HEADER*		pPopItemList;   ///
} POPUPOPTION_HEADER;






extern FRAME_HEADER fhWatchMode_watch;
extern FRAME_HEADER fhDeviceMode_poweroff;
extern FRAME_HEADER fhDeviceMode_booting;
extern FRAME_HEADER fhDeviceMode_popup;

extern POPUPOPTION_HEADER popupOption;

#define		POPUP_FRAME_OFFSET_W	20
#define		POPUP_FRAME_OFFSET_H	30





#endif	/*#ifndef __FIELDWATCHMODE_H__*/

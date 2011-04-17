
/*
	header of common UI frame page
*/

#ifndef _RENOUICCOMM_H_
#define	_RENOUICCOMM_H_

#include "RenoGUI.h"
#include "RenoUIframe.h"
#include "RenoActivityDataType.h"




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


/*
	frame page data of popup notify
*/
typedef struct {
	const GUI_ConstString	sTitle;
	const GUI_BITMAP*		pBitmap;
	FRAMEPAGE_HEADER*		pUplevelFrame;
	FRAMEPAGE_HEADER* 		pNextFrame;
///	int*					pListParam;
	WM_HWIN					hWinFrame;	///outline frame
} FP_POPUPNOTIFY_HEADER;

#define IsNotCurrFPListMenuLike		((FRAMEPAGE_LISTMENU != pCurrFramePageType) && (FRAMEPAGE_LISTMENU_BOOLOPTION != pCurrFramePageType))


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



/*
	Data enclose - GPS Satellites info
*/
typedef struct {
	int iID;		///satellites ID
	int iVaild;		///satellite vaild, 1 for true
	int iSignal;	///signal level
} _DE_GPS_SATELLITE;

typedef struct {
	int 					iTotal;			///numbers of satellites info included
	_DE_GPS_SATELLITE		satellites[12];	///satellites info, max 12.
} DE_GPS_SATELLITES_INFO;


/*
	Data enclose - activity data 
*/
typedef struct {
	int 				iID;		///activity ID
	char*			 	sDataStr;	///activity data string
} _DE_ACTIVITY;

typedef struct {
	int 					iTotal;			///numbers of activity data included
	_DE_ACTIVITY			activity[6];	///activity data info, max 6 per page.
} DE_ACTIVITY_DATA;






/*
	user data message type define
*/
#define			WM_GPS_SATELLITE			(WM_USER+0x30)
#define			WM_ACTIVITY_DATA			(WM_USER+0x30)+1



void spSetDefaultEffect ( void );
int spGetListParamType( int ListParam );
int spGetListParamValue( int ListParam );
void spcbRoundWinExt( WM_MESSAGE* pMsg, GUI_COLOR color, int radius, int pensize, int iFill );
void spcbPopupNumberWinExt( WM_MESSAGE* pMsg, GUI_COLOR color, int pensize, int iFill );
void cbPoweroffWindow(WM_MESSAGE* pMsg);
void PoweroffWindow( int iOption );
void BootWindow( int iOption );
void cbPopupWindowNotify(WM_MESSAGE* pMsg);
void PopupWindowNotify( int iOption );
void cbPopupWindowList(WM_MESSAGE* pMsg);
void PopupWindowList( int iOption );
void cbPopupWindowNumbers(WM_MESSAGE* pMsg);
void PopupWindowNumbers( int iOption );
void cbNavigationWindow(WM_MESSAGE* pMsg);
void NavigationWindow( int iOption );
void cbSGSGSWindow(WM_MESSAGE* pMsg);
void SGSGSWindow( int iOption );
void cbTextEntryWindow(WM_MESSAGE* pMsg);
void TextEntryWindow( int iOption );
void cbSAPDPADSASNSPopupWindowNumbers(WM_MESSAGE* pMsg);
void cbWatchWindow(WM_MESSAGE* pMsg);
void WatchWindow( int iOption );
void UnderConstructionWindow( int iOption );




#include "RenoUIframepage.h"


#endif	/* _RENOUICCOMM_H_ */
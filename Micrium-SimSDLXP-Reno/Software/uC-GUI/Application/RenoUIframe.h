
/*
	header of UI frame
*/

#ifndef _RENOUIFRAME_H_
#define	_RENOUIFRAME_H_


#include "RenoGUI.h"


/*
	type define for frame header and screen
*/

/*
	frame page main routin 
*/
typedef void (*PFNFRAMEPAGEMAIN)(int iOption);

/*
	frame page main callback routin
*/
typedef void (*PFNFRAMEPAGEMAINCB)(WM_MESSAGE* pMsg);

/*
	frame page types 
*/
typedef enum
{
	FRAMEPAGE_COMMON = 0,
    FRAMEPAGE_ACTIVITY,
	FRAMEPAGE_DATAMODE,
    FRAMEPAGE_TITLED,
    FRAMEPAGE_LISTMENU,
	FRAMEPAGE_LISTMENU_BOOLOPTION,	///list menu with all boolean option items
	FRAMEPAGE_POPUP_NOTIFY,
	FRAMEPAGE_POPUP_OPTION,
	FRAMEPAGE_POPUP_CONFIRM,
	FRAMEPAGE_POPUP_NUMBERS,
	FRAMEPAGE_FLOATTEXT_ENTRY,
	FRAMEPAGE_WATCH,
} FRAMEPAGE_TYPE;


/*
	frame page header
*/

struct H_FRAMEPAGE_HEADER
{
	FRAMEPAGE_TYPE				frametype;				///frame page type index
    PFNFRAMEPAGEMAIN			pfnFramePageMain;     	///address of main routine 
    PFNFRAMEPAGEMAINCB			pfnFramePageMainCb;    	///address of user callback
	WM_CALLBACK* 				pfnOldCB;				///old callback address store
	int							iWaits;					///frame page timeout
	int							iNextReady;				///flag for frame page done, 1 for ready
	int							iClearFirst;			///flag for clear before draw, 1 for action
	void*						pFrameData;				///frame data if needed, context info
	WM_HWIN 					hWinFramePageMain;		///handle of main frame page, callback hook on
	struct H_FRAMEPAGE_HEADER*	pTimeoutFrame;			///frame to go if timeout
};

typedef struct H_FRAMEPAGE_HEADER FRAMEPAGE_HEADER;


/*
	screen status record
*/
typedef struct
{
	FRAMEPAGE_HEADER*		pPrivFramePage;
	FRAMEPAGE_HEADER*		pNextFramePage;
	FRAMEPAGE_HEADER*		pNowFramePage;
} SCREEN_STATUS;




/*
	shortcut declare
*/
#define		pCurrFramePage					ScrStat.pNowFramePage
#define		pAfterFramePage					ScrStat.pNextFramePage
#define		pBeforeFramePage				ScrStat.pPrivFramePage

#define		pCurrFramePageType				ScrStat.pNowFramePage->frametype
#define		pCurrFramePageMain				ScrStat.pNowFramePage->pfnFramePageMain
#define		pCurrFramePageMainCb			ScrStat.pNowFramePage->pfnFramePageMainCb
#define		pCurrFramePageOldCb				ScrStat.pNowFramePage->pfnOldCB
#define		pCurrFramePageWait				ScrStat.pNowFramePage->iWaits
#define		pCurrFramePageNextReady			ScrStat.pNowFramePage->iNextReady
#define		pCurrFramePageClearFirst		ScrStat.pNowFramePage->iClearFirst
#define		pCurrFramePageFrameData			ScrStat.pNowFramePage->pFrameData
#define		pCurrFramePageHandle			ScrStat.pNowFramePage->hWinFramePageMain
#define		pCurrFramePageTimeoutFrame		ScrStat.pNowFramePage->pTimeoutFrame


#endif	/* _RENOUIFRAME_H_ */
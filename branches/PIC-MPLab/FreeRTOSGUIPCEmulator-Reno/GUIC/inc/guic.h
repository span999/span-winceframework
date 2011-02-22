#ifndef __GUIC_H__
#define  __GUIC_H__

#include <stdio.h>

/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include <kbkeymap.h>

typedef WORD (*PFNMSGCALLBACK)(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
typedef void (*PFNDRAWCALLBACK)(WORD wDrawOption);
typedef WORD (*PFNBTNHANDLE)(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);





/////////////////////////////////////////////////////////////////////////////
//                            DEMO STATES
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CREATE_BUTTONS          = 0,
    DISPLAY_BUTTONS,
    CREATE_CHECKBOXES,
    DISPLAY_CHECKBOXES,
    CREATE_RADIOBUTTONS,
    DISPLAY_RADIOBUTTONS,
    CREATE_STATICTEXT,
    DISPLAY_STATICTEXT,
    CREATE_PICTURE,
    DISPLAY_PICTURE,
    CREATE_SLIDER,
    DISPLAY_SLIDER,
    CURSOR_DRAW_SLIDER,
    CREATE_PROGRESSBAR,
    DISPLAY_PROGRESSBAR,
    CREATE_LISTBOX,
    DISPLAY_LISTBOX,
    CREATE_EDITBOX,
    DISPLAY_EDITBOX,
    CREATE_METER,
    DISPLAY_METER,
    CREATE_DIAL,
    DISPLAY_DIAL,
    CREATE_CUSTOMCONTROL,
    DISPLAY_CUSTOMCONTROL,
    CREATE_SIGNATURE,
    DISPLAY_SIGNATURE,
    BOX_DRAW_SIGNATURE,
    CREATE_POT,
    DISPLAY_POT,
    BOX_DRAW_POT,
    CREATE_ECG,
    DISPLAY_ECG,
    BOX_DRAW_ECG,
    CREATE_PULLDOWN,
    DISPLAY_PULLDOWN,
///	
	CREATE_RENO_DATASET,
	DISPLAY_RENO_DATASET,
	CREATE_TEXTENTRYPAD,
	DISPLAY_TEXTENTRYPAD,
	CREATE_NUMENTRYPAD,
	DISPLAY_NUMENTRYPAD,
///
    // these states are for time and date settings 0xF3xx is used here as a
    // state ID to check when date and time are to be updated or not.
    CREATE_DATETIME         = 0xF300,                       // creates the date and time setting menu		
    DISPLAY_DATETIME        = 0xF301,                       // displays the menu for the date and time setting
    DISPLAY_DATE_PDMENU     = 0xF302,                       // displays the pulldown menu to set one of the date items (month, day or year)
    SHOW_DATE_PDMENU        = 0xF303,                       // creates the pulldown menu to set one of the date items (month, day or year)
    HIDE_DATE_PDMENU        = 0xF304,                       // hides the pulldown menu to set one of the date items (month, day or year)
} SCREEN_STATES;





typedef struct
{
    PFNMSGCALLBACK		pfnMsgCallback;     // 
    PFNDRAWCALLBACK		pfnDrawCallback;    // 
	PFNBTNHANDLE		pfnBtnHandle;		// handle button event for this frame.
	WORD				wDrawOption;
    SHORT   height;          // 
    SHORT   width;           // 
} FRAME_HEADER;


typedef struct
{
	SCREEN_STATES		privStat;
	SCREEN_STATES		nowStat;
	SCREEN_STATES		nextStat;
	FRAME_HEADER*		pnowStatFrame;
	BOOL 				IsFrameCreate;
} SCREEN_STATUS;


/// API for screen status handle
void scrInitStat(SCREEN_STATUS* pScreenStat);
void scrNextStat(SCREEN_STATUS* pScreenStat);
void scrPrivStat(SCREEN_STATUS* pScreenStat);


/// use for MsgCallback & DrawCallback
WORD scrMsgCbHandler(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
void scrDrawCbHandler(void);




#endif	/*#ifndef __GUIC_H__*/

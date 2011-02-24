#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>


/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "guic.h"
#include "ObjectTest.h"
#include "FontGentium.h"
#include "1bpp_icons.h"
#include "4bpp_icons.h"
#include "16164bppIcon.h"

#include "FieldCommon.h"
#include "FieldWatchMode.h"
#include "FieldDataMode.h"

#ifdef USE_MAGELLAN_LOGO
#include "magellan_logo.h"
#endif


void CreateWatchMode_watch(WORD wDrawOption)
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();

	CreateDefaultBtn();

    WndCreate
    (
        ID_WINDOW1,                 // ID
        0,
        0,
        GetMaxX(),
        GetMaxY() - DEFAULTBTN_HEIGHT,                  // dimension
        WND_DRAW,                   // will be dislayed after creation
        (void *) &I16164_Clock,         // icon
///        pText,                      // set text
		(XCHAR *)"<<Watch Mode>>",         // text
        alt3Scheme
    );                              // default GOL scheme
	
	CreateDataSet( 0,  40, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "Watch Mode", "Time", "23:23", "24h");
	
}



WORD MsgWatchMode_watch(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}

WORD MsgWatchMode_watchDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///display mode change 1,2,3
			}
			return (0); 
        case ID_BTN_UP_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///data mode or power down
				scrSetStat(&fhDataMode_one);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///display mode change 3,2,1
			}
			return (0); 
        case ID_BTN_DOWN_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///???
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///record
			}
			return (0); 
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///reset
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///lap
			}
			return (0); 
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///mark-next
			}
			return (0); 

        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
	}
}


void CreateDeviceMode_poweroff(WORD wDrawOption)
{
    SHORT       width, height;

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();

	CreateDefaultBtn();

	// draw fonts in the screen
    SetFont((void *) &Gentium_Normal25);
	SetColor(LIGHTGREEN);
	width = GetTextWidth("PowerOff", (void *) &Gentium_Normal25);
    height = GetTextHeight((void *) &Gentium_Normal25);
	OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height - DEFAULTBTN_HEIGHT) >> 1, "PowerOff");
}



WORD MsgDeviceMode_poweroff(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}

WORD MsgDeviceMode_poweroffDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///display mode change 1,2,3
			}
			return (0); 
        case ID_BTN_UP_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///power up
				scrSetStat(&fhDeviceMode_booting);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///display mode change 3,2,1
			}
			return (0); 
        case ID_BTN_DOWN_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///???
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///record
			}
			return (0); 
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///reset
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///lap
			}
			return (0); 
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///mark-next
			}
			return (0); 

        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
	}
}


void CreateDeviceMode_booting(WORD wDrawOption)
{
    SHORT       width, height;

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();

	CreateDefaultBtn();

/***********************************************************************************************************/
	SetColor(BLACK);
	ClearDevice();
	// draw fonts in the screen
	SetFont((void *) &Gentium_Normal15);
	SetColor(LIGHTRED);
	width = GetTextWidth("This Demo bases on", (void *) &Gentium_Normal15);
	height = GetTextHeight((void *) &Gentium_Normal15);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-20, "This Demo bases on");
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	SetFont((void *) &Gentium_Normal15);
	SetColor(LIGHTGREEN);
	width = GetTextWidth("freeRTOS:v6.1.0", (void *) &Gentium_Normal15);
	height = GetTextHeight((void *) &Gentium_Normal15);
	OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "freeRTOS:v6.1.0");
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	SetFont((void *) &Gentium_Normal15);
	SetColor(LIGHTBLUE);
	width = GetTextWidth("MicroChip AppLib:v2.11", (void *) &Gentium_Normal15);
	height = GetTextHeight((void *) &Gentium_Normal15);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+20, "MicroChip AppLib:v2.11");
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	


	
/***********************************************************************************************************/

#ifdef USE_MAGELLAN_LOGO
	SetColor(BLACK);
	ClearDevice();

	width = GetImageWidth((void *) &magellan_logo_01);
	height = GetImageHeight((void *) &magellan_logo_01);
	
	width = (GetMaxX() - width) >> 1;
	height = (GetMaxY() - height) >> 1;
	
	WAIT_UNTIL_FINISH(PutImage(width, height, (void *) &magellan_logo_01, 1));
	DelayMs(DEMODELAY);
	WAIT_UNTIL_FINISH(PutImage(width, height, (void *) &magellan_logo_02, 1));
	DelayMs(DEMODELAY);
	WAIT_UNTIL_FINISH(PutImage(width, height, (void *) &magellan_logo_03, 1));
	DelayMs(DEMODELAY);
	WAIT_UNTIL_FINISH(PutImage(width, height, (void *) &magellan_logo_04, 1));
	DelayMs(DEMODELAY);
	WAIT_UNTIL_FINISH(PutImage(width, height, (void *) &magellan_logo_05, 1));
	DelayMs(DEMODELAY);
	WAIT_UNTIL_FINISH(PutImage(width, height, (void *) &magellan_logo_06, 1));
	DelayMs(DEMODELAY);
	WAIT_UNTIL_FINISH(PutImage(width, height, (void *) &magellan_logo_07, 1));
	DelayMs(DEMODELAY);
	WAIT_UNTIL_FINISH(PutImage(width, height, (void *) &magellan_logo_08, 1));
	DelayMs(DEMODELAY);
#endif

///				scrSetStat(&fhDataMode_two);
///				scrCreateInit();
	
}



WORD MsgDeviceMode_booting(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}

WORD MsgDeviceMode_bootingDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///display mode change 1,2,3
				scrSetStat(&fhDataMode_two);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_UP_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///power down
				scrSetStat(&fhDeviceMode_poweroff);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///display mode change 3,2,1
				scrSetStat(&fhDataMode_two);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_DOWN_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///???
				
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///record
				scrSetStat(&fhDataMode_two);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///reset
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///lap
				scrSetStat(&fhDataMode_two);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///mark-next
			}
			return (0); 

        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
	}
}



FRAME_HEADER fhWatchMode_watch = {
	MsgWatchMode_watch,
	CreateWatchMode_watch,
	MsgWatchMode_watchDefaultBtn,
	0,		///no option
	0,
	0
};


FRAME_HEADER fhDeviceMode_poweroff = {
	MsgDeviceMode_poweroff,
	CreateDeviceMode_poweroff,
	MsgDeviceMode_poweroffDefaultBtn,
	0,		///no option
	0,
	0
};


FRAME_HEADER fhDeviceMode_booting = {
	MsgDeviceMode_booting,
	CreateDeviceMode_booting,
	MsgDeviceMode_bootingDefaultBtn,
	0,		///no option
	0,
	0
};

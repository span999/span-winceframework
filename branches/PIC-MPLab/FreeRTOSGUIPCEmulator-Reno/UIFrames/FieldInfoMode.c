#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>


/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "guic.h"
#include "ObjectTest.h"
#include "fonts.h"
#include "icons.h"
#include "fields.h"
#include "StringID.h"



void CreateInfoMode_info(WORD wDrawOption)
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
        (void *) &I16164_Yes,         // icon
///        pText,                      // set text
		(XCHAR *)IdGetMString(6,gLanguage),         // text
        windowScheme
    );   
	
	updateLastView( scrGetStat() );	///update last view
}



WORD MsgInfoMode_info(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}

WORD MsgInfoMode_infoDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
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
				///watch mode or power down		
				PPMenuSetUp( 2, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(4,gLanguage), &I16164_Clock, &fhWatchMode_watch );
				{	///setup for fhDeviceMode_popask
					PPAskSetUp( 2, Ask02Str, scrGetStat() );
					PPAskItem1SetUp( Ask03Str, NULL, &fhDeviceMode_poweroff );
					PPAskItem2SetUp( Ask04Str, NULL, scrGetStat() );
				}
				///PPMenuItem2SetUp( IdGetMString(3,gLanguage), &I16164_Abort, &fhDeviceMode_poweroff );
				PPMenuItem2SetUp( IdGetMString(3,gLanguage), &I16164_Abort, &fhDeviceMode_popask );
				scrSetNEXT(&fhDeviceMode_popup);
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
				///Navigation mode, setting, avtivity data, quick info, history
				PPMenuSetUp( 4, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(9,gLanguage), &PCGaming1_4bpp_16x16, &fhDataMode_two );
				PPMenuItem2SetUp( IdGetMString(5,gLanguage), &I16164_Compass, &fhMapMode_navgation );
				PPMenuItem3SetUp( IdGetMString(7,gLanguage), &I16164_Apply, &fhSettingMenu_main );
				PPMenuItem4SetUp( IdGetMString(8,gLanguage), &I16164_Apply, &fhHistoryMenu_main );
				scrSetNEXT(&fhDeviceMode_popup);		
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




FRAME_HEADER fhInfoMode_info = {
	MsgInfoMode_info,
	CreateInfoMode_info,
	MsgInfoMode_infoDefaultBtn,
	0,		///no option
	0,
	0
};



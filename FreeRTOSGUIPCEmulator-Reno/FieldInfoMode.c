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
#include "FieldSettingMenu.h"
#include "FieldDataMode.h"
#include "FieldMapMode.h"


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
		(XCHAR *)"<<Info Mode>>",         // text
        alt3Scheme
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
				popupOption.PopItemNum = 2;
				popupOption.pPopTitle = "Options";
				popupOption.pPrivFrame = scrGetStat();
				popupOption.pPopItemList->pPopItem1->pPopMsg = "Watch Mode";
				popupOption.pPopItemList->pPopItem1->pIcon = &I16164_Clock;
				popupOption.pPopItemList->pPopItem1->pGoFrame = &fhWatchMode_watch;
				popupOption.pPopItemList->pPopItem2->pPopMsg = "Power Down";
				popupOption.pPopItemList->pPopItem2->pIcon = &I16164_Abort;
				popupOption.pPopItemList->pPopItem2->pGoFrame = &fhDeviceMode_poweroff;
				scrSetStat(&fhDeviceMode_popup);
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
				///Navigation mode, setting, avtivity data, quick info, history
				popupOption.PopItemNum = 3;
				popupOption.pPopTitle = "Options";
				popupOption.pPrivFrame = scrGetStat();
				popupOption.pPopItemList->pPopItem1->pPopMsg = "Activity Data";
				popupOption.pPopItemList->pPopItem1->pIcon = &PCGaming1_4bpp_16x16;
				popupOption.pPopItemList->pPopItem1->pGoFrame = &fhDataMode_two;
				popupOption.pPopItemList->pPopItem2->pPopMsg = "Navigation";
				popupOption.pPopItemList->pPopItem2->pIcon = &I16164_Compass;
				popupOption.pPopItemList->pPopItem2->pGoFrame = &fhMapMode_navgation;
				popupOption.pPopItemList->pPopItem3->pPopMsg = "Setting";
				popupOption.pPopItemList->pPopItem3->pIcon = &Settings_4bpp_16x16;
				popupOption.pPopItemList->pPopItem3->pGoFrame = &fhSettingMenu_main;

				scrSetStat(&fhDeviceMode_popup);
				scrCreateInit();
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



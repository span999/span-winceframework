#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>


/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "guic.h"
#include "ObjectTest.h"
#if 0
#include "FontGentium.h"
#include "1bpp_icons.h"
#include "4bpp_icons.h"
#include "16164bppIcon.h"

#include "FieldCommon.h"
#include "FieldWatchMode.h"
#include "FieldSettingMenu.h"
#include "FieldDataMode.h"
#include "FieldMapMode.h"
#else
#include "fonts.h"
#include "icons.h"
#include "fields.h"
#endif
#include "StringID.h"



void CreateMapMode_navigation(WORD wDrawOption)
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
        (void *) &I16164_Compass,         // icon
///        pText,                      // set text
		(XCHAR *)IdGetMString(5,gLanguage),         // text
        windowScheme
    );                              // default GOL scheme
	
	updateLastView( scrGetStat() );	///update last view
}



WORD MsgMapMode_navigation(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}

WORD MsgMapMode_navigationDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
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
			#if 0
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
			#else	
				PPMenuSetUp( 2, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(4,gLanguage), &I16164_Clock, &fhWatchMode_watch );
				PPMenuItem2SetUp( IdGetMString(3,gLanguage), &I16164_Abort, &fhDeviceMode_poweroff );
				scrSetNEXT(&fhDeviceMode_popup);
			#endif				
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
			#if 0
				popupOption.PopItemNum = 3;
				popupOption.pPopTitle = "Options";
				popupOption.pPrivFrame = scrGetStat();
				popupOption.pPopItemList->pPopItem1->pPopMsg = "Activity Data";
				popupOption.pPopItemList->pPopItem1->pIcon = &PCGaming1_4bpp_16x16;
				popupOption.pPopItemList->pPopItem1->pGoFrame = &fhDataMode_two;
				popupOption.pPopItemList->pPopItem2->pPopMsg = "Quick Info";
				popupOption.pPopItemList->pPopItem2->pIcon = &I16164_About;
				popupOption.pPopItemList->pPopItem2->pGoFrame = &fhInfoMode_info;
				popupOption.pPopItemList->pPopItem3->pPopMsg = "Setting";
				popupOption.pPopItemList->pPopItem3->pIcon = &Settings_4bpp_16x16;
				popupOption.pPopItemList->pPopItem3->pGoFrame = &fhSettingMenu_main;

				scrSetStat(&fhDeviceMode_popup);
				scrCreateInit();
			#else
				PPMenuSetUp( 4, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(9,gLanguage), &PCGaming1_4bpp_16x16, &fhDataMode_two );
				PPMenuItem2SetUp( IdGetMString(6,gLanguage), &I16164_About, &fhInfoMode_info );
				PPMenuItem3SetUp( IdGetMString(7,gLanguage), &I16164_Apply, &fhSettingMenu_main );
				PPMenuItem4SetUp( IdGetMString(8,gLanguage), &I16164_Apply, &fhSettingMenu_main );
				scrSetNEXT(&fhDeviceMode_popup);
			#endif						
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




FRAME_HEADER fhMapMode_navgation = {
	MsgMapMode_navigation,
	CreateMapMode_navigation,
	MsgMapMode_navigationDefaultBtn,
	0,		///no option
	0,
	0
};



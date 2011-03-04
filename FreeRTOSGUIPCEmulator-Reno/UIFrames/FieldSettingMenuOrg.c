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
#include "FieldSettingMenu.h"
#include "FieldDataMode.h"
#include "FieldWatchMode.h"
#include "FieldMapMode.h"
#include "FieldInfoMode.h"
#else
#include "fonts.h"
#include "icons.h"
#include "fields.h"
#endif
#include "StringID.h"


WORD MsgDefaultBtn_settings(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);
    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);

        case ID_BUTTON_BACK:
             if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);

        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
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
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			}
            return (0);
			
        case ID_BTN_DOWN_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///Navigation mode, setting, avtivity data, quick info, history
				popupOption.PopItemNum = 3;
				popupOption.pPopTitle = "Options";
				popupOption.pPrivFrame = scrGetStat();
				popupOption.pPopItemList->pPopItem1->pPopMsg = "Navigation";
				popupOption.pPopItemList->pPopItem1->pIcon = &I16164_Compass;
				popupOption.pPopItemList->pPopItem1->pGoFrame = &fhMapMode_navgation;
				popupOption.pPopItemList->pPopItem2->pPopMsg = "Quick Info";
				popupOption.pPopItemList->pPopItem2->pIcon = &I16164_About;
				popupOption.pPopItemList->pPopItem2->pGoFrame = &fhInfoMode_info;
				popupOption.pPopItemList->pPopItem3->pPopMsg = "Activity Data";
				popupOption.pPopItemList->pPopItem3->pIcon = &PCGaming1_4bpp_16x16;
				popupOption.pPopItemList->pPopItem3->pGoFrame = &fhDataMode_two;
			}
            return (0);
			
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{	
			}
            return (0);
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);                             // do nothing since it's HW key simulate
	
        default:
            return (1);                 // process by default
    }
}




void CreateSettingMenu_main(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)IdGetMString(13,gLanguage),
            alt3Scheme
        );                                          // use alternate scheme

	AddItemList( (XCHAR *)IdGetMString(10,gLanguage), pLb, &PCGaming2_1bpp_16x16);
	AddItemList( (XCHAR *)IdGetMString(11,gLanguage), pLb, &Settings_4bpp_16x16);
	AddItemList( (XCHAR *)IdGetMString(12,gLanguage), pLb, &PCGaming1_1bpp_16x16);
	AddItemList( (XCHAR *)IdGetMString(5,gLanguage), pLb, &I16164_Compass);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_main(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_mainDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_EXIT:
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{	///back to last info mode, map mode, data mode
				///how ???
				scrSetStat( getLastView() );
				scrCreateInit();
			}
            return (0);
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_user);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_device);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_activity);
						break;
					case 4:
						scrSetStat(&fhSettingMenu_navigation);
						break;
					default:
						scrSetStat(&fhSettingMenu_user);
						break;
				}
				scrCreateInit();
			}
			return (0); 
        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
    }
}

#if  0
void CreateSettingMenu_user(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<User Setting>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Physical Info", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Contact Info", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Emergency Info", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_user(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_userDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
#if 0
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
#endif
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_physicalinfo);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_contactinfo);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_physicalinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_main);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}
#endif

void CreateSettingMenu_physicalinfo(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Physical Info>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_physicalinfo(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_physicalinfoDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_user);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_contactinfo(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Contact Info>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_contactinfo(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_contactinfoDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_user);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_emergencyinfo(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Emergency Info>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_emergencyinfo(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_emergencyinfoDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_user);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_device(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)IdGetMString(11,gLanguage),
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)IdGetMString(31,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(34,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(35,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(40,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(46,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(48,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(54,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(59,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(60,gLanguage), pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_device(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_deviceDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_gps);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_language);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_datetime);
						break;
					case 4:
						scrSetStat(&fhSettingMenu_unitsofmeasure);
						break;
					case 5:
						scrSetStat(&fhSettingMenu_display);
						break;
					case 6:
						scrSetStat(&fhSettingMenu_recording);
						break;
					case 7:
						scrSetStat(&fhSettingMenu_feedback);
						break;
					case 8:
						scrSetStat(&fhSettingMenu_equipment2);
						break;
					case 9:
						scrSetStat(&fhSettingMenu_deviceinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_deviceinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_main);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}

#if 0
void CreateSettingMenu_gps(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<GPS>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Enable/Disable", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Satellite", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_gps(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_gpsDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_device);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}



void CreateSettingMenu_language(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Language>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_language(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_languageDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_device);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_datetime(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Date & Time>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Date Format", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Time Format", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Time Zone", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Daylight Saving Time", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_datetime(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_datetimeDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 4:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_device);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_unitsofmeasure(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Date & Time>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Distance", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Elevation", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Temperature", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Heart Rate", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Power", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_unitsofmeasure(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_unitsofmeasureDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 4:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 5:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_device);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_display(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Display>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Backlight", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_display(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_displayDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_device);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_recording(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Recording>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Start ctivity Reminder", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Data Recording Frequency", pLb, &I16164_About);
	AddItemList( (XCHAR *)"POWER Averaging Recording", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Cadence Averaging Recording", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Auto Power Down", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_recording(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_recordingDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 4:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 5:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_device);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_feedback(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Feedback>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Physical Button Presses", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Digital Button Presses", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Notifications", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Alerts", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_feedback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_feedbackDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 4:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_device);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}
#endif



void CreateSettingMenu_equipment2(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Equipment>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_equipment2(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_equipment2DefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_device);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_deviceinfo(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Device Info>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_deviceinfo(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_deviceinfoDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_device);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}




void CreateSettingMenu_activity(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Activity Setting>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Current Activity Profile", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Activity Profile Settings >", pLb, &I16164_About);

	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_activity(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_activityDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_curactivityprofi);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_activityprofiset);
						break;
					default:
						scrSetStat(&fhSettingMenu_curactivityprofi);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_main);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_curactivityprofi(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Current Activity Profile>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_curactivityprofi(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_curactivityprofiDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_activity);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_activityprofiset(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Activity Profile Setting>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Multi-Sport Setup", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Profile Name", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Activity Type", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Speed/Pace Preference", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Recording Preferences >", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Data Screens  >", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Training Zones >", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Training Alerts  >", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Equipment", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_activityprofiset(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_activityprofisetDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_multisportsetup);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_profilename);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_activitytype);
						break;
					case 4:
						scrSetStat(&fhSettingMenu_speedpacepref);
						break;
					case 5:
						scrSetStat(&fhSettingMenu_recordpref);
						break;
					case 6:
						scrSetStat(&fhSettingMenu_datascreens);
						break;
					case 7:
						scrSetStat(&fhSettingMenu_trainingzones);
						break;
					case 8:
						scrSetStat(&fhSettingMenu_trainingalerts);
						break;
					case 9:
						scrSetStat(&fhSettingMenu_equipment);						
						break;
					default:
						scrSetStat(&fhSettingMenu_equipment);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_main);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}

void CreateSettingMenu_multisportsetup(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Multi-sport Setup>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_multisportsetup(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_multisportsetupDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_activityprofiset);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_profilename(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Profile Name>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_profilename(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_profilenameDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_activityprofiset);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_activitytype(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Activity Type>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_activitytype(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_activitytypeDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_activityprofiset);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_speedpacepref(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Speed/Pace Preference>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_speedpacepref(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_speedpaceprefDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_activityprofiset);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_recordpref(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Recording Preference>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Auto Lap", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Auto Pause", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_recordpref(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_recordprefDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_activityprofiset);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}





void CreateSettingMenu_datascreens(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Data Screens>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Auto Scroll", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Activity Data Screen", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Activity Pacer Screen", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_datascreens(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_datascreensDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_activityprofiset);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_trainingzones(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Training Zones>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Heart Rate Zones", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Power Zones", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_trainingzones(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_trainingzonesDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_activityprofiset);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_trainingalerts(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Training Alerts>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Calories Alert", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Distance Alert", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Time Alert", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Spped/Pace Alert", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Heart Rate Alert", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Cadence Alert", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Power Alert", pLb, &I16164_About);

	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_trainingalerts(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_trainingalertsDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 3:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 4:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 5:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 6:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					case 7:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
					default:
						scrSetStat(&fhSettingMenu_emergencyinfo);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_activityprofiset);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}




void CreateSettingMenu_equipment(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Equipment>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}



WORD MsgSettingMenu_equipment(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_equipmentDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_activityprofiset);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}



void CreateSettingMenu_navigation(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();

	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)"<<Navigation Setting>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Map & Breadcrumb Screen", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Navigate to Location", pLb, &I16164_About);
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_navigation(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

            // The message was processed
            return (0);

        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_navigationDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( 1 == sFocusedItem )
					LbSetFocusedItem( pLb, (sLbCount - 1));
				else
					LbSetFocusedItem( pLb, (sFocusedItem - 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);
				if( (sLbCount - 1) == sFocusedItem )
					LbSetFocusedItem( pLb, 1);
				else
					LbSetFocusedItem( pLb, (sFocusedItem + 1));
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetStat(&fhSettingMenu_mapbreadcrumbscreen);
						break;
					case 2:
						scrSetStat(&fhSettingMenu_navtoloc);
						break;
					default:
						scrSetStat(&fhSettingMenu_mapbreadcrumbscreen);
						break;
				}
				scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_main);
				scrCreateInit();
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_mapbreadcrumbscreen(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Map & Breadcrumb Screen>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_mapbreadcrumbscreen(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_mapbreadcrumbscreenDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_navigation);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_navtoloc(WORD wDrawOption)
{

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

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
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)"<Navigate to Location>",         // text
        alt3Scheme
    );                              // default GOL scheme	
}


WORD MsgSettingMenu_navtoloc(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgSettingMenu_navtolocDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	
	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0);  	
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_ENTER:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhSettingMenu_navigation);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


/*
*/
FRAME_HEADER fhSettingMenu_main = {
	MsgSettingMenu_main,
	CreateSettingMenu_main,
	MsgSettingMenu_mainDefaultBtn,
	0,		///no option
	0,
	0
};


#if 1
/*
*/
COMMONMENU_HEADER mhCommonMenu_user = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    10,
	NULL, 
    NULL,
	1,
	3,
	{ 28, NULL, &I16164_Apply, &fhSettingMenu_physicalinfo },
	{ 29, NULL, &I16164_Apply, &fhSettingMenu_contactinfo },
	{ 30, NULL, &I16164_Apply, &fhSettingMenu_emergencyinfo },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_user = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_user
};

/*
*/
FRAME_HEADER fhSettingMenu_physicalinfo = {
	MsgSettingMenu_physicalinfo,
	CreateSettingMenu_physicalinfo,
	MsgSettingMenu_physicalinfoDefaultBtn,
	0,		///no option
	0,
	0
};

/*
*/
FRAME_HEADER fhSettingMenu_contactinfo = {
	MsgSettingMenu_contactinfo,
	CreateSettingMenu_contactinfo,
	MsgSettingMenu_contactinfoDefaultBtn,
	0,		///no option
	0,
	0
};

/*
*/
FRAME_HEADER fhSettingMenu_emergencyinfo = {
	MsgSettingMenu_emergencyinfo,
	CreateSettingMenu_emergencyinfo,
	MsgSettingMenu_emergencyinfoDefaultBtn,
	0,		///no option
	0,
	0
};

/*
*/
FRAME_HEADER fhSettingMenu_device = {
	MsgSettingMenu_device,
	CreateSettingMenu_device,
	MsgSettingMenu_deviceDefaultBtn,
	0,		///no option
	0,
	0
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_gps = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    31,
	NULL, 
    NULL,
	1,
	2,
	{ 32, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 33, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_gps = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_gps
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_language = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    34,
	NULL, 
    NULL,
	1,
	1,
	{ 34, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_language = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_language
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_datetime = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    35,
	NULL, 
    NULL,
	1,
	4,
	{ 36, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 37, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 38, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 39, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_datetime = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_datetime
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_unitsofmeasure = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    40,
	NULL, 
    NULL,
	1,
	5,
	{ 41, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 42, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 43, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 44, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 45, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_unitsofmeasure = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_unitsofmeasure
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_display = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    46,
	NULL, 
    NULL,
	1,
	1,
	{ 47, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_display = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_display
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_recording = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    48,
	NULL, 
    NULL,
	1,
	5,
	{ 49, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 50, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 51, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 52, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 53, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_recording = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_recording
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_feedback = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    54,
	NULL, 
    NULL,
	1,
	4,
	{ 55, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 56, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 57, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 58, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_feedback = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_user
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_equipment2 = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    59,
	NULL, 
    NULL,
	1,
	1,
	{ 59, NULL, &I16164_Apply, &fhSettingMenu_equipment },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_equipment2 = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_equipment2
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_deviceinfo = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    60,
	NULL, 
    NULL,
	1,
	1,
	{ 60, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_deviceinfo = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_deviceinfo
};

/*
*/
FRAME_HEADER fhSettingMenu_equipment = {
	MsgSettingMenu_equipment,
	CreateSettingMenu_equipment,
	MsgSettingMenu_equipmentDefaultBtn,
	0,		///no option
	NULL,
	NULL
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_activity = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    12,
	NULL, 
    NULL,
	1,
	1,
	{ 61, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 62, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_activity = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_activity
};

FRAME_HEADER fhSettingMenu_navigation = {
	MsgSettingMenu_navigation,
	CreateSettingMenu_navigation,
	MsgSettingMenu_navigationDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_mapbreadcrumbscreen = {
	MsgSettingMenu_mapbreadcrumbscreen,
	CreateSettingMenu_mapbreadcrumbscreen,
	MsgSettingMenu_mapbreadcrumbscreenDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_navtoloc = {
	MsgSettingMenu_navtoloc,
	CreateSettingMenu_navtoloc,
	MsgSettingMenu_navtolocDefaultBtn,
	0,		///no option
	0,
	0
};

#else

FRAME_HEADER fhSettingMenu_user = {
	MsgSettingMenu_user,
	CreateSettingMenu_user,
	MsgSettingMenu_userDefaultBtn,
	0,		///no option
	0,
	0
};



FRAME_HEADER fhSettingMenu_gps = {
	MsgSettingMenu_gps,
	CreateSettingMenu_gps,
	MsgSettingMenu_gpsDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_language = {
	MsgSettingMenu_language,
	CreateSettingMenu_language,
	MsgSettingMenu_languageDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_datetime = {
	MsgSettingMenu_datetime,
	CreateSettingMenu_datetime,
	MsgSettingMenu_datetimeDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_unitsofmeasure = {
	MsgSettingMenu_unitsofmeasure,
	CreateSettingMenu_unitsofmeasure,
	MsgSettingMenu_unitsofmeasureDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_display = {
	MsgSettingMenu_display,
	CreateSettingMenu_display,
	MsgSettingMenu_displayDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_recording = {
	MsgSettingMenu_recording,
	CreateSettingMenu_recording,
	MsgSettingMenu_recordingDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_feedback = {
	MsgSettingMenu_feedback,
	CreateSettingMenu_feedback,
	MsgSettingMenu_feedbackDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_equipment2 = {
	MsgSettingMenu_equipment2,
	CreateSettingMenu_equipment2,
	MsgSettingMenu_equipment2DefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_deviceinfo = {
	MsgSettingMenu_deviceinfo,
	CreateSettingMenu_deviceinfo,
	MsgSettingMenu_deviceinfoDefaultBtn,
	0,		///no option
	0,
	0
};




FRAME_HEADER fhSettingMenu_activity = {
	MsgSettingMenu_activity,
	CreateSettingMenu_activity,
	MsgSettingMenu_activityDefaultBtn,
	0,		///no option
	0,
	0
};


FRAME_HEADER fhSettingMenu_curactivityprofi = {
	MsgSettingMenu_curactivityprofi,
	CreateSettingMenu_curactivityprofi,
	MsgSettingMenu_curactivityprofiDefaultBtn,
	0,		///no option
	0,
	0
};


FRAME_HEADER fhSettingMenu_activityprofiset = {
	MsgSettingMenu_activityprofiset,
	CreateSettingMenu_activityprofiset,
	MsgSettingMenu_activityprofisetDefaultBtn,
	0,		///no option
	0,
	0
};


FRAME_HEADER fhSettingMenu_multisportsetup = {
	MsgSettingMenu_multisportsetup,
	CreateSettingMenu_multisportsetup,
	MsgSettingMenu_multisportsetupDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_profilename = {
	MsgSettingMenu_profilename,
	CreateSettingMenu_profilename,
	MsgSettingMenu_profilenameDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_activitytype = {
	MsgSettingMenu_activitytype,
	CreateSettingMenu_activitytype,
	MsgSettingMenu_activitytypeDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_speedpacepref = {
	MsgSettingMenu_speedpacepref,
	CreateSettingMenu_speedpacepref,
	MsgSettingMenu_speedpaceprefDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_recordpref = {
	MsgSettingMenu_recordpref,
	CreateSettingMenu_recordpref,
	MsgSettingMenu_recordprefDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_datascreens = {
	MsgSettingMenu_datascreens,
	CreateSettingMenu_datascreens,
	MsgSettingMenu_datascreensDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_trainingzones = {
	MsgSettingMenu_trainingzones,
	CreateSettingMenu_trainingzones,
	MsgSettingMenu_trainingzonesDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_trainingalerts = {
	MsgSettingMenu_trainingalerts,
	CreateSettingMenu_trainingalerts,
	MsgSettingMenu_trainingalertsDefaultBtn,
	0,		///no option
	0,
	0
};




#endif
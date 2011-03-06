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
				{	///setup for fhDeviceMode_popask
					PPAskSetUp( 2, Ask02Str, scrGetStat() );
					PPAskItem1SetUp( Ask03Str, NULL, &fhDeviceMode_poweroff );
					PPAskItem2SetUp( Ask04Str, NULL, scrGetStat() );
				}
				///PPMenuItem2SetUp( IdGetMString(3,gLanguage), &I16164_Abort, &fhDeviceMode_poweroff );
				PPMenuItem2SetUp( IdGetMString(3,gLanguage), &I16164_Abort, &fhDeviceMode_popask );
				scrSetNEXT(&fhDeviceMode_popup);
		#endif
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
		#if 0
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
		#else
				PPMenuSetUp( 4, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(9,gLanguage), &PCGaming1_4bpp_16x16, &fhDataMode_two );
				PPMenuItem2SetUp( IdGetMString(5,gLanguage), &I16164_Compass, &fhMapMode_navgation );
				PPMenuItem3SetUp( IdGetMString(6,gLanguage), &I16164_About, &fhInfoMode_info );
				PPMenuItem4SetUp( IdGetMString(8,gLanguage), &I16164_Apply, &fhHistoryMenu_main );
				scrSetNEXT(&fhDeviceMode_popup);
		#endif
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
            popupMenuScheme
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
				scrSetNEXT( getLastView() );
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
						scrSetNEXT( &fhSettingMenu_user );
						break;
					case 2:
						scrSetNEXT( &fhSettingMenu_device );
						break;
					case 3:
						scrSetNEXT( &fhSettingMenu_activity );
						break;
					case 4:
						scrSetNEXT( &fhSettingMenu_navigation );
						break;
					default:
						scrSetNEXT( &fhSettingMenu_user );
						break;
				}
			}
			return (0); 
        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
    }
}


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
		(XCHAR *)IdGetMString(28,gLanguage),         // text
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
				scrSetNEXT( &fhSettingMenu_user );
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
		(XCHAR *)IdGetMString(29,gLanguage),         // text
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
				scrSetNEXT( &fhSettingMenu_user );
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
		(XCHAR *)IdGetMString(30,gLanguage),         // text
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
				scrSetNEXT( &fhSettingMenu_user );
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
            popupMenuScheme
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
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetNEXT( &fhSettingMenu_gps );
						break;
					case 2:
						scrSetNEXT( &fhSettingMenu_language );
						break;
					case 3:
						scrSetNEXT( &fhSettingMenu_datetime );
						break;
					case 4:
						scrSetNEXT( &fhSettingMenu_unitsofmeasure );
						break;
					case 5:
						scrSetNEXT( &fhSettingMenu_display );
						break;
					case 6:
						scrSetNEXT( &fhSettingMenu_recording );
						break;
					case 7:
						scrSetNEXT( &fhSettingMenu_feedback );
						break;
					case 8:
						scrSetNEXT( &fhSettingMenu_equipment2 );
						break;
					case 9:
						scrSetNEXT( &fhSettingMenu_deviceinfo );
						break;
					default:
						scrSetNEXT( &fhSettingMenu_deviceinfo );
						break;
				}
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetNEXT( &fhSettingMenu_main );
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}



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
		(XCHAR *)IdGetMString(59,gLanguage),         // text
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
				scrSetNEXT( &fhSettingMenu_device );
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
		(XCHAR *)IdGetMString(59,gLanguage),         // text
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
				scrSetNEXT( &fhSettingMenu_main );
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
		(XCHAR *)IdGetMString(60,gLanguage),         // text
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
				scrSetNEXT( &fhSettingMenu_device );
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateSettingMenu_ASactivityprofilesettings(WORD wDrawOption)
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
            (XCHAR*)IdGetMString(5,gLanguage),
            popupMenuScheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)IdGetMString(92,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(91,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(19,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(90,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(89,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(85,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(81,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(78,gLanguage), pLb, &I16164_About);
	AddItemList( (XCHAR *)IdGetMString(59,gLanguage), pLb, &I16164_About);

	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_ASactivityprofilesettings(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
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

WORD MsgSettingMenu_ASactivityprofilesettingsDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
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
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						scrSetNEXT( &fhSettingMenu_multisportsetup );
						break;
					case 2:
						scrSetNEXT( &fhSettingMenu_profilename );
						break;
					case 3:
						scrSetNEXT( &fhSettingMenu_activitytype );
						break;
					case 4:
						scrSetNEXT( &fhSettingMenu_speedpacepref );
						break;
					case 5:
						scrSetNEXT( &fhSettingMenu_ASAPSrecordingpref );
						break;
					case 6:
						scrSetNEXT( &fhSettingMenu_ASAPSdatascreens );
						break;
					case 7:
						scrSetNEXT( &fhSettingMenu_ASAPStrainingzone );
						break;
					case 8:
						scrSetNEXT( &fhSettingMenu_ASAPStrainingalerts );
						break;
					case 9:
						scrSetNEXT( &fhSettingMenu_equipment );
						break;
					default:
						scrSetNEXT( &fhSettingMenu_main );
						break;
				}
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetNEXT( &fhSettingMenu_main );
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}






const XCHAR ADS01Str[] = {	0x0041, 0x0064, 0x0064, 0x0020, 0x0053, 0x0063, 0x0072, 0x0065, 0x0065, 0x006E,// Add Screen
							0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,// 
							0x0000};  
const XCHAR ADS02Str[] = {	0x004E, 0x0065, 0x0077, 0x0020, 0x0053, 0x0063, 0x0072, 0x0065, 0x0065, 0x006E,// New Screen
							0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,// 
							0x0000};  
const XCHAR ADS03Str[] = {	0x0043, 0x006F, 0x0070, 0x0079, 0x0020, 0x004C, 0x0061, 0x0073, 0x0074, 0x0020,// Copy Last 
							0x0053, 0x0063, 0x0072, 0x0065, 0x0065, 0x006E, 0x0000, 0x0000, 0x0000, 0x0000,// Screen
							0x0000};  









void CreateSettingMenu_SAPSDSactivitydatascreens(WORD wDrawOption)
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
            (XCHAR*)IdGetMString(83,gLanguage),
            popupMenuScheme
        );                                          // use alternate scheme
	
	if( 0 )
	{	///depends on what we had before...
	AddItemList( (XCHAR *)IdGetMString(89,gLanguage), pLb, NULL);
	AddItemList( (XCHAR *)IdGetMString(85,gLanguage), pLb, NULL);
	AddItemList( (XCHAR *)IdGetMString(81,gLanguage), pLb, NULL);
	AddItemList( (XCHAR *)IdGetMString(78,gLanguage), pLb, NULL);
	}
	AddItemList( (XCHAR *)ADS01Str, pLb, &I16164_About);

	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_SAPSDSactivitydatascreens(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
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

WORD MsgSettingMenu_SAPSDSactivitydatascreensDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
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
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				
				if( sFocusedItem == (sLbCount-1) )
				{
						///this is the last one, "add screen"
						scrSetNEXT( &fhSettingMenu_SAPSDSADSaddscreen );
				}
				else
				{
				switch(sFocusedItem)
				{
					case 9:
						scrSetNEXT( &fhSettingMenu_ASAPSDSactivitydatascreens );
						break;
					default:
						scrSetNEXT( &fhSettingMenu_ASAPSDSactivitydatascreens );
						break;
				}
				}
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetNEXT( &fhSettingMenu_main );
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}

void CreateSettingMenu_SAPSDSADSaddscreen(WORD wDrawOption)
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
            (XCHAR*)ADS01Str,
            popupMenuScheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)ADS02Str, pLb, &I16164_About);
	if( 0 )
	{	///depends on what we had before...
		///copy last screen
	AddItemList( (XCHAR *)IdGetMString(89,gLanguage), pLb, NULL);

	}
	LbSetFocusedItem( pLb, 1 );

}


WORD MsgSettingMenu_SAPSDSADSaddscreen(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
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

WORD MsgSettingMenu_SAPSDSADSaddscreenDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
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
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:	///new screen
						scrSetNEXT( &fhSettingMenu_SAPSDSADSaddscreenX );
						break;
					case 2:	///copy last screen
						scrSetNEXT( &fhSettingMenu_ASAPSDSactivitydatascreens );
						break;
					default:
						scrSetNEXT( &fhSettingMenu_ASAPSDSactivitydatascreens );
						break;
				}
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetNEXT( &fhSettingMenu_main );
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}

void CreateSettingMenu_SAPSDSADSaddscreenX(WORD wDrawOption)
{
    LISTBOX *pLb;

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
        ///GetMaxY() - DEFAULTBTN_HEIGHT,                  // dimension
		40,
        WND_DRAW,                   // will be dislayed after creation
        (void *) &I16164_About,         // icon
///        pText,                      // set text
		(XCHAR *)ADS01Str,         // text
        popupMenuScheme
    ); 

	CreateDataSet( 0,  40, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, NULL, Func01Str, Data01Str, Unit01Str);
	
}


WORD MsgSettingMenu_SAPSDSADSaddscreenX(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
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

WORD MsgSettingMenu_SAPSDSADSaddscreenXDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
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
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				SHORT sLbCount, sFocusedItem;
				sLbCount = LbGetCount(pLb);
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:	///new screen
						scrSetNEXT( &fhSettingMenu_main );
						break;
					case 2:	///copy last screen
						scrSetNEXT( &fhSettingMenu_main );
						break;
					default:
						scrSetNEXT( &fhSettingMenu_main );
						break;
				}
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetNEXT( &fhSettingMenu_main );
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
		(XCHAR *)IdGetMString(92,gLanguage),         // text
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
				scrSetStat(&fhSettingMenu_ASactivityprofilesettings);
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
		(XCHAR *)IdGetMString(91,gLanguage),         // text
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
				scrSetStat(&fhSettingMenu_ASactivityprofilesettings);
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
		(XCHAR *)IdGetMString(19,gLanguage),         // text
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
				scrSetStat(&fhSettingMenu_ASactivityprofilesettings);
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
		(XCHAR *)IdGetMString(90,gLanguage),         // text
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
				scrSetStat(&fhSettingMenu_ASactivityprofilesettings);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn_settings(objMsg, pObj, pMsg));                 // process by default
    }
}

#if 0
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
		(XCHAR *)IdGetMString(59,gLanguage),         // text
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
#endif

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
		(XCHAR *)IdGetMString(70,gLanguage),         // text
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
				//scrSetStat(&fhSettingMenu_navigation);
				//scrCreateInit();
				scrSetNEXT( &fhSettingMenu_navigation );
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
		(XCHAR *)IdGetMString(69,gLanguage),         // text
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
				scrSetNEXT( &fhSettingMenu_navigation );
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
	2,
	{ 61, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 62, NULL, &I16164_Apply, &fhSettingMenu_ASactivityprofilesettings },
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

/*
*/
FRAME_HEADER fhSettingMenu_ASactivityprofilesettings = {
	MsgSettingMenu_ASactivityprofilesettings,
	CreateSettingMenu_ASactivityprofilesettings,
	MsgSettingMenu_ASactivityprofilesettingsDefaultBtn,
	0,		///no option
	NULL,
	NULL
};

/*
*/
FRAME_HEADER fhSettingMenu_multisportsetup = {
	MsgSettingMenu_multisportsetup,
	CreateSettingMenu_multisportsetup,
	MsgSettingMenu_multisportsetupDefaultBtn,
	0,		///no option
	0,
	0
};

/*
*/
FRAME_HEADER fhSettingMenu_profilename = {
	MsgSettingMenu_profilename,
	CreateSettingMenu_profilename,
	MsgSettingMenu_profilenameDefaultBtn,
	0,		///no option
	0,
	0
};

/*
*/
FRAME_HEADER fhSettingMenu_activitytype = {
	MsgSettingMenu_activitytype,
	CreateSettingMenu_activitytype,
	MsgSettingMenu_activitytypeDefaultBtn,
	0,		///no option
	0,
	0
};

/*
*/
FRAME_HEADER fhSettingMenu_speedpacepref = {
	MsgSettingMenu_speedpacepref,
	CreateSettingMenu_speedpacepref,
	MsgSettingMenu_speedpaceprefDefaultBtn,
	0,		///no option
	0,
	0
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_ASAPSrecordingpref = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    89,
	NULL, 
    NULL,
	1,
	2,
	{ 87, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 88, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_ASAPSrecordingpref = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_ASAPSrecordingpref
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_ASAPSdatascreens = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    85,
	NULL, 
    NULL,
	1,
	3,
	{ 84, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 83, NULL, &I16164_Apply, &fhSettingMenu_ASAPSDSactivitydatascreens },
	{ 82, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_ASAPSdatascreens = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_ASAPSdatascreens
};

/*
*/
FRAME_HEADER fhSettingMenu_ASAPSDSactivitydatascreens = {
	MsgSettingMenu_SAPSDSactivitydatascreens,
	CreateSettingMenu_SAPSDSactivitydatascreens,
	MsgSettingMenu_SAPSDSactivitydatascreensDefaultBtn,
	0,		///no option
	NULL,
	NULL
};

/*
*/
FRAME_HEADER fhSettingMenu_SAPSDSADSaddscreen = {
	MsgSettingMenu_SAPSDSADSaddscreen,
	CreateSettingMenu_SAPSDSADSaddscreen,
	MsgSettingMenu_SAPSDSADSaddscreenDefaultBtn,
	0,		///no option
	NULL,
	NULL
};

/*
*/
FRAME_HEADER fhSettingMenu_SAPSDSADSaddscreenX = {
	MsgSettingMenu_SAPSDSADSaddscreenX,
	CreateSettingMenu_SAPSDSADSaddscreenX,
	MsgSettingMenu_SAPSDSADSaddscreenXDefaultBtn,
	0,		///no option
	NULL,
	NULL
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_ASAPStrainingzone = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    81,
	NULL, 
    NULL,
	1,
	2,
	{ 80, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 79, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_ASAPStrainingzone = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_ASAPStrainingzone
};

/*
*/
COMMONMENU_HEADER mhCommonMenu_ASAPStrainingalerts = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    78,
	NULL, 
    NULL,
	1,
	6,
	{ 77, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 76, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 75, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 74, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 73, NULL, &I16164_Apply, &fhSettingMenu_main },
	{ 72, NULL, &I16164_Apply, &fhSettingMenu_main },
};

FRAME_HEADER fhSettingMenu_ASAPStrainingalerts = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_ASAPStrainingalerts
};

/*
*/
#if 0
FRAME_HEADER fhSettingMenu_equipment = {
	MsgSettingMenu_equipment,
	CreateSettingMenu_equipment,
	MsgSettingMenu_equipmentDefaultBtn,
	0,		///no option
	NULL,
	NULL
};
#endif

/*
*/
COMMONMENU_HEADER mhCommonMenu_navigation = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    5,
	NULL, 
    NULL,
	1,
	2,
	{ 70, NULL, &I16164_Apply, &fhSettingMenu_mapbreadcrumbscreen },
	{ 69, NULL, &I16164_Apply, &fhSettingMenu_navtoloc },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhSettingMenu_navigation = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_navigation
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
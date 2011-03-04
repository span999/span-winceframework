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
				//scrSetStat( getLastView() );
				//scrCreateInit();
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
						//scrSetStat(&fhSettingMenu_user);
						scrSetNEXT( &fhSettingMenu_user );
						break;
					case 2:
						//scrSetStat(&fhSettingMenu_device);
						scrSetNEXT( &fhSettingMenu_device );
						break;
					case 3:
						//scrSetStat(&fhSettingMenu_activity);
						scrSetNEXT( &fhSettingMenu_activity );
						break;
					case 4:
						//scrSetStat(&fhSettingMenu_navigation);
						scrSetNEXT( &fhSettingMenu_navigation );
						break;
					default:
						//scrSetStat(&fhSettingMenu_user);
						scrSetNEXT( &fhSettingMenu_user );
						break;
				}
				//scrCreateInit();
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
				//scrSetStat(&fhSettingMenu_user);
				//scrCreateInit();
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
				//scrSetStat(&fhSettingMenu_user);
				//scrCreateInit();
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
				//scrSetStat(&fhSettingMenu_user);
				//scrCreateInit();
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
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						//scrSetStat(&fhSettingMenu_gps);
						scrSetNEXT( &fhSettingMenu_gps );
						break;
					case 2:
						//scrSetStat(&fhSettingMenu_language);
						scrSetNEXT( &fhSettingMenu_language );
						break;
					case 3:
						//scrSetStat(&fhSettingMenu_datetime);
						scrSetNEXT( &fhSettingMenu_datetime );
						break;
					case 4:
						//scrSetStat(&fhSettingMenu_unitsofmeasure);
						scrSetNEXT( &fhSettingMenu_unitsofmeasure );
						break;
					case 5:
						//scrSetStat(&fhSettingMenu_display);
						scrSetNEXT( &fhSettingMenu_display );
						break;
					case 6:
						//scrSetStat(&fhSettingMenu_recording);
						scrSetNEXT( &fhSettingMenu_recording );
						break;
					case 7:
						//scrSetStat(&fhSettingMenu_feedback);
						scrSetNEXT( &fhSettingMenu_feedback );
						break;
					case 8:
						//scrSetStat(&fhSettingMenu_equipment2);
						scrSetNEXT( &fhSettingMenu_equipment2 );
						break;
					case 9:
						//scrSetStat(&fhSettingMenu_deviceinfo);
						scrSetNEXT( &fhSettingMenu_deviceinfo );
						break;
					default:
						//scrSetStat(&fhSettingMenu_deviceinfo);
						scrSetNEXT( &fhSettingMenu_deviceinfo );
						break;
				}
				//scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				//scrSetStat(&fhSettingMenu_main);
				//scrCreateInit();
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
				//scrSetStat(&fhSettingMenu_device);
				//scrCreateInit();
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
				//scrSetStat(&fhSettingMenu_main);
				//scrCreateInit();
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
				//scrSetStat(&fhSettingMenu_device);
				//scrCreateInit();
				scrSetNEXT( &fhSettingMenu_device );
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
            popupMenuScheme
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
			#if 1
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			
				
				switch(sFocusedItem)
				{
					case 1:
						//scrSetStat(&fhSettingMenu_mapbreadcrumbscreen);
						scrSetNEXT( &fhSettingMenu_mapbreadcrumbscreen );
						break;
					case 2:
						//scrSetStat(&fhSettingMenu_navtoloc);
						scrSetNEXT( &fhSettingMenu_navtoloc );
						break;
					default:
						//scrSetStat(&fhSettingMenu_mapbreadcrumbscreen);
						scrSetNEXT( &fhSettingMenu_mapbreadcrumbscreen );
						break;
				}
				//scrCreateInit();
			#else	
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			#endif
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				//scrSetStat(&fhSettingMenu_main);
				//scrCreateInit();
				scrSetNEXT( &fhSettingMenu_main );
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
				//scrSetStat(&fhSettingMenu_navigation);
				//scrCreateInit();
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


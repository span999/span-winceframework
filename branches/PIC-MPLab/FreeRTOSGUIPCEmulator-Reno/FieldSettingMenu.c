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
#include "FieldSettingMenu.h"




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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
            (XCHAR*)"<<Setting Menu>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"User >", pLb, &PCGaming2_1bpp_16x16);
	AddItemList( (XCHAR *)"Device >", pLb, &Settings_4bpp_16x16);
	AddItemList( (XCHAR *)"Activity >", pLb, &PCGaming1_1bpp_16x16);
	AddItemList( (XCHAR *)"Navigation >", pLb, &I16164_Compass);
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
///				LbChangeSel(pLb, pLb->pFocusItem);
///				SetState(pLb, LB_DRAW_ITEMS);
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
				scrSetStat(&fhSettingMenu_user);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
            (XCHAR*)"<<Device Setting>>",
            alt3Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"GPS >", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Language", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Date & Time >", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Units of Measurement >", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Dispaly >", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Recording >", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Feedback >", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Equipment", pLb, &I16164_About);
	AddItemList( (XCHAR *)"Device Info", pLb, &I16164_About);
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
    }
}


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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
				scrSetStat(&fhSettingMenu_device);
				scrCreateInit();;
			}
			return (0); 

        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
    }
}




FRAME_HEADER fhSettingMenu_main = {
	MsgSettingMenu_main,
	CreateSettingMenu_main,
	MsgSettingMenu_mainDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_user = {
	MsgSettingMenu_user,
	CreateSettingMenu_user,
	MsgSettingMenu_userDefaultBtn,
	0,		///no option
	0,
	0
};


FRAME_HEADER fhSettingMenu_physicalinfo = {
	MsgSettingMenu_physicalinfo,
	CreateSettingMenu_physicalinfo,
	MsgSettingMenu_physicalinfoDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_contactinfo = {
	MsgSettingMenu_contactinfo,
	CreateSettingMenu_contactinfo,
	MsgSettingMenu_contactinfoDefaultBtn,
	0,		///no option
	0,
	0
};

FRAME_HEADER fhSettingMenu_emergencyinfo = {
	MsgSettingMenu_emergencyinfo,
	CreateSettingMenu_emergencyinfo,
	MsgSettingMenu_emergencyinfoDefaultBtn,
	0,		///no option
	0,
	0
};


FRAME_HEADER fhSettingMenu_device = {
	MsgSettingMenu_device,
	CreateSettingMenu_device,
	MsgSettingMenu_deviceDefaultBtn,
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

FRAME_HEADER fhSettingMenu_equipment = {
	MsgSettingMenu_equipment,
	CreateSettingMenu_equipment,
	MsgSettingMenu_equipmentDefaultBtn,
	0,		///no option
	0,
	0
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

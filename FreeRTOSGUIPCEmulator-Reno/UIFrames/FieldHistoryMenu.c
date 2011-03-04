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


WORD MsgDefaultBtn_history(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
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
				PPMenuSetUp( 4, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(9,gLanguage), &PCGaming1_4bpp_16x16, &fhDataMode_two );
				PPMenuItem2SetUp( IdGetMString(5,gLanguage), &I16164_Compass, &fhMapMode_navgation );
				PPMenuItem3SetUp( IdGetMString(6,gLanguage), &I16164_About, &fhInfoMode_info );
				PPMenuItem4SetUp( IdGetMString(7,gLanguage), &I16164_Apply, &fhSettingMenu_main );
				scrSetNEXT(&fhDeviceMode_popup);
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




void CreateHistoryMenu_main(WORD wDrawOption)
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
            (XCHAR*)IdGetMString(14,gLanguage),
            popupMenuScheme
        );                                          // use alternate scheme

	AddItemList( (XCHAR *)IdGetMString(15,gLanguage), pLb, &I16164_Apply);
	AddItemList( (XCHAR *)IdGetMString(23,gLanguage), pLb, &I16164_Apply);
	AddItemList( (XCHAR *)IdGetMString(25,gLanguage), pLb, &I16164_Apply);
	LbSetFocusedItem( pLb, 1 );
}


WORD MsgHistoryMenu_main(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
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

WORD MsgHistoryMenu_mainDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
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
						scrSetNEXT( &fhHistoryMenu_activityhistory );
						break;
					case 2:
						scrSetNEXT( &fhHistoryMenu_activitytotals );
						break;
					case 3:
						scrSetNEXT( &fhHistoryMenu_locationhistory );
						break;
					default:
						scrSetNEXT( &fhHistoryMenu_main );
						break;
				}
			}
			return (0); 
        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
    }
}


void CreateCommonMenu(WORD wDrawOption)
{
    LISTBOX *pLb;
	COMMONMENU_HEADER *pThisMenu = NULL;
	COMMONMENUITEM_HEADER *pThisItem = NULL;
	SHORT sTemp = 0;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();	

	CreateDefaultBtn();
	
	pThisMenu = (COMMONMENU_HEADER *)(scrGetStat()->pfnFrameAdvData);

    pLb = LbCreate
        (
            pThisMenu->Id,                            // ID
            pThisMenu->left,
            pThisMenu->top,
            pThisMenu->right,
            pThisMenu->bottom,                      // dimension
            pThisMenu->state,                   // will be dislayed after creation
            (XCHAR*)IdGetMString(pThisMenu->multilangId,gLanguage),
            ///pThisMenu->pScheme
			popupMenuScheme
        );                                          // use alternate scheme
	
	sTemp = 0;
	while( sTemp < pThisMenu->totalItemNum )
	{
		pThisItem = (&(pThisMenu->Item1)) + sTemp;
		AddItemList( (XCHAR *)IdGetMString(pThisItem->multilangId,gLanguage), pLb, pThisItem->pIcon);
		sTemp++;
	}

	LbSetFocusedItem( pLb, 1 );
}


WORD MsgCommonMenu(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
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

WORD MsgCommonMenu_DefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
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
				scrSetNEXT( scrGetPrivStat() );
			}
            return (0);
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				COMMONMENU_HEADER *pThisMenu = NULL;
				COMMONMENUITEM_HEADER *pThisItem = NULL;
				SHORT sFocusedItem;
				
				pThisMenu = (COMMONMENU_HEADER *)(scrGetStat()->pfnFrameAdvData);
				sFocusedItem = LbGetFocusedItem(pLb);			

				pThisItem = (&(pThisMenu->Item1)) + (sFocusedItem-1);
				scrSetNEXT(pThisItem->pGoFrame);

			}
			return (0); 
        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
    }
}




/*
	History Menu
*/
FRAME_HEADER fhHistoryMenu_main = {
	MsgHistoryMenu_main,
	CreateHistoryMenu_main,
	MsgHistoryMenu_mainDefaultBtn,
	0,		///no option
	NULL,
	NULL
};

/*
	Activity History 
*/
COMMONMENU_HEADER mhCommonMenu_activityhistory = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    15,
	NULL, 
    NULL,
	1,
	2,
	{ 16, NULL, &I16164_Apply, &fhHistoryMenu_AHview },
	{ 17, NULL, &I16164_Apply, &fhHistoryMenu_AHdelete },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhHistoryMenu_activityhistory = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_activityhistory
};

/*
	Activity History - View 
*/
COMMONMENU_HEADER mhCommonMenu_AHview = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    16,
	NULL, 
    NULL,
	1,
	2,
	{ 18, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 19, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhHistoryMenu_AHview = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_AHview
};

/*
	Activity History - Delete
*/
COMMONMENU_HEADER mhCommonMenu_AHdelete = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    17,
	NULL, 
    NULL,
	1,
	2,
	{ 18, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 19, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhHistoryMenu_AHdelete = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_AHdelete
};


/*
	Activity Totals
*/
COMMONMENU_HEADER mhCommonMenu_activitytotals = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    23,
	NULL, 
    NULL,
	1,
	2,
	{ 16, NULL, &I16164_Apply, &fhHistoryMenu_ATview },
	{ 24, NULL, &I16164_Apply, &fhHistoryMenu_ATclear },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhHistoryMenu_activitytotals = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_activitytotals
};

/*
	Activity Totals - View 
*/
COMMONMENU_HEADER mhCommonMenu_ATview = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    16,
	NULL, 
    NULL,
	1,
	2,
	{ 18, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 19, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhHistoryMenu_ATview = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_ATview
};

/*
	Activity Totals - Clear
*/
COMMONMENU_HEADER mhCommonMenu_ATclear = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    24,
	NULL, 
    NULL,
	1,
	2,
	{ 18, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 19, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhHistoryMenu_ATclear = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_ATclear
};


/*
	Location History
*/
COMMONMENU_HEADER mhCommonMenu_locationhistory = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    25,
	NULL, 
    NULL,
	1,
	2,
	{ 16, NULL, &I16164_Apply, &fhHistoryMenu_LHview },
	{ 17, NULL, &I16164_Apply, &fhHistoryMenu_LHdelete },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhHistoryMenu_locationhistory = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_locationhistory
};

/*
	Location History - View 
*/
COMMONMENU_HEADER mhCommonMenu_LHview = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    16,
	NULL, 
    NULL,
	1,
	1,
	{ 26, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhHistoryMenu_LHview = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_LHview
};

/*
	Location History - Delete 
*/
COMMONMENU_HEADER mhCommonMenu_LHdelete = {
	ID_LISTBOX1,
    0, 
    0, 
    GetMaxX(), 
    GetMaxY(), 
    (LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL), 
    17,
	NULL, 
    NULL,
	1,
	2,
	{ 26, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 27, NULL, &I16164_Apply, &fhHistoryMenu_main },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL },
	{ 0, NULL, NULL, NULL }
};

FRAME_HEADER fhHistoryMenu_LHdelete = {
	MsgCommonMenu,
	CreateCommonMenu,
	MsgCommonMenu_DefaultBtn,
	0,		///no option
	NULL,
	(void *)&mhCommonMenu_LHdelete
};

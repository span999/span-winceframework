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






FRAME_HEADER fhHistoryMenu_main = {
	MsgHistoryMenu_main,
	CreateHistoryMenu_main,
	MsgHistoryMenu_mainDefaultBtn,
	0,		///no option
	NULL,
	NULL
};

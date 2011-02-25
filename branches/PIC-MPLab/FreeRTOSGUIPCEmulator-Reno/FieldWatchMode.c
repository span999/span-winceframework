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

#ifdef USE_MAGELLAN_LOGO
#include "magellan_logo.h"
#endif


void PPMenuSetUp( SHORT	ItemNum, XCHAR *pTitle, FRAME_HEADER *pPrivF )
{
	popupOption.PopItemNum = ItemNum;
	popupOption.pPopTitle = pTitle;
	popupOption.pPrivFrame = pPrivF;
}

void PPMenuItemsSetUp( SHORT ItemNum, XCHAR *pMsg, void *pIcon, FRAME_HEADER *pFrame2Go )
{
	POPUPITEM_HEADER **ppThisMI = NULL;
	
	ppThisMI = &(popupOption.pPopItemList->pPopItem1) + (ItemNum-1);
	
	(*ppThisMI)->pPopMsg = pMsg;
	(*ppThisMI)->pIcon = pIcon;
	(*ppThisMI)->pGoFrame = pFrame2Go;
}




void CreateWatchMode_watch(WORD wDrawOption)
{
    GOLFree();   // free memory for the objects in the previous linked list and start new list
	gcCleanScreen();
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
				///fitness mode or power down
				///popupOption.PopItemNum = 2;
				///popupOption.pPopTitle = "Options";
				///popupOption.pPrivFrame = scrGetStat();
				///PPMenuSetUp( 2, "Options", scrGetStat() );
				PPMenuSetUp( 2, OptionsENStr, scrGetStat() );
				///popupOption.pPopItemList->pPopItem1->pPopMsg = "Fitness Mode";
				///popupOption.pPopItemList->pPopItem1->pIcon = &PCGaming1_4bpp_16x16;
				///popupOption.pPopItemList->pPopItem1->pGoFrame = &fhDataMode_two;
				///PPMenuItem1SetUp( "Fitness Mode", &PCGaming1_4bpp_16x16, &fhDataMode_two );
				PPMenuItem1SetUp( FitnessModeENStr, &PCGaming1_4bpp_16x16, &fhDataMode_two );
				///popupOption.pPopItemList->pPopItem2->pPopMsg = "Power Down";
				///popupOption.pPopItemList->pPopItem2->pIcon = &I16164_Abort;
				///popupOption.pPopItemList->pPopItem2->pGoFrame = &fhDeviceMode_poweroff;
				///PPMenuItem2SetUp( "Power Down", &I16164_Abort, &fhDeviceMode_poweroff );
				PPMenuItem2SetUp( PowerDownENStr, &I16164_Abort, &fhDeviceMode_poweroff );
				///scrSetStat(&fhDeviceMode_popup);
				///scrCreateInit();
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
	gcCleanScreen();
	CreateDefaultBtn();

	// draw fonts in the screen
    ///SetFont((void *) &Gentium_Normal25);
	///SetColor(LIGHTGREEN);
	width = GetTextWidth(PoweroffModeENStr, (void *) &Gentium_Normal25U);
    height = GetTextHeight((void *) &Gentium_Normal25U);
	///OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height - DEFAULTBTN_HEIGHT) >> 1, "PowerOff");
	gcColFntOutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height - DEFAULTBTN_HEIGHT) >> 1, PoweroffModeENStr, &Gentium_Normal25U, LIGHTGREEN);
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
				///scrSetStat(&fhDeviceMode_booting);
				///scrCreateInit();
				scrSetNEXT(&fhDeviceMode_booting);
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
	void	*pNowFont = NULL;

    GOLFree();   // free memory for the objects in the previous linked list and start new list
	gcCleanScreen();
	CreateDefaultBtn();

/***********************************************************************************************************/
	///SetColor(BLACK);
	///ClearDevice();

	// draw fonts in the screen
	pNowFont = (void *)&kaiu_Norma25;
	///pNowFont = (void *)&comic_Normal15;
	
	#if 0
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(MitacBrandingStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-60, MitacBrandingStr);
	#else
	width = GetTextWidth(MitacBrandingStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-60, MitacBrandingStr, pNowFont, LIGHTRED);
	#endif
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	
	
	// draw fonts in the screen
	///pNowFont = (void *)&Gentium_Normal15;
	///pNowFont = (void *)&kaiu_Normal18;
	///pNowFont = (void *)&comic_Normal15;
	pNowFont = (void *)&Gentium_Normal17U;
	///SetFont(pNowFont);
	///SetColor(LIGHTRED);
	width = GetTextWidth(BootupInfo01Str, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-20, BootupInfo01Str);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-20, BootupInfo01Str, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	///pNowFont = (void *)&Gentium_Normal15;
	///pNowFont = (void *)&comic_Normal19;
	pNowFont = (void *)&Gentium_Normal17U;
	///SetFont(pNowFont);
	///SetColor(LIGHTGREEN);
	width = GetTextWidth(BootupInfo02Str, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, BootupInfo02Str);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, BootupInfo02Str, pNowFont, LIGHTGREEN);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	///pNowFont = (void *)&Gentium_Normal15;
	///pNowFont = (void *)&comic_Normal15;
	pNowFont = (void *)&Gentium_Normal17U;
	///SetFont(pNowFont);
	///SetColor(LIGHTBLUE);
	width = GetTextWidth(BootupInfo03Str, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+20, BootupInfo03Str);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+20, BootupInfo03Str, pNowFont, LIGHTBLUE);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	if(1)
	{
	XCHAR *pxStr = NULL;
	///SetColor(BLACK);
	///ClearDevice();
	gcCleanScreen();
	
	pNowFont = (void *)&Gentium_Normal19U;
	// draw fonts in the screen
	pxStr = TestSettingENStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	///SetFont(pNowFont);
	///SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-80, pxStr);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 0, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestSettingSPStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	///SetFont(pNowFont);
	///SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-60, pxStr);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 20, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestSettingFRStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	///SetFont(pNowFont);
	///SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-40, pxStr);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 40, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestSettingITStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	///SetFont(pNowFont);
	///SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-20, pxStr);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 60, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestSettingPOStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	///SetFont(pNowFont);
	///SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+0, pxStr);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 80, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestSettingDUStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	///SetFont(pNowFont);
	///SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+20, pxStr);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 100, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestSettingGEStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	///SetFont(pNowFont);
	///SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+40, pxStr);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 120, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestSettingDAStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	///SetFont(pNowFont);
	///SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	///OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+60, pxStr);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 140, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	}

	if(1)
	{
	XCHAR *pxStr = NULL;
	///SetColor(BLACK);
	///ClearDevice();
	gcCleanScreen();
	
	// draw fonts in the screen
	pxStr = TestMenuENStr;
	pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-80, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuSPStr;
	pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-60, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuFRStr;
	pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-40, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuITStr;
	pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-20, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuPOStr;
	pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+0, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuDUStr;
	pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+20, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuGEStr;
	pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+40, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuDAStr;
	pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+60, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	}



	
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
				///scrSetStat(&fhDataMode_two);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_two);
			}
			return (0); 
        case ID_BTN_UP_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///power down
				///scrSetStat(&fhDeviceMode_poweroff);
				///scrCreateInit();
				scrSetNEXT(&fhDeviceMode_poweroff);
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///display mode change 3,2,1
				///scrSetStat(&fhDataMode_two);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_two);
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
				///scrSetStat(&fhDataMode_two);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_two);
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
				///scrSetStat(&fhDataMode_two);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_two);
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



void CreateDeviceMode_popup(WORD wDrawOption)
{
    LISTBOX *pLb;
	POPUPITEM_HEADER*	pPopItems;
	SHORT	sCount = 0;

	
    GOLFree();                                      // free memory for the objects in the previous linked list and start new list

	CreateDefaultBtn();
	
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            POPUP_FRAME_OFFSET_W,
            POPUP_FRAME_OFFSET_H,
            GetMaxX() - POPUP_FRAME_OFFSET_W,
            GetMaxY() - DEFAULTBTN_HEIGHT - POPUP_FRAME_OFFSET_H,  // dimension
            LB_DRAW | LB_FOCUSED | LB_SINGLE_SEL,                   // will be dislayed after creation
            (XCHAR*)(popupOption.pPopTitle),
            popupMenuScheme
        );                                          // use alternate scheme

	pPopItems = popupOption.pPopItemList->pPopItem1;
	while( sCount < popupOption.PopItemNum )
	{
		AddItemList( (XCHAR *)pPopItems->pPopMsg, pLb, pPopItems->pIcon);
		sCount++;
		pPopItems++;
	}

	LbSetFocusedItem( pLb, 1 );	
}



WORD MsgDeviceMode_popup(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_LISTBOX1:

            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);
            // The message was processed
            return (0);
        default:
            return 1; 							// process by default
    }
}

WORD MsgDeviceMode_popupDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	LISTBOX *pLb;
	POPUPITEM_HEADER*	pPopItems;

	
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
        case ID_BTN_UP_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///power down
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
				SHORT sFocusedItem;
				sFocusedItem = LbGetFocusedItem(pLb);			

				pPopItems = popupOption.pPopItemList->pPopItem1;
				///scrSetStat( (pPopItems+(sFocusedItem-1))->pGoFrame );			
				///scrCreateInit();
				scrSetNEXT( (pPopItems+(sFocusedItem-1))->pGoFrame );
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
				///lap, back to privious
				///scrSetStat(popupOption.pPrivFrame);
				///scrCreateInit();
				scrSetNEXT(popupOption.pPrivFrame);
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


POPUPITEM_HEADER popitem1 = 
{
	"1                   ",
	&I16164_About,
	&fhMapMode_navgation
};

POPUPITEM_HEADER popitem2 = 
{
	"2                   ",
	&I16164_About,
	&fhMapMode_navgation
};

POPUPITEM_HEADER popitem3 = 
{
	"3                   ",
	&I16164_About,
	&fhMapMode_navgation
};

POPUPITEM_HEADER popitem4 = 
{
	"4                   ",
	&I16164_About,
	&fhMapMode_navgation
};

POPUPITEM_HEADER popitem5 = 
{
	"5                   ",
	&I16164_About,
	&fhMapMode_navgation
};

POPUPITEM_HEADER popitem6 = 
{
	"6                   ",
	&I16164_About,
	&fhMapMode_navgation
};


POPUPITEMLIST_HEADER popitemlist =
{
	&popitem1,
	&popitem2,
	&popitem3,
	&popitem4,
	&popitem5,
	&popitem6,
};


POPUPOPTION_HEADER popupOption = 
{
	3,						///item number
	"t                  ",				///title
	&fhDataMode_three,		///privious frame
	&popitemlist
};




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


FRAME_HEADER fhDeviceMode_popup = {
	MsgDeviceMode_popup,
	CreateDeviceMode_popup,
	MsgDeviceMode_popupDefaultBtn,
	0,		///no option
	0,
	0
};

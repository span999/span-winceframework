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

const XCHAR Ask01Str[] = {	0x002D, 0x0050, 0x006F, 0x0077, 0x0065, 0x0072, 0x0020, 0x004F, 0x0066, 0x0066, 0x002D,// -Power Off-
							0x0000};  
const XCHAR Ask02Str[] = {	0x0041, 0x0072, 0x0065, 0x0020, 0x0079, 0x006F, 0x0075, 0x0020, 0x0073, 0x0075, 0x0072, 0x0065, 0x003F, // Are you sure?
							0x0000};  
const XCHAR Ask03Str[] = {	0x0042, 0x0061, 0x0063, 0x006B, 0x002D, 0x003E, 0x004E, 0x006F, // Back->No
							0x0000};  
const XCHAR Ask04Str[] = {	0x0045, 0x006E, 0x0074, 0x0065, 0x0072, 0x002D, 0x003E, 0x0059, 0x0065, 0x0073, // Enter->Yes
							0x0000};  


void PPAskSetUp( SHORT ItemNum, XCHAR *pTitle, FRAME_HEADER *pPrivF )
{
	popupAsk.PopItemNum = ItemNum;
	popupAsk.pPopTitle = pTitle;
	popupAsk.pPrivFrame = pPrivF;
}

void PPAskItemsSetUp( SHORT ItemNum, XCHAR *pMsg, void *pIcon, FRAME_HEADER *pFrame2Go )
{
	POPUPITEM_HEADER **ppThisMI = NULL;
	
	ppThisMI = &(popupAsk.pPopAskList->pPopItem11) + (ItemNum-1);
	
	(*ppThisMI)->pPopMsg = pMsg;
	(*ppThisMI)->pIcon = pIcon;
	(*ppThisMI)->pGoFrame = pFrame2Go;
}


void CreateTextEntryPadHook(WORD wDrawOption)
{
	///create objs for use
#define NTEXT_H	25
#define NTEXT_W	25
#define NBTN_H	25
#define NBTN_W	25

//    GOLFree();                                      // free memory for the objects in the previous linked list and start new list
//	SetColor(BLACK);
//	ClearDevice();	
//	CreateDefaultBtn();

	if( 1 == wDrawOption )
	{
		StCreate(
			ID_ENTRY_STATICTEXT,
			((GetMaxX() - (NTEXT_W)) >> 1),
			((GetMaxY() - (NTEXT_H)) >> 1),
			((GetMaxX() - (NTEXT_W)) >> 1) + NTEXT_W,
			((GetMaxY() - (NTEXT_H)) >> 1) + NTEXT_H,                      // dimension
			LB_DRAW | ST_CENTER_ALIGN | ST_FRAME | LB_FOCUSED,                   // will be dislayed after creation
			(XCHAR *)"A",
			entryPadScheme
		);
	}
	else
	{
		StCreate(
			ID_ENTRY_STATICTEXT,
			((GetMaxX() - (NTEXT_W)) >> 1),
			((GetMaxY() - (NTEXT_H)) >> 1),
			((GetMaxX() - (NTEXT_W)) >> 1) + NTEXT_W,
			((GetMaxY() - (NTEXT_H)) >> 1) + NTEXT_H,                      // dimension
			LB_DRAW | ST_CENTER_ALIGN | ST_FRAME | LB_FOCUSED,                   // will be dislayed after creation
			(XCHAR *)"0",
			entryPadScheme
		);
	}
	
    BtnCreate
    (
        ID_ENTRY_BTN_UP,                 // button ID
        ((GetMaxX() - (NTEXT_W)) >> 1),
        ((GetMaxY() - (NTEXT_H)) >> 1) - NBTN_H,
        ((GetMaxX() - (NTEXT_W)) >> 1) + NTEXT_W,
        ((GetMaxY() - (NTEXT_H)) >> 1) + NTEXT_H - NBTN_H,                      // dimension
        0,                         // set radius
        BTN_DRAW,                   // draw a beveled button
        (void *)&I16164_Top,                       // no bitmap
        (XCHAR *)NULL,         // text
        entryPadScheme
    );                              // use alternate scheme

    BtnCreate
    (
        ID_ENTRY_BTN_DOWN,                 // button ID
        ((GetMaxX() - (NTEXT_W)) >> 1),
        ((GetMaxY() - (NTEXT_H)) >> 1) + NBTN_H,
        ((GetMaxX() - (NTEXT_W)) >> 1) + NTEXT_W,
        ((GetMaxY() - (NTEXT_H)) >> 1) + NTEXT_H + NBTN_H,                      // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        (void *)&I16164_Bottom,   // use bitmap
        (XCHAR *)NULL,           // text
        popupMenuScheme
    );  
	

}

WORD MsgTextEntryPadHook(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgTextEntryPadDefaultBtnHook(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	static XCHAR sIdx = 0;
	static XCHAR scNum[2];
	STATICTEXT *pSt;
	pSt = (STATICTEXT *)GOLFindObject(ID_ENTRY_STATICTEXT);
	
	if( 0 == sIdx )
	{
		sIdx = *StGetText(pSt);
		scNum[0] = sIdx;
		scNum[1] = '\0';
	}
	
    switch(GetObjID(pObj))
    {	
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
		case ID_ENTRY_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				if( 90 == sIdx )
				{
					sIdx = 65;
				}
				else
				{
					sIdx++;
				}
				scNum[0] = sIdx;
				StSetText( pSt, scNum );
				printf("%d ",*StGetText(pSt));
				SetState(pSt, ST_DRAW);
			}
			return (0);  	
        case ID_BTN_DOWN:
		case ID_ENTRY_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				if( 65 == sIdx )
				{
					sIdx = 90;
				}
				else
				{
					sIdx--;
				}
				scNum[0] = sIdx;
				StSetText( pSt, scNum );
				printf("%d ",*StGetText(pSt));
				SetState(pSt, ST_DRAW);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///LbChangeSel(pLb, pLb->pFocusItem);
				///SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
    }
}

void CreateNumEntryPadHook(WORD wDrawOption)
{
	CreateTextEntryPadHook(wDrawOption);
}


WORD MsgNumEntryPadHook(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgNumEntryPadDefaultBtnHook(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	static XCHAR sIdx = 0;
	static XCHAR scNum[2];
	STATICTEXT *pSt;
	pSt = (STATICTEXT *)GOLFindObject(ID_ENTRY_STATICTEXT);
	
	///if( 0 == sIdx )
	if(1)
	{
		sIdx = *StGetText(pSt);
		scNum[0] = sIdx;
		scNum[1] = '\0';
	}
	
    switch(GetObjID(pObj))
    {	
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
		case ID_ENTRY_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				if( 57 == sIdx )
				{
					sIdx = 48;
				}
				else
				{
					sIdx++;
				}
				scNum[0] = sIdx;
				StSetText( pSt, scNum );
				printf("%d ",*StGetText(pSt));
				SetState(pSt, ST_DRAW);
			}
			return (0);  	
        case ID_BTN_DOWN:
		case ID_ENTRY_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				if( 48 == sIdx )
				{
					sIdx = 57;
				}
				else
				{
					sIdx--;
				}
				scNum[0] = sIdx;
				StSetText( pSt, scNum );
				printf("%d ",*StGetText(pSt));
				SetState(pSt, ST_DRAW);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///LbChangeSel(pLb, pLb->pFocusItem);
				///SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
    }
}









const XCHAR Time01Str[] = {	0x004D, 0x006F, 0x006E, 0x0064, 0x0061, 0x0079, // Monday
							0x0000};  
const XCHAR Time02Str[] = {	0x0046, 0x0065, 0x0062, 0x0020, 0x0032, 0x0038, 0x002C, 0x0020, 0x0032, 0x0030, 0x0031, 0x0031, // Feb 28, 2011
							0x0000};  
const XCHAR Time03Str[] = {	0x0030, 0x0030, 0x003A, 0x0030, 0x0030, 0x003A, 0x0030, 0x0030, // 00:00:00
							0x0000};  



void CreateWatchMode_watch(WORD wDrawOption)
{
	SHORT	width, height;

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
		(XCHAR *)IdGetMString(4,gLanguage),         // text
        windowScheme
    );                              // default GOL scheme
	
	///CreateDataSet( 0,  40, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "Watch Mode", "Time", "23:23", "24h");
	///CreateDataSet( 0,  40, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, NULL, IdGetMString(4,gLanguage), "23:23", "24h");
	
	width = GetTextWidth(Time01Str, dataSetScheme2->pFont);
	height = GetTextHeight(dataSetScheme2->pFont);
	StCreate(ID_STATICTEXT1,           	// ID 
			  5, 50, 5+width+ST_INDENT, 75,
              ST_DRAW,        	// will be dislayed, has frame
              Time01Str, // multi-line text
			  dataSetScheme2);                   	// default GOL scheme 
			  
	width = GetTextWidth(Time02Str, dataSetScheme2->pFont);
	StCreate(ID_STATICTEXT2,           	// ID 
			  5, 80, 5+width+ST_INDENT, 100,
              ST_DRAW,        	// will be dislayed, has frame
              Time02Str, // multi-line text
			  dataSetScheme2);                   	// default GOL scheme 

	width = GetTextWidth(Time03Str, dataSetScheme2->pFont);
	StCreate(ID_STATICTEXT3,           	// ID 
			  5, 105, 5+width+ST_INDENT, 125,
              ST_DRAW,        	// will be dislayed, has frame
              Time03Str, // multi-line text
			  dataSetScheme2);                   	// default GOL scheme 

}


XCHAR xString[] = { 0x0000, 0x0000, 0x003A, 0x0000, 0x0000, 0x003A, 0x0000, 0x0000, 0x0000, 0x0000, };

void UpdateWatchMode_watch(WORD wDrawOption)
{
	static WORD wMs = 0;
	static WORD wSec = 0;
	static WORD wSecO = 7;
	static WORD wMin = 0;
	static WORD wHour = 0;
		
	wMs = wMs + 100;
	///if( 1000 == wMs )
	if( 800 == wMs )
	{
		wMs = 0;	///reset
		wSec = wSec + 1;
		if( 60 == wSec )
		{
			wSec = 0;	///reset
			wMin = wMin + 1;
			if( 60 == wMin )
			{
				wMin = 0;
				wHour = wHour + 1;
				if( 24 == wHour )
					wHour = 0;
			}
		}		
	}
		
		
	if( wSecO != wSec )
	{
		STATICTEXT *pObj = NULL;
		pObj = (STATICTEXT *)GOLFindObject(ID_STATICTEXT3);

		xString[0] = Num2Xchar(wHour/10);
		xString[1] = Num2Xchar(wHour%10);		
		xString[3] = Num2Xchar(wMin/10);
		xString[4] = Num2Xchar(wMin%10);
		xString[6] = Num2Xchar(wSec/10);
		xString[7] = Num2Xchar(wSec%10);
		StSetText( pObj, xString );
		SetState( pObj, ST_DRAW );
		wSecO = wSec;
	}
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
				PPMenuSetUp( 2, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(2,gLanguage), &PCGaming1_4bpp_16x16, &fhDataMode_two );
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
    SHORT	width, height, NowY, Loop;
	void	*pNowFont = NULL;

    GOLFree();   // free memory for the objects in the previous linked list and start new list
	gcCleanScreen();
	CreateDefaultBtn();

#if 0
	// draw fonts in the screen
	///pNowFont = (void *)&Gentium_Normal25U;
	pNowFont = (void *)&Monaco_Normal30U;
	width = GetTextWidth(IdGetMString(0,EN), pNowFont);
    height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height - DEFAULTBTN_HEIGHT) >> 1, IdGetMString(0,EN), pNowFont, BLACK);
#else	
	NowY = 3;
	Loop = 0;
	pNowFont = (void *)&Monaco_Normal20U;
	
	///list down all language supported.
	for( Loop = 0; Loop < 8; Loop++ )
	{
		width = GetTextWidth(IdGetMString(0,Loop), pNowFont);
		height = GetTextHeight(pNowFont);
		gcColFntOutTextXY((GetMaxX() - width) >> 1, NowY, IdGetMString(0,Loop), pNowFont, (gLanguage==Loop)?LIGHTRED:BLACK);
		NowY = NowY + height;
	}
#endif	
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
				///change language setting test here
				gLanguage++;
				if( 8 == gLanguage )
					gLanguage = 0;
				
				scrSetNEXT(&fhDeviceMode_poweroff);
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


const XCHAR AdditionalInfo01Str[] = { 	0x0057, 0x0065, 0x0020, 0x0061, 0x0072, 0x0065, 0x0020, 0x0061, 0x0073, 0x006B, // We are ask
										0x0065, 0x0064, 0x0020, 0x0074, 0x006F, 0x0020, 0x0070, 0x0075, 0x0074, 0x0020, // ed to put 
										0x0073, 0x006F, 0x006D, 0x0065, 0x0020, 0x0061, 0x0064, 0x0064, 0x0069, 0x0074, // some addit
										0x0069, 0x006F, 0x006E, 0x0061, 0x006C, 0x0020, 0x0069, 0x006E, 0x0066, 0x006F, // ional info
										0x0028, 0x0045, 0x0055, 0x004C, 0x0041, 0x002C, 0x0077, 0x0061, 0x0072, 0x0072, // (EULA,warr
										0x0061, 0x006E, 0x0074, 0x0079, 0x002C, 0x0063, 0x006F, 0x0070, 0x0079, 0x0072, // anty,copyr
										0x0069, 0x0067, 0x0068, 0x0074, 0x0029, 0x0020, 0x0068, 0x0065, 0x0072, 0x0065, // ight) here
										0x002E, 0x002E, 0x002E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // ...
									0x0000}; 



void CreateDeviceMode_booting(WORD wDrawOption)
{
    SHORT	width, height;
	WORD 	wStrID = 0;
	void	*pNowFont = NULL;

	/*
	if( scrIsCreateDone() )
	{	
		STATICTEXT *pObj = NULL;
		
		printf("Display Booting start!!\n");
		pObj = GOLFindObject(ID_STATICTEXT1);
		StSetText( pObj, Ask01Str );
		SetState(pObj, ST_DRAW);
		return;
	}
	*/
    GOLFree();   // free memory for the objects in the previous linked list and start new list
	gcCleanScreen();
	CreateDefaultBtn();

/***********************************************************************************************************/
	///SetColor(BLACK);
	///ClearDevice();

	// draw fonts in the screen
	pNowFont = (void *)&kaiu_Norma25;
	///pNowFont = (void *)&comic_Normal15;
	width = GetTextWidth(MitacBrandingStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-60, MitacBrandingStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);
	
	// draw fonts in the screen
	///pNowFont = (void *)&Gentium_Normal15;
	///pNowFont = (void *)&kaiu_Normal18;
	///pNowFont = (void *)&comic_Normal15;
	///pNowFont = (void *)&Gentium_Normal17U;
	pNowFont = (void *)&Monaco_Normal15U;
	width = GetTextWidth(BootupInfo01Str, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-20, BootupInfo01Str, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);

	// draw fonts in the screen
	///pNowFont = (void *)&Gentium_Normal15;
	///pNowFont = (void *)&comic_Normal19;
	///pNowFont = (void *)&Gentium_Normal17U;
	pNowFont = (void *)&Monaco_Normal19U;
	width = GetTextWidth(BootupInfo02Str, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, BootupInfo02Str, pNowFont, GREEN);
	//DelayMs(DEMODELAY);	

	// draw fonts in the screen
	///pNowFont = (void *)&Gentium_Normal15;
	///pNowFont = (void *)&comic_Normal15;
	///pNowFont = (void *)&Gentium_Normal17U;
	pNowFont = (void *)&Monaco_Normal15U;
	width = GetTextWidth(BootupInfo03Str, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+20, BootupInfo03Str, pNowFont, LIGHTBLUE);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);

	/* multi-language test
	wStrID = 0;
	pNowFont = (void *)&Monaco_Normal18U;
	width = GetTextWidth(IdGetMString(wStrID,EN), pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+40, IdGetMString(wStrID,EN), pNowFont, LIGHTBLUE);
	DelayMs(DEMODELAY);

	pNowFont = (void *)&Monaco_Normal18U;
	width = GetTextWidth(IdGetMString(wStrID,SP), pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+60, IdGetMString(wStrID,SP), pNowFont, LIGHTBLUE);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);
	*/
	
	if(0)
	{
	XCHAR *pxStr = NULL;
	gcCleanScreen();
	
	///pNowFont = (void *)&Gentium_Normal19U;
	pNowFont = (void *)&Monaco_Normal19U;
	// draw fonts in the screen
	pxStr = TestSettingENStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 0, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);

	// draw fonts in the screen
	pxStr = TestSettingSPStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 20, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);

	// draw fonts in the screen
	pxStr = TestSettingFRStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 40, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);

	// draw fonts in the screen
	pxStr = TestSettingITStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 60, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestSettingPOStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 80, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);

	// draw fonts in the screen
	pxStr = TestSettingDUStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 100, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);

	// draw fonts in the screen
	pxStr = TestSettingGEStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 120, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);

	// draw fonts in the screen
	pxStr = TestSettingDAStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, 140, pxStr, pNowFont, LIGHTRED);
	DelayMs(DEMODELAY);

	}

	if(0)
	{
	XCHAR *pxStr = NULL;
	gcCleanScreen();
	
	pNowFont = (void *)&Monaco_Normal19U;
	// draw fonts in the screen
	pxStr = TestMenuENStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-80, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuSPStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-60, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuFRStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-40, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuITStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-20, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuPOStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+0, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuDUStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+20, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuGEStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+40, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	pxStr = TestMenuDAStr;
	///pNowFont = (void *)&Gentium_Normal21U;
	SetFont(pNowFont);
	SetColor(LIGHTRED);
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+60, pxStr);
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	}

	if(0)
	{
	XCHAR *pxStr = NULL;
	SHORT nowY = 0;
	gcCleanScreen();
	
	// draw fonts in the screen
	pxStr = IdGetMString(1,EN);
	pNowFont = (void *)&Monaco_Normal15U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY( 5, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);
	///nowY = nowY + height;

	// draw fonts in the screen
	pxStr = IdGetMString(1,SP);
	pNowFont = (void *)&Monaco_Normal16U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY( 70, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);
	nowY = nowY + height;

	// draw fonts in the screen
	pxStr = IdGetMString(1,FR);
	pNowFont = (void *)&Monaco_Normal19U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	// draw fonts in the screen
	pxStr = IdGetMString(1,IT);
	pNowFont = (void *)&Monaco_Normal20U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	// draw fonts in the screen
	pxStr = IdGetMString(1,PO);
	pNowFont = (void *)&Monaco_Normal23U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	// draw fonts in the screen
	pxStr = IdGetMString(1,DU);
	pNowFont = (void *)&Monaco_Normal26U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	// draw fonts in the screen
	pxStr = IdGetMString(1,GE);
	pNowFont = (void *)&Monaco_Normal27U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	// draw fonts in the screen
	pxStr = IdGetMString(1,DA);
	pNowFont = (void *)&Monaco_Normal30U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	
	}

	if(0)
	{
	XCHAR *pxStr = NULL;
	SHORT nowY = 0;
	gcCleanScreen();
	
	// draw fonts in the screen
	pxStr = NumbersENStr;
	pNowFont = (void *)&Monaco_Normal15U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY( 5, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);
	///nowY = nowY + height;

	// draw fonts in the screen
	///pxStr = OptionsSPStr;
	pNowFont = (void *)&Monaco_Normal16U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY( 60, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);
	nowY = nowY + height;

	// draw fonts in the screen
	///pxStr = OptionsFRStr;
	pNowFont = (void *)&Monaco_Normal19U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	// draw fonts in the screen
	///pxStr = OptionsITStr;
	pNowFont = (void *)&Monaco_Normal20U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	// draw fonts in the screen
	///pxStr = OptionsPOStr;
	pNowFont = (void *)&Monaco_Normal23U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	// draw fonts in the screen
	///pxStr = OptionsDUStr;
	pNowFont = (void *)&Monaco_Normal26U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	// draw fonts in the screen
	///pxStr = OptionsGEStr;
	pNowFont = (void *)&Monaco_Normal27U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	// draw fonts in the screen
	///pxStr = OptionsDAStr;
	pNowFont = (void *)&Monaco_Normal30U;
	width = GetTextWidth(pxStr, pNowFont);
	height = GetTextHeight(pNowFont);
	gcColFntOutTextXY((GetMaxX() - width) >> 1, nowY, pxStr, pNowFont, LIGHTRED);
	//DelayMs(DEMODELAY);
	//DelayMs(DEMODELAY);	
	nowY = nowY + height;

	
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	
	}


	
/***********************************************************************************************************/

#ifdef USE_MAGELLAN_LOGO
	#ifdef USE_ANIMATION
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
	#else
	gcCleanScreen();

	width = GetImageWidth((void *) &Reno_magellan_logo_8bit);
	height = GetImageHeight((void *) &Reno_magellan_logo_8bit);
	
	width = (GetMaxX() - width) >> 1;
	height = (GetMaxY() - height) >> 1;
	
	WAIT_UNTIL_FINISH(PutImage(width, height, (void *) &Reno_magellan_logo_8bit, 1));
	DelayMs(DEMODELAY);	
	DelayMs(DEMODELAY);	

	#endif
#endif
	
	if( 0 )
	{
		STATICTEXT *pObj = NULL;
		pObj = StCreate(ID_STATICTEXT1,           	// ID 
			  15, 130, 128, 150,
              ST_DRAW,        	// will be dislayed, has frame
              (XCHAR *)AdditionalInfo01Str, // multi-line text
			  popupAskScheme);                   	// default GOL scheme 


		//SetState(pObj, ST_DRAW);
		//scrFixStat();
		DelayMs(DEMODELAY);	
		DelayMs(DEMODELAY);	
	}

	printf("Create Booting done!!\n");

}


void UpdateDeviceMode_booting(WORD wDrawOption)
{
	static	WORD wCount = 0;
	static	SHORT sOffset = 0;
    SHORT	width, height;
	void	*pNowFont = NULL;	
	XCHAR 	*pxStr = NULL;
	

	wCount = wCount + 1;
	
	if( (0 == wCount%1) )
	{
		SetClip(1);
		SetClipRgn(15, 130, 128, 150);
		// draw fonts in the screen
		pxStr = AdditionalInfo01Str;
		pNowFont = (void *)&Monaco_Normal20U;
		width = GetTextWidth(pxStr, pNowFont);
		height = GetTextHeight(pNowFont);
		SetColor(WHITE);
		Bar(0, 130, GetMaxX(), 150);
		gcColFntOutTextXY( 15-sOffset, 130, pxStr, pNowFont, LIGHTBLUE);
		sOffset++;
		sOffset++;
		sOffset++;
		if( width < sOffset )
			sOffset = 0;
		SetClip(0);
	}
}


WORD MsgDeviceMode_booting(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	
	if( pMsg )
		printf("[%d, %d, %d, %d]\n", pMsg->type, pMsg->uiEvent, pMsg->param1, pMsg->param2);
	else
		printf("error!!!\n");
	
    switch(GetObjID(pObj))
    {
		case ID_TICKING:
			printf("ID_TICKING!!\n");
			return (0); 
        default:
            return 1; 							// process by default
    }
}

WORD MsgDeviceMode_bootingDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	if( pMsg )
		printf("22[%d, %d, %d, %d]\n", pMsg->type, pMsg->uiEvent, pMsg->param1, pMsg->param2);
	else
		printf("error!!!\n");

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
				LbChangeSel(pLb, pLb->pFocusItem);
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
				LbChangeSel(pLb, pLb->pFocusItem);
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


void CreateDeviceMode_popask(WORD wDrawOption)
{

	
    GOLFree();                                      // free memory for the objects in the previous linked list and start new list
	CreateDefaultBtn();

    BtnCreate
    (
        ID_BUTTON7,             // button ID
        0 + POASK_FRAME_OFFSET_W,
        0 + POASK_FRAME_OFFSET_H,                         // left, top corner	
        GetMaxX() - POASK_FRAME_OFFSET_W,
        GetMaxY() - POASK_FRAME_OFFSET_H,
        8,                          // right, bottom corner (with radius = 0)
        BTN_DRAW|BTN_TEXTTOP,                   // will be dislayed after creation
        NULL,                       // no bitmap	
        popupAsk.pPopTitle,      // LEFT arrow as text
        popupMenuScheme
    );

    BtnCreate
    (
        ID_BUTTON3,             // button ID
        ((GetMaxX() - POASK_FRAME_W) >> 1),
        ((GetMaxY() - POASK_FRAME_H) >> 1) - 40,                         // left, top corner	
        ((GetMaxX() - POASK_FRAME_W) >> 1) + POASK_FRAME_W,
        ((GetMaxY() - POASK_FRAME_H) >> 1) - 40 + POASK_FRAME_H,
        0,                          // right, bottom corner (with radius = 0)
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap	
        Ask01Str,      // LEFT arrow as text
        popupAskScheme
    ); 

    BtnCreate
    (
        ID_BUTTON4,             // button ID
        ((GetMaxX() - POASK_FRAME_W) >> 1),
        ((GetMaxY() - POASK_FRAME_H) >> 1) - 10,                         // left, top corner	
        ((GetMaxX() - POASK_FRAME_W) >> 1) + POASK_FRAME_W,
        ((GetMaxY() - POASK_FRAME_H) >> 1) - 10 + POASK_FRAME_H,
        0,                          // right, bottom corner (with radius = 0)
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap	
        Ask02Str,      // LEFT arrow as text
        popupAskScheme
    ); 
	
    BtnCreate
    (
        ID_BUTTON1,             // button ID
        ((GetMaxX() - POASK_FRAME_W) >> 1),
        ((GetMaxY() - POASK_FRAME_H) >> 1) + 25,                         // left, top corner	
        ((GetMaxX() - POASK_FRAME_W) >> 1) + POASK_FRAME_W,
        ((GetMaxY() - POASK_FRAME_H) >> 1) + 25 + POASK_FRAME_H,
        0,                          // right, bottom corner (with radius = 0)
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap	
        popupAsk.pPopAskList->pPopItem11->pPopMsg,      // LEFT arrow as text
        popupAskScheme
    ); 

    BtnCreate
    (
        ID_BUTTON2,             // button ID
        ((GetMaxX() - POASK_FRAME_W) >> 1),
        ((GetMaxY() - POASK_FRAME_H) >> 1) + 25 + POASK_FRAME_H + 2,                         // left, top corner	
        ((GetMaxX() - POASK_FRAME_W) >> 1) + POASK_FRAME_W,
        ((GetMaxY() - POASK_FRAME_H) >> 1) + 25 + POASK_FRAME_H + POASK_FRAME_H + 2,
        0,                          // right, bottom corner (with radius = 0)
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap	
        popupAsk.pPopAskList->pPopItem12->pPopMsg,      // LEFT arrow as text
        popupAskScheme
    ); 

}



WORD MsgDeviceMode_popask(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}

WORD MsgDeviceMode_popaskDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
//	LISTBOX *pLb;
//	POPUPITEM_HEADER*	pPopItems;

	
//	pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);
    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BUTTON7:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
			return (0); 
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
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
			}
			return (0); 
        case ID_BTN_DOWN_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///???
				
			}
			return (0);
		case ID_BUTTON2:
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///record
				///pPopItems = popupAsk.pPopAskList->pPopItem11;
				scrSetNEXT( popupAsk.pPopAskList->pPopItem11->pGoFrame );
			}
			return (0); 
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///reset
			}
			return (0);
		case ID_BUTTON1:
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///lap, back to privious
			#if 0	
				scrSetNEXT(popupAsk.pPrivFrame);
			#else
				scrSetNEXT( popupAsk.pPopAskList->pPopItem12->pGoFrame );
			#endif
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


POPUPITEM_HEADER popitem11 = 
{
	"11                  ",
	&I16164_About,
	&fhMapMode_navgation
};

POPUPITEM_HEADER popitem12 = 
{
	"12                  ",
	&I16164_About,
	&fhMapMode_navgation
};


///popup menu
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


///popup ask 
POPUPASKLIST_HEADER popasklist =
{
	&popitem11,
	&popitem12,
};


POPUPASK_HEADER popupAsk = 
{
	2,						///item number
	"t                  ",				///title
	&fhDataMode_three,		///privious frame
	&popasklist
};




FRAME_HEADER fhWatchMode_watch = {
	MsgWatchMode_watch,
	CreateWatchMode_watch,
	MsgWatchMode_watchDefaultBtn,
	0,		///no option
	UpdateWatchMode_watch,
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
	UpdateDeviceMode_booting,
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


FRAME_HEADER fhDeviceMode_popask = {
	MsgDeviceMode_popask,
	CreateDeviceMode_popask,
	MsgDeviceMode_popaskDefaultBtn,
	0,		///no option
	0,
	0
};
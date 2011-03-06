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


GOL_SCHEME      *altScheme;                                 // alternative style scheme
GOL_SCHEME      *alt2Scheme;                                // alternative 2 style scheme
GOL_SCHEME      *alt3Scheme;                                // alternative 3 style scheme
///GOL_SCHEME      *alt4Scheme;                                // alternative 4 style scheme
///GOL_SCHEME      *alt5Scheme;                                // alternative 5 style scheme
///GOL_SCHEME      *timeScheme;
///GOL_SCHEME      *meterScheme;                               // meter scheme
GOL_SCHEME      *navScheme;                                 // style scheme for the navigation
///GOL_SCHEME      *redScheme;                                 // alternative red style scheme
GOL_SCHEME      *greenScheme;                               // alternative green style scheme
GOL_SCHEME      *yellowScheme;                              // alternative yellow style scheme
GOL_SCHEME      *popupMenuScheme;                           // popup menu style scheme
GOL_SCHEME      *dataSetScheme;								// dataset style scheme	
GOL_SCHEME      *dataSetScheme2;								// dataset style scheme	
GOL_SCHEME      *windowScheme;								// dataset style scheme	
GOL_SCHEME      *popupAskScheme;                           // popup ask style scheme


OBJ_HEADER      *pNavList;                                  // pointer to navigation list
volatile DWORD  tick = 0;                                   // tick counter
char            animate;                                    // switch to control animation for picture demo
OBJ_HEADER      *pGenObj;                                   // pointer to a general purpose object


SCREEN_STATES   screenState = CREATE_BUTTONS;               // current state of main demo state mashine
SCREEN_STATES   prevState = CREATE_BUTTONS; // used to mark state where time setting was called	
SCREEN_STATES   prevRefreshState = CREATE_BUTTONS; // used to mark the start of the previous screen

///span use
SCREEN_STATUS	scrStatus;
SCREEN_STATUS	*psrcStat;
static FRAME_HEADER	*pLastViewFrame;
static BOOL bStatFixed = FALSE;




void CreateDefaultBtn(void)
{
#ifdef USE_DEFAULTBTN
    OBJ_HEADER  *obj;

    BtnCreate
    (
        ID_BUTTON_BACK,             // button ID
        0,
        0 + (GetMaxY()-DEFAULTBTN_HEIGHT),                         // left, top corner	
        0+DEFAULTBTN_WIDTH,
        GetMaxY(),
        0,                          // right, bottom corner (with radius = 0)
        BTN_DRAW,                   // will be dislayed after creation
        NULL,                       // no bitmap	
        (XCHAR *)"Priv",      // LEFT arrow as text
        navScheme
    );                              // use navigation scheme
    obj = (OBJ_HEADER *)BtnCreate
        (
            ID_BUTTON_NEXT,         // button ID
            (GetMaxX()/2),
            0 + (GetMaxY()-DEFAULTBTN_HEIGHT),
            (GetMaxX()/2) + DEFAULTBTN_WIDTH,
            GetMaxY(),
            0,                      // dimension (with radius = 0)
            BTN_DRAW,               // will be dislayed and disabled after creation
            NULL,                   // no bitmap
            (XCHAR *)"Next", // RIGHT arrow as text
            navScheme
        );                          // use navigation scheme
#endif

	/* add Reno 4(8) buttons simulate (include press/hold action) */
    BtnCreate
        (
            ID_BTN_UP,         // button ID
            0,
            0 + (HWBTN_HEIGHT*0),
            HWBTN_WIDTH,
            HWBTN_HEIGHT*1,
            0,                      // dimension (with radius = 0)
            BTN_HIDE,               // will be dislayed and disabled after creation
            NULL,                   // no bitmap
            (XCHAR *)NULL, 			// RIGHT arrow as text
            navScheme
        );                          // use navigation scheme
    BtnCreate
        (
            ID_BTN_DOWN,         // button ID
            0,
            0 + (HWBTN_HEIGHT*1),
            HWBTN_WIDTH,
            HWBTN_HEIGHT*2,
            0,                      // dimension (with radius = 0)
            BTN_HIDE,               // will be dislayed and disabled after creation
            NULL,                   // no bitmap
            (XCHAR *)NULL, 			// RIGHT arrow as text
            navScheme
        );                          // use navigation scheme
    BtnCreate
        (
            ID_BTN_EXIT,         // button ID
            0,
            0 + (HWBTN_HEIGHT*2),
            HWBTN_WIDTH,
            HWBTN_HEIGHT*3,
            0,                      // dimension (with radius = 0)
            BTN_HIDE,               // will be dislayed and disabled after creation
            NULL,                   // no bitmap
            (XCHAR *)NULL, 			// RIGHT arrow as text
            navScheme
        );                          // use navigation scheme
    BtnCreate
        (
            ID_BTN_ENTER,         // button ID
            0,
            0 + (HWBTN_HEIGHT*3),
            HWBTN_WIDTH,
            HWBTN_HEIGHT*4,
            0,                      // dimension (with radius = 0)
            BTN_HIDE,               // will be dislayed and disabled after creation
            NULL,                   // no bitmap
            (XCHAR *)NULL, 			// RIGHT arrow as text
            navScheme
        );                          // use navigation scheme

    BtnCreate
        (
            ID_BTN_UP_HOLD,         // button ID
            0,
            0 + (HWBTN_HEIGHT*4),
            HWBTN_WIDTH,
            HWBTN_HEIGHT*5,
            0,                      // dimension (with radius = 0)
            BTN_HIDE,               // will be dislayed and disabled after creation
            NULL,                   // no bitmap
            (XCHAR *)NULL, 			// RIGHT arrow as text
            navScheme
        );                          // use navigation scheme
    BtnCreate
        (
            ID_BTN_DOWN_HOLD,         // button ID
            0,
            0 + (HWBTN_HEIGHT*5),
            HWBTN_WIDTH,
            HWBTN_HEIGHT*6,
            0,                      // dimension (with radius = 0)
            BTN_HIDE,               // will be dislayed and disabled after creation
            NULL,                   // no bitmap
            (XCHAR *)NULL, 			// RIGHT arrow as text
            navScheme
        );                          // use navigation scheme
    BtnCreate
        (
            ID_BTN_EXIT_HOLD,         // button ID
            0,
            0 + (HWBTN_HEIGHT*6),
            HWBTN_WIDTH,
            HWBTN_HEIGHT*7,
            0,                      // dimension (with radius = 0)
            BTN_HIDE,               // will be dislayed and disabled after creation
            NULL,                   // no bitmap
            (XCHAR *)NULL, 			// RIGHT arrow as text
            navScheme
        );                          // use navigation scheme
    BtnCreate
        (
            ID_BTN_ENTER_HOLD,         // button ID
            0,
            0 + (HWBTN_HEIGHT*8),
            HWBTN_WIDTH,
            HWBTN_HEIGHT*9,
            0,                      // dimension (with radius = 0)
            BTN_HIDE,               // will be dislayed and disabled after creation
            NULL,                   // no bitmap
            (XCHAR *)NULL, 			// RIGHT arrow as text
            navScheme
        );                          // use navigation scheme

		
#if defined (USE_FOCUS) 
#ifdef USE_DEFAULTBTN                  
    GOLSetFocus(obj);   // set focus for the button
#endif
#endif  
  
}


WORD MsgDefaultBtn(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
				scrNextStat();
            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
				scrPrivStat();
            return (1);                             // process by default

        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
				scrPrivStat();
			return (0);
			
        case ID_BTN_UP_HOLD:
			return (0); 
			
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
				scrNextStat();
			return (0); 
			
        case ID_BTN_DOWN_HOLD:
			return (0); 
			
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
				scrPrivStat();
			return (0);  

        case ID_BTN_EXIT_HOLD:
        case ID_BTN_ENTER:
        case ID_BTN_ENTER_HOLD:
            return (0);                             // do nothing since it's HW key simulate
	
        default:
            return (1);                 // process by default
    }
}

STATICTEXT *CreateDataSet(SHORT left, SHORT top, SHORT right, SHORT bottom, XCHAR *pText, XCHAR *pFunc, XCHAR *pData, XCHAR *pUnit)
{
    SHORT	width, height, temp;
	SHORT	dataframeoffset, unitoffset;
	void	*pNowFont = NULL;
	STATICTEXT *pObj = NULL;
	GOL_SCHEME *pSch = NULL;
	
	///draw the frame
	SetColor(BLACK);
	Line(left,top,right,top);
	Line(left,top,left,bottom);
	Line(right,top,right,bottom);
	Line(left,bottom,right,bottom);

	if( (bottom-top) < (GetMaxY()/2)-5 )
	{	/// 1/3 height
		if( (right-left) < (GetMaxX()-5) )
		{	/// 1/2 width
			pSch = dataSetScheme2;
			pNowFont = (void *)&Monaco_Normal15U;
			pSch->pFont = (void *)&Monaco_Normal20U;
			dataframeoffset = 0;
			unitoffset = 0;
		}
		else
		{
			pSch = dataSetScheme;
			pNowFont = (void *)&Monaco_Normal20U;
			pSch->pFont = (void *)&Monaco_Normal41U;
			dataframeoffset = 5;
			unitoffset = 2;
		}	
	}
	else
	{
		pSch = dataSetScheme;
		pNowFont = (void *)&Monaco_Normal20U;
		pSch->pFont = (void *)&Monaco_Normal41U;
		dataframeoffset = 5;
		unitoffset = 2;
	}
	
	
	///draw function name
	///pNowFont = (void *)&Monaco_Normal20U;
	height = GetTextHeight(pNowFont);
	temp = top+2;
	gcColFntOutTextXY( left+4, temp, pFunc, pNowFont, BLACK);
	temp = temp+height;

	///draw the data value	
	width = GetTextWidth(pData, pSch->pFont);
	height = GetTextHeight(pSch->pFont);
    pObj = StCreate(ID_DATASETVALUE1,           	// ID 
			  (left+(((right-left)-width)>>1))-ST_INDENT, (temp+(((bottom-temp)-height)>>1))+dataframeoffset, (left+(((right-left)-width)>>1))+width, (temp+(((bottom-temp)-height)>>1))+height-dataframeoffset,
              ST_DRAW|ST_CENTER_ALIGN,        	// will be dislayed, has frame
              pData, // multi-line text
			  pSch);                   	// default GOL scheme 

	///draw the unit
	if( NULL != pUnit )
	{
		width = (left+(((right-left)-width)>>1))+width;
		temp = (temp+(((bottom-temp)-height)>>1))+height-dataframeoffset+unitoffset;
		///pNowFont = (void *)&Monaco_Normal20U;
		height = GetTextHeight(pNowFont);
		gcColFntOutTextXY( width+2, temp-height, pUnit, pNowFont, BLACK);
	}

	return pObj;
}



void AddItemList(XCHAR *pText, LISTBOX *pLb, void *pIcon)
{
    XCHAR   *pointer;
    WORD    ctr;

    if(pText != NULL)
    {
        pointer = pText;
        ctr = 0;
        while(*pointer)
        {
            ///if(NULL == LbAddItem(pLb, NULL, pointer, NULL, 0, ctr))
			if(NULL == LbAddItem(pLb, NULL, pointer, pIcon, 0, ctr))
                break;
            while(*pointer++ > 31);
            if(*(pointer - 1) == 0)
                break;
            ctr++;
        }
    }
}


void CreateTextEntryPad(WORD wDrawOption)
{
#define NTEXT_H	25
#define NTEXT_W	25
#define NBTN_H	25
#define NBTN_W	25

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list
	SetColor(BLACK);
	ClearDevice();	
	CreateDefaultBtn();

	if( 1 == wDrawOption )
	{
	StCreate(
		ID_STATICTEXT1,
        ((GetMaxX() - (NTEXT_W)) >> 1),
        ((GetMaxY() - (NTEXT_H)) >> 1),
        ((GetMaxX() - (NTEXT_W)) >> 1) + NTEXT_W,
        ((GetMaxY() - (NTEXT_H)) >> 1) + NTEXT_H,                      // dimension
		LB_DRAW | ST_CENTER_ALIGN | ST_FRAME | LB_FOCUSED,                   // will be dislayed after creation
		(XCHAR *)"A",
		alt2Scheme
	);
	}
	else
	{
	StCreate(
		ID_STATICTEXT1,
        ((GetMaxX() - (NTEXT_W)) >> 1),
        ((GetMaxY() - (NTEXT_H)) >> 1),
        ((GetMaxX() - (NTEXT_W)) >> 1) + NTEXT_W,
        ((GetMaxY() - (NTEXT_H)) >> 1) + NTEXT_H,                      // dimension
		LB_DRAW | ST_CENTER_ALIGN | ST_FRAME | LB_FOCUSED,                   // will be dislayed after creation
		(XCHAR *)"0",
		alt2Scheme
	);
	}
	
    BtnCreate
    (
        ID_BUTTON1,                 // button ID
        ((GetMaxX() - (NTEXT_W)) >> 1),
        ((GetMaxY() - (NTEXT_H)) >> 1) - NBTN_H,
        ((GetMaxX() - (NTEXT_W)) >> 1) + NTEXT_W,
        ((GetMaxY() - (NTEXT_H)) >> 1) + NTEXT_H - NBTN_H,                      // dimension
        0,                         // set radius
        BTN_DRAW,                   // draw a beveled button
        (void *)&I16164_Top,                       // no bitmap
        (XCHAR *)NULL,         // text
        alt2Scheme
    );                              // use alternate scheme
    BtnCreate
    (
        ID_BUTTON2,                 // button ID
        ((GetMaxX() - (NTEXT_W)) >> 1),
        ((GetMaxY() - (NTEXT_H)) >> 1) + NBTN_H,
        ((GetMaxX() - (NTEXT_W)) >> 1) + NTEXT_W,
        ((GetMaxY() - (NTEXT_H)) >> 1) + NTEXT_H + NBTN_H,                      // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        (void *)&I16164_Bottom,   // use bitmap
        (XCHAR *)NULL,           // text
        alt2Scheme
    );  
	
}


WORD MsgTextEntryPad(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgTextEntryPadDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	static XCHAR sIdx = 0;
	static XCHAR scNum[2];
	STATICTEXT *pSt;
	pSt = (STATICTEXT *)GOLFindObject(ID_STATICTEXT1);
	
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
		case ID_BUTTON1:
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
		case ID_BUTTON2:
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


void CreateNumEntryPad(WORD wDrawOption)
{
	CreateTextEntryPad(wDrawOption);
}


WORD MsgNumEntryPad(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return (1);                             // process by default
    }
}

WORD MsgNumEntryPadDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
	static XCHAR sIdx = 0;
	static XCHAR scNum[2];
	STATICTEXT *pSt;
	pSt = (STATICTEXT *)GOLFindObject(ID_STATICTEXT1);
	
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
		case ID_BUTTON1:
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
		case ID_BUTTON2:
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


FRAME_HEADER fhTextEnteryPad = {
	MsgTextEntryPad,
	CreateTextEntryPad,
	MsgTextEntryPadDefaultBtn,
	1,		///text pad
	0,
	0
};


FRAME_HEADER fhNumEnteryPad = {
	MsgNumEntryPad,
	CreateNumEntryPad,
	MsgNumEntryPadDefaultBtn,
	0,		///number pad
	0,
	0
};



void myCreateScheme( void )
{
    // create the alternate schemes
    navScheme = GOLCreateScheme();      // alternative scheme for the navigate buttons
    altScheme = GOLCreateScheme();      // create alternative 1 style scheme
    alt2Scheme = GOLCreateScheme();     // create alternative 2 style scheme
    alt3Scheme = GOLCreateScheme();     // create alternative 3 style scheme
///    alt4Scheme = GOLCreateScheme();     // create alternative 4 style scheme
///    alt5Scheme = GOLCreateScheme();     // create alternative 5 style scheme
///    timeScheme = GOLCreateScheme();
///    meterScheme = GOLCreateScheme();    // create meter scheme
///    redScheme = GOLCreateScheme();      // create red style scheme
    greenScheme = GOLCreateScheme();    // create green style scheme
    yellowScheme = GOLCreateScheme();   // create yellow style scheme
	popupMenuScheme = GOLCreateScheme();   // create popup menu style scheme
	dataSetScheme = GOLCreateScheme();   // create DataSet style scheme
	dataSetScheme2 = GOLCreateScheme();   // create DataSet style scheme
	windowScheme = GOLCreateScheme();   // create Window style scheme
	popupAskScheme = GOLCreateScheme();   // create popup ask style scheme
	
    /* for Truly display */
    altScheme->Color0 = RGB565CONVERT(0x4C, 0x8E, 0xFF);
    altScheme->Color1 = RGB565CONVERT(255, 102, 0);
    altScheme->EmbossDkColor = RGB565CONVERT(0x1E, 0x00, 0xE5);
    altScheme->EmbossLtColor = RGB565CONVERT(0xA9, 0xDB, 0xEF);
    altScheme->ColorDisabled = RGB565CONVERT(0xD4, 0xE1, 0xF7);
    altScheme->TextColor1 = BRIGHTBLUE;
    altScheme->TextColor0 = RGB565CONVERT(255, 102, 0);
    altScheme->TextColorDisabled = RGB565CONVERT(0xB8, 0xB9, 0xBC);


    popupMenuScheme->Color0 = GRAY80;	//item background
    popupMenuScheme->Color1 = BLACK;	//reversed
    popupMenuScheme->EmbossDkColor = GRAY20;
    popupMenuScheme->EmbossLtColor = GRAY20;
    popupMenuScheme->ColorDisabled = BRIGHTGREEN;
    popupMenuScheme->TextColor1 = GRAY80;		//reversed
    popupMenuScheme->TextColor0 = BLACK;		//font color
    popupMenuScheme->TextColorDisabled = BRIGHTRED;
	///popupMenuScheme->pFont = (void *)Gentium_Normal19U;
	popupMenuScheme->pFont = (void *)&Monaco_Normal19U;
	popupMenuScheme->CommonBkColor = BRIGHTYELLOW;			

    dataSetScheme->Color0 = GRAY80;	//item background
    dataSetScheme->Color1 = BLACK;	//reversed
    dataSetScheme->EmbossDkColor = GRAY20;
    dataSetScheme->EmbossLtColor = GRAY20;
    dataSetScheme->ColorDisabled = BRIGHTGREEN;
    dataSetScheme->TextColor1 = GRAY80;		//reversed
    dataSetScheme->TextColor0 = BLACK;		//font color
    dataSetScheme->TextColorDisabled = BRIGHTRED;
	dataSetScheme->pFont = (void *)&Monaco_Normal45U;
	dataSetScheme->CommonBkColor = GRAY80;			

    dataSetScheme2->Color0 = GRAY80;	//item background
    dataSetScheme2->Color1 = BLACK;	//reversed
    dataSetScheme2->EmbossDkColor = GRAY20;
    dataSetScheme2->EmbossLtColor = GRAY20;
    dataSetScheme2->ColorDisabled = BRIGHTGREEN;
    dataSetScheme2->TextColor1 = GRAY80;		//reversed
    dataSetScheme2->TextColor0 = BLACK;		//font color
    dataSetScheme2->TextColorDisabled = BRIGHTRED;
	///dataSetScheme2->pFont = (void *)&Monaco_Normal45U;
	dataSetScheme2->pFont = (void *)&Monaco_Normal20U;
	dataSetScheme2->CommonBkColor = GRAY80;			

	windowScheme->Color0 = GRAY80;	//item background
    windowScheme->Color1 = BLACK;	//reversed
    windowScheme->EmbossDkColor = GRAY20;
    windowScheme->EmbossLtColor = GRAY20;
    windowScheme->ColorDisabled = BRIGHTGREEN;
    windowScheme->TextColor1 = GRAY80;		//reversed
    windowScheme->TextColor0 = BLACK;		//font color
    windowScheme->TextColorDisabled = BRIGHTRED;
	windowScheme->pFont = (void *)&Monaco_Normal20U;
	windowScheme->CommonBkColor = WHITE;			
	
	popupAskScheme->Color0 = GRAY80;	//item background
    popupAskScheme->Color1 = BLACK;	//reversed
    popupAskScheme->EmbossDkColor = GRAY20;
    popupAskScheme->EmbossLtColor = GRAY20;
    popupAskScheme->ColorDisabled = BRIGHTGREEN;
    popupAskScheme->TextColor1 = GRAY80;		//reversed
    popupAskScheme->TextColor0 = BLACK;		//font color
    popupAskScheme->TextColorDisabled = BRIGHTRED;
	popupAskScheme->pFont = (void *)&Monaco_Normal19U;
	popupAskScheme->CommonBkColor = BRIGHTYELLOW;
	
	
///    altScheme->pFont = (void *)ptrLargeAsianFont;
///    navScheme->pFont = (void *)ptrLargeAsianFont;
	altScheme->pFont = (void *)Gentium_Normal15;
	altScheme->CommonBkColor = BLACK;	
	
	navScheme->pFont = (void *)Gentium_Normal15;
	navScheme->CommonBkColor = BLACK;

    alt2Scheme->TextColor1 = BRIGHTRED;
///    alt2Scheme->TextColor0 = BRIGHTBLUE;
    alt2Scheme->TextColor0 = LIGHTBLUE;
///    alt2Scheme->pFont = (void *)ptrSmallAsianFont;
	alt2Scheme->pFont = (void *)Gentium_Normal25;
	alt2Scheme->CommonBkColor = BLACK;	

    alt3Scheme->Color0 = LIGHTBLUE;
    alt3Scheme->Color1 = BRIGHTGREEN;
    alt3Scheme->TextColor0 = BLACK;
    alt3Scheme->TextColor1 = WHITE;
    alt3Scheme->pFont = (void *)Gentium_Normal21;
	alt3Scheme->CommonBkColor = BLACK;	
	
///    alt4Scheme->Color0 = LIGHTBLUE;
///    alt4Scheme->Color1 = BRIGHTGREEN;
///    alt4Scheme->TextColor0 = BLACK;
///    alt4Scheme->TextColor1 = WHITE;
///    alt4Scheme->pFont = (void *)ptrSmallAsianFont;

///    alt5Scheme->Color0 = LIGHTBLUE;
///    alt5Scheme->Color1 = BRIGHTRED;
///    alt5Scheme->TextColor0 = BLACK;
///    alt5Scheme->TextColor1 = WHITE;
///    alt5Scheme->pFont = (void *) &monofont; //monofont is equal width font, required for digitalmeter widget

///    redScheme->Color0 = RGB565CONVERT(0xCC, 0x00, 0x00);
///    redScheme->Color1 = BRIGHTRED;
///    redScheme->EmbossDkColor = RED4;
///    redScheme->EmbossLtColor = FIREBRICK1;
///    redScheme->TextColor0 = RGB565CONVERT(0xC8, 0xD5, 0x85);
///    redScheme->TextColor1 = BLACK;
///    redScheme->pFont = (void *)ptrLargeAsianFont;
///	redScheme->pFont = (void *)Gentium_Normal25;

    greenScheme->Color0 = RGB565CONVERT(0x23, 0x9E, 0x0A);
    greenScheme->Color1 = BRIGHTGREEN;
    greenScheme->EmbossDkColor = DARKGREEN;
    greenScheme->EmbossLtColor = PALEGREEN;
    greenScheme->TextColor0 = RGB565CONVERT(0xDF, 0xAC, 0x83);
    greenScheme->TextColor1 = BLACK;
///    greenScheme->pFont = (void *)ptrLargeAsianFont;
	greenScheme->pFont = (void *)Gentium_Normal25;

    yellowScheme->Color0 = BRIGHTYELLOW;
    yellowScheme->Color1 = YELLOW;
    yellowScheme->EmbossDkColor = RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowScheme->EmbossLtColor = RGB565CONVERT(0xFD, 0xFF, 0xB2);
    yellowScheme->TextColor0 = RGB565CONVERT(0xAF, 0x34, 0xF3);
    yellowScheme->TextColor1 = RED;
///    yellowScheme->pFont = (void *)ptrLargeAsianFont;
	yellowScheme->pFont = (void *)Gentium_Normal25;

///    timeScheme->Color0 = BLACK;
///    timeScheme->Color1 = WHITE;
///    timeScheme->TextColor0 = BRIGHTBLUE;
///    timeScheme->TextColor1 = WHITE;
///    timeScheme->EmbossDkColor = GRAY20;
///    timeScheme->EmbossLtColor = GRAY80;
///    timeScheme->pFont = (void *) &GOLSmallFont;

///    meterScheme->Color0 = BLACK;
///    meterScheme->Color1 = WHITE;
///    meterScheme->TextColor0 = BRIGHTBLUE;
///	meterScheme->TextColor0 = BRIGHTYELLOW;
///    meterScheme->TextColor1 = WHITE;
///    meterScheme->EmbossDkColor = GRAY20;
///    meterScheme->EmbossLtColor = GRAY80;
///    meterScheme->pFont = (void *)ptrSmallAsianFont;
///	meterScheme->pFont = (void *)Gentium_Normal15;
///	meterScheme->CommonBkColor = BLACK;

}



XCHAR Num2Xchar( WORD wNum )
{
	XCHAR xRet = 0x0;

	switch (wNum)
	{
		case 0:
			xRet = 0x0030;
			break;
		case 1:
			xRet = 0x0031;
			break;
		case 2:
			xRet = 0x0032;
			break;
		case 3:
			xRet = 0x0033;
			break;
		case 4:
			xRet = 0x0034;
			break;
		case 5:
			xRet = 0x0035;
			break;
		case 6:
			xRet = 0x0036;
			break;
		case 7:
			xRet = 0x0037;
			break;
		case 8:
			xRet = 0x0038;
			break;
		case 9:
			xRet = 0x0039;
			break;
		default:
			xRet = 0x003F;
			break;

	}
	
	return xRet;
}




void updateLastView(FRAME_HEADER* pLastFrame)
{
	pLastViewFrame = pLastFrame;
}
	
FRAME_HEADER* getLastView(void)
{
	return pLastViewFrame;
}
	
	
/// API for screen status handle
void scrInitStat(void)
{
	psrcStat = &scrStatus;
	
	if( psrcStat )
	{
		psrcStat->pprivStatFrame = NULL;
		psrcStat->pnextStatFrame = NULL;
		psrcStat->pnowStatFrame = &fhDeviceMode_poweroff;
		psrcStat->IsFrameCreate = TRUE;
		scrUnFixStat();
	}
}


void scrNextStat(void)
{
}


void scrPrivStat(void)
{
}


void scrCreateDone(void)
{
	if( !bStatFixed )
		psrcStat->IsFrameCreate = FALSE;
}

BOOL scrIsCreateDone(void)
{
	return (!psrcStat->IsFrameCreate);
}

void scrCreateInit(void)
{
	psrcStat->IsFrameCreate = TRUE;
}


void scrStepIn(void)
{
	psrcStat->pprivStatFrame = psrcStat->pnowStatFrame;
}


void scrSetStat(FRAME_HEADER* phFrame)
{
	psrcStat->pnowStatFrame = phFrame;
	scrUnFixStat();
}


void scrSetNEXT(FRAME_HEADER* phFrame)
{
	scrStepIn();
	psrcStat->pnowStatFrame = phFrame;
	psrcStat->IsFrameCreate = TRUE;
	scrUnFixStat();
}

void scrFixStat(void)
{
	bStatFixed = TRUE;
}

void scrUnFixStat(void)
{
	bStatFixed = FALSE;
}


FRAME_HEADER* scrGetStat(void)
{
	return (psrcStat->pnowStatFrame);
}

FRAME_HEADER* scrGetPrivStat(void)
{
	return (psrcStat->pprivStatFrame);
}


BOOL scrIsUpdateVaild(void)
{
	BOOL bRet = FALSE;
	
	if( psrcStat->pnowStatFrame->pfnUpdateCallback )
		bRet = TRUE;
		
	return bRet;
}


/// use for MsgCallback & DrawCallback
WORD scrMsgCbHandler(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
	WORD wRet;
	
	///go check if key/button msg first...
	wRet = psrcStat->pnowStatFrame->pfnBtnHandle(objMsg, pObj, pMsg);
	if( wRet )
		wRet = psrcStat->pnowStatFrame->pfnMsgCallback(objMsg, pObj, pMsg);
	
	return wRet;
}


void scrDrawCbHandler(void)
{
	if( psrcStat->IsFrameCreate )
	{
		psrcStat->pnowStatFrame->pfnDrawCallback(psrcStat->pnowStatFrame->wDrawOption);
		scrCreateDone();	///create->display
	}
	else
	{
		if( psrcStat->pnowStatFrame->pfnUpdateCallback )
		{	///display update
			///printf("scrDrawCbHandler display\n");
			psrcStat->pnowStatFrame->pfnUpdateCallback(psrcStat->pnowStatFrame->wDrawOption);
		}
	}
}





/// API for screen status handle

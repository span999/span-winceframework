#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>


/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "guic.h"
#include "ObjectTest.h"
///#include "FontMonacoUfontref.h"
#if 0
#include "FontGentium.h"
///#include "FontGentiumU.h"
#include "FontMonacoU.h"
#include "1bpp_icons.h"
#include "4bpp_icons.h"
#include "16164bppIcon.h"

#include "FieldCommon.h"
#include "FieldSettingMenu.h"
#include "FieldDataMode.h"
#include "FieldWatchMode.h"
#else
#include "fonts.h"
#include "icons.h"
#include "fields.h"
#endif
#include "StringID.h"


GOL_SCHEME      *altScheme;                                 // alternative style scheme
GOL_SCHEME      *alt2Scheme;                                // alternative 2 style scheme
GOL_SCHEME      *alt3Scheme;                                // alternative 3 style scheme
GOL_SCHEME      *alt4Scheme;                                // alternative 4 style scheme
GOL_SCHEME      *alt5Scheme;                                // alternative 5 style scheme
GOL_SCHEME      *timeScheme;
GOL_SCHEME      *meterScheme;                               // meter scheme
GOL_SCHEME      *navScheme;                                 // style scheme for the navigation
GOL_SCHEME      *redScheme;                                 // alternative red style scheme
GOL_SCHEME      *greenScheme;                               // alternative green style scheme
GOL_SCHEME      *yellowScheme;                              // alternative yellow style scheme
GOL_SCHEME      *popupMenuScheme;                           // popup menu style scheme
GOL_SCHEME      *dataSetScheme;								// dataset style scheme	
GOL_SCHEME      *dataSetScheme2;								// dataset style scheme	


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

#if 0
///void CreateDataSet(SHORT left, SHORT top, SHORT right, SHORT bottom, char *pText, char *pFunc, char *pData, char *pUnit)
void CreateDataSet(SHORT left, SHORT top, SHORT right, SHORT bottom, XCHAR *pText, XCHAR *pFunc, XCHAR *pData, XCHAR *pUnit)
{

    GbCreate(ID_GROUPBOX1,             	// ID 
              left,top,right,bottom,           	// dimension
              GB_DRAW|GB_RIGHT_ALIGN,                 	// will be dislayed after creation
              pText,             	// text
              meterScheme);                   	// default GOL scheme 

    StCreate(ID_STATICTEXT1,           	// ID 
              left+5,top+10,(left+5+70),(top+10+15),           	// dimension
              ST_DRAW,        	// will be dislayed, has frame
              pFunc, // multi-line text
              meterScheme);                   	// default GOL scheme 
#if 0
    StCreate(ID_STATICTEXT2,           	// ID 
			  left+((right-left)/3),top+((bottom-top)/2),right-((right-left)/3),bottom-2,
              ST_DRAW|ST_RIGHT_ALIGN,        	// will be dislayed, has frame
              pData, // multi-line text
			  alt2Scheme);                   	// default GOL scheme 
#endif
    StCreate(ID_STATICTEXT3,           	// ID 
              right-5-30,bottom-5-15,(right-5),(bottom-5),           	// dimension
              ST_DRAW|ST_RIGHT_ALIGN,        	// will be dislayed, has frame
              pUnit, // multi-line text
              meterScheme);                   	// default GOL scheme 

    StCreate(ID_STATICTEXT2,           	// ID 
			  left+((right-left)/3),top+((bottom-top)/2),right-((right-left)/3),bottom-2,
              ST_DRAW|ST_RIGHT_ALIGN,        	// will be dislayed, has frame
              pData, // multi-line text
			  alt2Scheme);                   	// default GOL scheme 

			  
}
#else
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
			pSch->pFont = (void *)&Monaco_Normal23U;
			dataframeoffset = 0;
			unitoffset = 0;
		}
		else
		{
			pSch = dataSetScheme;
			pNowFont = (void *)&Monaco_Normal20U;
			pSch->pFont = (void *)&Monaco_Normal42U;
			dataframeoffset = 5;
			unitoffset = 2;
		}	
	}
	else
	{
		pSch = dataSetScheme;
		pNowFont = (void *)&Monaco_Normal20U;
		pSch->pFont = (void *)&Monaco_Normal46U;
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
			  (left+(((right-left)-width)>>1)), (temp+(((bottom-temp)-height)>>1))+dataframeoffset, (left+(((right-left)-width)>>1))+width, (temp+(((bottom-temp)-height)>>1))+height-dataframeoffset,
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
#endif




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


void myCreateScheme( void )
{
    // create the alternate schemes
    navScheme = GOLCreateScheme();      // alternative scheme for the navigate buttons
    altScheme = GOLCreateScheme();      // create alternative 1 style scheme
    alt2Scheme = GOLCreateScheme();     // create alternative 2 style scheme
    alt3Scheme = GOLCreateScheme();     // create alternative 3 style scheme
    alt4Scheme = GOLCreateScheme();     // create alternative 4 style scheme
    alt5Scheme = GOLCreateScheme();     // create alternative 5 style scheme
    timeScheme = GOLCreateScheme();
    meterScheme = GOLCreateScheme();    // create meter scheme
    redScheme = GOLCreateScheme();      // create red style scheme
    greenScheme = GOLCreateScheme();    // create green style scheme
    yellowScheme = GOLCreateScheme();   // create yellow style scheme
	popupMenuScheme = GOLCreateScheme();   // create popup menu style scheme
	dataSetScheme = GOLCreateScheme();   // create DataSet style scheme
	dataSetScheme2 = GOLCreateScheme();   // create DataSet style scheme
	
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
	popupMenuScheme->pFont = (void *)&Monaco_Normal18U;
	popupMenuScheme->CommonBkColor = BRIGHTYELLOW;			

    dataSetScheme->Color0 = GRAY80;	//item background
    dataSetScheme->Color1 = BLACK;	//reversed
    dataSetScheme->EmbossDkColor = GRAY20;
    dataSetScheme->EmbossLtColor = GRAY20;
    dataSetScheme->ColorDisabled = BRIGHTGREEN;
    dataSetScheme->TextColor1 = GRAY80;		//reversed
    dataSetScheme->TextColor0 = BLACK;		//font color
    dataSetScheme->TextColorDisabled = BRIGHTRED;
	dataSetScheme->pFont = (void *)&Monaco_Normal46U;
	dataSetScheme->CommonBkColor = GRAY80;			

    dataSetScheme2->Color0 = GRAY80;	//item background
    dataSetScheme2->Color1 = BLACK;	//reversed
    dataSetScheme2->EmbossDkColor = GRAY20;
    dataSetScheme2->EmbossLtColor = GRAY20;
    dataSetScheme2->ColorDisabled = BRIGHTGREEN;
    dataSetScheme2->TextColor1 = GRAY80;		//reversed
    dataSetScheme2->TextColor0 = BLACK;		//font color
    dataSetScheme2->TextColorDisabled = BRIGHTRED;
	dataSetScheme2->pFont = (void *)&Monaco_Normal46U;
	dataSetScheme2->CommonBkColor = GRAY80;			

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
	
    alt4Scheme->Color0 = LIGHTBLUE;
    alt4Scheme->Color1 = BRIGHTGREEN;
    alt4Scheme->TextColor0 = BLACK;
    alt4Scheme->TextColor1 = WHITE;
///    alt4Scheme->pFont = (void *)ptrSmallAsianFont;

    alt5Scheme->Color0 = LIGHTBLUE;
    alt5Scheme->Color1 = BRIGHTRED;
    alt5Scheme->TextColor0 = BLACK;
    alt5Scheme->TextColor1 = WHITE;
///    alt5Scheme->pFont = (void *) &monofont; //monofont is equal width font, required for digitalmeter widget

    redScheme->Color0 = RGB565CONVERT(0xCC, 0x00, 0x00);
    redScheme->Color1 = BRIGHTRED;
    redScheme->EmbossDkColor = RED4;
    redScheme->EmbossLtColor = FIREBRICK1;
    redScheme->TextColor0 = RGB565CONVERT(0xC8, 0xD5, 0x85);
    redScheme->TextColor1 = BLACK;
///    redScheme->pFont = (void *)ptrLargeAsianFont;
	redScheme->pFont = (void *)Gentium_Normal25;

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

    timeScheme->Color0 = BLACK;
    timeScheme->Color1 = WHITE;
    timeScheme->TextColor0 = BRIGHTBLUE;
    timeScheme->TextColor1 = WHITE;
    timeScheme->EmbossDkColor = GRAY20;
    timeScheme->EmbossLtColor = GRAY80;
///    timeScheme->pFont = (void *) &GOLSmallFont;

    meterScheme->Color0 = BLACK;
    meterScheme->Color1 = WHITE;
///    meterScheme->TextColor0 = BRIGHTBLUE;
	meterScheme->TextColor0 = BRIGHTYELLOW;
    meterScheme->TextColor1 = WHITE;
    meterScheme->EmbossDkColor = GRAY20;
    meterScheme->EmbossLtColor = GRAY80;
///    meterScheme->pFont = (void *)ptrSmallAsianFont;
	meterScheme->pFont = (void *)Gentium_Normal15;
	meterScheme->CommonBkColor = BLACK;

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
		psrcStat->privStat = CREATE_BUTTONS;
		psrcStat->nowStat = CREATE_BUTTONS;
		psrcStat->nextStat = CREATE_BUTTONS;
///		psrcStat->pnowStatFrame = &fhButtons;
		psrcStat->pnowStatFrame = &fhDeviceMode_poweroff;
///		psrcStat->pnowStatFrame = &fhDataMode_one;
///		psrcStat->pnowStatFrame = &fhDataMode_two;
		psrcStat->IsFrameCreate = TRUE;
	}
}


void scrNextStat(void)
{
#if 0
	if( psrcStat )
	{
		switch(psrcStat->nowStat)
		{
			case CREATE_BUTTONS:
				psrcStat->privStat = psrcStat->nowStat;
				psrcStat->nowStat = DISPLAY_BUTTONS;
				psrcStat->nextStat = CREATE_RENO_DATASET;
				psrcStat->pnowStatFrame = &fhButtons;
				psrcStat->IsFrameCreate = FALSE;
				break;
			case DISPLAY_BUTTONS:
				psrcStat->privStat = psrcStat->nowStat;
				psrcStat->nowStat = psrcStat->nextStat;
				psrcStat->nextStat = DISPLAY_RENO_DATASET;
				psrcStat->pnowStatFrame = &fhRenoDataSet;
				psrcStat->IsFrameCreate = TRUE;
				break;
			case CREATE_RENO_DATASET:
				psrcStat->privStat = psrcStat->nowStat;
				psrcStat->nowStat = psrcStat->nextStat;
				psrcStat->nextStat = CREATE_LISTBOX;
				psrcStat->pnowStatFrame = &fhRenoDataSet;
				psrcStat->IsFrameCreate = FALSE;
				break;
			case DISPLAY_RENO_DATASET:
				psrcStat->privStat = psrcStat->nowStat;
				psrcStat->nowStat = psrcStat->nextStat;
				psrcStat->nextStat = DISPLAY_LISTBOX;
				psrcStat->pnowStatFrame = &fhListBox;
				psrcStat->IsFrameCreate = TRUE;
				break;
			case CREATE_LISTBOX:
				psrcStat->privStat = psrcStat->nowStat;
				psrcStat->nowStat = psrcStat->nextStat;
				psrcStat->nextStat = CREATE_TEXTENTRYPAD;
				psrcStat->pnowStatFrame = &fhListBox;
				psrcStat->IsFrameCreate = FALSE;
				break;
			case DISPLAY_LISTBOX:
				psrcStat->privStat = psrcStat->nowStat;
				psrcStat->nowStat = psrcStat->nextStat;
				psrcStat->nextStat = DISPLAY_TEXTENTRYPAD;
				psrcStat->pnowStatFrame = &fhTextEnteryPad;
				psrcStat->IsFrameCreate = TRUE;
				break;
			case CREATE_TEXTENTRYPAD:
				psrcStat->privStat = psrcStat->nowStat;
				psrcStat->nowStat = psrcStat->nextStat;
				psrcStat->nextStat = CREATE_NUMENTRYPAD;
				psrcStat->pnowStatFrame = &fhTextEnteryPad;
				psrcStat->IsFrameCreate = FALSE;
				break;
			case DISPLAY_TEXTENTRYPAD:
				psrcStat->privStat = psrcStat->nowStat;
				psrcStat->nowStat = psrcStat->nextStat;
				psrcStat->nextStat = DISPLAY_NUMENTRYPAD;
				psrcStat->pnowStatFrame = &fhNumEnteryPad;
				psrcStat->IsFrameCreate = TRUE;
				break;
			case CREATE_NUMENTRYPAD:
				psrcStat->privStat = psrcStat->nowStat;
				psrcStat->nowStat = psrcStat->nextStat;
				psrcStat->nextStat = CREATE_BUTTONS;
				psrcStat->pnowStatFrame = &fhNumEnteryPad;
				psrcStat->IsFrameCreate = FALSE;
				break;
			case DISPLAY_NUMENTRYPAD:
				psrcStat->privStat = psrcStat->nowStat;
				psrcStat->nowStat = psrcStat->nextStat;
				psrcStat->nextStat = DISPLAY_BUTTONS;
				psrcStat->pnowStatFrame = &fhButtons;
				psrcStat->IsFrameCreate = TRUE;
				break;
			default:
				break;
		}
	}
#endif	
}


void scrPrivStat(void)
{
#if 0
	if( psrcStat )
	{
		switch(psrcStat->nowStat)
		{
			case CREATE_BUTTONS:
				break;
			case DISPLAY_BUTTONS:
				psrcStat->privStat = DISPLAY_TEXTENTRYPAD;
				psrcStat->nowStat = CREATE_NUMENTRYPAD;
				psrcStat->nextStat = DISPLAY_NUMENTRYPAD;
				psrcStat->pnowStatFrame = &fhNumEnteryPad;
				psrcStat->IsFrameCreate = TRUE;
				break;
			case CREATE_RENO_DATASET:
				break;
			case DISPLAY_RENO_DATASET:
				psrcStat->privStat = DISPLAY_NUMENTRYPAD;
				psrcStat->nowStat = CREATE_BUTTONS;
				psrcStat->nextStat = DISPLAY_BUTTONS;
				psrcStat->pnowStatFrame = &fhButtons;
				psrcStat->IsFrameCreate = TRUE;
				break;
			case CREATE_LISTBOX:
				break;
			case DISPLAY_LISTBOX:
				psrcStat->privStat = DISPLAY_BUTTONS;
				psrcStat->nowStat = CREATE_RENO_DATASET;
				psrcStat->nextStat = DISPLAY_RENO_DATASET;
				psrcStat->pnowStatFrame = &fhRenoDataSet;
				psrcStat->IsFrameCreate = TRUE;
				break;
			case CREATE_TEXTENTRYPAD:
				break;
			case DISPLAY_TEXTENTRYPAD:
				psrcStat->privStat = DISPLAY_RENO_DATASET;
				psrcStat->nowStat = CREATE_LISTBOX;
				psrcStat->nextStat = DISPLAY_LISTBOX;
				psrcStat->pnowStatFrame = &fhListBox;
				psrcStat->IsFrameCreate = TRUE;
				break;
			case CREATE_NUMENTRYPAD:
				break;
			case DISPLAY_NUMENTRYPAD:
				psrcStat->privStat = DISPLAY_LISTBOX;
				psrcStat->nowStat = CREATE_TEXTENTRYPAD;
				psrcStat->nextStat = DISPLAY_TEXTENTRYPAD;
				psrcStat->pnowStatFrame = &fhTextEnteryPad;
				psrcStat->IsFrameCreate = TRUE;
				break;
			default:
				break;
		}
	}
#endif	
}

void scrCreateDone(void)
{
	psrcStat->IsFrameCreate = FALSE;
}

void scrCreateInit(void)
{
	psrcStat->IsFrameCreate = TRUE;
}

void scrSetStat(FRAME_HEADER* phFrame)
{
	psrcStat->pnowStatFrame = phFrame;
}

void scrSetNEXT(FRAME_HEADER* phFrame)
{
	psrcStat->pnowStatFrame = phFrame;
	psrcStat->IsFrameCreate = TRUE;
}


FRAME_HEADER* scrGetStat(void)
{
	return (psrcStat->pnowStatFrame);
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
///		scrNextStat( psrcStat );	///create->display
		scrCreateDone();	///create->display
	}
}





/// API for screen status handle

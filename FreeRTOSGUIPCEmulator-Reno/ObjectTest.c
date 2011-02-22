#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

#define		USE_MAGELLAN_LOGO

/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "guic.h"
#include "ObjectTest.h"
#include "FontGentium.h"
#include "1bpp_icons.h"
#include "4bpp_icons.h"
#include "16164bppIcon.h"
#ifdef USE_MAGELLAN_LOGO
#include "magellan_logo.h"
#endif

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
OBJ_HEADER      *pNavList;                                  // pointer to navigation list
volatile DWORD  tick = 0;                                   // tick counter
char            animate;                                    // switch to control animation for picture demo
OBJ_HEADER      *pGenObj;                                   // pointer to a general purpose object
///PICTURE         *pPicture;                                  // pointer to picture object for picture demo
///PROGRESSBAR     *pProgressBar;                              // pointer to progress bar object for progress bar demo
///SLIDER          *pSlider;                                   // pointer to the slider controlling the animation speed

SCREEN_STATUS	scrStatus;
SCREEN_STATUS* psrcStat;

SCREEN_STATES   screenState = CREATE_BUTTONS;               // current state of main demo state mashine

SCREEN_STATES   prevState = CREATE_BUTTONS; // used to mark state where time setting was called	
SCREEN_STATES   prevRefreshState = CREATE_BUTTONS; // used to mark the start of the previous screen


///#define DEFAULTBTN_WIDTH	(GetMaxY()/2)
#define DEFAULTBTN_WIDTH	70
#define DEFAULTBTN_HEIGHT	20
#define HWBTN_WIDTH		20
#define HWBTN_HEIGHT	15


void CreateDefaultBtn(void)
{
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
    GOLSetFocus(obj);   // set focus for the button
#endif  
  
}


WORD MsgDefaultBtn(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
				scrNextStat( psrcStat );
            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
				scrPrivStat( psrcStat );
            return (1);                             // process by default

        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
				scrPrivStat( psrcStat );
			return (0);
			
        case ID_BTN_UP_HOLD:
			return (0); 
			
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
				scrNextStat( psrcStat );
			return (0); 
			
        case ID_BTN_DOWN_HOLD:
			return (0); 
			
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
				scrPrivStat( psrcStat );
			return (0);  

        case ID_BTN_EXIT_HOLD:
        case ID_BTN_ENTER:
        case ID_BTN_ENTER_HOLD:
            return (0);                             // do nothing since it's HW key simulate
	
        default:
            return (1);                 // process by default
    }
}


void CreateDataSet(SHORT left, SHORT top, SHORT right, SHORT bottom, char *pText, char *pFunc, char *pData, char *pUnit)
{

    GbCreate(ID_GROUPBOX1,             	// ID 
              left,top,right,bottom,           	// dimension
              GB_DRAW|GB_RIGHT_ALIGN,                 	// will be dislayed after creation
              pText,             	// text
              meterScheme);                   	// default GOL scheme 

    StCreate(ID_STATICTEXT1,           	// ID 
              left+5,top+15,(left+5+70),(top+15+15),           	// dimension
              ST_DRAW,        	// will be dislayed, has frame
              pFunc, // multi-line text
              meterScheme);                   	// default GOL scheme 

    StCreate(ID_STATICTEXT2,           	// ID 
///             200+20,150+35,(200+20+45),(150+35+25),           	// dimension
///			  left+((right-left)/3),top+((bottom-top)/2),left+((right-left)/3)+55,top+((bottom-top)/2)+25,
			  left+((right-left)/3),top+((bottom-top)/2),left+((right-left)/3)+55,bottom-2,
///              ST_DRAW|ST_FRAME|ST_RIGHT_ALIGN,        	// will be dislayed, has frame
              ST_DRAW|ST_RIGHT_ALIGN,        	// will be dislayed, has frame
              pData, // multi-line text
              ///NULL);                   	// default GOL scheme 
			  alt2Scheme);                   	// default GOL scheme 

    StCreate(ID_STATICTEXT3,           	// ID 
              right-5-30,bottom-5-15,(right-5),(bottom-5),           	// dimension
              ST_DRAW|ST_RIGHT_ALIGN,        	// will be dislayed, has frame
              pUnit, // multi-line text
              meterScheme);                   	// default GOL scheme 

}



void CreateButtons(WORD wDrawOption)
{
    #define BTN_ORIGIN_X    (GetMaxX() / 2)
    #define BTN_ORIGIN_Y    (GetMaxY() / 2)
	#define BTN_ORIGIN_W    50
	#define BTN_ORIGIN_H    30
	
    GOLFree();                      // free memory for the objects in the previous linked list and start new list
///    CreatePage((XCHAR *)ButtonStr);
	SetColor(BLACK);
	ClearDevice();
	CreateDefaultBtn();

    BtnCreate
    (
        ID_BUTTON1,                 // button ID
        BTN_ORIGIN_X - 70,
        BTN_ORIGIN_Y - (BTN_ORIGIN_H/2) - 40,
        BTN_ORIGIN_X - 70 + BTN_ORIGIN_W,
        BTN_ORIGIN_Y - (BTN_ORIGIN_H/2) + BTN_ORIGIN_H - 40,			// dimension
        10,                         // set radius
        BTN_DRAW,                   // draw a beveled button
        NULL,                       // no bitmap
///        (XCHAR *)ButtonStr,         // text
        (XCHAR *)"Test",         // text
        altScheme
    );                              // use alternate scheme
    BtnCreate
    (
        ID_BUTTON2,                 // button ID
        BTN_ORIGIN_X + (BTN_ORIGIN_X-70),
        BTN_ORIGIN_Y - (BTN_ORIGIN_H/2) - 40,
        BTN_ORIGIN_X + (BTN_ORIGIN_X-70) + BTN_ORIGIN_W,
        BTN_ORIGIN_Y- (BTN_ORIGIN_H/2) + BTN_ORIGIN_H - 40,         // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
///        (void *) &gradientButton,   // use bitmap
        (void *)&ECG_1bpp_16x16,   // use bitmap
///        (XCHAR *)HomeStr,           // text
        (XCHAR *)NULL,           // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_BUTTON3,                 // button ID
        BTN_ORIGIN_X - 70,
        BTN_ORIGIN_Y - (BTN_ORIGIN_H/2) + 40,
        BTN_ORIGIN_X - 70 + BTN_ORIGIN_W,
        BTN_ORIGIN_Y - (BTN_ORIGIN_H/2) + BTN_ORIGIN_H + 40,			// dimension
        0,                         // set radius
        BTN_DRAW | BTN_TOGGLE,      // draw a vertical capsule button
        							// that has a toggle behavior
        NULL, 						// no bitmap
///        (XCHAR *)LowStr,            // text
        (XCHAR *)"Test",            // text
        yellowScheme
    );                          	// use alternate scheme
    BtnCreate
    (
        ID_BUTTON4,             	// button ID
        BTN_ORIGIN_X + (BTN_ORIGIN_X-70),
        BTN_ORIGIN_Y - (BTN_ORIGIN_H/2) + 40,
        BTN_ORIGIN_X + (BTN_ORIGIN_X-70) + BTN_ORIGIN_W,
        BTN_ORIGIN_Y - (BTN_ORIGIN_H/2) + BTN_ORIGIN_H + 40,			// dimension
        5,                     	// set radius
        BTN_DRAW,               	// draw a vertical capsule button
        NULL,                   	// no bitmap
///        (XCHAR *)OnStr,         	// text
        (XCHAR *)NULL,         	// text
        greenScheme
    );                          	// use alternate scheme

}

/* */
WORD MsgButtons(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    OBJ_HEADER  *pOtherRbtn;
	static BOOL bPressed = FALSE;

    switch(GetObjID(pObj))
    {			
        case ID_BUTTON1:
            return (1);                 // process by default

        case ID_BUTTON2:
            return (1);                 // process by default
			
        case ID_BUTTON3:
            if(objMsg == BTN_MSG_PRESSED)
            {                           // change text and scheme
                ///BtnSetText((BUTTON *)pObj, (XCHAR *)HighStr);
				if( TRUE == bPressed )
				{
					BtnSetText((BUTTON *)pObj, (XCHAR *)"Go");
					bPressed = FALSE;
				}
				else
				{
					BtnSetText((BUTTON *)pObj, (XCHAR *)"Stop");
					bPressed = TRUE;
				}
            }
            else
            {
                ;///BtnSetText((BUTTON *)pObj, (XCHAR *)LowStr);
				;///BtnSetText((BUTTON *)pObj, (XCHAR *)"Stop");
            }

            return (1);                 // process by default

        case ID_BUTTON4:
            if(objMsg == BTN_MSG_PRESSED)
            {
                ///if(!GetState(pObj, BTN_PRESSED))
				if(0)
                {
                    pOtherRbtn = GOLFindObject(ID_BUTTON5);
                    ClrState(pOtherRbtn, BTN_PRESSED);
                    SetState(pOtherRbtn, BTN_DRAW);
                    SetState(pObj, BTN_PRESSED | BTN_DRAW);
#if defined (USE_FOCUS)                    
                    GOLSetFocus(pObj);  // set focus for the button
#endif                    
                }
            }

            return (0);                 // Do not process by default

		default:
            return (1);                 // process by default
    }
}

WORD MsgButtonsDefaultBtn(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
    }
}



void CreateRenoDataSet(WORD wDrawOption)
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	///CreateStatusBar("Reno");
	SetColor(BLACK);
	ClearDevice();
	CreateDefaultBtn();
#if 0
	CreateDataSet( 30, 40,( 30+260),( 40+65),"DataSet1", "Time", "23:23", "24h");
	CreateDataSet( 30,105+2,( 30+130-2),(105+65),"DataSet2", "Elevation", "155", "ft");
	CreateDataSet(160,105+2,(160+130),(105+65),"DataSet3", "Lap Avg HR", "128", "bpm");
	CreateDataSet( 30,170+2,( 30+130-2),(170+65),"DataSet4", "Distance", "3.12", "mi");
	CreateDataSet(160,170+2,(160+130),(170+65),"DataSet5", "Avg HR", "88", "bpm");
#else
	CreateDataSet( 0,  0,( 0+143),( 0+48),"DataSet1", "Time", "23:23", "24h");
	CreateDataSet( 0, 50,( 0+143),(50+48),"DataSet2", "Elevation", "155", "ft");
	CreateDataSet( 0, 98,( 0+143),(98+47),"DataSet3", "Lap Avg HR", "128", "bpm");

#endif
}



WORD MsgRenoDataSet(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{

    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}


WORD MsgRenoDataSetDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {	
		///case ID_BUTTON: here, if you want different key response.
        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
    }
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



// creates list box demo screen
void CreateListBox(WORD wDrawOption)
{
    LISTBOX *pLb;

    GOLFree();                                      // free memory for the objects in the previous linked list and start new list
	
	CreateDefaultBtn();
///    CreatePage((XCHAR *)ListBoxStr);                //
    pLb = LbCreate
        (
            ID_LISTBOX1,                            // ID
            0,
            0,
            GetMaxX(),
            GetMaxY()-DEFAULTBTN_HEIGHT,                      // dimension
            LB_DRAW | LB_FOCUSED,                   // will be dislayed after creation
            (XCHAR*)"Activity Data Screen",
            alt2Scheme
        );                                          // use alternate scheme
	
	
	AddItemList( (XCHAR *)"Screen 1 >", pLb, &PCGaming2_1bpp_16x16);
	AddItemList( (XCHAR *)"Screen 2 >", pLb, &PCGaming1_1bpp_16x16);
	AddItemList( (XCHAR *)"Add Screen >", pLb, NULL);
	LbSetFocusedItem( pLb, 1 );
}

// processes messages for list box demo screen
WORD MsgListBox(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
///    LISTBOX *pLb;

///    pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);

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



WORD MsgListBoxDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
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
				LbChangeSel(pLb, pLb->pFocusItem);
				SetState(pLb, LB_DRAW_ITEMS);
			}
			return (0); 
        default:
            return (MsgDefaultBtn(objMsg, pObj, pMsg));                 // process by default
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




//*****************************************************************************
//
//! Sends a pointer message.
//!
//! \param ulMessage is the pointer message to be sent.
//! \param lX is the X coordinate associated with the message.
//! \param lY is the Y coordinate associated with the message.
//!
//! This function sends a pointer message to the root widget.  A pointer driver
//! (such as a touch screen driver) can use this function to deliver pointer
//! activity to the widget tree without having to have direct knowledge of the
//! structure of the widget framework.
//!
//! \return Returns 1 if the message was added to the queue, and 0 if it could
//! not be added since the queue is full.
//
//*****************************************************************************

static BOOL ioMsgUpdated = FALSE;
static GOL_MSG ioMsg; 

void TransIOMsg(unsigned long ulMessage, long lX, long lY)
{
    //
    // Add the message to the widget message queue.
    //
    //return(WidgetMessageQueueAdd(WIDGET_ROOT, ulMessage, lX, lY, 1, 1));
	
	///printf("In TransIOMsg\n");
	
	if( WIDGET_MSG_PTR_DOWN == ulMessage )
	{
		printf("  mice down: x=%d y=%d\n", lX, lY );
		ioMsg.type = TYPE_TOUCHSCREEN;
		ioMsg.uiEvent = EVENT_PRESS;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)lY;
	}
	else
	if( WIDGET_MSG_PTR_UP == ulMessage )
	{
		printf("  mice up: x=%d y=%d\n", lX, lY );
		ioMsg.type = TYPE_TOUCHSCREEN;
		ioMsg.uiEvent = EVENT_RELEASE;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)lY;
	}
	else
	if( WIDGET_MSG_PTR_MOVE == ulMessage )
	{
		///printf("  mice move: x=%d y=%d\n", lX, lY );
		ioMsg.type = TYPE_TOUCHSCREEN;
		ioMsg.uiEvent = EVENT_MOVE;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)lY;
	}
	else
	if( WIDGET_MSG_KEY_DOWN == ulMessage )
	{
		SHORT	kbscan;
		SHORT	kbstat;
		
		kbscan = (lY&0xFFFF);
		kbstat = ((lY>>16)&0xFFFF);
		printf("  keybd down: ID=%d kbscan=%d kbstat=0x%x\n", lX, kbscan, kbstat );
		ioMsg.type = TYPE_KEYBOARD;
		ioMsg.uiEvent = EVENT_KEYSCAN;
		ioMsg.param1 = (SHORT)lX;
///		ioMsg.param2 = (SHORT)lY;
		ioMsg.param2 = (SHORT)kbscan;
		
		/*re-mapping key, simulate the key pressed/repeased*/
		if( KB_KEY_1 == ioMsg.param2 )
		{	/*simulate the "BACK" key repeased*/
			ioMsg.param1 = ID_BUTTON_BACK;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate BACK key\n");
		}
		else
		if( KB_KEY_2 == ioMsg.param2 )
		{	/*simulate the "NEXT" key repeased*/
			ioMsg.param1 = ID_BUTTON_NEXT;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate NEXT key\n");
		}
		else
		if( KB_KEY_I == ioMsg.param2 )
		{	/*simulate the "UP" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_UP_HOLD;
			else
				ioMsg.param1 = ID_BTN_UP;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate UP key\n");
		}
		else
		if( KB_KEY_O == ioMsg.param2 )
		{	/*simulate the "UP" key hold*/
			ioMsg.param1 = ID_BTN_UP_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate UP_HOLD key\n");
		}
		else
		if( KB_KEY_K == ioMsg.param2 )
		{	/*simulate the "DOWN" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_DOWN_HOLD;
			else
				ioMsg.param1 = ID_BTN_DOWN;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate DOWN key\n");
		}
		else
		if( KB_KEY_L == ioMsg.param2 )
		{	/*simulate the "DOWN" key hold*/
			ioMsg.param1 = ID_BTN_DOWN_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate DOWN_HOLD key\n");
		}
		else
		if( KB_KEY_U == ioMsg.param2 )
		{	/*simulate the "EXIT" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_EXIT_HOLD;
			else
				ioMsg.param1 = ID_BTN_EXIT;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate EXIT key\n");
		}
		else
		if( KB_KEY_Y == ioMsg.param2 )
		{	/*simulate the "EXIT" key hold*/
			ioMsg.param1 = ID_BTN_EXIT_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate EXIT_HOLD key\n");
		}
		else
		if( KB_KEY_J == ioMsg.param2 )
		{	/*simulate the "ENTER" key repeased*/
			if( (kbstat&0x3) > 0 )
				ioMsg.param1 = ID_BTN_ENTER_HOLD;
			else
				ioMsg.param1 = ID_BTN_ENTER;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate ENTER key\n");
		}
		else
		if( KB_KEY_H == ioMsg.param2 )
		{	/*simulate the "ENTER" key hold*/
			ioMsg.param1 = ID_BTN_ENTER_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
			printf("  keybd down: simulate ENTER_HOLD key\n");
		}
		else
			ioMsg.param1 = 0;
	}
	else
	{
		printf("  io unknow: x=%d y=%d\n", lX, lY );
		ioMsg.type = TYPE_UNKNOWN;
		ioMsg.uiEvent = EVENT_INVALID;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)lY;
	}
		
	ioMsgUpdated = TRUE;
	
#if 0
    tWidgetMessageQueue message;


        message.ulFlags =  MQ_FLAG_POST_ORDER  | MQ_FLAG_STOP_ON_SUCCESS ;
    	message.pWidget = WIDGET_ROOT;
    	message.ulMessage = ulMessage;
    	message.ulParam1 = lX;
    	message.ulParam2 = lY;



        //
        // Write this message into the message queue.
        //
        if( errQUEUE_FULL == xQueueSend( grlibQueue,(const void *)&message, 0 ))
        {
            g_ulMQOverflow++;
        }

   //     portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
#endif
        //
        // Success.
        //
        return;
}

void IOGetMsg( GOL_MSG *msg )
{
	/* TODO: move it to queue wait!! */
	while( !ioMsgUpdated )
	{
		/* printf("Wait touch event!!\n"); */
	}
	
	if( TRUE == ioMsgUpdated )
	{
		msg->type = ioMsg.type;
		msg->uiEvent = ioMsg.uiEvent;
		msg->param1 = ioMsg.param1;
		msg->param2 = ioMsg.param2;
		
		if( EVENT_MOVE != msg->uiEvent )
			printf("  io event %d,%d,%d,%d\n", msg->type, msg->uiEvent, msg->param1, msg->param2);
		
		ioMsgUpdated = FALSE;
	}
}


WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
///	printf("In GOLMsgCallback\n");
	return scrMsgCbHandler(objMsg, pObj, pMsg);
}


WORD GOLDrawCallback(void)
{
///	printf("In GOLDrawCallback\n");

#if 0	
    switch(screenState)
    {
        case CREATE_BUTTONS:
            prevRefreshState = CREATE_BUTTONS;
            CreateButtons();        									// create window and buttons
            screenState = DISPLAY_BUTTONS;                              // switch to next state
            return (1);                                                 // draw objects created

        case DISPLAY_BUTTONS:
            return (1);                                                 // redraw objects if needed
			
        case CREATE_RENO_DATASET:
			prevRefreshState = CREATE_BUTTONS;
            CreateRenoDataSet(); 							// create window and buttons
            screenState = DISPLAY_RENO_DATASET; 			// switch to next state
            return (1); 									// draw objects created
            
        case DISPLAY_RENO_DATASET:
            return (1); 									// redraw objects if needed

        case CREATE_LISTBOX:
            prevRefreshState = CREATE_LISTBOX;
            CreateListBox();                    // create list box test screen
            screenState = DISPLAY_LISTBOX;      // switch to next state
            return (1);                         // draw objects created

        case DISPLAY_LISTBOX:

            // this moves the slider and editbox for the date setting to
            // move while the up or down arrow buttons are pressed
			{    
				LISTBOX *pLb;
				pLb = (LISTBOX *)GOLFindObject(ID_LISTBOX1);
				SetState(pLb, LB_DRAW_ITEMS);
			}	
            return (1);                         // draw objects
			
		default:
			return (1);    
	}
#else
	scrDrawCbHandler();
#endif
	return (1);
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

    /* for Truly display */
    altScheme->Color0 = RGB565CONVERT(0x4C, 0x8E, 0xFF);
    altScheme->Color1 = RGB565CONVERT(255, 102, 0);
    altScheme->EmbossDkColor = RGB565CONVERT(0x1E, 0x00, 0xE5);
    altScheme->EmbossLtColor = RGB565CONVERT(0xA9, 0xDB, 0xEF);
    altScheme->ColorDisabled = RGB565CONVERT(0xD4, 0xE1, 0xF7);
    altScheme->TextColor1 = BRIGHTBLUE;
    altScheme->TextColor0 = RGB565CONVERT(255, 102, 0);
    altScheme->TextColorDisabled = RGB565CONVERT(0xB8, 0xB9, 0xBC);

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
///    alt3Scheme->pFont = (void *)ptrSmallAsianFont;

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


#define MIN(x,y)                ((x > y)? y: x)
#define WAIT_UNTIL_FINISH(x)    while(!x)	
#define DEMODELAY				1000

void ObjectTest( void )
{
    SHORT       width, height;
	
	GOL_MSG msg;                        // GOL message structure to interact with GOL

	psrcStat = &scrStatus;
	scrInitStat( psrcStat );
	
/***********************************************************************************************************/
	SetColor(BLACK);
	ClearDevice();
	// draw fonts in the screen
	SetFont((void *) &Gentium_Normal15);
	SetColor(LIGHTRED);
	width = GetTextWidth("This Demo bases on", (void *) &Gentium_Normal15);
	height = GetTextHeight((void *) &Gentium_Normal15);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-20, "This Demo bases on");
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	SetFont((void *) &Gentium_Normal15);
	SetColor(LIGHTGREEN);
	width = GetTextWidth("freeRTOS:v6.1.0", (void *) &Gentium_Normal15);
	height = GetTextHeight((void *) &Gentium_Normal15);
	OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "freeRTOS:v6.1.0");
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	

	// draw fonts in the screen
	SetFont((void *) &Gentium_Normal15);
	SetColor(LIGHTBLUE);
	width = GetTextWidth("MicroChip AppLib:v2.11", (void *) &Gentium_Normal15);
	height = GetTextHeight((void *) &Gentium_Normal15);
	OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+20, "MicroChip AppLib:v2.11");
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	


	
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
	
	
/***********************************************************************************************************/
    GOLInit();                          // Initialize graphics library and crete default style scheme for GOL
	
	myCreateScheme();
	
    while(1)
    {
///		printf("In message loop\n");
        if(GOLDraw())
        {                               // Draw GOL objects
            // Drawing is done here, process messages
            IOGetMsg(&msg);          // Get message from touch screen
            GOLMsg(&msg);               // Process message
#if defined (USE_FOCUS)                    
            #if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__))
///            SideButtonsMsg(&msg);       // Get message from side buttons
///            GOLMsg(&msg);               // Process message
            #endif
#endif //#if defined (USE_FOCUS)
        }
		else
		{
			printf("In message loop: !GOLDraw()\n");
		}
    }
	
}


FRAME_HEADER fhButtons = {
	MsgButtons,
	CreateButtons,
	MsgButtonsDefaultBtn,
	0,
	0,
	0
};

FRAME_HEADER fhRenoDataSet = {
	MsgRenoDataSet,
	CreateRenoDataSet,
	MsgRenoDataSetDefaultBtn,
	0,
	0,
	0
};

FRAME_HEADER fhListBox = {
	MsgListBox,
	CreateListBox,
	MsgListBoxDefaultBtn,
	0,
	0,
	0
};


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



/// API for screen status handle
void scrInitStat(SCREEN_STATUS* pScreenStat)
{
	if( pScreenStat )
	{
		pScreenStat->privStat = CREATE_BUTTONS;
		pScreenStat->nowStat = CREATE_BUTTONS;
		pScreenStat->nextStat = CREATE_BUTTONS;
		pScreenStat->pnowStatFrame = &fhButtons;
		pScreenStat->IsFrameCreate = TRUE;
	}
}


void scrNextStat(SCREEN_STATUS* pScreenStat)
{
	if( pScreenStat )
	{
		switch(pScreenStat->nowStat)
		{
			case CREATE_BUTTONS:
				pScreenStat->privStat = pScreenStat->nowStat;
				pScreenStat->nowStat = DISPLAY_BUTTONS;
				pScreenStat->nextStat = CREATE_RENO_DATASET;
				pScreenStat->pnowStatFrame = &fhButtons;
				pScreenStat->IsFrameCreate = FALSE;
				break;
			case DISPLAY_BUTTONS:
				pScreenStat->privStat = pScreenStat->nowStat;
				pScreenStat->nowStat = pScreenStat->nextStat;
				pScreenStat->nextStat = DISPLAY_RENO_DATASET;
				pScreenStat->pnowStatFrame = &fhRenoDataSet;
				pScreenStat->IsFrameCreate = TRUE;
				break;
			case CREATE_RENO_DATASET:
				pScreenStat->privStat = pScreenStat->nowStat;
				pScreenStat->nowStat = pScreenStat->nextStat;
				pScreenStat->nextStat = CREATE_LISTBOX;
				pScreenStat->pnowStatFrame = &fhRenoDataSet;
				pScreenStat->IsFrameCreate = FALSE;
				break;
			case DISPLAY_RENO_DATASET:
				pScreenStat->privStat = pScreenStat->nowStat;
				pScreenStat->nowStat = pScreenStat->nextStat;
				pScreenStat->nextStat = DISPLAY_LISTBOX;
				pScreenStat->pnowStatFrame = &fhListBox;
				pScreenStat->IsFrameCreate = TRUE;
				break;
			case CREATE_LISTBOX:
				pScreenStat->privStat = pScreenStat->nowStat;
				pScreenStat->nowStat = pScreenStat->nextStat;
				pScreenStat->nextStat = CREATE_TEXTENTRYPAD;
				pScreenStat->pnowStatFrame = &fhListBox;
				pScreenStat->IsFrameCreate = FALSE;
				break;
			case DISPLAY_LISTBOX:
				pScreenStat->privStat = pScreenStat->nowStat;
				pScreenStat->nowStat = pScreenStat->nextStat;
				pScreenStat->nextStat = DISPLAY_TEXTENTRYPAD;
				pScreenStat->pnowStatFrame = &fhTextEnteryPad;
				pScreenStat->IsFrameCreate = TRUE;
				break;
			case CREATE_TEXTENTRYPAD:
				pScreenStat->privStat = pScreenStat->nowStat;
				pScreenStat->nowStat = pScreenStat->nextStat;
				pScreenStat->nextStat = CREATE_NUMENTRYPAD;
				pScreenStat->pnowStatFrame = &fhTextEnteryPad;
				pScreenStat->IsFrameCreate = FALSE;
				break;
			case DISPLAY_TEXTENTRYPAD:
				pScreenStat->privStat = pScreenStat->nowStat;
				pScreenStat->nowStat = pScreenStat->nextStat;
				pScreenStat->nextStat = DISPLAY_NUMENTRYPAD;
				pScreenStat->pnowStatFrame = &fhNumEnteryPad;
				pScreenStat->IsFrameCreate = TRUE;
				break;
			case CREATE_NUMENTRYPAD:
				pScreenStat->privStat = pScreenStat->nowStat;
				pScreenStat->nowStat = pScreenStat->nextStat;
				pScreenStat->nextStat = CREATE_BUTTONS;
				pScreenStat->pnowStatFrame = &fhNumEnteryPad;
				pScreenStat->IsFrameCreate = FALSE;
				break;
			case DISPLAY_NUMENTRYPAD:
				pScreenStat->privStat = pScreenStat->nowStat;
				pScreenStat->nowStat = pScreenStat->nextStat;
				pScreenStat->nextStat = DISPLAY_BUTTONS;
				pScreenStat->pnowStatFrame = &fhButtons;
				pScreenStat->IsFrameCreate = TRUE;
				break;
			default:
				break;
		}
	}
}


void scrPrivStat(SCREEN_STATUS* pScreenStat)
{
	if( pScreenStat )
	{
		switch(pScreenStat->nowStat)
		{
			case CREATE_BUTTONS:
				break;
			case DISPLAY_BUTTONS:
				pScreenStat->privStat = DISPLAY_TEXTENTRYPAD;
				pScreenStat->nowStat = CREATE_NUMENTRYPAD;
				pScreenStat->nextStat = DISPLAY_NUMENTRYPAD;
				pScreenStat->pnowStatFrame = &fhNumEnteryPad;
				pScreenStat->IsFrameCreate = TRUE;
				break;
			case CREATE_RENO_DATASET:
				break;
			case DISPLAY_RENO_DATASET:
				pScreenStat->privStat = DISPLAY_NUMENTRYPAD;
				pScreenStat->nowStat = CREATE_BUTTONS;
				pScreenStat->nextStat = DISPLAY_BUTTONS;
				pScreenStat->pnowStatFrame = &fhButtons;
				pScreenStat->IsFrameCreate = TRUE;
				break;
			case CREATE_LISTBOX:
				break;
			case DISPLAY_LISTBOX:
				pScreenStat->privStat = DISPLAY_BUTTONS;
				pScreenStat->nowStat = CREATE_RENO_DATASET;
				pScreenStat->nextStat = DISPLAY_RENO_DATASET;
				pScreenStat->pnowStatFrame = &fhRenoDataSet;
				pScreenStat->IsFrameCreate = TRUE;
				break;
			case CREATE_TEXTENTRYPAD:
				break;
			case DISPLAY_TEXTENTRYPAD:
				pScreenStat->privStat = DISPLAY_RENO_DATASET;
				pScreenStat->nowStat = CREATE_LISTBOX;
				pScreenStat->nextStat = DISPLAY_LISTBOX;
				pScreenStat->pnowStatFrame = &fhListBox;
				pScreenStat->IsFrameCreate = TRUE;
				break;
			case CREATE_NUMENTRYPAD:
				break;
			case DISPLAY_NUMENTRYPAD:
				pScreenStat->privStat = DISPLAY_LISTBOX;
				pScreenStat->nowStat = CREATE_TEXTENTRYPAD;
				pScreenStat->nextStat = DISPLAY_TEXTENTRYPAD;
				pScreenStat->pnowStatFrame = &fhTextEnteryPad;
				pScreenStat->IsFrameCreate = TRUE;
				break;
			default:
				break;
		}
	}
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
		scrNextStat( psrcStat );	///create->display
	}
}



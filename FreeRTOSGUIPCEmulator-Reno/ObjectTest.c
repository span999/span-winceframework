#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

///#define		USE_MAGELLAN_LOGO

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

#include "FieldCommon.h"
///#include "FieldSettingMenu.h"



DWORD gdwTickCount = 0;
static DWORD dwTickCnt = 0;


void CreateButtons(WORD wDrawOption)
{
    #define BTN_ORIGIN_X    (GetMaxX() / 2)
    #define BTN_ORIGIN_Y    ((GetMaxY() - DEFAULTBTN_HEIGHT) / 2)
	#define BTN_ORIGIN_W    40
	#define BTN_ORIGIN_H    25
	
    GOLFree();                      // free memory for the objects in the previous linked list and start new list
///    CreatePage((XCHAR *)ButtonStr);
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
        (void *) &I16164_Alert,         // icon
///        pText,                      // set text
		(XCHAR *)"Main Page...",         // text
        altScheme
    );                              // default GOL scheme	
	
    BtnCreate
    (
        ID_BUTTON1,                 // button ID
        BTN_ORIGIN_X - 60,
        BTN_ORIGIN_Y - 30,
        BTN_ORIGIN_X - 60 + BTN_ORIGIN_W,
        BTN_ORIGIN_Y - 30 + BTN_ORIGIN_H,			// dimension
        10,                         // set radius
        BTN_DRAW,                   // draw a beveled button
        NULL,                       // no bitmap
        (XCHAR *)"Test",         // text
        altScheme
    );                              // use alternate scheme
    BtnCreate
    (
        ID_BUTTON2,                 // button ID
        BTN_ORIGIN_X + 10,
        BTN_ORIGIN_Y - 30,
        BTN_ORIGIN_X + 10 + BTN_ORIGIN_W,
        BTN_ORIGIN_Y - 30 + BTN_ORIGIN_H,         // dimension
        0,
        BTN_DRAW,                   // will be dislayed after creation
        (void *)&ECG_1bpp_16x16,   // use bitmap
        (XCHAR *)NULL,           // text
        altScheme
    );                              // alternative GOL scheme
    BtnCreate
    (
        ID_BUTTON3,                 // button ID
        BTN_ORIGIN_X - 60,
        BTN_ORIGIN_Y + 20,
        BTN_ORIGIN_X - 60 + BTN_ORIGIN_W,
        BTN_ORIGIN_Y + 20 + BTN_ORIGIN_H,			// dimension
        0,                         // set radius
        BTN_DRAW | BTN_TOGGLE,      // draw a vertical capsule button
        							// that has a toggle behavior
        NULL, 						// no bitmap
        (XCHAR *)"Test",            // text
        yellowScheme
    );                          	// use alternate scheme
    BtnCreate
    (
        ID_BUTTON4,             	// button ID
        BTN_ORIGIN_X + 10,
        BTN_ORIGIN_Y + 20,
        BTN_ORIGIN_X + 10 + BTN_ORIGIN_W,
        BTN_ORIGIN_Y + 20 + BTN_ORIGIN_H,         // dimension
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

#if 0
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
#endif


//*****************************************************************************
//
//! Sends a IO message.
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
	/*
	TODO:
	*/
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

#define TICK_SEMULATE


void IOGetMsg( GOL_MSG *msg )
{
#ifdef TICK_SEMULATE
	if( 0 == dwTickCnt )	///init
		dwTickCnt = gdwTickCount;
#endif		
	/* TODO: move it to queue wait!! */
	while( !ioMsgUpdated )
	{
		/* printf("Wait touch event!!\n"); */
		Sleep(50);
#ifdef TICK_SEMULATE		
		if( scrIsUpdateVaild() && ((gdwTickCount - dwTickCnt) > 100) )
		{
			dwTickCnt = gdwTickCount;
			break;
		}
#endif
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
#ifdef TICK_SEMULATE	
	else	///tickcount hit
	{
		///printf("123");
		///msg->type = TYPE_KEYBOARD;
		///msg->type = TYPE_TOUCHSCREEN;
		msg->type = TYPE_UNKNOWN;
		msg->uiEvent = EVENT_RELEASE;
		msg->param1 = ID_TICKING;
		msg->param2 = SCAN_CRA_RELEASED;	
	}
#endif
}


WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
///	printf("In GOLMsgCallback\n");
	return scrMsgCbHandler(objMsg, pObj, pMsg);
}


WORD GOLDrawCallback(void)
{
///	printf("In GOLDrawCallback\n");
	scrDrawCbHandler();
	return (1);
}


///#define	BOOTMESSAGE

void ObjectTest( void )
{
#ifdef BOOTMESSAGE
    SHORT       width, height;
#endif	
	GOL_MSG msg;                        // GOL message structure to interact with GOL

	scrInitStat();
#if BOOTMESSAGE	
/*
	you place some message on screen even before frames state.
*/
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


FRAME_HEADER fhListBox = {
	MsgListBox,
	CreateListBox,
	MsgListBoxDefaultBtn,
	0,
	0,
	0
};

#if 0
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
#endif








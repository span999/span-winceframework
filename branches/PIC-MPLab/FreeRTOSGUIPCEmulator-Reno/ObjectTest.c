#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "kbkeymap.h"
#include "ObjectTest.h"
#include "FontGentium.h"
#include "1bpp_icons.h"
#include "4bpp_icons.h"



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
///        (XCHAR *)LeftArrowStr,      // LEFT arrow as text
        (XCHAR *)"Left",      // LEFT arrow as text
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
///            (XCHAR *)RightArrowStr, // RIGHT arrow as text
            (XCHAR *)"Right", // RIGHT arrow as text
            navScheme
        );                          // use navigation scheme

	/* add Reno 4(8) buttons simulate (include press/hold action) */
    obj = (OBJ_HEADER *)BtnCreate
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
    obj = (OBJ_HEADER *)BtnCreate
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
    obj = (OBJ_HEADER *)BtnCreate
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
    obj = (OBJ_HEADER *)BtnCreate
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

    obj = (OBJ_HEADER *)BtnCreate
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
    obj = (OBJ_HEADER *)BtnCreate
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
    obj = (OBJ_HEADER *)BtnCreate
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
    obj = (OBJ_HEADER *)BtnCreate
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



void CreateButtons(void)
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
WORD MsgButtons(WORD objMsg, OBJ_HEADER *pObj)
{
    OBJ_HEADER  *pOtherRbtn;
	static BOOL bPressed = FALSE;

    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                ///screenState = CREATE_CHECKBOXES;    // goto check box demo screen
				screenState = CREATE_RENO_DATASET;
            }

            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                ///screenState = CREATE_ECG;           // goto ECG demo screen
				screenState = CREATE_RENO_DATASET;
            }

            return (1);                             // process by default

        case ID_BTN_UP:
        case ID_BTN_UP_HOLD:
        case ID_BTN_DOWN:
        case ID_BTN_DOWN_HOLD:
        case ID_BTN_EXIT:
        case ID_BTN_EXIT_HOLD:
        case ID_BTN_ENTER:
        case ID_BTN_ENTER_HOLD:
            return (0);                             // do nothing since it's HW key simulate
	
			
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
#if 0
        case ID_BUTTON5:
            if(objMsg == BTN_MSG_PRESSED)
            {
                if(!GetState(pObj, BTN_PRESSED))
                {
                    pOtherRbtn = GOLFindObject(ID_BUTTON4);
                    ClrState(pOtherRbtn, BTN_PRESSED);
                    SetState(pOtherRbtn, BTN_DRAW);
                    SetState(pObj, BTN_PRESSED | BTN_DRAW);
#if defined (USE_FOCUS)                                        
                    GOLSetFocus(pObj);  // set focus for the button
#endif                    
                }
            }

            return (0);                 // Do not process by default

        case ID_BUTTON6:
            if(objMsg == BTN_MSG_PRESSED)
            {                           // change face picture
                BtnSetBitmap(pObj, (void *) &bulbon);
                BtnSetText((BUTTON *)pObj, (XCHAR *)OnBulbStr);
                ClrState(pObj, 0x00F0);
                SetState(pObj, BTN_TEXTBOTTOM | BTN_TEXTRIGHT);
            }

            if((objMsg == BTN_MSG_RELEASED) || (objMsg == BTN_MSG_CANCELPRESS))
            {
                BtnSetBitmap(pObj, (void *) &bulboff);
                BtnSetText((BUTTON *)pObj, (XCHAR *)OffBulbStr);
                ClrState(pObj, 0x00F0);
                SetState(pObj, BTN_TEXTTOP | BTN_TEXTLEFT);
            }

            return (1);                 // process by default
#endif
        default:
            return (1);                 // process by default
    }
}


void CreateRenoDataSet()
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



WORD MsgRenoDataSet(WORD objMsg, OBJ_HEADER* pObj)
{
///	OBJ_HEADER* pOtherRbtn;

    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
               ///screenState = CREATE_BUTTONS;// goto check box demo screen
			   screenState = CREATE_BUTTONS;// goto check box demo screen
            }
            return 1; 							// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED){
                ///screenState = CREATE_RENO_HOME; 		// goto ECG demo screen
				screenState = CREATE_BUTTONS;// goto check box demo screen
            }
            return 1; 							// process by default

        default:
            return 1; 							// process by default
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
		printf("  keybd down: ID=%d kbscan=%d\n", lX, lY );
		ioMsg.type = TYPE_KEYBOARD;
		ioMsg.uiEvent = EVENT_KEYSCAN;
		ioMsg.param1 = (SHORT)lX;
		ioMsg.param2 = (SHORT)lY;
		
		/*re-mapping key, simulate the key pressed/repeased*/
		if( KB_KEY_1 == ioMsg.param2 )
		{	/*simulate the "BACK" key repeased*/
			ioMsg.param1 = ID_BUTTON_BACK;
			ioMsg.param2 = SCAN_CRA_RELEASED;
		}
		else
		if( KB_KEY_2 == ioMsg.param2 )
		{	/*simulate the "NEXT" key repeased*/
			ioMsg.param1 = ID_BUTTON_NEXT;
			ioMsg.param2 = SCAN_CRA_RELEASED;
		}
		else
		if( KB_KEY_I == ioMsg.param2 )
		{	/*simulate the "UP" key repeased*/
			ioMsg.param1 = ID_BTN_UP;
			ioMsg.param2 = SCAN_CRA_RELEASED;
		}
		else
		if( KB_KEY_O == ioMsg.param2 )
		{	/*simulate the "UP" key hold*/
			ioMsg.param1 = ID_BTN_UP_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
		}
		else
		if( KB_KEY_K == ioMsg.param2 )
		{	/*simulate the "DOWN" key repeased*/
			ioMsg.param1 = ID_BTN_DOWN;
			ioMsg.param2 = SCAN_CRA_RELEASED;
		}
		else
		if( KB_KEY_L == ioMsg.param2 )
		{	/*simulate the "DOWN" key hold*/
			ioMsg.param1 = ID_BTN_DOWN_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
		}
		else
		if( KB_KEY_U == ioMsg.param2 )
		{	/*simulate the "EXIT" key repeased*/
			ioMsg.param1 = ID_BTN_EXIT;
			ioMsg.param2 = SCAN_CRA_RELEASED;
		}
		else
		if( KB_KEY_Y == ioMsg.param2 )
		{	/*simulate the "EXIT" key hold*/
			ioMsg.param1 = ID_BTN_EXIT_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
		}
		else
		if( KB_KEY_J == ioMsg.param2 )
		{	/*simulate the "ENTER" key repeased*/
			ioMsg.param1 = ID_BTN_ENTER;
			ioMsg.param2 = SCAN_CRA_RELEASED;
		}
		else
		if( KB_KEY_H == ioMsg.param2 )
		{	/*simulate the "ENTER" key hold*/
			ioMsg.param1 = ID_BTN_ENTER_HOLD;
			ioMsg.param2 = SCAN_CRA_RELEASED;
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
	printf("In GOLMsgCallback\n");

#if 0
    // beep if button is pressed
    if(objMsg == BTN_MSG_PRESSED)
    {
///        Beep();
    }
    else
    {
        if(GetObjType(pObj) == OBJ_RADIOBUTTON)
        {
///            Beep();
            
            if(pObj->ID == ID_RADIOBUTTON5)
            {
///                _language = LANG_ENGLISH;
                screenState = prevRefreshState ; //Goto CREATE_XXX state
                return(1);
            }
            else if(pObj->ID == ID_RADIOBUTTON6)
            {
///                _language = LANG_CHINESE;
                screenState = prevRefreshState ; //Goto CREATE_XXX state
                return(1);
            }
        }
        else
        {
            if(GetObjType(pObj) == OBJ_CHECKBOX)
                ;///Beep();
        }
    }
#endif

#if 0
    if((screenState & 0xF300) != 0xF300)
    {

        // check for time setting press, process only when not setting time and date
        if(objMsg == ST_MSG_SELECTED)
        {

            /* note how the states are arranged in the enumeration, the display state is 
		       right after the create state. So at the time the static text box of the 
		       time is selected, the state is in one of the displays. So a minus one is needed
		       to adjust to go back to the create state of the current page.
		       After the date and time adjust screen is exited, the saved create state will
		       be entered as the next state. So we get the effect of going back to the previous
		       screen after date and time settings are done.
		    */
            if((GetObjID(pObj) == ID_STATICTEXT1) || (GetObjID(pObj) == ID_STATICTEXT2))
            {
                prevState = screenState - 1;        // save the current create state
                screenState = CREATE_DATETIME;      // go to date and time setting screen
                return (1);
            }
        }
    }
#endif

#if 0
    // check if pull down menu is called
    if(GetObjID(pObj) == ID_WINDOW1)
    {
        if((objMsg == WND_MSG_TITLE) && (screenState != DISPLAY_PULLDOWN))
        {

            // check area of press
            if((pMsg->param1 <= 220) && (pMsg->param2 <= 40))
            {
                switch(screenState)
                {

                    // if one of these states we redraw the whole screen since
                    // these screens have customized graphics.
                    case DISPLAY_SLIDER:
                        prevState = CREATE_SLIDER;
                        break;

                    case DISPLAY_CUSTOMCONTROL:
                        prevState = CREATE_CUSTOMCONTROL;
                        break;

                    case DISPLAY_SIGNATURE:
                        prevState = CREATE_SIGNATURE;
                        break;

                    case DISPLAY_POT:
                        prevState = CREATE_POT;
                        break;

                    case DISPLAY_ECG:
                        prevState = CREATE_ECG;
                        break;

                    case DISPLAY_PROGRESSBAR:
                        prevState = CREATE_PROGRESSBAR;
                        break;

                    // pull down is disabled when setting date and time
                    case CREATE_DATETIME:
                    case DISPLAY_DATETIME:
                    case DISPLAY_DATE_PDMENU:
                    case SHOW_DATE_PDMENU:
                    case HIDE_DATE_PDMENU:
                        return (0);

                    default:
                        prevState = screenState;    // save the current create state
                        break;
                }

                screenState = CREATE_PULLDOWN;      // go to show pulldown menu state
                
                if(pMsg->param1 <= 136)
                {
///                    PulldownId = 0;
                }
                else
                {
                    //PulldownId = 1;
                }
                
                return (1);
            }
        }
    }
#endif

#if defined(WIN32)
#if 0
	printf("In GOLMsgCallback 00\n");
    if( (screenState == DISPLAY_BUTTONS) )
    {
		printf("In GOLMsgCallback 11\n");
        prevState = screenState - 1;        // save the current create state
        screenState = CREATE_RENO_DATASET;      // go to date and time setting screen
		return (1);
	}
	else
    if( (screenState == DISPLAY_RENO_DATASET) )
    {
		printf("In GOLMsgCallback 22\n");
        prevState = screenState - 1;        // save the current create state
        screenState = CREATE_BUTTONS;      // go to date and time setting screen
		return (1);
	}
	else
		;
#endif
#endif

    // process messages for demo screens
    switch(screenState)
    {
        case DISPLAY_BUTTONS:
            return (MsgButtons(objMsg, pObj));

        case DISPLAY_RENO_DATASET:
            return (MsgRenoDataSet(objMsg, pObj));
#if 0
        case DISPLAY_CHECKBOXES:
            return (MsgCheckBoxes(objMsg, pObj));

        case DISPLAY_RADIOBUTTONS:
            return (MsgRadioButtons(objMsg, pObj));

        case DISPLAY_STATICTEXT:
            return (MsgStaticText(objMsg, pObj));

        case DISPLAY_PICTURE:
            return (MsgPicture(objMsg, pObj));

        case DISPLAY_SLIDER:
            return (MsgSlider(objMsg, pObj, pMsg));

        case DISPLAY_PROGRESSBAR:
            return (MsgProgressBar(objMsg, pObj));

        // date and time settings display
        case DISPLAY_DATETIME:
            return (MsgDateTime(objMsg, pObj));

        case DISPLAY_DATE_PDMENU:
            return (MsgSetDate(objMsg, pObj, pMsg));

        case CREATE_DATETIME:
        case SHOW_DATE_PDMENU:
        case HIDE_DATE_PDMENU:
            return (0);

        case DISPLAY_METER:
            return (MsgMeter(objMsg, pObj));

        case DISPLAY_DIAL:
            return (MsgDial(objMsg, pObj));

        case DISPLAY_CUSTOMCONTROL:
            return (MsgCustomControl(objMsg, pObj, pMsg));

        case DISPLAY_LISTBOX:
            return (MsgListBox(objMsg, pObj, pMsg));

        case DISPLAY_EDITBOX:
            return (MsgEditBox(objMsg, pObj, pMsg));

        case DISPLAY_SIGNATURE:
            return (MsgSignature(objMsg, pObj, pMsg));

        case DISPLAY_POT:
            return (MsgPotentiometer(objMsg, pObj));

        case DISPLAY_ECG:
            return (MsgECG(objMsg, pObj));

        case DISPLAY_PULLDOWN:
            return (MsgPullDown(objMsg, pObj, pMsg));
#endif
        default:

            // process message by default
            return (1);
    }

}


WORD GOLDrawCallback(void)
{
	printf("In GOLDrawCallback\n");
	
    switch(screenState)
    {
        case CREATE_BUTTONS:
            prevRefreshState = CREATE_BUTTONS;
            CreateButtons();        // create window and buttons
            screenState = DISPLAY_BUTTONS;                              // switch to next state
            return (1);                                                 // draw objects created

        case DISPLAY_BUTTONS:
///			screenState = CREATE_BUTTONS;                              // switch to next state
            return (1);                                                 // redraw objects if needed
			
        case CREATE_RENO_DATASET:
			prevRefreshState = CREATE_BUTTONS;
            CreateRenoDataSet(); 							// create window and buttons
            screenState = DISPLAY_RENO_DATASET; 			// switch to next state
            return (1); 									// draw objects created
            
        case DISPLAY_RENO_DATASET:
            return (1); 									// redraw objects if needed

		default:
			return (1);    
	}

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


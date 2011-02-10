#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

/// include this for PIC graphic lib
#include "PICgraphic_set.h"

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





void CreateButtons(void)
{
    #define BTN_ORIGIN_X    (GetMaxX() / 2)
    #define BTN_ORIGIN_Y    (GetMaxY() / 2)
	#define BTN_ORIGIN_W    50
	#define BTN_ORIGIN_H    30
	
    GOLFree();                      // free memory for the objects in the previous linked list and start new list
///    CreatePage((XCHAR *)ButtonStr);
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

    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_CHECKBOXES;    // goto check box demo screen
            }

            return (1);                             // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_ECG;           // goto ECG demo screen
            }

            return (1);                             // process by default

        case ID_BUTTON3:
            if(objMsg == BTN_MSG_PRESSED)
            {                           // change text and scheme
                ///BtnSetText((BUTTON *)pObj, (XCHAR *)HighStr);
				BtnSetText((BUTTON *)pObj, (XCHAR *)"Go");
            }
            else
            {
                ///BtnSetText((BUTTON *)pObj, (XCHAR *)LowStr);
				BtnSetText((BUTTON *)pObj, (XCHAR *)"Stop");
            }

            return (1);                 // process by default

        case ID_BUTTON4:
            if(objMsg == BTN_MSG_PRESSED)
            {
                if(!GetState(pObj, BTN_PRESSED))
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







WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
	printf("In GOLMsgCallback\n");
	return (1);
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
            return (1);                                                 // redraw objects if needed
			
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
	navScheme->pFont = (void *)Gentium_Normal15;

    alt2Scheme->TextColor1 = BRIGHTRED;
    alt2Scheme->TextColor0 = BRIGHTBLUE;
///    alt2Scheme->pFont = (void *)ptrSmallAsianFont;

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
    meterScheme->TextColor0 = BRIGHTBLUE;
    meterScheme->TextColor1 = WHITE;
    meterScheme->EmbossDkColor = GRAY20;
    meterScheme->EmbossLtColor = GRAY80;
///    meterScheme->pFont = (void *)ptrSmallAsianFont;

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
		printf("In message loop\n");
        if(GOLDraw())
        {                               // Draw GOL objects
            // Drawing is done here, process messages
///            TouchGetMsg(&msg);          // Get message from touch screen
            GOLMsg(&msg);               // Process message
#if defined (USE_FOCUS)                    
            #if !(defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__))
///            SideButtonsMsg(&msg);       // Get message from side buttons
            GOLMsg(&msg);               // Process message
            #endif
#endif //#if defined (USE_FOCUS)
        }
    }
	
}


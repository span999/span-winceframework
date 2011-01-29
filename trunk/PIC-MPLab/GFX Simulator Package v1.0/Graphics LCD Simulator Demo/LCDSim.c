/***************************************************************************
Code in this file is nearly all from AN1136Demo.c
Original code by Paolo A. Tamayo Microchip Technology Incorporated

Changes made...
- Stripped conditional compililation preprocessing statements for testing
under PIC32 only
- Stripped a lot of hardware initialization code, as we're using the LCD
  Simulator to test gfx output not hardware
- Changed call from GetTouchMsg to GetEmulatorTouchMsg
AlfBaz
****************************************************************************/
#include <p32xxxx.h>
#include <plib.h>
#include "GenericTypeDefs.h"
#include "Graphics\Graphics.h"


#pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
#pragma config OSCIOFNC = ON, POSCMOD = XT, FSOSCEN = ON, FNOSC = PRIPLL
#pragma config CP = OFF, BWP = OFF, PWP = OFF

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
#define ID_BTN1 		10
#define ID_BTN2 		11
#define ID_SLD1 		20

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT DIMENSIONS
/////////////////////////////////////////////////////////////////////////////
#define SLDR_XSTART		20
#define SLDR_YSTART		105
#define SLDR_WIDTH		(GetMaxX()-40)
#define SLDR_HEIGHT		45

#define BTN_WIDTH		130
#define BTN_YSTART		160
#define BTN1_XSTART		((GetMaxX()>>1)-10-BTN_WIDTH)
#define BTN2_XSTART		((GetMaxX()>>1)+10)
#define BTN_HEIGHT		50

#define BAR_XSTART		((GetMaxX()-200)>>1)
#define BAR_WIDTH		4

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void            TickInit(void);                 // starts tick counter

/////////////////////////////////////////////////////////////////////////////
//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
extern const BITMAP_FLASH redRightArrow;
extern const BITMAP_FLASH redLeftArrow;

/////////////////////////////////////////////////////////////////////////////
//                                  MAIN
/////////////////////////////////////////////////////////////////////////////
GOL_SCHEME                  *altScheme; // alternative style scheme
SLIDER                      *pSld;      // pointer to the slider object
WORD                        update = 0; // variable to update customized graphics

/* */
int main(void)
{
	GOL_MSG msg;                    // GOL message structure to interact with GOL
	
	INTEnableSystemMultiVectoredInt();
    SYSTEMConfigPerformance(GetSystemClock());
        
    GOLInit();                      // initialize graphics library &
    
    // create default style scheme for GOL
    TickInit();                     // initialize tick counter (for random number generation)
    EmulatorTouchInit();
    
	altScheme = GOLCreateScheme();  // create alternative style scheme
    altScheme->TextColor0 = BLACK;
    altScheme->TextColor1 = BRIGHTBLUE;

    BtnCreate
    (
        ID_BTN1,                    // object’s ID
        BTN1_XSTART,
        BTN_YSTART,
        BTN1_XSTART+BTN_WIDTH,
        BTN_YSTART+BTN_HEIGHT,      // object’s dimension
        0,                          // radius of the rounded edge
        BTN_DRAW,                   // draw the object after creation
        NULL,                       // no bitmap used
        "LEFT",                     // use this text
        altScheme
    );                              // use alternative style scheme

    BtnCreate
    (
    	ID_BTN2, 
    	BTN2_XSTART, 
    	BTN_YSTART, 
    	BTN2_XSTART+BTN_WIDTH, 
    	BTN_YSTART+BTN_HEIGHT, 
    	0, 
    	BTN_DRAW, 
    	NULL, 
    	"RIGHT", 
    	altScheme
    );

    pSld = SldCreate
        (
            ID_SLD1,                // object’s ID
            SLDR_XSTART,
            SLDR_YSTART,
            SLDR_XSTART+SLDR_WIDTH,
            SLDR_YSTART+SLDR_HEIGHT,// object’s dimension
            SLD_DRAW,               // draw the object after creation
            100,                    // range
            5,                      // page
            50,                     // initial position
            NULL
        );                          // use default style scheme

    update = 1;                     // to initialize the user graphics
    while(1)
    {
        if(GOLDraw())
        {                           // Draw GOL object
            GetEmulatorTouchMsg(&msg);      // Get message from touch screen
            GOLMsg(&msg);           // Process message
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed by default
// Overview: it's a user defined function. GOLMsg() function calls it each

//           time the valid message for the object received
/////////////////////////////////////////////////////////////////////////////
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD    objectID;
    SLIDER  *pSldObj;

    objectID = GetObjID(pObj);

	#if defined (GFX_PICTAIL_V1) || defined (GFX_PICTAIL_V2)
	    #if !defined (__PIC32MX__)
	    if(objMsg == BTN_MSG_PRESSED)
	    {
	        Beep();
	    }
		#endif
	#endif
	
    if(objectID == ID_BTN1)
    {
        if(objMsg == BTN_MSG_PRESSED)
        {   // check if button is pressed
            BtnSetBitmap(pObj, (void *) &redLeftArrow);     // set bitmap to show
            SetState(pObj, BTN_TEXTRIGHT);                  // move the text to the right
            pSldObj = (SLIDER *)GOLFindObject(ID_SLD1);     // find slider pointer
            SldDecPos(pSldObj);                             // decrement the slider position
            SetState(pSldObj, SLD_DRAW_THUMB);              // redraw only the thumb
        }
        else
        {
            BtnSetBitmap(pObj, NULL);                       // remove the bitmap
            ClrState(pObj, BTN_TEXTRIGHT);                  // place the text back in the middle
        }

        update = 1;
    }

    if(objectID == ID_BTN2)
    {
        if(objMsg == BTN_MSG_PRESSED)
        {
            BtnSetBitmap(pObj, (void *) &redRightArrow);    // set bitmap to show
            SetState(pObj, BTN_TEXTLEFT);                   // move the text to the left
            pSldObj = (SLIDER *)GOLFindObject(ID_SLD1);     // find slider pointer
            SldIncPos(pSldObj);                             // increment the slider position
            SetState(pSldObj, SLD_DRAW_THUMB);              // redraw only the thumb
        }
        else
        {
            BtnSetBitmap(pObj, NULL);                       // remove the bitmap
            ClrState(pObj, BTN_TEXTLEFT);                   // place the text back in the middle
        }

        update = 1;
    }

	if(objectID == ID_SLD1)
	{
		if((objMsg == SLD_MSG_INC) || (objMsg == SLD_MSG_DEC))
			update = 1;

	}
	return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLDrawCallback()
// Output: if the function returns non-zero the draw control will be passed to GOL
// Overview: it's a user defined function. GOLDraw() function calls it each
//           time when GOL objects drawing is completed. User drawing should be done here.
//           GOL will not change color, line type and clipping region settings while

//           this function returns zero.
/////////////////////////////////////////////////////////////////////////////
WORD GOLDrawCallback(void)
{
    WORD        value, y, x;    // variables for the slider position
    static WORD prevValue = 0;

    if(update)
    {

        /* User defined graphics:	
		    This draws a series of bars indicating the value/position of the 
			slider's thumb. The height of the bars follows the equation of a 
			parabola "(y-k)^2 = 4a(x-h) with vertex at (k, h) at (60,100) on 
			the display. The value 110 is the 4*a constant. x & y are calculated
			based on the value of the slider thumb. The bars are drawn from 
			60 to 260 in the x-axis and 10 to 100 in the y-axis. Bars are drawn
			every 6 pixels with width of 4 pixels.
			
			Only the bars that are added or removed are drawn. Thus resulting 
			in an efficient customized drawing. 
		*/

        // check the value of slider
        value = SldGetPos(pSld);

        // remove bars if there the new value is less
        // than the previous
        SetColor(BLACK);
        if(value < prevValue)
        {
            while(prevValue > value)
            {

                // get the height of the bar to be removed
                y = (prevValue * prevValue) / 110;

                // bars are drawn every 6 pixels with width = 4 pixels.
                x = (prevValue * 2);
                x = x - (x % 6);

                // draw a BLACK colored bar to remove the current bar drawn
                while(!Bar(x+BAR_XSTART, 100-y, x+BAR_XSTART+BAR_WIDTH, 100));

                // decrement by three since we are drawing every 6 pixels
                prevValue -= 3;
            }
        }

        // Draw bars if there the new value is greater
        // than the previous
        else
        {
            while(prevValue < value)
            {
                if(prevValue < 60)
                {
                    SetColor(BRIGHTGREEN);
                }
                else if((prevValue < 80) && (prevValue >= 60))
                {
                    SetColor(BRIGHTYELLOW);
                }
                else if(prevValue >= 80)
                {
                    SetColor(BRIGHTRED);
                }

                // get the height of the bar to be drawn
                y = (prevValue * prevValue) / 110;

                // bars are drawn every 6 pixels with width = 4 pixels.
                x = (prevValue * 2);
                x = x - (x % 6);

                // draw a bar to increase in value
                while(!Bar(x+BAR_XSTART, 100-y, x+BAR_XSTART+BAR_WIDTH, 100));

                // increment by three since we are drawing every 6 pixels
                prevValue += 3;
            }
        }

        // prevValue will have the current value after drawing or removing bars.
        // reset the update flag
        update = 0;
    }

    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: void TickInit(void)
// Input: none
// Output: none
// Overview: Initilizes the tick timer.
/////////////////////////////////////////////////////////////////////////////

/*********************************************************************
 * Section: Tick Delay
 *********************************************************************/
#define SAMPLE_PERIOD       500 // us
#define TICK_PERIOD			(GetPeripheralClock() * SAMPLE_PERIOD) / 4000000

/* */
void TickInit(void)
{

    // Initialize Timer4
    OpenTimer3(T3_ON | T3_PS_1_8, TICK_PERIOD);
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_4);
}

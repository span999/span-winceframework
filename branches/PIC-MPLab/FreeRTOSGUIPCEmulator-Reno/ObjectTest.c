#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "ObjectTest.h"
#include "FontGentium.h"
///#include "1bpp_icons.h"
///#include "4bpp_icons.h"



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


/////////////////////////////////////////////////////////////////////////////
//                            COLORS USED
/////////////////////////////////////////////////////////////////////////////
#define GRAY20      RGB565CONVERT(51, 51, 51)
#define GRAY40      RGB565CONVERT(102, 102, 102)
#define GRAY80      RGB565CONVERT(204, 204, 204)
#define GRAY90      RGB565CONVERT(229, 229, 229)
#define GRAY95      RGB565CONVERT(242, 242, 242)
#define RED4        RGB565CONVERT(139, 0, 0)
#define FIREBRICK1  RGB565CONVERT(255, 48, 48)
#define DARKGREEN   RGB565CONVERT(0, 100, 0)
#define PALEGREEN   RGB565CONVERT(152, 251, 152)
#define LIGHTYELLOW RGB565CONVERT(238, 221, 130)
#define GOLD        RGB565CONVERT(255, 215, 0)
#define DARKORANGE  RGB565CONVERT(255, 140, 0)



WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
	printf("In GOLMsgCallback\n");
	return (1);
}


WORD GOLDrawCallback(void)
{
	printf("In GOLDrawCallback\n");
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

    greenScheme->Color0 = RGB565CONVERT(0x23, 0x9E, 0x0A);
    greenScheme->Color1 = BRIGHTGREEN;
    greenScheme->EmbossDkColor = DARKGREEN;
    greenScheme->EmbossLtColor = PALEGREEN;
    greenScheme->TextColor0 = RGB565CONVERT(0xDF, 0xAC, 0x83);
    greenScheme->TextColor1 = BLACK;
///    greenScheme->pFont = (void *)ptrLargeAsianFont;

    yellowScheme->Color0 = BRIGHTYELLOW;
    yellowScheme->Color1 = YELLOW;
    yellowScheme->EmbossDkColor = RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowScheme->EmbossLtColor = RGB565CONVERT(0xFD, 0xFF, 0xB2);
    yellowScheme->TextColor0 = RGB565CONVERT(0xAF, 0x34, 0xF3);
    yellowScheme->TextColor1 = RED;
///    yellowScheme->pFont = (void *)ptrLargeAsianFont;

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
	DelayMs(DEMODELAY);
	DelayMs(DEMODELAY);	
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


/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30 V3.00
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright  2007 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok		30/05/07	...
 * Paolo Tamayo         09/20/07    Round buttons demo screen is added
 * Paolo Tamayo			10/05/07    Modified navigation, added Meter, Dial, 
 *									RTCC demos
 *****************************************************************************/

#include "MainDemo.h"

#include <signal.h>
#include <sys/time.h>

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
#define ID_WINDOW1      10

#define ID_BUTTON1      11
#define ID_BUTTON2      12
#define ID_BUTTON3      13
#define ID_BUTTON4      14
#define ID_BUTTON5      15
#define ID_BUTTON6      16
#define ID_BUTTON7      17

#define ID_BUTTON_BACK  18
#define ID_BUTTON_NEXT  19

#define ID_CHECKBOX1    20
#define ID_CHECKBOX2    21
#define ID_CHECKBOX3    22
#define ID_CHECKBOX4    23
#define ID_CHECKBOX5    24

#define ID_RADIOBUTTON1 30
#define ID_RADIOBUTTON2 31
#define ID_RADIOBUTTON3 32
#define ID_RADIOBUTTON4 33
#define ID_RADIOBUTTON5 34
#define ID_RADIOBUTTON6 35
#define ID_RADIOBUTTON7 36
#define ID_RADIOBUTTON8 37

#define ID_SLIDER1      40
#define ID_SLIDER2      41
#define ID_SLIDER3      42

#define ID_GROUPBOX1    50
#define ID_GROUPBOX2    51
#define ID_STATICTEXT1  52
#define ID_STATICTEXT2  53
#define ID_STATICTEXT3  54

#define ID_PICTURE1     55

#define ID_PROGRESSBAR1 60
#define ID_CUSTOM1      70

#define ID_LISTBOX1     80
#define ID_LISTBOX2     81
#define ID_LISTBOX3     82
#define ID_EDITBOX1     83

#define ID_CALL         91
#define ID_STOPCALL		92
#define ID_HOLD			93
#define ID_BACKSPACE    94
#define ID_POUND		95
#define ID_ASTERISK		96
#define ID_KEYPAD       100	/* uses 100 to 110 for phone key pad demo */


#define ID_ROUNDDIAL    120
#define ID_METER1	    130
#define ID_METER2	    131

#define ID_BUTTONHR_P   150
#define ID_BUTTONHR_M   151
#define ID_BUTTONMN_P   152
#define ID_BUTTONMN_M   153
#define ID_BUTTONSC_P   154
#define ID_BUTTONSC_M   155

#define ID_BUTTON_DATE_UP 180
#define ID_BUTTON_DATE_DN 181

#define ID_BUTTON_MO    186
#define ID_BUTTON_DY    187
#define ID_BUTTON_YR    188
#define ID_BUTTON_RET   189

#define ID_EB_MONTH		200
#define ID_EB_DAY		201
#define ID_EB_YEAR		202

#define ID_EB_HOUR      205
#define ID_EB_MINUTE    206
#define ID_EB_SECOND    207


/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void StartScreen();								// draws intro screen
void CreatePage(char *pText); 					// creates the navigation buttons and window for each screen

///span110110, add status bar component
void CreateStatusBar(char *pText); 				// creates the navigation buttons and window for each screen
///span110110,

void CreateButtons();                           // creates buttons demo screen
WORD MsgButtons(WORD objMsg, OBJ_HEADER* pObj); // processes messages for buttons demo screen

///span110110, add screen for Reno demo
void CreateRenoHome();                           // creates Reno demo screen
WORD MsgRenoHome(WORD objMsg, OBJ_HEADER* pObj); // processes messages for Reno demo screen
///span110110, 

void CreateRoundButtons();                      // creates rounded buttons demo screen
WORD MsgRoundButtons(WORD objMsg, OBJ_HEADER* pObj); // processes messages for rounded buttons demo screen

void CreateCheckBoxes();                        // creates checkboxes demo screen
WORD MsgCheckBoxes(WORD objMsg, OBJ_HEADER* pObj); // processes messages for checkboxes demo screen

void CreateRadioButtons();                      // creates radio buttons demo screen
WORD MsgRadioButtons(WORD objMsg, OBJ_HEADER* pObj); // processes messages for radio buttons demo screen

void CreateStaticText();                        // creates static text demo screen
WORD MsgStaticText(WORD objMsg, OBJ_HEADER* pObj); // processes messages for static text demo screen

void CreatePicture();                        	// creates picture demo screen
WORD MsgPicture(WORD objMsg, OBJ_HEADER* pObj); // processes messages for picture demo screen

void CreateSlider();                        	// creates slider demo screen
void DrawSliderCursor(WORD color);          	// draws cursor for slider demo screen
WORD MsgSlider(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg); // processes messages for slider demo screen

void CreateProgressBar();                   	// creates progress bar demo screen
WORD MsgProgressBar(WORD objMsg, OBJ_HEADER* pObj); //  processes messages for progress bar demo screen

void CreateSignature();                        	// creates signature demo screen
WORD PanelSignature();                        	// draws box for the signature
WORD MsgSignature(WORD objMsg, OBJ_HEADER* pObj,  GOL_MSG* pMsg); // processes messages for signature demo screen

void CreatePotentiometer();                 	// scrolling graph demo screen
WORD PanelPotentiometer();                  	// draws box for the scrolling graph
void GraphPotentiometer();                  	// draws scrolling graph graph
WORD GetPotSamples(WORD number);            	// adds sample from ADC potentiometer channel into temporary buffer
                                            	// returns zero if samples number in the buffer is less than defined by parameter
WORD MsgPotentiometer(WORD objMsg, OBJ_HEADER* pObj); // processes messages for ECG demo screen

void CreateECG();                           	// creates ECG demo screen
WORD PanelECG();                            	// draws box for the ECG
void GraphECG();                            	// draws ECG graph
WORD GetECGSamples(WORD number);            	// adds sample from FLASH (can be any source) into temporary buffer
                                            	// returns zero if samples number in the buffer is less than defined by parameter
WORD MsgECG(WORD objMsg, OBJ_HEADER* pObj); 	// processes messages for ECG demo screen

void CreateCustomControl();                     // creates custom control demo screen
WORD MsgCustomControl(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg); //  processes messages for custom control demo screen

void CreateListBox();                        	// creates list box demo screen
WORD MsgListBox(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg); //  processes messages for list box demo screen

void CreateEditBox();                        	// creates edit box demo screen
WORD MsgEditBox(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg); //  processes messages for edit box demo screen

void CreateDateTime();							// creates date time demo screen
WORD MsgDateTime(WORD objMsg, OBJ_HEADER* pObj);//  processes messages for date and time demo screen
void UpdateDateEb(char *pText, LISTBOX* pLb); 	// update the date edit boxes 
WORD MsgSetDate(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg); // processes messages for setting dates
void ShowPullDownMenu();						// create objects for pull down menu and draw 
WORD RemovePullDownMenu();						// removes pop out pull down menu from screen
void updateDateTimeEb();						// update edit boxes with current date and time values 


void CreateMeter();								// creates meter demo screen
void UpdateMeter();
WORD MsgMeter(WORD objMsg, OBJ_HEADER* pObj); 	// processes messages for meter demo screen

void CreateDial();								// creates dial demo screen
WORD MsgDial(WORD objMsg, OBJ_HEADER* pObj); 	// processes messages for dial demo screen

void ErrorTrap(char* message);              	// outputs text message on screen and stop execution

void TickInit(void);                        	// starts tick counter  

void CreatePullDown(); 
WORD MsgPullDown(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg); // processes screen pull down menu


/////////////////////////////////////////////////////////////////////////////
//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
extern BITMAP_FLASH intro;
extern BITMAP_FLASH mchpLogo;

extern BITMAP_FLASH mchpIcon;
extern BITMAP_FLASH mchpIcon0;

extern BITMAP_FLASH duckPressed;
extern BITMAP_FLASH duckReleased;

extern BITMAP_FLASH arrowUp;
extern BITMAP_FLASH arrowDown;

extern BITMAP_FLASH Engine1;
extern BITMAP_FLASH Engine2;
extern BITMAP_FLASH Engine3;
extern BITMAP_FLASH Engine4;

extern BITMAP_FLASH phone;
extern BITMAP_FLASH redphone;
extern BITMAP_FLASH greenphone;
extern BITMAP_FLASH gradientButton;

/////////////////////////////////////////////////////////////////////////////
//                             FONTS USED
/////////////////////////////////////////////////////////////////////////////
extern FONT_FLASH GOLFontDefault; 		// default GOL font
extern FONT_FLASH GOLMediumFont; 		// medium font
extern FONT_FLASH GOLSmallFont; 		// small font

/////////////////////////////////////////////////////////////////////////////
//                            COLORS USED
/////////////////////////////////////////////////////////////////////////////
#define GRAY20             RGB565CONVERT( 51,  51,  51)
#define GRAY40             RGB565CONVERT(102, 102, 102)
#define GRAY80             RGB565CONVERT(204, 204, 204)
#define GRAY90             RGB565CONVERT(229, 229, 229)
#define GRAY95             RGB565CONVERT(242, 242, 242)

#define RED4 			   RGB565CONVERT(139,   0,   0)
#define FIREBRICK1 		   RGB565CONVERT(255,  48,  48)
#define DARKGREEN          RGB565CONVERT(  0, 100,   0)
#define PALEGREEN          RGB565CONVERT(152, 251, 152)
#define LIGHTYELLOW        RGB565CONVERT(238, 221, 130)
#define GOLD               RGB565CONVERT(255, 215,   0)
#define DARKORANGE		   RGB565CONVERT(255, 140,   0)

/////////////////////////////////////////////////////////////////////////////
//                            DEMO STATES      
/////////////////////////////////////////////////////////////////////////////
typedef enum {
    CREATE_BUTTONS = 0,
    DISPLAY_BUTTONS,
    CREATE_CHECKBOXES,
    DISPLAY_CHECKBOXES,
    CREATE_RADIOBUTTONS,
    DISPLAY_RADIOBUTTONS,
    CREATE_STATICTEXT,
    DISPLAY_STATICTEXT,
    CREATE_PICTURE,
    DISPLAY_PICTURE,
    CREATE_SLIDER,
    DISPLAY_SLIDER,
    CURSOR_DRAW_SLIDER,
    CREATE_PROGRESSBAR,
    DISPLAY_PROGRESSBAR,
    CREATE_LISTBOX,
    DISPLAY_LISTBOX,
    CREATE_EDITBOX,
    DISPLAY_EDITBOX,
    CREATE_METER,
    DISPLAY_METER,
    CREATE_DIAL,
    DISPLAY_DIAL,
    CREATE_CUSTOMCONTROL,
    DISPLAY_CUSTOMCONTROL,
    CREATE_SIGNATURE,
    DISPLAY_SIGNATURE,
    BOX_DRAW_SIGNATURE,
    CREATE_POT,
    DISPLAY_POT,
    BOX_DRAW_POT,
    CREATE_ECG,
    DISPLAY_ECG,
    BOX_DRAW_ECG,
    CREATE_PULLDOWN,
    DISPLAY_PULLDOWN,
///span110110, add screen for Reno demo
	CREATE_RENO_HOME,
	DISPLAY_RENO_HOME,
///span110110,    
    CREATE_DATETIME = 0xF300,		// these states are for time and date settings
    DISPLAY_DATETIME = 0xF301,		// 0xF3xx is used here as a state ID to check when 
    DISPLAY_SETDATE = 0xF302,		// date and time are to be updated or not.
    HIDE_DATE_PDMENU = 0xF303,

} SCREEN_STATES;

/////////////////////////////////////////////////////////////////////////////
//                       GLOBAL VARIABLES FOR DEMO
/////////////////////////////////////////////////////////////////////////////

///span110110, set Reno as first demo page.
#if 0
SCREEN_STATES  screenState = CREATE_BUTTONS; // current state of main demo state mashine 
#else
SCREEN_STATES  screenState = CREATE_RENO_HOME;
#endif

GOL_SCHEME*    altScheme;					 // alternative style scheme
GOL_SCHEME*    alt2Scheme;					 // alternative 2 style scheme
GOL_SCHEME*    alt3Scheme;					 // alternative 3 style scheme
GOL_SCHEME*    meterScheme;					 // meter scheme
GOL_SCHEME*    navScheme;		 			 // style scheme for the navigation
GOL_SCHEME*    redScheme;                    // alternative red style scheme
GOL_SCHEME*    greenScheme;                  // alternative green style scheme
GOL_SCHEME*    yellowScheme;                 // alternative yellow style scheme
///span110109, add my test code for Scheme 
GOL_SCHEME*    myScheme;                 	 // alternative style scheme of my
///span110109,

OBJ_HEADER*	   pNavList;					 // pointer to navigation list

volatile DWORD tick = 0;                     // tick counter

char           animate;                      // switch to control animation for picture demo
OBJ_HEADER*	   pGenObj;						 // pointer to a general purpose object
PICTURE*       pPicture;                     // pointer to picture object for picture demo  

PROGRESSBAR*   pProgressBar;                 // pointer to progress bar object for progress bar demo

SLIDER*		   pSlider;						 // pointer to the slider controlling the animation speed 

// GLOBAL DEFINITIONS AND VARIABLES FOR SLIDER DEMO
#define CUR_BAR_SIZE    3                    // half size of center point for cursor
#define CUR_BRD_LEFT    67+CUR_BAR_SIZE      // cursor area left border
#define CUR_BRD_RIGHT   253-CUR_BAR_SIZE     // cursor area right border
#define CUR_BRD_TOP     52+CUR_BAR_SIZE      // cursor area top border
#define CUR_BRD_BOTTOM  184-CUR_BAR_SIZE     // cursor area bottom border

SHORT x;                                     // cursor X position
SHORT y;                                     // cursor Y position

// GLOBAL DEFINITIONS AND VARIABLES FOR ECG DEMO
// Array of ECG points to emulate ECG input
const SHORT ecgPoints[] =
{ 0x0068,0x0068,0x0068,0x0068,0x0068,0x0069,0x0068,0x0068,0x0068,0x0067,0x0067,0x0066,0x0066,0x0068,0x0068,0x0068,0x0067,0x0066,0x0066,0x0066,0x0066,0x0065,0x0065,0x0065,0x0065,0x0065,0x0065,0x0064,0x0063,0x0063,0x0063,0x0064
, 0x0064,0x0065,0x0065,0x0064,0x0064,0x0064,0x0064,0x0065,0x0066,0x0066,0x0062,0x0061,0x0060,0x0059,0x0055,0x004a,0x0049,0x0047,0x0047,0x0048,0x004a,0x004f,0x0053,0x0056,0x005c,0x005d,0x0060,0x0061,0x0062,0x0064,0x0064,0x0064
, 0x0065,0x0066,0x0067,0x0068,0x0068,0x0069,0x0070,0x0070,0x0055,0x0030,0x0004,0x0028,0x005d,0x006c,0x0069,0x0069,0x0069,0x0069,0x0068,0x0067,0x0065,0x0063,0x0061,0x005e,0x005e,0x0062,0x0064,0x0066,0x0067,0x0068,0x0068,0x006a
};
// Scanning window size
#define ECG_WINDOW_SIZE 8
// ECG data circular buffer size
#define ECG_BUFFER_SIZE 256
// ECG data circular buffer
SHORT  ecgBuffer[ECG_BUFFER_SIZE];

// GLOBAL DEFINITIONS AND VARIABLES FOR PLOTTING POTENTIOMETER DEMO
// Graph shift
#define POT_MOVE_DELTA  2
// Potentiometer circular buffer size
#define POT_BUFFER_SIZE 320
// Potentiometer data circular buffer
SHORT  potBuffer[POT_BUFFER_SIZE];

// Temporary buffer for graph demo screens
SHORT tempBuffer[10];

// Variables for date and time 

SCREEN_STATES prevState = -1;	// used to mark state where time setting was called	
OBJ_HEADER *pListSaved;			// temporary storage for object list
WORD 		DateItemID;			// ID of date item being modified (ID_EB_MONTH(DAY)(YEAR))


/////////////////////////////////////////////////////////////////////////////
//                                  MAIN
/////////////////////////////////////////////////////////////////////////////

#if 0
int main(void)
{
	GOL_MSG msg;       			// GOL message structure to interact with GOL

    EEPROMInit();   			// Initialize EEPROM
    TickInit();     			// Start tick counter    
    GOLInit();      			// Initialize graphics library and crete default style scheme for GOL
    BeepInit();     			// Initialize beeper
    TouchInit();    			// Initialize touch screen
    RTCCInit(); 				// Setup the RTCC
    RTCCProcessEvents();

    // If S3 button on Explorer 16 board is pressed calibrate touch screen
    if(PORTDbits.RD6 == 0)
    {
        TouchCalibration();
        TouchStoreCalibration();
    }

    // If it's a new board (EEPROM_VERSION byte is not programed) calibrate touch screen
    if(GRAPHICS_LIBRARY_VERSION != EEPROMReadWord(EEPROM_VERSION))
    {
        TouchCalibration();
        TouchStoreCalibration();
        //EEPROMWriteWord(GRAPHICS_LIBRARY_VERSION,EEPROM_VERSION);
    }

    // Load touch screen calibration parameters from EEPROM
    TouchLoadCalibration();

    StartScreen();  // Show intro screen and wait for touch

	// create the alternate schemes
	navScheme = GOLCreateScheme(); 		// alternative scheme for the navigate buttons
    altScheme = GOLCreateScheme(); 		// create alternative 1 style scheme
    alt2Scheme = GOLCreateScheme(); 	// create alternative 2 style scheme
    alt3Scheme = GOLCreateScheme(); 	// create alternative 3 style scheme
    meterScheme = GOLCreateScheme(); 	// create meter scheme
    redScheme = GOLCreateScheme();   	// create red style scheme
    greenScheme = GOLCreateScheme(); 	// create green style scheme
    yellowScheme = GOLCreateScheme(); 	// create yellow style scheme

#ifdef _S6D0129L_H 
	/* for Microtips display */
	altScheme->Color0 = RGB565CONVERT(0x4C, 0x7E, 0xFF);
    altScheme->Color1 = RGB565CONVERT(0xFF, 0xCB, 0x3C);
	altScheme->EmbossDkColor = RGB565CONVERT(0x1E, 0x00, 0xE5);
	altScheme->EmbossLtColor = RGB565CONVERT(0xA9, 0xDB, 0xEF);
    altScheme->ColorDisabled = RGB565CONVERT(0xD4, 0xE1, 0xF7);
    altScheme->TextColor1 = RGB565CONVERT(0x4C, 0x7E, 0xFF);
    altScheme->TextColor0 = RGB565CONVERT(0xFF, 0xCB, 0x3C);
    altScheme->TextColorDisabled = RGB565CONVERT(0xB8, 0xB9, 0xBC);
#endif // _S6D0129L_H 

#ifdef _R61505UL_H 
	/* for Truly display */
	altScheme->Color0 = RGB565CONVERT(0x4C, 0x8E, 0xFF);
    altScheme->Color1 = RGB565CONVERT(0xFF, 0xBB, 0x4C);
	altScheme->EmbossDkColor = RGB565CONVERT(0x1E, 0x00, 0xE5);
	altScheme->EmbossLtColor = RGB565CONVERT(0xA9, 0xDB, 0xEF);
    altScheme->ColorDisabled = RGB565CONVERT(0xD4, 0xE1, 0xF7);

    altScheme->TextColor1 = BRIGHTBLUE;
    altScheme->TextColor0 = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    altScheme->TextColorDisabled = RGB565CONVERT(0xB8, 0xB9, 0xBC);
#endif // _R61505UL_H 

    alt2Scheme->TextColor1 = BRIGHTRED;
    alt2Scheme->TextColor0 = BRIGHTBLUE;
	alt2Scheme->pFont = &GOLMediumFont;
	
    alt3Scheme->Color0 = LIGHTBLUE; 		
    alt3Scheme->Color1 = BRIGHTGREEN;
    alt3Scheme->TextColor0 = BLACK;
    alt3Scheme->TextColor1 = WHITE;
	alt3Scheme->pFont = &GOLMediumFont;

    redScheme->Color0 = RGB565CONVERT(0xCC, 0x00, 0x00); 
    redScheme->Color1 = BRIGHTRED;
    redScheme->EmbossDkColor = RED4;
    redScheme->EmbossLtColor = FIREBRICK1;
    redScheme->TextColor0 = RGB565CONVERT(0xC8, 0xD5, 0x85); 
    redScheme->TextColor1 = BLACK; 
    
    greenScheme->Color0 = RGB565CONVERT(0x23, 0x9E, 0x0A); 
    greenScheme->Color1 = BRIGHTGREEN;
    greenScheme->EmbossDkColor = DARKGREEN;
    greenScheme->EmbossLtColor = PALEGREEN;
    greenScheme->TextColor0 = RGB565CONVERT(0xDF, 0xAC, 0x83); 
    greenScheme->TextColor1 = BLACK; 

    yellowScheme->Color0 = BRIGHTYELLOW; 
    yellowScheme->Color1 = YELLOW;
    yellowScheme->EmbossDkColor = RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowScheme->EmbossLtColor = RGB565CONVERT(0xFD, 0xFF, 0xB2);
    yellowScheme->TextColor0 = RGB565CONVERT(0xAF, 0x34, 0xF3);
    yellowScheme->TextColor1 = RED;

    meterScheme->Color0 = BLACK; 		
    meterScheme->Color1 = WHITE;
    meterScheme->TextColor0 = BRIGHTBLUE;
    meterScheme->TextColor1 = WHITE;
	meterScheme->pFont = &GOLSmallFont;
    meterScheme->EmbossDkColor = GRAY20; 		
    meterScheme->EmbossLtColor = GRAY80;    

    while(1)
    {
        if(GOLDraw())
        {             // Draw GOL objects
            // Drawing is done here, process messages
            TouchGetMsg(&msg);     // Get message from touch screen
            GOLMsg(&msg);          // Process message
            SideButtonsMsg(&msg);  // Get message from side buttons
            GOLMsg(&msg);          // Process message
        }
    }
}
#else

int init_mainDemo( void )
{
	//EEPROMInit();   			// Initialize EEPROM
    TickInit();     			// Start tick counter    
    GOLInit();      			// Initialize graphics library and crete default style scheme for GOL
    BeepInit();     			// Initialize beeper
    TouchInit();    			// Initialize touch screen
    RTCCInit(); 				// Setup the RTCC
    //RTCCProcessEvents();

    // If S3 button on Explorer 16 board is pressed calibrate touch screen
//    if(PORTDbits.RD6 == 0)
//    {
//        TouchCalibration();
//        TouchStoreCalibration();
//    }
//
//    // If it's a new board (EEPROM_VERSION byte is not programed) calibrate touch screen
//    if(GRAPHICS_LIBRARY_VERSION != EEPROMReadWord(EEPROM_VERSION))
//    {
//        TouchCalibration();
//        TouchStoreCalibration();
//        //EEPROMWriteWord(GRAPHICS_LIBRARY_VERSION,EEPROM_VERSION);
//    }
//
//    // Load touch screen calibration parameters from EEPROM
//    TouchLoadCalibration();

    StartScreen();  // Show intro screen and wait for touch

	// create the alternate schemes
	navScheme = GOLCreateScheme(); 		// alternative scheme for the navigate buttons
    altScheme = GOLCreateScheme(); 		// create alternative 1 style scheme
    alt2Scheme = GOLCreateScheme(); 	// create alternative 2 style scheme
    alt3Scheme = GOLCreateScheme(); 	// create alternative 3 style scheme
    meterScheme = GOLCreateScheme(); 	// create meter scheme
    redScheme = GOLCreateScheme();   	// create red style scheme
    greenScheme = GOLCreateScheme(); 	// create green style scheme
    yellowScheme = GOLCreateScheme(); 	// create yellow style scheme
///span110109, add my test code for Scheme 
    myScheme = GOLCreateScheme(); 		// create my style scheme
///span110109,

    alt2Scheme->TextColor1 = BRIGHTRED;
    alt2Scheme->TextColor0 = BRIGHTBLUE;
	alt2Scheme->pFont = &GOLMediumFont;
	
    alt3Scheme->Color0 = LIGHTBLUE; 		
    alt3Scheme->Color1 = BRIGHTGREEN;
    alt3Scheme->TextColor0 = BLACK;
    alt3Scheme->TextColor1 = WHITE;
	alt3Scheme->pFont = &GOLMediumFont;

    redScheme->Color0 = RGB565CONVERT(0xCC, 0x00, 0x00); 
    redScheme->Color1 = BRIGHTRED;
    redScheme->EmbossDkColor = RED4;
    redScheme->EmbossLtColor = FIREBRICK1;
    redScheme->TextColor0 = RGB565CONVERT(0xC8, 0xD5, 0x85); 
    redScheme->TextColor1 = BLACK; 
    
    greenScheme->Color0 = RGB565CONVERT(0x23, 0x9E, 0x0A); 
    greenScheme->Color1 = BRIGHTGREEN;
    greenScheme->EmbossDkColor = DARKGREEN;
    greenScheme->EmbossLtColor = PALEGREEN;
    greenScheme->TextColor0 = RGB565CONVERT(0xDF, 0xAC, 0x83); 
    greenScheme->TextColor1 = BLACK; 

    yellowScheme->Color0 = BRIGHTYELLOW; 
    yellowScheme->Color1 = YELLOW;
    yellowScheme->EmbossDkColor = RGB565CONVERT(0xFF, 0x94, 0x4C);
    yellowScheme->EmbossLtColor = RGB565CONVERT(0xFD, 0xFF, 0xB2);
    yellowScheme->TextColor0 = RGB565CONVERT(0xAF, 0x34, 0xF3);
    yellowScheme->TextColor1 = RED;

///span110109, add my test code for Scheme 
    myScheme->Color0 = BRIGHTCYAN; 
    myScheme->Color1 = CYAN;
    myScheme->EmbossDkColor = RGB565CONVERT(0xFF, 0x94, 0x4C);
    myScheme->EmbossLtColor = RGB565CONVERT(0xFD, 0xFF, 0xB2);
    myScheme->TextColor0 = RGB565CONVERT(0xAF, 0x34, 0xF3);
    myScheme->TextColor1 = BLACK;
///span110109,

    meterScheme->Color0 = BLACK; 		
    meterScheme->Color1 = WHITE;
    meterScheme->TextColor0 = BRIGHTBLUE;
    meterScheme->TextColor1 = WHITE;
	meterScheme->pFont = &GOLSmallFont;
    meterScheme->EmbossDkColor = GRAY20; 		
    meterScheme->EmbossLtColor = GRAY80;  
	
	return 0;
}

#include <stdio.h> // for printf

int mainDemo( GOL_MSG * pMsg )
{
	if(GOLDraw())           // Draw GOL objects
    {   
        // Drawing is done here, process messages
        TouchGetMsg(pMsg);     // Get message from touch screen
        GOLMsg(pMsg);          // Process message
        SideButtonsMsg(pMsg);  // Get message from side buttons
        GOLMsg(pMsg);          // Process message
        //printf("%s:%s():%d\n", __FILE__, __FUNCTION__, __LINE__);     	
    }
    
    return 0;
}

#endif

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message 
// Output: if the function returns non-zero the message will be processed by default
// Overview: this function must be implemented by user. GOLMsg() function calls it each
//           time the valid message for the object received
/////////////////////////////////////////////////////////////////////////////
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{

    // beep if button is pressed
    if(objMsg == BTN_MSG_PRESSED)
    {
        Beep();
    }
    else
    {
        if(GetObjType(pObj) == OBJ_RADIOBUTTON)
        {
            Beep();
        }
        else
        {
            if(GetObjType(pObj) == OBJ_CHECKBOX)
                Beep();
        }
    }

	if ((screenState & 0xF300) != 0xF300) 
	{
		// check for time setting press, process only when not setting time and date
		if (objMsg == ST_MSG_SELECTED) 
		{
		    /* note how the states are arranged in the enumeration, the display state is 
		       right after the create state. So at the time the static text box of the 
		       time is selected, the state is in one of the displays. So a minus one is needed
		       to adjust to go back to the create state of the current page.
		       After the date and time adjust screen is exited, the saved create state will
		       be entered as the next state. So we get the effect of going back to the previous
		       screen after date and time settings are done.
		    */ 	
			if ((GetObjID(pObj) == ID_STATICTEXT1) || (GetObjID(pObj) == ID_STATICTEXT2)) 
			{
				prevState = screenState - 1;			// save the current create state
	    	    screenState = CREATE_DATETIME;			// go to date and time setting screen
		    	return 1;	
		    }
	    } 
    }

	// check if pull down menu is called
    if (GetObjID(pObj) == ID_WINDOW1) 
    {
	    if ((objMsg == WND_MSG_TITLE) && (screenState != DISPLAY_PULLDOWN))
	    {
		    // check area of press
		    if ((pMsg->param1 <= 40) && (pMsg->param2 <= 40)) 
		    {
			    switch(screenState) 
			    {
				    // if one of these states we redraw the whole screen since
				    // these screens have customized graphics.
			    	case DISPLAY_SLIDER:
			    		prevState = CREATE_SLIDER; break;
			    	case DISPLAY_CUSTOMCONTROL:
			    		prevState = CREATE_CUSTOMCONTROL; break;
			    	case DISPLAY_SIGNATURE:
			    		prevState = CREATE_SIGNATURE; break;
			    	case DISPLAY_POT:
			    		prevState = CREATE_POT; break;
			    	case DISPLAY_ECG:
			    		prevState = CREATE_ECG; break;
			    	case DISPLAY_PROGRESSBAR:
			    		prevState = CREATE_PROGRESSBAR; break;
			    		
					// pull down is disabled when setting date and time
			    	case CREATE_DATETIME: 		
    				case DISPLAY_DATETIME: 		
    				case DISPLAY_SETDATE: 
    				case HIDE_DATE_PDMENU:
    					return 0; 
    		    	default:	
		    			prevState = screenState;		// save the current create state
			    	 	break;
			   	}
			    screenState = CREATE_PULLDOWN;			// go to show pulldown menu state 
			    return 1;
			}
		}
	}
   
    // process messages for demo screens
    switch(screenState){
///span110110, add screen for Reno demo
        case DISPLAY_RENO_HOME:
            return MsgRenoHome(objMsg, pObj);
///span110110,
        case DISPLAY_BUTTONS:
            return MsgButtons(objMsg, pObj);
        case DISPLAY_CHECKBOXES:
            return MsgCheckBoxes(objMsg, pObj);
        case DISPLAY_RADIOBUTTONS:
            return MsgRadioButtons(objMsg, pObj);
        case DISPLAY_STATICTEXT:
            return MsgStaticText(objMsg, pObj);
        case DISPLAY_PICTURE:
            return MsgPicture(objMsg, pObj);
        case DISPLAY_SLIDER:
            return MsgSlider(objMsg, pObj, pMsg);
        case DISPLAY_PROGRESSBAR:
            return MsgProgressBar(objMsg, pObj);
        case DISPLAY_DATETIME:
            return MsgDateTime(objMsg, pObj);
        case DISPLAY_SETDATE:
         	return MsgSetDate(objMsg, pObj, pMsg);    
        case DISPLAY_METER:
            return MsgMeter(objMsg, pObj);
        case DISPLAY_DIAL:
            return MsgDial(objMsg, pObj);
        case DISPLAY_CUSTOMCONTROL:
            return MsgCustomControl(objMsg, pObj, pMsg);
        case DISPLAY_LISTBOX:
            return MsgListBox(objMsg, pObj, pMsg);
        case DISPLAY_EDITBOX:
            return MsgEditBox(objMsg, pObj, pMsg);
        case DISPLAY_SIGNATURE:
            return MsgSignature(objMsg, pObj, pMsg);
        case DISPLAY_POT:
            return MsgPotentiometer(objMsg, pObj);
        case DISPLAY_ECG:
            return MsgECG(objMsg, pObj);
        case DISPLAY_PULLDOWN:
        	return MsgPullDown(objMsg, pObj, pMsg);
        default:
            // process message by default
            return 1;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLDrawCallback()
// Output: if the function returns non-zero the draw control will be passed to GOL
// Overview: this function must be implemented by user. GOLDraw() function calls it each
//           time when GOL objects drawing is completed. User drawing should be done here.
//           GOL will not change color, line type and clipping region settings while 
//           this function returns zero.
/////////////////////////////////////////////////////////////////////////////
WORD GOLDrawCallback()
{
	static DWORD prevTick  = 0;  		// keeps previous value of tick
	static DWORD prevTime  = 0;  		// keeps previous value of time tick
	static BYTE  direction = 1; 		// direction switch for progress bar
	static BYTE  arrowPos  = 0;  		// arrows pictures position for custom control demo
	static BYTE  pBDelay   = 40;  		// progress bar delay variable
	static char  dateTimeStr[26] = "";	// string variable for date and time display
	OBJ_HEADER 	*pObj;					// used to change text in Window
	SLIDER     	*pSld;					// used when updating date and time
	LISTBOX    	*pLb;					// used when updating date and time
	WORD  		i;								

	// update the time display
	if ((screenState & 0xF300) != 0xF300) 
	{				// process only when NOT setting time and date
		if ((tick-prevTime) > 1000)
		{
			RTCCProcessEvents();						// update the date and time string variabes 
			i = 0;						
			while (i < 12) 
			{
				dateTimeStr[i] = _time_str[i];
				dateTimeStr[i+13] = _date_str[i];
				i++;
			}
			dateTimeStr[12] = '\n';
			dateTimeStr[25] = '\0';
			pObj = GOLFindObject(ID_STATICTEXT1);		// get the time display obj pointer
			if( pObj )
			{
				StSetText((STATICTEXT *)pObj, dateTimeStr); // now display the new date & time
				SetState(pObj, ST_DRAW);					// redraw the time display
			}
			prevTime = tick;							// reset tick timer
		}		
	}
	else 
	{	
		if (screenState != DISPLAY_SETDATE) 
		{			// do not update when pull down menus are on
			if ((tick-prevTime) > 1000)
			{
			    updateDateTimeEb();						// update edit boxes for date and time settings
				prevTime = tick;						// reset tick timer
			}
		}
	}
	
    switch(screenState)
    {
		
///span110110, add screen for Reno demo
        case CREATE_RENO_HOME:
            CreateRenoHome(); 							// create window and buttons
            screenState = DISPLAY_RENO_HOME; 			// switch to next state
            return 1; 									// draw objects created
            
        case DISPLAY_RENO_HOME:
            return 1; 									// redraw objects if needed
            
///span110110,
        case CREATE_BUTTONS:
            CreateButtons(); 							// create window and buttons
            screenState = DISPLAY_BUTTONS; 				// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_BUTTONS:
            return 1; 									// redraw objects if needed

        case CREATE_CHECKBOXES:
            CreateCheckBoxes(); 						// create window and check boxes
            screenState = DISPLAY_CHECKBOXES; 			// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_CHECKBOXES:
            return 1; 									// redraw objects if needed

        case CREATE_RADIOBUTTONS:
            CreateRadioButtons(); 						// create window and radio buttons
            screenState = DISPLAY_RADIOBUTTONS; 		// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_RADIOBUTTONS:
            return 1; 									// redraw objects if needed

        case CREATE_STATICTEXT:
            CreateStaticText();  						// create window, group box, static text 
            											// and radio buttons for control
            screenState = DISPLAY_STATICTEXT; 			// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_STATICTEXT:
        	 if((tick-prevTick)>4000) 
        	 {
	        	 pObj = GOLFindObject(ID_WINDOW1);
	        	 if (direction) 
	        	 {
	        	 	WndSetText((WINDOW*)pObj,"Group Box"); 
	        	 	direction = 0;
	        	 }
	        	 else 
	        	 {	
	        	 	WndSetText((WINDOW*)pObj,"Static text"); 
	        		direction = 1;
	        	 }
	        	 SetState(pObj, WND_DRAW_TITLE); 	
	        	 prevTick = tick;
	         }
             return 1; 									// redraw objects if needed

        case CREATE_SLIDER:
            CreateSlider(); 							// create window and sliders
            screenState = CURSOR_DRAW_SLIDER; 			// switch to next state
            return 1; 									// draw objects created

        case CURSOR_DRAW_SLIDER:
            DrawSliderCursor(BLACK); 					// draw sliders position cursor
            screenState = DISPLAY_SLIDER; 				// switch to next state

        case DISPLAY_SLIDER:
            return 1; 									// redraw objects if needed

        case CREATE_PROGRESSBAR:
            CreateProgressBar(); 						// create window and progress bar
            screenState = DISPLAY_PROGRESSBAR; 			// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_PROGRESSBAR:
        	if(GetState(pGenObj, BTN_PRESSED)) 
        		pBDelay = 8;
        	else	
        		pBDelay = 40;
        		
            if((tick-prevTick)>pBDelay)
            {
                if(direction)
                {
                    if(pProgressBar->pos == pProgressBar->range)
                        direction = 0;    				// change direction
                    else
                        PbSetPos(pProgressBar,PbGetPos(pProgressBar)+1); // increase
                    
                }
                else
                {
                    if(pProgressBar->pos == 0)
                        direction = 1;    				// change direction
                    else
                        PbSetPos(pProgressBar,PbGetPos(pProgressBar)-1); // decrease
                }
                SetState(pProgressBar,PB_DRAW_BAR); 	// redraw bar only
                prevTick = tick;
            }
            return 1; 									// redraw objects if needed

        case CREATE_EDITBOX:
            CreateEditBox();  					    	// create edit box test screen
            screenState = DISPLAY_EDITBOX;   			// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_EDITBOX:
            return 1; 									// draw objects

        case CREATE_LISTBOX:
            CreateListBox();  					    	// create list box test screen
            screenState = DISPLAY_LISTBOX;   			// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_LISTBOX:
			// this moves the slider and editbox for the date setting to 
        	// move while the up or down arrow buttons are pressed
        	if((tick-prevTick)>100) 
        	{
        		pLb = (LISTBOX*)GOLFindObject(ID_LISTBOX1);		
			    pSld = (SLIDER*)GOLFindObject(ID_SLIDER1);
				pObj = GOLFindObject(ID_BUTTON1);
				
            	if(GetState(pObj, BTN_PRESSED)) 
            	{
                	LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)-1);
                	SetState(pLb, LB_DRAW_ITEMS);
                	SldSetPos(pSld,SldGetPos(pSld)+1);
                	SetState(pSld, SLD_DRAW_THUMB);
                }	
				pObj = GOLFindObject(ID_BUTTON2);
				
            	if(GetState(pObj, BTN_PRESSED)) 
            	{
	                LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)+1);
    	            SetState(pLb, LB_DRAW_ITEMS);
        	        SldSetPos(pSld,SldGetPos(pSld)-1);
            	    SetState(pSld, SLD_DRAW_THUMB);
            	}
            	prevTick = tick; 
            }
            return 1; 									// draw objects

        case CREATE_DATETIME:
            CreateDateTime(); 							// create date and time demo
            screenState = DISPLAY_DATETIME; 			// switch to next state
            return 1; 									// draw objects created

		case HIDE_DATE_PDMENU:
			if (RemovePullDownMenu())
	            screenState = DISPLAY_DATETIME; 		// switch to next state
			return 1;

        case DISPLAY_SETDATE:
        	// this moves the slider and editbox for the date setting to 
        	// move while the up or down arrow buttons are pressed
        	if((tick-prevTick)>100) 
        	{
        		pLb = (LISTBOX*)GOLFindObject(ID_LISTBOX1);		
			    pSld = (SLIDER*)GOLFindObject(ID_SLIDER1);
				pObj = GOLFindObject(ID_BUTTON_DATE_UP);
				
            	if(GetState(pObj, BTN_PRESSED)) 
            	{
                	LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)-1);
                	SetState(pLb, LB_DRAW_ITEMS);
                	SldSetPos(pSld,SldGetPos(pSld)+1);
                	SetState(pSld, SLD_DRAW_THUMB);
                }	
				pObj = GOLFindObject(ID_BUTTON_DATE_DN);
				
            	if(GetState(pObj, BTN_PRESSED)) 
            	{
	                LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)+1);
    	            SetState(pLb, LB_DRAW_ITEMS);
        	        SldSetPos(pSld,SldGetPos(pSld)-1);
            	    SetState(pSld, SLD_DRAW_THUMB);
            	}
            	prevTick = tick; 
            }
            return 1;

       case DISPLAY_DATETIME:
        	// this increments the values for the time settings
        	// while the + or - buttons are pressed
       		if((tick-prevTick)>200) 
       		{
				pObj = GOLFindObject(ID_BUTTONHR_P);
            	if(GetState(pObj, BTN_PRESSED)) 
            	{
	            	MsgDateTime(BTN_MSG_PRESSED, pObj);
	            }
				pObj = GOLFindObject(ID_BUTTONHR_M);
            	if(GetState(pObj, BTN_PRESSED)) 
            	{
	            	MsgDateTime(BTN_MSG_PRESSED, pObj);
	            }
				pObj = GOLFindObject(ID_BUTTONMN_P);
            	if(GetState(pObj, BTN_PRESSED)) 
            	{
	            	MsgDateTime(BTN_MSG_PRESSED, pObj);
	            }
				pObj = GOLFindObject(ID_BUTTONMN_M);
            	if(GetState(pObj, BTN_PRESSED)) 
            	{
	            	MsgDateTime(BTN_MSG_PRESSED, pObj);
	            }
				pObj = GOLFindObject(ID_BUTTONSC_P);
            	if(GetState(pObj, BTN_PRESSED)) 
            	{
	            	MsgDateTime(BTN_MSG_PRESSED, pObj);
	            }
				pObj = GOLFindObject(ID_BUTTONSC_M);
            	if(GetState(pObj, BTN_PRESSED)) 
            	{
	            	MsgDateTime(BTN_MSG_PRESSED, pObj);
	            }
            	prevTick = tick; 
	        }
	        return 1;
 
        case CREATE_METER:
            CreateMeter(); 								// create meter demo
            screenState = DISPLAY_METER; 				// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_METER:
            if((tick-prevTick)>60)
            {
	        	UpdateMeter();
                prevTick = tick;
            }
            return 1; 									// redraw objects if needed

        case CREATE_DIAL:
            CreateDial(); 								// create dial demo
            screenState = DISPLAY_DIAL; 				// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_DIAL:
            return 1; 									// redraw objects if needed

        case CREATE_PICTURE:
            CreatePicture(); 							// create window, picture control and some check boxes
            screenState = DISPLAY_PICTURE; 				// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_PICTURE:
            if(animate)
            {                  				// if animation is on change picture
	            if((tick-prevTick)>(SldGetPos(pSlider)+40))
	            {
				    switch(animate)
				    {
                        case 1:
                            PictSetBitmap(pPicture,&Engine1);
                            animate++;
                            break;
                        case 2:
                            PictSetBitmap(pPicture,&Engine2);
                            animate++;
                            break;
                        case 3:
                            PictSetBitmap(pPicture,&Engine3);
                            animate++;
                            break;
                        case 4:
                            PictSetBitmap(pPicture,&Engine4);
                            animate = 1;
                            break;
                    }
                    SetState(pPicture,PICT_DRAW); 		// must be redrawn
                    prevTick = tick;
                }
            }
            return 1; 									// redraw objects if needed

        case CREATE_CUSTOMCONTROL:
            CreateCustomControl(); 						// create window and custom control
            screenState = DISPLAY_CUSTOMCONTROL; 		// switch to next state
            return 1; 									// draw objects created

        case DISPLAY_CUSTOMCONTROL:
			#define SX_ARROW 18
			#define SY_ARROW 26
            //if((tick-prevTick)>2)
            {
                arrowPos++;  							// change arrows' position
                if(arrowPos>60-SY_ARROW) 
                    arrowPos = 0;

                PutImage(70,110-SY_ARROW-arrowPos,&arrowUp,1); // draw arrows
                PutImage(70,110+arrowPos,&arrowDown,1);
                PutImage(250-SX_ARROW,110-SY_ARROW-arrowPos,&arrowUp,1);
                PutImage(250-SX_ARROW,110+arrowPos,&arrowDown,1);
                prevTick = tick;
            }
            return 1; 									// redraw objects if needed

        case CREATE_SIGNATURE:
            CreateSignature();  						// create window 
            screenState = BOX_DRAW_SIGNATURE; 			// switch to next state
            return 1; 									// draw objects created

        case BOX_DRAW_SIGNATURE:
            if(0 == PanelSignature()) 					// draw box for signature
                return 0; 								// drawing is not completed, don't pass 
                										// drawing control to GOL, try it again
            screenState = DISPLAY_SIGNATURE; 			// switch to next state
            return 1; 									// pass drawing control to GOL, redraw objects if needed

        case DISPLAY_SIGNATURE:
            return 1; 									// redraw objects if needed

        case CREATE_POT:
            CreatePotentiometer();                  	// create window
            screenState = BOX_DRAW_POT; 				// switch to next state
            return 1; 									// draw objects created

        case BOX_DRAW_POT:
            if(0 == PanelPotentiometer())        	    // draw box for potentiometer graph
                return 0;  								// drawing is not completed, don't pass 
                										// drawing control to GOL, try it again
            screenState = DISPLAY_POT; 					// switch to next state
            return 1; 									// pass drawing control to GOL, redraw objects if needed

        case DISPLAY_POT:
            if((tick-prevTick)>20)
            {
                if(GetPotSamples(POT_MOVE_DELTA))
                    GraphPotentiometer();           	// redraw graph
                prevTick = tick;
            }
            return 1; 									// redraw objects if needed

        case CREATE_ECG:
            CreateECG(); 								// create window
            screenState = BOX_DRAW_ECG; 				// switch to next state
            return 1; 									// draw objects created

        case BOX_DRAW_ECG:
            if(0 == PanelECG()) 						// draw box for ECG graph
                return 0;  								// drawing is not completed, don't pass 
                										// drawing control to GOL, try it again
            screenState = DISPLAY_ECG; 					// switch to next state
            return 1; 									// pass drawing control to GOL, redraw objects if needed

        case DISPLAY_ECG:
            //if((tick-prevTick)>1)
            {
                if(GetECGSamples(ECG_WINDOW_SIZE))
                    GraphECG(); 						// redraw graph
                prevTick = tick;
            }
            return 1; 									// redraw objects if needed
        case CREATE_PULLDOWN:	
        	CreatePullDown();
        	screenState = DISPLAY_PULLDOWN; 
        	return 1;    
        	
        case DISPLAY_PULLDOWN:
            return 1; 									// redraw objects if needed
    }

    return 1;    										// release drawing control to GOL

}

#include <stdio.h>  // for printf()...

// Shows intro screen and waits for touch
void StartScreen(void)
{	
	SHORT counter;
	static const char text[] = "Touch screen to continue";
	WORD i, j, k;
	WORD ytemp, xtemp, srRes = 0x0001; 
	
    SetColor(WHITE); 
    ClearDevice();      
    PutImage(0,0,&mchpLogo,IMAGE_NORMAL);
    PutImage(2,60,&intro,IMAGE_X2);

    for(counter=0;counter<320-32;counter++)
    {  // move Microchip icon
        PutImage(counter,205,&mchpIcon0,IMAGE_NORMAL);
        ///span110110, put more delay so we can see the logo.
        usleep(5000); ///usleep(100);
    }
    SetColor(BRIGHTRED);
    SetFont(&FONTDEFAULT);
	
    MoveTo((GetMaxX()-GetTextWidth((char*)text,&FONTDEFAULT))>>1,182);
    while(!OutText((char*)text))
    	;    
    // wait for touch
    while(TouchGetX() == -1)
    	; 
    Beep();
	
	// random fade effect using a Linear Feedback Shift Register (LFSR)
    SetColor(WHITE);
    for (i = 1800; i > 0 ; i--) 
    {
		// for a 16 bit LFSR variable the taps are at bits: 1, 2, 4, and 15
   		srRes = (srRes >> 1) ^ (-(int)(srRes & 1) & 0x8006);  
    	xtemp = (srRes & 0x00FF)%40;
    	ytemp = (srRes >> 8)%30;
    	usleep( i/10 );
    	// by replicating the white (1x1) bars into 8 areas fading is faster
		for (j = 0; j < 8; j++) 
		{
    		for (k = 0; k < 8; k++)
	    		PutPixel(xtemp+(j*40), ytemp+(k*30)); 
	    }		
	}    
	//printf("%s:%s():%d\n", __FILE__, __FUNCTION__, __LINE__);
}

void CreatePage(char *pText) 
{
	OBJ_HEADER* obj;
	static char  dateTimeStr[26] = "";		// string variable for date and time display
	SHORT i;

    WndCreate(ID_WINDOW1,       		// ID
              0,0,GetMaxX(),GetMaxY(), 	// dimension
              WND_DRAW,                	// will be dislayed after creation
              &mchpIcon,               	// icon
              pText,	   				// set text 
              NULL);                   	// default GOL scheme 
       
    BtnCreate(ID_BUTTON_BACK,         	// button ID 
              0,40,					   	// left, top corner	
              25,GetMaxY(),0, 		   	// right, bottom corner (with radius = 0)
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "\x1c",                  	// BACK arrow as text
              navScheme);          	   	// use navigation scheme

    obj = (OBJ_HEADER*)BtnCreate(
    		  ID_BUTTON_NEXT,   	    // button ID 
              GetMaxX()-25,40,
              GetMaxX(),GetMaxY(),0,   	// dimension (with radius = 0)
              BTN_DRAW,               	// will be dislayed and disabled after creation
              NULL,					   	// no bitmap
              "\x1d",                  	// FORWARD arrow as text
              navScheme);          	   	// use navigation scheme

              
	RTCCProcessEvents();				// update the date and time strings
	i = 0;						
	while (i < 12) 
	{
		dateTimeStr[i] = _time_str[i];
		dateTimeStr[i+13] = _date_str[i];
		i++;
	}
	dateTimeStr[12] = '\n';
	dateTimeStr[25] = '\0';
	
    StCreate(ID_STATICTEXT1,           	// ID 
             GetMaxX()-(GetTextWidth(_time_str,&GOLSmallFont)+15),
             3,							// dimensions
             GetMaxX()-5,
             3+(GetTextHeight(&GOLSmallFont)<<1),	
             ST_DRAW|ST_FRAME|
             ST_CENTER_ALIGN,     		// will be dislayed, has frame and center aligned
             dateTimeStr, 				// text is from time value
             meterScheme);             	// alternate scheme   
                      
	GOLSetFocus(obj); 					// set focus for the button              
              
}

///span110110, add status bar component
void CreateStatusBar(char *pText) 
{
	OBJ_HEADER* obj;
	static char  dateTimeStr[26] = "";		// string variable for date and time display
	SHORT i;

    WndCreate(ID_WINDOW1,       		// ID
              0,0,GetMaxX(),GetMaxY(), 	// dimension
              WND_DRAW,                	// will be dislayed after creation
              &mchpIcon,               	// icon
              pText,	   				// set text 
              NULL);                   	// default GOL scheme 
       
    BtnCreate(ID_BUTTON_BACK,         	// button ID 
              0,40,					   	// left, top corner	
              25,GetMaxY(),0, 		   	// right, bottom corner (with radius = 0)
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "\x1c",                  	// BACK arrow as text
              navScheme);          	   	// use navigation scheme

    obj = (OBJ_HEADER*)BtnCreate(
    		  ID_BUTTON_NEXT,   	    // button ID 
              GetMaxX()-25,40,
              GetMaxX(),GetMaxY(),0,   	// dimension (with radius = 0)
              BTN_DRAW,               	// will be dislayed and disabled after creation
              NULL,					   	// no bitmap
              "\x1d",                  	// FORWARD arrow as text
              navScheme);          	   	// use navigation scheme

              
	RTCCProcessEvents();				// update the date and time strings
	i = 0;						
	while (i < 12) 
	{
		dateTimeStr[i] = _time_str[i];
		dateTimeStr[i+13] = _date_str[i];
		i++;
	}
	dateTimeStr[12] = '\n';
	dateTimeStr[25] = '\0';
	
    StCreate(ID_STATICTEXT1,           	// ID 
             GetMaxX()-(GetTextWidth(_time_str,&GOLSmallFont)+15),
             3,							// dimensions
             GetMaxX()-5,
             3+(GetTextHeight(&GOLSmallFont)<<1),	
             ST_DRAW|ST_FRAME|
             ST_CENTER_ALIGN,     		// will be dislayed, has frame and center aligned
             dateTimeStr, 				// text is from time value
             meterScheme);             	// alternate scheme   
                      
	GOLSetFocus(obj); 					// set focus for the button              
              
}
///span110110,

/* definitions for the pull down menu demo */
#define BOTTOM_NORMAL   40
#define BOTTOM_DROPPED  220

const char ScreenItems[] = "Buttons\rCheckbox\rRadio buttons\nGroup box\nSlider\nProgress bar\nList box\nEdit box\nMORE......";
const char ScreenItems2[] = "BACK......\n" "Meter\n" "Dial\n" "Picture\n" "Custom control\n" "Signature\n" "Plotting\n" "ECG"; 

void CreatePullDown() 
{
	OBJ_HEADER *pFObj;
	
	// before moving to a new list the current focused object is set to be not focused.
	// this saves us the effort to save the pointer to the object and setting 
	// the global pointer to the focused object to that saved object.
	// this application does not need it to be saved.
	pFObj = GOLGetFocus();				// get the focused object
	if (pFObj != NULL) 
	{	
		ClrState(pFObj, FOCUSED);		// clear the focus state
		SetState(pFObj, DRAW);			// set to be redrawn
	}
	pListSaved = GOLGetList();
	GOLNewList();

	LbCreate(ID_LISTBOX2,        		// ID
             40,BOTTOM_NORMAL,			// dimensions
             155,BOTTOM_DROPPED,            
             LB_SINGLE_SEL|LB_DRAW|
             LB_FOCUSED,				// will be created with single select and focused 
             (char*)ScreenItems,		// list of demo screens
             alt2Scheme);              	// use alternate scheme              

	// This object is a ghost object to enable to get messages that touch is outside 
	// the list box. It will never be displayed.
    GbCreate(ID_GROUPBOX2,             	// ID 
              0,0,GetMaxX(),GetMaxY(), 	// cover the whole screen
              0,                 		// do not display
              NULL,             		// no text
              NULL);                   	
}	

/* this adds items on the items list of the list box */
void AddItemList(char* pText, LISTBOX *pLb) 
{
	char* pointer;
	WORD  ctr; 
	
    if(pText != NULL)
    {
        pointer = pText;
        ctr = 0;
        while(*pointer)
        {
            if(NULL == LbAddItem(pLb, NULL, pointer, 0, ctr))
                break;
            while(*pointer++ > 31)
            	;
            if(*(pointer-1) == 0)
                break;
            ctr++;
        }	
    }    
}	

WORD MsgPullDown(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg) 
{
	
	LISTBOX *pLb2;
	SHORT    item, left, top, right, bottom;
	static SHORT  List2Enable = 0;


	pLb2 = (LISTBOX*)GOLFindObject(ID_LISTBOX2);					// find the pull down object
	if(pMsg->uiEvent == EVENT_RELEASE)
	{								// state transition must be done
	   	if (GetObjID(pObj) == ID_LISTBOX2) 
	   	{  						// if released outside list box, go back 
			item = LbGetFocusedItem(pLb2);							// get selected page/screen
			if (!List2Enable) 
			{										// For list 1
				switch (item) 
				{
					default: 
					case 0: screenState = CREATE_BUTTONS; 		break;
					case 1: screenState = CREATE_CHECKBOXES;  	break;
					case 2: screenState = CREATE_RADIOBUTTONS;  break;
					case 3: screenState = CREATE_STATICTEXT;  	break;
					case 4: screenState = CREATE_SLIDER;  		break;
					case 5: screenState = CREATE_PROGRESSBAR;   break;
					case 6: screenState = CREATE_LISTBOX;  		break;
					case 7: screenState = CREATE_EDITBOX;  		break;
					case 8: screenState = prevState;			break;
				}
			} 
			else 
			{
				switch (item) 
				{										// For list 2
					default: 
					case 0: screenState = prevState;  			break;
					case 1: screenState = CREATE_METER;  		break;
					case 2: screenState = CREATE_DIAL;  		break;
					case 3: screenState = CREATE_PICTURE;  		break;
					case 4: screenState = CREATE_CUSTOMCONTROL; break;
					case 5: screenState = CREATE_SIGNATURE;  	break;
					case 6: screenState = CREATE_POT;  			break;
					case 7: screenState = CREATE_ECG;  			break;
				}
			}
        } 
        else 
        {
			screenState = prevState;								// revert to saved state
    	}
    	left = pLb2->left; top = pLb2->top; 						// save the dimensions
    	right = pLb2->right; bottom = pLb2->bottom;
    	SetColor(pLb2->pGolScheme->CommonBkColor);					// set the color
        Bar(left, top, right, bottom);								// hide the pull down menu
		GOLFree();   												// remove the pull down menu
   		GOLSetList(pListSaved);										// set active list back to saved list
    	GOLRedrawRec(left, top, right, bottom);						// redraw all objects that the pulldown 
    																// menu has overlapped
		return 1;

	} 
	else 
	{
		if ( (pMsg->uiEvent == EVENT_MOVE) ||					// convert messages to press
			 (pMsg->uiEvent == EVENT_PRESS)
		   )  
		{

			if (GetObjID(pObj) == ID_LISTBOX2) 
			{ 					// process only for list box(pull down menu)
				pMsg->uiEvent = EVENT_PRESS;						// now convert the message
				
				item = LbGetFocusedItem(pLb2);						// check item selected
				if ((!List2Enable) && (item == 8)) 
				{				// if item MORE.... change menu
					List2Enable = 1;
					LbDelItemsList(pLb2);							// remove the current item
					AddItemList((char*)ScreenItems2, pLb2);			// set the items list to ScreenItems2
					SetState(pLb2, LB_DRAW);
				}
				if ((List2Enable) && (item == 0)) 
				{					// if item BACK.... change back menu
					List2Enable = 0;								// remove the current item
					LbDelItemsList(pLb2);
					AddItemList((char*)ScreenItems, pLb2);			// set the items list to ScreenItems
					SetState(pLb2, LB_DRAW);
				}
				return 1;
			} 
		}
		return 0;
	}  	
	
}


void CreateButtons()
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	CreatePage("Buttons");

    BtnCreate(ID_BUTTON1, 				// button ID 
              30,50,156,100,           	// dimension
              10,					   	// set radius 
              BTN_DRAW,  			   	// draw a beveled button
              NULL,                    	// no bitmap
              "Button",               	// text
              altScheme);              	// use alternate scheme

    BtnCreate(ID_BUTTON2, 				// button ID 
              30,110,156,160,         	// dimension
              0,
              BTN_DRAW, 				// will be dislayed after creation 
              &gradientButton,          // use bitmap
              "HOME", 	            	// text
              altScheme);	            // alternative GOL scheme 
              
    BtnCreate(ID_BUTTON3,             	// button ID 
              165,50,215,160,          	// dimension
              25,					   	// set radius 
              BTN_DRAW|BTN_TOGGLE,   	// draw a vertical capsule button 
              						   	// that has a toggle behavior
              NULL,                    	// no bitmap
              "LO", 		           	// text              
              yellowScheme);           	// use alternate scheme

    BtnCreate(ID_BUTTON4,             	// button ID 
              225,43,285,103,          	// dimension
              30,					   	// set radius 
              BTN_DRAW, 			   	// draw a vertical capsule button
              NULL,                    	// no bitmap
              "ON",		               	// text
              greenScheme);             // use alternate scheme 

    BtnCreate(ID_BUTTON5,             	// button ID 
              225,107,285,167,         	// dimension
              30,					   	// set radius 
              BTN_DRAW|BTN_PRESSED,  	// draw a vertical capsule button
              							// that is initially pressed
              NULL,                    	// no bitmap
              "OFF",		           	// text
              redScheme);            	// use alternate scheme 	

    BtnCreate(ID_BUTTON6, 				// button ID 
              30,170,156,220,         	// dimension
              0,
              BTN_DRAW|BTN_TEXTLEFT 	// will be dislayed after creation with text
              |BTN_TEXTTOP, 			// left top position
              &duckReleased,            // use bitmap
              "TWEET!",             	// text
              alt2Scheme);	            // alternative GOL scheme 

    BtnCreate(ID_BUTTON7,          		// button ID 
              162,170,288,220,         	// dimension
              0,
              BTN_DRAW|BTN_DISABLED, 	// will be dislayed and disabled after creation
              NULL,                    	// no bitmap
              "Disabled",               // text
              altScheme);              	// use alternate scheme

}


///span110110, add screen for Reno demo
void CreateRenoHome()
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	///CreatePage("Reno");
	CreateStatusBar("Reno");

    BtnCreate(ID_BUTTON1, 				// button ID 
              30,50,156,100,           	// dimension
              10,					   	// set radius 
              BTN_DRAW,  			   	// draw a beveled button
              NULL,                    	// no bitmap
              "Button",               	// text
              altScheme);              	// use alternate scheme

    BtnCreate(ID_BUTTON2, 				// button ID 
              30,110,156,160,         	// dimension
              0,
              BTN_DRAW, 				// will be dislayed after creation 
              &gradientButton,          // use bitmap
              "HOME", 	            	// text
              altScheme);	            // alternative GOL scheme 
              
    BtnCreate(ID_BUTTON3,             	// button ID 
              165,50,215,160,          	// dimension
              25,					   	// set radius 
              BTN_DRAW|BTN_TOGGLE,   	// draw a vertical capsule button 
              						   	// that has a toggle behavior
              NULL,                    	// no bitmap
              "LO", 		           	// text
///span110109, add my test code for Scheme
#if 0               
              yellowScheme);           	// use alternate scheme
#else
              myScheme);           	// use my alternate scheme
#endif
///span110109, 

    BtnCreate(ID_BUTTON4,             	// button ID 
              225,43,285,103,          	// dimension
              30,					   	// set radius 
              BTN_DRAW, 			   	// draw a vertical capsule button
              NULL,                    	// no bitmap
              "ON",		               	// text
              greenScheme);             // use alternate scheme 
/*
    BtnCreate(ID_BUTTON5,             	// button ID 
              225,107,285,167,         	// dimension
              30,					   	// set radius 
              BTN_DRAW|BTN_PRESSED,  	// draw a vertical capsule button
              							// that is initially pressed
              NULL,                    	// no bitmap
              "OFF",		           	// text
              redScheme);            	// use alternate scheme 	

    BtnCreate(ID_BUTTON6, 				// button ID 
              30,170,156,220,         	// dimension
              0,
              BTN_DRAW|BTN_TEXTLEFT 	// will be dislayed after creation with text
              |BTN_TEXTTOP, 			// left top position
              &duckReleased,            // use bitmap
              "TWEET!",             	// text
              alt2Scheme);	            // alternative GOL scheme 

    BtnCreate(ID_BUTTON7,          		// button ID 
              162,170,288,220,         	// dimension
              0,
              BTN_DRAW|BTN_DISABLED, 	// will be dislayed and disabled after creation
              NULL,                    	// no bitmap
              "Disabled",               // text
              altScheme);              	// use alternate scheme
*/
}


WORD MsgRenoHome(WORD objMsg, OBJ_HEADER* pObj)
{
	OBJ_HEADER* pOtherRbtn;

    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
               screenState = CREATE_BUTTONS;// goto check box demo screen
            }
            return 1; 							// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_ECG; 		// goto ECG demo screen
            }
            return 1; 							// process by default
            
        case ID_BUTTON3:
        	if(objMsg == BTN_MSG_PRESSED){ 		// change text and scheme
                BtnSetText((BUTTON*)pObj, "HI");
            }
            else {
                BtnSetText((BUTTON*)pObj, "LO");
            }
            return 1;  							// process by default

        case ID_BUTTON4:

			if(objMsg == BTN_MSG_PRESSED) {
				if (!GetState(pObj,BTN_PRESSED)) { 	
	            	pOtherRbtn = GOLFindObject(ID_BUTTON5);
	            	ClrState(pOtherRbtn, BTN_PRESSED);
		        	SetState(pOtherRbtn, BTN_DRAW);  
	            	SetState(pObj, BTN_PRESSED|BTN_DRAW);	
	            	GOLSetFocus(pObj); 			// set focus for the button
	            }
	        }
		    return 0; 							// Do not process by default

        case ID_BUTTON5:
        
			if(objMsg == BTN_MSG_PRESSED) {
				if (!GetState(pObj,BTN_PRESSED)) { 
	        		pOtherRbtn = GOLFindObject(ID_BUTTON4);
	            	ClrState(pOtherRbtn, BTN_PRESSED);
		        	SetState(pOtherRbtn, BTN_DRAW);  
	            	SetState(pObj, BTN_PRESSED|BTN_DRAW);	
	            	GOLSetFocus(pObj); 			// set focus for the button
	            }
	        }
		    return 0; 							// Do not process by default

        case ID_BUTTON6:
            if(objMsg == BTN_MSG_PRESSED){ 		// change face picture
                BtnSetBitmap(pObj,&duckPressed);
                BtnSetText((BUTTON*)pObj, "OUCH!");
	        	//ClrState(pObj, 0x00F0);  
	        	//SetState(pObj, BTN_TEXTBOTTOM|BTN_TEXTRIGHT);  

            }
            if(objMsg == BTN_MSG_RELEASED){
                BtnSetBitmap(pObj,&duckReleased);
                BtnSetText((BUTTON*)pObj, "TWEET!");
	        	ClrState(pObj, 0x00F0);  
	        	SetState(pObj, BTN_TEXTTOP|BTN_TEXTLEFT);  
            }
            return 1; 							// process by default

        default:
            return 1; 							// process by default
    }
}

///span110110,


WORD MsgButtons(WORD objMsg, OBJ_HEADER* pObj)
{
	OBJ_HEADER* pOtherRbtn;

    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
               screenState = CREATE_CHECKBOXES;// goto check box demo screen
            }
            return 1; 							// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED){
///span110110, 
#if 0
                screenState = CREATE_ECG; 		// goto ECG demo screen
#else
                screenState = CREATE_RENO_HOME; 		// goto ECG demo screen
#endif                
            }
            return 1; 							// process by default
            
        case ID_BUTTON3:
        	if(objMsg == BTN_MSG_PRESSED){ 		// change text and scheme
                BtnSetText((BUTTON*)pObj, "HI");
            }
            else {
                BtnSetText((BUTTON*)pObj, "LO");
            }
            return 1;  							// process by default

        case ID_BUTTON4:

			if(objMsg == BTN_MSG_PRESSED) {
				if (!GetState(pObj,BTN_PRESSED)) { 	
	            	pOtherRbtn = GOLFindObject(ID_BUTTON5);
	            	ClrState(pOtherRbtn, BTN_PRESSED);
		        	SetState(pOtherRbtn, BTN_DRAW);  
	            	SetState(pObj, BTN_PRESSED|BTN_DRAW);	
	            	GOLSetFocus(pObj); 			// set focus for the button
	            }
	        }
		    return 0; 							// Do not process by default

        case ID_BUTTON5:
        
			if(objMsg == BTN_MSG_PRESSED) {
				if (!GetState(pObj,BTN_PRESSED)) { 
	        		pOtherRbtn = GOLFindObject(ID_BUTTON4);
	            	ClrState(pOtherRbtn, BTN_PRESSED);
		        	SetState(pOtherRbtn, BTN_DRAW);  
	            	SetState(pObj, BTN_PRESSED|BTN_DRAW);	
	            	GOLSetFocus(pObj); 			// set focus for the button
	            }
	        }
		    return 0; 							// Do not process by default

        case ID_BUTTON6:
            if(objMsg == BTN_MSG_PRESSED){ 		// change face picture
                BtnSetBitmap(pObj,&duckPressed);
                BtnSetText((BUTTON*)pObj, "OUCH!");
	        	//ClrState(pObj, 0x00F0);  
	        	//SetState(pObj, BTN_TEXTBOTTOM|BTN_TEXTRIGHT);  

            }
            if(objMsg == BTN_MSG_RELEASED){
                BtnSetBitmap(pObj,&duckReleased);
                BtnSetText((BUTTON*)pObj, "TWEET!");
	        	ClrState(pObj, 0x00F0);  
	        	SetState(pObj, BTN_TEXTTOP|BTN_TEXTLEFT);  
            }
            return 1; 							// process by default

        default:
            return 1; 							// process by default
    }
}

void CreateCheckBoxes()
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	CreatePage("Checkboxes");

    CbCreate(ID_CHECKBOX1,             	// ID 
              100,110,265,145,         	// dimension
              CB_DRAW,                 	// will be dislayed after creation
              "Text Left",              // text
              altScheme);              	// alternative GOL scheme 

    CbCreate(ID_CHECKBOX2,             	// ID 
              100,150,265,185,         	// dimension
              CB_DRAW,      			// will be dislayed and checked after creation
              "Text Bottom",           	// text
              altScheme);              	// alternative GOL scheme 

    CbCreate(ID_CHECKBOX3,             	// ID 
              100,190,250,225,         	// dimension
              CB_DRAW|CB_DISABLED,     	// will be dislayed and disabled after creation
              "Disabled",              	// text
              altScheme);              	// alternative GOL scheme 
              
    pGenObj = (OBJ_HEADER*)BtnCreate(
    		  ID_BUTTON1, 				// button ID 
              100,50,215,100,         	// dimension
              10,					   	// set radius 
              BTN_DRAW,  			   	// draw a beveled button
              NULL,                    	// no bitmap
              "HOME",               	// text
              altScheme);              	// use alternate scheme

}

WORD MsgCheckBoxes(WORD objMsg, OBJ_HEADER* pObj)
{

    switch(GetObjID(pObj))
    {

        case ID_CHECKBOX1:
        	if (objMsg == CB_MSG_CHECKED) 
        	{
				SetState(pGenObj, 
						 BTN_TEXTLEFT| 
						 BTN_DRAW);        			// set align left and redraw button	
			} else {
				ClrState(pGenObj, BTN_TEXTLEFT);	// clear all text alignment	
				SetState(pGenObj, BTN_DRAW);       	// set align left and redraw button	
			}
            return 1; 								// process by default

        case ID_CHECKBOX2:
        	if (objMsg == CB_MSG_CHECKED) {
				SetState(pGenObj, 
						 BTN_TEXTBOTTOM| 
						 BTN_DRAW);        			// set align bottom and redraw button	
			} else {
				ClrState(pGenObj, BTN_TEXTBOTTOM);	// clear all text alignment	
				SetState(pGenObj, BTN_DRAW);       	// set align left and redraw button	
			}
            return 1; 								// process by default

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_RADIOBUTTONS; 	// goto radio buttons screen
            }
            return 1; 								// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_BUTTONS;  		// goto round buttons screen
            }
            return 1; 								// process by default

        default:
            return 1; 								// process by default
    }
}

void CreateRadioButtons()
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list
	
	CreatePage("Radio buttons");


    GbCreate(ID_GROUPBOX1,             	// ID 
              30,45,157,230,           	// dimension
              GB_DRAW|GB_CENTER_ALIGN,  // will be dislayed after creation
              "Group 1",             	// text
              alt3Scheme);              // alternate scheme 

    GbCreate(ID_GROUPBOX2,             	// ID 
              160,45,290,230,           // dimension
              GB_DRAW|GB_CENTER_ALIGN,  // will be dislayed after creation
              "Group 2",             	// text
              alt3Scheme);              // alternate scheme 

    RbCreate(ID_RADIOBUTTON1,    	   	// ID 
              35,70,155,105,           	// dimension
              RB_DRAW|RB_GROUP|RB_CHECKED,	// will be dislayed and checked after creation
                                       	// first button in the group
              "Rb1",              		// text
              altScheme);              	// alternative GOL scheme 

    RbCreate(ID_RADIOBUTTON2,          	// ID 
              35,110,155,145,         	// dimension
              RB_DRAW,      			// will be dislayed 
              "Rb2",	                // text
              altScheme);              	// alternative GOL scheme 

    RbCreate(ID_RADIOBUTTON3,          	// ID 
              35,150,155,185,          	// dimension
              RB_DRAW,                 	// will be dislayed after creation
              "Rb3",	                // text
              altScheme);              	// alternative GOL scheme 

    RbCreate(ID_RADIOBUTTON4,          	// ID 
              35,190,155,225,         	// dimension
              RB_DRAW|RB_DISABLED,     	// will be dislayed and disabled after creation
              "Disabled",              	// text
              altScheme);              	// alternative GOL scheme 

    RbCreate(ID_RADIOBUTTON5,    	   	// ID 
              165,70,285,105,           // dimension
              RB_DRAW|RB_GROUP,        	// will be dislayed and focused after creation
                                       	// first button in the group
              "Rb4",               		// text
              altScheme);              	// alternative GOL scheme 

    RbCreate(ID_RADIOBUTTON6,          	// ID 
              165,110,285,145,          // dimension
              RB_DRAW|RB_CHECKED,      	// will be dislayed and checked after creation
              "Rb5",               		// text
              altScheme);              	// alternative GOL scheme 

    RbCreate(ID_RADIOBUTTON7,          	// ID 
              165,150,285,185,          // dimension
              RB_DRAW,                 	// will be dislayed after creation
              "Rb6",               		// text
              altScheme);              	// alternative GOL scheme 

}

WORD MsgRadioButtons(WORD objMsg, OBJ_HEADER* pObj)
{

    switch(GetObjID(pObj)){

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_STATICTEXT;// goto static text screen
            }
            return 1; 							// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_CHECKBOXES;// goto check boxes screen
            }
            return 1; 							// process by default

        default:
            return 1; 							// process by default
    }
}

void CreateStaticText(){

    GOLFree();  // free memory for the objects in the previous linked list and start new list

	CreatePage("Static text");

    GbCreate(ID_GROUPBOX1,             	// ID 
              30,45,235,230,           	// dimension
              GB_DRAW,                 	// will be dislayed after creation
              "Group Box",             	// text
              NULL);                   	// default GOL scheme 

    StCreate(ID_STATICTEXT3,           	// ID 
              35,75,225,220,           	// dimension
              ST_DRAW|ST_FRAME,        	// will be dislayed, has frame
              "Microchip\nGraphics\nLibrary\nStatic Text and\nGroup Box Test.", // multi-line text
              NULL);                   	// default GOL scheme 
    
    RbCreate(ID_RADIOBUTTON1,          	// ID 
              240,70,290,100,           // dimension
              RB_DRAW|RB_GROUP|	\
              RB_CHECKED, 				// will be dislayed and checked after creation
                                       	// first button in the group
              "L",                     	// text
              altScheme);              	// use alternate scheme

    RbCreate(ID_RADIOBUTTON2,          // ID 
              240,110,290,140,         // dimension
              RB_DRAW,                 // will be dislayed after creation
              "C",                     // text
              altScheme);              // use alternate scheme

    RbCreate(ID_RADIOBUTTON3,          // ID 
              240,150,290,180,         // dimension
              RB_DRAW,                 // will be dislayed after creation
              "R",                     // text
              altScheme);              // use alternate scheme

}

WORD MsgStaticText(WORD objMsg, OBJ_HEADER* pObj){
GROUPBOX   *pGb;
STATICTEXT *pSt;

    switch(GetObjID(pObj)){
        case ID_RADIOBUTTON1: // change aligment to left
            pGb = (GROUPBOX*) GOLFindObject(ID_GROUPBOX1); 	// get pointer to group box
            ClrState(pGb,GB_RIGHT_ALIGN|GB_CENTER_ALIGN);  	// clear right and center alignment states
            SetState(pGb, GB_DRAW);                        	// set redraw state
            pSt = (STATICTEXT*) GOLFindObject(ID_STATICTEXT3); // get pointer to static text
            ClrState(pSt,ST_RIGHT_ALIGN|ST_CENTER_ALIGN);  	// clear right and center alignment states    
            SetState(pSt, ST_DRAW);                        	// set redraw state
            return 1; 										// process by default

        case ID_RADIOBUTTON2:
            pGb = (GROUPBOX*) GOLFindObject(ID_GROUPBOX1); 	// get pointer to group box
            ClrState(pGb, GB_RIGHT_ALIGN);  				// clear right alignment state
            SetState(pGb, GB_CENTER_ALIGN|GB_DRAW);  		// set center alignment and redraw states
            pSt = (STATICTEXT*) GOLFindObject(ID_STATICTEXT3); // get pointer to static text
            ClrState(pSt,ST_RIGHT_ALIGN);  					// clear right aligment state
            SetState(pSt, ST_DRAW|ST_CENTER_ALIGN);  		// set center alignment and redraw states
            return 1; 										// process by default

        case ID_RADIOBUTTON3:
            pGb = (GROUPBOX*) GOLFindObject(ID_GROUPBOX1); 	// get pointer to group box
            ClrState(pGb, GB_CENTER_ALIGN);  				// clear center alignment state
            SetState(pGb, GB_RIGHT_ALIGN|GB_DRAW);  		// set right alignment and redraw state
            pSt = (STATICTEXT*) GOLFindObject(ID_STATICTEXT3); // get pointer to static text
            ClrState(pSt,ST_CENTER_ALIGN);  				// clear center aligment state
            SetState(pSt,ST_RIGHT_ALIGN|ST_DRAW); 			// set right alignment and redraw states
            return 1; // process by default

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_SLIDER;				// goto slider screen
            }
            return 1; 										// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_RADIOBUTTONS; 			// goto radio buttons screen
            }
            return 1; 										// process by default

        default:
            return 1;
    }
}

void CreateSlider()
{

    GOLFree();  // free memory for the objects in the previous linked list and start new list

	CreatePage("Slider");

    GbCreate(ID_GROUPBOX1,             		// ID 
              65,50,255,185,           		// dimension
              GB_DRAW,                 		// will be dislayed after creation
              NULL,                    		// no text
              altScheme);              		// alternative GOL scheme 

    SldCreate(ID_SLIDER1,                   // ID
              30, 190, 290, 220,            // dimension
              SLD_DRAW,                     // will be dislayed after creation
              CUR_BRD_RIGHT-CUR_BRD_LEFT,   // range
              10,                           // page 
              (CUR_BRD_RIGHT-CUR_BRD_LEFT)/2, 	// pos
              altScheme);              		// alternative GOL scheme 

    SldCreate(ID_SLIDER2,           		// ID
              30, 50, 60, 185,      		// dimension
              SLD_DRAW|SLD_VERTICAL|	
              SLD_DISABLED, 				// vertical, will be dislayed and disabled after creation
              100,                     		// range
              100,                     		// page 
              50,                      		// pos
              altScheme);              		// alternative GOL scheme 

    SldCreate(ID_SLIDER3,               	// ID
              260, 50, 290, 185,        	// dimension
              SLD_DRAW|SLD_VERTICAL,    	// vertical, will be dislayed after creation
              CUR_BRD_BOTTOM-CUR_BRD_TOP,	// range
              10,                       	// page 
              (CUR_BRD_BOTTOM-CUR_BRD_TOP)/2, // pos
              altScheme);              		// alternative GOL scheme 

    // Set cursor to the middle
    x = (CUR_BRD_RIGHT+CUR_BRD_LEFT)/2; 	// cursor X position
    y = (CUR_BRD_BOTTOM+CUR_BRD_TOP)/2; 	// cursor Y position 
}

// Draw sliders' position cursor
void DrawSliderCursor(WORD color)
{
    SetColor(color);    
    Bar(x-CUR_BAR_SIZE,y-CUR_BAR_SIZE,x+CUR_BAR_SIZE,y+CUR_BAR_SIZE);    
    SetLineType(DOTTED_LINE);
    Line(x,CUR_BRD_TOP,x,CUR_BRD_BOTTOM);
    Line(CUR_BRD_LEFT,y,CUR_BRD_RIGHT,y);    
    SetLineType(SOLID_LINE);
}

// Process messages for slider screen
WORD MsgSlider(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
	WORD id;
	SLIDER* pSld;

    switch(id = GetObjID(pObj))
    {

        case ID_WINDOW1:
            // check if window is touched in cursor box area
            if( (pMsg->param1>CUR_BRD_LEFT)&&
                (pMsg->param1<CUR_BRD_RIGHT)&&
                (pMsg->param2>CUR_BRD_TOP)&&
                (pMsg->param2<CUR_BRD_BOTTOM) )
            {
                // hide previous cursor
                DrawSliderCursor(altScheme->CommonBkColor);
                // get new cursor x,y 
                x = pMsg->param1;
                y = pMsg->param2;
                // draw new cursor
                DrawSliderCursor(BLACK);
                // set new sliders positions
                pSld = (SLIDER*)GOLFindObject(ID_SLIDER1); 	// get pointer for horizontal slider
                SldSetPos(pSld,x-CUR_BRD_LEFT); 			// set new position
                SetState(pSld,SLD_DRAW_THUMB);  			// redraw thumb only
                pSld = (SLIDER*)GOLFindObject(ID_SLIDER3); 	// get pointer for vertical slider
                SldSetPos(pSld,CUR_BRD_BOTTOM-y);  			// set new position
                SetState(pSld,SLD_DRAW_THUMB);  			// redraw thumb only
            }
            return 1; 										// process by default

        case ID_SLIDER1:
        case ID_SLIDER3:
            // process message by default
            SldMsgDefault(objMsg,(SLIDER*)pObj, pMsg);

            // hide previous cursor
            DrawSliderCursor(altScheme->CommonBkColor);
            // get new X position from horizontal slider
            if(id==ID_SLIDER1)
            {
                x = CUR_BRD_LEFT+SldGetPos((SLIDER*)pObj);
                if(x > CUR_BRD_RIGHT)
                    x = CUR_BRD_RIGHT;
            }
            // get new Y position from vertical slider
            if(id==ID_SLIDER3)
            {
                y = CUR_BRD_BOTTOM-SldGetPos((SLIDER*)pObj);
                if(y < CUR_BRD_TOP)
                    y = CUR_BRD_TOP;
            }
            // draw new cursor
            DrawSliderCursor(BLACK);
            return 0; // it was processed

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_PROGRESSBAR; 			// goto progress bar screen
            }
            return 1; // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_STATICTEXT; 			// goto static text screen   
            }
            return 1; // process by default

        default:
            return 1; // process by default
    }
}

// Creates progress bar screen
void CreateProgressBar()
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list

   	CreatePage("Progress bar");

	pProgressBar = PbCreate(ID_PROGRESSBAR1,// ID
              40,85,280,135,          		// dimension
              PB_DRAW,                 		// will be dislayed after creation
              25,                      		// position
              50,                      		// range
              NULL);         				// use default scheme
              
    StCreate(ID_STATICTEXT3,           		// ID 
              70,155,215,215,         		// dimension
              ST_DRAW|ST_CENTER_ALIGN,		// display text
              "How fast can \nPIC24F go", 	// text
              altScheme);                   // use alternate scheme

    pGenObj = (OBJ_HEADER*)BtnCreate(
    		  ID_BUTTON1, 					// button ID 
              220,168,260,198,          	// dimension
              10,					   		// set radius 
              BTN_DRAW|BTN_TOGGLE,   		// draw button
              NULL,                    		// no bitmap
              "?",               			// text
              altScheme); 		            // use alternate scheme

}

// Processes messages for progress bar screen
WORD MsgProgressBar(WORD objMsg, OBJ_HEADER* pObj)
{

    switch(GetObjID(pObj))
    {

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_LISTBOX; 	// goto list box screen
            }
            return 1; // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_SLIDER; 	// goto slider screen
            }
            return 1; // process by default

        default:
            return 1; // process by default
    }
}


const char pMyItemList[] = "Applications\n"
"Home Appliances\n"
"Home Automation\n"
"Industrial Controls\n"
"Medical Devices\n"
"Automotive\n"
"and much more...";

// creates list box demo screen
void CreateListBox()
{
	LISTBOX*    pLb;

    GOLFree();  // free memory for the objects in the previous linked list and start new list

	CreatePage("List box");

	pLb = LbCreate(ID_LISTBOX1,        	// ID
              40,95,250,235,            // dimension
              LB_DRAW|LB_FOCUSED,      	// will be dislayed after creation
              (char*)pMyItemList,
              altScheme);               // use alternate scheme

    SldCreate(ID_SLIDER1,              	// ID
              250,125,280,203,         	// dimension
              SLD_DRAW|SLD_SCROLLBAR|SLD_VERTICAL,   // vertical, will be dislayed after creation
              LbGetCount(pLb),       	// range
              1,                       	// page 
              LbGetCount(pLb)-1,       	// pos
              altScheme);               // use alternate scheme

    BtnCreate(ID_BUTTON1,              	// ID 
              250,95,280,125,0,        	// dimension (no radius)
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "\x1e",                  	// text
              altScheme);               // use alternate scheme

    BtnCreate(ID_BUTTON2,              	// ID 
              250,205,280,235,0,       	// dimension (no radius)
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "\x1f",                  	// text
              altScheme);               // use alternate scheme

    CbCreate(ID_CHECKBOX1,             	// ID 
              40,63,140,88,           	// dimension
              CB_DRAW,                 	// will be dislayed after creation
              "Single",                	// text
              altScheme);              	// alternative GOL scheme 

    CbCreate(ID_CHECKBOX2,             	// ID 
              170,63,270,88,          	// dimension
              CB_DRAW,                 	// will be dislayed after creation
              "Center",                	// text
              altScheme);              	// alternative GOL scheme 

    GbCreate(ID_GROUPBOX1,             	// ID 
              30,43,157,93,           	// dimension
              GB_DRAW|GB_CENTER_ALIGN,  // will be dislayed after creation
              "Selection",             	// text
              alt3Scheme);              // alternate scheme 

    GbCreate(ID_GROUPBOX2,             	// ID 
              160,43,290,93,            // dimension
              GB_DRAW|GB_CENTER_ALIGN,  // will be dislayed after creation
              "Alignment",             	// text
              alt3Scheme);              // alternate scheme 
              
              

}

// processes messages for list box demo screen
WORD MsgListBox(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg){

LISTBOX* pLb;
SLIDER*  pSld;

    pLb = (LISTBOX*)GOLFindObject(ID_LISTBOX1);
    pSld = (SLIDER*)GOLFindObject(ID_SLIDER1);

    switch(GetObjID(pObj)){

        case ID_LISTBOX1:
            // Process message by default
            LbMsgDefault(objMsg, (LISTBOX*)pObj, pMsg);
            // Set new list box position
            SldSetPos(pSld,LbGetCount(pLb)-LbGetFocusedItem(pLb)-1);
            SetState(pSld, SLD_DRAW_THUMB);
            // The message was processed
            return 0;

        case ID_SLIDER1:
            // Process message by default
            SldMsgDefault(objMsg, (SLIDER*)pObj, pMsg);
            // Set new list box position
            if(LbGetFocusedItem(pLb) != LbGetCount(pLb)-SldGetPos(pSld)){
                LbSetFocusedItem(pLb,LbGetCount(pLb)-SldGetPos(pSld));
                SetState(pLb, LB_DRAW_ITEMS);
            }
            // The message was processed
            return 0;

        case ID_BUTTON1:
            if(objMsg == BTN_MSG_RELEASED){
                LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)-1);
                SetState(pLb, LB_DRAW_ITEMS);
                SldSetPos(pSld,SldGetPos(pSld)+1);
                SetState(pSld, SLD_DRAW_THUMB);
            }
            return 1;

        case ID_BUTTON2:
            if(objMsg == BTN_MSG_RELEASED){
                LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)+1);
                SetState(pLb, LB_DRAW_ITEMS);
                SldSetPos(pSld,SldGetPos(pSld)-1);
                SetState(pSld, SLD_DRAW_THUMB);
            }
            return 1;

        case ID_CHECKBOX1:
            if(objMsg == CB_MSG_CHECKED){
                SetState(pLb, LB_SINGLE_SEL|LB_DRAW);
                LbChangeSel(pLb,pLb->pFocusItem);
            }else{
                ClrState(pLb, LB_SINGLE_SEL);
            }
            return 1;

        case ID_CHECKBOX2:
            if(objMsg == CB_MSG_CHECKED){
                SetState(pLb, LB_CENTER_ALIGN);
            }else{
                ClrState(pLb, LB_CENTER_ALIGN);
            }
            SetState(pLb, LB_DRAW);
            return 1;

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_EDITBOX;     // goto edit box screen
            }
            return 1; // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_PROGRESSBAR; // goto progress bar screen
            }
            return 1; // process by default

        default:
            return 1; // process by default
    }

}

// Creates the edit box demo screen
void CreateEditBox(){

    GOLFree();  // free memory for the objects in the previous linked list and start new list

	CreatePage("Edit box");

#define KEYSTARTX 	50
#define KEYSTARTY 	43
#define KEYSIZEX  	53
#define KEYSIZEY  	39
#define MAXCHARSIZE 20

	EbCreate(ID_EDITBOX1,              	// ID
              35,KEYSTARTY+1,GetMaxX()-35,KEYSTARTY+1*KEYSIZEY-GOL_EMBOSS_SIZE,  // dimension
              EB_DRAW|EB_CARET|EB_FOCUSED, // will be dislayed after creation
              NULL,
              MAXCHARSIZE,
              altScheme);               // default GOL scheme

    BtnCreate(ID_KEYPAD+0,             	// ID 
              KEYSTARTX+1*KEYSIZEX,		
              KEYSTARTY+4*KEYSIZEY,
              KEYSTARTX+2*KEYSIZEX,
              KEYSTARTY+5*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "0",                     	// text
              altScheme);              	// alternative GOL scheme 

    BtnCreate(ID_KEYPAD+1,             	// ID 
              KEYSTARTX+0*KEYSIZEX,
              KEYSTARTY+1*KEYSIZEY,
              KEYSTARTX+1*KEYSIZEX,
              KEYSTARTY+2*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "1",                     	// text
              altScheme);              	// alternative GOL scheme   

    BtnCreate(ID_KEYPAD+2,             	// ID 
              KEYSTARTX+1*KEYSIZEX,
              KEYSTARTY+1*KEYSIZEY,
              KEYSTARTX+2*KEYSIZEX,
              KEYSTARTY+2*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "2",                     	// text
              altScheme);              	// alternative GOL scheme 

    BtnCreate(ID_KEYPAD+3,             	// ID 
              KEYSTARTX+2*KEYSIZEX,
              KEYSTARTY+1*KEYSIZEY,
              KEYSTARTX+3*KEYSIZEX,
              KEYSTARTY+2*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "3",                     	// text
              altScheme);              	// alternative GOL scheme 

    BtnCreate(ID_KEYPAD+4,             	// ID 
              KEYSTARTX+0*KEYSIZEX,
              KEYSTARTY+2*KEYSIZEY,
              KEYSTARTX+1*KEYSIZEX,
              KEYSTARTY+3*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "4",                     	// text
              altScheme);              	// alternative GOL scheme   

    BtnCreate(ID_KEYPAD+5,             	// ID 
              KEYSTARTX+1*KEYSIZEX,
              KEYSTARTY+2*KEYSIZEY,
              KEYSTARTX+2*KEYSIZEX,
              KEYSTARTY+3*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "5",                     	// text
              altScheme);              	// alternative GOL scheme 

    BtnCreate(ID_KEYPAD+6,             	// ID 
              KEYSTARTX+2*KEYSIZEX,
              KEYSTARTY+2*KEYSIZEY,
              KEYSTARTX+3*KEYSIZEX,
              KEYSTARTY+3*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "6",                     	// text
              altScheme);              	// alternative GOL scheme 

    BtnCreate(ID_KEYPAD+7,             	// ID 
              KEYSTARTX+0*KEYSIZEX,
              KEYSTARTY+3*KEYSIZEY,
              KEYSTARTX+1*KEYSIZEX,
              KEYSTARTY+4*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "7",                     	// text
              altScheme);              	// alternative GOL scheme   

    BtnCreate(ID_KEYPAD+8,             	// ID 
              KEYSTARTX+1*KEYSIZEX,
              KEYSTARTY+3*KEYSIZEY,
              KEYSTARTX+2*KEYSIZEX,
              KEYSTARTY+4*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "8",                     	// text
              altScheme);              	// alternative GOL scheme 

    BtnCreate(ID_KEYPAD+9,             	// ID 
              KEYSTARTX+2*KEYSIZEX,
              KEYSTARTY+3*KEYSIZEY,
              KEYSTARTX+3*KEYSIZEX,
              KEYSTARTY+4*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "9",                     	// text
              altScheme);              	// alternative GOL scheme 

    BtnCreate(ID_ASTERISK,             	// ID 
              KEYSTARTX+0*KEYSIZEX,
              KEYSTARTY+4*KEYSIZEY,
              KEYSTARTX+1*KEYSIZEX,
              KEYSTARTY+5*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "*",                     	// text
              altScheme);              	// alternative GOL scheme   

    BtnCreate(ID_POUND,             	// ID 
              KEYSTARTX+2*KEYSIZEX,
              KEYSTARTY+4*KEYSIZEY,
              KEYSTARTX+3*KEYSIZEX,
              KEYSTARTY+5*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "#",                     	// text
              altScheme);              	// alternative GOL scheme   

    BtnCreate(ID_BACKSPACE,            	// ID 
              KEYSTARTX+3*KEYSIZEX,
              KEYSTARTY+1*KEYSIZEY,
              KEYSTARTX+4*KEYSIZEX,
              KEYSTARTY+2*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "\x1c",                  	// text
              altScheme);              	// alternative GOL scheme 

    BtnCreate(ID_CALL,                	// ID 
              KEYSTARTX+3*KEYSIZEX,
              KEYSTARTY+2*KEYSIZEY,
              KEYSTARTX+4*KEYSIZEX,
              KEYSTARTY+3*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              &greenphone,              // use green phone bitmap
              NULL,                     // text
              altScheme);              	// alternative GOL scheme 

    BtnCreate(ID_STOPCALL,            	// ID 
              KEYSTARTX+3*KEYSIZEX,
              KEYSTARTY+3*KEYSIZEY,
              KEYSTARTX+4*KEYSIZEX,
              KEYSTARTY+4*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              &redphone,                // use redphone bitmap
              NULL,                     // text
              altScheme);              	// alternative GOL scheme 

    BtnCreate(ID_HOLD,            		// ID 
              KEYSTARTX+3*KEYSIZEX,
              KEYSTARTY+4*KEYSIZEY,
              KEYSTARTX+4*KEYSIZEX,
              KEYSTARTY+5*KEYSIZEY,    	// dimension
              0,
              BTN_DRAW,                	// will be dislayed after creation
              NULL,                    	// no bitmap
              "Hld",                    // text
              altScheme);              	// alternative GOL scheme 

}

// Processes messages for the edit box demo screen
WORD MsgEditBox(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg){

EDITBOX* pEb;
SHORT    id;
static char status = 0;			// status to check if calling, holding or not

    id = GetObjID(pObj);

    // If number key is pressed
    if(objMsg == BTN_MSG_PRESSED)
    if(id >= ID_KEYPAD)
    if(id  < ID_KEYPAD+10){
	    if (!status) {
	        pEb = (EDITBOX*)GOLFindObject(ID_EDITBOX1);    
	        EbAddChar(pEb,'0'+id-ID_KEYPAD);
	        SetState(pEb, EB_DRAW);
	    } 
        return 1;        
    }

    switch(id){
        case ID_CALL:
            if(objMsg == BTN_MSG_PRESSED){
                pEb = (EDITBOX*)GOLFindObject(ID_EDITBOX1);    
                EbSetText(pEb, "Calling...");
                SetState(pEb, EB_DRAW);
                status = 1;
            }
            return 1;

        case ID_STOPCALL:
            if(objMsg == BTN_MSG_PRESSED){
                pEb = (EDITBOX*)GOLFindObject(ID_EDITBOX1);    
                EbSetText(pEb, "");
                SetState(pEb, EB_DRAW);
                status = 0;
            }
            return 1;

        case ID_BACKSPACE:
            if(objMsg == BTN_MSG_PRESSED){
                if (!status) {
	                pEb = (EDITBOX*)GOLFindObject(ID_EDITBOX1);    
	                EbDeleteChar(pEb);
	                SetState(pEb, EB_DRAW);
	            } 
            }
            return 1;

        case ID_HOLD:
            if(objMsg == BTN_MSG_PRESSED){
                pEb = (EDITBOX*)GOLFindObject(ID_EDITBOX1);    
	            if (status == 1) {
	                EbSetText(pEb, "Holding...");
	                status = 2;
    	        } else if (status == 2) {
	                EbSetText(pEb, "Calling...");
	                status = 1;
	            } 
   	            SetState(pEb, EB_DRAW);
            }
            return 1;

        case ID_ASTERISK:
	    	if (!status) {
	            if(objMsg == BTN_MSG_PRESSED){
	                pEb = (EDITBOX*)GOLFindObject(ID_EDITBOX1);    
	                EbAddChar(pEb,'*');
	                SetState(pEb, EB_DRAW);
	            }
	        } 
            return 1;

        case ID_POUND:
	    	if (!status) {
	            if(objMsg == BTN_MSG_PRESSED){
	                pEb = (EDITBOX*)GOLFindObject(ID_EDITBOX1);    
	                EbAddChar(pEb,'#');
	                SetState(pEb, EB_DRAW);
	            }
	        }
            return 1;

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_METER; 	// goto meter screen
            }
            return 1; // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_LISTBOX; 	// goto list box screen
            }
            return 1; // process by default

        default:
            return 1; // process by default
    }


}

#define EB1STARTX		30
#define EB2STARTX		122
#define EB3STARTX		200
#define EBSTARTY		55
#define EB4STARTX		40
#define EB5STARTX		125
#define EB6STARTX		210
#define EBSTARTY2		110

#define SELECTBTNWIDTH	22

// Items list
const char MonthItems[] =
"Jan\n" "Feb\n" "Mar\n" "Apr\n" "May\n" "Jun\n" 
"Jul\n" "Aug\n" "Sep\n" "Oct\n" "Nov\n" "Dec"; 

const char DayItems[] =
"01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" "10\n"
"11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n" "20\n"
"21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n" "30\n" "31";

const char YearItems[] =
"00\n" "01\n" "02\n" "03\n" "04\n" "05\n" "06\n" "07\n" "08\n" "09\n" "10\n"
"11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n" "20";

// Creates date and time screen
void CreateDateTime() {
OBJ_HEADER* obj;
WORD		textWidth, textHeight;

    GOLFree();   // free memory for the objects in the previous linked list and start new list
	
    WndCreate(ID_WINDOW1,       		// ID
              0,0,GetMaxX(),GetMaxY(), 	// dimension
              WND_DRAW,                	// will be dislayed after creation
              &mchpIcon,               	// icon
              "Set time and date",		// set text 
              NULL);                   	// default GOL scheme 

	RTCCProcessEvents();				// update the global time and date strings
	
	/* ************************************* */
	/* Create Month, Day and Year Edit Boxes */ 
	/* ************************************* */
	textWidth  = GetTextWidth("MMM",altScheme->pFont);
	textHeight = GetTextHeight(altScheme->pFont) + GOL_EMBOSS_SIZE;

	/* *************************** */
	// create month components
	/* *************************** */

	EbCreate(ID_EB_MONTH,              	// ID
              EB1STARTX,
              EBSTARTY,
              EB1STARTX+textWidth, 
              EBSTARTY+textHeight,
              EB_DRAW, 					// will be dislayed after creation
              &_date_str[0],			// use the month string value
              3,						// max characters is 3
              altScheme);               // default GOL scheme

    obj = (OBJ_HEADER*)BtnCreate(
    		  ID_BUTTON_MO,         	// button ID 
              EB1STARTX+textWidth+1,
              EBSTARTY+1,
              EB1STARTX+textWidth+SELECTBTNWIDTH+1,
              EBSTARTY+textHeight-1,  		   	
              0,					   	// draw rectangular button	
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "\x1f",                  	// DOWN arrow as text
              altScheme);             	// use navigation scheme
              
	/* *************************** */
	// create day components
	/* *************************** */
	textWidth  = GetTextWidth("MM",altScheme->pFont);

	EbCreate(ID_EB_DAY,              	// ID
              EB2STARTX,
              EBSTARTY,
              EB2STARTX+textWidth,
              EBSTARTY+textHeight,
              EB_DRAW, 					// will be dislayed after creation
              &_date_str[4],			// use the day string value
              2,						// max characters is 3
              altScheme);               // default GOL scheme

    BtnCreate(ID_BUTTON_DY,         	// button ID 
              EB2STARTX+textWidth+1,
              EBSTARTY+1,
              EB2STARTX+textWidth+SELECTBTNWIDTH+1,
              EBSTARTY+textHeight-1,  		   	
              0,					   	// draw rectangular button	
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "\x1f",                  	// DOWN arrow as text
              altScheme);             	// use navigation scheme


	/* *************************** */
	// create year components
	/* *************************** */
	textWidth  = GetTextWidth("MMMM",altScheme->pFont);

	EbCreate(ID_EB_YEAR,              	// ID
              EB3STARTX,
              EBSTARTY,
              EB3STARTX+textWidth,
              EBSTARTY+textHeight,
              EB_DRAW, 					// will be dislayed after creation
              &_date_str[8],			// use the year string value
              4,						// max characters is 4
              altScheme);               // default GOL scheme

    BtnCreate(ID_BUTTON_YR,         	// button ID 
              EB3STARTX+textWidth+1,
              EBSTARTY+1,
              EB3STARTX+textWidth+SELECTBTNWIDTH+1,
              EBSTARTY+textHeight-1,  		   	
              0,					   	// draw rectangular button	
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "\x1f",                  	// DOWN arrow as text
              altScheme);             	// use navigation scheme

	/* ****************************************** */
	/* Create Hour, Minute and Seconds Edit Boxes */ 
	/* ****************************************** */

	RTCCProcessEvents();				// update the time and date

	textWidth  = GetTextWidth("MM",altScheme->pFont)+(GOL_EMBOSS_SIZE<<1);
	textHeight = (GetTextHeight(altScheme->pFont)<<1) - 10;

	/* *************************** */
	// create hour components
	/* *************************** */

	EbCreate(ID_EB_HOUR,              	// ID
              EB4STARTX,
              EBSTARTY2,
              EB4STARTX+textWidth, 
              EBSTARTY2+textHeight,
              EB_DRAW, 					// will be dislayed after creation
              &_time_str[4],			// use hour string value
              2,						// max characters is 3
              altScheme);               // default GOL scheme

    BtnCreate(ID_BUTTONHR_P,         	// button ID 
              EB4STARTX+textWidth+1,
              EBSTARTY2+1,
              EB4STARTX+textWidth+SELECTBTNWIDTH+1,
              EBSTARTY2+(textHeight>>1),  		   	
              0,					   	// draw rectangular button	
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "+",                  	// DOWN arrow as text
              altScheme);             	// use navigation scheme

    BtnCreate(ID_BUTTONHR_M,         	// button ID 
              EB4STARTX+textWidth+1,
              EBSTARTY2+(textHeight>>1),
              EB4STARTX+textWidth+SELECTBTNWIDTH+1,
              EBSTARTY2+textHeight-1,  		   	
              0,					   	// draw rectangular button	
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "-",                  	// DOWN arrow as text
              altScheme);             	// use navigation scheme

	/* *************************** */
	// create minute components
	/* *************************** */

	EbCreate(ID_EB_MINUTE,              // ID
              EB5STARTX,
              EBSTARTY2,
              EB5STARTX+textWidth, 
              EBSTARTY2+textHeight,
              EB_DRAW, 					// will be dislayed after creation
              &_time_str[7],			// use minute string value
              2,						// max characters is 3
              altScheme);               // default GOL scheme

    BtnCreate(ID_BUTTONMN_P,         	// button ID 
              EB5STARTX+textWidth+1,
              EBSTARTY2+1,
              EB5STARTX+textWidth+SELECTBTNWIDTH+1,
              EBSTARTY2+(textHeight>>1),  		   	
              0,					   	// draw rectangular button	
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "+",                  	// DOWN arrow as text
              altScheme);             	// use navigation scheme

    BtnCreate(ID_BUTTONMN_M,         	// button ID 
              EB5STARTX+textWidth+1,
              EBSTARTY2+(textHeight>>1),
              EB5STARTX+textWidth+SELECTBTNWIDTH+1,
              EBSTARTY2+textHeight-1,  		   	
              0,					   	// draw rectangular button	
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "-",                  	// DOWN arrow as text
              altScheme);             	// use navigation scheme

	/* *************************** */
	// create seconds components
	/* *************************** */

	EbCreate(ID_EB_SECOND,              // ID
              EB6STARTX,
              EBSTARTY2,
              EB6STARTX+textWidth, 
              EBSTARTY2+textHeight,
              EB_DRAW, 					// will be dislayed after creation
              &_time_str[10],			// use seconds string value
              2,						// max characters is 3
              altScheme);               // default GOL scheme

    BtnCreate(ID_BUTTONSC_P,         	// button ID 
              EB6STARTX+textWidth+1,
              EBSTARTY2+1,
              EB6STARTX+textWidth+SELECTBTNWIDTH+1,
              EBSTARTY2+(textHeight>>1),  		   	
              0,					   	// draw rectangular button	
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "+",                  	// DOWN arrow as text
              altScheme);             	// use navigation scheme

    BtnCreate(ID_BUTTONSC_M,         	// button ID 
              EB6STARTX+textWidth+1,
              EBSTARTY2+(textHeight>>1),
              EB6STARTX+textWidth+SELECTBTNWIDTH+1,
              EBSTARTY2+textHeight-1,  		   	
              0,					   	// draw rectangular button	
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "-",                  	// DOWN arrow as text
              altScheme);             	// use navigation scheme

	/* *************************** */
	// create exit button 
	/* *************************** */
              
    BtnCreate(ID_BUTTON4,         		// button ID 
              EB6STARTX,
              185,
              EB6STARTX+textWidth+SELECTBTNWIDTH,
              215,  		   	
              0,					   	// draw rectangular button	
              BTN_DRAW,               	// will be dislayed after creation
              NULL,					   	// no bitmap	
              "exit",                   // set text
              altScheme);             	// use navigation scheme

    GOLSetFocus(obj); // set focus for the button
}

void updateDateTimeEb() 
{
	OBJ_HEADER* obj;

	RTCCProcessEvents();				// update the time and date
	
	//printf("%s:%s():%d\n", __FILE__, __FUNCTION__, __LINE__ );
	// update time edit boxes
	obj = GOLFindObject(ID_EB_HOUR);
	EbSetText((EDITBOX*)obj, &_time_str[4]);
	SetState(obj, EB_DRAW);
	obj = GOLFindObject(ID_EB_MINUTE);
	EbSetText((EDITBOX*)obj, &_time_str[7]);
	SetState(obj, EB_DRAW);
	obj = GOLFindObject(ID_EB_SECOND);
	EbSetText((EDITBOX*)obj, &_time_str[10]);
	SetState(obj, EB_DRAW);

	// update date edit boxes
	obj = GOLFindObject(ID_EB_MONTH);
	EbSetText((EDITBOX*)obj, &_date_str[0]);
	SetState(obj, EB_DRAW);
	obj = GOLFindObject(ID_EB_DAY);
	EbSetText((EDITBOX*)obj, &_date_str[4]);
	SetState(obj, EB_DRAW);
	obj = GOLFindObject(ID_EB_YEAR);
	EbSetText((EDITBOX*)obj, &_date_str[8]);
	SetState(obj, EB_DRAW);
	
	//printf("%s:%s():%d\n", __FILE__, __FUNCTION__, __LINE__ );
}

/* this will show the pull down menus */
void ShowPullDownMenu() 
{	
	SHORT left=0, top, right=0, bottom;
	WORD textWidth=0, textHeight;
	LISTBOX*    pLb;
	char *	pItems = NULL;

	pListSaved = GOLGetList();
	GOLNewList();

	textHeight = GetTextHeight(altScheme->pFont);
	bottom = 230;
	top    = EBSTARTY;

	switch (DateItemID) {
		case ID_EB_MONTH:
			textWidth  = GetTextWidth("MMM",altScheme->pFont)+GOL_EMBOSS_SIZE;
			left   = EB1STARTX;
			right  = EB1STARTX+textWidth;
			pItems = (char*)MonthItems;
			break;
		case ID_EB_DAY:
			textWidth  = GetTextWidth("MM",altScheme->pFont)+GOL_EMBOSS_SIZE;
			left   = EB2STARTX;
			right  = EB2STARTX+textWidth;
			pItems = (char*)DayItems;
			break;
		case ID_EB_YEAR:
			textWidth  = GetTextWidth("MMMM",altScheme->pFont)+GOL_EMBOSS_SIZE;
			left   = EB3STARTX;
			right  = EB3STARTX+textWidth;
			pItems = (char*)YearItems;
			break;
	}

	pLb = (LISTBOX *)LbCreate(
			   ID_LISTBOX1,         	// List Box ID
		       left,
		       //top+textHeight,
		       top,
		       right,
		       bottom, 					// dimensions
		       LB_DRAW|LB_SINGLE_SEL, 	// draw after creation, single selection
		       pItems,        			// items list 
		       altScheme);  			// scheme
		
	SldCreate(ID_SLIDER1,              	// Slider ID
		      left+textWidth,
		      //top+(textHeight<<1),  		   	
		      top+textHeight,  		   	
		      right+SELECTBTNWIDTH,
		      bottom-textHeight,
		      SLD_DRAW|SLD_SCROLLBAR|
		      SLD_VERTICAL,   			// vertical, draw after creation
		      LbGetCount(pLb),       	// range
		      5,                       	// page 
		      LbGetCount(pLb)-1,       	// pos
		      altScheme);               // use alternate scheme
		
	BtnCreate(ID_BUTTON_DATE_UP,       	// up button ID 
			  left+textWidth,
		      top,
		      right+SELECTBTNWIDTH,
		      top+textHeight,0, 		   	
		      BTN_DRAW,                 // draw after creation
		      NULL,                    	// no bitmap
		      "\x1e",                  	// text
		      altScheme);               // use alternate scheme
		
	BtnCreate(ID_BUTTON_DATE_DN,        // down button ID 
			  left+textWidth,
		      bottom-textHeight,
		      right+SELECTBTNWIDTH,
		      bottom,0,  		   	
		      BTN_DRAW,                 // draw after creation
		      NULL,                    	// no bitmap
		      "\x1f",                  	// text
		      altScheme);               // use alternate scheme	

	// This object is a ghost object to enable to get messages that touch is outside 
	// the list box. It will never be displayed.
    GbCreate(ID_GROUPBOX2,             	// ID 
              0,0,GetMaxX(),GetMaxY(), 	// cover the whole screen
              0,                 		// do not display
              NULL,             		// no text
              NULL);                   	
		      
}  

WORD RemovePullDownMenu() {
OBJ_HEADER* pObjTemp;
LISTBOX*   	pLb;
LISTITEM*	pItemSel;
SHORT    	left, top, right, bottom;
char 		dateDisplay[] = "20xx\n";
char*		pTemp;

    pLb = (LISTBOX*)GOLFindObject(ID_LISTBOX1);	// find the list box (drop doen menu) pointer
	pItemSel = LbGetSel(pLb,pLb->pItemList); 	// get the selected item
    
    left = pLb->left;							// get dimension of area to be redrawn
    top = pLb->top;
    right = pLb->right+SELECTBTNWIDTH;
    bottom = pLb->bottom;

	// check if objects has been hidden
	pObjTemp = GOLGetList();
	while(pObjTemp) {							// parse the active list
		if (GetState(pObjTemp, HIDE))			// an object is still not hidden 
			return 0;							// go back without changing state
		pObjTemp = pObjTemp->pNxtObj;			// go to next object
	}

	// all objects are hidden we can now remove the list	
	GOLFree();   								// remove the list of pull down menu
    GOLSetList(pListSaved);						// set active list back to saved list

	if (pItemSel != NULL) {						// process only when an item was selected
		UpdateDateEb(pItemSel->pText, pLb);		// update the RTCC values 
	
		pObjTemp = GOLFindObject(DateItemID);	// find the date item to be updated
		if (pItemSel != NULL) {
			if (DateItemID == ID_EB_YEAR) {		// year is special case: we only change last two digits
				pTemp = pItemSel->pText;
				dateDisplay[2] = *pTemp++;
				dateDisplay[3] = *pTemp;
				EbSetText((EDITBOX*)pObjTemp, dateDisplay);
			}
			else 
				EbSetText((EDITBOX*)pObjTemp, pItemSel->pText);
		}
	}

    GOLRedrawRec(left, top, right, bottom);		// redraw objects that were overlapped by pulldown menu
	screenState = DISPLAY_DATETIME;      		// set state back to main state of date and time settings 
	return 1;									// go back normally 

}	

/* this selects the new month or day or year from the list box*/
WORD MsgSetDate(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg) {

LISTBOX*   	pLb;
SLIDER*  	pSld;
OBJ_HEADER* pObjHide;

    pLb = (LISTBOX*)GOLFindObject(ID_LISTBOX1);		
    pSld = (SLIDER*)GOLFindObject(ID_SLIDER1);

    switch(GetObjID(pObj)){

		case ID_GROUPBOX2:
            if(objMsg == GB_MSG_SELECTED){		// return button was pressed and released
	            pObjHide = GOLGetList();  		// hide all the objects in the current list
	            while (pObjHide) {				// set all objects with HIDE state
		            
		            // do not hide this since this is not shown anyway
					if (GetObjID(pObjHide) != ID_GROUPBOX2) 
	            		SetState(pObjHide, HIDE);
	            	pObjHide = pObjHide->pNxtObj;
	            }
		        screenState = HIDE_DATE_PDMENU;	// go to hide state
            }
            return 0;
		
        case ID_LISTBOX1:
        	if (pMsg->uiEvent == EVENT_MOVE) {
	        	pMsg->uiEvent = EVENT_PRESS;	// change event for listbox
	            // Process message by default
	            LbMsgDefault(objMsg, (LISTBOX*)pObj, pMsg);
	            // Set new list box position
	            SldSetPos(pSld,LbGetCount(pLb)-LbGetFocusedItem(pLb)-1);
	            SetState(pSld, SLD_DRAW_THUMB);
	        	pMsg->uiEvent = EVENT_MOVE;		// change event for listbox
	        }
	        // The message was processed
	        return 0;

        case ID_SLIDER1:
            // Process message by default
            SldMsgDefault(objMsg, (SLIDER*)pObj, pMsg);
            // Set new list box position
            if(LbGetFocusedItem(pLb) != LbGetCount(pLb)-SldGetPos(pSld)){
                LbSetFocusedItem(pLb,LbGetCount(pLb)-SldGetPos(pSld));
                SetState(pLb, LB_DRAW_ITEMS);
            }
            // The message was processed
            return 0;

        case ID_BUTTON_DATE_UP:					// slider button up was pressed
            if(objMsg == BTN_MSG_RELEASED){
                LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)-1);
                SetState(pLb, LB_DRAW_ITEMS);
                SldSetPos(pSld,SldGetPos(pSld)+1);
                SetState(pSld, SLD_DRAW_THUMB);
            }
            return 1;

        case ID_BUTTON_DATE_DN:					// slider button down was pressed
            if(objMsg == BTN_MSG_RELEASED){
                LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)+1);
                SetState(pLb, LB_DRAW_ITEMS);
                SldSetPos(pSld,SldGetPos(pSld)-1);
                SetState(pSld, SLD_DRAW_THUMB);
            }
            return 1;
    }    
    return 1;    
}	
			

// Processes messages for date and time screen
WORD MsgDateTime(WORD objMsg, OBJ_HEADER* pObj) {
	
	EDITBOX *pEbObj;
	char   hour = 0, minute = 0, second = 0;
	WORD   btnID; 
	WORD   ebID = 0, i;

    switch(btnID = GetObjID(pObj)) {
	    
        case ID_BUTTON4:							// exit setting of date and time
            if(objMsg == BTN_MSG_RELEASED){
                screenState = prevState; 			// goto last state screen
                return 1;
            }
			break;

        case ID_BUTTON_MO:							// month is selected show the pull down menu
            if(objMsg == BTN_MSG_PRESSED){
	            DateItemID = ID_EB_MONTH;
	            ShowPullDownMenu();
                screenState = DISPLAY_SETDATE;  	// change state 
                return 0;
            }
			break;

        case ID_BUTTON_DY:							// month is selected show the pull down menu
            if(objMsg == BTN_MSG_PRESSED){
	            DateItemID = ID_EB_DAY;
	            ShowPullDownMenu();
                screenState = DISPLAY_SETDATE;  	// change state 
                return 0;
            }
			break;

        case ID_BUTTON_YR:							// month is selected show the pull down menu
            if(objMsg == BTN_MSG_PRESSED){
	            DateItemID = ID_EB_YEAR;
	            ShowPullDownMenu();
                screenState = DISPLAY_SETDATE;  	// change state 
                return 0;
            }
			break;
			
		case ID_BUTTONHR_P:  						// increment hour value
		    if(objMsg == BTN_MSG_PRESSED){
				hour = +1;
				ebID = ID_EB_HOUR;
				i = 4;								
			}
			break;
        case ID_BUTTONHR_M:							// decrement hour value
			if(objMsg == BTN_MSG_PRESSED){
				hour = -1;
				ebID = ID_EB_HOUR;
				i = 4;
			}
			break;
        case ID_BUTTONMN_P:  						// increment minute value
        	if(objMsg == BTN_MSG_PRESSED){
				minute = +1;
				ebID = ID_EB_MINUTE;
				i = 7;
			}
			break;
        case ID_BUTTONMN_M:							// decrement minute value
        	if(objMsg == BTN_MSG_PRESSED){
				minute = -1;
				ebID = ID_EB_MINUTE;
				i = 7;
			}
			break;
        case ID_BUTTONSC_P:  						// increment seconds value
        	if(objMsg == BTN_MSG_PRESSED){
				second = +1;
				ebID = ID_EB_SECOND;
				i = 10;
			}
			break;				
        case ID_BUTTONSC_M:							// decrement seconds value
        	if(objMsg == BTN_MSG_PRESSED){
				second = -1;
				ebID = ID_EB_SECOND;
				i = 10;
			} 
            break; 
            
        default:
            break; 
    }
	
	if (ebID) 
	{										// check if time was modified
		//mRTCCOff();
    	RTCCSetBinHour(mRTCCGetBinHour() + hour);	// update RTCC registers
    	RTCCSetBinMin(mRTCCGetBinMin() + minute);
    	RTCCSetBinSec(mRTCCGetBinSec() + second);
		mRTCCSet();									// copy the new values to the RTCC registers

		// update the edit box that was changed. This is done to immediately update
		// the values on the edit box. The normal update by updateDateTimeEb() is done
		// for every second. This may be too long to wait when the increment or decrement 
		// was successfully performed.
		RTCCProcessEvents();					// update string values
		pEbObj = (EDITBOX*)GOLFindObject(ebID);	// find the pointer to the updated edit box
		EbSetText(pEbObj, &_time_str[i]);		// set the new value (string)
		SetState(pEbObj, EB_DRAW);				// redraw the edit box that was updated.
    }	
		
    return 1;
}

void UpdateDateEb(char *pText, LISTBOX* pLb) {
	char value;
	char ch;

	switch (DateItemID) {

		default :
		case ID_EB_MONTH:
			value = LbGetFocusedItem(pLb)+1;	// get the selected month item +1 is needed since 
												// RTCC stores months from index 1 as january
			RTCCSetBinMonth(value);				// set the month value in RTCC module
   	        RTCCCalculateWeekDay();				// calculate the weekday from this new value
			break;

        case ID_EB_YEAR:
		case ID_EB_DAY:
			ch = *pText++;						// get the first character
			value = (ch-48) * 10;				// get the value of the first character
			ch = *pText;						// get the next character
			value = value + (ch-48);			// add the value of the second character to the first one
			if (DateItemID == ID_EB_DAY)				
				RTCCSetBinDay(value);			// set the day value in the RTCC module
			else {
				RTCCSetBinYear(value);			// set the day value in the RTCC module
			}
            RTCCCalculateWeekDay();				// calculate the weekday from this new value
            break;
	}
	mRTCCSet();									// copy the new values to the RTCC registers	
}	

#define MAXMETER1VALUE 50
#define MAXMETER2VALUE 30

// Creates meter screen
void CreateMeter() {

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	CreatePage("Meter");

	MtrCreate(ID_METER1, 
			  30, 50, 150, 180, 	   	// set dimension
			  MTR_DRAW|MTR_RING,	   	// draw object after creation
			  0,				   		// set initial value
			  MAXMETER1VALUE,			// set range
			  "METER1", 		   	   	// Text Label
			  meterScheme);            	// alternative GOL scheme                     

	MtrCreate(ID_METER2, 
			  160, 50, 280, 180, 	   	// set dimension
			  MTR_DRAW,				   	// draw object after creation
			  0,				   		// set initial value
			  MAXMETER2VALUE, 			// set range
			  "METER2", 		   	   	// Text Label
			  meterScheme);            	// alternative GOL scheme                     

    BtnCreate(ID_BUTTON1,         		// button ID 
              50,190,					// left, top corner	
              130,230, 		   			// right, bottom corner
              0,					   	// draw rectangular button	
              BTN_DRAW|BTN_TOGGLE,		// will be dislayed after creation
              NULL,					   	// no bitmap	
              "dclrt",                 	// set text
              altScheme);          	   	// use alternate scheme

    BtnCreate(ID_BUTTON2,         		// button ID 
              180,190,					// left, top corner	
              260,230, 		   			// right, bottom corner
              0,					   	// draw rectangular button	
              BTN_DRAW|BTN_TOGGLE,		// will be dislayed after creation
              NULL,					   	// no bitmap	
              "dclrt",                	// set text
              altScheme);          	   	// use alternate scheme
}

// Processes messages for meter screen
WORD MsgMeter(WORD objMsg, OBJ_HEADER* pObj) {
	
    switch(GetObjID(pObj)){

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_DIAL; 			// goto dial screen
            }
            return 1; 								// process by default

        case ID_BUTTON1:
        case ID_BUTTON2:
            if(objMsg == BTN_MSG_PRESSED){
                BtnSetText((BUTTON*)pObj, "accel");
            }
            if(objMsg == BTN_MSG_RELEASED){
                BtnSetText((BUTTON*)pObj, "dclrt");
            }
            return 1; 								// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_EDITBOX; 		// goto edit box screen
            }
            return 1; 								// process by default
            
        default:
            return 1; 								// process by default
    }
}

// this updates the meters on the screen
void UpdateMeter() {
	
	OBJ_HEADER 	*pObj; 
	METER 		*pMtrObj;
	WORD		i, value;

	/* Meter 1 is controlled by ID_BUTTON1
  	   Meter 2 is controlled by ID_BUTTON2
	   Both meters goes from 0 to maximum when their respective 
	   buttons are pressed.	Both button have a toggle property. So
	   when pressed it will remain to be pressed until the meter has reached
	   the maximum value. The only difference is that ID_BUTTON1 is controlled
	   by the code while ID_BUTTON2 is manually controlled.
	*/
	for (i = ID_BUTTON1; i < ID_BUTTON2+1; i++) {
	   	pObj = GOLFindObject(i);

		if (i == ID_BUTTON1) {
       		pMtrObj = (METER*)GOLFindObject(ID_METER1);		// get meter 1 pointer
			value = MtrGetVal(pMtrObj);						// get current value
			if (value == 0) {								// when zero turn on ID_BUTTON1
				BtnSetText((BUTTON*)pObj, "accel");		
       			SetState(pObj,BTN_PRESSED|BTN_DRAW); 
			}
			if (value == MAXMETER1VALUE) {					// when maximum turn off ID_BUTTON1
				BtnSetText((BUTTON*)pObj, "dclrt");
				ClrState(pObj,BTN_PRESSED);
       			SetState(pObj,BTN_DRAW); 
			}
			if (GetState(pObj, BTN_PRESSED))				// increment meter value when button 
   				MtrSetVal(pMtrObj, value+1); 				// is pressed
   			else	
   				MtrSetVal(pMtrObj, value-1); 				// decrement meter value when button
   			SetState(pMtrObj,MTR_DRAW_UPDATE); 				// is released
       		
       	} else {
       		pMtrObj = (METER*)GOLFindObject(ID_METER2);		// get meter 2 pointer

			value = MtrGetVal(pMtrObj);						// get meter current value
			if (GetState(pObj,BTN_PRESSED)) { 				// check if ID_BUTTON2 is pressed
	       		MtrSetVal(pMtrObj, value+1); 				// increment value of meter
	       		SetState(pMtrObj,MTR_DRAW_UPDATE); 			// redraw meter
			} else {
				if (value) {								// if ID_BUTTON2 is released
	       			MtrSetVal(pMtrObj, value-1); 			// decrement meter if not yet zero
	       			SetState(pMtrObj,MTR_DRAW_UPDATE); 		// redraw meter
	       		}
			}
	
			if (value == MAXMETER2VALUE) {					// check if meter 2 reached max value
				ClrState(pObj, BTN_PRESSED);				// release ID_BUTTON2 when max is reached
				BtnSetText((BUTTON*)pObj, "dclrt");			// set text of ID_BUTTON2 to "dclrt"
				SetState(pObj, BTN_DRAW);					// redraw button
			}
       	}
	}
}	
	
// Creates dial screen
void CreateDial() 
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list
#if 0
    TRISA = 0xFF80;		// set IO pins (0:6) to output mode, 
    					// pin 7 is used as side button S5 switch for press and release
	LATA  = 0x0001;		// light only one LED
#endif
	CreatePage("Dial");

	RdiaCreate(ID_ROUNDDIAL,           	// ID
              95,135,55,				// dimensions
              RDIA_DRAW, 				// draw after creation
              1,						// resolution
              400,						// initial value
              700, 						// maximum value
              altScheme);              	// use alternative scheme
              
	MtrCreate(ID_METER1, 
			  160, 70, 280, 200, 	   	// set dimension
			  MTR_DRAW, 			   	// draw object after creation
			  400,				   		// set initial value
			  700, 						// set range
			  "VALUE", 		   		   	// Text Label
			  meterScheme);            	// alternative GOL scheme                     
}

// Processes messages for dial screen
WORD MsgDial(WORD objMsg, OBJ_HEADER* pObj) 
{

	OBJ_HEADER *pMtrObj;
	SHORT dialVal;
	
    switch(GetObjID(pObj))
    {
            
		case ID_ROUNDDIAL:
        	dialVal = RdiaGetVal((ROUNDDIAL*)pObj);
        	pMtrObj = GOLFindObject(ID_METER1);
        	MtrSetVal((METER*)pMtrObj, dialVal); 
        	SetState(pMtrObj,MTR_DRAW_UPDATE); 
        	if (objMsg == RD_MSG_CLOCKWISE)
        		if (dialVal == 700)
        			return 0;
        	if (objMsg == RD_MSG_CTR_CLOCKWISE)
        		if (dialVal == 0)
        			return 0;

       	    dialVal = (dialVal-1)%70;				// -1 is used to avoid 70 which is also 0 after mod.
#if 0       	          
        	if (dialVal <= 10)
        		LATA  = 0x0001;						// light LED 0
        	else if (dialVal <= 20)
        		LATA  = 0x0002;						// light LED 1
        	else if (dialVal <= 30)
        		LATA  = 0x0004;						// light LED 2
        	else if (dialVal <= 40)
        		LATA  = 0x0008;						// light LED 3
        	else if (dialVal <= 50)
        		LATA  = 0x0010;						// light LED 4
        	else if (dialVal <= 60)
        		LATA  = 0x0020;						// light LED 5
        	else if (dialVal <= 70)
        		LATA  = 0x0040;						// light LED 6
#endif	
            break; 									// process by default

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
#if 0            	
		    	LATA  = 0xFF00;						// turn off all LED
		    	TRISA = 0xFFFF;						// set IO pin to input mode
#endif		    	
                screenState = CREATE_PICTURE; 		// goto picture control screen
            }
            break; 									// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
#if 0            	
		    	LATA  = 0xFF00;						// turn off all LED
		    	TRISA = 0xFFFF;						// set IO pin to input mode
#endif		    	
                screenState = CREATE_METER; 		// goto meter screen
            }
            break; 									// process by default

        default:
            break; // process by default
    }
    return 1;
}

void CreatePicture()
{ 
	GOLFree();   // free memory for the objects in the previous linked list and start new list

	CreatePage("Picture control");

    pPicture = PictCreate(ID_PICTURE1,	// ID 
              70,50,225,170,          	// dimension
              PICT_DRAW|PICT_FRAME,    	// will be dislayed, has frame
              1,                       	// scale factor is x1
              &Engine1,               	// bitmap
              altScheme);               // default GOL scheme 

    pSlider = SldCreate(ID_SLIDER1,    	// ID
              230,50,260,170,         	// dimension
              SLD_DRAW|SLD_DISABLED| \
              SLD_VERTICAL, 			// draw with disabled state 
              50,   				   	// range
              1,                       	// page 
              25, 					   	// pos
              altScheme);              	// alternative GOL scheme 

    CbCreate(ID_CHECKBOX1,             	// ID 
              50,180,150,220,          	// dimension
              CB_DRAW,                 	// will be dislayed after creation
              "Scale",                 	// text
              altScheme);              	// alternative GOL scheme 

    CbCreate(ID_CHECKBOX2,             	// ID 
              160,180,290,220,          // dimension
              CB_DRAW,                 	// will be dislayed after creation
              "Animate",               	// text
              altScheme);              	// alternative GOL scheme 

}

WORD MsgPicture(WORD objMsg, OBJ_HEADER* pObj)
{
    switch(GetObjID(pObj))
    {
        case ID_CHECKBOX1:
            if(objMsg == CB_MSG_CHECKED)
            { 	// set new scale factor
                PictSetScale(pPicture,2); 	// x2
            }
            else
            {
                PictSetScale(pPicture,1); 	// x1
            }
            SetState(pPicture,PICT_DRAW); 	// set redraw state
            return 1; 						// process by default

        case ID_CHECKBOX2:
            if(objMsg == CB_MSG_CHECKED)
            { 	// switch on/off animation
	            ClrState(pSlider, SLD_DISABLED); // enable slider 
	            SetState(pSlider, SLD_DRAW);// redraw slider to show new state 	
                animate = 1;
            }
            else
            {
	            SetState(pSlider, SLD_DISABLED | SLD_DRAW); // disable the slider and redraw
                animate = 0;                
            }
            return 1; 						// process by default

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_CUSTOMCONTROL;	// goto cutom control screen
                animate = 0; 				// switch off animation
            }
            return 1; 						// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_DIAL; 	// goto dial screen
                animate = 0; 				// switch off animation
            }
            return 1; 						// process by default

        default:
            return 1; 						// process by default
    }
}

// Creates custom control screen
void CreateCustomControl()
{
    GOLFree();  // free memory for the objects in the previous linked list and start new list

	CreatePage("Custom control");

	CcCreate(ID_CUSTOM1,               	// ID
              100,50,220,170,          	// dimension
              CC_DRAW,                 	// will be dislayed after creation
              alt3Scheme);         		// use alternate 3 scheme

    RbCreate(ID_RADIOBUTTON1,    	   	// ID 
              50,180,150,200,           // dimension
              RB_DRAW|RB_GROUP,        	// will be dislayed and focused after creation
                                       	// first button in the group
              "Heavy Load",             // text
              alt2Scheme);              	// alternative GOL scheme 

    RbCreate(ID_RADIOBUTTON2,           // ID 
              50,205,150,225,          	// dimension
              RB_DRAW|RB_CHECKED,      	// will be dislayed and checked after creation
              "Normal Load",            // text
              alt2Scheme);              // alternative GOL scheme 

    RbCreate(ID_RADIOBUTTON3,          	// ID 
              170,180,270,200,          // dimension
              RB_DRAW,                 	// will be dislayed after creation
              "Light Load",             // text
              alt2Scheme);              // alternative GOL scheme 

}

// Processes messages for custom control screen
WORD MsgCustomControl(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
	RADIOBUTTON *pRb;
	CUSTOM      *pCc;

    switch(GetObjID(pObj)){

        case ID_RADIOBUTTON1: 								// set load to high
            pCc = (CUSTOM*) GOLFindObject(ID_CUSTOM1); 		// get pointer to custom control object
        	CcSetPos(pCc, 75); 
            SetState(pCc, CC_DRAW_BAR);                    	// set to redraw the level
            return 1; 										// process by default

        case ID_RADIOBUTTON2: 								// set load to normal
            pCc = (CUSTOM*) GOLFindObject(ID_CUSTOM1); 		// get pointer to custom control object
        	CcSetPos(pCc, 120); 
            SetState(pCc, CC_DRAW_BAR);                    	// set to redraw the level
            return 1; 										// process by default

        case ID_RADIOBUTTON3: 								// set load to light
            pCc = (CUSTOM*) GOLFindObject(ID_CUSTOM1); 		// get pointer to custom control object
        	CcSetPos(pCc, 150); 
            SetState(pCc, CC_DRAW_BAR);                    	// set to redraw the level
            return 1; 										// process by default

        case ID_CUSTOM1: 									// level is changed
            if(objMsg == CC_MSG_SELECTED){
                if (pMsg->param2 <= 110) {
		            pRb = (RADIOBUTTON*) GOLFindObject(ID_RADIOBUTTON1); // get pointer heavy load setting
	                RbSetCheck(pRb, ID_RADIOBUTTON1);		// set radio button for heavy to be checked
	                SetState(pRb, RB_DRAW_CHECK);
	            } else if (pMsg->param2 <= 140) {
		            pRb = (RADIOBUTTON*) GOLFindObject(ID_RADIOBUTTON2); // get pointer heavy load setting
	                RbSetCheck(pRb, ID_RADIOBUTTON2);		// set radio button for normal to be checked
	                SetState(pRb, RB_DRAW_CHECK);
	            } else {
		            pRb = (RADIOBUTTON*) GOLFindObject(ID_RADIOBUTTON3); // get pointer heavy load setting
	                RbSetCheck(pRb, ID_RADIOBUTTON3);		// set radio button for light to be checked
	                SetState(pRb, RB_DRAW_CHECK);
	            } 
	         } 
	         return 1;   

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_SIGNATURE; 			// goto signature screen
            }
            return 1; // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_PICTURE; 				// goto picture screen
            }
            return 1; // process by default

        default:
            return 1; // process by default
    }

}

// dimensions for signature box
#define SIG_PANEL_LEFT   30
#define SIG_PANEL_RIGHT  290
#define SIG_PANEL_TOP    50
#define SIG_PANEL_BOTTOM 220

// Creates signature screen
void CreateSignature()
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	CreatePage("Signature");
}


// Draws box for signature
WORD PanelSignature()
{
#define SIG_STATE_SET   0
#define SIG_STATE_DRAW  1

static BYTE state = SIG_STATE_SET;

    if(state == SIG_STATE_SET){ // set data for panel drawing
        GOLPanelDraw(SIG_PANEL_LEFT,SIG_PANEL_TOP,SIG_PANEL_RIGHT,SIG_PANEL_BOTTOM,0,
            WHITE,
            altScheme->EmbossLtColor,
            altScheme->EmbossDkColor,
            NULL, GOL_EMBOSS_SIZE);

        state = SIG_STATE_DRAW; // change state
    }

    if(!GOLPanelDrawTsk()){  
       return 0; // drawing is not completed
    }else{
       state = SIG_STATE_SET; // set state to initial
       return 1; // drawing is done
    }            
}

// Processes messages for signature screen
WORD MsgSignature(WORD objMsg, OBJ_HEADER* pObj,  GOL_MSG* pMsg)
{
static SHORT prevX=-1; // previous point
static SHORT prevY=-1;
SHORT x,y;             // current point

    switch(GetObjID(pObj)){

        case ID_WINDOW1:
            // get touched position
            x = pMsg->param1; y = pMsg->param2;

           	// Check if it falls in the signature box
	        if( ( (SIG_PANEL_LEFT+GOL_EMBOSS_SIZE) < x) &&
   	            ( (SIG_PANEL_RIGHT-GOL_EMBOSS_SIZE) > x) &&
                ( (SIG_PANEL_TOP+GOL_EMBOSS_SIZE) < y) &&
                ( (SIG_PANEL_BOTTOM-GOL_EMBOSS_SIZE) > y) ){

                SetColor(BRIGHTRED);

                // if previous position is not valid or event is PRESS
                if((pMsg->uiEvent == EVENT_PRESS) || (prevX < 0) ){
                    PutPixel(x,y); 					// draw pixel
                }else{
                    Line(prevX,prevY,x,y); 			// connect with thick line previous and current points
                    Line(prevX-1,prevY,x-1,y);
                    Line(prevX+1,prevY,x+1,y);
                    Line(prevX,prevY-1,x,y-1);
                    Line(prevX,prevY+1,x,y+1);
                }

                prevX = x; prevY = y;  				// store position
            }else{
                // reset if outside of box
                prevX = -1;
            }

            return 1; // process by default

        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_POT; 			// goto potentiometer graph screen
                animate = 0;
            }
            return 1; // process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED){
                screenState = CREATE_CUSTOMCONTROL; // goto custom control screen
                animate = 0;
            }
            return 1; // process by default

        default:
            return 1; // process by default
    }
}

// dimensions for potentiometer graph area
#define POT_PANEL_LEFT   30
#define POT_PANEL_RIGHT  290
#define POT_PANEL_TOP    50
#define POT_PANEL_BOTTOM 210

// Graph area borders
#define POTGR_LEFT   (POT_PANEL_LEFT+GOL_EMBOSS_SIZE)  
#define POTGR_RIGHT  (POT_PANEL_RIGHT-GOL_EMBOSS_SIZE)
#define POTGR_TOP    (POT_PANEL_TOP+GOL_EMBOSS_SIZE) 
#define POTGR_BOTTOM (POT_PANEL_BOTTOM-GOL_EMBOSS_SIZE)

volatile SHORT buffer[256];
volatile SHORT counter=0;
volatile SHORT inc=10;


// Creates ntr screen
void CreatePotentiometer()
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	CreatePage("Plotting");

}

// Draws box for potentiometer graph
WORD PanelPotentiometer()
{
#define POTPNL_STATE_SET       	0
#define POTPNL_STATE_DRAW      	1
#define POTPNL_STATE_TEXT		2

static BYTE state = POTPNL_STATE_SET;

    if(state == POTPNL_STATE_SET)
    { 		// set data for panel
        GOLPanelDraw(	POT_PANEL_LEFT,POT_PANEL_TOP,POT_PANEL_RIGHT,POT_PANEL_BOTTOM,0,
            			WHITE,
            			altScheme->EmbossDkColor,
            			altScheme->EmbossLtColor,
            			NULL, GOL_EMBOSS_SIZE);

        state = POTPNL_STATE_DRAW; 		// change state
    }
	if (state == POTPNL_STATE_DRAW) 
	{
	    if(!GOLPanelDrawTsk()){
	    	 		// draw box for graph
	       return 0; 					// drawing is not completed
	    }
	    else
	    {
	    	state = POTPNL_STATE_TEXT;	// change to initial state
		    SetFont(&GOLMediumFont);           
			SetColor(BRIGHTBLUE);
			//MoveTo(GetMaxX()-(GetTextWidth("Using R6 potentiometer" ,&GOLMediumFont)+30),215);            
			MoveTo(GetMaxX()-(GetTextWidth("Using pseudo-random samples..." ,&GOLMediumFont)+30),215);
	       	//state = POTPNL_STATE_SET;	// change to initial state
	       	//return 1; 				// drawing is done
	    } 
	} 
	if (state == POTPNL_STATE_TEXT) 
	{
		while(!OutText("Using pseudo-random samples..."))
			return 0;	
	    state = POTPNL_STATE_SET;		// change to initial state
	    return 1; 						// drawing is done
	} 
	return 1;
    
}

#include <stdlib.h>

// Add sample from potentiometer ADC channel into temporary buffer
// Returns zero if samples number in the buffer is less than defined by parameter
WORD GetPotSamples(WORD number)
{
	static BYTE counter = 0;
	volatile SHORT  temp;

    temp = (rand()%1024)>>3;//temp =  ADCGetPot()>>3;
    if((temp+POTGR_TOP) > POTGR_BOTTOM)
        temp = POTGR_BOTTOM-POTGR_TOP;
        
    tempBuffer[counter++] = temp;
    if(counter>=number)
    {
        counter = 0;
        return 1;
    }
    return 0;
}

// Draws graph
void GraphPotentiometer()
{
	SHORT x,y;
	SHORT sy,ey;
	SHORT* ptr;
	SHORT counter;
	static SHORT pos;

    // remove graph
    SetColor(WHITE);
    ptr = potBuffer+pos;
    sy = *ptr++;
    for(x=POTGR_RIGHT; x>=POTGR_LEFT; x--)
    {
        if( ptr == (potBuffer + POT_BUFFER_SIZE) )
            ptr = potBuffer;
        ey = *ptr++;
        if(ey>sy)
        {
            for(y=sy+POTGR_TOP; y<ey+POTGR_TOP+1;y++)
                PutPixel(x,y);
        }
        else
        {
            for(y=ey+POTGR_TOP; y<sy+POTGR_TOP+1;y++)
                PutPixel(x,y);
        }
        sy = ey;
    }

    // draw grid
    SetColor(LIGHTGRAY);
    for(x=POTGR_LEFT+((POTGR_RIGHT-POTGR_LEFT)>>3); x<POTGR_RIGHT; x+=(POTGR_RIGHT-POTGR_LEFT)>>3)
        Bar(x,POTGR_TOP,x,POTGR_BOTTOM);

    for(y=POTGR_TOP+((POTGR_BOTTOM-POTGR_TOP)>>3); y<POTGR_BOTTOM; y+=(POTGR_BOTTOM-POTGR_TOP)>>3)
        Bar(POTGR_LEFT,y,POTGR_RIGHT,y);


    pos-=POT_MOVE_DELTA;
    if(pos<0)
        pos = POT_BUFFER_SIZE-1;   

    // copy new data from temporary buffer
    ptr = potBuffer+pos;
    for(counter=POT_MOVE_DELTA-1; counter>=0; counter--)
    {
        *ptr++ = tempBuffer[counter];
        if( ptr == (potBuffer + POT_BUFFER_SIZE) )
            ptr = potBuffer;
    }

    // draw graph
    SetColor(BRIGHTRED);
    ptr = potBuffer+pos;
    sy = *ptr++;
    for(x=POTGR_RIGHT; x>=POTGR_LEFT; x--)
    {
        if( ptr == (potBuffer + POT_BUFFER_SIZE) )
            ptr = potBuffer;
        ey = *ptr++;
        if(ey>sy)
        {
            for(y=sy+POTGR_TOP; y<ey+POTGR_TOP+1;y++)
                PutPixel(x,y);
        }
        else
        {
            for(y=ey+POTGR_TOP; y<sy+POTGR_TOP+1;y++)
                PutPixel(x,y);
        }
        sy = ey;
    }
}

// Processes messages for ECG screen
WORD MsgPotentiometer(WORD objMsg, OBJ_HEADER* pObj)
{

    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_ECG; 	   // goto ECG screen
            }
            return 1; 							// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_SIGNATURE; // goto signature screen
            }
            return 1; 							// process by default

        default:
            return 1; 							// process by default
    }
}


// dimensions for ECG graph area
#define ECG_PANEL_LEFT   29
#define ECG_PANEL_RIGHT  291
#define ECG_PANEL_TOP    45
#define ECG_PANEL_BOTTOM 210

// Graph area borders
#define GR_LEFT   ECG_PANEL_LEFT+GOL_EMBOSS_SIZE  
#define GR_RIGHT  ECG_PANEL_RIGHT-GOL_EMBOSS_SIZE
#define GR_TOP    ECG_PANEL_TOP+GOL_EMBOSS_SIZE 
#define GR_BOTTOM ECG_PANEL_BOTTOM-GOL_EMBOSS_SIZE

// Creates ECG screen
void CreateECG(){

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	CreatePage("ECG");

}

// Add sample from FLASH (can be any source) into temporary buffer
// Returns zero if samples number in the buffer is less than defined by parameter
WORD GetECGSamples(WORD number)
{
	static const SHORT* pointer = ecgPoints;
	static BYTE         counter = 0;

    if( pointer == (ecgPoints + sizeof(ecgPoints)/sizeof(SHORT)) )
        pointer = ecgPoints;

    tempBuffer[counter++] = *pointer++;

    if(counter>=number)
    {
        counter = 0;
        return 1;
    }
    return 0;
}

// Draws box for ECG
WORD PanelECG()
{
	#define ECG_STATE_SET       0
	#define ECG_STATE_DRAW      1
	#define ECG_STATE_VERLINE   2
	#define ECG_STATE_HORLINE   3
	#define ECG_STATE_TEXT	   	4
	
	static BYTE state = ECG_STATE_SET;
	static SHORT  pos;
      
    switch(state)
    { 
        case ECG_STATE_SET:
            // set parameters for panel
            GOLPanelDraw(ECG_PANEL_LEFT,ECG_PANEL_TOP,ECG_PANEL_RIGHT,ECG_PANEL_BOTTOM,0,
                WHITE,
                altScheme->EmbossDkColor,
                altScheme->EmbossLtColor,
                NULL, GOL_EMBOSS_SIZE);

            state = ECG_STATE_DRAW;   		// change state

        case ECG_STATE_DRAW:
            if(!GOLPanelDrawTsk())    		// draw panel for graph
                return 0;             		// drawing is not completed

            SetColor(LIGHTGRAY);
            SetLineType(DOTTED_LINE);
            pos = GR_LEFT+((GR_RIGHT-GR_LEFT)>>3);
            state = ECG_STATE_VERLINE; 		// change state

        case ECG_STATE_VERLINE:
            while(pos<GR_RIGHT)
            {       		// draw vertical grid lines
                if(IsDeviceBusy())
                    return 0;          		// drawing is not completed
                Line(pos,GR_TOP,pos,GR_BOTTOM);
                pos+=(GR_RIGHT-GR_LEFT)>>3;
            }
            pos = GR_TOP+((GR_BOTTOM-GR_TOP)>>3);
            state = ECG_STATE_HORLINE; 		// change state

        case ECG_STATE_HORLINE:
            while(pos<GR_BOTTOM)
            {       	// draw vertical grid lines
                if(IsDeviceBusy())
                    return 0;          		// drawing is not completed
                Line(GR_LEFT,pos,GR_RIGHT,pos);
                pos+=(GR_BOTTOM-GR_TOP)>>3;
            }
            SetLineType(SOLID_LINE);
		    SetFont(&GOLMediumFont);           
			SetColor(BRIGHTBLUE);
			MoveTo(GetMaxX()-(GetTextWidth("Scan mode" ,&GOLMediumFont)+30),215);            
            state = ECG_STATE_TEXT; 		// change to text display state

        case ECG_STATE_TEXT:
			while(!OutText("Scan mode"))
				return 0;	
		    state = ECG_STATE_SET;			// change to initial state
		    return 1; 						// drawing is done
	     
	} 
	return 1;
}

// Draws graph
void GraphECG()
{
	SHORT x,y;
	static SHORT sy = 0;
	static SHORT tsy = 0;
	SHORT ey;
	static SHORT* ptr = ecgBuffer;
	static SHORT pos = 0;
	SHORT counter;
	SHORT* pTemp;
	SHORT temp;

    // remove graph
    SetColor(WHITE);

    pTemp = ptr; temp = pos;
   
    for(x=0; x<ECG_WINDOW_SIZE; x++)
    {
        ey = *ptr++; pos++;

        if(ey>sy)
        {
            for(y=sy+GR_TOP; y<ey+GR_TOP+1;y++)
                PutPixel(GR_LEFT+pos,y);
        }
        else
        {
            for(y=ey+GR_TOP; y<sy+GR_TOP+1;y++)
                PutPixel(GR_LEFT+pos,y);
        }

        if( ptr == (ecgBuffer + ECG_BUFFER_SIZE) )
        {
            ptr = ecgBuffer; pos = 0;
        }

        sy = ey;
    }

    // copy new data from temporary buffer
    ptr = pTemp; pos = temp;

    for(counter=0; counter<ECG_WINDOW_SIZE; counter++)
    {
        *ptr++ = tempBuffer[counter]; pos++;
        if( ptr == (ecgBuffer + ECG_BUFFER_SIZE) )
        {
            ptr = ecgBuffer;  pos = 0;
        }
    }

   // draw graph
    SetColor(BRIGHTRED);

    ptr = pTemp; pos = temp;

    for(x=0; x<ECG_WINDOW_SIZE; x++)
    {
        ey = *ptr++; pos++;
        if(ey>tsy)
        {
            for(y=tsy+GR_TOP; y<ey+GR_TOP+1;y++)
                PutPixel(GR_LEFT+pos,y);
        }
        else
        {
            for(y=ey+GR_TOP; y<tsy+GR_TOP+1;y++)
                PutPixel(GR_LEFT+pos,y);
        }

        if( ptr == (ecgBuffer + ECG_BUFFER_SIZE) )
        {
            ptr = ecgBuffer; pos = 0;
        }

        tsy = ey;
    }
    // draw grid
    SetColor(LIGHTGRAY);
    SetLineType(DOTTED_LINE);
    for(x=GR_LEFT+((GR_RIGHT-GR_LEFT)>>3); x<GR_RIGHT; x+=(GR_RIGHT-GR_LEFT)>>3)
    {
        if((x>=GR_LEFT+temp) && (x<=GR_LEFT+ECG_WINDOW_SIZE+temp))
            Line(x,GR_TOP,x,GR_BOTTOM);
    }
    
    for(y=GR_TOP+((GR_BOTTOM-GR_TOP)>>3); y<GR_BOTTOM; y+=(GR_BOTTOM-GR_TOP)>>3)
        Line(GR_LEFT+temp,y,temp+GR_LEFT+ECG_WINDOW_SIZE,y);
    SetLineType(SOLID_LINE);
}

// Processes messages for ECG screen
WORD MsgECG(WORD objMsg, OBJ_HEADER* pObj)
{

    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_BUTTONS; 	// goto buttons screen
            }
            return 1; 							// process by default

        case ID_BUTTON_BACK:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = CREATE_POT;       // goto potentiometer screen
            }
            return 1; 							// process by default

        default:
            return 1; 							// process by default
    }
}

// Output text message on screen and stop execution
void ErrorTrap(char* message)
{
    SetColor(BLACK);
    ClearDevice();
    SetFont(&FONTDEFAULT);
    SetColor(WHITE);    
    OutTextXY(0,0,message);
    while(1);
}



/*********************************************************************
* Function: Timer4 ISR
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: increments tick counter. Tick is approx. 1 ms.
*
* Note: none
*
********************************************************************/
#if 0
void __attribute__((interrupt, shadow, auto_psv)) _T4Interrupt(void)
{
    tick++;    
    // Clear flag
    IFS1bits.T4IF = 0;
}
#else
void timerfunc(int sig)
{
  	signal(SIGALRM, timerfunc);
  	tick += 100;    
  	//printf("%s:%s():%d %u\n", __FILE__, __FUNCTION__, __LINE__, tick );
}
#endif
/*********************************************************************
* Function: void TickInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: sets tick timer
*
* Note: none
*
********************************************************************/
void TickInit(void)
{
#if 0	
	#define TICK_PERIOD    16000
    // Initialize Timer4
    TMR4 = 0;
    PR4 = TICK_PERIOD;
    T3CONbits.TCKPS = 0b01;         //Set prescale to 1:8
    IFS1bits.T4IF = 0;              //Clear flag
    IEC1bits.T4IE = 1;              //Enable interrupt
    T4CONbits.TON = 1;              //Run timer
#else
	struct itimerval iv;

  	iv.it_value.tv_sec  = iv.it_interval.tv_sec  = 0;
  	iv.it_value.tv_usec = iv.it_interval.tv_usec = 100000;

  	setitimer(ITIMER_REAL, &iv, NULL);
  	signal(SIGALRM, timerfunc);    
#endif      
}

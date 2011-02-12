#ifndef __OBJECTTEST_H__
#define  __OBJECTTEST_H__

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
#define ID_WINDOW1          10

#define ID_BUTTON1          11
#define ID_BUTTON2          12
#define ID_BUTTON3          13
#define ID_BUTTON4          14
#define ID_BUTTON5          15
#define ID_BUTTON6          16
#define ID_BUTTON7          17

#define ID_BUTTON_BACK      18
#define ID_BUTTON_NEXT      19

#define ID_CHECKBOX1        20
#define ID_CHECKBOX2        21
#define ID_CHECKBOX3        22
#define ID_CHECKBOX4        23
#define ID_CHECKBOX5        24

#define ID_RADIOBUTTON1     30
#define ID_RADIOBUTTON2     31
#define ID_RADIOBUTTON3     32
#define ID_RADIOBUTTON4     33
#define ID_RADIOBUTTON5     34
#define ID_RADIOBUTTON6     35
#define ID_RADIOBUTTON7     36
#define ID_RADIOBUTTON8     37

#define ID_SLIDER1          40
#define ID_SLIDER2          41
#define ID_SLIDER3          42

#define ID_GROUPBOX1        50
#define ID_GROUPBOX2        51
#define ID_STATICTEXT1      52
#define ID_STATICTEXT2      53
#define ID_STATICTEXT3      54

#define ID_PICTURE1         55
//#define ID_STATICTEXT4      56

#define ID_PROGRESSBAR1     60
#define ID_CUSTOM1          70

#define ID_LISTBOX1         80
#define ID_LISTBOX2         81
#define ID_LISTBOX3         82
#define ID_EDITBOX1         83
//#define ID_LISTBOX4         84

#define ID_CALL             91
#define ID_STOPCALL         92
#define ID_HOLD             93
#define ID_BACKSPACE        94
#define ID_POUND            95
#define ID_ASTERISK         96
#define ID_KEYPAD           100                         /* uses 100 to 110 for phone key pad demo */

#define ID_ROUNDDIAL        120
#define ID_METER1           130
#define ID_METER2           131
#define ID_DIGITALMETER1    132
#define ID_DIGITALMETER2    133

#define ID_BUTTONHR_P       150
#define ID_BUTTONHR_M       151
#define ID_BUTTONMN_P       152
#define ID_BUTTONMN_M       153
#define ID_BUTTONSC_P       154
#define ID_BUTTONSC_M       155

#define ID_BUTTON_DATE_UP   180
#define ID_BUTTON_DATE_DN   181

#define ID_BUTTON_MO        186
#define ID_BUTTON_DY        187
#define ID_BUTTON_YR        188
#define ID_BUTTON_RET       189

#define ID_EB_MONTH         200
#define ID_EB_DAY           201
#define ID_EB_YEAR          202

#define ID_EB_HOUR          205
#define ID_EB_MINUTE        206
#define ID_EB_SECOND        207

#define NAV_BTN_WIDTH		25

#define WAIT_UNTIL_FINISH(x)    while(!x)


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





/////////////////////////////////////////////////////////////////////////////
//                            DEMO STATES
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CREATE_BUTTONS          = 0,
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
///	
	CREATE_RENO_DATASET,
	DISPLAY_RENO_DATASET,
///
    // these states are for time and date settings 0xF3xx is used here as a
    // state ID to check when date and time are to be updated or not.
    CREATE_DATETIME         = 0xF300,                       // creates the date and time setting menu		
    DISPLAY_DATETIME        = 0xF301,                       // displays the menu for the date and time setting
    DISPLAY_DATE_PDMENU     = 0xF302,                       // displays the pulldown menu to set one of the date items (month, day or year)
    SHOW_DATE_PDMENU        = 0xF303,                       // creates the pulldown menu to set one of the date items (month, day or year)
    HIDE_DATE_PDMENU        = 0xF304,                       // hides the pulldown menu to set one of the date items (month, day or year)
} SCREEN_STATES;







#endif	/*#ifndef __OBJECTTEST_H__*/
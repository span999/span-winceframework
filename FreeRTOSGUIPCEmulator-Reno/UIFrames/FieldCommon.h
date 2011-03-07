#ifndef __FIELDCOMMON_H__
#define  __FIELDCOMMON_H__


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


#define ID_RENO_BASE		500

///for ticking simulate
#define ID_TICKING         		(ID_RENO_BASE+8)

///for HW key simulate
#define ID_BTN_UP          		(ID_RENO_BASE+11)
#define ID_BTN_UP_HOLD     		(ID_RENO_BASE+12)
#define ID_BTN_DOWN         	(ID_RENO_BASE+13)
#define ID_BTN_DOWN_HOLD    	(ID_RENO_BASE+14)
#define ID_BTN_EXIT          	(ID_RENO_BASE+15)
#define ID_BTN_EXIT_HOLD     	(ID_RENO_BASE+16)
#define ID_BTN_ENTER          	(ID_RENO_BASE+17)
#define ID_BTN_ENTER_HOLD     	(ID_RENO_BASE+18)
#define ID_DATASETVALUE1      	(ID_RENO_BASE+19)
#define ID_DATASETVALUE2      	(ID_RENO_BASE+20)
#define ID_DATASETVALUE3      	(ID_RENO_BASE+21)
#define ID_DATASETVALUE4      	(ID_RENO_BASE+22)
#define ID_DATASETVALUE5      	(ID_RENO_BASE+23)
#define ID_DATASETVALUE6      	(ID_RENO_BASE+24)
#define ID_ENTRY_STATICTEXT     (ID_RENO_BASE+25)
#define ID_ENTRY_BTN_UP     	(ID_RENO_BASE+26)
#define ID_ENTRY_BTN_DOWN     	(ID_RENO_BASE+27)




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


extern GOL_SCHEME      *altScheme;                                 // alternative style scheme
extern GOL_SCHEME      *alt2Scheme;                                // alternative 2 style scheme
extern GOL_SCHEME      *alt3Scheme;                                // alternative 3 style scheme
///extern GOL_SCHEME      *alt4Scheme;                                // alternative 4 style scheme
///extern GOL_SCHEME      *alt5Scheme;                                // alternative 5 style scheme
///extern GOL_SCHEME      *timeScheme;
///extern GOL_SCHEME      *meterScheme;                               // meter scheme
extern GOL_SCHEME      *navScheme;                                 // style scheme for the navigation
///extern GOL_SCHEME      *redScheme;                                 // alternative red style scheme
extern GOL_SCHEME      *greenScheme;                               // alternative green style scheme
extern GOL_SCHEME      *yellowScheme;                              // alternative yellow style scheme
extern OBJ_HEADER      *pNavList;                                  // pointer to navigation list

extern GOL_SCHEME      *popupMenuScheme;				// Popup Menu style scheme
extern GOL_SCHEME      *dataSetScheme;					// dataSet style scheme
extern GOL_SCHEME      *dataSetScheme2;					// dataSet style scheme
extern GOL_SCHEME      *windowScheme;					// window style scheme
extern GOL_SCHEME      *popupAskScheme;					// Popup Ask style scheme
extern GOL_SCHEME      *entryPadScheme;					// entry pad style scheme
void myCreateScheme( void );


void updateLastView(FRAME_HEADER* pLastFrame);
FRAME_HEADER* getLastView(void);


extern XCHAR Num2Xchar( WORD wNum );

/*
	API for screen status handle
*/
void scrInitStat(void);
void scrNextStat(void);
void scrPrivStat(void);
void scrCreateDone(void);
void scrCreateInit(void);
BOOL scrIsCreateDone(void);
void scrSetStat(FRAME_HEADER* phFrame);
FRAME_HEADER* scrGetStat(void);
FRAME_HEADER* scrGetPrivStat(void);
void scrSetNEXT(FRAME_HEADER* phFrame);
void scrFixStat(void);
void scrUnFixStat(void);
BOOL scrIsUpdateVaild(void);


/// use for MsgCallback & DrawCallback
WORD scrMsgCbHandler(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
void scrDrawCbHandler(void);


void CreateDefaultBtn(void);
WORD MsgDefaultBtn(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
///void CreateDataSet(SHORT left, SHORT top, SHORT right, SHORT bottom, char *pText, char *pFunc, char *pData, char *pUnit);
///void CreateDataSet(SHORT left, SHORT top, SHORT right, SHORT bottom, XCHAR *pText, XCHAR *pFunc, XCHAR *pData, XCHAR *pUnit);
STATICTEXT *CreateDataSet(SHORT left, SHORT top, SHORT right, SHORT bottom, XCHAR *pText, XCHAR *pFunc, XCHAR *pData, XCHAR *pUnit);
void AddItemList(XCHAR *pText, LISTBOX *pLb, void *pIcon);


///#define  USE_DEFAULTBTN
#define		USE_MAGELLAN_LOGO

#ifdef USE_DEFAULTBTN
#define DEFAULTBTN_WIDTH	70
#define DEFAULTBTN_HEIGHT	20
#else
#define DEFAULTBTN_WIDTH	0
#define DEFAULTBTN_HEIGHT	0
#endif
///#define HWBTN_WIDTH		20
///#define HWBTN_HEIGHT	15
#define HWBTN_WIDTH		2
#define HWBTN_HEIGHT	1


#define NAV_BTN_WIDTH		25
///#define WAIT_UNTIL_FINISH(x)    while(!x)

#define MIN(x,y)                ((x > y)? y: x)
#define WAIT_UNTIL_FINISH(x)    while(!x)	
#define DEMODELAY				1000


extern FRAME_HEADER fhTextEnteryPad;
extern FRAME_HEADER fhNumEnteryPad;



#endif	/*#ifndef __FIELDCOMMON_H__*/

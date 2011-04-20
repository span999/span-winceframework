/*
	declare all frame page here
*/


#include "RenoGUI.h"
#include "RenoUItoolhelp.h"
#include "RenoUIframe.h"
#include "RenoUIcommon.h"

#include "RenoUICcomm.h"
#include "RenoUICdatamode.h"
#include "RenoUIClistmenu.h"
#include "RenoUICftextentry.h"
#include "RenoUICwatchmode.h"


static const GUI_ConstString _apListBox[] = {
  "English", "Deutsch", "Français", "Japanese", "Italiano", NULL
};








/*
*/
FRAMEPAGE_HEADER headPoweroffWindow = {
	FRAMEPAGE_COMMON,
	PoweroffWindow,
	cbPoweroffWindow,
	NULL,
	0,
	0,
	1,
	NULL,
	NULL,
	NULL,
};

/*
*/
FRAMEPAGE_HEADER headBootWindow = {
	FRAMEPAGE_COMMON,
	BootWindow,
	NULL,
	NULL,
	100,
	0,
	1,
	NULL,
	NULL,
	&headDataMode1Window,
};

FRAMEPAGE_HEADER headDataModeWindow = {
	FRAMEPAGE_ACTIVITY,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	NULL,
	NULL,
	NULL,
};


/*
	float text entry
*/
static const GUI_ConstString _FloatTextEntryBox[] = {
  "A", "B", "C", "D", "E", "F",
  "G", "H", "I", "J", "K", "L",
  "M", "N", "O", "P", "Q", "R",
  "S", "T", "U", "V", "W", "X",
  "Y", "Z", 
  NULL
};

FP_FTEXTENTRY_HEADER fpFloatTextEntry_Windows[] = {
	26,
	0,
	"F Text entry",
	_FloatTextEntryBox,
	&headSUICIWindow,
	NULL,
	NULL,
	NULL,
	NULL,
	"ABCDEFG              ",
	0
};

FRAMEPAGE_HEADER headFloatTextEntryWindow = {
	FRAMEPAGE_FLOATTEXT_ENTRY,
	FloatTextEntryWindow,
	cbFloatTextEntryWindow,
	NULL,
	0,
	0,
	1,
	&fpFloatTextEntry_Windows,
	NULL,
	NULL,
};



/*
	Popup notify
*/
extern GUI_CONST_STORAGE GUI_BITMAP bmReno_Icon_20_GPS_Search;
extern GUI_CONST_STORAGE GUI_BITMAP bmReno_Icon_20_GPS_Off;
extern GUI_CONST_STORAGE GUI_BITMAP bmReno_Icon_20_GPS_Lock;

FP_POPUPNOTIFY_HEADER fpPopupNotify_Windows[] = {
	"GPS signal\n\r searching ...",
	&bmReno_Icon_20_GPS_Search,
	NULL,
	NULL,
	NULL
};

FRAMEPAGE_HEADER headPopupNotifyWindow = {
	FRAMEPAGE_POPUP_NOTIFY,
	PopupWindowNotify,
	cbPopupWindowNotify,
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupNotify_Windows,
	NULL,
	NULL,
};

/* GPS Search */
FP_POPUPNOTIFY_HEADER fpPopupNotifyGPSSearch_Windows[] = {
	"GPS signal\n\r searching ...",
	&bmReno_Icon_20_GPS_Search,
	NULL,
	NULL,
	NULL
};

FRAMEPAGE_HEADER headPopupNotifyGPSSearchWindow = {
	FRAMEPAGE_POPUP_NOTIFY,
	PopupWindowNotify,
	cbPopupWindowNotify,
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupNotifyGPSSearch_Windows,
	NULL,
	NULL,
};

/* GPS Off */
FP_POPUPNOTIFY_HEADER fpPopupNotifyGPSOff_Windows[] = {
	"GPS signal\n\r Off ...",
	&bmReno_Icon_20_GPS_Off,
	NULL,
	NULL,
	NULL
};

FRAMEPAGE_HEADER headPopupNotifyGPSOffWindow = {
	FRAMEPAGE_POPUP_NOTIFY,
	PopupWindowNotify,
	cbPopupWindowNotify,
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupNotifyGPSOff_Windows,
	NULL,
	NULL,
};

/* GPS Lock */
FP_POPUPNOTIFY_HEADER fpPopupNotifyGPSLock_Windows[] = {
	"GPS signal\n\r Lock ...",
	&bmReno_Icon_20_GPS_Lock,
	NULL,
	NULL,
	NULL
};

FRAMEPAGE_HEADER headPopupNotifyGPSLockWindow = {
	FRAMEPAGE_POPUP_NOTIFY,
	PopupWindowNotify,
	cbPopupWindowNotify,
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupNotifyGPSLock_Windows,
	NULL,
	NULL,
};


/*
	Data mode with 1 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_1Window[] = {
	{ 0, ACTDATA_ID_DISTANCE, "distance", "/mi", "10.01" },
	{ 1, ACTDATA_ID_TIME, "time", "", "21:45" },
	{ 2, ACTDATA_ID_NONE, "avg pace", "/mi", "7:15" },
	{ 3, ACTDATA_ID_ELEVATION, "elevation", "/ft", "155" },
	{ 4, ACTDATA_ID_NONE, "lap avg hr", "bpm", "118" },
	{ 5, ACTDATA_ID_POWER, "power", "w", "1025" },
};

static TEXT_Handle _DataModeText_1Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static WM_HWIN _DataModeWin_1Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static FRAMEPAGE_HEADER* _DataModeFrame_1Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_1Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_1Window = {
	1,
	NULL,
	_DataModeContent_1Window,
	_DataModeText_1Window,
	_DataModeWin_1Window,
	_DataModeFrame_1Window,
	_DataModeParam_1Window,
};

FRAMEPAGE_HEADER headDataMode1Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_1Window,
	NULL,
	NULL,
};


/*
	Data mode with 2 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_2Window[] = {
	{ 0, ACTDATA_ID_DISTANCE, "distance", "/mi", "10.01" },
	{ 1, ACTDATA_ID_TIME, "time", "", "21:45" },
	{ 2, ACTDATA_ID_NONE, "avg pace", "/mi", "7:15" },
	{ 3, ACTDATA_ID_ELEVATION, "elevation", "/ft", "155" },
	{ 4, ACTDATA_ID_NONE, "lap avg hr", "bpm", "118" },
	{ 5, ACTDATA_ID_POWER, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_2Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_2Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_2Window = {
	2,
	NULL,
	_DataModeContent_2Window,
	_DataModeText_1Window,
	_DataModeWin_1Window,
	_DataModeFrame_2Window,
	_DataModeParam_2Window,
};

FRAMEPAGE_HEADER headDataMode2Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_2Window,
	NULL,
	NULL,
};


/*
	Data mode with 3 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_3Window[] = {
	{ 0, ACTDATA_ID_DISTANCE, "distance", "/mi", "10.01" },
	{ 1, ACTDATA_ID_TIME, "time", "", "21:45" },
	{ 2, ACTDATA_ID_NONE, "avg pace", "/mi", "7:15" },
	{ 3, ACTDATA_ID_ELEVATION, "elevation", "/ft", "155" },
	{ 4, ACTDATA_ID_NONE, "lap avg hr", "bpm", "118" },
	{ 5, ACTDATA_ID_POWER, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_3Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_3Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_3Window = {
	3,
	NULL,
	_DataModeContent_3Window,
	_DataModeText_1Window,
	_DataModeWin_1Window,
	_DataModeFrame_3Window,
	_DataModeParam_3Window,
};

FRAMEPAGE_HEADER headDataMode3Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_3Window,
	NULL,
	NULL,
};


/*
	Data mode with 4 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_4Window[] = {
	{ 0, ACTDATA_ID_DISTANCE, "distance", "/mi", "10.01" },
	{ 1, ACTDATA_ID_TIME, "time", "", "21:45" },
	{ 2, ACTDATA_ID_NONE, "avg pace", "/mi", "7:15" },
	{ 3, ACTDATA_ID_ELEVATION, "elevation", "/ft", "155" },
	{ 4, ACTDATA_ID_NONE, "lap avg hr", "bpm", "118" },
	{ 5, ACTDATA_ID_POWER, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_4Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_4Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_4Window = {
	4,
	NULL,
	_DataModeContent_4Window,
	_DataModeText_1Window,
	_DataModeWin_1Window,
	_DataModeFrame_4Window,
	_DataModeParam_4Window,
};

FRAMEPAGE_HEADER headDataMode4Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_4Window,
	NULL,
	NULL,
};


/*
	Data mode with 5 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_5Window[] = {
	{ 0, ACTDATA_ID_DISTANCE, "distance", "/mi", "10.01" },
	{ 1, ACTDATA_ID_TIME, "time", "", "21:45" },
	{ 2, ACTDATA_ID_NONE, "avg pace", "/mi", "7:15" },
	{ 3, ACTDATA_ID_ELEVATION, "elevation", "/ft", "155" },
	{ 4, ACTDATA_ID_NONE, "lap avg hr", "bpm", "118" },
	{ 5, ACTDATA_ID_POWER, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_5Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_5Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_5Window = {
	5,
	NULL,
	_DataModeContent_5Window,
	_DataModeText_1Window,
	_DataModeWin_1Window,
	_DataModeFrame_5Window,
	_DataModeParam_5Window,
};

FRAMEPAGE_HEADER headDataMode5Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_5Window,
	NULL,
	NULL,
};


/*
	Data mode with 6 Data(Activity) set
*/
DATASET_HEADER _DataModeContent_6Window[] = {
	{ 0, ACTDATA_ID_DISTANCE, "distance", "/mi", "10.01" },
	{ 1, ACTDATA_ID_TIME, "time", "", "21:45" },
	{ 2, ACTDATA_ID_NONE, "avg pace", "/mi", "7:15" },
	{ 3, ACTDATA_ID_ELEVATION, "elevation", "/ft", "155" },
	{ 4, ACTDATA_ID_NONE, "lap avg hr", "bpm", "118" },
	{ 5, ACTDATA_ID_POWER, "power", "w", "1025" },
};

static FRAMEPAGE_HEADER* _DataModeFrame_6Window[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static int _DataModeParam_6Window[] = {
	0, 0, 0, 0, 0, 0
};

FP_DATASETS_HEADER fpDataModeData_6Window = {
	6,
	NULL,
	_DataModeContent_6Window,
	_DataModeText_1Window,
	_DataModeWin_1Window,
	_DataModeFrame_6Window,
	_DataModeParam_6Window,
};

FRAMEPAGE_HEADER headDataMode6Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpDataModeData_6Window,
	NULL,
	NULL,
};


/*
	Popup Fitness
*/
static const GUI_ConstString _PopupListBox_Fitness[] = {
  "Activity", "Navigation", "Settings", "History", NULL
};

static FRAMEPAGE_HEADER* _PopupListFrame_Fitness[] = {
	&headDataMode1Window, &headNavigationWindow, &headSettingsWindow, &headHistoryWindow
};

static int _PopupListParam_Fitness[] = {
	0, 0, 0, 0
};

FP_POPUPLIST_HEADER fpPopupListData_Fitness = {
	4,
	0,
	NULL,
	_PopupListBox_Fitness,
	NULL,
	_PopupListFrame_Fitness,
	_PopupListParam_Fitness,
	0,
};

FRAMEPAGE_HEADER headPopupListWindow_Fitness = {
	FRAMEPAGE_POPUP_OPTION,
	PopupWindowList,
	cbPopupWindowList,
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupListData_Fitness,
	NULL,
	NULL,
};


/*
	Popup Watch
*/
static const GUI_ConstString _PopupListBox_DeviceModeFitness[] = {
  "Watch mode", "Power down", NULL
};

static FRAMEPAGE_HEADER* _PopupListFrame_DeviceModeFitness[] = {
	&headWatchWindow, &headPoweroffWindow
};

static int _PopupListParam_DeviceModeFitness[] = {
	0, 0
};

FP_POPUPLIST_HEADER fpPopupListData_DeviceModeFitness = {
	2,
	0,
	NULL,
	_PopupListBox_DeviceModeFitness,
	NULL,
	_PopupListFrame_DeviceModeFitness,
	_PopupListParam_DeviceModeFitness,
	0,
};

FRAMEPAGE_HEADER headPopupListWindow_DeviceModeFitness = {
	FRAMEPAGE_POPUP_OPTION,
	PopupWindowList,
	cbPopupWindowList,
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupListData_DeviceModeFitness,
	NULL,
	NULL,
};


/*
	Popup Number entry
*/
static const GUI_ConstString _PopupListBox_NumberEntry[] = {
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", NULL
};

static FRAMEPAGE_HEADER* _PopupListFrame_NumberEntry[] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

static int _PopupListParam_NumberEntry[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

FP_POPUPLIST_HEADER fpPopupListData_NumberEntry = {
	10,
	0,
	NULL,
	_PopupListBox_NumberEntry,
	NULL,
	_PopupListFrame_NumberEntry,
	_PopupListParam_NumberEntry,
	0,
};

FRAMEPAGE_HEADER headPopupListWindow_NumberEntry = {
	FRAMEPAGE_POPUP_NUMBERS,
	PopupWindowNumbers,
	cbPopupWindowNumbers,
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupListData_NumberEntry,
	NULL,
	NULL,
};


/*
*/
FRAMEPAGE_HEADER headPopupListWindow = {
	FRAMEPAGE_POPUP_OPTION,
	PopupWindowList,
	cbPopupWindowList,
	NULL,
	0,
	0,
	0,
	NULL,
	NULL,
	NULL,
};


/*
	Navigation
*/
FRAMEPAGE_HEADER headNavigationWindow = {
	FRAMEPAGE_TITLED,
	NavigationWindow,
	cbNavigationWindow,
	NULL,
	0,
	0,
	1,
	NULL,
	NULL,
	NULL,
};


/*
	Settings 
*/
static const GUI_ConstString _SettingListBox[] = {
  "GPS Settings",
  "Device Settings",
  "Activity Profiles",
  "User Information",
  "Device Information",
  NULL
};

static FRAMEPAGE_HEADER* _SettingListFrame[] = {
	&headSGSWindow,
	&headSDSWindow,
	&headSAPWindow,
	&headSUIWindow,
	&headUnderConstructionWindow
};

static int _SettingListParam[] = {
	0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SettingsWindow = {
	5,
	"Settings",
	_SettingListBox,
	NULL,
	NULL,
	_SettingListFrame,
	_SettingListParam,
	0
};

FRAMEPAGE_HEADER headSettingsWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SettingsWindow,
	NULL,
	NULL,
};


/*
	Settings / GPS Settings
*/
static const GUI_ConstString _SGSListBox[] = {
	"GPS Enabled: ",
	"GPS Satellites",
	"Navigation",
	NULL
};

static FRAMEPAGE_HEADER* _SGSListFrame[] = {
	&headUnderConstructionWindow,
	&headSGSGSWindow,
	&headSGSNWindow
};

static int _SGSListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1,
	0, 
	0
};

FP_LISTMENU_HEADER fpListMenuData_SGSWindow = {
	3,
	"GPS Settings",
	_SGSListBox,
	NULL,
	&headSettingsWindow,
	_SGSListFrame,
	_SGSListParam,
	0
};

FRAMEPAGE_HEADER headSGSWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SGSWindow,
	NULL,
	NULL,
};


/*
	Settings / GPS Settings / GPS Satellites
*/
/*
static const GUI_ConstString _SGSGSListBox[] = {
	"GPS Satellites",
	"GPS Satellites",
	"GPS Satellites",
	NULL
};

static FRAMEPAGE_HEADER* _SGSGSListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SGSGSListParam[] = {
	0,
	0, 
	0
};

FP_LISTMENU_HEADER fpListMenuData_SGSGSWindow = {
	3,
	"GPS Satellites",
	_SGSGSListBox,
	NULL,
	&headSGSWindow,
	_SGSGSListFrame,
	_SGSGSListParam,
	0
};
*/
FRAMEPAGE_HEADER headSGSGSWindow = {
	FRAMEPAGE_TITLED,
	SGSGSWindow,
	cbSGSGSWindow,
	NULL,
	0,
	0,
	1,
	NULL,
	NULL,
	NULL,
};


/*
	Settings / GPS Settings / Navigation
*/
static const GUI_ConstString _SGSNListBox[] = {
	"GPS Navigation",
	"GPS Navigation",
	"GPS Navigation",
	NULL
};

static FRAMEPAGE_HEADER* _SGSNListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SGSNListParam[] = {
	0,
	0, 
	0
};

FP_LISTMENU_HEADER fpListMenuData_SGSNWindow = {
	3,
	"Navigation",
	_SGSNListBox,
	NULL,
	&headSGSWindow,
	_SGSNListFrame,
	_SGSNListParam,
	0
};

FRAMEPAGE_HEADER headSGSNWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SGSNWindow,
	NULL,
	NULL,
};



/*
	Settings / User Information
*/
static const GUI_ConstString _SUIListBox[] = {
	"Physical Information",
	"Contact Information",
	"Emergency Information",
	NULL
};

static FRAMEPAGE_HEADER* _SUIListFrame[] = {
	&headSUIPIWindow,
	&headSUICIWindow,
	&headUnderConstructionWindow
};

static int _SUIListParam[] = {
	0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SUIWindow = {
	3,
	"User Information",
	_SUIListBox,
	NULL,
	&headSettingsWindow,
	_SUIListFrame,
	_SUIListParam,
	0
};

FRAMEPAGE_HEADER headSUIWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUIWindow,
	NULL,
	NULL,
};


/*
	Settings / User Information / Physical Information
*/
static const GUI_ConstString _SUIPIListBox[] = {
	"Age",
	"Gender",
	"Height",
	"Weight",
	"Resting Heart Rate",
	NULL
};

static FRAMEPAGE_HEADER* _SUIPIListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SUIPIListParam[] = {
	0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SUIPIWindow = {
	5,
	"Physical Information",
	_SUIPIListBox,
	NULL,
	&headSUIWindow,
	_SUIPIListFrame,
	_SUIPIListParam,
	0
};

FRAMEPAGE_HEADER headSUIPIWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUIPIWindow,
	NULL,
	NULL,
};


/*
	Settings / User Information / Contact Information
*/
static const GUI_ConstString _SUICIListBox[] = {
	"Phone",
	"E-mail",
	"Address",
	NULL
};

static FRAMEPAGE_HEADER* _SUICIListFrame[] = {
	&headSUICIPWindow,
	&headSUICIEWindow,
///	&headSUICIAWindow
	&headFloatTextEntryWindow
};

static int _SUICIListParam[] = {
	0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SUICIWindow = {
	3,
	"Contact Information",
	_SUICIListBox,
	NULL,
	&headSUIWindow,
	_SUICIListFrame,
	_SUICIListParam,
	0
};

FRAMEPAGE_HEADER headSUICIWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUICIWindow,
	NULL,
	NULL,
};


/*
	Settings / User Information / Contact Information / Address
*/
static const GUI_ConstString _SUICIAListBox[] = {
	"<-Bak",
	"A/B/C",
	"D/E/F",
	"G/H/I",
	"J/K/L",
	"M/N/O",
	"P/Q/R",
	"S/T/U",
	"V/W/X",
	"Y/Z/ ",
	",/./:",
	"!/@/-",
	"(/)/0",
	"1/2/3",
	"4/5/6",
	"7/8/9",
	NULL
};

static FRAMEPAGE_HEADER* _SUICIAListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SUICIAListParam[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SUICIAWindow = {
	16,
	"Address",
	_SUICIAListBox,
	NULL,
	&headSUICIWindow,
	_SUICIAListFrame,
	_SUICIAListParam,
	0
};

FRAMEPAGE_HEADER headSUICIAWindow = {
	FRAMEPAGE_LISTMENU,
	TextEntryWindow,
	cbTextEntryWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUICIAWindow,
	NULL,
	NULL,
};


/*
	Settings / User Information / Contact Information / Phone
*/
FP_LISTMENU_HEADER fpListMenuData_SUICIPWindow = {
	16,
	"Phone",
	_SUICIAListBox,
	NULL,
	&headSUICIWindow,
	_SUICIAListFrame,
	_SUICIAListParam,
	0
};

FRAMEPAGE_HEADER headSUICIPWindow = {
	FRAMEPAGE_LISTMENU,
	TextEntryWindow,
	cbTextEntryWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUICIPWindow,
	NULL,
	NULL,
};


/*
	Settings / User Information / Contact Information / Email
*/
FP_LISTMENU_HEADER fpListMenuData_SUICIEWindow = {
	16,
	"Email",
	_SUICIAListBox,
	NULL,
	&headSUICIWindow,
	_SUICIAListFrame,
	_SUICIAListParam,
	0
};

FRAMEPAGE_HEADER headSUICIEWindow = {
	FRAMEPAGE_LISTMENU,
	TextEntryWindow,
	cbTextEntryWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SUICIEWindow,
	NULL,
	NULL,
};



/*
	Settings / Device Settings
*/
static const GUI_ConstString _SDSListBox[] = {
  "Language: ",
  "Date & Time",
  "Units of Measurement",
  "Activity Recording",
  "ANT+ Sensors",
  "Feedback",
  "Equipment",
  NULL
};

static FRAMEPAGE_HEADER* _SDSListFrame[] = {
	&headSDSLWindow,
	&headSDSDTWindow,
	&headSDSUMWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SDSListParam[] = {
	0, 0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SDSWindow = {
	7,
	"Device Settings",
	_SDSListBox,
	NULL,
	&headSettingsWindow,
	_SDSListFrame,
	_SDSListParam,
	0
};

FRAMEPAGE_HEADER headSDSWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSWindow,
	NULL,
	NULL,
};


/*
	Settings / Device Settings / Language
*/
static const GUI_ConstString _SDSLListBox[] = {
  "English",
  "Spanish",
  "French",
  "Italian",
  "Portuguese",
  "Dutch",
  "German",
  "Danish",
  "Greek",
  "Nordic",
  NULL
};

static FRAMEPAGE_HEADER* _SDSLListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SDSLListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};


FP_LISTMENU_HEADER fpListMenuData_SDSLWindow = {
	10,
	"Language",
	_SDSLListBox,
	NULL,
	&headSDSWindow,
	_SDSLListFrame,
	_SDSLListParam,
	0
};

FRAMEPAGE_HEADER headSDSLWindow = {
///	FRAMEPAGE_LISTMENU,
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSLWindow,
	NULL,
	NULL,
};


/*
	Settings / Device Settings / Date & Time
*/
static const GUI_ConstString _SDSDTListBox[] = {
  "<Date> <Time>",
  "Date Format: ",
  "Time Format: ",
  "Time Source: ",
  "Time Zone: ",
  "DST: ",
  NULL
};

static FRAMEPAGE_HEADER* _SDSDTListFrame[] = {
	&headUnderConstructionWindow,
	&headSDSDTDFWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SDSDTListParam[] = {
	(COMMON_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(COMMON_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(COMMON_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(COMMON_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(COMMON_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(COMMON_FIELD<<ELEMENT_FIELD_OFFSET)|0
};


FP_LISTMENU_HEADER fpListMenuData_SDSDTWindow = {
	6,
	"Date & Time",
	_SDSDTListBox,
	NULL,
	&headSDSWindow,
	_SDSDTListFrame,
	_SDSDTListParam,
	0
};

FRAMEPAGE_HEADER headSDSDTWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSDTWindow,
	NULL,
	NULL,
};


/*
	Settings / Device Settings / Date & Time / Date Format
*/
static const GUI_ConstString _SDSDTDFListBox[] = {
	"Year-Month-Day\r\n(YYYY-MM-DD)",
	"Month/Day/Year\r\n(MM/DD/YYYY)",
	"Day-Month-Year\r\n(DD-MM-YYYY)",
	NULL
};

static const GUI_ConstString _SDSDTDFListShort[] = {
	"Y-M-D",
	"M/D/Y",
	"D-M-Y",
	NULL
};

static FRAMEPAGE_HEADER* _SDSDTDFListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSDTDFListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSDTDFWindow = {
	3,
	"Date Format",
	_SDSDTDFListBox,
	_SDSDTDFListShort,
	&headSDSDTWindow,
	_SDSDTDFListFrame,
	_SDSDTDFListParam,
	15
};

FRAMEPAGE_HEADER headSDSDTDFWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSDTDFWindow,
	NULL,
	NULL,
};



/*
	Settings / Device Settings / Units of Measurement
*/
static const GUI_ConstString _SDSUMListBox[] = {
  "Coordinates: ",
  "Speed/Distance: ",
  "Elevation: ",
  "Height/Weight: ",
  "Temperature: ",
  "Heart Rate: ",
  "Power: ",
  NULL
};

static FRAMEPAGE_HEADER* _SDSUMListFrame[] = {
	&headSDSUMCWindow,
	&headSDSUMSDWindow,
	&headSDSUMEWindow,
	&headSDSUMHWWindow,
	&headSDSUMTWindow,
	&headSDSUMHRWindow,
	&headSDSUMPWindow,
};

static int _SDSUMListParam[] = {
	0, 0, 0, 0, 0, 0, 0
};


FP_LISTMENU_HEADER fpListMenuData_SDSUMWindow = {
	7,
	"Units of Measurement",
	_SDSUMListBox,
	NULL,
	&headSDSWindow,
	_SDSUMListFrame,
	_SDSUMListParam,
	0
};

FRAMEPAGE_HEADER headSDSUMWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMWindow,
	NULL,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Coordinates
*/
static const GUI_ConstString _SDSUMCListBox[] = {
	"Decimal Degrees\r\n(hddd.ddddd)",
	"Degrees Minutes\r\n(hddd mm.mmm')",
	"Degrees Minutes Seconds\r\n(hddd mm'ss.s\")",
	NULL
};

static const GUI_ConstString _SDSUMCListShort[] = {
	"Dec-Deg",
	"Deg-Min",
	"Deg-Min-Sec",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMCListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMCListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMCWindow = {
	3,
	"Coordinates",
	_SDSUMCListBox,
	_SDSUMCListShort,
	&headSDSUMWindow,
	_SDSUMCListFrame,
	_SDSUMCListParam,
	15
};

FRAMEPAGE_HEADER headSDSUMCWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMCWindow,
	NULL,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Speed/Distance
*/
static const GUI_ConstString _SDSUMSDListBox[] = {
	"Metric",
	"Statute",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMSDListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMSDListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMSDWindow = {
	2,
	"Speed/Distance",
	_SDSUMSDListBox,
	NULL,
	&headSDSUMWindow,
	_SDSUMSDListFrame,
	_SDSUMSDListParam,
	0
};

FRAMEPAGE_HEADER headSDSUMSDWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMSDWindow,
	NULL,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Elevation
*/
static const GUI_ConstString _SDSUMEListBox[] = {
	"Metric",
	"Statute",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMEListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMEListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMEWindow = {
	2,
	"Elevation",
	_SDSUMEListBox,
	NULL,
	&headSDSUMWindow,
	_SDSUMEListFrame,
	_SDSUMEListParam,
	0
};

FRAMEPAGE_HEADER headSDSUMEWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMEWindow,
	NULL,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Height/Weight
*/
static const GUI_ConstString _SDSUMHWListBox[] = {
	"Metric",
	"Statute",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMHWListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMHWListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMHWWindow = {
	2,
	"Height/Weight",
	_SDSUMHWListBox,
	NULL,
	&headSDSUMWindow,
	_SDSUMHWListFrame,
	_SDSUMHWListParam,
	0
};

FRAMEPAGE_HEADER headSDSUMHWWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMHWWindow,
	NULL,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Temperature
*/
static const GUI_ConstString _SDSUMTListBox[] = {
	"Metric",
	"Statute",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMTListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMTListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMTWindow = {
	2,
	"Temperature",
	_SDSUMTListBox,
	NULL,
	&headSDSUMWindow,
	_SDSUMTListFrame,
	_SDSUMTListParam,
	0
};

FRAMEPAGE_HEADER headSDSUMTWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMTWindow,
	NULL,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Heart Rate
*/
static const GUI_ConstString _SDSUMHRListBox[] = {
	"Beats Per Minute (bpm)",
	"% of Max Heart Rate (%hrr)",
	"% of Heart Rate Reserve (%hrm)",
	"Heart Rate Zone (zone)",
	NULL
};

static const GUI_ConstString _SDSUMHRListShort[] = {
	"BPM",
	"%MHR",
	"%HRR",
	"Zone",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMHRListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMHRListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0,
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0,
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMHRWindow = {
	4,
	"Heart Rate",
	_SDSUMHRListBox,
	_SDSUMHRListShort,
	&headSDSUMWindow,
	_SDSUMHRListFrame,
	_SDSUMHRListParam,
	15
};

FRAMEPAGE_HEADER headSDSUMHRWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMHRWindow,
	NULL,
	NULL,
};


/*
	Settings / Device Settings / Units of Measurement / Power
*/
static const GUI_ConstString _SDSUMPListBox[] = {
	"Watts (w)",
	"% of FTP (%ftp)",
	"kiloJoules (kj)",
	"Power Zone (zone)",
	NULL
};

static const GUI_ConstString _SDSUMPListShort[] = {
	"Watts",
	"%FTP",
	"kJ",
	"Zone",
	NULL
};

static FRAMEPAGE_HEADER* _SDSUMPListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
};

static int _SDSUMPListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0,
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0,
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SDSUMPWindow = {
	4,
	"Power",
	_SDSUMPListBox,
	_SDSUMPListShort,
	&headSDSUMWindow,
	_SDSUMPListFrame,
	_SDSUMPListParam,
	0
};

FRAMEPAGE_HEADER headSDSUMPWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SDSUMPWindow,
	NULL,
	NULL,
};


/*
	Settings / Activity Profiles
*/
static const GUI_ConstString _SAPListBox[] = {
  "Enabled:",
  "Name:",
  "Activity Type:",
  "Display Preferences",
  "Recording Preferences",
  "Training Zones",
  "Train Alerts",
  "Equipment",
  "Copy Settings From",
  NULL
};

static FRAMEPAGE_HEADER* _SAPListFrame[] = {
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headSAPDPWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow
};

static int _SAPListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 0, 0, 0, 0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SAPWindow = {
	9,
	"Activity Profile Setting",
	_SAPListBox,
	NULL,
	&headSettingsWindow,
	_SAPListFrame,
	_SAPListParam,
	0
};

FRAMEPAGE_HEADER headSAPWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPWindow,
	NULL,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences 
*/
static const GUI_ConstString _SAPDPListBox[] = {
  "Speed/Pace: ",
  "Activity Data Screen",
  "Activity Pacer",
  "Auto Scroll:",
  "Scroll Speed: ",
  NULL
};

static FRAMEPAGE_HEADER* _SAPDPListFrame[] = {
	&headSAPDPSPWindow,
	&headSAPDPADSWindow,
	&headUnderConstructionWindow,
	&headUnderConstructionWindow,
	&headSAPDPSSWindow
};

static int _SAPDPListParam[] = {
	0,
	0, 
	0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	0
};

FP_LISTMENU_HEADER fpListMenuData_SAPDPWindow = {
	5,
	"Display Preferences",
	_SAPDPListBox,
	NULL,
	&headSAPWindow,
	_SAPDPListFrame,
	_SAPDPListParam,
	0
};

FRAMEPAGE_HEADER headSAPDPWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPDPWindow,
	NULL,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences / Speed/Pace
*/
static const GUI_ConstString _SAPDPSPListBox[] = {
	"Speed",
	"Pace",
	NULL
};

static FRAMEPAGE_HEADER* _SAPDPSPListFrame[] = {
	NULL,
	NULL
};

static int _SAPDPSPListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SAPDPSPWindow = {
	2,
	"Speed/Pace",
	_SAPDPSPListBox,
	NULL,
	&headSAPDPWindow,
	_SAPDPSPListFrame,
	_SAPDPSPListParam,
	0
};

FRAMEPAGE_HEADER headSAPDPSPWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPDPSPWindow,
	NULL,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences / Scroll Speed
*/
static const GUI_ConstString _SAPDPSSListBox[] = {
	"Slow",
	"Medium",
	"Fast",
	NULL
};

static FRAMEPAGE_HEADER* _SAPDPSSListFrame[] = {
	NULL,
	NULL,
	NULL
};

static int _SAPDPSSListParam[] = {
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|1, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0, 
	(BOOLEAN_OPTION_FIELD<<ELEMENT_FIELD_OFFSET)|0
};

FP_LISTMENU_HEADER fpListMenuData_SAPDPSSWindow = {
	3,
	"Scroll Speed",
	_SAPDPSSListBox,
	NULL,
	&headSAPDPWindow,
	_SAPDPSSListFrame,
	_SAPDPSSListParam,
	0
};

FRAMEPAGE_HEADER headSAPDPSSWindow = {
	FRAMEPAGE_LISTMENU_BOOLOPTION,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPDPSSWindow,
	NULL,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences / Activity Data Screen
*/
static const GUI_ConstString _SAPDPADSListBox[] = {
  "Add Screen",
  NULL
};

static FRAMEPAGE_HEADER* _SAPDPADSListFrame[] = {
	&headSAPDPADSASWindow
};

static int _SAPDPADSListParam[] = {
	0
};

FP_LISTMENU_HEADER fpListMenuData_SAPDPADSWindow = {
	1,
	"Activity Data Screens",
	_SAPDPADSListBox,
	NULL,
	&headSAPDPWindow,
	_SAPDPADSListFrame,
	_SAPDPADSListParam,
	0
};

FRAMEPAGE_HEADER headSAPDPADSWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPDPADSWindow,
	NULL,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences / Activity Data Screen / Add Screen
*/
static const GUI_ConstString _SAPDPADSASListBox[] = {
  "New Screen",
  "Copy Last Screen",
  NULL
};

static FRAMEPAGE_HEADER* _SAPDPADSASListFrame[] = {
	&headSAPDPADSASNS1Window,
	&headUnderConstructionWindow
};

static int _SAPDPADSASListParam[] = {
	0, 0
};

FP_LISTMENU_HEADER fpListMenuData_SAPDPADSASWindow = {
	2,
	"Add Screen",
	_SAPDPADSASListBox,
	NULL,
	&headSAPDPADSWindow,
	_SAPDPADSASListFrame,
	_SAPDPADSASListParam,
	0
};

FRAMEPAGE_HEADER headSAPDPADSASWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_SAPDPADSASWindow,
	NULL,
	NULL,
};


/*
	Settings / Activity Profiles / Display Preferences / Activity Data Screen / Add Screen / New Screen
*/
/// we will re-use old data struct from above
FRAMEPAGE_HEADER headSAPDPADSASNS1Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_1Window,
	NULL,
	&headSAPDPADSASNSWindow_NumberEntry,
};

FRAMEPAGE_HEADER headSAPDPADSASNS2Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_2Window,
	NULL,
	&headSAPDPADSASNSWindow_NumberEntry,
};

FRAMEPAGE_HEADER headSAPDPADSASNS3Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_3Window,
	NULL,
	&headSAPDPADSASNSWindow_NumberEntry,
};

FRAMEPAGE_HEADER headSAPDPADSASNS4Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_4Window,
	NULL,
	&headSAPDPADSASNSWindow_NumberEntry,
};

FRAMEPAGE_HEADER headSAPDPADSASNS5Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_5Window,
	NULL,
	&headSAPDPADSASNSWindow_NumberEntry,
};

FRAMEPAGE_HEADER headSAPDPADSASNS6Window = {
	FRAMEPAGE_DATAMODE,
	DataModeWindow,
	cbDataModeWindow,
	NULL,
	2,
	0,
	1,
	(void*)&fpDataModeData_6Window,
	NULL,
	&headSAPDPADSASNSWindow_NumberEntry,
};

/*
	Popup Number entry for
	Settings / Activity Profiles / Display Preferences / Activity Data Screen / Add Screen / New Screen
*/
static const GUI_ConstString _PopupListBox_SAPDPADSASNSNumberEntry[] = {
  "1", "2", "3", "4", "5", "6", NULL
};

/* */
static FRAMEPAGE_HEADER* _PopupListFrame_SAPDPADSASNSNumberEntry[] = {
	&headSAPDPADSASNS1Window, 
	&headSAPDPADSASNS2Window, 
	&headSAPDPADSASNS3Window, 
	&headSAPDPADSASNS4Window, 
	&headSAPDPADSASNS5Window, 
	&headSAPDPADSASNS6Window
};

static int _PopupListParam_SAPDPADSASNSNumberEntry[] = {
	0, 0, 0, 0, 0, 0
};

FP_POPUPLIST_HEADER fpPopupListData_SAPDPADSASNSNumberEntry = {
	6,
	0,
	NULL,
	_PopupListBox_SAPDPADSASNSNumberEntry,
	NULL,
	_PopupListFrame_SAPDPADSASNSNumberEntry,
	_PopupListParam_SAPDPADSASNSNumberEntry,
	0,
};

FRAMEPAGE_HEADER headSAPDPADSASNSWindow_NumberEntry = {
	FRAMEPAGE_POPUP_NUMBERS,
	PopupWindowNumbers,
	cbSAPDPADSASNSPopupWindowNumbers,	///special handling
	NULL,
	0,
	0,
	0,
	(void*)&fpPopupListData_SAPDPADSASNSNumberEntry,
	NULL,
	NULL,
};







/*
	History
*/
static const GUI_ConstString _HistoryListBox[] = {
	"Activity History",
	"Activity Totals", 
	"Location History", 
	NULL
};

static FRAMEPAGE_HEADER* _HistoryListFrame[] = {
	&headHAHWindow, 
	&headHATWindow, 
	&headUnderConstructionWindow
};

static int _HistoryListParam[] = {
	0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_HistoryWindow = {
	3,
	"History",
	_HistoryListBox,
	NULL,
	NULL,
	_HistoryListFrame,
	_HistoryListParam,
	0
};

FRAMEPAGE_HEADER headHistoryWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_HistoryWindow,
	NULL,
	NULL,
};


/*
	History / Activity History
*/
static const GUI_ConstString _HAHListBox[] = {
	"Activities",
	"Totals", 
	"Delete All Activities", 
	NULL
};

static FRAMEPAGE_HEADER* _HAHListFrame[] = {
	&headUnderConstructionWindow, 
	&headHATWindow, 
	&headUnderConstructionWindow
};

static int _HAHListParam[] = {
	0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_HAHWindow = {
	3,
	"Activity History",
	_HAHListBox,
	NULL,
	&headHistoryWindow,
	_HAHListFrame,
	_HAHListParam,
	0
};

FRAMEPAGE_HEADER headHAHWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_HAHWindow,
	NULL,
	NULL,
};


/*
	History / Activity Totals
*/
static const GUI_ConstString _HATListBox[] = {
	"All Activities",
	"Running", 
	"Cycling", 
	"Other", 
	"Reset All Totals", 
	NULL
};

static FRAMEPAGE_HEADER* _HATListFrame[] = {
	&headHAHWindow, 
	&headUnderConstructionWindow, 
	&headUnderConstructionWindow, 
	&headUnderConstructionWindow, 
	&headUnderConstructionWindow
};

static int _HATListParam[] = {
	0, 0, 0, 0, 0
};

FP_LISTMENU_HEADER fpListMenuData_HATWindow = {
	5,
	"Activity Totals",
	_HATListBox,
	NULL,
	&headHistoryWindow,
	_HATListFrame,
	_HATListParam,
	0
};

FRAMEPAGE_HEADER headHATWindow = {
	FRAMEPAGE_LISTMENU,
	ListMenuWindow,
	cbListMenuWindow,
	NULL,
	0,
	0,
	1,
	(void*)&fpListMenuData_HATWindow,
	NULL,
	NULL,
};



/*
	Watch mode
*/
FP_WATCH_HEADER fpWatchData_WatchWindow = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

FRAMEPAGE_HEADER headWatchWindow = {
	FRAMEPAGE_WATCH,
	WatchWindow,
	cbWatchWindow,
	NULL,
	0,
	0,
	1,
	&fpWatchData_WatchWindow,
	NULL,
	NULL,
};


/*
	underconstruction
*/
FRAMEPAGE_HEADER headUnderConstructionWindow = {
	FRAMEPAGE_COMMON,
	UnderConstructionWindow,
	NULL,
	NULL,
	100,
	0,
	1,
	NULL,
	NULL,
	NULL,
};


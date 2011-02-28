#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>


/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "guic.h"
#include "ObjectTest.h"
#if 0
#include "FontGentium.h"
#include "1bpp_icons.h"
#include "4bpp_icons.h"
#include "16164bppIcon.h"

#include "FieldCommon.h"
#include "FieldDataMode.h"
#include "FieldMapMode.h"
#include "FieldWatchMode.h"
#include "FieldInfoMode.h"
#include "FieldSettingMenu.h"
#else
#include "fonts.h"
#include "icons.h"
#include "fields.h"
#endif
#include "StringID.h"





WORD MsgDefaultBtn_datamodes(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        case ID_BUTTON_NEXT:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);

        case ID_BUTTON_BACK:
             if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);

        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);
			
        case ID_BTN_UP_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///watch mode or power down
				///popupOption.PopItemNum = 2;
				///popupOption.pPopTitle = "Options";
				///popupOption.pPrivFrame = scrGetStat();
				///popupOption.pPopItemList->pPopItem1->pPopMsg = "Watch Mode";
				///popupOption.pPopItemList->pPopItem1->pIcon = &I16164_Clock;
				///popupOption.pPopItemList->pPopItem1->pGoFrame = &fhWatchMode_watch;
				///popupOption.pPopItemList->pPopItem2->pPopMsg = "Power Down";
				///popupOption.pPopItemList->pPopItem2->pIcon = &I16164_Abort;
				///popupOption.pPopItemList->pPopItem2->pGoFrame = &fhDeviceMode_poweroff;
				///scrSetStat(&fhDeviceMode_popup);
				///scrCreateInit();
				PPMenuSetUp( 2, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(4,gLanguage), &I16164_Clock, &fhWatchMode_watch );
				PPMenuItem2SetUp( IdGetMString(3,gLanguage), &I16164_Abort, &fhDeviceMode_poweroff );
				scrSetNEXT(&fhDeviceMode_popup);
			}
            return (0);
			
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);
			
        case ID_BTN_DOWN_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///Navigation mode, setting, avtivity data, quick info, history
				///popupOption.PopItemNum = 3;
				///popupOption.pPopTitle = "Options";
				///popupOption.pPrivFrame = scrGetStat();
				///popupOption.pPopItemList->pPopItem1->pPopMsg = "Navigation";
				///popupOption.pPopItemList->pPopItem1->pIcon = &I16164_Compass;
				///popupOption.pPopItemList->pPopItem1->pGoFrame = &fhMapMode_navgation;
				///popupOption.pPopItemList->pPopItem2->pPopMsg = "Quick Info";
				///popupOption.pPopItemList->pPopItem2->pIcon = &I16164_About;
				///popupOption.pPopItemList->pPopItem2->pGoFrame = &fhInfoMode_info;
				///popupOption.pPopItemList->pPopItem3->pPopMsg = "Setting";
				///popupOption.pPopItemList->pPopItem3->pIcon = &Settings_4bpp_16x16;
				///popupOption.pPopItemList->pPopItem3->pGoFrame = &fhSettingMenu_main;
				///scrSetStat(&fhDeviceMode_popup);
				///scrCreateInit();
				PPMenuSetUp( 4, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(5,gLanguage), &I16164_Compass, &fhMapMode_navgation );
				PPMenuItem2SetUp( IdGetMString(6,gLanguage), &I16164_About, &fhInfoMode_info );
				PPMenuItem3SetUp( IdGetMString(7,gLanguage), &I16164_Apply, &fhSettingMenu_main );
				PPMenuItem4SetUp( IdGetMString(8,gLanguage), &I16164_Apply, &fhSettingMenu_main );
				scrSetNEXT(&fhDeviceMode_popup);
			}
            return (0);
			
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{	
			}
            return (0);
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
			}
            return (0);                             // do nothing since it's HW key simulate
	
        default:
            return (1);                 // process by default
    }
}


/*
a= 0x0061, 
b= 0x0062, 
c= 0x0063, 
d= 0x0064, 
e= 0x0065, 
f= 0x0066, 
g= 0x0067, 
h= 0x0068, 
i= 0x0069, 
j= 0x006A, 
k= 0x006B, 
l= 0x006C, 
m= 0x006D, 
n= 0x006E, 
o= 0x006F, 
p= 0x0070, 
q= 0x0071, 
r= 0x0072, 
s= 0x0073, 
t= 0x0074, 
u= 0x0075, 
v= 0x0076, 
w= 0x0077, 
x= 0x0078, 
y= 0x0079, 
z= 0x007A,
*/
const XCHAR Func01Str[] = {	0x006C, 0x0061, 0x0070, 0x0020, 0x0061, 0x0076, 0x0067, 0x0020, 0x0070, 0x0061, 0x0063, 0x0065, // lap avg pace
							0x0000};  
const XCHAR Func02Str[] = {	0x0065, 0x006C, 0x0061, 0x0070, 0x0073, 0x0065, 0x0064, 0x0020, 0x0074, 0x0069, 0x006D, 0x0065, // elapsed time
							0x0000};  
const XCHAR Func03Str[] = {	0x006C, 0x0061, 0x0070, 0x0020, 0x006D, 0x0061, 0x0078, 0x0020, 0x007D, 0x0020, 0x0067, 0x0072, 0x0064, // lap max - grd
							0x0000};
const XCHAR Func04Str[] = {	0x006C, 0x0061, 0x0070, 0x0020, 0x0061, 0x0076, 0x0067, 0x0020, 0x0068, 0x0072, // lap avg hr
							0x0000};  
const XCHAR Func05Str[] = {	0x0064, 0x0069, 0x0073, 0x0074, 0x0061, 0x006E, 0x0063, 0x0065, // distance
							0x0000};  
const XCHAR Func06Str[] = {	0x0065, 0x006C, 0x0065, 0x0076, 0x0061, 0x0074, 0x0069, 0x006F, 0x006E, // elevation
							0x0000};  

							
const XCHAR Unit01Str[] = {	0x002F, 0x006D, 0x0069, // /mi
							0x0000};  
const XCHAR Unit02Str[] = {	0x0025, // %
							0x0000};  
const XCHAR Unit03Str[] = {	0x0062, 0x0070, 0x006D, // bpm
							0x0000};  
const XCHAR Unit04Str[] = {	0x0066, 0x0074, // ft
							0x0000};  

						
/*
/= 0x002F, 
:= 0x003A,
%= 0x0025,
.= 0x002E.
0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039,
*/							
const XCHAR Data01Str[] = {	0x0031, 0x0030, 0x003A, 0x0032, 0x0030, // 10:20
							0x0000};  
const XCHAR Data02Str[] = {	0x0032, 0x0030, 0x003A, 0x0031, 0x0039, 0x003A, 0x0030, 0x0031, // 20:19:01
							0x0000};  
const XCHAR Data03Str[] = {	0x0031, 0x0035, // 15
							0x0000};  
const XCHAR Data04Str[] = {	0x0031, 0x0036, 0x0030, // 160
							0x0000};  
const XCHAR Data05Str[] = {	0x0033, 0x002E, 0x0030, 0x0030, // 3.00
							0x0000};  
const XCHAR Data06Str[] = {	0x0031, 0x0035, 0x0035, // 155
							0x0000};  
							
							
							
void CreateRenoDataSet(WORD wDrawOption)
{

    GOLFree();   // free memory for the objects in the previous linked list and start new list

	SetColor(BLACK);
	ClearDevice();

	CreateDefaultBtn();
	
	switch(wDrawOption)
	{
		case 1:
			///CreateDataSet( 0,  0, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0,  0, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, NULL, Func01Str, Data01Str, Unit01Str);
			break;
		case 2:
			///CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/2, "DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/2, NULL, Func01Str, Data01Str, Unit01Str);
			///CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet2", "Elevation", "155", "ft");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, NULL, Func02Str, Data02Str, NULL);
			break;
		case 3:
		#if 0	///landscape
			CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/2, "DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet2", "Elevation", "155", "ft");
			CreateDataSet( GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet3", "Lap Avg HR", "128", "bpm");
		#else
			///CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3,"DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3, NULL, Func03Str, Data03Str, Unit02Str);
			///CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet2", "Elevation", "155", "ft");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, NULL, Func02Str, Data02Str, NULL);
			///CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet3", "Lap Avg HR", "128", "bpm");
			CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT), NULL, Func04Str, Data04Str, Unit03Str);
		#endif
			break;
		case 4:
		#if 0	///landscape
			CreateDataSet( 0,  0, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, "DataSet1", "Time", "23:23", "24h");
			CreateDataSet( GetMaxX()/2, 0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/2, "DataSet2", "Distance", "3.12", "mi");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet3", "Elevation", "155", "ft");
			CreateDataSet( GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet4", "Lap Avg HR", "128", "bpm");
		#else
			///CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3,"DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3, NULL, Func03Str, Data03Str, Unit02Str);
			///CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet2", "Elevation", "155", "ft");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, NULL, Func02Str, Data02Str, NULL);
			///CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet3", "Lap Avg HR", "128", "bpm");
			CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT), NULL, Func04Str, Data04Str, Unit03Str);
			///CreateDataSet( GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet4", "Lap Avg HR", "128", "bpm");
			CreateDataSet( GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT), NULL, Func01Str, Data01Str, Unit01Str);
		#endif
			break;
		case 5:
			///CreateDataSet( 0, 0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3, "DataSet1", "Time", "23:23", "24h");
			///CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet2", "Elevation", "155", "ft");
			///CreateDataSet( GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet3", "Lap Avg HR", "128", "bpm");
			///CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX()/2, GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet4", "Distance", "3.12", "mi");
			///CreateDataSet( GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet5", "Avg HR", "88", "bpm");
			CreateDataSet( 0, 0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3, NULL, Func03Str, Data03Str, Unit02Str);
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, NULL, Func02Str, Data02Str, NULL);
			CreateDataSet( GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, NULL, Func04Str, Data04Str, Unit03Str);
			CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX()/2, GetMaxY()-DEFAULTBTN_HEIGHT, NULL, Func01Str, Data01Str, Unit01Str);
			CreateDataSet( GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, NULL, Func05Str, Data05Str, Unit01Str);
			break;
		case 6:
			///CreateDataSet( 0, 0, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, "DataSet1", "Time", "23:23", "24h");
			///CreateDataSet( GetMaxX()/2, 0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3, "DataSet2", "Distance", "3.12", "mi");
			///CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet3", "Elevation", "155", "ft");
			///CreateDataSet( GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet4", "Lap Avg HR", "128", "bpm");
			///CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX()/2, GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet5", "Distance", "3.12", "mi");
			///CreateDataSet( GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet6", "Avg HR", "88", "bpm");
			CreateDataSet( 0, 0, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, NULL, Func03Str, Data03Str, Unit02Str);
			CreateDataSet( GetMaxX()/2, 0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3, NULL, Func02Str, Data02Str, NULL);
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, NULL, Func04Str, Data04Str, Unit03Str);
			CreateDataSet( GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, NULL, Func01Str, Data01Str, Unit01Str);
			CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX()/2, GetMaxY()-DEFAULTBTN_HEIGHT, NULL, Func05Str, Data05Str, Unit01Str);
			CreateDataSet( GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, NULL, Func06Str, Data06Str, Unit04Str);			
			break;			
		default:
			///CreateDataSet( 0,  0, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0,  0, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, NULL, Func01Str, Data01Str, Unit01Str);
			break;
	}
	
	updateLastView( scrGetStat() );	///update last view
}



WORD MsgRenoDataSet_one(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}

WORD MsgRenoDataSet_oneDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_six);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_six);
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_two);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_two);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///record
			}
			return (0); 
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///reset
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///lap
			}
			return (0); 
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///mark-next
			}
			return (0); 

        default:
            return (MsgDefaultBtn_datamodes(objMsg, pObj, pMsg));                 // process by default
	}
}


WORD MsgRenoDataSet_two(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}


WORD MsgRenoDataSet_twoDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {	
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_one);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_one);
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_three);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_three);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///record
			}
			return (0); 
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///reset
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///lap
			}
			return (0); 
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///mark-next
			}
			return (0); 

        default:
            return (MsgDefaultBtn_datamodes(objMsg, pObj, pMsg));                 // process by default
    }
}


WORD MsgRenoDataSet_three(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}


WORD MsgRenoDataSet_threeDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {	
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_two);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_two);
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_four);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_four);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///record
			}
			return (0); 
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///reset
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///lap
			}
			return (0); 
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///mark-next
			}
			return (0); 

        default:
            return (MsgDefaultBtn_datamodes(objMsg, pObj, pMsg));                 // process by default
    }
}


WORD MsgRenoDataSet_four(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}


WORD MsgRenoDataSet_fourDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {	
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_three);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_three);
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_five);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_five);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///record
			}
			return (0); 
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///reset
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///lap
			}
			return (0); 
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///mark-next
			}
			return (0); 

        default:
            return (MsgDefaultBtn_datamodes(objMsg, pObj, pMsg));                 // process by default
    }
}


WORD MsgRenoDataSet_five(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}


WORD MsgRenoDataSet_fiveDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {	
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_four);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_four);
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_six);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_six);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///record
			}
			return (0); 
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///reset
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///lap
			}
			return (0); 
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///mark-next
			}
			return (0); 

        default:
            return (MsgDefaultBtn_datamodes(objMsg, pObj, pMsg));                 // process by default
    }
}


WORD MsgRenoDataSet_six(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {
        default:
            return 1; 							// process by default
    }
}


WORD MsgRenoDataSet_sixDefaultBtn(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG *pMsg)
{
    switch(GetObjID(pObj))
    {	
		///case ID_BUTTON: here, if you want different key response.
        case ID_BTN_UP:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_five);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_five);
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///scrSetStat(&fhDataMode_one);
				///scrCreateInit();
				scrSetNEXT(&fhDataMode_one);
			}
			return (0); 
        case ID_BTN_ENTER:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///record
			}
			return (0); 
        case ID_BTN_ENTER_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///reset
			}
			return (0); 
        case ID_BTN_EXIT:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///lap
			}
			return (0); 
        case ID_BTN_EXIT_HOLD:
            if(objMsg == BTN_MSG_RELEASED)
			{
				///mark-next
			}
			return (0); 

        default:
            return (MsgDefaultBtn_datamodes(objMsg, pObj, pMsg));                 // process by default
    }
}



FRAME_HEADER fhDataMode_one = {
	MsgRenoDataSet_one,
	CreateRenoDataSet,
	MsgRenoDataSet_oneDefaultBtn,
	1,		///no option
	0,
	0
};


FRAME_HEADER fhDataMode_two = {
	MsgRenoDataSet_two,
	CreateRenoDataSet,
	MsgRenoDataSet_twoDefaultBtn,
	2,		///no option
	0,
	0
};


FRAME_HEADER fhDataMode_three = {
	MsgRenoDataSet_three,
	CreateRenoDataSet,
	MsgRenoDataSet_threeDefaultBtn,
	3,		///no option
	0,
	0
};


FRAME_HEADER fhDataMode_four = {
	MsgRenoDataSet_four,
	CreateRenoDataSet,
	MsgRenoDataSet_fourDefaultBtn,
	4,		///no option
	0,
	0
};


FRAME_HEADER fhDataMode_five = {
	MsgRenoDataSet_five,
	CreateRenoDataSet,
	MsgRenoDataSet_fiveDefaultBtn,
	5,		///no option
	0,
	0
};


FRAME_HEADER fhDataMode_six = {
	MsgRenoDataSet_six,
	CreateRenoDataSet,
	MsgRenoDataSet_sixDefaultBtn,
	6,		///no option
	0,
	0
};


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
				PPMenuSetUp( 2, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(4,gLanguage), &I16164_Clock, &fhWatchMode_watch );
				{	///setup for fhDeviceMode_popask
					///PPAskSetUp( 2, Ask02Str, scrGetStat() );
					PPAskSetUp( 2, NULL, scrGetStat() );
					PPAskItem1SetUp( Ask03Str, NULL, &fhDeviceMode_poweroff );
					PPAskItem2SetUp( Ask04Str, NULL, scrGetStat() );
				}
				///PPMenuItem2SetUp( IdGetMString(3,gLanguage), &I16164_Abort, &fhDeviceMode_poweroff );
				PPMenuItem2SetUp( IdGetMString(3,gLanguage), &I16164_Abort, &fhDeviceMode_popask );
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
				PPMenuSetUp( 4, IdGetMString(1,gLanguage), scrGetStat() );
				PPMenuItem1SetUp( IdGetMString(5,gLanguage), &I16164_Compass, &fhMapMode_navgation );
				PPMenuItem2SetUp( IdGetMString(6,gLanguage), &I16164_About, &fhInfoMode_info );
				PPMenuItem3SetUp( IdGetMString(7,gLanguage), &I16164_Apply, &fhSettingMenu_main );
				PPMenuItem4SetUp( IdGetMString(8,gLanguage), &I16164_Apply, &fhHistoryMenu_main );
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


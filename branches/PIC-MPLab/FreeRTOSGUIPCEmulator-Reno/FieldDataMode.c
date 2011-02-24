#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>


/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "guic.h"
#include "ObjectTest.h"
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
				popupOption.PopItemNum = 2;
				popupOption.pPopTitle = "Options";
				popupOption.pPrivFrame = scrGetStat();
				popupOption.pPopItemList->pPopItem1->pPopMsg = "Watch Mode";
				popupOption.pPopItemList->pPopItem1->pIcon = &I16164_Clock;
				popupOption.pPopItemList->pPopItem1->pGoFrame = &fhWatchMode_watch;
				popupOption.pPopItemList->pPopItem2->pPopMsg = "Power Down";
				popupOption.pPopItemList->pPopItem2->pIcon = &I16164_Abort;
				popupOption.pPopItemList->pPopItem2->pGoFrame = &fhDeviceMode_poweroff;
				scrSetStat(&fhDeviceMode_popup);
				scrCreateInit();
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
				popupOption.PopItemNum = 3;
				popupOption.pPopTitle = "Options";
				popupOption.pPrivFrame = scrGetStat();
				popupOption.pPopItemList->pPopItem1->pPopMsg = "Navigation";
				popupOption.pPopItemList->pPopItem1->pIcon = &I16164_Compass;
				popupOption.pPopItemList->pPopItem1->pGoFrame = &fhMapMode_navgation;
				popupOption.pPopItemList->pPopItem2->pPopMsg = "Quick Info";
				popupOption.pPopItemList->pPopItem2->pIcon = &I16164_About;
				popupOption.pPopItemList->pPopItem2->pGoFrame = &fhInfoMode_info;
				popupOption.pPopItemList->pPopItem3->pPopMsg = "Setting";
				popupOption.pPopItemList->pPopItem3->pIcon = &Settings_4bpp_16x16;
				popupOption.pPopItemList->pPopItem3->pGoFrame = &fhSettingMenu_main;

				scrSetStat(&fhDeviceMode_popup);
				scrCreateInit();
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
			CreateDataSet( 0,  0, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet1", "Time", "23:23", "24h");
			break;
		case 2:
			CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/2, "DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet2", "Elevation", "155", "ft");
			break;
		case 3:
		#if 0	///landscape
			CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/2,"DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet2", "Elevation", "155", "ft");
			CreateDataSet( GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet3", "Lap Avg HR", "128", "bpm");
		#else
			CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3,"DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet2", "Elevation", "155", "ft");
			CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet3", "Lap Avg HR", "128", "bpm");
		#endif
			break;
		case 4:
		#if 0	///landscape
			CreateDataSet( 0,  0, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, "DataSet1", "Time", "23:23", "24h");
			CreateDataSet( GetMaxX()/2, 0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/2, "DataSet2", "Distance", "3.12", "mi");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet3", "Elevation", "155", "ft");
			CreateDataSet( GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/2, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet4", "Lap Avg HR", "128", "bpm");
		#else
			CreateDataSet( 0,  0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3,"DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet2", "Elevation", "155", "ft");
			CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet3", "Lap Avg HR", "128", "bpm");
			CreateDataSet( GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT), "DataSet4", "Lap Avg HR", "128", "bpm");
		#endif
			break;
		case 5:
			CreateDataSet( 0, 0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3, "DataSet1", "Time", "23:23", "24h");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet2", "Elevation", "155", "ft");
			CreateDataSet( GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet3", "Lap Avg HR", "128", "bpm");
			CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX()/2, GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet4", "Distance", "3.12", "mi");
			CreateDataSet( GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet5", "Avg HR", "88", "bpm");
			break;
		case 6:
			CreateDataSet( 0, 0, GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, "DataSet1", "Time", "23:23", "24h");
			CreateDataSet( GetMaxX()/2, 0, GetMaxX(), (GetMaxY()-DEFAULTBTN_HEIGHT)/3, "DataSet2", "Distance", "3.12", "mi");
			CreateDataSet( 0, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet3", "Elevation", "155", "ft");
			CreateDataSet( GetMaxX()/2, (GetMaxY()-DEFAULTBTN_HEIGHT)/3, GetMaxX(), ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, "DataSet4", "Lap Avg HR", "128", "bpm");
			CreateDataSet( 0, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX()/2, GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet5", "Distance", "3.12", "mi");
			CreateDataSet( GetMaxX()/2, ((GetMaxY()-DEFAULTBTN_HEIGHT)/3)*2, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet6", "Avg HR", "88", "bpm");
			break;			
		default:
			CreateDataSet( 0,  0, GetMaxX(), GetMaxY()-DEFAULTBTN_HEIGHT, "DataSet1", "Time", "23:23", "24h");
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
				scrSetStat(&fhDataMode_six);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhDataMode_two);
				scrCreateInit();
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
				scrSetStat(&fhDataMode_one);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhDataMode_three);
				scrCreateInit();
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
				scrSetStat(&fhDataMode_two);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhDataMode_four);
				scrCreateInit();
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
				scrSetStat(&fhDataMode_three);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhDataMode_five);
				scrCreateInit();
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
				scrSetStat(&fhDataMode_four);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhDataMode_six);
				scrCreateInit();
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
				scrSetStat(&fhDataMode_five);
				scrCreateInit();
			}
			return (0); 
        case ID_BTN_DOWN:
            if(objMsg == BTN_MSG_RELEASED)
			{
				scrSetStat(&fhDataMode_one);
				scrCreateInit();
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


/*

File			:	PMserv_if.h				
Description		:	service interface include file. include this if you want to access PM service.
Initialor		:	span.liu


*/


#ifndef __PMSERV_IF_H__
#define __PMSERV_IF_H__


#include "mFreeRTOSDef.h"
#include "mTypeDef.h"
#include "..\battery\batteryserv_if.h"

///PM command list
typedef enum {
    PMCmd_ENABLE = 1,		/*  */
    PMCmd_DISABLE,			/*  */
	PMCmd_PAUSE,			/*  */
} PMCMD;


xQueueHandle xMicPMGetCmdQueueHandle();
xQueueHandle xMicPMGetStatQueueHandle();

void* pvPMSERV_ServInit(void* pvParameter);
xPMSYSSTAT xMicPMGetPwrSt();
bRET xMicPMSetPwrSt();
xBATTLEVEL xMicPMGetBattLevel();


#endif	///#ifndef __PMSERV_IF_H__



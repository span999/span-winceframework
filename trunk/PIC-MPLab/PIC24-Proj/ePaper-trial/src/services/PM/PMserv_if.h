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
    PMCmd_ENABLE = 1,		/* PM command, enable */
    PMCmd_DISABLE,			/* PM command, disable */
	PMCmd_PAUSE,			/* PM command, pause */
	PMCmd_TOUCH,			/* PM command, touch from anything */
	PMCmd_EVENT,			/* PM command, event */
	PMCmd_SETSYSSTAT_ON,	/* PM command, set system state to ON */
	PMCmd_SETSYSSTAT_SAVE,	/* PM command, set system state to SAVE */
	PMCmd_SETSYSSTAT_OFF,	/* PM command, set system state to OFF */
} PMCMD;


///get the queue handle for command in
xQueueHandle xMicPMGetCmdQueueHandle();
///get the queue handle for status out
xQueueHandle xMicPMGetStatQueueHandle();


///main service initial routine
void* pvPMSERV_ServInit(void* pvParameter);


///power management
xPMSYSSTAT xMicPMGetPwrSt();
bRET xMicPMSetPwrSt( xPMSYSSTAT xSetPMState );


///battery function
xBATTLEVEL xMicPMGetBattLevel();
xBATTLEVEL xMicPMGetBattLevelForce();



#endif	///#ifndef __PMSERV_IF_H__



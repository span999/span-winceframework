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


/* PM command list, all availalbe command for xMicPMGetCmdQueueHandle */	
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


/* System State Timeout table */
typedef struct PMStstReport
{
	xPMSYSSTAT	PMsystemState;
	xPMMODSTAT	PMpowerState;
	xPMSTATTOTBLE PMtimeoutTable;
} xPMSTATREPORT;


///get the queue handle for command in
xQueueHandle xMicPMGetCmdQueueHandle( void );
///get the queue handle for status out
xQueueHandle xMicPMGetStatQueueHandle( void );


///main service initial routine
void* pvPMSERV_ServInit(void* pvParameter);


///power management
xPMSYSSTAT xMicPMGetPwrSt( void );
bRET xMicPMSetPwrSt( xPMSYSSTAT xSetPMState );
void xMicPMtouchit( void );


///service management
bRET xMicServPwrDwn_PM( void );
xPMMODSTAT xMicServInPwrSt_PM( void );


///battery function
xBATTLEVEL xMicPMGetBattLevel( void );
xBATTLEVEL xMicPMGetBattLevelForce( void );



#endif	///#ifndef __PMSERV_IF_H__



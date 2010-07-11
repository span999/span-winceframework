/*

File			:	PMserv_if.c				
Description		:	service interface body file. PM service interface body.
Initialor		:	span.liu


*/

#include <stdio.h>
#include "PMserv_if.h"
#include "mFreeRTOSDef.h"
#include "PMserv_core.h"


#define PM_CMD_QUEUE_SIZE		4	// number of messages queue can contain
#define PM_QUEUE_CMD_SIZE		36	// size of each message

#define PM_STAT_QUEUE_SIZE		2	// number of messages queue can contain
#define PM_QUEUE_STAT_SIZE		sizeof(char)	// size of each message

#define STACK_SIZE_PMSERV		(configMINIMAL_STACK_SIZE * 5)
#define TASK_PRIORITY_PMSERV	(tskIDLE_PRIORITY + 1)

///#define OM_CMD_QUEUE_WAITTICK	portMAX_DELAY
#define PM_CMD_QUEUE_WAITTICK		(5000 / portTICK_RATE_MS)	///5000 ms timeout


///////////////////////////////////////////////////////////////////
// Variables 
///queue for PM system to recieve command
static xQueueHandle hPMSERV_ServCmdQueue;
///queue for PM system to export current status
static xQueueHandle hPMSERV_ServStatQueue;
///task handle for this service
static xTaskHandle hPMSERV_ServTask;

static PMCMD	CmdLoopCurrStat = PMCmd_ENABLE;



///internal functions
static bRET xMicServInit_PM()
{
	bRET bRet = bRET_TRUE;
	
	bRet = ServInit_PM();
	CmdLoopCurrStat = PMCmd_ENABLE;
	
	return bRet;
}


static bRET xMicServDeInit_PM()
{
	bRET bRet = bRET_TRUE;
	
	bRet = ServDeInit_PM();
	
	return bRet;
}


///public functions
bRET xMicServPwrDwn_PM()
{
	bRET bRet = bRET_TRUE;
	
	///need no power down function for servies
	bRet = ServPwrDwn_PM();
	
	return bRet;
}


xPMMODSTAT xMicServInPwrSt_PM()
{
	xPMMODSTAT pmStat;
	if( IsServIdle_PM() )
		pmStat = PMMODSTAT_IDLE;
	else
		pmStat = PMMODSTAT_ACTIVE;
	
	return pmStat;
}


xBATTLEVEL xMicPMGetBattLevel()
{
	xBATTLEVEL value = 0;
	
	value = xMicServGetBatteryLevel();	///access battery service

	return value;
}


xQueueHandle xMicPMGetCmdQueueHandle()
{
	return hPMSERV_ServCmdQueue;
}


xQueueHandle xMicPMGetStatQueueHandle()
{
	return hPMSERV_ServStatQueue;
}


static void vPMSERV_taskMain(void* pvParameter)
{
	PMCMD cmd;
	portBASE_TYPE	xQR;
		
	/// in this task we wait for a string to be received and then
	while (1) {
		// wait until a message is received
		xQR = xQueueReceive( xMicPMGetCmdQueueHandle(), &cmd, PM_CMD_QUEUE_WAITTICK );

		if( pdPASS == xQR )
		{	/// here comes cmd...
			if( PMCmd_ENABLE == CmdLoopCurrStat )
			{	///handle the cmd here
			}
			else
			{	///skip the cmd
			}
		}
		else
		{	///it suppose to be time out.. 
			xBATTLEVEL value = 0;
			///check the timed functions...
			printf("PMGetCmdQueue timeout!!!\r\n");
			
			///check battery
			xMicServSetDebug_BAT( xON );	///set battery in debug mode
			value = xMicServGetBatteryLevel();	///access battery service
			xMicServSetDebug_BAT( xOFF );	///set battery in debug mode
			///vPrintString( "PMGetCmdQueue get battery level=%d\r\n", value );
			printf("PMGetCmdQueue get battery level=%d \r\n", value);
		}
	}	
}


void* pvPMSERV_ServInit( void* pvParameter )
{
	printf("about to run pvPMSERV_ServInit!!!\r\n");
	
	// create the queue for PM system to recieve command
	hPMSERV_ServCmdQueue = xQueueCreate(PM_CMD_QUEUE_SIZE, PM_QUEUE_CMD_SIZE);
	// and the queue for PM system to export current status
	hPMSERV_ServStatQueue = xQueueCreate(PM_STAT_QUEUE_SIZE, PM_QUEUE_STAT_SIZE);
	
	if( bRET_FALSE == xMicServInit_PM() )
		return NULL;
	
	// create the PM system task
	xTaskCreate( vPMSERV_taskMain, (signed char*)"PM_SYSTEM", STACK_SIZE_PMSERV, 
		NULL, TASK_PRIORITY_PMSERV, &hPMSERV_ServTask );
		
	return xMicPMGetCmdQueueHandle();
}

/// here we have static function below
///#include "PMserv_core.c"

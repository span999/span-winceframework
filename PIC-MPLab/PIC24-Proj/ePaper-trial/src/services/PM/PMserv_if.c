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
///#define PM_CMD_QUEUE_WAITTICK		(5000 / portTICK_RATE_MS)	///5000 ms timeout
#define PM_CMD_QUEUE_WAITTICK		(1000 / portTICK_RATE_MS)	///1000 ms timeout



///////////////////////////////////////////////////////////////////
// Variables 
///queue for PM system to recieve command
static xQueueHandle hPMSERV_ServCmdQueue;
///queue for PM system to export current status
static xQueueHandle hPMSERV_ServStatQueue;
///task handle for this service
static xTaskHandle hPMSERV_ServTask;

static PMCMD	CmdLoopCurrStat = PMCmd_ENABLE;
static xPMSYSSTAT	PMCurrSysStat = PMSYSSTAT_ON;
static xBATTLEVEL 	BattLevelValue = 0;

static xPMSTATTOTBLE defaultPMstatTOtbl = {
	5,	/// ON TO
	4,	/// SAVE TO
	3,	/// OFF TO
};


///internal functions
static bRET xMicServInit_PM()
{
	bRET bRet = bRET_TRUE;
	
	bRet = ServInit_PM();
	CmdLoopCurrStat = PMCmd_ENABLE;
	PMCurrSysStat = PMSYSSTAT_ON;
	
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
	return BattLevelValue;
}


xBATTLEVEL xMicPMGetBattLevelForce()
{
	xBATTLEVEL value = 0;
	
	value = xMicServGetBatteryLevel();	///access battery service

	return value;
}


xPMSYSSTAT xMicPMGetPwrSt()
{
	return PMCurrSysStat;
}


bRET xMicPMSetPwrSt( xPMSYSSTAT xSetPMState )
{
	bRET bRet = TRUE;
	PMCMD SetCmd = 0;
	
	if( xSetPMState == PMSYSSTAT_ON )
		SetCmd = PMCmd_SETSYSSTAT_ON;
	else
	if( xSetPMState == PMSYSSTAT_SAVE )
		SetCmd = PMCmd_SETSYSSTAT_SAVE;
	else
	if( xSetPMState == PMSYSSTAT_OFF )
		SetCmd = PMCmd_SETSYSSTAT_OFF;
	else
		SetCmd = PMCmd_TOUCH;
	
	xQueueSend( xMicPMGetCmdQueueHandle(), &SetCmd, 0 );
	
	return bRet;
}


xQueueHandle xMicPMGetCmdQueueHandle()
{
	return hPMSERV_ServCmdQueue;
}


xQueueHandle xMicPMGetStatQueueHandle()
{
	return hPMSERV_ServStatQueue;
}


static BOOL PMcmdParser( PMCMD *pPMcmd )
{
	BOOL bRet = TRUE;
	
	if( NULL != pPMcmd )
	{
		switch( *pPMcmd )
		{
			case PMCmd_ENABLE:
				if( PMCmd_ENABLE != CmdLoopCurrStat )
					CmdLoopCurrStat = PMCmd_ENABLE;
				bRet = TRUE;
				break;
		
			case PMCmd_DISABLE:
				if( PMCmd_DISABLE != CmdLoopCurrStat )
					CmdLoopCurrStat = PMCmd_DISABLE;
				bRet = FALSE;
				break;
			
			case PMCmd_PAUSE:
				if( PMCmd_PAUSE != CmdLoopCurrStat )
					CmdLoopCurrStat = PMCmd_PAUSE;
				bRet = FALSE;
				break;
			
			default:
				bRet = TRUE;
				break;
		}///switch
	}
	
	return bRet;
}


static BOOL PMeventHandler()
{
	BOOL bRet = TRUE;
	
	///check 
	
	
	return bRet;
}


static BOOL PMcheckBattery()
{
	BOOL bRet = TRUE;
	xBATTLEVEL value = 0;
			
	///check battery

	///xMicServSetDebug_BAT( xON );	///set battery in debug mode
	value = xMicServGetBatteryLevel();	///access battery service
	///xMicServSetDebug_BAT( xOFF );	///set battery off debug mode

	BattLevelValue = value;	///sync 
	printf("PMGetCmdQueue get battery level=%d \r\n", value);	
	
	return bRet;
}


UINT PMGetCurrStateTO()
{
	UINT uiRet = 1;
	
	if( PMSYSSTAT_ON == PMCurrSysStat )
		uiRet = defaultPMstatTOtbl.xtoSysStat_ON;
	else	
	if( PMSYSSTAT_SAVE == PMCurrSysStat )
		uiRet = defaultPMstatTOtbl.xtoSysStat_SAVE;
	else	
	if( PMSYSSTAT_OFF == PMCurrSysStat )
		uiRet = defaultPMstatTOtbl.xtoSysStat_OFF;
	else
		uiRet = 9999;
	
	printf("PMGetCurrStateTO get TO=%d \r\n", uiRet);		
	return uiRet;
}


BOOL PMSetCurrTOState()
{
	BOOL bRet = TRUE;
	
	if( PMSYSSTAT_ON == PMCurrSysStat )
		PMCurrSysStat = PMSYSSTAT_SAVE;
	else	
	if( PMSYSSTAT_SAVE == PMCurrSysStat )
		PMCurrSysStat = PMSYSSTAT_OFF;
	else	
	if( PMSYSSTAT_OFF == PMCurrSysStat )
		PMCurrSysStat = PMSYSSTAT_ON;
	else
		PMCurrSysStat = PMSYSSTAT_ON;
	
	printf("PMSetCurrTOState timeout !!!! set state to=0x%x \r\n", PMCurrSysStat);
	if( PMSYSSTAT_ON == PMCurrSysStat )
		printf("PMSetCurrTOState set state =PMSYSSTAT_ON\r\n");
	else	
	if( PMSYSSTAT_SAVE == PMCurrSysStat )
		printf("PMSetCurrTOState set state =PMSYSSTAT_SAVE\r\n");
	else	
	if( PMSYSSTAT_OFF == PMCurrSysStat )
		printf("PMSetCurrTOState set state =PMSYSSTAT_OFF\r\n");
	else
		printf("PMSetCurrTOState set state =PMSYSSTAT_ON\r\n");

	return bRet;
}


static void vPMSERV_taskMain(void* pvParameter)
{
	PMCMD cmd;
	portBASE_TYPE	xQR;
	static DWORD	dwTimeCount = 0;
	static UINT		uiStatTOCount = 0;
		
	/// in this task we wait for a string to be received and then
	while (1) {
		/// wait until a command message is received
		xQR = xQueueReceive( xMicPMGetCmdQueueHandle(), &cmd, PM_CMD_QUEUE_WAITTICK );

		if( pdPASS == xQR )
		{	/// here comes cmd...
			uiStatTOCount = 0;	///reset timer, if any command
		#if 0	
			if( PMCmd_TOUCH == cmd )
				uiStatTOCount = 0;	///reset timer
			else
		#endif	
			if( PMcmdParser( &cmd ) )
				PMeventHandler();
		}
		else
		{	///it suppose to be time out.. 

			///check the timed functions...
			printf("PMGetCmdQueue timeout!!!\r\n");
			dwTimeCount++;
			uiStatTOCount++;
			
			if( uiStatTOCount > PMGetCurrStateTO() )		///check system state timeout
			{	///system state timeout! change state
				PMSetCurrTOState();
				uiStatTOCount = 0;
			}
			
			if( 0 == dwTimeCount % 5 )		///every five seconds.
				PMcheckBattery();
				
				
		}
	}	
}


void* pvPMSERV_ServInit( void* pvParameter )
{
	printf("about to launch pvPMSERV_ServInit service routine !!!\r\n");
	
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

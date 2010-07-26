/*

File			:	PMserv_if.c				
Description		:	service interface body file. PM service interface body.
Initialor		:	span.liu


*/

#include <stdio.h>
#include "PMserv_if.h"
#include "mFreeRTOSDef.h"
#include "PMserv_core.h"
#include "..\Toolbox\Toolbox.h"


#define NMEA_TEST

#ifdef NMEA_TEST
#include "..\NMEAparser\NMEAparser.h"	///for NMEA test
#endif

#ifdef EPAPER_PIC24_PORT
	///private port
#else
	///release port
	#include "MainHandleTask.h"
	extern xQueueHandle ghQueueMainHandleTask;
#endif


#define PM_CMD_QUEUE_SIZE		3	// number of messages queue can contain
#define PM_QUEUE_CMD_SIZE		sizeof(PMCMD)	// size of each message

#define PM_STAT_QUEUE_SIZE		3	// number of messages queue can contain
#define PM_QUEUE_STAT_SIZE		sizeof(xPMSTATREPORT)	// size of each message

///#define STACK_SIZE_PMSERV		(configMINIMAL_STACK_SIZE * 5)
#define INCREASE_MINIMAL_STACK_SIZE		10

#ifdef NMEA_TEST
#define STACK_SIZE_PMSERV_CMD		(configMINIMAL_STACK_SIZE * 3 + INCREASE_MINIMAL_STACK_SIZE * 0)
#define STACK_SIZE_PMSERV_STAT		(configMINIMAL_STACK_SIZE * 4 + INCREASE_MINIMAL_STACK_SIZE * 0)
#else
#define STACK_SIZE_PMSERV_CMD		(configMINIMAL_STACK_SIZE * 2 + INCREASE_MINIMAL_STACK_SIZE * 0)
#define STACK_SIZE_PMSERV_STAT		(configMINIMAL_STACK_SIZE * 3 + INCREASE_MINIMAL_STACK_SIZE * 0)
#endif

#define TASK_PRIORITY_PMSERV	(tskIDLE_PRIORITY + 1)

///#define OM_CMD_QUEUE_WAITTICK	portMAX_DELAY
///#define PM_CMD_QUEUE_WAITTICK		(5000 / portTICK_RATE_MS)	///5000 ms timeout
#define PM_CMD_QUEUE_WAITTICK		(1000 / portTICK_RATE_MS)	///1000 ms timeout


typedef struct {
	DWORD	dwTimeCount;
	UINT	uiStatTOCount;
} PM_TIMER_DATA;


///////////////////////////////////////////////////////////////////
// Variables 

static xPMSTATTOTBLE defaultPMstatTOtbl = {
	TRUE,
	8,	/// ON TO
	20,	/// SAVE TO
	3,	/// OFF TO
};

static xPMSTATTOTBLE newPMstatTOtbl = {
	FALSE,
	8,	/// ON TO
	20,	/// SAVE TO
	3,	/// OFF TO
};

///queue for PM system to recieve command
static xQueueHandle hPMSERV_ServCmdQueue;
///queue for PM system to export current status
static xQueueHandle hPMSERV_ServStatQueue;
///task handle for this service
static xTaskHandle hPMSERV_ServTaskCmd;
static xTaskHandle hPMSERV_ServTaskState;

///state control for PM cmd parser
static PMCMD	CmdLoopCurrStat = PMCmd_ENABLE;
///state control for PM system state
static xPMSYSSTAT	PMCurrSysStat = PMSYSSTAT_ON;
///latest battery level
static xBATTLEVEL 	BattLevelValue = 0;

///timer data structure
static PM_TIMER_DATA pmTimeData;
///current TO table
static xPMSTATTOTBLE* pThisPMstatTOtbl = &defaultPMstatTOtbl;




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


static BOOL PMSendCmd( PMCMD SetCmd )
{
	BOOL bRet = TRUE;

	SPPRINTF("PMSendCmd send cmd =%d\r\n", SetCmd);
	if( pdPASS != xQueueSend( xMicPMGetCmdQueueHandle(), &SetCmd, 0 ) )
	///if( pdPASS != xQueueSendToBack( xMicPMGetCmdQueueHandle(), &SetCmd, 0 ) )
		bRet = FALSE;
	
	return bRet;
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
	
	///xQueueSend( xMicPMGetCmdQueueHandle(), &SetCmd, 0 );
	PMSendCmd( SetCmd );
	
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


void xMicPMtouchit( void )
{
	PMSendCmd( PMCmd_TOUCH );
}


static void PMupdateReport()
{
	xPMSTATREPORT ThisReport;
	
	ThisReport.PMsystemState = xMicPMGetPwrSt();
	ThisReport.PMpowerState = xMicServInPwrSt_PM();
	ThisReport.PMtimeoutTable.bValid = pThisPMstatTOtbl->bValid;
	ThisReport.PMtimeoutTable.xtoSysStat_ON = pThisPMstatTOtbl->xtoSysStat_ON;
	ThisReport.PMtimeoutTable.xtoSysStat_SAVE = pThisPMstatTOtbl->xtoSysStat_SAVE;
	ThisReport.PMtimeoutTable.xtoSysStat_OFF = pThisPMstatTOtbl->xtoSysStat_OFF;
	
	xMicSetProtectedData( xMicPMGetStatQueueHandle(), &ThisReport );
	
	SPPRINTF("PMupdateReport\r\n");
	
}

static BOOL PMsystemGoON()
{
	BOOL bRet = TRUE;

	PMCurrSysStat = PMSYSSTAT_ON;
	SPPRINTF("PMSetCurrTOState set state =PMSYSSTAT_ON\r\n");
	
	return bRet;
}


static BOOL PMsystemGoSAVE()
{
	BOOL bRet = TRUE;

	PMCurrSysStat = PMSYSSTAT_SAVE;
	SPPRINTF("PMSetCurrTOState set state =PMSYSSTAT_SAVE\r\n");

	return bRet;
}


static BOOL PMsystemGoOFF()
{
	BOOL bRet = TRUE;

	PMCurrSysStat = PMSYSSTAT_OFF;
	SPPRINTF("PMSetCurrTOState set state =PMSYSSTAT_OFF\r\n");

	return bRet;
}



/*
	PMcmdParser
	check if we need to handle this command, or just do it right here!

*/
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
				bRet = FALSE;
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
				
			case PMCmd_TOUCH:
			case PMCmd_EVENT:
				bRet = FALSE;
				break;
				
			default:
				bRet = TRUE;
				break;
		}///switch
	}
	
	return bRet;
}


static BOOL PMeventHandler( PMCMD *pPMcmd )
{
	BOOL bRet = TRUE;
	
	///check 
	
	if( NULL != pPMcmd )
	{
		switch( *pPMcmd )
		{
			case PMCmd_ENABLE:
			case PMCmd_DISABLE:
			case PMCmd_PAUSE:
				bRet = FALSE;
				break;
				
			case PMCmd_SETSYSSTAT_ON:
				PMsystemGoON();
				bRet = TRUE;
				break;

			case PMCmd_SETSYSSTAT_SAVE:
				PMsystemGoSAVE();
				bRet = TRUE;
				break;

			case PMCmd_SETSYSSTAT_OFF:
				PMsystemGoOFF();
				bRet = TRUE;
				break;
				
			default:
				bRet = TRUE;
				break;
		}///switch
		PMupdateReport();
	}
	
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

	if( value != BattLevelValue )
	{
		BattLevelValue = value;	///sync 
		bRet = TRUE;
	}	
	else
		bRet = FALSE;
	
	SPPRINTF("PMcheckBattery get %s battery level=%d \r\n", bRet?"changed":"same", value);
	
	return bRet;
}



#ifdef EPAPER_PIC24_PORT
	///private port
#else
	///release port
static int customBattLvSend( xBATTLEVEL value )
{
	int iRet = 0;
	MainTaskData SetData;
	
	SetData.bSenderDevID = BatteryDrvID;
	SetData.ucBatteryLife = value;
	
	if( ghQueueMainHandleTask )
		if( pdPASS != xQueueSend( ghQueueMainHandleTask, &SetData, 0 ) )
		{
			SPPRINTF("customBattLvSend %d fail\r\n", value);
			iRet = (-1);
		}
	else	
		iRet = (-2);

	return iRet;
}	
#endif


static int PMupdateBattLv2User( void )
{
	int iRet = 0;

#ifdef EPAPER_PIC24_PORT
	///private port
#else
	///release port
	customBattLvSend( BattLevelValue );	
#endif

	SPPRINTF("PMupdateBattLv2User level=%d \r\n", BattLevelValue);
	return iRet;
}


static UINT PMGetCurrStateTO()
{
	UINT uiRet = 1;
	
	if( PMSYSSTAT_ON == PMCurrSysStat )
		uiRet = pThisPMstatTOtbl->xtoSysStat_ON;
	else	
	if( PMSYSSTAT_SAVE == PMCurrSysStat )
		uiRet = pThisPMstatTOtbl->xtoSysStat_SAVE;
	else	
	if( PMSYSSTAT_OFF == PMCurrSysStat )
		uiRet = pThisPMstatTOtbl->xtoSysStat_OFF;
	else
		uiRet = 9999;
	
	SPPRINTF("PMGetCurrStateTO get TO=%d \r\n", uiRet);		
	return uiRet;
}


static BOOL PMSetCurrTOState()
{
	BOOL bRet = TRUE;
	PMCMD SetCmd;
	
	if( PMSYSSTAT_ON == PMCurrSysStat )
		SetCmd = PMCmd_SETSYSSTAT_SAVE;
	else	
	if( PMSYSSTAT_SAVE == PMCurrSysStat )
		SetCmd = PMCmd_SETSYSSTAT_OFF;
	else	
	if( PMSYSSTAT_OFF == PMCurrSysStat )
		SetCmd = PMCmd_SETSYSSTAT_ON;
	else
		SetCmd = PMCmd_SETSYSSTAT_ON;
	
	PMSendCmd( SetCmd );

	SPPRINTF("PMSetCurrTOState timeout !!!! set state to=0x%x \r\n", PMCurrSysStat);
	
	return bRet;
}


static void vPMSERV_taskMainCmd(void* pvParameter)
{
	PMCMD cmd;
	portBASE_TYPE	xQR;
///	static DWORD	dwTimeCount = 0;
///	static UINT		uiStatTOCount = 0;
		
	/// in this task we wait for a string to be received and then
	while (1) {
		/// wait until a command message is received
		xQR = xQueueReceive( xMicPMGetCmdQueueHandle(), &cmd, PM_CMD_QUEUE_WAITTICK );

		if( pdPASS == xQR )
		{	/// here comes cmd...
			pmTimeData.uiStatTOCount = 0;	///reset timer, if any command
		#if 0	
			if( PMCmd_TOUCH == cmd )
				pmTimeData.uiStatTOCount = 0;	///reset timer
			else
		#endif	
			if( PMcmdParser( &cmd ) )
				PMeventHandler( &cmd );
		}
		else
		{	///it suppose to be time out.. 

			///check for timeout...
			SPPRINTF("PMGetCmdQueue timeout!!!\r\n");				
		}
	}	
}



#ifdef NMEA_TEST
				const char buf0[] = "$GPRMC,180946.369,A,3926.0580,N,11946.0001,W,1.234,153.8,070106,15.2,E,A*3E\n\0";
				const char buf1[] = "$GPGGA,180946.123,3926.0581,N,11946.0002,W,2,10,1.1,1378.4,M,-22.1,M,,*4C\0";
				const char buf2[] = "$GPGSA,A,3,02,04,05,07,09,17,24,26,28,29,,,2.1,1.1,1.8*30\0";
				const char buf3[] = "$GPRMC,180948.456,A,3926.0582,N,11946.0003,W,34.5678,153.8,070106,15.2,E,A*36\0";
				const char buf4[] = "$GPGGA,180948.789,3926.0583,N,11946.0004,W,2,10,1.1,1378.4,M,-22.1,M,,*44\0";
				const char buf5[] = "$GPGSA,A,3,02,04,05,07,09,17,24,26,28,29,,,2.1,1.1,1.8*30\0";
				const char buf6[] = "$GPRMC,180950.234,A,3926.0584,N,11946.0005,W,3456.3435,153.8,070106,15.2,E,A*3F\0";
				const char buf7[] = "$GPGGA,180950.567,3926.0585,N,11946.0006,W,2,10,1.1,1378.3,M,-22.1,M,,*4A\0";
				const char buf8[] = "$GPGSA,A,3,02,04,05,07,09,17,24,26,28,29,,,2.1,1.1,1.8*30\0";
				const char buf9[] = "$GPRMC,180952.789,A,3926.0586,N,11946.0007,W,4444.4444,153.8,070106,15.2,E,A*3D\0";
#endif

static void vPMSERV_taskMainState(void* pvParameter)
{
#define PM_TIMER_BASE			1000
#define PM_BATTERY_POLL			5000
#define PM_BATERRY_COUNT		(PM_BATTERY_POLL/PM_TIMER_BASE)
	
	/// in this task we wait for a string to be received and then
	while (1) {
		/// wait until specified timeout
		vMicSLEEPuntil( PM_TIMER_BASE );
		
		///if( PMCurrSysStat == PMSYSSTAT_ON )
		if( 1 )
		{	///it suppose to be time out.. 

			///check the timed functions...
			SPPRINTF("vPMSERV_taskMainState timeout!!!\r\n");
			pmTimeData.dwTimeCount++;
			pmTimeData.uiStatTOCount++;
			
			if( pmTimeData.uiStatTOCount > PMGetCurrStateTO() )		///check system state timeout
			{	///system state timeout! change state
				PMSetCurrTOState();
				pmTimeData.uiStatTOCount = 0;
			}
			
			if( 0 == pmTimeData.dwTimeCount % PM_BATERRY_COUNT )		///every five seconds.
				if( PMcheckBattery() )
					PMupdateBattLv2User();

#ifdef NMEA_TEST					
			if(1)	///for NMEA test 
			{
				gps_NMEA_session	nmeaSession;
				
				const char *pbuf[10] = { buf0, buf1, buf2, buf3, buf4, buf5, buf6, buf7, buf8, buf9 };
				
				SPPRINTF("NMEA test>>>0x%x %s 0x%x \r\n", pbuf[(pmTimeData.dwTimeCount%10)], pbuf[(pmTimeData.dwTimeCount%10)], &nmeaSession );
				NMEAparser( pbuf[(pmTimeData.dwTimeCount%10)], &nmeaSession );
				Handle_NMEA_session( &nmeaSession );
			}
#endif	///#ifdef NMEA_TEST				
		}
	}	
}



void* pvPMSERV_ServInit( void* pvParameter )
{
	SPPRINTF("about to launch pvPMSERV_ServInit service routine !!!\r\n");
	
	// create the queue for PM system to recieve command
	hPMSERV_ServCmdQueue = xQueueCreate(PM_CMD_QUEUE_SIZE, PM_QUEUE_CMD_SIZE);
	// and the queue for PM system to export current status
	hPMSERV_ServStatQueue = xQueueCreate(PM_STAT_QUEUE_SIZE, PM_QUEUE_STAT_SIZE);
	
	if( bRET_FALSE == xMicServInit_PM() )
		return NULL;
	
	// create the PM system task
	xTaskCreate( vPMSERV_taskMainCmd, (signed char*)"PM_SYSTEM_CMD", STACK_SIZE_PMSERV_CMD, 
		NULL, TASK_PRIORITY_PMSERV, &hPMSERV_ServTaskCmd );

	xTaskCreate( vPMSERV_taskMainState, (signed char*)"PM_SYSTEM_STATE", STACK_SIZE_PMSERV_STAT, 
		NULL, TASK_PRIORITY_PMSERV, &hPMSERV_ServTaskState );
		
	return xMicPMGetCmdQueueHandle();
}

/// here we have static function below
///#include "PMserv_core.c"

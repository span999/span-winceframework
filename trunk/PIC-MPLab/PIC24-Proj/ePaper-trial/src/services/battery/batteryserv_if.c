/*

File			:	batteryserv_if.c				
Description		:	service interface body file. battery service interface body.
Initialor		:	span.liu


*/

#include <stdio.h>
#include "batteryserv_if.h"
#include "mFreeRTOSDef.h"
#include "batteryserv_core.h"


static xOnOff DebugModeCtrl = xOFF;

bRET xMicServInit_BAT()
{
	bRET bRet = bRET_TRUE;
	
	bRet = MicServInit();
	
	return bRet;
}


bRET xMicServDeInit_BAT()
{
	bRET bRet = bRET_TRUE;
	
	bRet = MicServDeInit();
	
	return bRet;
}


bRET xMicServPwrDwn_BAT()
{
	bRET bRet = bRET_TRUE;
	
	///need no power down function for servies
	
	return bRet;
}


xPMMODSTAT xMicServInPwrSt_BAT()
{
	xPMMODSTAT pmStat;
	if( MicIsServIdle() )
		pmStat = PMMODSTAT_IDLE;
	else
		pmStat = PMMODSTAT_ACTIVE;
	
	return pmStat;
}


xBATTLEVEL xMicServGetBatteryLevel()
{
	xBATTLEVEL value = 0;
	
	if( xOFF == DebugModeCtrl )
		value = MicServGetBattLv();
	else
		value = 88;
	
	printf("xMicServGetBatteryLevel~~\r\n");
	
	return value;
}


xOnOff xMicServSetDebug_BAT( xOnOff do_sets )
{
	xOnOff xOnOffStat = xON;

	DebugModeCtrl = do_sets;
	xOnOffStat = DebugModeCtrl;
	if( xON == DebugModeCtrl )
		printf("xMicServSetDebug_BAT set as ON~~\r\n");
	else
		printf("xMicServSetDebug_BAT set as OFF~~\r\n");
	
	return xOnOffStat;
}


/// here we have static function below
#include "batteryserv_core.c"

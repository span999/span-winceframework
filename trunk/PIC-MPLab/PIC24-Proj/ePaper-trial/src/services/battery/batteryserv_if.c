/*

File			:	batteryserv_if.c				
Description		:	service interface body file. battery service interface body.
Initialor		:	span.liu


*/


#include "batteryserv_if.h"
#include "mFreeRTOSDef.h"
#include "batteryserv_core.h"




bRET xMicServInit_BAT()
{
	bRET bRet = bRET_TRUE;
	
	bRet = MicServInit();
	
	return bRet;
}


bRET xMicServModDeInit_BAT()
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
	
	value = MicServGetBattLv();

	return value;
}


/// here we have static function below
#include "batteryserv_core.c"

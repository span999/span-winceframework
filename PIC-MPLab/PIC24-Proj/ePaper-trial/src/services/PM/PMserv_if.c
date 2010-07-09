/*

File			:	batteryserv_if.c				
Description		:	service interface body file. battery service interface body.
Initialor		:	span.liu


*/


#include "PMserv_if.h"
#include "mFreeRTOSDef.h"
#include "PMserv_core.h"




bRET xMicServInit_PM()
{
	bRET bRet = bRET_TRUE;
	
	bRet = MicServInit();
	
	return bRet;
}


bRET xMicServDeInit_PM()
{
	bRET bRet = bRET_TRUE;
	
	bRet = MicServDeInit();
	
	return bRet;
}


bRET xMicServPwrDwn_PM()
{
	bRET bRet = bRET_TRUE;
	
	///need no power down function for servies
	
	return bRet;
}


xPMMODSTAT xMicServInPwrSt_PM()
{
	xPMMODSTAT pmStat;
	if( MicIsServIdle() )
		pmStat = PMMODSTAT_IDLE;
	else
		pmStat = PMMODSTAT_ACTIVE;
	
	return pmStat;
}


xBATTLEVEL xMicPMGetBattLevel()
{
	xBATTLEVEL value = 0;
	
	value = xMicServGetBatteryLevel();

	return value;
}


/// here we have static function below
#include "PMserv_core.c"

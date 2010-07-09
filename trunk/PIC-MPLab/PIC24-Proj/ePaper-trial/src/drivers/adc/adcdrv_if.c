/*

File			:	adcdrv_if.c				
Description		:	driver interface body file. ADC driver interface body.
Initialor		:	span.liu


*/


#include "adcdrv_if.h"
#include "mFreeRTOSDef.h"
#include "adcdrv_core.h"




bRET xMicModInit_ADC()
{
	bRET bRet = bRET_TRUE;
	
	bRet = MicModInit();
	
	return bRet;
}


bRET xMicModDeInit_ADC()
{
	bRET bRet = bRET_TRUE;
	
	bRet = MicModDeInit();
	
	return bRet;
}


bRET xMicModPwrDwn_ADC()
{
	bRET bRet = bRET_TRUE;
	
	bRet = MicModPwrDwn();
	
	return bRet;
}


xPMMODSTAT xMicModInPwrSt_ADC()
{
	xPMMODSTAT pmStat;
	if( MicIsModIdle() )
		pmStat = PMMODSTAT_IDLE;
	else
		pmStat = PMMODSTAT_ACTIVE;
	
	return pmStat;
}


xADCval xMicModGetADCValue()
{
	xADCval value = 0;
	
	value = MicModGetADC();

	return value;
}


/// here we have static function below
#include "adcdrv_core.c"

/*

File			:	adcdrv_if.c				
Description		:	driver interface body file. ADC driver interface body.
Initialor		:	span.liu


*/


#include "adcdrv_if.h"
#include "mFreeRTOSDef.h"




bRET xMicModInit_ADC()
{
	bRET bRet = bRET_TRUE;
	
	
	return bRet;
}


bRET xMicModDeInit_ADC()
{
	bRET bRet = bRET_TRUE;
	
	
	return bRet;
}


bRET xMicModPwrDwn_ADC()
{
	bRET bRet = bRET_TRUE;
	
	
	return bRet;
}


xPMMODSTAT xMicModInPwrSt_ADC()
{
	xPMMODSTAT pmStat = PMMODSTAT_IDLE;
	
	
	return pmStat;
}


xADCval xMicModGetADCValue()
{
	xADCval value = 0;

	return value;
}




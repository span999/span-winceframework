/*

File			:	adcdrv_if.c				
Description		:	driver interface body file. ADC driver interface body.
Initialor		:	span.liu


*/


#include <stdio.h>
#include "adcdrv_if.h"
#include "mFreeRTOSDef.h"
#include "adcdrv_core.h"
#include "..\..\services\Toolbox\Toolbox.h"


/* flag for debug mode */
static xOnOff DebugModeCtrl = xOFF;

bRET xMicModInit_ADC()
{
	bRET bRet = bRET_TRUE;
	
	bRet = ModInit_ADC();
	
	return bRet;
}


bRET xMicModDeInit_ADC()
{
	bRET bRet = bRET_TRUE;
	
	bRet = ModDeInit_ADC();
	
	return bRet;
}


bRET xMicModPwrDwn_ADC()
{
	bRET bRet = bRET_TRUE;
	
	bRet = ModPwrDwn_ADC();
	
	return bRet;
}


xPMMODSTAT xMicModInPwrSt_ADC()
{
	xPMMODSTAT pmStat;
	if( IsModIdle_ADC() )
		pmStat = PMMODSTAT_IDLE;
	else
		pmStat = PMMODSTAT_ACTIVE;
	
	return pmStat;
}


xADCval xMicModGetADCValue()
{
	xADCval value = 0;

	if( xOFF == DebugModeCtrl )	
		value = ModGetADC_ADC();
	else	
		value = ModGetADCDBG_ADC();
	
	SPPRINTF("xMicModGetADCValue = %d\r\n", value);	
	return value;
}


xOnOff xMicModSetDebug_ADC( xOnOff do_sets )
{
	xOnOff xOnOffStat = xON;

	DebugModeCtrl = do_sets;
	xOnOffStat = DebugModeCtrl;
	if( xON == DebugModeCtrl )
		SPPRINTF("xMicModSetDebug_ADC set as ON~~\r\n");
	else
		SPPRINTF("xMicModSetDebug_ADC set as OFF~~\r\n");
	
	return xOnOffStat;
}


/// here we have static function below
///#include "adcdrv_core.c"

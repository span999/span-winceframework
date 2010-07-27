/*

File			:	batteryserv_if.c				
Description		:	service interface body file. battery service interface body.
Initialor		:	span.liu


*/

#include <stdio.h>
#include "mFreeRTOSDef.h"
#include "batteryserv_if.h"
#include "batteryserv_core.h"
#include "..\..\drivers\adc\adcdrv_if.h"
#include "..\Toolbox\Toolbox.h"






/* flag for debug mode */
static xOnOff DebugModeCtrl = xOFF;


///internal functions
static INT32 MicServCalcBatteryLevel()
{
	INT32 i32Ret = 0;
	UINT uiRet = 0;
	///TODO: read ADC, and to a battery mapping
	
///	xMicModSetDebug_ADC( xON );		///set battery in debug mode
	uiRet = xMicModGetADCValue();
///	xMicModSetDebug_ADC( xOFF );	///set battery off debug mode
	
	///mapping table??
	i32Ret = ServMappingADC2BATT_BAT( uiRet );

	return i32Ret;
}


static INT32 MicServGetBattLv()
{
	INT32 i32Ret = 0;
	
	i32Ret = MicServCalcBatteryLevel();
	
	return i32Ret;
}



///public functions
bRET xMicServInit_BAT()
{
	bRET bRet = bRET_TRUE;
	
	bRet = ServInit_BAT();
	
	return bRet;
}


bRET xMicServDeInit_BAT()
{
	bRET bRet = bRET_TRUE;
	
	bRet = ServDeInit_BAT();
	
	return bRet;
}


bRET xMicServPwrDwn_BAT()
{
	bRET bRet = bRET_TRUE;
	
	///need no power down function for servies
	bRet = ServPwrDwn_BAT();
	
	return bRet;
}


xPMMODSTAT xMicServInPwrSt_BAT()
{
	xPMMODSTAT pmStat;
	if( IsServIdle_BAT() )
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
		value = ServGetBattLvDBG_BAT();
	
	SPPRINTF("xMicServGetBatteryLevel~~\r\n");
	
	return value;
}


xOnOff xMicServSetDebug_BAT( xOnOff do_sets )
{
	xOnOff xOnOffStat = xON;

	DebugModeCtrl = do_sets;
	xOnOffStat = DebugModeCtrl;
	if( xON == DebugModeCtrl )
		SPPRINTF("xMicServSetDebug_BAT set as ON~~\r\n");
	else
		SPPRINTF("xMicServSetDebug_BAT set as OFF~~\r\n");
	
	return xOnOffStat;
}



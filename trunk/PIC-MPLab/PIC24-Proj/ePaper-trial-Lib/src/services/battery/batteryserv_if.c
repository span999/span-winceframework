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


static ADC_BATT_MAPS	adcBattTbl[12] =
{
	{  0,  734,  0 },
	{  1,  760,  1 },
	{  2,  786,  2 },
	{  3,  812,  3 },
	{  4,  838,  4 },
	{  5,  864,  5 },
	{  6,  890,  6 },
	{  7,  916,  7 },
	{  8,  942,  8 },
	{  9,  968,  9 },
	{ 10, 9999, 10 },
	{ -1,    0, 10 }
};



/* flag for debug mode */
static xOnOff DebugModeCtrl = xOFF;


///internal functions
static INT32 MicServCalcBatteryLevel()
{
	INT32 i32Ret = 0;
	UINT uiRet = 0;
	///TODO: read ADC, and to a battery mapping
	
#ifdef EPAPER_PIC24_PORT
	///private port
	xMicModSetDebug_ADC( xON );		///set battery in debug mode
	uiRet = xMicModGetADCValue();
	xMicModSetDebug_ADC( xOFF );	///set battery off debug mode
#else
	///release port
	uiRet = xMicModGetADCValue();
#endif

	
	///mapping table??
#if 0	
	i32Ret = ServMappingADC2BATT_BAT( uiRet );
#else
	if( uiRet > 0 )
	{
		int iLoop = 0;
		for( iLoop = 0; iLoop < 15; iLoop++ )
		{
			if( 0 == adcBattTbl[iLoop].adcMAX )	///end of list
				break;
				
			if( uiRet < adcBattTbl[iLoop].adcMAX )
			{
				i32Ret = adcBattTbl[iLoop].level;
				break;
			}
			
			///try next
		}
	}
#endif

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



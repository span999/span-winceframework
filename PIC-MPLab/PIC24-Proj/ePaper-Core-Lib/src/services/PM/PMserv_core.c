/*

File			:	PMserv_core.c		
Description		:	service main body file. PM service core body.
Initialor		:	span.liu


*/

#include "PMserv_core.h"




static BOOL IsIdle = FALSE;

///internal functions
/*
static INT32 MicModServCalcBatteryLevel()
{
	INT32 i32Ret = 0;
	///TODO: read ADC, and to a battery mapping

	i32Ret = xMicModGetADCValue();
	
	///mapping table??
	
	return i32Ret;
}
*/


///common functions
BOOL ServInit_PM()
{
	BOOL bRet = FALSE;
	
	///need no initial for battery
	bRet = TRUE;
	
	return bRet;
}


BOOL ServDeInit_PM()
{
	BOOL bRet = FALSE;
	
	///need no initial for battery
	bRet = TRUE;
	
	return bRet;
}



BOOL ServPwrDwn_PM()
{
	BOOL bRet = FALSE;

	///need no initial for battery
	bRet = TRUE;
	
	return bRet;
}



BOOL IsServIdle_PM()
{
	BOOL bRet = FALSE;
	
	///PM is always in idle since it's a polling / passive driver
	///bRet = TRUE;
	bRet = IsIdle;
	
	return bRet;
}


///UINT ServGetBattLvDBG_BAT()
///{
///	return 88;
///}


void ServIsIdle_PM()
{
	IsIdle = TRUE;
}


void ServIsActive_PM()
{
	IsIdle = FALSE;
}



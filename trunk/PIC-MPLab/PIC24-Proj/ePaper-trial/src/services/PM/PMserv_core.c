/*

File			:	PMserv_core.c		
Description		:	service main body file. battery service core body.
Initialor		:	span.liu


*/

#include "PMserv_core.h"
#include "mFreeRTOSDef.h"



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
static BOOL MicServInit()
{
	BOOL bRet = FALSE;
	
	///need no initial for battery
	bRet = TRUE;
	
	return bRet;
}


static BOOL MicServDeInit()
{
	BOOL bRet = FALSE;
	
	///need no initial for battery
	bRet = TRUE;
	
	return bRet;
}


/*
static BOOL MicServPwrDwn()
{
	BOOL bRet = FALSE;

	///need no initial for battery
	bRet = TRUE;
	
	return bRet;
}
*/


static BOOL MicIsServIdle()
{
	BOOL bRet = FALSE;
	
	///battrey is always in idle since it's a polling / passive driver
	bRet = TRUE;
	
	return bRet;
}




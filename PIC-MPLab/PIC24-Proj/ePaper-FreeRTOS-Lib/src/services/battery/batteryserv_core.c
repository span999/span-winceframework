/*

File			:	batteryserv_core.c		
Description		:	service main body file. battery service core body.
Initialor		:	span.liu


*/

#include "batteryserv_core.h"



static BOOL IsIdle = FALSE;

///internal functions




///common functions
BOOL ServInit_BAT()
{
	BOOL bRet = FALSE;
	
	///need no initial for battery
	bRet = TRUE;
	
	return bRet;
}


BOOL ServDeInit_BAT()
{
	BOOL bRet = FALSE;
	
	///need no initial for battery
	bRet = TRUE;
	
	return bRet;
}



BOOL ServPwrDwn_BAT()
{
	BOOL bRet = FALSE;

	///need no initial for battery
	bRet = TRUE;
	
	return bRet;
}


BOOL IsServIdle_BAT()
{
	BOOL bRet = FALSE;
	
	///battrey is always in idle since it's a polling / passive driver
	///bRet = TRUE;
	bRet = IsIdle;
	
	return bRet;
}


UINT ServGetBattLvDBG_BAT()
{
	return 88;
}


void ServIsIdle_BAT()
{
	IsIdle = TRUE;
}


void ServIsActive_BAT()
{
	IsIdle = FALSE;
}



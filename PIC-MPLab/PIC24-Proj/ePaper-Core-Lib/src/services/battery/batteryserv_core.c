/*

File			:	batteryserv_core.c		
Description		:	service main body file. battery service core body.
Initialor		:	span.liu


*/

#include "batteryserv_core.h"
#include "batteryserv_adcmap.h"


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


#if 0
INT32 ServMappingADC2BATT_BAT( UINT uiADC )
{
	INT32 i32Ret = 0;
	
	i32Ret = (uiADC & 0x7);
	
	return i32Ret;
}
#else
INT32 ServMappingADC2BATT_BAT( UINT uiADC )
{
	INT32 i32Ret = 0;
	
	if( uiADC > 0 )
	{
		int iLoop = 0;
		for( iLoop = 0; iLoop < 15; iLoop++ )
		{
			if( 0 == adcBattTbl[iLoop].adcMAX )	///end of list
				break;
				
			if( uiADC < adcBattTbl[iLoop].adcMAX )
			{
				i32Ret = adcBattTbl[iLoop].level;
				break;
			}
			
			///try next
		}
	}
	
	return i32Ret;
}
#endif


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



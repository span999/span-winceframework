/*

File			:	batteryserv_if.h				
Description		:	service interface include file. include this if you want to access battery service.
Initialor		:	span.liu


*/


#ifndef __PMSERV_IF__
#define __PMSERV_IF__


#include "mTypeDef.h"
#include "..\battery\batteryserv_if.h"


xPMSYSSTAT xMicPMGetPwrSt();
bRET xMicPMSetPwrSt();
xBATTLEVEL xMicPMGetBattLevel();


#endif	///#ifndef __PMSERV_IF__



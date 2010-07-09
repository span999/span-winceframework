/*

File			:	batteryserv_core.h				
Description		:	service main body include file. include for function core.
Initialor		:	span.liu


*/


#ifndef __PMSERV_CORE__
#define __PMSERV_CORE__

///#include "GenericTypeDefs.h"
#include "mTypeDef.h"



static BOOL MicServInit();
static BOOL MicServDeInit();
///static BOOL MicServPwrDwn();
static BOOL MicIsServIdle();

///static INT32 MicServGetBattLv();


#endif	///#ifndef __PMSERV_CORE__

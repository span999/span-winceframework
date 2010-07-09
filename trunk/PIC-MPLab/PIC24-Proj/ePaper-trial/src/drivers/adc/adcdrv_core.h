/*

File			:	adcdrv_core.h				
Description		:	driver main body include file. include for function core.
Initialor		:	span.liu


*/


#ifndef __ADCDRV_CORE__
#define __ADCDRV_CORE__

///#include "GenericTypeDefs.h"
#include "mTypeDef.h"



static BOOL MicModInit();
static BOOL MicModDeInit();
static BOOL MicModPwrDwn();
static BOOL MicIsModIdle();

static INT32 MicModGetADC();


#endif	///#ifndef __ADCDRV_CORE__

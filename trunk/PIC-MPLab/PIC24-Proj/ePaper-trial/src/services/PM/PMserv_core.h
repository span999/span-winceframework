/*

File			:	PMserv_core.h				
Description		:	service main body include file. include for function core.
Initialor		:	span.liu


*/


#ifndef __PMSERV_CORE_H__
#define __PMSERV_CORE_H__

#include "mTypeDef.h"



BOOL ServInit_PM();
BOOL ServDeInit_PM();
BOOL ServPwrDwn_PM();
BOOL IsServIdle_PM();
///UINT ServGetBattLvDBG_BAT();
void ServIsIdle_PM();
void ServIsActive_PM();

///static INT32 MicServGetBattLv();



#endif	///#ifndef __PMSERV_CORE_H__

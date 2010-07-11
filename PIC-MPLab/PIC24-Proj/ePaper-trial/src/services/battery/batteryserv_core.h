/*

File			:	batteryserv_core.h				
Description		:	service main body include file. include for function core.
Initialor		:	span.liu


*/


#ifndef __BATTERYSERV_CORE_H__
#define __BATTERYSERV_CORE_H__


#include "mTypeDef.h"



BOOL ServInit_BAT();
BOOL ServDeInit_BAT();
BOOL ServPwrDwn_BAT();
BOOL IsServIdle_BAT();
UINT ServGetBattLvDBG_BAT();
void ServIsIdle_BAT();
void ServIsActive_BAT();




#endif	///#ifndef __BATTERYSERV_CORE_H__

/*

File			:	batteryserv_if.h				
Description		:	service interface include file. include this if you want to access battery service.
Initialor		:	span.liu


*/


#ifndef __BATTERYSERV_IF_H__
#define __BATTERYSERV_IF_H__


#include "mTypeDef.h"


/* type def for Battery level */
typedef    unsigned int            xBATTLEVEL;


typedef struct {
	BYTE numID;
	unsigned int adcMAX;
	BYTE level;
} ADC_BATT_MAPS;



xBATTLEVEL xMicServGetBatteryLevel();
xOnOff xMicServSetDebug_BAT( xOnOff do_sets );


#endif	///#ifndef __BATTERYSERV_IF_H__



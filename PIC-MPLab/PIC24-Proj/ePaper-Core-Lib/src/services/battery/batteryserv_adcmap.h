/*

File			:	batteryserv_adcmap.h				
Description		:	battery adc mappimg table.
Initialor		:	span.liu


*/


#ifndef __BATTERYSERV_ADCMAP_H__
#define __BATTERYSERV_ADCMAP_H__

#include "batteryserv_if.h"


static ADC_BATT_MAPS	adcBattTbl[12] =
{
	{  0,  734,  0 },
	{  1,  760,  1 },
	{  2,  786,  2 },
	{  3,  812,  3 },
	{  4,  838,  4 },
	{  5,  864,  5 },
	{  6,  890,  6 },
	{  7,  916,  7 },
	{  8,  942,  8 },
	{  9,  968,  9 },
	{ 10, 9999, 10 },
	{ -1,    0, 10 }
};


#endif	///#ifndef __BATTERYSERV_ADCMAP_H__



/*

File			:	adcdrv_core.h				
Description		:	driver main body include file. include for function core.
Initialor		:	span.liu


*/


#ifndef __ADCDRV_CORE_H__
#define __ADCDRV_CORE_H__


#include "mTypeDef.h"



BOOL ModInit_ADC();
BOOL ModDeInit_ADC();
BOOL ModPwrDwn_ADC();
BOOL IsModIdle_ADC();

UINT ModGetADCDBG_ADC();
UINT ModGetADC_ADC();
void ServIsIdle_BAT();
void ServIsActive_BAT();


#endif	///#ifndef __ADCDRV_CORE_H__

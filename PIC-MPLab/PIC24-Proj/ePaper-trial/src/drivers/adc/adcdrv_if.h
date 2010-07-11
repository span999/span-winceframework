/*

File			:	adcdrv_if.h				
Description		:	driver interface include file. include this if you want to access ADC driver.
Initialor		:	span.liu


*/


#ifndef __ADCDRV_IF_H__
#define __ADCDRV_IF_H__


#include "mTypeDef.h"


/* type def for ADC value */
typedef    unsigned int			xADCval;

bRET xMicModInit_ADC();
bRET xMicModDeInit_ADC();
bRET xMicModPwrDwn_ADC();
xPMMODSTAT xMicModInPwrSt_ADC();

xADCval xMicModGetADCValue();


#endif	///#ifndef __ADCDRV_IF_H__



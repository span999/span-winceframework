/*

File			:	adcdrv_core.c		
Description		:	driver main body file. ADC driver core body.
Initialor		:	span.liu


*/


#include "adcdrv_core.h"
#include "..\..\services\Toolbox\Toolbox.h"


#define USE_AND_OR /* To enable AND_OR mask setting */
#include <adc.h>


static BOOL IsIdle = FALSE;

///internal functions
static BOOL ModHWInit()
{
	BOOL bRet = FALSE;
	///TODO: hardware initial
	
	return bRet;
}


static BOOL ModHWdeInit()
{
	BOOL bRet = FALSE;
	///TODO: hardware de-initial
	
	return bRet;
}


static UINT ModHWreadADCport()
{
	UINT uiRet = 0;
	///TODO: read ADC hardware port

	unsigned char i=0;
	unsigned int channel,config1,config2,config3,configportL,configportH,configscanL,configscanH;
	unsigned int ADCResult[16];
	unsigned int ADCResultAvg;

	CloseADC10();

	/*set adc channel*/
	///AD1CHS:
	///channel= ADC_CH0_POS_SAMPLEA_AN5;
	channel= ADC_CH0_POS_SAMPLEB_AN2 | ADC_CH0_POS_SAMPLEA_AN2;
	SetChanADC10(channel);

	/*Configure adc*/
	///AD1CON1:
	config1 = ADC_MODULE_OFF | ADC_IDLE_CONTINUE | ADC_FORMAT_INTG | ADC_CLK_AUTO |ADC_AUTO_SAMPLING_ON ;
	///AD1CON2:
	config2 = ADC_SCAN_ON | ADC_INTR_16_CONV | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF;
	///AD1CON3:
	config3 = ADC_CONV_CLK_SYSTEM | ADC_SAMPLE_TIME_17 | ADC_CONV_CLK_254Tcy;
	///ADPCFGL:
	configportL = ~ENABLE_AN2_DIG;	///only AN2 analog 
	///ADPCFGH:
	configportH = 0;	///
	///AD1CSSL:
	///configscan = ADC_SCAN_AN5 ;
	configscanL = ADC_SCAN_AN2;
	configscanH = 0;
	OpenADC10(config1,config2,config3,configportL,configportH,configscanL,configscanH);
	
	EnableADC1;
	ADCResultAvg = 0;
	while( i < 16 )
    {
		ConvertADC10();
		while( BusySampADC1 ); /*wait till conversion complete*/
		ADCResult[i] = ReadADC10(i);
		ADCResultAvg = ADCResultAvg + ADCResult[i];
		i++;
	}
	
	CloseADC10();

	uiRet = ADCResultAvg / 16;
	return uiRet;
}



///common functions
BOOL ModInit_ADC()
{
	BOOL bRet = FALSE;
	
	bRet = ModHWInit();
	
	return bRet;
}


BOOL ModDeInit_ADC()
{
	BOOL bRet = FALSE;
	
	bRet = ModHWdeInit();
	
	return bRet;
}



BOOL ModPwrDwn_ADC()
{
	BOOL bRet = FALSE;
	
	bRet = ModHWdeInit();
	
	return bRet;
}


BOOL IsModIdle_ADC()
{
	BOOL bRet = FALSE;
	
	///ADC port is always in idle since it's a polling / passive driver
	bRet = TRUE;
	bRet = IsIdle;
	
	return bRet;
}


UINT ModGetADC_ADC()
{
	UINT uiRet = 0;
	
	uiRet = ModHWreadADCport();
	
	return uiRet;
}


UINT ModGetADCDBG_ADC()
{
	return uMicGetRAND();
}	


void ModIsIdle_ADC()
{
	IsIdle = TRUE;
}


void ModIsActive_ADC()
{
	IsIdle = FALSE;
}

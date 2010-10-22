/*

File			:	spADC_libcore.c		
Description		:	driver main body file. ADC driver core body.
Initialor		:	span.liu


*/

#define USE_AND_OR /* To enable AND_OR mask setting */
#include <adc.h>





unsigned int spGetADCdata( void )
{
	unsigned int uiRet = 0;
	unsigned char i = 0;
	unsigned int ADCResult[16];
	unsigned int ADCResultAvg = 0;

	///TODO: read ADC hardware port
	
	CloseADC10();

	/*set adc channel*/
	///AD1CHS:
	SetChanADC10( ADC_CH0_POS_SAMPLEA_AN2 );

	/*Configure adc*/

#if defined (adc_v3_1) ||defined (adc_v3_2)
#warning "build with libary ADCv3 up"
	OpenADC10( 
		(ADC_MODULE_OFF | ADC_IDLE_CONTINUE | ADC_FORMAT_INTG | ADC_CLK_AUTO |ADC_AUTO_SAMPLING_ON),	///AD1CON1:
		(ADC_SCAN_ON | ADC_INTR_16_CONV | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF),							///AD1CON2:
		(ADC_CONV_CLK_SYSTEM | ADC_SAMPLE_TIME_17 | ADC_CONV_CLK_254Tcy),								///AD1CON3:
		BG_VREF_ENABLE,																					///ANCFG:
		(ADC_SCAN_AN2),																					///AD1CSSL:
		0																								///AD1CSSH:
	);
#else
#warning "build with ADC libary ADCv2 or below"
	OpenADC10( 
		(ADC_MODULE_OFF | ADC_IDLE_CONTINUE | ADC_FORMAT_INTG | ADC_CLK_AUTO |ADC_AUTO_SAMPLING_ON),	///AD1CON1:
		(ADC_SCAN_ON | ADC_INTR_16_CONV | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF),							///AD1CON2:
		(ADC_CONV_CLK_SYSTEM | ADC_SAMPLE_TIME_17 | ADC_CONV_CLK_254Tcy),								///AD1CON3:
		(~(ENABLE_AN2_DIG | ENABLE_AN4_DIG | ENABLE_AN5_DIG) ),											///ADPCFGL:
		(ENABLE_AN16_DIG | ENABLE_AN17_DIG),															///ADPCFGH:							///ADPCFGH:
		(ADC_SCAN_AN2),																					///AD1CSSL:
		0																								///AD1CSSH:
	);
#endif

	
	EnableADC1;

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


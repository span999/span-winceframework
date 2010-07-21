/*




*/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "mTypeDef.h"
#include "Toolbox.h"





void SPPRINTF( char* pString, ... )
{
	va_list ap;
	char szbuf[128];

	va_start( ap, pString );
	vsprintf( szbuf, pString, ap );
	va_end( ap );

	printf( szbuf );
	///DBGPRINTF( szbuf );
	
}



/*
	uMicGetRAND

*/
UINT uMicGetRAND( void )
{
	UINT uiRet = 0;
	static UINT prevValue = 0;
	
	srand( prevValue ); 
	uiRet = abs( rand() );
	prevValue = uiRet;	

	return uiRet;
}


/*
	vMicSLEEP

*/
void vMicSLEEP( UINT uiMs )
{
	vTaskDelay( (uiMs/portTICK_RATE_MS) );
}



/*
	vMicSLEEPuntil

*/
void vMicSLEEPuntil( UINT uiMs )
{
	static portTickType xLastWakeTime = 0;
	

	if( 0 == xLastWakeTime )
		xLastWakeTime = xTaskGetTickCount();

	vTaskDelayUntil( &xLastWakeTime, (uiMs/portTICK_RATE_MS) );
}



///return of wait
typedef    unsigned int			waitRET;
#define		waitRET_TO			0x0001
#define		waitRET_OBJECT		0x0010

/* 


*/
waitRET xMicWaitforMultiQueue( UINT ObjNum, xQueueHandle* pQueueArray, UINT MStimeout )
{	
	waitRET wRet;
	
	
	return wRet;
}



/* 


*/
hProtectedData xMicCreateProtectedData( UINT uiDataSizeInByte )
{
	hProtectedData hData = NULL;
	
	hData = xQueueCreate( 1, uiDataSizeInByte );
	
	return hData; 
}



/* 


*/
BOOL xMicSetProtectedData( hProtectedData hData, void* pDataSet )
{
	BOOL bRet = FALSE;
	
	if( NULL != hData && NULL != pDataSet )
	{
		BYTE bBuffer[128];
		
		///read it out till empty
		while( uxQueueMessagesWaiting( hData ) > 0 )
			xQueueReceive( hData, bBuffer, 10 ); 
		
		if( pdPASS == xQueueSend( hData, pDataSet, 10 ) )
			bRet = TRUE;
	}	
	
	return bRet;
}



/* 


*/
BOOL xMicGetProtectedData( hProtectedData hData, void* pDataGet )
{
	BOOL bRet = FALSE;

	if( NULL != hData && NULL != pDataGet )
	{
		if( pdPASS == xQueuePeek( hData, pDataGet, 10 ) )
		bRet = TRUE;
	}	
	
	return bRet;
}



/* 


*/
void xMicDeleteProtectedData( hProtectedData hData )
{
	if( NULL != hData )
		vQueueDelete( hData );
}



/* EOF */

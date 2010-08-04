/*




*/


#ifndef __TOOLBOX_H__
#define __TOOLBOX_H__

#include "mFreeRTOSDef.h"

typedef    xQueueHandle            hProtectedData;
typedef 	void (*pfnDBGMSGEXT)( char* pString, ... );

void SPPRINTF( char* pString, ... );
void SetExtDbgMsg( pfnDBGMSGEXT pfnDBG );

UINT uMicGetRAND( void );
void vMicSLEEP( UINT uiMs );
void vMicSLEEPuntil( UINT uiMs );

hProtectedData xMicCreateProtectedData( UINT uiDataSizeInByte );
BOOL xMicSetProtectedData( hProtectedData hData, void* pDataSet );
BOOL xMicGetProtectedData( hProtectedData hData, void* pDataGet );
void xMicDeleteProtectedData( hProtectedData hData );


#endif	///#ifndef __TOOLBOX_H__


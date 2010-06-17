// include device specific peripheral libraries
#if defined(__PIC24F__)
	#define USE_AND_OR
	#include <uart.h>
#else
	#include <plib.h>
	#include <GenericTypeDefs.h>
#endif

#include <string.h>

///#include "HardwareProfile.h"
///#include "taskUART.h"
#include "mFreeRTOSDef.h"
#include "mTypeDef.h"

///////////////////////////////////////////////////////////////////
// forward declarations
void* UARTDRV_DEBUG_drvInit(void* pvParameter);
bRET UARTDRV_DEBUG_drvBoardInit( void );
void UARTDRV_DEBUG_taskMain(void* pvParameter);


///////////////////////////////////////////////////////////////////
// Variables 
xQueueHandle hUARTDRV_DEBUG_drvTxQueue;
xQueueHandle hUARTDRV_DEBUG_drvRxQueue;
xTaskHandle hUARTDRV_DEBUG_drvTask;

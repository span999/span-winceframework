#ifndef __UARTDRV_DEBUG_H__
#define __UARTDRV_DEBUG_H__


extern xQueueHandle hUARTDRV_DEBUG_drvTxQueue;
extern xQueueHandle hUARTDRV_DEBUG_drvRxQueue;
extern xTaskHandle hUARTDRV_DEBUG_drvTask;


void* pvUARTDRV_DEBUG_drvInit(void* pvParameter);


#endif	///#ifndef __UARTDRV_DEBUG_H__
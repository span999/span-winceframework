#ifndef __UARTDRV_DEBUG_H__
#define __UARTDRV_DEBUG_H__

#define UART_QUEUE_SIZE	4	// number of messages queue can contain
#define UART_ENTRY_SIZE 60	// size of each message

// structure used to output messages via the uart
// the application must ensure that messages are less than 60 characters
typedef struct {
	char	buff[UART_ENTRY_SIZE];	
} UARTMsg;

extern xQueueHandle hUARTDRV_DEBUG_drvTxQueue;
extern xQueueHandle hUARTDRV_DEBUG_drvRxQueue;
extern xTaskHandle hUARTDRV_DEBUG_drvTask;


void* pvUARTDRV_DEBUG_drvInit(void* pvParameter);
void UARTprintf(char* msg);
void UARTPrintChar(BYTE c);

#endif	///#ifndef __UARTDRV_DEBUG_H__



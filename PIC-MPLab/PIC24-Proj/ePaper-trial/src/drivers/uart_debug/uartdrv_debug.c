// include device specific peripheral libraries
#if defined(__PIC24F__)
	#define USE_AND_OR
	#include <uart.h>
#else
	#include <plib.h>
	#include <GenericTypeDefs.h>
#endif

#include <string.h>

#include "HardwareProfile.h"
///#include "taskUART.h"
#include "mFreeRTOSDef.h"
#include "mTypeDef.h"
#include "uartdrv_debug.h"
#include "uartdrv_debug_private.h"



///////////////////////////////////////////////////////////////////
// forward declarations
void* pvUARTDRV_DEBUG_drvInit(void* pvParameter);
static bRET bUARTDRV_DEBUG_drvBoardInit( void );
static void vUARTDRV_DEBUG_taskMain(void* pvParameter);


///////////////////////////////////////////////////////////////////
// Variables 
xQueueHandle hUARTDRV_DEBUG_drvTxQueue;
xQueueHandle hUARTDRV_DEBUG_drvRxQueue;
xTaskHandle hUARTDRV_DEBUG_drvTask;




void* pvUARTDRV_DEBUG_drvInit(void* pvParameter)
{
	// create the UART queue for transmitting data
	hUARTDRV_DEBUG_drvTxQueue = xQueueCreate(UART_QUEUE_SIZE, sizeof(UARTMsg));
	// and the queue for receiving characters
	hUARTDRV_DEBUG_drvRxQueue = xQueueCreate(UART_QUEUE_SIZE, sizeof(char));
	
	if( bRET_FALSE == bUARTDRV_DEBUG_drvBoardInit() )
		return NULL;
	
	// create the UART transmit task
	xTaskCreate(vUARTDRV_DEBUG_taskMain, (signed char*) "UART_DEBUG", STACK_SIZE_UART, 
		NULL, tskIDLE_PRIORITY + 1, &hUARTDRV_DEBUG_drvTask );
		
	return hUARTDRV_DEBUG_drvTxQueue;
}


bRET bUARTDRV_DEBUG_drvBoardInit( void )
{
	bRET bRet = bRET_FALSE;
	
	// set up the UART
	UARTTX_TRIS = 0;
	UARTRX_TRIS = 1;
	OpenUART(UART_CONFIG1, UART_CONFIG2, CLOSEST_BRG);
	ConfigIntUART(UART_INT_CONFIG);	
	
	bRet = bRET_TRUE;
	
	return bRet;
}



void vUARTDRV_DEBUG_taskMain(void* pvParameter)
{
	UARTMsg msg;
	char* pChar;
		
	// in this task we wait for a string to be received and then
	// print it out.
	while (1) {
		// wait until a message is received
		xQueueReceive(hUARTDRV_DEBUG_drvTxQueue, &msg, portMAX_DELAY);
		
		// print out the string
		pChar = msg.buff;
		while (*pChar != '\0') {
			// while there is space in the tx buffer
			while ((!USTAbits.UTXBF) && (*pChar != '\0')) {
				UTXREG = *pChar++;	
			}			
		}
	}	
}


#if defined(__C30__)
///void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void)
void _ThisUARTINTtype_ _ThisUARTInterrupt_(void)
{
	char cChar;
	portBASE_TYPE xTaskWoken = pdFALSE;
	
	// get the character and post it on the queue possibly starting a context
	// switch if a higher priority task was woken
	IFS1bits.U2RXIF = 0;
	while (USTAbits.URXDA) {
		cChar = URXREG;
		// send the data to the queue, if the queue is full then we
		// lose the data rather than blocking the ISR
		xQueueSendFromISR(hUARTDRV_DEBUG_drvRxQueue, &cChar, &xTaskWoken);
	}	
	
	if (xTaskWoken != pdFALSE)
		taskYIELD();	
}
#else // PIC32
// the actual PIC32 handling routine 
///void __attribute__( (interrupt(ipl2), vector(_UART2_VECTOR))) vU2InterruptHandler(void)
void _ThisUARTINTtype_ _ThisUARTInterrupt_(void)
{
	char cChar;
	portBASE_TYPE xTaskWoken = pdFALSE;
	
	// check for RX interrupts
	if (mU2RXGetIntFlag()) {
		while (USTAbits.URXDA) {
			/* Retrieve the received character and place it in the queue of
			received characters. */
			cChar = U2RXREG;
			xQueueSendFromISR(hUARTDRV_DEBUG_drvRxQueue, &cChar, &xTaskWoken);
		}
		mU2RXClearIntFlag();
	}

	// if there are TX interrupts then just clear them as we don't handle them
	if (mU2TXGetIntFlag())
		mU2TXClearIntFlag();

	// If sending or receiving necessitates a context switch, then switch now.
	if (xTaskWoken != pdFALSE)
		taskYIELD();	
}
#endif


void UARTprintf(char* msg)
{
	UARTMsg sMsg;
	
	// copy the message and ensure it is NULL terminated
	strncpy(sMsg.buff, msg, UART_ENTRY_SIZE);
	sMsg.buff[UART_ENTRY_SIZE - 1] = '\0';
	
	// send the message to the UART task
	// we use a zero wait time so as not to block any high priority tasks
	// if the low priority UART task is currently printing and the queue
	// is full then we just fail to print the message.
	// This is okay since it is only used for diagnostic messages
	xQueueSendToBack(hUARTDRV_DEBUG_drvTxQueue, &sMsg, 0);	
}


const unsigned char HexCharArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void UARTPrintChar(BYTE c)
{
	UARTMsg sMsg;
	BYTE toPrint;
	
	// convert the character and print it
	toPrint = (c >> 4) & 0x0F;
	sMsg.buff[0] = HexCharArray[toPrint];
	toPrint = c & 0x0F;
	sMsg.buff[1] = HexCharArray[toPrint];
	sMsg.buff[2] = '\0';
	
	// send the message to the UART task
	xQueueSendToBack(hUARTDRV_DEBUG_drvTxQueue, &sMsg, portMAX_DELAY);	
}



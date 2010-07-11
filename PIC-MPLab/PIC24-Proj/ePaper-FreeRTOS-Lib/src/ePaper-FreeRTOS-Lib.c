/*****************************************************************************
 * Microchip RTOS and Stacks Demo
 *****************************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright ?2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS?WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Version  Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 1.0      D.Wenn               09/01/08    Initial Version  
 * 1.1		D.Wenn				 07/15/09	 Added GFX3 and ENC624J600 support
 * 1.2      D.Wenn               09/04/09    Upgrade to latest stacks  
 *****************************************************************************/
 
// Standard includes
#include <stdio.h>

// FreeRTOS includes
#include "mFreeRTOSDef.h"

// local includes





// set the configuration fuses for the processor
#if defined(__PIC24F__)
	_CONFIG2(FNOSC_PRIPLL & POSCMOD_XT)		// Primary XT OSC with 4x PLL
	_CONFIG1(ICS_PGx2 & JTAGEN_OFF & FWDTEN_OFF) // JTAG off, watchdog timer off
#elif defined(__PIC32MX__)
	#pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2
	#pragma config FWDTEN = OFF, FPBDIV = DIV_2, POSCMOD = XT, FNOSC = PRIPLL, CP = OFF
#endif



#define UART_ENTRY_SIZE 60

typedef struct {
	char	buff[UART_ENTRY_SIZE];	
} LIBMsg;


///////////////////////////////////////////////////////////////////
// Semaphores used to guard access to the various hardware resources
// they are controlled because multiple tasks may access them
///xSemaphoreHandle SPI2Semaphore;
///xSemaphoreHandle FLASHSemaphore;
///xSemaphoreHandle QVGASemaphore;
///xSemaphoreHandle METERSemaphore;

///xTaskHandle hMETERTask;///
///xTaskHandle hMIWITask;///
///xTaskHandle hTCPIPTask;///

///xQueueHandle hMETERQueue;

// global meter object
///structMeter gMeter;

///////////////////////////////////////////////////////////////////
// Forward references in this module


///////////////////////////////////////////////////////////////////
// local variables and constants
const LIBMsg msgLibStart = {"\r\nFreeRTOS library Started. Build: " __TIME__ "\r\n"};

signed portBASE_TYPE xQueueSendToBackForce( xQueueHandle pxQueue, const void * const pvItemToQueue, portTickType xTicksToWait )
{
	signed portBASE_TYPE thisRet;
	
	thisRet = xQueueGenericSend( pxQueue, pvItemToQueue, 0, queueSEND_TO_BACK );
	if( errQUEUE_FULL == thisRet )
	{
		char *pvBuffer;
		///pvBuffer = malloc( sizeof(*pvItemToQueue) );
		pvBuffer = pvPortMalloc( sizeof(*pvItemToQueue) );
		thisRet = xQueueGenericReceive( pxQueue, pvBuffer, 0, pdFALSE );
		///free( pvBuffer );
		vPortFree( pvBuffer );
		if( pdPASS == thisRet )
		{
			thisRet = xQueueGenericSend( pxQueue, pvItemToQueue, 0, queueSEND_TO_BACK );
		}
	}
	
	return thisRet;
}





/* EOF */

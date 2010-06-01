#include <p18cxxx.h>
#include <stdio.h>
#include <delays.h>
#include <usart.h>

#include "FreeRTOSConfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <list.h>
#include <semphr.h>
#include <alloc.h>

///#pragma config OSC = INTIO67	//Internal oscillator
#pragma config FCMEN = OFF
#pragma config IESO = OFF
///#pragma config PWRT = ON
///#pragma config BOREN = OFF
#pragma config WDT = OFF
///#pragma config MCLRE = OFF		//MCLR is disabled
///#pragma config PBADEN = OFF
///#pragma config LVP = OFF
#pragma config XINST = ON		//extended mode enabled

long int i, j = 0;
xSemaphoreHandle xSem = NULL;

void Task1(void *params) {
	__reclaim_stack();
	xSem = xSemaphoreCreateMutex();
	while(1) {		
		if(xSemaphoreTake(xSem, (portTickType) 10) == pdTRUE) {
			puts("abcd");
			xSemaphoreGive(xSem);
		} 
		else {
		} 
		vTaskDelay(100);
		i++;
	}
}

void Task2(void *params) {
	taskENTER_CRITICAL();
	while( xSem == NULL )
		vTaskDelay(1);
	taskEXIT_CRITICAL();

	while(1) {				
		if(xSemaphoreTake(xSem, (portTickType) 10) == pdTRUE) {
			puts("1234");
			xSemaphoreGive(xSem);
		} 
		else {
		}
		vTaskDelay(100);
		j++;
	}
}

void vSerialTxISR()	{
}

void vSerialRxISR() {
}

void main(void) {
	heapinit();

	OSCCON = 0b01110000; 	//8 MHz
	OSCTUNE = 0b01011111;	//enable PLL
	TRISC = 0x00;
	stdout = _H_USART;
///	OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, 51);
	
	xTaskCreate(Task1, (const portCHAR * const) "Ts1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(Task2, (const portCHAR * const) "Ts2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();

	while(1) {
		ClrWdt();
	}
}

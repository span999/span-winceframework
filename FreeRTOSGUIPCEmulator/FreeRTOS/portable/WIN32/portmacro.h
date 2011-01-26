/*
	FreeRTOS.org V4.2.1 (WIN32 port) - Copyright (C) 2007-2008 Dushara Jayasinghe.

	This file is part of the FreeRTOS.org distribution.

	FreeRTOS.org is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	FreeRTOS.org is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FreeRTOS.org; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	A special exception to the GPL can be applied should you wish to distribute
	a combined work that includes FreeRTOS.org, without being obliged to provide
	the source code for any proprietary components.  See the licensing section
	of http://www.FreeRTOS.org for full details of how and when the exception
	can be applied.

	***************************************************************************
	See http://www.FreeRTOS.org for documentation, latest information, license
	and contact details.  Please ensure to read the configuration and relevant
	port sections of the online documentation.

	Also see http://www.SafeRTOS.com for an IEC 61508 compliant version along
	with commercial development and support options.
	***************************************************************************
*/

#ifndef PORTMACRO_H
#define PORTMACRO_H

#include <Windows.h>
#include <tchar.h>
#include "cpuemu.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
	Defines
******************************************************************************/
/* Type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short
#define portSTACK_TYPE	unsigned portLONG
#define portBASE_TYPE	portLONG

#define portDEBUG		0

#if( USE_16_BIT_TICKS == 1 )
    typedef unsigned portSHORT portTickType;
    #define portMAX_DELAY ( portTickType ) 0xffff
#else
    typedef unsigned portLONG portTickType;
    #define portMAX_DELAY ( portTickType ) 0xffffffff
#endif

/* Hardware specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_RATE_MS			( 4) //( ( portTickType ) 1000 / 15625 )
#define portMS_TO_TICKS(ms)			( ( portTickType ) (1000*(ms) + (15625)/2)/15625 )
#define portBYTE_ALIGNMENT			4

#define portYIELD()			__swi();
#define portDISABLE_INTERRUPTS()	__disable_interrupt()
#define portENABLE_INTERRUPTS()		__enable_interrupt()

/* Critical section handling. */
#define portENTER_CRITICAL()		vPortEnterCritical()
#define portEXIT_CRITICAL()			vPortExitCritical()

/* Task utilities. */
#define portEND_SWITCHING_ISR( xSwitchRequired ) 	\
{													\
extern void vTaskSwitchContext( void );				\
													\
	if( xSwitchRequired ) 							\
	{												\
		vTaskSwitchContext();						\
	}												\
}

/* Compiler specifics. */
#define inline

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void * pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void * pvParameters )

/******************************************************************************
	Global variables
******************************************************************************/
extern volatile portSTACK_TYPE dwPendingIsr;	// pending interrupts

/******************************************************************************
	Global functions
******************************************************************************/

/* Assign a handler to an interrupt no */
int iPortSetIsrHandler(int iNo, void (*handler)(void));

/* enable specified interrupt no */
void vPortEnableInt(int iNo);

/* disable specified interrupt no */
void vPortDisableInt(int iNo);

/* query interrupt enabled status */
BOOL bPortIsEnabledInt(int iNo);

/* global interrupt disable */
void __disable_interrupt(void);

/* global interrupt enable */
void __enable_interrupt(void);

/* generate software interrupt - non - maskable */
void __swi(void);

/* To be used by interrupt generation threads. e.g. Create a thread that 
monitors console events and generate an interrupt whenever a key is pressed.
See also implimentation of tick_generator.
*/
void __generate_interrupt(int iNo);

void vPortEnterCritical( void );
void vPortExitCritical( void );

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif

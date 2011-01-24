/*
	FreeRTOS.org V4.2.1 (WIN32 port) - Copyright (C) 2007-2008 Optiscan Pty Ltd.
	Author: Dushara Jayasinghe

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
#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semtest.h>
#include <blockq.h>
#include <blocktim.h>
#include <print.h>

/******************************************************************************
	Defines
******************************************************************************/
#define mainQUEUE_BLOCK_PRIORITY	( tskIDLE_PRIORITY + 3 )
#define mainSEMAPHORE_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )

#define mainCheckTimeout		5000

/******************************************************************************
	National prototypes
******************************************************************************/
/******************************************************************************
	Global variables
******************************************************************************/
/******************************************************************************
	National variables
******************************************************************************/
/******************************************************************************
	National functions
******************************************************************************/
/******************************************************************************
	Global functions
******************************************************************************/
void vApplicationIdleHook()
{
	Sleep(INFINITE);	// to reduce processor usage
}

static void vErrorChecks( void *pvParameters )
{
	portTickType xTimeStamp, xTimeout;
	const portCHAR *str;

	xTimeStamp = xTaskGetTickCount();

	for(;;)
	{
		xTimeout = mainCheckTimeout - (xTaskGetTickCount() - xTimeStamp);
		str = pcPrintGetNextMessage(xTimeout);

		if(str)
		{
			printf(str);
		}

		if((xTaskGetTickCount() - xTimeStamp) > mainCheckTimeout )
		{
			xTimeStamp = xTaskGetTickCount();

			if( xAreSemaphoreTasksStillRunning() != pdTRUE )
			{
				str = "Semaphore test !running\n";
			}
			if( xAreBlockingQueuesStillRunning() != pdTRUE )
			{
				str = "Blocking Queues !running\n";
			}
			if( xAreBlockTimeTestTasksStillRunning() != pdTRUE )
			{
				str = "Block Time !running\n";
			}

			if(str)
			{
				printf(str);
			}
		}
	}
}

int main()
{
	/* Setup the microcontroller hardware for the demo. */
        //prvSetupHardware();
	
	vPrintInitialise();
	/* Create the common demo application tasks, for example: */
	vStartBlockingQueueTasks(mainQUEUE_BLOCK_PRIORITY);
	vCreateBlockTimeTasks();
	vStartSemaphoreTasks(mainSEMAPHORE_TASK_PRIORITY);

	/* Create the "Print" task as described at the top of the file. */
	xTaskCreate( vErrorChecks, "ErrorChk", 100, NULL, 0, NULL );


		
	/* Start the scheduler, this function should not return as it causes the execution
	context to change from main() to one of the created tasks. */
	vTaskStartScheduler();
	
        /* Should never get here! */
        return 0;
}

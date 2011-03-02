
#include <stdio.h>
#include <stdlib.h>


#include <FreeRTOS.h>
#include <task.h>




///include this for grlib
///#include "grlib/grlib_set.h"


/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "SDLDislpayEmulator/SDLDisplay.h"



extern "C" DWORD gdwTickCount;


#ifndef OLD_DEMO
extern "C" void TransIOMsg(unsigned long ulMessage, long lX, long lY);
void IODriver( void * pvParameters )
{
	SDLInit();	
	SDLScreenCallbackSet(TransIOMsg);	
	bool done = false;
	while(!done)
	{
		done = !SDLProcessEventWIN32();			
		///
		vTaskDelay(1);
	}
	SDLDestroy();
	while(1)
	{;}	
}
#endif


void vUserTask1(void *pvParameters)
{
	
	// Suspend ourselves.
	vTaskSuspend( NULL );

	while (1)
	{;}
}



void vUserTask2(void *pvParameters)
{

	InitGraph();
	
	while(1)
	{
		// Clear screen
		ClearDevice();
		PrimitiveTest();
		vTaskDelay(800);
	}

	// Suspend ourselves.
	vTaskSuspend( NULL );

	while (1)
	{;}
}


void vUserTask3(void *pvParameters)
{

	InitGraph();
	
	// Clear screen
	ClearDevice();
	ObjectTest();

	vTaskDelay(800);
	
	// Suspend ourselves.
	vTaskSuspend( NULL );

	while (1)
	{;}
}


void vUserTask4(void *pvParameters)
{
#if 0 ///portTICK_RATE_MS is not working here in semulator
	portTickType xLastWakeTime;
	while(1)
	{
		///vTaskDelay(100);
		vTaskDelayUntil( &xLastWakeTime, 1000/portTICK_RATE_MS);
		gdwTickCount = gdwTickCount + 1000;
		printf("Ticking...%d...", gdwTickCount);
	}

#else	
	while(1)
	{
		Sleep( 100 );
		gdwTickCount = gdwTickCount + 100;
		///printf("Ticking...%d...", gdwTickCount);
	}
#endif
	// Suspend ourselves.
	vTaskSuspend( NULL );

	while (1)
	{;}
}



int main()
{
#ifdef OLD_DEMO						
	///xTaskCreate( graphicsDriver, "gd", 100, NULL, configMAX_PRIORITIES, NULL );
	xTaskCreate( graphicsDriver, ( signed char * )"gd", 100, NULL, configMAX_PRIORITIES, NULL );
	printf(" graphicsDriver created\n");
#else
	xTaskCreate( IODriver, ( signed char * )"IO", 100, NULL, configMAX_PRIORITIES, NULL );
	printf(" IODriver created\n");
#endif

#ifdef OLD_DEMO		
	xTaskCreate( vUserTask1, "Task2",100, NULL, 1, NULL );
	xTaskCreate( vUserTask1, ( signed char * )"Task1", 100, NULL, 1, NULL );
	printf(" vUserTask1 created\n");
#endif

#if 0
	/* for PrimitiveTest */
	xTaskCreate( vUserTask2, ( signed char * )"Task2", 100, NULL, 1, NULL );
	printf(" vUserTask2 created\n");
#else
	/* for ObjectTest */
	xTaskCreate( vUserTask3, ( signed char * )"Task3", 100, NULL, 1, NULL );
	printf(" vUserTask3 created\n");
#endif

	/* ticking simulate */
	xTaskCreate( vUserTask4, ( signed char * )"Task4", 100, NULL, 1, NULL );
	printf(" vUserTask4 created\n");
	
	/* Start the scheduler, this function should not return as it causes the execution
	context to change from main() to one of the created tasks. */
	vTaskStartScheduler();

	/* Should never get here! */
	return 0;
}

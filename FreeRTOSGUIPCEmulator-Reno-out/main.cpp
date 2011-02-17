
#include <stdio.h>
#include <stdlib.h>


#include <FreeRTOS.h>
#include <task.h>




///include this for grlib
///#include "grlib/grlib_set.h"


/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "SDLDislpayEmulator/SDLDisplay.h"






#ifndef OLD_DEMO
extern "C" void TransIOMsg(unsigned long ulMessage, long lX, long lY);
void IODriver( void * pvParameters )
{
	SDLInit();	
	
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




int main()
{

	xTaskCreate( IODriver, ( signed char * )"IO", 100, NULL, configMAX_PRIORITIES, NULL );
	printf(" IODriver created\n");


#ifdef OLD_DEMO		
	xTaskCreate( vUserTask1, "Task2",100, NULL, 1, NULL );
	xTaskCreate( vUserTask1, ( signed char * )"Task1", 100, NULL, 1, NULL );
	printf(" vUserTask1 created\n");
#endif

	/* for PrimitiveTest */
	xTaskCreate( vUserTask2, ( signed char * )"Task2", 100, NULL, 1, NULL );
	printf(" vUserTask2 created\n");
	
	/* Start the scheduler, this function should not return as it causes the execution
	context to change from main() to one of the created tasks. */
	vTaskStartScheduler();

	/* Should never get here! */
	return 0;
}

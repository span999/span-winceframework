
#include <stdio.h>
#include <stdlib.h>


#include <FreeRTOS.h>
#include <task.h>

///#define		OLD_DEMO

///include this for grlib
#include "grlib/grlib_set.h"

/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "SDLDislpayEmulator/SDLDisplay.h"





#ifdef OLD_DEMO
void graphicsDriver( void * pvParameters )
{
	SDLInit();	
	WidgetMutexInit();
	SDLScreenCallbackSet(WidgetPointerMessage);	
	bool done = false;
	while(!done)
	{
		done = !SDLProcessEvent();			
		WidgetMessageQueueProcess();
		vTaskDelay(1);
	}
	SDLDestroy();
	while(1)
	{;}	
}


/******************************************************************************
Users functions
******************************************************************************/
void OnSliderChange(tWidget *pWidget, long lValue)
{
	static char pcSliderText[10];
	sprintf(pcSliderText, "%3d%%", lValue);     
	SliderTextSet((tSliderWidget *)pWidget, pcSliderText);
	WidgetPaint(pWidget);
}
#endif

#ifndef OLD_DEMO
extern "C" void TransIOMsg(unsigned long ulMessage, long lX, long lY);
void IODriver( void * pvParameters )
{
	SDLInit();	
	WidgetMutexInit();
	SDLScreenCallbackSet(TransIOMsg);	
	bool done = false;
	while(!done)
	{
		done = !SDLProcessEventWIN32();			
///		WidgetMessageQueueProcess();
		vTaskDelay(1);
	}
	SDLDestroy();
	while(1)
	{;}	
}
#endif

#ifdef OLD_DEMO
void vUserTask1(void *pvParameters)
{
	Slider(sld, WIDGET_ROOT, 0,0,
		&SDLDisplay, 5, 115, 220, 30, 0, 100, 25,
		(SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
		SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
		ClrGray, ClrBlack, ClrSilver, ClrWhite, ClrWhite,
		&g_sFontCm20, "25%", 0, 0, OnSliderChange);

	WidgetAdd(WIDGET_ROOT, (tWidget *)&sld);
	WidgetPaint(WIDGET_ROOT);
	
	// Suspend ourselves.
	vTaskSuspend( NULL );

	while (1)
	{;}
}
#endif


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
	
///	xTaskCreate( vUserTask2, ( signed char * )"Task2", 100, NULL, 1, NULL );
///	printf(" vUserTask2 created\n");

	xTaskCreate( vUserTask3, ( signed char * )"Task3", 100, NULL, 1, NULL );
	printf(" vUserTask3 created\n");
	
	/* Start the scheduler, this function should not return as it causes the execution
	context to change from main() to one of the created tasks. */
	vTaskStartScheduler();

	/* Should never get here! */
	return 0;
}

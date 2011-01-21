
#include <stdio.h>
#include <stdlib.h>


#include <FreeRTOS.h>
#include <task.h>

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
#include "grlib/pushbutton.h"
#include "grlib/imgbutton.h"
#include "grlib/listbox.h"

#include "SDLDislpayEmulator/SDLDisplay.h"



#ifdef __cplusplus
extern "C" {
#endif
	void vApplicationIdleHook()
	{
		Sleep(INFINITE);	// to reduce processor usage

	}
#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif
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


int main()
{						
	xTaskCreate( graphicsDriver, "gd", 100, NULL, configMAX_PRIORITIES, NULL );

	
	xTaskCreate( vUserTask1, "Task2",100, NULL, 1, NULL );

	/* Start the scheduler, this function should not return as it causes the execution
	context to change from main() to one of the created tasks. */
	vTaskStartScheduler();

	/* Should never get here! */
	return 0;
}

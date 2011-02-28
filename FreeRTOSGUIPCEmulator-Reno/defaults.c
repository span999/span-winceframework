
#include <FreeRTOS.h>
#include <task.h>



void vApplicationIdleHook()
{
	Sleep(INFINITE);	// to reduce processor usage
}

void DelayMs(WORD time)
{
	Sleep(time);
}

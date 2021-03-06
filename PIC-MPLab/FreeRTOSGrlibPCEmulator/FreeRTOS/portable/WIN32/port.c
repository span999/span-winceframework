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

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

extern DWORD * pxCurrentTCB;

/******************************************************************************
	Defines
******************************************************************************/
#define NMI	(1<<CPU_INTR_SWI)

/*
	Win32 simulator doesn't really use a stack. Instead It just
	keep some task specific info in the pseudostack
*/
typedef struct
{
	void (*entry)(void *pvParameters);	/* Task Entry function */
	void *pvParameters;					/* parameters to Entry function */
	portSTACK_TYPE ulCriticalNesting;	/* Critical nesting count */
	HANDLE hThread;						/* handle of thread associated with task */
	HANDLE hSemaphore;					/* Semaphore thread (task) waits on at start and after yielding */
	portSTACK_TYPE dwGlobalIsr;			/* mask used to enable/disable interrupts */
	BOOL yielded;						/* Need to know how task went out of focus */
}SSIM_T;

#define portNO_CRITICAL_NESTING 		( ( unsigned portLONG ) 0 )

/******************************************************************************
	National prototypes
******************************************************************************/
/******************************************************************************
	Global variables
******************************************************************************/
volatile DWORD dwPendingIsr;	// pending interrupts
HANDLE hIsrInvoke;	// event to signal an interrupt
HANDLE hIsrMutex;	// mutex to protect above 2

/******************************************************************************
	National variables
******************************************************************************/
static HANDLE hIsrDispatcher;	// handle of the main thread

static volatile portSTACK_TYPE dwGlobalIsr = NMI;	// interrupts disabled @ start
static volatile portSTACK_TYPE dwEnabledIsr = NMI;	// mask of enabled ISRs (individual)

static HANDLE hTickAck;		// acknolwledge tick interrupt
static HANDLE hTermAck;		// acknowledge termination

static enum
{
	SWI_ID_YIELD,
	SWI_ID_TERMINATE
}
ESWI_ID;

unsigned portLONG ulCriticalNesting = ( unsigned portLONG ) 9999;
void (*vIsrHandler[CPU_INTR_COUNT])(void);

#if portDEBUG == 1

#define MAX_TRACE	1024

const char * TraceStr[MAX_TRACE];
int TracePtr;

#define TRACE_INFO(s)	do {			\
	if(TracePtr < MAX_TRACE) {		\
		TraceStr[TracePtr++]=s;		\
	}					\
}while(0)

static char *TskName(DWORD *TCB)
{
	// HACK for debugging
	return ((char*)TCB + 56);
}

#else

#define TRACE_INFO(s)

#endif

/******************************************************************************
	National functions
******************************************************************************/
static DWORD WINAPI tick_generator(LPVOID lpParameter)
{
	HANDLE hTimer;
	LARGE_INTEGER liDueTime;
	HANDLE hObjList[2];

	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	liDueTime.QuadPart = -20000;	// 1ms interval

	hObjList[0] = hIsrMutex;
	hObjList[1] = hTimer;

	for(;;)
	{
		SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, FALSE);
		WaitForMultipleObjects(2, hObjList, TRUE, INFINITE);
		
		// generate the tick interrupt
		dwPendingIsr |= (1<<CPU_INTR_TICK);
		SetEvent(hIsrInvoke);

		// wait till interrupt handler acknowledges the interrupt (avoids
		// overruns).
		SignalObjectAndWait(hIsrMutex, hTickAck, INFINITE, FALSE);
	}
}

static DWORD WINAPI TaskSimThread( LPVOID lpParameter )
{
	SSIM_T *psSim=(SSIM_T*)lpParameter;
	ulCriticalNesting = psSim->ulCriticalNesting;

	psSim->entry(psSim->pvParameters);

	return 0;
}

static void create_system_objects(void)
{
	DuplicateHandle(
		GetCurrentProcess(),
		GetCurrentThread(),
		GetCurrentProcess(),
		&hIsrDispatcher,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS);

	SetThreadPriority(hIsrDispatcher, THREAD_PRIORITY_BELOW_NORMAL);

	hIsrMutex = CreateMutex(NULL, FALSE, NULL);
	hIsrInvoke = CreateEvent(NULL, FALSE, FALSE, NULL);
	hTickAck = CreateEvent(NULL, FALSE, FALSE, NULL);
	hTermAck = CreateEvent(NULL, FALSE, FALSE, NULL);

	dwEnabledIsr |= (1<<CPU_INTR_TICK);

#if configUSE_PREEMPTION != 0
	SetThreadPriority(CreateThread(NULL, 0, tick_generator, NULL, 0, NULL),
		THREAD_PRIORITY_ABOVE_NORMAL);
#endif
}

/******************************************************************************
	Global functions
******************************************************************************/
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
	BOOL ok;
	SSIM_T *psSim;

	psSim=(SSIM_T*)(pxTopOfStack - sizeof(SSIM_T));
	psSim->entry = ( void (*)(void *)) pxCode;
	psSim->pvParameters=pvParameters;
	psSim->ulCriticalNesting=portNO_CRITICAL_NESTING;
	psSim->dwGlobalIsr = (DWORD)-1;

	// semaphore that's used to handle yielding
	psSim->hSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	psSim->yielded = FALSE;
	psSim->hThread = CreateThread(NULL, 0, TaskSimThread, psSim, CREATE_SUSPENDED, NULL);
	ok=SetThreadPriority(psSim->hThread, THREAD_PRIORITY_IDLE);
	return (portSTACK_TYPE *) psSim;
}

portBASE_TYPE xPortStartScheduler( void )
{
	BOOL bSwitch;
	SSIM_T *psSim;
	DWORD dwIntr;
	int i, iIsrCount;
	HANDLE hObjList[2]; 

	create_system_objects();

	/* Start the first task. */
	psSim=(SSIM_T *)*pxCurrentTCB;
	ulCriticalNesting = portNO_CRITICAL_NESTING;
	ResumeThread(psSim->hThread);

	hObjList[0] = hIsrMutex;
	hObjList[1] = hIsrInvoke;

	WaitForSingleObject(hIsrMutex, INFINITE);
	dwGlobalIsr = psSim->dwGlobalIsr;
	ReleaseMutex(hIsrMutex);

	for(;;)
	{
		WaitForMultipleObjects(2, hObjList, TRUE, INFINITE);

		psSim=(SSIM_T *)*pxCurrentTCB;

		psSim->ulCriticalNesting = ulCriticalNesting ;
		psSim->dwGlobalIsr = dwGlobalIsr;

		dwIntr = ((dwGlobalIsr & dwEnabledIsr) & dwPendingIsr);

		bSwitch = (dwIntr != 0);	// switch only for a real interrupt
		iIsrCount = 0;			// no suspensions after handling first int

		for(i=0; dwIntr && i<CPU_INTR_COUNT; i++)
		{
			if(dwIntr & (1<<i))
			{
				dwIntr &= ~(1<<i);

				switch(i)
				{
				case CPU_INTR_SWI:
					dwPendingIsr &= ~(1<<CPU_INTR_SWI);

					if(ESWI_ID == SWI_ID_TERMINATE)
					{
						SetEvent(hTermAck);
						ReleaseMutex(hIsrMutex);
						return 0;
					}

					psSim->yielded = TRUE;
					iIsrCount++;
					break;

				case CPU_INTR_TICK:
					dwPendingIsr &= ~(1<<CPU_INTR_TICK);

					if(!iIsrCount++)
						SuspendThread(psSim->hThread);

					vTaskIncrementTick();
					SetEvent(hTickAck);
					break;

				default:
					if(!iIsrCount++)
						SuspendThread(psSim->hThread);

					vIsrHandler[i]();
					break;
				}
			}
		}

		if(bSwitch)
		{
			vTaskSwitchContext();
			psSim=(SSIM_T *)*pxCurrentTCB;
			ulCriticalNesting = psSim->ulCriticalNesting;
			dwGlobalIsr = psSim->dwGlobalIsr;
			
			if(psSim->yielded) {
				psSim->yielded = FALSE;
				ReleaseSemaphore(psSim->hSemaphore, 1, NULL);		// awake next task
			} else {
				ResumeThread(psSim->hThread);
			}
		}

		if((dwGlobalIsr & dwEnabledIsr) & dwPendingIsr)
		{
			// we just enabled interrupts (by task switching in) and an interrupt is
			// pending. Reinvoke ourselves
			SetEvent(hIsrInvoke);
		}

		ReleaseMutex(hIsrMutex);
	}
}

void vPortEndScheduler( void )
{
	if(hIsrDispatcher)
	{
		WaitForSingleObject(hIsrMutex, INFINITE);
		dwPendingIsr |= (1<<CPU_INTR_SWI);
		ESWI_ID = SWI_ID_TERMINATE;
		SetEvent(hIsrInvoke);
		SignalObjectAndWait(hIsrMutex, hTermAck, INFINITE, FALSE);
	}
}

void __disable_interrupt(void)
{
	if(hIsrDispatcher)
	{
		WaitForSingleObject(hIsrMutex, INFINITE);
		dwGlobalIsr = NMI;
		ReleaseMutex(hIsrMutex);
	}
	else
	{
		dwGlobalIsr = NMI;
	}
}

void __enable_interrupt(void)
{
	if(hIsrDispatcher)
	{
		WaitForSingleObject(hIsrMutex, INFINITE);
		dwGlobalIsr = (DWORD)-1;
		
		if(dwPendingIsr)
		{
			SetEvent(hIsrInvoke);
		}
		ReleaseMutex(hIsrMutex);
	}
	else
	{
		dwGlobalIsr = (DWORD)-1;
	}
}

void __swi(void)
{
	SSIM_T *psSim=(SSIM_T *)*pxCurrentTCB;

	if(hIsrDispatcher)
	{
		WaitForSingleObject(hIsrMutex, INFINITE);
		dwPendingIsr |= (1<<CPU_INTR_SWI);
		ESWI_ID = SWI_ID_YIELD;
		SetEvent(hIsrInvoke);
		SignalObjectAndWait(hIsrMutex, psSim->hSemaphore, INFINITE, FALSE);
	}
}

void __generate_interrupt(int iNo)
{
	if(hIsrDispatcher)
	{
		WaitForSingleObject(hIsrMutex, INFINITE);
		dwPendingIsr |= (1<<iNo);
		SetEvent(hIsrInvoke);
		ReleaseMutex(hIsrMutex);
	}
}

int iPortSetIsrHandler(int iNo, void (*handler)(void))
{
	if(iNo < CPU_INTR_COUNT) {
		if(hIsrDispatcher) {
			SSIM_T *psSim=(SSIM_T *)*pxCurrentTCB;

			WaitForSingleObject(hIsrMutex, INFINITE);
			vIsrHandler[iNo]=handler;
			ReleaseMutex(hIsrMutex);
		}
		else
		{
			vIsrHandler[iNo]=handler;
		}

		return 0;
	}

	return -1;
}

void vPortEnableInt(int iNo)
{
	if(hIsrDispatcher)
	{
		WaitForSingleObject(hIsrMutex, INFINITE);
		dwEnabledIsr |= (1<<iNo);

		if((dwGlobalIsr & dwEnabledIsr) & dwPendingIsr)
		{
			SetEvent(hIsrInvoke);
		}

		ReleaseMutex(hIsrMutex);
	}
	else
	{
		dwEnabledIsr |= (1<<iNo);
	}
}

void vPortDisableInt(int iNo)
{
	if(hIsrDispatcher)
	{
		WaitForSingleObject(hIsrMutex, INFINITE);
		dwEnabledIsr &= ~(1<<iNo);
		ReleaseMutex(hIsrMutex);
	}
	else
	{
		dwEnabledIsr &= ~(1<<iNo);
	}
}

BOOL bPortIsEnabledInt(int iNo)
{
	DWORD dwEnabled;

	if(hIsrDispatcher)
	{
		WaitForSingleObject(hIsrMutex, INFINITE);
		dwEnabled = dwEnabledIsr;
		ReleaseMutex(hIsrMutex);
	}
	else
	{
		dwEnabled = dwEnabledIsr;
	}

	return (dwEnabled & (1<<iNo)) != 0;
}

void vPortEnterCritical( void )
{
	/* Disable interrupts first! */
	__disable_interrupt();

	/* Now interrupts are disabled ulCriticalNesting can be accessed
	directly.  Increment ulCriticalNesting to keep a count of how many times
	portENTER_CRITICAL() has been called. */
	ulCriticalNesting++;
}

void vPortExitCritical( void )
{
	if( ulCriticalNesting > portNO_CRITICAL_NESTING )
	{
		/* Decrement the nesting count as we are leaving a critical section. */
		ulCriticalNesting--;

		/* If the nesting level has reached zero then interrupts should be
		re-enabled. */
		if( ulCriticalNesting == portNO_CRITICAL_NESTING )
		{
			__enable_interrupt();
		}
	}
}

/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2004, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            WIN32 Sample Code
*
* File : APP.C
* By   : Eric Shufro
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                                CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE    128
#define  TASK_START_PRIO    5
#define  TASK_1_PRIO		(TASK_START_PRIO+1)
#define  TASK_2_PRIO		(TASK_START_PRIO+2)
#define  TASK_1_ID			(TASK_START_PRIO+1)
#define  TASK_2_ID			(TASK_START_PRIO+2)


/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

OS_STK        AppStartTaskStk[TASK_STK_SIZE];
OS_STK        AppTask1Stk[TASK_STK_SIZE];
OS_STK        AppTask2Stk[TASK_STK_SIZE];

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppStartTask(void *p_arg);
static  void  AppTask1(void *p_arg);
static  void  AppTask2(void *p_arg);

#if OS_VIEW_MODULE > 0
static  void  AppTerminalRx(INT8U rx_data);
#endif

/*
*********************************************************************************************************
*                                                _tmain()
*
* Description : This is the standard entry point for C++ WIN32 code.  
* Arguments   : none
*********************************************************************************************************
*/

void main(int argc, char *argv[])
{
	INT8U  err;


#if 0
    BSP_IntDisAll();                       /* For an embedded target, disable all interrupts until we are ready to accept them */
#endif

    OSInit();                              /* Initialize "uC/OS-II, The Real-Time Kernel"                                      */

    OSTaskCreateExt(AppStartTask,
                    (void *)0,
                    (OS_STK *)&AppStartTaskStk[TASK_STK_SIZE-1],
                    TASK_START_PRIO,
                    TASK_START_PRIO,
                    (OS_STK *)&AppStartTaskStk[0],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskCreateExt(AppTask1,
                    (void *)0,
                    (OS_STK *)&AppTask1Stk[TASK_STK_SIZE-1],
                    TASK_1_PRIO,
                    TASK_1_ID,
                    (OS_STK *)&AppTask1Stk[0],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskCreateExt(AppTask2,
                    (void *)0,
                    (OS_STK *)&AppTask2Stk[TASK_STK_SIZE-1],
                    TASK_2_PRIO,
                    TASK_2_ID,
                    (OS_STK *)&AppTask2Stk[0],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

					
#if OS_TASK_NAME_SIZE > 11
    OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)"Start Task", &err);
#endif

#if OS_TASK_NAME_SIZE > 14
    OSTaskNameSet(OS_IDLE_PRIO, (INT8U *)"uC/OS-II Idle", &err);
#endif

#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    OSTaskNameSet(OS_STAT_PRIO, "uC/OS-II Stat", &err);
#endif

    OSStart();                             /* Start multitasking (i.e. give control to uC/OS-II)                               */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/

void  AppStartTask (void *p_arg)
{
    p_arg = p_arg;

#if 0
    BSP_Init();                                  /* For embedded targets, initialize BSP functions                             */
#endif


#if OS_TASK_STAT_EN > 0
    OSStatInit();                                /* Determine CPU capacity                                                     */
#endif
    
    while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{       		
		OS_Printf("StartTask: Delay 1 second and print\n");  /* your code here. Create more tasks, etc.                                    */
        OSTimeDlyHMSM(0, 0, 1, 0);       
    }
}


void  AppTask1 (void *p_arg)
{
    p_arg = p_arg;

#if 0
    BSP_Init();                                  /* For embedded targets, initialize BSP functions                             */
#endif


#if OS_TASK_STAT_EN > 0
//    OSStatInit();                                /* Determine CPU capacity                                                     */
#endif
    
    while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{       		
		OS_Printf("Task1:Delay 500 ms and print\n");  /* your code here. Create more tasks, etc.                                    */
        OSTimeDlyHMSM(0, 0, 0, 500);       
    }
}


void  AppTask2 (void *p_arg)
{
    p_arg = p_arg;

#if 0
    BSP_Init();                                  /* For embedded targets, initialize BSP functions                             */
#endif


#if OS_TASK_STAT_EN > 0
//    OSStatInit();                                /* Determine CPU capacity                                                     */
#endif
    
    while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{       		
		OS_Printf("Task2:Delay 2 sec and print\n");  /* your code here. Create more tasks, etc.                                    */
        OSTimeDlyHMSM(0, 0, 2, 0);       
    }
}












///for v2.9.0
void OSTaskReturnHook(OS_TCB *ptcb)
{
	///none: must export by user in v2.9.0
}
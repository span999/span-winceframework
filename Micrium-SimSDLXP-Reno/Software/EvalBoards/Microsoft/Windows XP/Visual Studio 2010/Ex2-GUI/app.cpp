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

#define  TASK_STK_SIZE		128
#define  TASK_START_PRIO    5
#define  TASK_1_PRIO		(TASK_START_PRIO+1)
#define  TASK_2_PRIO		(TASK_START_PRIO+2)
#define  TASK_3_PRIO		(TASK_START_PRIO+3)
#define  TASK_4_PRIO		(TASK_START_PRIO+4)
#define  TASK_5_PRIO		(TASK_START_PRIO+5)
#define  TASK_1_ID			(TASK_START_PRIO+1)
#define  TASK_2_ID			(TASK_START_PRIO+2)
#define  TASK_3_ID			(TASK_START_PRIO+3)
#define  TASK_4_ID			(TASK_START_PRIO+4)
#define  TASK_5_ID			(TASK_START_PRIO+5)


/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

OS_STK		AppStartTaskStk[TASK_STK_SIZE];
OS_STK		AppTask1Stk[TASK_STK_SIZE];
OS_STK      AppTask2Stk[TASK_STK_SIZE];

OS_STK		AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE];
OS_STK		AppTaskKbdStk[APP_TASK_KBD_STK_SIZE];
OS_STK		AppTaskSDLIOStk[APP_TASK_SDLIO_STK_SIZE];


/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppStartTask(void *p_arg);
static  void  AppTask1(void *p_arg);
static  void  AppTask2(void *p_arg);

static  void  AppTaskCreate(void);
static void AppTaskUserIF(void *p_arg);
static void AppTaskKbd(void *p_arg);
static void AppTaskSDLio(void *p_arg);


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

#ifdef OS_PLUS_GUI
void main(int argc, char *argv[])
#else
void __main(int argc, char *argv[])
#endif
{
	INT8U  err;

	OS_Printf("\nmain start!!!\n");
	fflush(stdout);
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
///    OSTaskNameSet(OS_IDLE_PRIO, (INT8U *)"uC/OS-II Idle", &err);
#endif

#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
///    OSTaskNameSet(OS_STAT_PRIO, "uC/OS-II Stat", &err);
#endif

	AppTaskCreate();		


	OS_Printf("\nabout to start all tasks !!!\n");
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
		OS_Printf("StartTask: Delay 2 second and print\n");  /* your code here. Create more tasks, etc.                                    */
        OSTimeDlyHMSM(0, 0, 2, 0);       
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
		OS_Printf("Task1:Delay 5000 ms and print\n");  /* your code here. Create more tasks, etc.                                    */
        OSTimeDlyHMSM(0, 0, 0, 5000);       
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
    
///	MainTask();
	
    while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{       		
		OS_Printf("Task2:Delay 2.7 sec and print\n");  /* your code here. Create more tasks, etc.                                    */
        OSTimeDlyHMSM(0, 0, 2, 700);       
    }
}




/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppTaskCreate(void)
{
  INT8U  err;

  OSTaskCreateExt(AppTaskUserIF,
					(void *)0,
					(OS_STK *)&AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE-1],
			//		APP_TASK_USER_IF_PRIO,
					TASK_3_PRIO,
					TASK_3_ID,
					(OS_STK *)&AppTaskUserIFStk[0],
                    APP_TASK_USER_IF_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);


#if (OS_TASK_NAME_SIZE > 8)
  OSTaskNameSet(APP_TASK_USER_IF_PRIO, (INT8U*)"User_IF", &err);
#endif


  OSTaskCreateExt(AppTaskKbd,
					(void *)0,
					(OS_STK *)&AppTaskKbdStk[APP_TASK_KBD_STK_SIZE-1],
			//		APP_TASK_KBD_PRIO,
					TASK_4_PRIO,
					TASK_4_ID,
					(OS_STK *)&AppTaskKbdStk[0],
                    APP_TASK_KBD_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);

#if (OS_TASK_NAME_SIZE > 8)
  OSTaskNameSet(APP_TASK_KBD_PRIO, (INT8U*)"Keyboard", &err);
#endif


  OSTaskCreateExt(AppTaskSDLio,
					(void *)0,
					(OS_STK *)&AppTaskSDLIOStk[APP_TASK_SDLIO_STK_SIZE-1],
			//		APP_TASK_KBD_PRIO,
					TASK_5_PRIO,
					TASK_5_ID,
					(OS_STK *)&AppTaskSDLIOStk[0],
                    APP_TASK_SDLIO_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);

#if (OS_TASK_NAME_SIZE > 8)
  OSTaskNameSet(TASK_5_PRIO, (INT8U*)"SDL_IO", &err);
#endif

  
}


#if defined(OS_PLUS_GUI)
//void MainTask_GUI(void);
extern "C" void MainTask_GUI(void);
#endif

/*
*********************************************************************************************************
*                                         USER INTERFACE TASK
*
* Description : This task updates the LCD screen based on messages passed to it by AppTaskKbd().
*
* Arguments   : p_arg   is the argument passed to 'AppStartUserIF()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

static  void  AppTaskUserIF (void *p_arg)
{
	OS_Printf("\nAppTaskUserIF !!!\n");
	///(void)p_arg;

	// GUI_Init();
	while(DEF_TRUE) 
    {
		OS_Printf("\nAppTaskUserIF !!!\n");
		OSTimeDlyHMSM(0,0,1,0);
		MainTask_GUI(); 
//	  	GUIDEMO_Touch();
		break;
    }
}


/*
*********************************************************************************************************
*                                    KEYBOARD RESPONSE TASK
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Arguments   : p_arg   is the argument passed to 'AppStartKbd()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/
extern void TaskKbdHook( void * pvParameters );

static  void  AppTaskKbd (void *p_arg)
{
	///u8 tick=0;
	INT8U tick = 0;
	(void)p_arg;
	
	OSTimeDlyHMSM(0,0,0,800);
	
	while(DEF_TRUE) 
    {
#if 1	
		tick++;
        ///OSTimeDlyHMSM(0,0,0,10);
		OSTimeDlyHMSM(0,0,0,800);

///		if( thouch changed )
///		GUI_TOUCH_Exec();
///		if( key changed )
///		GUI_TOUCH_Exec();
 
		if(tick&0x10)
		{
		    ///GPIO_SetBits(GPIOD,GPIO_Pin_8);
			///GPIO_ResetBits(GPIOD,GPIO_Pin_11);
			OS_Printf("\nAppTaskKbd !!\n");
		}
		else
		{
		 	///GPIO_ResetBits(GPIOD,GPIO_Pin_8);
			///GPIO_SetBits(GPIOD,GPIO_Pin_11);
		}
	   ;
#else
		///TaskKbdHook(0);
#endif   
	   /*	 
		OSTimeDlyHMSM(0,0,0,500);
		*/
	}
}


extern void SDLIODriver( void * pvParameters );

static  void  AppTaskSDLio (void *p_arg)
{
	
	(void)p_arg;
	OS_Printf("\nAppTaskSDLio !!");
	SDLIODriver(0);
}







///for v2.9.0
void OSTaskReturnHook(OS_TCB *ptcb)
{
	///none: must export by user in v2.9.0
}
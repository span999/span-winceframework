/*
*********************************************************************************************************
*                                       APPLICATION CONFIGURATION
*
*                             (c) Copyright 2005, Micrium, Inc., Weston, FL
*                                          All Rights Reserved
*
*                                            Philips LPC2000
*********************************************************************************************************
*/

#define		DEF_TRUE		1
#define		DEF_FALSE		0

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO          4

#define  APP_TASK_KBD_PRIO                     4
#define  APP_TASK_USER_IF_PRIO                 6

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE    128

#define  APP_TASK_USER_IF_STK_SIZE  256
#define  APP_TASK_KBD_STK_SIZE      256
#define  APP_TASK_SDLIO_STK_SIZE    128


#define	 OS_TASK_TMR_PRIO			3

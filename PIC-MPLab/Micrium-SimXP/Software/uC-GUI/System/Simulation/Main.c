/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : Main.c
Purpose     : Call of MainTask
--------------------END-OF-HEADER-------------------------------------
*/

///void MainTask(void);

#ifdef TEST_PROJ
void MainTask_test(void);
#endif
#ifdef MEDIT_PROJ
void MainTask_MultiEdit(void);
#endif
#ifdef EYES_PROJ
void MainTask_eyes(void);
#endif
#ifdef GUIDEMO_PROJ
void GUIDEMO_main(void);
#endif
#ifdef DASHBOARD_PROJ
void MainTask_DashB(void);
#endif

///void main(void) {
	///MainTask_test
	///MainTask();
	///MainTask_multi();
///}

void MainTask(void) {
#ifdef TEST_PROJ
	MainTask_test();
#endif
#ifdef EYES_PROJ
	MainTask_eyes();
#endif
	///MainTask_multi();
#ifdef MEDIT_PROJ
	MainTask_MultiEdit();
#endif
#ifdef GUIDEMO_PROJ
	///GUI_Init();
	GUIDEMO_main();
#endif
#ifdef DASHBOARD_PROJ	
	MainTask_DashB();
#endif	
}
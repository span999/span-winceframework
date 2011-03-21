/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
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
void GUIDEMO_main(void);

///void main(void) {
	///MainTask_test
	///MainTask();
	///MainTask_multi();
///}

void MainTask(void) {
	///MainTask_test();
	///MainTask_eyes();
	///MainTask_multi();
	///MainTask_MultiEdit();
	GUIDEMO_main();
}
/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <sys/time.h>

#include "helper.h"
#include "tasks.h"





int task00_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task01_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task02_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);
	_SPMSG(dINIT|dBEGN, "%s:%s Timer task start !!!\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task03_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task04_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task05_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task06_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task07_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task08_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task09_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task10_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task11_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task12_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task13_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task14_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}

int task15_begin( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT|dBEGN, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);

	_SPMSG(dINIT|dBEGN, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}



/* ---------------------------------------------------------------------- */


int task00_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task01_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task02_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);
	_SPMSG(dINIT|dDUTY, "%s:%s Timer task duty !!!\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task03_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task04_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task05_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task06_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task07_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task08_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task09_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task10_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task11_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task12_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task13_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task14_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task15_duty( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	iRet = TASK_IST_REQUEST;
	_SPMSG(dDUTY, "%s:%s TASK_IST_REQUEST\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}





int task00_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task01_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task02_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);
	_SPMSG(dINIT|dIST, "%s:%s Timer task IST !!!\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task03_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task04_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task05_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task06_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task07_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task08_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task09_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task10_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task11_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task12_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task13_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task14_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}

int task15_IST( int iParam ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	MsSleep(50);
	_SPMSG(dIST, "%s:%s running\r\n", _MSGHEAD_, __func__);

	_SPMSG(dINIT, "%s:%s iRet=[%d]--\r\n", _MSGHEAD_, __func__, iRet);

	return iRet;
}


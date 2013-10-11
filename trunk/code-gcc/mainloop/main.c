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
#include "usertasks.h"




TaskList tasks = {
	{
	TASK_VALID, TASK_VALID, TASK_VALID, TASK_VALID, \
	TASK_INVALID, TASK_INVALID, TASK_INVALID, TASK_INVALID, \
	TASK_INVALID, TASK_INVALID, TASK_INVALID, TASK_INVALID, \
	TASK_INVALID, TASK_INVALID, TASK_INVALID, TASK_INVALID
	},
	{
	TASK_PRIOR_NORNAL, TASK_PRIOR_HIGH, TASK_PRIOR_LOW, TASK_PRIOR_NORNAL, \
	TASK_PRIOR_NORNAL, TASK_PRIOR_NORNAL, TASK_PRIOR_NORNAL, TASK_PRIOR_NORNAL, \
	TASK_PRIOR_NORNAL, TASK_PRIOR_NORNAL, TASK_PRIOR_NORNAL, TASK_PRIOR_NORNAL, \
	TASK_PRIOR_NORNAL, TASK_PRIOR_NORNAL, TASK_PRIOR_NORNAL, TASK_PRIOR_NORNAL
	},
	{
	TASK_IST_NONREQ, TASK_IST_NONREQ, TASK_IST_NONREQ, TASK_IST_NONREQ, \
	TASK_IST_NONREQ, TASK_IST_NONREQ, TASK_IST_NONREQ, TASK_IST_NONREQ, \
	TASK_IST_NONREQ, TASK_IST_NONREQ, TASK_IST_NONREQ, TASK_IST_NONREQ, \
	TASK_IST_NONREQ, TASK_IST_NONREQ, TASK_IST_NONREQ, TASK_IST_NONREQ
	},
	{
	{task00_begin, task00_duty, task00_begin, task00_begin, task00_IST}, \
	{task01_begin, task01_duty, task01_begin, task01_begin, task01_IST}, \
	{task02_begin, task02_duty, task02_begin, task02_begin, task02_IST}, \
	{task03_begin, task03_duty, task03_begin, task03_begin, task03_IST}, \
	{task04_begin, task04_duty, task04_begin, task04_begin, task04_IST}, \
	{task05_begin, task05_duty, task05_begin, task05_begin, task05_IST}, \
	{task06_begin, task06_duty, task06_begin, task06_begin, task06_IST}, \
	{task07_begin, task07_duty, task07_begin, task07_begin, task07_IST}, \
	{task08_begin, task08_duty, task08_begin, task08_begin, task08_IST}, \
	{task09_begin, task09_duty, task09_begin, task09_begin, task09_IST}, \
	{task10_begin, task10_duty, task10_begin, task10_begin, task10_IST}, \
	{task11_begin, task11_duty, task11_begin, task11_begin, task11_IST}, \
	{task12_begin, task12_duty, task12_begin, task12_begin, task12_IST}, \
	{task13_begin, task13_duty, task13_begin, task13_begin, task13_IST}, \
	{task14_begin, task14_duty, task14_begin, task14_begin, task14_IST}, \
	{task15_begin, task15_duty, task15_begin, task15_begin, task15_IST}
	}
};



int main( int argc, char *argv[] ) {
	int iRet = -1;
	
	printf("%s:%s ++\r\n", _MSGHEAD_, __func__);

	TasksDump( &tasks );
	TasksStarter( &tasks );

	for(;;)
		TasksPicker( &tasks );

	printf("%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}




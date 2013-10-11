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







void TasksDump( TaskList *ptasklist ) {
	
	printf("%s:%s ++\r\n", _MSGHEAD_, __func__);
	if(ptasklist)
	{
		int tmp = 0;
		for(tmp=0;tmp<16;tmp++)
		{
			printf("%s:%s valid[%d]=%d\r\n", _MSGHEAD_, __func__, tmp, ptasklist->valid[tmp]);
			printf("%s:%s tasks[%d].begin=0x%08x\r\n", _MSGHEAD_, __func__, tmp, (unsigned)ptasklist->tasks[tmp].begin);
		}
	}

	printf("%s:%s --\r\n", _MSGHEAD_, __func__);
}


int TasksStarter( TaskList *ptasklist ) {
	int iRet = -1;
	
	printf("%s:%s ++\r\n", _MSGHEAD_, __func__);

	if(ptasklist)
	{
		int tmp = 0;
		for(tmp=0;tmp<16;tmp++)
		{
			if(ptasklist->valid[tmp] > 0)
				if(ptasklist->tasks[tmp].begin > 0)
					ptasklist->tasks[tmp].begin(0);
				else
					printf("%s:%s tasks[%d].begin=NULL\r\n", _MSGHEAD_, __func__, tmp);
			else
				printf("%s:%s valid[%d]=%d\r\n", _MSGHEAD_, __func__, tmp, ptasklist->valid[tmp]);
		}
	}	

	printf("%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}


int TasksPicker( TaskList *ptasklist ) {
	int iRet = -1;
	
	printf("%s:%s ++\r\n", _MSGHEAD_, __func__);

	if(ptasklist)
	{
		int tmp = 0;
		int count = 0;
		int priority = 0;

		/* check available tasks first */
		for(tmp=0;tmp<16;tmp++)
		{
			if(ptasklist->valid[tmp] > 0)
			{
				if(ptasklist->tasks[tmp].duty > 0)
				{
					ptasklist->requested[tmp] = ptasklist->tasks[tmp].duty(0);
					count++;
				}
				else
					printf("%s:%s tasks[%d].duty=NULL\r\n", _MSGHEAD_, __func__, tmp);
			}
			else
			{
				;///printf("%s:%s valid[%d]=%d\r\n", _MSGHEAD_, __func__, tmp, ptasklist->valid[tmp]);
			}
		}
		printf("%s:%s avalable task=%d\r\n", _MSGHEAD_, __func__, count);

		/* now, execute duty with priority */
		priority = TASK_PRIOR_MAX;	/* start with first priority */
		while( (priority>(TASK_PRIOR_NONE)) && (count>0) )
		{
			printf("%s:%s check avalable task for priority=%d\r\n", _MSGHEAD_, __func__, priority);
			for(tmp=0;tmp<16;tmp++)
			{
				if(ptasklist->valid[tmp] > 0)
				{
					if(ptasklist->tasks[tmp].IST > 0)
					{
						if(ptasklist->priority[tmp] == priority)
						{
							ptasklist->tasks[tmp].IST(0);
							count--;
							ptasklist->requested[tmp] = TASK_IST_NONREQ;
						}
					}
					else
					{
						printf("%s:%s tasks[%d].duty=NULL\r\n", _MSGHEAD_, __func__, tmp);
					}
				}
				else
				{
					;///printf("%s:%s valid[%d]=%d\r\n", _MSGHEAD_, __func__, tmp, ptasklist->valid[tmp]);
				}
			}
			printf("%s:%s avalable task=%d\r\n", _MSGHEAD_, __func__, count);
			priority--;
		}
	}

	MsSleep(500);

	printf("%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}



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
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	if(ptasklist)
	{
		int tmp = 0;
		for(tmp=0;tmp<16;tmp++)
		{
			_SPMSG(dDUMP, "%s:%s valid[%d]=%d\r\n", _MSGHEAD_, __func__, tmp, ptasklist->valid[tmp]);
			_SPMSG(dDUMP, "%s:%s tasks[%d].begin=0x%08lu\r\n", _MSGHEAD_, __func__, tmp, (unsigned long)ptasklist->tasks[tmp].begin);
		}
	}

	_SPMSG(dINIT, "%s:%s --\r\n", _MSGHEAD_, __func__);
}


int TasksStarter( TaskList *ptasklist ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

	if(ptasklist)
	{
		int tmp = 0;
		for(tmp=0;tmp<16;tmp++)
		{
			if(ptasklist->valid[tmp] > 0)
			{
				if(ptasklist->tasks[tmp].begin > 0)
				{
					_SPMSG(dTIME, "%s:%s GetTime=%lu\r\n", _MSGHEAD_, __func__, _GetTime() );
					ptasklist->tasks[tmp].begin(0);
					_SPMSG(dTIME, "%s:%s GetTimeDiff=%lu\r\n", _MSGHEAD_, __func__, _GetTimeDiff() );
				}
				else
				{
					_SPMSG(dERR, "%s:%s tasks[%d].begin=NULL\r\n", _MSGHEAD_, __func__, tmp);
				}
			}
			else
			{
				_SPMSG(dINFO, "%s:%s valid[%d]=%d\r\n", _MSGHEAD_, __func__, tmp, ptasklist->valid[tmp]);
			}
		}
	}	

	_SPMSG(dINIT, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}


int TasksPicker( TaskList *ptasklist ) {
	int iRet = -1;
	
	_SPMSG(dINIT, "%s:%s ++\r\n", _MSGHEAD_, __func__);

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
					_SPMSG(dTIME, "%s:%s GetTime=%lu\r\n", _MSGHEAD_, __func__, _GetTime() );
					ptasklist->requested[tmp] = ptasklist->tasks[tmp].duty(0);
					_SPMSG(dTIME, "%s:%s GetTimeDiff=%lu\r\n", _MSGHEAD_, __func__, _GetTimeDiff() );
					count++;
				}
				else
					_SPMSG(dERR, "%s:%s tasks[%d].duty=NULL\r\n", _MSGHEAD_, __func__, tmp);
			}
			else
			{
				_SPMSG(dINFO, "%s:%s valid[%d]=%d\r\n", _MSGHEAD_, __func__, tmp, ptasklist->valid[tmp]);
			}
		}
		_SPMSG(dINFO, "%s:%s avalable task=%d\r\n", _MSGHEAD_, __func__, count);

		/* now, execute duty with priority */
		priority = TASK_PRIOR_MAX;	/* start with first priority */
		while( (priority>(TASK_PRIOR_NONE)) && (count>0) )
		{
			_SPMSG(dINFO, "%s:%s check avalable task for priority=%d\r\n", _MSGHEAD_, __func__, priority);
			for(tmp=0;tmp<16;tmp++)
			{
				if(ptasklist->valid[tmp] > 0)
				{
					if(ptasklist->tasks[tmp].IST > 0)
					{
						if(ptasklist->priority[tmp] == priority)
						{
							_SPMSG(dTIME, "%s:%s GetTime=%lu\r\n", _MSGHEAD_, __func__, _GetTime() );
							ptasklist->tasks[tmp].IST(0);
							_SPMSG(dTIME, "%s:%s GetTimeDiff=%lu\r\n", _MSGHEAD_, __func__, _GetTimeDiff() );
							count--;
							ptasklist->requested[tmp] = TASK_IST_NONREQ;
						}
					}
					else
					{
						_SPMSG(dERR, "%s:%s tasks[%d].duty=NULL\r\n", _MSGHEAD_, __func__, tmp);
					}
				}
				else
				{
					_SPMSG(dINFO, "%s:%s valid[%d]=%d\r\n", _MSGHEAD_, __func__, tmp, ptasklist->valid[tmp]);
				}
			}
			_SPMSG(dINFO, "%s:%s avalable task=%d\r\n", _MSGHEAD_, __func__, count);
			priority--;
		}
	}

	MsSleep(500);

	_SPMSG(dINIT, "%s:%s --\r\n", _MSGHEAD_, __func__);

	return iRet;
}



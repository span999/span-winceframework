/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <sys/time.h>

#include "cpuutil.h"







int main()
{
	int return_value = 0;
	pthread_t thread_id;
	struct ProcStatNums	 NowChk;
	struct ProcStatNums	 OldChk;
	struct ProcStatNums	 DiffChk;
	struct ProcStatNums	 NowChk0;
	struct ProcStatNums	 OldChk0;
	struct ProcStatNums	 DiffChk0;
	struct ProcStatNums	 NowChk1;
	struct ProcStatNums	 OldChk1;
	struct ProcStatNums	 DiffChk1;
	struct ProcStatNums	 *pThis;

	int iTmp = 0;
	double iValue = 0;
	double iValue0 = 0;
	double iValue1 = 0;
	
	if( -1 == getProcStat( &OldChk, -1 ) )
	{
		printf( "\ngetProcStat failed !!\n" );
		goto _ERROR;		
	}

	if( -1 == getProcStat( &OldChk0, 0 ) )
	{
		printf( "\ngetProcStat 0 failed !!\n" );
		///goto _ERROR;		
	}

	if( -1 == getProcStat( &OldChk1, 1 ) )
	{
		printf( "\ngetProcStat 1 failed !!\n" );
		///goto _ERROR;		
	}


	while( iTmp++ < 100 )
	{
		sleep( 1 );

		if( -1 == getProcStat( &NowChk, -1 ) )
		{
			printf( "\ngetProcStat failed !!\n" );
			goto _ERROR;		
		}
		else
		{
			updateNUM( &OldChk, &NowChk, &DiffChk );
		}

		if( -1 == getProcStat( &NowChk0, 0 ) )
		{
			printf( "\ngetProcStat 0 failed !!\n" );
			///goto _ERROR;		
		}
		else
		{
			updateNUM( &OldChk0, &NowChk0, &DiffChk0 );
		}

		if( -1 == getProcStat( &NowChk1, 1 ) )
		{
			printf( "\ngetProcStat 1 failed !!\n" );
			///goto _ERROR;		
		}
		else
		{
			updateNUM( &OldChk1, &NowChk1, &DiffChk1 );
		}


#if 0
		pThis = &DiffChk;
		iValue = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM))*100 )/( pThis->sumNUM );
		pThis = &DiffChk0;
		iValue0 = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM))*100 )/( pThis->sumNUM );
		pThis = &DiffChk1;
		iValue1 = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM))*100 )/( pThis->sumNUM );
#else
		pThis = &DiffChk;
		iValue = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM)+(pThis->iowaitNUM)+(pThis->irqNUM)+(pThis->softirqNUM))*100 )/( pThis->sumNUM );
		pThis = &DiffChk0;
		iValue0 = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM)+(pThis->iowaitNUM)+(pThis->irqNUM)+(pThis->softirqNUM))*100 )/( pThis->sumNUM );
		pThis = &DiffChk1;
		iValue1 = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM)+(pThis->iowaitNUM)+(pThis->irqNUM)+(pThis->softirqNUM))*100 )/( pThis->sumNUM );
#endif
		///printf( "CPU usage: %3.2f%%.[0:%3.2f%%]\n", iValue, iValue0 );
		printf( "CPU usage:%3.2f%%.[0:%3.2f%%][1:%3.2f%%]\n", iValue, iValue0, iValue1 );
	}	///while


	printf("The process ID is %d\n", (int)getpid() );
	printf("The parent process ID is %d\n", (int)getppid() );

	///pthread_create( &thread_id, NULL, &print_xs, NULL );

	printf( "\ndone \n" );
	return 0;

_ERROR:
	printf( "\nsomething wrong !!\n" );
	return return_value;
}


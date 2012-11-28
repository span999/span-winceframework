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
#include "framebuffer.h"





#define	CHECKLOOP		5


int main( int argc, char *argv[] )
{
	int return_value = 0;
	pthread_t thread_id;
	
#ifdef _USE_NO_GREP_	/* cpuutil.h */
	struct ProcStatSets  StatSetsOld;
	struct ProcStatSets  StatSetsNow;
	struct ProcStatSets  StatSetsDiff;
#else
	struct ProcStatNums	 NowChk;
	struct ProcStatNums	 OldChk;
	struct ProcStatNums	 DiffChk;
	struct ProcStatNums	 NowChk0;
	struct ProcStatNums	 OldChk0;
	struct ProcStatNums	 DiffChk0;
	struct ProcStatNums	 NowChk1;
	struct ProcStatNums	 OldChk1;
	struct ProcStatNums	 DiffChk1;
#endif
	struct ProcStatNums	 *pThis;
	struct ProcMeminfoNums	 MemChk;

	int iTmp = 0;
	double iValue = 0;
	double iValue0 = 0;
	double iValue1 = 0;
	double iValue2 = 0;
	double iValue3 = 0;

	int checkloop = 0;
	int loopmode = 0;
	checkloop = CHECKLOOP;

	if( (2 == argc) && (0 == strcmp("loop", argv[1])) )
	{
		checkloop = 65534;
		loopmode = 1;
	}
	
	printf( "\n   cpu usage tool v2, powered by span.\n" );


#ifdef _USE_NO_GREP_	/* cpuutil.h */
	if( -1 == getProcStatSet( &StatSetsOld ) )
	{
		printf( "\ngetProcStat 1st failed !!\n" );
		goto _ERROR;		
	}
#else
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
#endif

	getFBinfo();

	while( iTmp++ < checkloop )
	{
		
		sleep( 1 );
#ifdef _USE_NO_GREP_	/* cpuutil.h */
		if( -1 == getProcStatSet( &StatSetsNow ) )
		{
			printf( "\ngetProcStat failed !!\n" );
			goto _ERROR;		
		}
		else
		{
			updateSetsNUM( &StatSetsOld, &StatSetsNow, &StatSetsDiff );
		}
#else
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
#endif
		getProcMeminfo( &MemChk );


#ifdef _USE_NO_GREP_	/* cpuutil.h */
		pThis = &(StatSetsDiff.cpu);
		iValue = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM)+(pThis->iowaitNUM)+(pThis->irqNUM)+(pThis->softirqNUM))*100 )/( pThis->sumNUM );
		pThis = &(StatSetsDiff.cpu0);
		iValue0 = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM)+(pThis->iowaitNUM)+(pThis->irqNUM)+(pThis->softirqNUM))*100 )/( pThis->sumNUM );
		pThis = &(StatSetsDiff.cpu1);
		iValue1 = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM)+(pThis->iowaitNUM)+(pThis->irqNUM)+(pThis->softirqNUM))*100 )/( pThis->sumNUM );
		pThis = &(StatSetsDiff.cpu2);
		iValue2 = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM)+(pThis->iowaitNUM)+(pThis->irqNUM)+(pThis->softirqNUM))*100 )/( pThis->sumNUM );
		pThis = &(StatSetsDiff.cpu3);
		iValue3 = (double)( ((pThis->userNUM)+(pThis->niceNUM)+(pThis->systemNUM)+(pThis->iowaitNUM)+(pThis->irqNUM)+(pThis->softirqNUM))*100 )/( pThis->sumNUM );
#else
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
		#endif	///#if 0
#endif	///#ifdef _USE_NO_GREP_

		if( 0 == loopmode )
		{
			///printf( "CPU usage: %3.2f%%.[0:%3.2f%%]\n", iValue, iValue0 );
			printf( "CPU usage:%3.2f%%[0:%3.2f%%/1:%3.2f%%/2:%3.2f%%/3:%3.2f%%] ", iValue, iValue0, iValue1, iValue2, iValue3 );
			printf( "Mem:[Totl:%ld/Used:%ld/Free:%ld]kB\n", MemChk.memtotalNUM, MemChk.memusedNUM, MemChk.memfreeNUM );
		}
	#if 1
		drawHbar( 0, 0, (100*5), 10, _BLACK_COLOR );
		drawHbar( 0, 2, (iValue*5), 8, _RED_COLOR );

		drawHbar( 0, 10, (100*5), 10, _BLACK_COLOR );
		drawHbar( 0, 12, (iValue0*5), 8, _PINK_COLOR );

		drawHbar( 0, 20, (100*5), 10, _BLACK_COLOR );
		drawHbar( 0, 22, (iValue1*5), 8, _PINK_COLOR );

		drawHbar( 0, 30, (100*5), 10, _BLACK_COLOR );
		drawHbar( 0, 32, (iValue2*5), 8, _PINK_COLOR );

		drawHbar( 0, 40, (100*5), 10, _BLACK_COLOR );
		drawHbar( 0, 42, (iValue3*5), 8, _PINK_COLOR );
	#else
		drawHbar( 0, 0, (100), 10, _BLACK_COLOR );
		drawHbar( 0, 2, (iValue), 8, _RED_COLOR );

		drawHbar( 100, 0, (100), 10, _BLACK_COLOR );
		drawHbar( 100, 2, (iValue0), 8, _PINK_COLOR );

		drawHbar( 200, 0, (100), 10, _BLACK_COLOR );
		drawHbar( 200, 2, (iValue1), 8, _PINK_COLOR );

		drawHbar( 300, 0, (100), 10, _BLACK_COLOR );
		drawHbar( 300, 2, (iValue2), 8, _PINK_COLOR );

		drawHbar( 400, 0, (100), 10, _BLACK_COLOR );
		drawHbar( 400, 2, (iValue3), 8, _PINK_COLOR );
	#endif
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


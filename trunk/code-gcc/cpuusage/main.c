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
	int iTmp = 0;
	///int iValue = 0;
	double iValue = 0;
	
	if( -1 == getProcStat( &OldChk ) )
	{
		printf( "\ngetProcStat failed !!\n" );
		goto _ERROR;		
	}

	while( iTmp++ < 100 )
	{
		sleep( 1 );

		if( -1 == getProcStat( &NowChk ) )
		{
			printf( "\ngetProcStat failed !!\n" );
			goto _ERROR;		
		}
		else
		{
			DiffChk.userNUM = (NowChk.userNUM-OldChk.userNUM);
			DiffChk.niceNUM = (NowChk.niceNUM-OldChk.niceNUM);
			DiffChk.systemNUM = (NowChk.systemNUM-OldChk.systemNUM);
			DiffChk.idleNUM = (NowChk.idleNUM-OldChk.idleNUM);
			DiffChk.iowaitNUM = (NowChk.iowaitNUM-OldChk.iowaitNUM);
			DiffChk.irqNUM = (NowChk.irqNUM-OldChk.irqNUM);
			DiffChk.softirqNUM = (NowChk.softirqNUM-OldChk.softirqNUM);
			DiffChk.sumNUM = DiffChk.userNUM+DiffChk.niceNUM+DiffChk.systemNUM+DiffChk.idleNUM+DiffChk.iowaitNUM+DiffChk.irqNUM+DiffChk.softirqNUM;
			memcpy( &OldChk, &NowChk, sizeof(struct ProcStatNums) );
		}

#if 0
		iValue = (double)( ((DiffChk.userNUM)+(DiffChk.niceNUM)+(DiffChk.systemNUM))*100 )/( DiffChk.sumNUM );
#else
		iValue = (double)( ((DiffChk.userNUM)+(DiffChk.niceNUM)+(DiffChk.systemNUM)+(DiffChk.iowaitNUM)+(DiffChk.irqNUM)+(DiffChk.softirqNUM))*100 )/( DiffChk.sumNUM );
#endif
		printf( "CPU usage: %3.2f%%.\n", iValue );
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


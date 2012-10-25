/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <sys/time.h>



/* 
Parameters in /proc/stat
*/
struct ProcStatNums
{
	int userNUM;
	int niceNUM;
	int systemNUM;
	int idleNUM;
	int iowaitNUM;
	int irqNUM;
	int softirqNUM;
};




static int setProcStatNum( struct ProcStatNums *pIn, int iIdx, int Num )
{
	int iRet = 0;

	if( NULL != pIn )
	{
		if( iIdx == 0 )
			pIn->userNUM = Num;
		else
		if( iIdx == 1 )
			pIn->niceNUM = Num;
		else
		if( iIdx == 2 )
			pIn->systemNUM = Num;
		else
		if( iIdx == 3 )
			pIn->idleNUM = Num;
		else
		if( iIdx == 4 )
			pIn->iowaitNUM = Num;
		else
		if( iIdx == 5 )
			pIn->irqNUM = Num;
		else
		if( iIdx == 6 )
			pIn->softirqNUM = Num;
		else
			;
	}
	return iRet;
}


static int getProcStat( struct ProcStatNums *pIn )
{
	int iRet = (-1);
	char statOut[64];
	char statNum[16];
	FILE *fp;
	char *pCh;
	int iTmp;
	int iCnt;

	memset( statOut, sizeof(statOut), 0 );

	///fp = popen( "cat /proc/stat | grep '^\\cpu '", "r" );
	fp = popen( "cat /proc/stat | grep '^\cpu '", "r" );
	if( fp == NULL )
	{
		printf("Failed on cat /proc/stat !!\n");
		goto _pEXIT;
	}
	
	while( fgets( statOut, sizeof(statOut)-1, fp ) != NULL )
	{
		;///printf("%s", statOut );
	}

	pclose( fp );

	///pCh = &(statOut[0]);	///set start point
	pCh = statOut;	///set start point

	///if( (*(pCh+0) != 'c') || (*(pCh+1) != 'p') || (*(pCh+2) != 'u') )
	///if( (*(pCh+2) != 'u') )
	if( (*(pCh+0) != 'c') && (*(pCh+1) != 'p') && (*(pCh+2) != 'u') )
	{
		printf("It's NOT a /proc/stat cat !![%c][%c][%c]\n", *(pCh+0), *(pCh+1), *(pCh+2) );
		goto _pEXIT;
	} 

	pCh = pCh+5;	///first number set
	
	iTmp = 0;
	iCnt = 0;
	while( iCnt < 7 )	///11 number set
	{
		while( *(pCh+iTmp) != ' ' )
		{
			statNum[iTmp] = *(pCh+iTmp);
			iTmp++;
		}
		statNum[iTmp] = '\0';
		///printf( "Num%d:%s[%d]\n", iCnt, statNum, atoi(statNum) );
		pCh = pCh + iTmp + 1;

		setProcStatNum( pIn, iCnt, atoi(statNum) );		

		memset( statNum, sizeof(statNum), 0 );
		iCnt++;
		iTmp = 0;
	}
	iRet = 0;

_pEXIT:
	return iRet;
}



int main()
{
	int return_value = 0;
	pthread_t thread_id;
	struct ProcStatNums	 NowChk;
	struct ProcStatNums	 OldChk;
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

		///getProcStat( &NowChk );
		if( -1 == getProcStat( &NowChk ) )
		{
			printf( "\ngetProcStat failed !!\n" );
			goto _ERROR;		
		}


#if 0

		iValue = (double)(( (NowChk.userNUM-OldChk.userNUM)+(NowChk.niceNUM-OldChk.niceNUM)+(NowChk.systemNUM-OldChk.systemNUM) )*100)/( (NowChk.userNUM-OldChk.userNUM)+(NowChk.userNUM-OldChk.userNUM)+(NowChk.userNUM-OldChk.userNUM)+(NowChk.idleNUM-OldChk.idleNUM)+(NowChk.iowaitNUM-OldChk.iowaitNUM)+(NowChk.irqNUM-OldChk.irqNUM)+(NowChk.softirqNUM-OldChk.softirqNUM) );

#else

		iValue = (double)(( (NowChk.userNUM-OldChk.userNUM)+(NowChk.niceNUM-OldChk.niceNUM)+(NowChk.systemNUM-OldChk.systemNUM)+(NowChk.iowaitNUM-OldChk.iowaitNUM)+(NowChk.irqNUM-OldChk.irqNUM)+(NowChk.softirqNUM-OldChk.softirqNUM) )*100)/( (NowChk.userNUM-OldChk.userNUM)+(NowChk.userNUM-OldChk.userNUM)+(NowChk.userNUM-OldChk.userNUM)+(NowChk.idleNUM-OldChk.idleNUM)+(NowChk.iowaitNUM-OldChk.iowaitNUM)+(NowChk.irqNUM-OldChk.irqNUM)+(NowChk.softirqNUM-OldChk.softirqNUM) );

#endif
		printf( "CPU usage: %f%%.\n", iValue );
		memcpy( &OldChk, &NowChk, sizeof(struct ProcStatNums) );
	}


	printf("The process ID is %d\n", (int)getpid() );
	printf("The parent process ID is %d\n", (int)getppid() );

	///pthread_create( &thread_id, NULL, &print_xs, NULL );

	printf( "\ndone \n" );
	return 0;

_ERROR:
	printf( "\nsomething wrong !!\n" );
	return return_value;
}


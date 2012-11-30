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


static struct cpuBarCfgS V2 = \
{
	{ 0,  2, 100,  8, 5, _RED_COLOR },
	{ 0,  0, 100, 10, 5, _BLACK_COLOR },
	{ 0, 12, 100,  8, 5, _PINK_COLOR },
	{ 0, 10, 100, 10, 5, _BLACK_COLOR },
	{ 0, 22, 100,  8, 5, _YELLOW_COLOR },
	{ 0, 20, 100, 10, 5, _BLACK_COLOR },
	{ 0, 32, 100,  8, 5, _CYAN_COLOR },
	{ 0, 30, 100, 10, 5, _BLACK_COLOR },
	{ 0, 42, 100,  8, 5, _GREEN_COLOR },
	{ 0, 40, 100, 10, 5, _BLACK_COLOR },
};

static struct cpuBarCfgS V3 = \
{
	{   0,  2, 100,  8, 1, _RED_COLOR },
	{   0,  0, 100, 10, 1, _BLACK_COLOR },
	{ 100,  2, 100,  8, 1, _PINK_COLOR },
	{ 100,  0, 100, 10, 1, _BLACK_COLOR },
	{ 200,  2, 100,  8, 1, _YELLOW_COLOR },
	{ 200,  0, 100, 10, 1, _BLACK_COLOR },
	{ 300,  2, 100,  8, 1, _CYAN_COLOR },
	{ 300,  0, 100, 10, 1, _BLACK_COLOR },
	{ 400,  2, 100,  8, 1, _GREEN_COLOR },
	{ 400,  0, 100, 10, 1, _BLACK_COLOR },
};

static struct cpuBarCfgS V4 = \
{
	{   0,  2, 100,  8, 4, _RED_COLOR },
	{   0,  0, 100, 10, 4, _BLACK_COLOR },
	{   0, 12, 100,  8, 1, _PINK_COLOR },
	{   0, 10, 100, 10, 1, _BLACK_COLOR },
	{ 100, 12, 100,  8, 1, _YELLOW_COLOR },
	{ 100, 10, 100, 10, 1, _BLACK_COLOR },
	{ 200, 12, 100,  8, 1, _CYAN_COLOR },
	{ 200, 10, 100, 10, 1, _BLACK_COLOR },
	{ 300, 12, 100,  8, 1, _GREEN_COLOR },
	{ 300, 10, 100, 10, 1, _BLACK_COLOR },
};


static struct cpuBarCfgS* pV = &V2; /* graphic mode 0 by default */
static char verStr[] = "v5.3";

static long g_loop = 5;
static long g_interval = 1000;
static long g_text = 1;
static long g_graphic = 1;
static long g_graphicmode = 0;


#define	CHECKLOOP			5
#define	LOOPFOREVER			0xffffffff

void logo( void )
{
	printf( "\n=============================================" );
	printf( "\n=   cpu usage tool %s, powered by span.   =", verStr );
	printf( "\n=   additional -h for more information.     =" );
	printf( "\n=============================================" );
	printf( "\n" );
}


void helps( void )
{
	printf( "\n==============================================================" );
	printf( "\n=  cpu usage tool %s. parameter info                       =", verStr );
	printf( "\n=  -h     : this help list.                                  =" );
	printf( "\n=  -l num : loops of checking. 0=forever, default=5          =" );
	printf( "\n=  -i num : interval of checking. 1000=1sec, default=1000.   =" );
	printf( "\n=  -t num : text msg. 0=off 1=on, default=1                  =" );
	printf( "\n=  -g num : graphic msg. 0=off 1=on, default=1               =" );
	printf( "\n=  -v num : graphic msg mode. 0=type0 ..., default=0         =" );
	printf( "\n=                                            powered by span =" );
	printf( "\n==============================================================" );
	printf( "\n" );
	printf( "\n" );
}


void configs( void )
{
	long tmploop = g_loop;
	
	if( tmploop == LOOPFOREVER )
		tmploop = 0;
	printf( "\n== run with -l %lu, -i %lu, -t %lu, -g %lu, -v %lu config. ==", tmploop, g_interval, g_text, g_graphic, g_graphicmode );
	printf( "\n" );
}


int main( int argc, char *argv[] )
{
	int return_value = 0;
/*
	pthread_t thread_id;
*/
	
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

	long lTmp = 0;
	double iValue = 0;
	double iValue0 = 0;
	double iValue1 = 0;
	double iValue2 = 0;
	double iValue3 = 0;

	if( (2 == argc) && (0 == strcmp("-h", argv[1])) )
	{
		helps();
		exit(0);
	}
	
	/* check user parameter */
	lTmp = 1;
	while( (argc>1) && (argc>lTmp) )
	{
		if( (0 == strcmp("-l", argv[lTmp])) )
		{
			g_loop = atoi( argv[lTmp+1] );
			if( 0 == g_loop )
				g_loop = LOOPFOREVER;
			lTmp++;
		}
		else
		if( (0 == strcmp("-i", argv[lTmp])) )
		{
			g_interval = atoi( argv[lTmp+1] );
			lTmp++;
		}
		else
		if( (0 == strcmp("-t", argv[lTmp])) )
		{
			g_text = atoi( argv[lTmp+1] );
			lTmp++;
		}
		else
		if( (0 == strcmp("-g", argv[lTmp])) )
		{
			g_graphic = atoi( argv[lTmp+1] );
			lTmp++;
		}
		else
		if( (0 == strcmp("-v", argv[lTmp])) )
		{
			g_graphicmode = atoi( argv[lTmp+1] );
			if( g_graphicmode == 0 )
				pV = &V2;
			else
			if( g_graphicmode == 1 )
				pV = &V3;
			else
			if( g_graphicmode == 2 )
				pV = &V4;
			lTmp++;
		}
		else
			printf( "\n  unknow parameter %s !!\n", argv[lTmp] );
	
		lTmp++;
	}
	
	logo();
	
	configs();

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

	lTmp = 0;
	while( (lTmp++ < g_loop) || (g_loop == LOOPFOREVER) )
	{
		usleep( (g_interval*1000) );

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

		if( g_text > 0 )
		{
			///printf( "CPU usage: %3.2f%%.[0:%3.2f%%]\n", iValue, iValue0 );
			printf( "CPU usage:%3.2f%%[0:%3.2f%%/1:%3.2f%%/2:%3.2f%%/3:%3.2f%%] ", iValue, iValue0, iValue1, iValue2, iValue3 );
			printf( "Mem:[Totl:%ld/Used:%ld/Free:%ld]kB\n", MemChk.memtotalNUM, MemChk.memusedNUM, MemChk.memfreeNUM );
		}

		if( g_graphic > 0 )
		{
			drawHbar( pV->_ab.X, pV->_ab.Y, (pV->_ab.L*pV->_ab.M), pV->_ab.H, pV->_ab.C );
			drawHbar( pV->_af.X, pV->_af.Y, (iValue*pV->_af.M), pV->_af.H, pV->_af.C );

			drawHbar( pV->_0b.X, pV->_0b.Y, (pV->_0b.L*pV->_0b.M), pV->_0b.H, pV->_0b.C );
			drawHbar( pV->_0f.X, pV->_0f.Y, (iValue0*pV->_0f.M), pV->_0f.H, pV->_0f.C );

			drawHbar( pV->_1b.X, pV->_1b.Y, (pV->_1b.L*pV->_1b.M), pV->_1b.H, pV->_1b.C );
			drawHbar( pV->_1f.X, pV->_1f.Y, (iValue1*pV->_1f.M), pV->_1f.H, pV->_1f.C );

			drawHbar( pV->_2b.X, pV->_2b.Y, (pV->_2b.L*pV->_2b.M), pV->_2b.H, pV->_2b.C );
			drawHbar( pV->_2f.X, pV->_2f.Y, (iValue2*pV->_2f.M), pV->_2f.H, pV->_2f.C );

			drawHbar( pV->_3b.X, pV->_3b.Y, (pV->_3b.L*pV->_3b.M), pV->_3b.H, pV->_3b.C );
			drawHbar( pV->_3f.X, pV->_3f.Y, (iValue3*pV->_3f.M), pV->_3f.H, pV->_3f.C );
		}
	}	///while

/*
	printf("The process ID is %d\n", (int)getpid() );
	printf("The parent process ID is %d\n", (int)getppid() );
*/
/*
	pthread_create( &thread_id, NULL, &print_xs, NULL );
*/
	printf( "\ndone \n" );
	return 0;

_ERROR:
	printf( "\nsomething wrong !!\n" );
	return return_value;
}


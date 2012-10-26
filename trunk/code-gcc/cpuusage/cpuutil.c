/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpuutil.h"




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


static FILE* openProcStat( int cpuIdx )
{
	FILE *fRet = NULL;

	/* open proc stat ... */
	if( (-1) == cpuIdx )
	{
		///fRet = popen( "cat /proc/stat | grep '^\\cpu '", "r" );
		fRet = popen( "cat /proc/stat | grep '^\cpu '", "r" );
	}
	else
	if( (0) == cpuIdx )
	{
		fRet = popen( "cat /proc/stat | grep '^\cpu0 '", "r" );
	}
	else
	if( (1) == cpuIdx )
	{
		fRet = popen( "cat /proc/stat | grep '^\cpu1 '", "r" );
	}
	else
	if( (2) == cpuIdx )
	{
		fRet = popen( "cat /proc/stat | grep '^\cpu2 '", "r" );
	}
	else
	if( (3) == cpuIdx )
	{
		fRet = popen( "cat /proc/stat | grep '^\cpu3 '", "r" );
	}
	else
	if( (4) == cpuIdx )
	{
		fRet = popen( "cat /proc/stat | grep '^\cpu4 '", "r" );
	}
	else
	if( (5) == cpuIdx )
	{
		fRet = popen( "cat /proc/stat | grep '^\cpu5 '", "r" );
	}
	else
	if( (6) == cpuIdx )
	{
		fRet = popen( "cat /proc/stat | grep '^\cpu6 '", "r" );
	}
	else
	if( (7) == cpuIdx )
	{
		fRet = popen( "cat /proc/stat | grep '^\cpu7 '", "r" );
	}
	else
	if( (8) == cpuIdx )
	{
		fRet = popen( "cat /proc/stat | grep '^\cpu8 '", "r" );
	}


	if( fRet == NULL )
	{
		printf("Failed on cat /proc/stat !!\n");
		goto _tEXIT;
	}


_tEXIT:
	return fRet;
}


int getProcStat( struct ProcStatNums *pIn, int cpuIdx )
{
	int iRet = (-1);
	char statOut[64];
	char statNum[16];
	FILE *fp;
	char *pCh;
	int iTmp;
	int iCnt;

	memset( statOut, sizeof(statOut), 0 );

	/* get proc stat ... */
	///fp = popen( "cat /proc/stat | grep '^\\cpu '", "r" );
	///fp = popen( "cat /proc/stat | grep '^\cpu '", "r" );
	fp = openProcStat( cpuIdx );
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

	/* check proc stat output ... */
	///pCh = &(statOut[0]);	///set start point
	pCh = statOut;	///set start point

	///if( (*(pCh+0) != 'c') || (*(pCh+1) != 'p') || (*(pCh+2) != 'u') )
	///if( (*(pCh+2) != 'u') )
	if( (*(pCh+0) != 'c') && (*(pCh+1) != 'p') && (*(pCh+2) != 'u') )
	{
		printf("It's NOT a /proc/stat cat !![%c][%c][%c]\n", *(pCh+0), *(pCh+1), *(pCh+2) );
		goto _pEXIT;
	} 

	/* parce proc stat number ... */
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

		///setProcStatNum( pIn, iCnt, atoi(statNum) );
		setProcStatNum( pIn, iCnt, atol(statNum) );		

		memset( statNum, sizeof(statNum), 0 );
		iCnt++;
		iTmp = 0;
	}
	iRet = 0;

_pEXIT:
	return iRet;
}


int updateNUM( struct ProcStatNums *pOld, struct ProcStatNums *pNew, struct ProcStatNums *pDiff )
{
	int iRet = 0;

	if( (pOld) && (pNew) && (pDiff) )
	{
		pDiff->userNUM = (pNew->userNUM-pOld->userNUM);
		pDiff->niceNUM = (pNew->niceNUM-pOld->niceNUM);
		pDiff->systemNUM = (pNew->systemNUM-pOld->systemNUM);
		pDiff->idleNUM = (pNew->idleNUM-pOld->idleNUM);
		pDiff->iowaitNUM = (pNew->iowaitNUM-pOld->iowaitNUM);
		pDiff->irqNUM = (pNew->irqNUM-pOld->irqNUM);
		pDiff->softirqNUM = (pNew->softirqNUM-pOld->softirqNUM);
		pDiff->sumNUM = pDiff->userNUM+pDiff->niceNUM+pDiff->systemNUM+pDiff->idleNUM+pDiff->iowaitNUM+pDiff->irqNUM+pDiff->softirqNUM;
		memcpy( pOld, pNew, sizeof(struct ProcStatNums) );		
	}
	else
		iRet = -1;

	return iRet;
}




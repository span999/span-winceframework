/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpuutil.h"


#if 0
	/* Look through proc status contents line by line */
	char delims[] = "\n";
	char* line = strtok(buffer, delims);

	while (line != NULL && (found_vmrss == 0 || found_vmsize == 0) )
	{
		search_result = strstr(line, "VmRSS:");
		if (search_result != NULL)
		{
			sscanf(line, "%*s %ld", vmrss_kb);
			found_vmrss = 1;
		}

		search_result = strstr(line, "VmSize:");
		if (search_result != NULL)
		{
			sscanf(line, "%*s %ld", vmsize_kb);
			found_vmsize = 1;
		}

		line = strtok(NULL, delims);
	}

	return (found_vmrss == 1 && found_vmsize == 1) ? 0 : 1;
#endif



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


int getProcMeminfo( struct ProcMeminfoNums *pIn )
{
	int iRet = (-1);
	char statOut[1024];
	char statNum[16];
	FILE *fp;
	char *pCh;
	int iTmp;
	int iCnt;
	char delims[] = "\n";
	char* line;
	char* search_result;


	memset( statOut, sizeof(statOut), 0 );

	/* get proc meminfo ... */
	fp = popen( "cat /proc/meminfo", "r" );

	if( fp == NULL )
	{
		printf("Failed on cat /proc/meminfo !!\n");
		goto _pEXIT;
	}

	pCh = statOut;	///set start point
	///while( fgets( statOut, sizeof(statOut)-1, fp ) != NULL )
	while( fgets( pCh, sizeof(statOut)-1, fp ) != NULL )
	{
#if 0
		printf("%s", pCh );	
#endif
		pCh = pCh + strlen(pCh) + 1;
	}

#if 0	
	pCh = statOut;	///set start point
	iTmp = 0;
	while( iTmp < 1024 )
	{
		printf("%c", *(pCh+iTmp) );
		iTmp++;
	}
#endif
	pclose( fp );
	///sleep(1);

	/* check proc meminfo output ... */
	////pCh = &(statOut[0]);	///set start point
	pCh = statOut;	///set start point

	///if( (*(pCh+0) != 'D') || (*(pCh+1) != 'i') || (*(pCh+2) != 'r') )
	if( (*(pCh+0) != 'M') && (*(pCh+1) != 'e') && (*(pCh+2) != 'm') )
	{
		printf("It's NOT a /proc/meminfo cat !![%c][%c][%c][%c]\n", *(pCh+0), *(pCh+1), *(pCh+2), *(pCh+3) );
		printf("It's NOT a /proc/meminfo cat !![%s]\n", pCh );
		goto _pEXIT;
	} 

#if 0
	/* parce proc meminfo number ... */
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
#else
	iCnt = 10;
	line = strtok( statOut, delims );
	///line = strtok( (pCh+29), delims );

	while( (line != NULL) && iCnt > 0 )
	{
		printf("Mem Cnt:%d <%s>\n", iCnt, line );

		pCh = strstr(line, "MemTotal:");

		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld", &(pIn->memtotalNUM) );
			printf("MemTotal:%ld\n", pIn->memtotalNUM );
		}

		pCh = strstr(line, "MemFree:");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld", &(pIn->memfreeNUM) );
			printf("MemFree:%ld\n", pIn->memfreeNUM );
		}

		line = strtok( NULL, delims );
		iCnt--;
	}

#endif
	iRet = 0;

_pEXIT:
	return iRet;
}


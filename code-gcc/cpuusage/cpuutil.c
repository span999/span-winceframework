/*
 * 
 * 


example:
int main()
{
	struct ProcStatSets  StatSetsOld;
	struct ProcStatSets  StatSetsNow;
	struct ProcStatSets  StatSetsDiff;

	struct ProcStatNums	 *pThis;
	struct ProcMeminfoNums	 MemChk;

	int iTmp = 0;
	double iValue = 0;
	double iValue0 = 0;
	double iValue1 = 0;
	double iValue2 = 0;
	double iValue3 = 0;

	if( -1 == getProcStatSet( &StatSetsOld ) )
	{
		printf( "\ngetProcStat 1st failed !!\n" );
		goto _ERROR;		
	}

	while( iTmp++ < CHECKLOOP )
	{
		sleep( 1 );
		if( -1 == getProcStatSet( &StatSetsNow ) )
		{
			printf( "\ngetProcStat failed !!\n" );
			goto _ERROR;		
		}
		else
		{
			updateSetsNUM( &StatSetsOld, &StatSetsNow, &StatSetsDiff );
		}
		getProcMeminfo( &MemChk );


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

		printf( "CPU usage:%3.2f%%[0:%3.2f%%/1:%3.2f%%/2:%3.2f%%/3:%3.2f%%] ", iValue, iValue0, iValue1, iValue2, iValue3 );
		printf( "Mem:[Totl:%ld/Used:%ld/Free:%ld]kB\n", MemChk.memtotalNUM, MemChk.memusedNUM, MemChk.memfreeNUM );

	}	///while

	printf( "\ndone !!\n" );
	return 0;

_ERROR:
	printf( "\nsomething wrong !!\n" );
	return 1;
}


 *
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if 0
#include <sys/timeb.h>	/* for timeb in busystress */
#endif
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


#ifndef _USE_NO_GREP_
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

	memset( statOut, 0, sizeof(statOut) );

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

	/* parse proc stat number ... */
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

		memset( statNum, 0, sizeof(statNum) );
		iCnt++;
		iTmp = 0;
	}
	iRet = 0;

_pEXIT:
	return iRet;
}
#endif	/* #ifndef _USE_NO_GREP_ */


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


int updateSetsNUM( struct ProcStatSets *pOld, struct ProcStatSets *pNow, struct ProcStatSets *pDiff )
{
	int iRet = 0;
	struct ProcStatNums *pcpuOld;
	struct ProcStatNums *pcpuNow;
	struct ProcStatNums *pcpuDiff;

	pcpuOld = &(pOld->cpu);
	pcpuNow = &(pNow->cpu);
	pcpuDiff = &(pDiff->cpu);
	iRet = updateNUM( pcpuOld, pcpuNow, pcpuDiff );

	pcpuOld = &(pOld->cpu0);
	pcpuNow = &(pNow->cpu0);
	pcpuDiff = &(pDiff->cpu0);
	iRet = updateNUM( pcpuOld, pcpuNow, pcpuDiff );

	pcpuOld = &(pOld->cpu1);
	pcpuNow = &(pNow->cpu1);
	pcpuDiff = &(pDiff->cpu1);
	iRet = updateNUM( pcpuOld, pcpuNow, pcpuDiff );

	pcpuOld = &(pOld->cpu2);
	pcpuNow = &(pNow->cpu2);
	pcpuDiff = &(pDiff->cpu2);
	iRet = updateNUM( pcpuOld, pcpuNow, pcpuDiff );

	pcpuOld = &(pOld->cpu3);
	pcpuNow = &(pNow->cpu3);
	pcpuDiff = &(pDiff->cpu3);
	iRet = updateNUM( pcpuOld, pcpuNow, pcpuDiff );

	pcpuOld = &(pOld->cpu4);
	pcpuNow = &(pNow->cpu4);
	pcpuDiff = &(pDiff->cpu4);
	iRet = updateNUM( pcpuOld, pcpuNow, pcpuDiff );

	pcpuOld = &(pOld->cpu5);
	pcpuNow = &(pNow->cpu5);
	pcpuDiff = &(pDiff->cpu5);
	iRet = updateNUM( pcpuOld, pcpuNow, pcpuDiff );

	pcpuOld = &(pOld->cpu6);
	pcpuNow = &(pNow->cpu6);
	pcpuDiff = &(pDiff->cpu6);
	iRet = updateNUM( pcpuOld, pcpuNow, pcpuDiff );

	pcpuOld = &(pOld->cpu7);
	pcpuNow = &(pNow->cpu7);
	pcpuDiff = &(pDiff->cpu7);
	iRet = updateNUM( pcpuOld, pcpuNow, pcpuDiff );

	return iRet;
}


///#define	_STATDEBUG_

int getProcStatSet( struct ProcStatSets *pIn )
{	
	int iRet = (-1);
	char statOut[4096];
	char statNum[64];
	FILE *fp;
	char *pCh;
	int iTmp;
	int iCnt;
	char delims[] = "\n";
	char* line;
	char* search_result;
	struct ProcStatNums *pcpu = NULL;

	memset( statOut, 0, sizeof(statOut) );

	/* get proc stat ... */
	fp = popen( "cat /proc/stat", "r" );
	if( fp == NULL )
	{
		printf("Failed on cat /proc/stat !!\n");
		goto _pEXIT;
	}
	
	pCh = statOut;	///set start point
	///grep /proc/stat out line by line
	///while( fgets( statOut, sizeof(statOut)-1, fp ) != NULL )
	while( fgets( pCh, sizeof(statOut)-1, fp ) != NULL )
	{
#ifdef _STATDEBUG_
		///printf("0x%08x:%s", (unsigned int)pCh, statOut );
		printf("0x%08x:%s", (unsigned int)pCh, pCh );	
#endif
		pCh = pCh + strlen(pCh);
	}

#ifdef _STATDEBUG_	
	pCh = statOut;	///set start point
	iTmp = 0;
	printf("-----------dump from 0x%08x-------------\n", (unsigned int)pCh);
	while( (iTmp < 4096) && (0 != *(pCh+iTmp)) )
	{
		printf("%c", *(pCh+iTmp) );
		iTmp++;
	}
#endif

	pclose( fp );

	/* check proc stat output ... */
	////pCh = &(statOut[0]);	///set start point
	pCh = statOut;	///set start point

	if( (*(pCh+0) != 'c') || (*(pCh+1) != 'p') || (*(pCh+2) != 'u') )
	{
		printf("It's NOT a /proc/stat cat !![%c][%c][%c][%c]\n", *(pCh+0), *(pCh+1), *(pCh+2), *(pCh+3) );
		printf("It's NOT a /proc/stat cat !![%s]\n", pCh );
		goto _pEXIT;
	} 

	/* parse proc stat number ... */
	iCnt = 50;
	line = strtok( statOut, delims );
	///line = strtok( (pCh+29), delims );

	while( (line != NULL) && (iCnt > 0) )
	{
#ifdef _STATDEBUG_
		printf("Cpu Cnt:%d <%s>\n", iCnt, line );
#endif

		pcpu = &(pIn->cpu);
		pCh = strstr(line, "cpu ");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld %ld %ld %ld %ld %ld %ld", &(pcpu->userNUM), &(pcpu->niceNUM), &(pcpu->systemNUM), &(pcpu->idleNUM), &(pcpu->iowaitNUM), &(pcpu->irqNUM), &(pcpu->softirqNUM) );
			pcpu->sumNUM = pcpu->userNUM+pcpu->niceNUM+pcpu->systemNUM+pcpu->idleNUM+pcpu->iowaitNUM+pcpu->irqNUM+pcpu->softirqNUM;
			pIn->cpuNUM = 1;
#ifdef _STATDEBUG_
			printf("cpu :%ld %ld %ld %ld %ld %ld %ld %ld\n", pcpu->userNUM, pcpu->niceNUM, pcpu->systemNUM, pcpu->idleNUM, pcpu->iowaitNUM, pcpu->irqNUM, pcpu->softirqNUM, pIn->cpu.sumNUM );
#endif
		}

		pcpu = &(pIn->cpu0);
		pCh = strstr(line, "cpu0");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld %ld %ld %ld %ld %ld %ld", &(pcpu->userNUM), &(pcpu->niceNUM), &(pcpu->systemNUM), &(pcpu->idleNUM), &(pcpu->iowaitNUM), &(pcpu->irqNUM), &(pcpu->softirqNUM) );
			pcpu->sumNUM = pcpu->userNUM+pcpu->niceNUM+pcpu->systemNUM+pcpu->idleNUM+pcpu->iowaitNUM+pcpu->irqNUM+pcpu->softirqNUM;
			pIn->cpuNUM = 1;
#ifdef _STATDEBUG_
			printf("cpu0:%ld %ld %ld %ld %ld %ld %ld %ld\n", pcpu->userNUM, pcpu->niceNUM, pcpu->systemNUM, pcpu->idleNUM, pcpu->iowaitNUM, pcpu->irqNUM, pcpu->softirqNUM, pIn->cpu.sumNUM );
#endif
		}

		pcpu = &(pIn->cpu1);
		pCh = strstr(line, "cpu1");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld %ld %ld %ld %ld %ld %ld", &(pcpu->userNUM), &(pcpu->niceNUM), &(pcpu->systemNUM), &(pcpu->idleNUM), &(pcpu->iowaitNUM), &(pcpu->irqNUM), &(pcpu->softirqNUM) );
			pcpu->sumNUM = pcpu->userNUM+pcpu->niceNUM+pcpu->systemNUM+pcpu->idleNUM+pcpu->iowaitNUM+pcpu->irqNUM+pcpu->softirqNUM;
			pIn->cpuNUM = 2;
#ifdef _STATDEBUG_
			printf("cpu1:%ld %ld %ld %ld %ld %ld %ld %ld\n", pcpu->userNUM, pcpu->niceNUM, pcpu->systemNUM, pcpu->idleNUM, pcpu->iowaitNUM, pcpu->irqNUM, pcpu->softirqNUM, pIn->cpu.sumNUM );
#endif
		}

		pcpu = &(pIn->cpu2);
		pCh = strstr(line, "cpu2");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld %ld %ld %ld %ld %ld %ld", &(pcpu->userNUM), &(pcpu->niceNUM), &(pcpu->systemNUM), &(pcpu->idleNUM), &(pcpu->iowaitNUM), &(pcpu->irqNUM), &(pcpu->softirqNUM) );
			pcpu->sumNUM = pcpu->userNUM+pcpu->niceNUM+pcpu->systemNUM+pcpu->idleNUM+pcpu->iowaitNUM+pcpu->irqNUM+pcpu->softirqNUM;
			pIn->cpuNUM = 3;
#ifdef _STATDEBUG_
			printf("cpu2:%ld %ld %ld %ld %ld %ld %ld %ld\n", pcpu->userNUM, pcpu->niceNUM, pcpu->systemNUM, pcpu->idleNUM, pcpu->iowaitNUM, pcpu->irqNUM, pcpu->softirqNUM, pIn->cpu.sumNUM );
#endif
		}

		pcpu = &(pIn->cpu3);
		pCh = strstr(line, "cpu3");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld %ld %ld %ld %ld %ld %ld", &(pcpu->userNUM), &(pcpu->niceNUM), &(pcpu->systemNUM), &(pcpu->idleNUM), &(pcpu->iowaitNUM), &(pcpu->irqNUM), &(pcpu->softirqNUM) );
			pcpu->sumNUM = pcpu->userNUM+pcpu->niceNUM+pcpu->systemNUM+pcpu->idleNUM+pcpu->iowaitNUM+pcpu->irqNUM+pcpu->softirqNUM;
			pIn->cpuNUM = 4;
#ifdef _STATDEBUG_
			printf("cpu3:%ld %ld %ld %ld %ld %ld %ld %ld\n", pcpu->userNUM, pcpu->niceNUM, pcpu->systemNUM, pcpu->idleNUM, pcpu->iowaitNUM, pcpu->irqNUM, pcpu->softirqNUM, pIn->cpu.sumNUM );
#endif
		}

		pcpu = &(pIn->cpu4);
		pCh = strstr(line, "cpu4");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld %ld %ld %ld %ld %ld %ld", &(pcpu->userNUM), &(pcpu->niceNUM), &(pcpu->systemNUM), &(pcpu->idleNUM), &(pcpu->iowaitNUM), &(pcpu->irqNUM), &(pcpu->softirqNUM) );
			pcpu->sumNUM = pcpu->userNUM+pcpu->niceNUM+pcpu->systemNUM+pcpu->idleNUM+pcpu->iowaitNUM+pcpu->irqNUM+pcpu->softirqNUM;
			pIn->cpuNUM = 5;
#ifdef _STATDEBUG_
			printf("cpu4:%ld %ld %ld %ld %ld %ld %ld %ld\n", pcpu->userNUM, pcpu->niceNUM, pcpu->systemNUM, pcpu->idleNUM, pcpu->iowaitNUM, pcpu->irqNUM, pcpu->softirqNUM, pIn->cpu.sumNUM );
#endif
		}

		pcpu = &(pIn->cpu5);
		pCh = strstr(line, "cpu5");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld %ld %ld %ld %ld %ld %ld", &(pcpu->userNUM), &(pcpu->niceNUM), &(pcpu->systemNUM), &(pcpu->idleNUM), &(pcpu->iowaitNUM), &(pcpu->irqNUM), &(pcpu->softirqNUM) );
			pcpu->sumNUM = pcpu->userNUM+pcpu->niceNUM+pcpu->systemNUM+pcpu->idleNUM+pcpu->iowaitNUM+pcpu->irqNUM+pcpu->softirqNUM;
			pIn->cpuNUM = 6;
#ifdef _STATDEBUG_
			printf("cpu5:%ld %ld %ld %ld %ld %ld %ld %ld\n", pcpu->userNUM, pcpu->niceNUM, pcpu->systemNUM, pcpu->idleNUM, pcpu->iowaitNUM, pcpu->irqNUM, pcpu->softirqNUM, pIn->cpu.sumNUM );
#endif
		}

		pcpu = &(pIn->cpu6);
		pCh = strstr(line, "cpu6");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld %ld %ld %ld %ld %ld %ld", &(pcpu->userNUM), &(pcpu->niceNUM), &(pcpu->systemNUM), &(pcpu->idleNUM), &(pcpu->iowaitNUM), &(pcpu->irqNUM), &(pcpu->softirqNUM) );
			pcpu->sumNUM = pcpu->userNUM+pcpu->niceNUM+pcpu->systemNUM+pcpu->idleNUM+pcpu->iowaitNUM+pcpu->irqNUM+pcpu->softirqNUM;
			pIn->cpuNUM = 7;
#ifdef _STATDEBUG_
			printf("cpu6:%ld %ld %ld %ld %ld %ld %ld %ld\n", pcpu->userNUM, pcpu->niceNUM, pcpu->systemNUM, pcpu->idleNUM, pcpu->iowaitNUM, pcpu->irqNUM, pcpu->softirqNUM, pIn->cpu.sumNUM );
#endif
		}

		pcpu = &(pIn->cpu7);
		pCh = strstr(line, "cpu7");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld %ld %ld %ld %ld %ld %ld", &(pcpu->userNUM), &(pcpu->niceNUM), &(pcpu->systemNUM), &(pcpu->idleNUM), &(pcpu->iowaitNUM), &(pcpu->irqNUM), &(pcpu->softirqNUM) );
			pcpu->sumNUM = pcpu->userNUM+pcpu->niceNUM+pcpu->systemNUM+pcpu->idleNUM+pcpu->iowaitNUM+pcpu->irqNUM+pcpu->softirqNUM;
			pIn->cpuNUM = 8;
#ifdef _STATDEBUG_
			printf("cpu7:%ld %ld %ld %ld %ld %ld %ld %ld\n", pcpu->userNUM, pcpu->niceNUM, pcpu->systemNUM, pcpu->idleNUM, pcpu->iowaitNUM, pcpu->irqNUM, pcpu->softirqNUM, pIn->cpu.sumNUM );
#endif
		}



		line = strtok( NULL, delims );
		iCnt--;
	}
	iRet = 0;


_pEXIT:
	return iRet;
}


///#define	_MEMDEBUG_

int getProcMeminfo( struct ProcMeminfoNums *pIn )
{
	int iRet = (-1);
	char statOut[2048];
	char statNum[16];
	FILE *fp;
	char *pCh;
	int iTmp;
	int iCnt;
	char delims[] = "\n";
	char* line;
	char* search_result;


	memset( statOut, 0, sizeof(statOut) );

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
#ifdef _MEMDEBUG_
		printf("%s", pCh );	
#endif
		///pCh = pCh + strlen(pCh) + 1;
		pCh = pCh + strlen(pCh);
	}

#ifdef _MEMDEBUG_	
	pCh = statOut;	///set start point
	iTmp = 0;
	printf("------------------------------------\n");
	while( (iTmp < 2048) && (0 != *(pCh+iTmp)) )
	{
		printf("%c", *(pCh+iTmp) );
		iTmp++;
	}
#endif
	pclose( fp );

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

	/* parse proc meminfo number ... */
	iCnt = 50;
	line = strtok( statOut, delims );
	///line = strtok( (pCh+29), delims );

	while( (line != NULL) && (iCnt > 0) )
	{
#ifdef _MEMDEBUG_
		printf("Mem Cnt:%d <%s>\n", iCnt, line );
#endif
		pCh = strstr(line, "MemTotal:");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld", &(pIn->memtotalNUM) );
#ifdef _MEMDEBUG_
			printf("MemTotal:%ld\n", pIn->memtotalNUM );
#endif
		}

		pCh = strstr(line, "MemFree:");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld", &(pIn->memfreeNUM) );
#ifdef _MEMDEBUG_
			printf("MemFree:%ld\n", pIn->memfreeNUM );
#endif
		}

		pIn->memusedNUM = pIn->memtotalNUM - pIn->memfreeNUM;

		pCh = strstr(line, "Buffers:");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld", &(pIn->buffersNUM) );
#ifdef _MEMDEBUG_
			printf("Buffers:%ld\n", pIn->buffersNUM );
#endif
		}

		pCh = strstr(line, "Cached:");
		if( pCh != NULL )
		{
			if( 0 == pIn->cachedNUM )
			{
				sscanf(line, "%*s %ld", &(pIn->cachedNUM) );
#ifdef _MEMDEBUG_
				printf("Cached:%ld\n", pIn->cachedNUM );
#endif
			}
		}

		pCh = strstr(line, "VmallocTotal:");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld", &(pIn->vmalloctotalNUM) );
#ifdef _MEMDEBUG_
			printf("VmallocTotal:%ld\n", pIn->vmalloctotalNUM );
#endif
		}

		pCh = strstr(line, "VmallocUsed:");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld", &(pIn->vmallocusedNUM) );
#ifdef _MEMDEBUG_
			printf("VmallocUsed:%ld\n", pIn->vmallocusedNUM );
#endif
		}

		pCh = strstr(line, "VmallocChunk:");
		if( pCh != NULL )
		{
			sscanf(line, "%*s %ld", &(pIn->vmallocchunkNUM) );
#ifdef _MEMDEBUG_
			printf("VmallocChunk:%ld\n", pIn->vmallocchunkNUM );
#endif
		}

		line = strtok( NULL, delims );
		iCnt--;
	}

	iRet = 0;
_pEXIT:
	return iRet;
}

static void loopit( long loops )
{
	volatile long cnt1, cnt2, cnt3;
	
	cnt1 = cnt2 = cnt3 = 0;
	
	while( cnt1++ < loops )
	{
		while( cnt2++ < loops )
		{
			while( cnt3++ < loops )
			{
				volatile long long pretendWeNeedTheResult = 0;
				pretendWeNeedTheResult = cnt1^cnt2^cnt3%cnt1%cnt2%cnt3;
			}
			cnt3 = 0;
		}
		cnt2 = 0;
	}
	cnt1 = 0;
}

int stressbusy( long second )
{
	int iRet = -1;
	
#if 0
	struct timeb tp;
	ftime(&tp);
#endif
	time_t tls, tle;
	
	tle = tls = time(NULL);
	tle = tls + second;
	
	while( tle > time(NULL) )
	{
	#if 0	
		printf("%s:%s: time=%ld\n", __FILE__, __FUNCTION__, time(NULL) );
	#endif
		loopit( 2 );
	}
	
	return iRet;
}

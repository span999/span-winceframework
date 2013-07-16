/*
 * 
 * 


example:
int main()
{
}


 *
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "battutil.h"



#define	CMD1ADC "cat /sys/class/power_supply/vehicle_battery/current_now"
#define	CMD1VOLT "cat /sys/class/power_supply/vehicle_battery/voltage_now"
#define	CMD1PERC "cat /sys/class/power_supply/vehicle_battery/capacity"
#define	CMD1ONLN "cat /sys/class/power_supply/vehicle_battery/online"
#define	CMD2ADC "cat /sys/class/power_supply/internal_battery/current_now"
#define	CMD2VOLT "cat /sys/class/power_supply/internal_battery/voltage_now"
#define	CMD2PERC "cat /sys/class/power_supply/internal_battery/capacity"
#define	CMD2STAT "cat /sys/class/power_supply/internal_battery/status"



static char statOut[4096];
static char statNum[64];



///#define	_STATDEBUG_


static int getBatteryitem( int iItem )
{
	int iRet = (-1);
	FILE *fp;
	char *pCh;
#ifdef _STATDEBUG_
	int iTmp;
#endif

	switch( iItem )
	{
		case 1:
			fp = popen( CMD1ADC, "r" );
			break;
		case 2:
			fp = popen( CMD1VOLT, "r" );			
			break;
		case 3:
			fp = popen( CMD1PERC, "r" );			
			break;
		case 4:
			fp = popen( CMD1ONLN, "r" );			
			break;
		case 11:
			fp = popen( CMD2ADC, "r" );
			break;
		case 12:
			fp = popen( CMD2VOLT, "r" );			
			break;
		case 13:
			fp = popen( CMD2PERC, "r" );			
			break;
		case 14:
			fp = popen( CMD2STAT, "r" );			
			break;
		default:
			break;
	}
	
	if( fp == NULL )
	{
		printf("Failed on %s !!\n", CMD1ADC);
		goto _pEXIT;
	}
	
	pCh = statOut;	///set start point
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
	
_pEXIT:
	return iRet;
}


int getBatteryinfo( struct BattStatSets *pIn )
{	
	int iRet = (-1);
	char *pCh;
	struct BattStatNums *pBatt = NULL;

	memset( statOut, 0, sizeof(statOut) );

	pBatt = &(pIn->vehicle);

	/* get battery stat ... ADC */
	getBatteryitem( 1 );
	pCh = statOut;	///set start point
#if 0 /* do it later */
#endif
	/* parse battery stat number ... */
	pBatt->adc = atol(pCh);
#ifdef _STATDEBUG_	
	printf("pBatt->adc[%d]\n", pBatt->adc );
#endif

	/* get battery stat ... voltage */
	getBatteryitem( 2 );
	pCh = statOut;	///set start point
#if 0 /* do it later */
#endif
	/* parse battery stat number ... */
	pBatt->voltage = atol(pCh);
#ifdef _STATDEBUG_
	printf("pBatt->voltage[%d]\n", pBatt->voltage );
#endif

	/* get battery stat ... percentage */
	getBatteryitem( 3 );
	pCh = statOut;	///set start point
#if 0 /* do it later */
#endif
	/* parse battery stat number ... */
	pBatt->percentage = atol(pCh);
#ifdef _STATDEBUG_
	printf("pBatt->percentage[%d]\n", pBatt->percentage );
#endif

	/* get battery stat ... online */
	getBatteryitem( 4 );
	pCh = statOut;	///set start point
#if 0 /* do it later */
#endif
	/* parse battery stat number ... */
	pBatt->online = atol(pCh);
#ifdef _STATDEBUG_
	printf("pBatt->online[%d]\n", pBatt->online );
#endif


	pBatt = &(pIn->internal);

	/* get battery stat ... ADC */
	getBatteryitem( 11 );
	pCh = statOut;	///set start point
#if 0 /* do it later */
#endif
	/* parse battery stat number ... */
	pBatt->adc = atol(pCh);
#ifdef _STATDEBUG_
	printf("pBatt->adc[%d]\n", pBatt->adc );
#endif

	/* get battery stat ... voltage */
	getBatteryitem( 12 );
	pCh = statOut;	///set start point
#if 0 /* do it later */
#endif
	/* parse battery stat number ... */
	pBatt->voltage = atol(pCh);
#ifdef _STATDEBUG_
	printf("pBatt->voltage[%d]\n", pBatt->voltage );
#endif

	/* get battery stat ... percentage */
	getBatteryitem( 13 );
	pCh = statOut;	///set start point
#if 0 /* do it later */
#endif
	/* parse battery stat number ... */
	pBatt->percentage = atol(pCh);
#ifdef _STATDEBUG_
	printf("pBatt->percentage[%d]\n", pBatt->percentage );
#endif

	/* get battery stat ... status */
	getBatteryitem( 14 );
	pCh = statOut;	///set start point
#if 0 /* do it later */
#endif
	/* parse battery stat number ... */
	strcpy( pBatt->status, pCh );
#ifdef _STATDEBUG_
	printf("pBatt->status[%s]\n", pBatt->status );
#endif


_pEXIT:
	return iRet;
}


#if 0 /* do it later */
	if( (*(pCh+0) != 'c') || (*(pCh+1) != 'p') || (*(pCh+2) != 'u') )
	{
		printf("It's NOT a /proc/stat cat !![%c][%c][%c][%c]\n", *(pCh+0), *(pCh+1), *(pCh+2), *(pCh+3) );
		printf("It's NOT a /proc/stat cat !![%s]\n", pCh );
		goto _pEXIT;
	} 
#endif







#define	CPUTEMP "cat /sys/devices/virtual/thermal/thermal_zone0/temp"



static int getThermalitem( int iItem )
{
	int iRet = (-1);
	FILE *fp;
	char *pCh;
#ifdef _STATDEBUG_
	int iTmp;
#endif

	switch( iItem )
	{
		case 1:
			fp = popen( CPUTEMP, "r" );
			break;
		default:
			break;
	}
	
	if( fp == NULL )
	{
		printf("Failed on %s !!\n", CPUTEMP);
		goto _pEXIT;
	}
	
	pCh = statOut;	///set start point
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
	
_pEXIT:
	return iRet;
}


int getThermalinfo( struct ThermalStatSets *pIn )
{	
	int iRet = (-1);
	char *pCh;
	struct ThermalStatNums *pThermal = NULL;

	memset( statOut, 0, sizeof(statOut) );

	pThermal = &(pIn->internal);

	/* get thermal temp ... */
	getThermalitem( 1 );
	pCh = statOut;	///set start point

	/* parse thermal temp number ... */
	pThermal->temp = atol(pCh);
#ifdef _STATDEBUG_	
	printf("pThermal->temp[%d]\n", pThermal->temp );
#endif

	if( (pThermal->temp_max > 150) || (pThermal->temp_min < 15) )
	{
		pThermal->temp_max = 25;
		pThermal->temp_min = 101;
	}

	if( pThermal->temp > pThermal->temp_max )
		pThermal->temp_max = pThermal->temp;

	if( pThermal->temp < pThermal->temp_min )
		pThermal->temp_min = pThermal->temp;

_pEXIT:
	return iRet;
}




#define	CPUFREQ0 "cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"



static int getCpuFreqitem( int iItem )
{
	int iRet = (-1);
	FILE *fp;
	char *pCh;
#ifdef _STATDEBUG_
	int iTmp;
#endif

	switch( iItem )
	{
		case 1:
			fp = popen( CPUFREQ0, "r" );
			break;
		default:
			break;
	}
	
	if( fp == NULL )
	{
		printf("Failed on %s !!\n", CPUTEMP);
		goto _pEXIT;
	}
	
	pCh = statOut;	///set start point
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
	
_pEXIT:
	return iRet;
}


int getCpuFreqinfo( struct CpuFreqStatSets *pIn )
{	
	int iRet = (-1);
	char *pCh;
	struct CpuFreqStatNums *pCpuFreq = NULL;

	memset( statOut, 0, sizeof(statOut) );

	pIn->cpuNUM = 1;
	pCpuFreq = &(pIn->cpu0);

	/* get current cpu freq ... */
	pCpuFreq->num = 0;
	getCpuFreqitem( 1 );	///cpu0 current freq
	pCh = statOut;	///set start point

	/* parse current cpu freq ... */
	strcpy( pCpuFreq->cur_freq, pCh );
#ifdef _STATDEBUG_
	printf("pCpuFreq->cur_freq[%s]\n", pCpuFreq->cur_freq );
#endif

_pEXIT:
	return iRet;
}




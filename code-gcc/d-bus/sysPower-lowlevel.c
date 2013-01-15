/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#include "toolhelps.h"



/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
#if 0
	#define	DBGFSET		(dDBG|dINFO|dERR)
#else
	#define	DBGFSET		(dINFO|dERR)
#endif
#define	dF(x)		(DBGFSET&x)




static int CPUcoreActivated = 4;



void setCPUcoreDOWN( int CoreNum )
{
	/* CoreNum = 0,1,2,3 */
	
	char devCPU[] = "echo 0 > /sys/devices/system/cpu/cpuX/online";
	char base0 = '0';
	FILE *fp;

	devCPU[36] = base0+CoreNum;
#ifdef __ARM_CODE__
	fp = (FILE *)popen( devCPU, "r" );
	if( fp == NULL )
	{
		spMSG( dF(dERR), "%s:%s: Failed on %s !!!\n", __FILE__, __FUNCTION__, devCPU );
		return;
	}
	sleep(1);
	pclose( fp );
#else
	spMSG( dF(dERR), "%s:%s: OK on %s !!!\n", __FILE__, __FUNCTION__, devCPU );
#endif
	CPUcoreActivated = CoreNum;	
	spMSG( dF(dERR), "%s:%s: set core [%d] DOWN\n", __FILE__, __FUNCTION__, CoreNum );
}


void setCPUcoreUP( int CoreNum )
{
	/* CoreNum = 0,1,2,3 */

	char devCPU[] = "echo 1 > /sys/devices/system/cpu/cpuX/online";
	char base0 = '0';
	FILE *fp;

	devCPU[36] = base0+CoreNum;
#ifdef __ARM_CODE__
	fp = (FILE *)popen( devCPU, "r" );
	if( fp == NULL )
	{
		spMSG( dF(dERR), "%s:%s: Failed on %s !!!\n", __FILE__, __FUNCTION__, devCPU );
		return;
	}
	sleep(1);
	pclose( fp );
#else
	spMSG( dF(dERR), "%s:%s: OK on %s !!!\n", __FILE__, __FUNCTION__, devCPU );
#endif
	CPUcoreActivated = CoreNum + 1;
	spMSG( dF(dERR), "%s:%s: set core[%d] UP\n", __FILE__, __FUNCTION__, CoreNum );
}


int setCPUcoreActivatedNumber( int num )
{
	int iRet = -1;
	

	if( CPUcoreActivated == num )
	{
		spMSG( dF(dDBG), "%s:%s: set core[%d] already [%d] \n", __FILE__, __FUNCTION__, num, CPUcoreActivated );
		iRet = 0;
	}
	else
	{
		if( num > CPUcoreActivated )
		{	/* steps up */ 
			int iDiff = 0;
			int iCurN = 0;
			iDiff = num - CPUcoreActivated;
			iCurN = CPUcoreActivated;
			
			while( iDiff > 0 )
			{
				setCPUcoreUP( iCurN );
				iDiff--;
				iCurN++;
			}
		}
		else
		{	/* steps down */
			int iDiff = 0;
			int iCurN = 0;
			iDiff = CPUcoreActivated - num;
			iCurN = CPUcoreActivated - 1;
			
			while( iDiff > 0 )
			{
				setCPUcoreDOWN( iCurN );
				iDiff--;
				iCurN--;
			} 
		}
		iRet = 0;
	}

	
	return iRet;
}


int ifCoreNumValid( int iCore )
{
	int iRet = -1;
	
	if( (iCore > 0) && (iCore < 5) )
	{
		iRet = 0;
	}
	
	return iRet;
}


int setCPUdvfs( int iOn )
{
	int iRet = -1;
	FILE *fp;
	char devDVFSon[] = "echo interactive > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";
	char devDVFSoff[] = "echo userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";

#ifdef __ARM_CODE__
	if( 0 == iOn )
		fp = (FILE *)popen( devDVFSoff, "r" );
	else
	if( 1 == iOn )
		fp = (FILE *)popen( devDVFSon, "r" );
	else
		;

	if( fp == NULL )
	{
		spMSG( dF(dERR), "%s:%s: Failed on %s !!!\n", __FILE__, __FUNCTION__, (0==iOn)?devDVFSoff:devDVFSon );
		return iRet;
	}
	sleep(1);
	pclose( fp );
	iRet = 0;
#else
	spMSG( dF(dERR), "%s:%s: OK on %s !!!\n", __FILE__, __FUNCTION__, (0==iOn)?devDVFSoff:devDVFSon );
	iRet = 0;
#endif

	return iRet;
}


int setCPUspeed( int nSpeed )
{
	int iRet = -1;
	int iLoop = 0;
	FILE *fp;
	char base0 = '0';
	char devCPUspeed400[] = "echo 396000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed";
	char devCPUspeed800[] = "echo 792000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed";
	char devCPUspeed1000[] = "echo 996000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed";
	char devCPUcurrspeed[] = "cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq";
	char *devCPUspeed = NULL;
	char stdOut[32];
	
	
	if( nSpeed == 400 )
		devCPUspeed = devCPUspeed400;
	else
	if( nSpeed == 800 )
		devCPUspeed = devCPUspeed800;
	else
		devCPUspeed = devCPUspeed1000;

#ifdef __ARM_CODE__
	
	iLoop = 0;
	while( iLoop < CPUcoreActivated )
	{
		devCPUspeed[41] = base0+iLoop;
		devCPUcurrspeed[31] = base0+iLoop;
		fp = (FILE *)popen( devCPUspeed, "r" );

		if( fp == NULL )
		{
			spMSG( dF(dERR), "%s:%s: Failed on %s !!!\n", __FILE__, __FUNCTION__, devCPUspeed );
		}
		else
		{
			pclose( fp );
			sleep(1);
			spMSG( dF(dINFO), "%s:%s: CPU core%d set freq. %dMHz !!!\n", __FILE__, __FUNCTION__, iLoop, nSpeed );
			/* check it */
			fp = (FILE *)popen( devCPUcurrspeed, "r" );
			while( fgets( stdOut, sizeof(stdOut)-1, fp ) != NULL )
			{
				/* printf("%s", stdOut ); */ 
				spMSG( dF(dINFO), "%s:%s: CPU core%d now freq. %sMHz.\n", __FILE__, __FUNCTION__, iLoop, stdOut );
			}
			pclose( fp );
		}
		
		iLoop++;
	}
	iRet = 0;
#else
	spMSG( dF(dERR), "%s:%s: OK on %s !!!\n", __FILE__, __FUNCTION__, devCPUspeed );
	iRet = 0;
#endif

	return iRet;
}


int setCPUsuspend( void )
{
	int iRet = -1;
	FILE *fp;
	char suspend[] = "echo mem > /sys/power/state";

#ifdef __ARM_CODE__
#if 1
	fp = (FILE *)popen( suspend, "r" );

	if( fp == NULL )
	{
		spMSG( dF(dERR), "%s:%s: Failed on %s !!!\n", __FILE__, __FUNCTION__, suspend );
		return iRet;
	}
	sleep(1);
	pclose( fp );
#else
	/*
	sprintf(suspend, "echo mem > /sys/power/state");
	*/
	system(suspend); 
#endif
	iRet = 0;
#else
	spMSG( dF(dERR), "%s:%s: OK on %s !!!\n", __FILE__, __FUNCTION__, suspend );
	iRet = 0;
#endif

	return iRet;
}



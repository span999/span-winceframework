/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>

#include "toolhelps.h"
#include "ipcpacket.h"
#include "sysPowerSRV.h"
#include "sysIPCSRV.h"



/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
/* #define	DBGFSET		(dDBG|dINFO|dERR) */
#define	DBGFSET		(dINFO|dERR)
#define	dF(x)		(DBGFSET&x)






static pthread_t thread_id;
static int CPUcoreActivated = 4;
static char verStr[] = "v2.1";

/* for mutex */
static pthread_mutex_t mutex;
static int mutexINITED = 0;



static void setCPUcoreDOWN( int CoreNum )
{
	/* CoreNum = 0,1,2,3 */
	
	char devCPU[] = "echo 0 > /sys/devices/system/cpu/cpuX/online";
	char base0 = '0';
	FILE *fp;

	devCPU[36] = base0+CoreNum;
#ifdef __ARM_CODE__
	fp = popen( devCPU, "r" );
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


static void setCPUcoreUP( int CoreNum )
{
	/* CoreNum = 0,1,2,3 */

	char devCPU[] = "echo 1 > /sys/devices/system/cpu/cpuX/online";
	char base0 = '0';
	FILE *fp;

	devCPU[36] = base0+CoreNum;
#ifdef __ARM_CODE__
	fp = popen( devCPU, "r" );
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


static int setCPUcoreActivatedNumber( int num )
{
	int iRet = -1;
	
	spMxL( &mutex, &mutexINITED );
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
	
	spMxU( &mutex, &mutexINITED );
	
	return iRet;
}


static int ifCoreNumValid( int iCore )
{
	int iRet = -1;
	
	if( (iCore > 0) && (iCore < 5) )
	{
		iRet = 0;
	}
	
	return iRet;
}


static int setCPUdvfs( int iOn )
{
	int iRet = -1;
	FILE *fp;
	char devDVFSon[] = "echo interactive > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";
	char devDVFSoff[] = "echo userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";

#ifdef __ARM_CODE__
	if( 0 == iOn )
		fp = popen( devDVFSoff, "r" );
	else
	if( 1 == iOn )
		fp = popen( devDVFSon, "r" );
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


static int setCPUspeed( int nSpeed )
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
		fp = popen( devCPUspeed, "r" );

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
			fp = popen( devCPUcurrspeed, "r" );
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


static int PowerCmdParser( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	int iParam1 = -1;
	
	if( pCmd )
	{
		/* get power command */
		iRet = getPowerCmdID( pCmd );
		spMSG( dF(dDBG), "%s:%s: get power command [%d] \n", __FILE__, __FUNCTION__, iRet );
		
		/* proceed the command */
		switch( iRet )
		{
			case GETCPUACTIVATEDNUM:				
				setPowerCmdReturn( pCmd, CPUcoreActivated );
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;

			case SETCPUACTIVATEDNUM:
				iParam1 = getPowerCmdParam1( pCmd );
				if( 0 == ifCoreNumValid( iParam1 ) )
				{
					iRet = setCPUcoreActivatedNumber( iParam1 );
					setPowerCmdReturn( pCmd, iRet );
					spMSG( dF(dDBG), "%s:%s: get power command iParam1[%d] %s !!\n", __FILE__, __FUNCTION__, iParam1, (0==iRet)?"OK":"Fail" );
				}
				else
				{
					setPowerCmdReturn( pCmd, -1 );
					spMSG( dF(dERR), "%s:%s: get power command iParam1[%d] over specification !!\n", __FILE__, __FUNCTION__, iParam1 );
				}
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;

			case SETCPUDVFS:
				iParam1 = getPowerCmdParam1( pCmd );
				iRet = setCPUdvfs( iParam1 );
				setPowerCmdReturn( pCmd, iRet );
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;

			case SETCPUSPEED:
				iParam1 = getPowerCmdParam1( pCmd );
				setCPUdvfs( 0 );
				iRet = setCPUspeed( iParam1 );
				setPowerCmdReturn( pCmd, iRet );
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;
				
			case LOOPBACKTEST:
				setPowerCmdReturn( pCmd, getPowerCmdParam1(pCmd) );
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;
				
			default:
				break;
		}	/* switch */
		iRet = 0;
	}

	return iRet;
}



#define	_USE_USLEEP_
#define CMD_INTERVAL	200		/* 200 ms polling */
#define US_MS			1000000
#define	HEREMSGTIME		45


void *mainPowerMGR( void *argv )
{
	int iLoop = 0;
	struct sysPowerCmd pwrCmd;
	struct ipcpacket ipcPack;
	int Len = 0;
	int iRet = -1;

	while( 1 )
	{
		#ifdef _USE_USLEEP_
		usleep( ((0>iRet)?CMD_INTERVAL:0) );
		#else
		sleep( ((0>iRet)?1:0) );
		#endif


		#ifdef _USE_USLEEP_
		if( ( ++iLoop % ((HEREMSGTIME*US_MS)/CMD_INTERVAL) ) == 0 )
			spMSG( dF(dINFO), "%s:%s: %s is here ... %d \n", __FILE__, __FUNCTION__, verStr, iLoop );
		#else
		if( ++iLoop % HEREMSGTIME == 0 )
			spMSG( dF(dINFO), "%s:%s: %s is here ... %d \n", __FILE__, __FUNCTION__, verStr, iLoop );
		#endif
			
		/*  */
	#if 0
		Len = sizeof(struct sysPowerCmd);
		iRet = spIPCpayloadGet( NULL, (char *)&pwrCmd, &Len );
		if( iRet > 0 )
		{
			spMSG( dF(dDBG), "%s:%s: get data %d bytes... \n", __FILE__, __FUNCTION__, iRet );
			PowerCmdDump( &pwrCmd );	
		}
	#else
		/* try to get data pack */
		iRet = spIPCPackBuffOUT( &ipcPack );
		if( 0 == iRet )
		{
			/* parse the payload */
			Len = sizeof(struct sysPowerCmd);
			iRet = spIPCpayloadGet( &ipcPack, (char *)&pwrCmd, &Len );
			if( iRet > 0 )
			{
				spMSG( dF(dDBG), "%s:%s: get data %d bytes... \n", __FILE__, __FUNCTION__, iRet );
				if( dF(dDBG) ) PowerCmdDump( &pwrCmd );
				
				iRet = PowerCmdParser( &pwrCmd );
				
				if( 0 == iRet )
					iRet = spIPCPackResponse( &ipcPack, (char *)&pwrCmd, sizeof(struct sysPowerCmd) );

			}
			else
				spMSG( dF(dERR), "%s:%s: get payload data %d bytes... fail !! \n", __FILE__, __FUNCTION__, iRet );
		}
	#endif	
	}

	spMSG( dF(dINFO), "%s:%s: Exit !!! \n", __FILE__, __FUNCTION__ );

	return 0;
}


void IPCCallBack( void )
{

	spMSG( dF(dDBG), "IPCCallBack in !!! \n", __FILE__, __FUNCTION__ );
	return;
}


int main( int argc, char *argv[] )
{
	int iRet = -1;

	/* setup ipc communication routine */
	spIPCInit();
	/* set server type & callback */
	spIPCinitServer( POWERMGR, IPCCallBack );
	
	spMxI( &mutex, &mutexINITED );
	
	/* create routine for power manager */
	pthread_create( &thread_id, NULL, &mainPowerMGR, NULL );

#ifdef __ARM_CODE__
	/* loop forever */
	while(1)
		sleep(3);
#else
	getchar();
#endif	/* #ifdef __ARM_CODE__ */

	spMxD( &mutex, &mutexINITED );
	
	spIPCDeinit();
	spMSG( dF(dINFO), "%s:%s: Exit !!! \n", __FILE__, __FUNCTION__ );
	return iRet;
}

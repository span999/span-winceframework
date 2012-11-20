/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>

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


static int ifCoreNumValid( int iCore )
{
	int iRet = -1;
	
	if( (iCore > 0) && (iCore < 5) )
	{
		iRet = 0;
	}
	
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
					/* fake code .... */
					CPUcoreActivated = iParam1;
					setPowerCmdReturn( pCmd, 0 );
					spMSG( dF(dDBG), "%s:%s: get power command iParam1[%d] OK !!\n", __FILE__, __FUNCTION__, iParam1 );
				}
				else
				{
					setPowerCmdReturn( pCmd, -1 );
					spMSG( dF(dERR), "%s:%s: get power command iParam1[%d] over specification !!\n", __FILE__, __FUNCTION__, iParam1 );
				}
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;
				
			case LOOPBACKTEST:
				setPowerCmdReturn( pCmd, getPowerCmdParam1(pCmd) );
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;
				
			default:
				break;
		}	///switch
		iRet = 0;
	}

	return iRet;
}



#define	_USE_USLEEP_
#define CMD_INTERVAL	200		/* 200 ms polling */
#define US_MS			1000000

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
		if( ( ++iLoop % ((5*US_MS)/CMD_INTERVAL) ) == 0 )
			spMSG( dF(dINFO), "%s:%s: is here ... %d \n", __FILE__, __FUNCTION__, iLoop );
		#else
		if( ++iLoop % 5 == 0 )
			spMSG( dF(dINFO), "%s:%s: is here ... \n", __FILE__, __FUNCTION__ );
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

	return;
}


void IPCCallBack( void )
{

	spMSG( dF(dDBG), "IPCCallBack in !!! \n", __FILE__, __FUNCTION__ );
	return;
}


int main( int argc, char *argv )
{
	int iRet = -1;
	int iLoop = 0;


	/* setup ipc communication routine */
	spIPCInit();
	/* set server type & callback */
	spIPCinitServer( POWERMGR, IPCCallBack );
	
	/* create routine for power manager */
	pthread_create( &thread_id, NULL, &mainPowerMGR, NULL );

#if 0	/* loop forever */
	while(1)
		;
#else
	getchar();
#endif


	spIPCDeinit();
	spMSG( dF(dINFO), "%s:%s: Exit !!! \n", __FILE__, __FUNCTION__ );
	return iRet;
}

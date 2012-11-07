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




static char PROGRAMNAME[] = "sysPowerMGR";
static pthread_t thread_id;

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
			spQMSG( "%s is here ... %d \n", "mainPowerMGR", iLoop );
		#else
		if( ++iLoop % 5 == 0 )
			spQMSG( "%s is here ... \n", "mainPowerMGR" );
		#endif
			
		/*  */
	#if 0
		Len = sizeof(struct sysPowerCmd);
		iRet = spIPCpayloadGet( NULL, (char *)&pwrCmd, &Len );
		if( iRet > 0 )
		{
			spQMSG( "%s get data %d bytes... \n", "mainPowerMGR", iRet );
			PowerCmdDump( &pwrCmd );	
		}
	#else
		/* try to get data pack */
		iRet = spIPCPackBuffOUT( &ipcPack );
		if( 0 == iRet )
		{
			Len = sizeof(struct sysPowerCmd);
			iRet = spIPCpayloadGet( &ipcPack, (char *)&pwrCmd, &Len );
			if( iRet > 0 )
			{
				struct sysPowerCmd *pThisCmd = NULL;
				spQMSG( "%s get data %d bytes... \n", "mainPowerMGR", iRet );
				PowerCmdDump( &pwrCmd );
				
				/* get power command */
				iRet = getPowerCmdID( &pwrCmd );
				spQMSG( "%s get power command %d \n", "mainPowerMGR", iRet );
				/* proceed the command */
				switch( iRet )
				{
					case 1:				
					case 2:
						pThisCmd = (struct sysPowerCmd *)ipcPack.payload;
						pThisCmd->rspReturn = 5;
						pThisCmd->rsptimestamp = spGetTimetick();
						iRet = spIPCPackResponse( &ipcPack );
						break;
						
					default:
						break;
				}	///switch
				
				/* response */
			}
		}
	#endif	
	}

	spQMSG( "Exit %s !!! \n", PROGRAMNAME );

	return;
}


void IPCCallBack( void )
{

	spQMSG( "IPCCallBack in %s !!! \n", PROGRAMNAME );
	return;
}


int main( int argc, char *argv )
{
	int iRet = -1;
	int iLoop = 0;


	/* setup ipc communication routine */
	/* set server type & callback */
	spIPCinitServer( POWERMGR, IPCCallBack );
	
	/* create routine for power manager */
	pthread_create( &thread_id, NULL, &mainPowerMGR, NULL );

	/* wait until loop end */
	while( 1 )
	{
		sleep( 1 );
		if( ++iLoop % 5 == 0 )
			spQMSG( "%s is here ... \n", PROGRAMNAME );
	}

	spQMSG( "Exit %s !!! \n", PROGRAMNAME );
	return iRet;
}

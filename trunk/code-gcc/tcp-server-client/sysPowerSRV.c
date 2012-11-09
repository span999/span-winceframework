/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <string.h>

#include "toolhelps.h"
#include "ipcpacket.h"
#include "sysIPCSRV.h"
#include "sysPowerCOMM.h"
#include "sysPowerSRV.h"




/*
	these function routine should be placed at sysPowerSRV.so / sysPowerSRV.a
*/

static pthread_mutex_t mutex;
static int mutexON = 0;

static int mutex_INIT( void )
{
	int iRet = -1;
	
	if( !mutexON )
	{
		if( pthread_mutex_init( &mutex, NULL ) != 0 )
		{
			spQMSG( "%s: mutex init failed !!\n", __FUNCTION__ );
		}
		else
		{
			mutexON = 1;
			iRet = 0;
		}
	}
	else
		iRet = 0;
	
	return iRet;
}


static int mutex_DESTROY( void )
{
	int iRet = -1;

	if( mutexON )
	{
		pthread_mutex_destroy( &mutex );
		spQMSG( "%s: mutex destroy !!\n", __FUNCTION__ );
		mutexON = 0;
		iRet = 0;
	}
	
	return iRet;
}


static int mutex_LOCK( void )
{
	int iRet = -1;
	
	mutex_INIT();
	if( mutexON )
		pthread_mutex_lock( &mutex );
	else
		spQMSG( "%s: fail !!\n", __FUNCTION__ );
	
	return iRet;
}


static int mutex_UNLOCK( void )
{
	int iRet = -1;
	
	mutex_INIT();
	if( mutexON )
		pthread_mutex_unlock( &mutex );
	else
		spQMSG( "%s: fail !!\n", __FUNCTION__ );
	
	return iRet;
}


static void PowerCmdInit( struct sysPowerCmd *pCmd )
{

	if( pCmd )
	{
		pCmd->packType = POWERMGRPACKSIGN;
		pCmd->packSN = spGetTimetick();
		pCmd->cmdID = -1;
		pCmd->cmdParam1 = -1;
		pCmd->cmdParam2 = -1;
		pCmd->cmdtimestamp = -1;
		pCmd->rspReturn = -1;
		pCmd->rspParam1 = -1;
		pCmd->rspParam2 = -1;
		pCmd->rsptimestamp = -1;
	}

	return;
} 



static void PowerCmdTime( struct sysPowerCmd *pCmd )
{
	if( pCmd )
		pCmd->cmdtimestamp = spGetTimetick();
	return;
}


static int PowerCmdSend( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	
	PowerCmdTime( pCmd );
	/* call ipc */
	spIPCsend( (char *)pCmd, sizeof(struct sysPowerCmd), POWERMGR );

	return iRet;
} 


static int PowerCmdRequest( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	int iSize = 0;
	
	iSize = sizeof(struct sysPowerCmd);
	PowerCmdTime( pCmd );	/* mark the time */
	/* call ipc */ /* wait for response */
	iRet = spIPCrequest( (char *)pCmd, &iSize, POWERMGR );

	return iRet;
} 


void PowerCmdDump( struct sysPowerCmd *pCmd )
{

	if( pCmd )
	{
		spQMSG( "Power Cmd sign [%d]\n", pCmd->packType );
		spQMSG( "Power Cmd SN [%u/0x%02x]\n", pCmd->packSN, pCmd->packSN );
		spQMSG( "Power Cmd ID [%d/0x%02x]\n", pCmd->cmdID, pCmd->cmdID );
		spQMSG( "Power Cmd Param1 [%d/0x%02x]\n", pCmd->cmdParam1, pCmd->cmdParam1 );
		spQMSG( "Power Cmd Param2 [%d/0x%02x]\n", pCmd->cmdParam2, pCmd->cmdParam2 );
		spQMSG( "Power Cmd timestamp [%u/0x%08x]\n", pCmd->cmdtimestamp, pCmd->cmdtimestamp );
		spQMSG( "Power Ret value [%d/0x%02x]\n", pCmd->rspReturn, pCmd->rspReturn );
		spQMSG( "Power Rsp Param1 [%d/0x%02x]\n", pCmd->rspParam1, pCmd->rspParam1 );
		spQMSG( "Power Rsp Param2 [%d/0x%02x]\n", pCmd->rspParam2, pCmd->rspParam2 );
		spQMSG( "Power Rsp timestamp [%u/0x%08x]\n", pCmd->rsptimestamp, pCmd->rsptimestamp );
	}

	return;
} 


int getPowerCmdID( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( pCmd->packType == POWERMGRPACKSIGN )
		{
			iRet = pCmd->cmdID;
		}
		else
		{
			spQMSG( "ERROR !!! power command sign error [%d]\n", pCmd->packType );
			iRet = 0;
		}	
	}
	
	return iRet;
}


int getPowerCmdReturn( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( pCmd->packType == POWERMGRPACKSIGN )
		{
			iRet = pCmd->rspReturn;
		}
		else
		{
			spQMSG( "ERROR !!! power command sign error [%d]\n", pCmd->packType );
			iRet = 0;
		}	
	}
	
	return iRet;
}


int setPowerCmdParam1( struct sysPowerCmd *pCmd, int iVal )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( pCmd->packType == POWERMGRPACKSIGN )
		{
			pCmd->cmdParam1 = iVal;
			iRet = 0;
		}
		else
		{
			spQMSG( "ERROR !!! power command sign error [%d]\n", pCmd->packType );
		}	
	}
	
	return iRet;
}


int getPowerCmdParam1( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( pCmd->packType == POWERMGRPACKSIGN )
		{
			iRet = pCmd->cmdParam1;
		}
		else
		{
			spQMSG( "ERROR !!! power command sign error [%d]\n", pCmd->packType );
			iRet = 0;
		}	
	}
	
	return iRet;
}


int setPowerCmdReturn( struct sysPowerCmd *pCmd, int iVal )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( pCmd->packType == POWERMGRPACKSIGN )
		{
			pCmd->rspReturn = iVal;
			iRet = 0;
		}
		else
		{
			spQMSG( "ERROR !!! power command sign error [%d]\n", pCmd->packType );
		}	
	}
	
	return iRet;
}


int setPowerCmdRsptime( struct sysPowerCmd *pCmd, long iVal )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( pCmd->packType == POWERMGRPACKSIGN )
		{
			pCmd->rsptimestamp = iVal;
			iRet = 0;
		}
		else
		{
			spQMSG( "ERROR !!! power command sign error [%d]\n", pCmd->packType );
		}	
	}
	
	return iRet;
}


int getCPUActivatedNum( void )
{
	int iRet = -1;
	struct sysPowerCmd PwrCmd;

	PowerCmdInit( &PwrCmd );
	/* pack the data for Power Manager */
	PwrCmd.cmdID = GETCPUACTIVATEDNUM;

	/* issue command to Power Manager */
	/* iRet = PowerCmdSend( &PwrCmd ); */
	/* wait & get the response from Power Manager */
	iRet = PowerCmdRequest( &PwrCmd );
	/* PowerCmdDump( &PwrCmd ); */

	if( 0 == iRet )
	{
		iRet = getPowerCmdReturn( &PwrCmd );
	}

	return iRet;
}


int setCPUActivatedNum( int num )
{
	int iRet = -1;
	struct sysPowerCmd PwrCmd;

	PowerCmdInit( &PwrCmd );
	/* pack the data for Power Manager */
	PwrCmd.cmdID = SETCPUACTIVATEDNUM;

	/* issue command to Power Manager */
	/* iRet = PowerCmdSend( &PwrCmd ); */
	/* wait & get the response from Power Manager */
	iRet = PowerCmdRequest( &PwrCmd );

	if( 0 == iRet )
	{
		iRet = getPowerCmdReturn( &PwrCmd );
	}

	return iRet;
}


int loopbackTest( int test )
{
	int iRet = -1;
	struct sysPowerCmd PwrCmd;

	mutex_LOCK();
	
	PowerCmdInit( &PwrCmd );
	/* pack the data for Power Manager */
	PwrCmd.cmdID = LOOPBACKTEST;

	setPowerCmdParam1( &PwrCmd, test );

	/* issue command to Power Manager */
	/* iRet = PowerCmdSend( &PwrCmd ); */
	/* wait & get the response from Power Manager */
	iRet = PowerCmdRequest( &PwrCmd );

	if( 0 == iRet )
	{
		iRet = getPowerCmdReturn( &PwrCmd );
	}
	
	mutex_UNLOCK();
	
	return iRet;
}

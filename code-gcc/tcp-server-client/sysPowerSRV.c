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



/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
/* #define	DBGFSET		(dDBG|dINFO|dERR) */
#define	DBGFSET		(dINFO|dERR)
#define	dF(x)		(DBGFSET&x)


/*
	these function routine should be placed at sysPowerSRV.so / sysPowerSRV.a
*/






/* for mutex */
static pthread_mutex_t mutex;
static int mutexINITED = 0;





static int chkPowerCmdsign( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( pCmd->packType == POWERMGRPACKSIGN )
		{
			iRet = 0;
		}
		else
		{
			spMSG( dF(dERR), "ERROR !!! %s:%s: power command sign error [%d]\n", __FILE__, __FUNCTION__, pCmd->packType );
		}	
	}
	else
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail !!\n", __FILE__, __FUNCTION__ );

	return iRet;
}


int setPowerCmdID( struct sysPowerCmd *pCmd, int iVal )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( 0 == chkPowerCmdsign( pCmd ) )
		{
			pCmd->cmdID = iVal;
			iRet = 0;
		}
	}
	else
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int getPowerCmdID( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( 0 == chkPowerCmdsign( pCmd ) )
		{
			iRet = pCmd->cmdID;
		}
	}
	else
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


static void setPowerCmdtime( struct sysPowerCmd *pCmd )
{
	if( pCmd )
		pCmd->cmdtimestamp = spGetTimetick();
		
	return;
}


int getPowerCmdReturn( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( 0 == chkPowerCmdsign( pCmd ) )
		{
			iRet = pCmd->rspReturn;
		}
	}
	else
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int setPowerCmdParam1( struct sysPowerCmd *pCmd, int iVal )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( 0 == chkPowerCmdsign( pCmd ) )
		{
			pCmd->cmdParam1 = iVal;
			iRet = 0;
		}
	}
	else
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int getPowerCmdParam1( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( 0 == chkPowerCmdsign( pCmd ) )
		{
			iRet = pCmd->cmdParam1;
		}
	}
	else
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int setPowerCmdReturn( struct sysPowerCmd *pCmd, int iVal )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( 0 == chkPowerCmdsign( pCmd ) )
		{
			pCmd->rspReturn = iVal;
			iRet = 0;
		}
	}
	else
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int setPowerCmdRsptime( struct sysPowerCmd *pCmd, long iVal )
{
	int iRet = -1;
	
	if( pCmd )
	{
		if( 0 == chkPowerCmdsign( pCmd ) )
		{
			pCmd->rsptimestamp = iVal;
			iRet = 0;
		}
	}
	else
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail !!\n", __FILE__, __FUNCTION__ );
	
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
		pCmd->cmdtimestamp = 0;
		pCmd->rspReturn = -1;
		pCmd->rspParam1 = -1;
		pCmd->rspParam2 = -1;
		pCmd->rsptimestamp = 0;
	}
	else
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail \n", __FILE__, __FUNCTION__ );
	
	return;
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


static int PowerCmdSend( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	
	setPowerCmdtime( pCmd );
	/* call ipc */
	iRet = spIPCsend( (char *)pCmd, sizeof(struct sysPowerCmd), POWERMGR );

	if( 0 != iRet )
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail \n", __FILE__, __FUNCTION__ );

	return iRet;
} 


static int PowerCmdRequest( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	int iSize = 0;
	
	iSize = sizeof(struct sysPowerCmd);
	setPowerCmdtime( pCmd );	/* mark the time */
	/* call ipc */ /* wait for response */
	iRet = spIPCrequest( (char *)pCmd, &iSize, POWERMGR );

	if( 0 != iRet )
		spMSG( dF(dERR), "ERROR !!! %s:%s: fail \n", __FILE__, __FUNCTION__ );
	
	return iRet;
} 




int sPSgetCPUActivatedNum( void )
{
	int iRet = -1;
	struct sysPowerCmd PwrCmd;

	spMxL( &mutex, &mutexINITED );
	
	PowerCmdInit( &PwrCmd );
	/* pack the data for Power Manager */
	/* PwrCmd.cmdID = GETCPUACTIVATEDNUM; */
	setPowerCmdID( &PwrCmd, GETCPUACTIVATEDNUM);

	/* issue command to Power Manager */
	/* iRet = PowerCmdSend( &PwrCmd ); */
	/* wait & get the response from Power Manager */
	iRet = PowerCmdRequest( &PwrCmd );
	/* PowerCmdDump( &PwrCmd ); */

	if( 0 == iRet )
	{
		iRet = getPowerCmdReturn( &PwrCmd );
	}

	spMxU( &mutex, &mutexINITED );
	
	return iRet;
}


int sPSsetCPUActivatedNum( int num )
{
	int iRet = -1;
	struct sysPowerCmd PwrCmd;

	spMxL( &mutex, &mutexINITED );
	
	PowerCmdInit( &PwrCmd );
	/* pack the data for Power Manager */
	/* PwrCmd.cmdID = SETCPUACTIVATEDNUM; */
	setPowerCmdID( &PwrCmd, SETCPUACTIVATEDNUM);
	setPowerCmdParam1( &PwrCmd, num );

	/* issue command to Power Manager */
	/* iRet = PowerCmdSend( &PwrCmd ); */
	/* wait & get the response from Power Manager */
	iRet = PowerCmdRequest( &PwrCmd );

	if( 0 == iRet )
	{
		iRet = getPowerCmdReturn( &PwrCmd );
	}
	
	spMxU( &mutex, &mutexINITED );

	return iRet;
}


int sPSsetCPUDVFS( int num )
{
	int iRet = -1;
	struct sysPowerCmd PwrCmd;

	spMxL( &mutex, &mutexINITED );
	
	PowerCmdInit( &PwrCmd );
	/* pack the data for Power Manager */
	setPowerCmdID( &PwrCmd, SETCPUDVFS );
	setPowerCmdParam1( &PwrCmd, num );

	/* issue command to Power Manager */
	/* iRet = PowerCmdSend( &PwrCmd ); */
	/* wait & get the response from Power Manager */
	iRet = PowerCmdRequest( &PwrCmd );

	if( 0 == iRet )
	{
		iRet = getPowerCmdReturn( &PwrCmd );
	}
	
	spMxU( &mutex, &mutexINITED );

	return iRet;
}


int sPSsetCPUspeed( int nMHz )
{
	int iRet = -1;
	struct sysPowerCmd PwrCmd;

	/* set limitation here */
	if( (nMHz != 400) && (nMHz != 800) && (nMHz != 1000) )
	{
		spMSG( dF(dERR), "%s:%s: parameter invalid [%d] !!\n", __FILE__, __FUNCTION__, nMHz );
		iRet = -2;
		return iRet;
	}

	spMxL( &mutex, &mutexINITED );
	
	PowerCmdInit( &PwrCmd );
	/* pack the data for Power Manager */
	setPowerCmdID( &PwrCmd, SETCPUSPEED );
	setPowerCmdParam1( &PwrCmd, nMHz );

	/* issue command to Power Manager */
	/* iRet = PowerCmdSend( &PwrCmd ); */
	/* wait & get the response from Power Manager */
	iRet = PowerCmdRequest( &PwrCmd );

	if( 0 == iRet )
	{
		iRet = getPowerCmdReturn( &PwrCmd );
	}
	
	spMxU( &mutex, &mutexINITED );

	return iRet;
}


int sPSloopbackTest( int test )
{
	int iRet = -1;
	struct sysPowerCmd PwrCmd;

	spMxL( &mutex, &mutexINITED );
	
	PowerCmdInit( &PwrCmd );
	/* pack the data for Power Manager */
	/* PwrCmd.cmdID = LOOPBACKTEST; */
	setPowerCmdID( &PwrCmd, LOOPBACKTEST);

	setPowerCmdParam1( &PwrCmd, test );

	if( dF(dDBG) ) PowerCmdDump( &PwrCmd );
	/* issue command to Power Manager */
	/* iRet = PowerCmdSend( &PwrCmd ); */
	/* wait & get the response from Power Manager */
	iRet = PowerCmdRequest( &PwrCmd );
	if( dF(dDBG) ) PowerCmdDump( &PwrCmd );

	if( 0 == iRet )
	{
		iRet = getPowerCmdReturn( &PwrCmd );
	}

	spMxU( &mutex, &mutexINITED );
	
	if( iRet != test )
		spMSG( dF(dERR), "%s:%s: fail !!\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


int sysPowerSRVInit( void )
{
	spMxI( &mutex, &mutexINITED );
	
	return 0;
}


int sysPowerSRVDeinit( void )
{
	spMxD( &mutex, &mutexINITED );
	
	return 0;
}



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




static void PowerCmdInit( struct sysPowerCmd *pCmd )
{

	if( pCmd )
	{
		pCmd->packType = POWERMGRPACKSIGN;
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


void PowerCmdDump( struct sysPowerCmd *pCmd )
{

	if( pCmd )
	{
		spQMSG( "Power Cmd sign [%d]\n", pCmd->packType );
		spQMSG( "Power Cmd ID [%d/0x%02x]\n", pCmd->cmdID, pCmd->cmdID );
		spQMSG( "Power Cmd Param1 [%d/0x%02x]\n", pCmd->cmdParam1, pCmd->cmdParam1 );
		spQMSG( "Power Cmd Param2 [%d/0x%02x]\n", pCmd->cmdParam2, pCmd->cmdParam2 );
		spQMSG( "Power Cmd timestamp [%d/0x%02x]\n", pCmd->cmdtimestamp, pCmd->cmdtimestamp );
		spQMSG( "Power Ret value [%d/0x%02x]\n", pCmd->rspReturn, pCmd->rspReturn );
		spQMSG( "Power Rsp Param1 [%d/0x%02x]\n", pCmd->rspParam1, pCmd->rspParam1 );
		spQMSG( "Power Rsp Param2 [%d/0x%02x]\n", pCmd->rspParam2, pCmd->rspParam2 );
		spQMSG( "Power Rsp timestamp [%d/0x%02x]\n", pCmd->rsptimestamp, pCmd->rsptimestamp );
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
			iRet = 0;
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
	iRet = PowerCmdSend( &PwrCmd );

	/* wait & get the response from Power Manager */

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
	iRet = PowerCmdSend( &PwrCmd );

	/* wait & get the response from Power Manager */

	return iRet;
}


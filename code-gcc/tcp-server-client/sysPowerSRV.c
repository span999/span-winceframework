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


static int PowerCmdSend( struct sysPowerCmd *pCmd )
{
	int iRet = -1;

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




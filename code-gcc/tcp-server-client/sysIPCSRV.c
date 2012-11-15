/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/*
#include <pthread.h>
*/
#include <string.h>
/*
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
*/
#include "toolhelps.h"
#include "ipcpacket.h"
#include "sysIPCSRV.h"
#include "sysIPCSRVtcp.h"
#include "sysIPCSRVbuffer.h"
/*
#include "spRingBuf.h"
*/



static tSRVMGRTYP serverType = 0;
static PFNIPCCALLBACK ipcCallback = NULL;




int spIPCpayloadGet( struct ipcpacket *pPack, char *pBuf, int *pLen )
{
	int iRet = -1;
	
	if( pPack )
	{
		if( 1 )
		{
			struct ipcpacket *pThisPack = NULL;	
			
			pThisPack = pPack;

			iRet = 0;
			if( pBuf && *pLen >= pThisPack->payloadnum )
			{
				iRet = pThisPack->payloadnum;
				memcpy( pBuf, pThisPack->payload, pThisPack->payloadnum );
			}
		}
	}
	else /* pPack == NULL */
	{
		/* get last pack from buffer */
		iRet = spIPCPackBuffpayloadGet( pBuf, pLen );
	}

	/* return actual size */
	return iRet;
}


int spIPCPackBuffOUT( struct ipcpacket *pBuf )
{
	int iRet = -1;
	
	if( pBuf )
	{
		iRet = spIPCPackBuffGET( pBuf );
	}
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );

	return iRet;
}


int spIPCgetMgrID( tSRVMGRTYP type )
{
	int iRet = -1;
	
	if( POWERMGR == type )
		iRet = 3456;
	
	return iRet;
}


int spIPCgetMgrPort( tSRVMGRTYP type )
{
	int iRet = -1;

	if( POWERMGR == type )
		iRet = INITIPCHOSTPORTNUM+1;
	
	return iRet;
}


int spIPCgetClientID( void )
{
	int iRet = -1;
	
	iRet = 1234;
	
	return iRet;
}


int spIPCgetClientPort( void )
{
	int iRet = -1;
	
	iRet = INITIPCCLIENTPORTNUM;
	
	return iRet;
}


int spIPCsend( char *pData, int iLen, tSRVMGRTYP type )
{
	int iRet = -1;
#if 0	
	int tarID = -1;
	int tarPort = -1;
	int srcID = -1;
	int srcPort = -1;

	/* parse target user ID by manager type */
	tarID = spIPCgetMgrID( type );
	/* parse target port number by manager type */
	tarPort = spIPCgetMgrPort( type );
	/* get/create source user ID for current user */
	srcID = spIPCgetClientID();
	/* get/create source port number for current user */
	srcPort = spIPCgetClientPort();
	
	iRet = spIPCsendEx( pData, iLen, srcID, srcPort, tarID, tarPort );	
#endif
	return iRet;
}


/*
int spIPCrecv( char *pData, int *piLen, int iSrcID, int iTarID )
*/
int spIPCrecv( char *pData, int *piLen, tSRVMGRTYP type  )
{
	int iRet = -1;
/*
	iRet = spIPCrecvEx( pData, piLen, 0, 0 );
*/
	return iRet;
}


int spIPCrequest( char *pData, int *piLen, tSRVMGRTYP type )
{
	int iRet = -1;
	
	iRet = spIPCrequestTCP( pData, piLen, type );

	return iRet;
}


int spIPCPackResponse( struct ipcpacket *pBuf, char *pData, int iLen )
{
	int iRet = -1;
	
	iRet = spIPCPackResponseTCP( pBuf, pData, iLen );

	return iRet;
}


int spIPCsetCallback( PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	if( pCB )
	{
		ipcCallback = pCB;
		spIPCsetCallbackTCP( pCB );
		iRet = 0;
	}

	return iRet;
}


int spIPCinitServer( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	serverType = servertype;
	iRet = spIPCsetCallback( pCB );

	iRet = spIPCinitServerTCP( servertype, pCB );


	return iRet;
}


int spIPCInit( void )
{
	spIPCInitTCP();
	return 0;
}


int spIPCDeinit( void )
{
	spIPCDeinitTCP();
	return 0;
}

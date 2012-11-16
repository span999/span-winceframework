/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "toolhelps.h"
#include "ipcpacket.h"
#include "sysIPCSRV.h"
#include "sysIPCSRVbuffer.h"

#if 0
#define	__USE_TCP__
#endif


#ifdef __USE_TCP__
#include "sysIPCSRVtcp.h"
#else
#include "sysIPCSRVshm.h"
#endif



/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
/* #define	DBGFSET		(dDBG|dINFO|dERR) */
#define	DBGFSET		(dINFO|dERR)
#define	dF(x)		(DBGFSET&x)




static tSRVMGRTYP serverType = 0;
static PFNIPCCALLBACK ipcCallback = NULL;




int spIPCpayloadGet( struct ipcpacket *pPack, char *pBuf, int *pLen )
{
	int iRet = -1;
	
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
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
	
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	if( pBuf )
	{
		iRet = spIPCPackBuffGET( pBuf );
	}
	else
		spMSG( dF(dERR), "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );

	return iRet;
}


int spIPCgetMgrID( tSRVMGRTYP type )
{
	int iRet = -1;
	
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	if( POWERMGR == type )
		iRet = 3456;
	
	return iRet;
}


int spIPCgetMgrPort( tSRVMGRTYP type )
{
	int iRet = -1;

	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	if( POWERMGR == type )
		iRet = INITIPCHOSTPORTNUM+1;
	
	return iRet;
}


int spIPCgetClientID( void )
{
	int iRet = -1;
	
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	iRet = 1234;
	
	return iRet;
}


int spIPCgetClientPort( void )
{
	int iRet = -1;
	
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	iRet = INITIPCCLIENTPORTNUM;
	
	return iRet;
}


int spIPCsend( char *pData, int iLen, tSRVMGRTYP type )
{
	int iRet = -1;

	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
#ifdef __USE_TCP__

#else

#endif

	return iRet;
}


/*
int spIPCrecv( char *pData, int *piLen, int iSrcID, int iTarID )
*/
int spIPCrecv( char *pData, int *piLen, tSRVMGRTYP type  )
{
	int iRet = -1;
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
/*
	iRet = spIPCrecvEx( pData, piLen, 0, 0 );
*/
#ifdef __USE_TCP__

#else

#endif

	return iRet;
}


int spIPCrequest( char *pData, int *piLen, tSRVMGRTYP type )
{
	int iRet = -1;

	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
#ifdef __USE_TCP__	
	iRet = spIPCrequestTCP( pData, piLen, type );
#else
	iRet = spIPCrequestSHM( pData, piLen, type );
#endif

	return iRet;
}


int spIPCPackResponse( struct ipcpacket *pBuf, char *pData, int iLen )
{
	int iRet = -1;
	
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
#ifdef __USE_TCP__
	iRet = spIPCPackResponseTCP( pBuf, pData, iLen );
#else
	iRet = spIPCPackResponseSHM( pBuf, pData, iLen );
#endif

	return iRet;
}


int spIPCsetCallback( PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	if( pCB )
	{
		ipcCallback = pCB;
#ifdef __USE_TCP__
		spIPCsetCallbackTCP( pCB );
#else
		spIPCsetCallbackSHM( pCB );
#endif
		iRet = 0;
	}

	return iRet;
}


int spIPCinitServer( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	serverType = servertype;
	iRet = spIPCsetCallback( pCB );
#ifdef __USE_TCP__
	iRet = spIPCinitServerTCP( servertype, pCB );
#else
	iRet = spIPCinitServerSHM( servertype, pCB );
#endif


	return iRet;
}


int spIPCInit( void )
{
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
#ifdef __USE_TCP__
	spIPCInitTCP();
#else
	spIPCInitSHM();
#endif
	return 0;
}


int spIPCDeinit( void )
{
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
#ifdef __USE_TCP__
	spIPCDeinitTCP();
#else
	spIPCDeinitSHM();
#endif
	return 0;
}

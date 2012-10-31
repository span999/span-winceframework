/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ipcpacket.h"
#include "tcp-server.h"
#include "tcp-client.h"
#include "sysIPCSRV.h"




static int spIPCsendEx( char *pData, int iLen, int iSrcID, int iTarID )
{
	int iRet = -1;

	return iRet;
}


static int spIPCrecvEx( char *pData, int *piLen, int iSrcID, int iTarID )
{
	int iRet = -1;

	return iRet;
}


int spIPCsend( char *pData, int iLen )
{
	int iRet = -1;

	iRet = spIPCsendEx( pData, iLen, 0, 0 );	

	return iRet;
}


int spIPCrecv( char *pData, int *piLen, int iSrcID, int iTarID )
{
	int iRet = -1;

	iRet = spIPCrecvEx( pData, piLen, 0, 0 );

	return iRet;
}

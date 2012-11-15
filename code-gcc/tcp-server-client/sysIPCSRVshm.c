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
#include "sysIPCSRVbuffer.h"
#include "spRingBuf.h"



static tSRVMGRTYP serverTypeSHM = 0;
static PFNIPCCALLBACK ipcCallbackSHM = NULL;
static pthread_t shmserv_thread_id;




static pthread_mutex_t mutex;
static int mutexINITED = 0;




static void *shmServer( void *argv )
{
	unsigned int *piRet = NULL;


	/* wait for SEM/SHM */
	
	
	
	
	spMxL( &mutex, &mutexINITED );

	/* get data to ring buffer */

	/* callback to user/server */
	if( ipcCallbackSHM )
		ipcCallbackSHM();
		
	spMxU( &mutex, &mutexINITED );


	return piRet;
}


int spIPCrequestSHM( char *pData, int *piLen, tSRVMGRTYP type )
{
	int iRet = -1;
#if 0
	int socket = 0;
	int tarID = -1;
	int tarPort = -1;
	int srcID = -1;
	int srcPort = -1;
	struct ipcpacket recvPack;
	

	
	/* parse target user ID by manager type */
	tarID = spIPCgetMgrID( type );
	/* parse target port number by manager type */
	tarPort = spIPCgetMgrPort( type );
	/* get/create source user ID for current user */
	srcID = spIPCgetClientID();
	/* get/create source port number for current user */
	srcPort = spIPCgetClientPort();

	/* setup recv port before send */
	socket = tcpClient( srcPort );
	
	 usleep( 100*1000 ); /* work around for issue "Address already in use" */
	
	/* send request */
	iRet = spIPCsendEx( pData, *piLen, srcID, srcPort, tarID, tarPort );

	/* wait response */
	/* get feedback */
	iRet = spIPCrecvWait( socket, srcPort, &recvPack );
	if( 0 == iRet && (*piLen >= recvPack.payloadnum ) )
	{
		memcpy( pData, recvPack.payload, recvPack.payloadnum );
		*piLen = recvPack.payloadnum;
	}

	close( socket );
#endif	
	return iRet;
}


int spIPCPackResponseSHM( struct ipcpacket *pBuf, char *pData, int iLen )
{
	int iRet = -1;
	struct ipcpacket ipcPak;


	/* init packet */
	spIPCPacketInit( &ipcPak );
	
	if( pBuf )
	{
		memcpy( &ipcPak, pBuf, sizeof(struct ipcpacket) ); 
	}
	
	spQMSG( "spIPCPackResponse: !!! \n" );
	
	if( pBuf && pData && (iLen<=255) )
	{
		/* setup packet */
	#if 0	
		/* ipcPak.userID = iSrcID; */
		memcpy( ipcPak.srcip, pBuf->tarip, 10 );
		ipcPak.srcport = pBuf->tarport;
		memcpy( ipcPak.tarip, pBuf->srcip, 10 );
		ipcPak.tarport = pBuf->srcport;
		/* ipcPak.serialnum = 0; */
		ipcPak.packetnum = 0;
	#endif	
		/* copy payload data */
		ipcPak.payloadnum = iLen;
		memcpy( ipcPak.payload, pData, iLen );
	}
	else
		spQMSG( "spIPCPackResponse: error !!! \n" );
		
	/* add CRC sign */
	spIPCPacketCRCsign( &ipcPak );
	spIPCPacketDump( &ipcPak );
	
	/* send out with tcp socket */
	#if 0
	iRet = tcpSockSend( ipcPak.tarip, ipcPak.tarport, (char *)&ipcPak, sizeof(struct ipcpacket) );
	#endif

	return iRet;
}


int spIPCsetCallbackSHM( PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	if( pCB )
	{
		ipcCallbackSHM = pCB;
		iRet = 0;
	}

	return iRet;
}


int spIPCinitServerSHM( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	serverTypeSHM = servertype;
	iRet = spIPCsetCallbackSHM( pCB );
	pthread_create( &shmserv_thread_id, NULL, &shmServer, NULL );

	return iRet;
}


int spIPCInitSHM( void )
{
	spMxI( &mutex, &mutexINITED );
	return 0;
}


int spIPCDeinitSHM( void )
{
	spMxD( &mutex, &mutexINITED );
	return 0;
}

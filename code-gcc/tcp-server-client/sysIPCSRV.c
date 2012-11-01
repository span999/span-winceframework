/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "toolhelps.h"
#include "ipcpacket.h"
/*
#include "tcp-server.h"
#include "tcp-client.h"
*/
#include "sysIPCSRV.h"


static PFNIPCCALLBACK ipcCallback = NULL;
static pthread_t tcpserv_thread_id;




static int tcpSockgetData( int newSock )
{
	int iRet = -1;
	int ndo = 0;
	char buffer[256];



	SETZERO( buffer, 256 );
#if 0
	ndo = read( newSock, buffer, 255 );
#else
	ndo = recv( newSock, buffer, 255, 0 );
#endif
	if( ndo < 0 )
		spERR( "read fail return !!" );

	/*
	printf( "[%s]\r\n", buffer ); fflush( stdout );
	*/
	spQMSG( "CLIENT:%s\r\n", buffer );
	
#if 0
	ndo = write( newSock, "Got your message", 16 );
#else
	ndo = send( newSock, "Got your message", 16, 0 );
#endif
	if( ndo < 0 )
		spERR( "write fail return !!" );

	iRet = ndo;
	return iRet;
}


static int tcpSockListenWait( int iSocket )
{
	int iRet = 0;
	int clilen = 0;
	int ServSock = 0;
	int newSock = 0;
	int ndo = 0;
	struct sockaddr_in cli_addr;
	/*
	char buffer[256];
	*/
	
	ServSock = iSocket;
	clilen = sizeof( cli_addr );

waitLoop:
	/*
	printf( "[wait client data~]\r\n" ); fflush( stdout );
	*/
	spQMSG( "[wait client data~][0x%08x]\r\n", tcpserv_thread_id );
	newSock = accept( ServSock, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen );

	if( newSock < 0 )
		spERR( "\r\n  accept fail return !!" );

	/*
	printf( "[got client data~]\r\n" ); fflush( stdout );
	*/
	spQMSG( "[got client data~][0x%08x]\r\n", tcpserv_thread_id );
	
	ndo = tcpSockgetData( newSock ); 
	if( ipcCallback )
		ipcCallback();
	
	goto waitLoop;
	
	
	return iRet;
}


static void *tcpServer( void *argv )
{
	unsigned int *piRet = NULL;
	int ServSock = 0;
	int portno = 0;
	struct sockaddr_in serv_addr;

	/* create socket */
	ServSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( ServSock < 0 )
		spERR( "\r\n  socket() fail return !!" );
	
	/*
	portno = SERVERPORTNO;
	*/
	portno = INITIPCHOSTPORTNUM;

	SETZERO( &serv_addr, sizeof(serv_addr) );
	/*
	 * prepare socket address data
	 * 
	 * reference link, 
	 * http://www.umiacs.com/sockaddr_inman.html
	 */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	/*
	 * bind socket with specified address data
	 */
	if( bind( ServSock, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) < 0 )
		spERR( "\r\n  bind fail return !!" );

	/*
	 * listen to the socket with queue deepth in 5
	 */
	if( listen( ServSock, 5 ) < 0 )
		spERR( "\r\n  listen fail return !!" );

	tcpSockListenWait( ServSock );

	return piRet;
}



static int spIPCsendEx( char *pData, int iLen, int iSrcID, int iSrcPort, int iTarID, int iTarPort )
{
	int iRet = -1;
	struct ipcpacket ipcPak;
	
	spIPCPacketInit( &ipcPak );
	
	ipcPak.userID = iSrcID;
	memcpy( ipcPak.srcip, "127.0.0.1", 10 );
	ipcPak.srcport = iSrcPort;
	memcpy( ipcPak.tarip, "127.0.0.1", 10 );
	ipcPak.tarport = iTarPort;
	ipcPak.serialnum = 0;
	ipcPak.packetnum = 0;
	ipcPak.payloadnum = iLen;
	memcpy( ipcPak.payload, pData, iLen );

	iRet = 0;
	return iRet;
}


static int spIPCrecvEx( char *pData, int *piLen, int iSrcID, int iTarID )
{
	int iRet = -1;

	return iRet;
}


static int spIPCgetMgrID( tSRVMGRTYP type )
{
	int iRet = -1;
	
	if( POWERMGR == type )
		iRet = 3456;
	
	return iRet;
}


static int spIPCgetMgrPort( tSRVMGRTYP type )
{
	int iRet = -1;

	if( POWERMGR == type )
		iRet = INITIPCHOSTPORTNUM+1;
	
	return iRet;
}


static int spIPCgetClientID( void )
{
	int iRet = -1;
	
	iRet = 1234;
	
	return iRet;
}


static int spIPCgetClientPort( void )
{
	int iRet = -1;
	
	iRet = INITIPCCLIENTPORTNUM;
	
	return iRet;
}


int spIPCsend( char *pData, int iLen, tSRVMGRTYP type )
{
	int iRet = -1;
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
	
	iRet = spIPCsendEx( pData, iLen, tarID, tarPort, srcID, srcPort );	

	return iRet;
}


int spIPCrecv( char *pData, int *piLen, int iSrcID, int iTarID )
{
	int iRet = -1;

	iRet = spIPCrecvEx( pData, piLen, 0, 0 );

	return iRet;
}


int spIPCsetCallback( PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	if( pCB )
	{
		ipcCallback = pCB;
		iRet = 0;
	}

	return iRet;
}


int spIPCinitServer( PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	iRet = spIPCsetCallback( pCB );
	pthread_create( &tcpserv_thread_id, NULL, &tcpServer, NULL );

	return iRet;
}






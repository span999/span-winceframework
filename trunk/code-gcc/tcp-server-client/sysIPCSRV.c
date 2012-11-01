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


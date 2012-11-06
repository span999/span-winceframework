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
#include "spRingBuf.h"



static int spIPCgetMgrPort( tSRVMGRTYP type );



static tSRVMGRTYP serverType = 0;
static PFNIPCCALLBACK ipcCallback = NULL;
static pthread_t tcpserv_thread_id;



static CircularBuffer cb;
static CircularBuffer *pcb = NULL;
static int RingBufferSize = 16;
static void spIPCPackBuffINIT( void )
{
	if( NULL == pcb )
	{
		cbInit(&cb, RingBufferSize, sizeof(struct ipcpacket));
		pcb = &cb;
	}	
	
	return;
}


static int spIPCPackBuffADD( struct ipcpacket *pBuf )
{
	int iRet = -1;
	
	if( pBuf )
	{
		
		spIPCPackBuffINIT();
		spQMSG( "spIPCPackBuffADD +++\n" );
		cbWrite( pcb, (unsigned char *)pBuf, sizeof(struct ipcpacket) );
		spQMSG( "spIPCPackBuffADD ---\n" );
	}
	
	return iRet;
}


static int spIPCPackBuffGET( struct ipcpacket *pBuf )
{
	int iRet = -1;
	
	if( pBuf )
	{
		/*
		spIPCPackBuffINIT();
		*/
		if( !cbIsEmpty( pcb ) )
		{
			cbRead( pcb, (unsigned char *)pBuf, sizeof(struct ipcpacket) );
			iRet = 0;
		}
/*		
		else
			spQMSG( "spIPCPackBuffGET cbIsEmpty fail !!\n" );
*/
	}
/*	
	if( -1 == iRet )
		spQMSG( "spIPCPackBuffGET fail !!\n" );
*/
	return iRet;
}


static int spIPCPackBuffDUMP( void )
{
	int iRet = -1;
	
	if( pcb )
	{
		struct ipcpacket elem;
		
		spQMSG( "RingBuffer dump:\n");
		/* Remove and print all elements */
		while( !cbIsEmpty( pcb ) ) {
			cbRead( pcb, (unsigned char *)&elem, sizeof(struct ipcpacket) );
			spIPCPacketDump( &elem );
		}
	}
	
	return iRet;
}


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
	else
	{
		if( !cbIsEmpty( pcb ) )
		{
			struct ipcpacket elem;
			struct ipcpacket *pThisPack = NULL;
		
			pThisPack = &elem;
			cbCopy( pcb, (unsigned char *)pThisPack, sizeof(struct ipcpacket) );

			iRet = 0;
			if( pBuf && *pLen >= pThisPack->payloadnum )
			{
				iRet = pThisPack->payloadnum;
				cbRead( pcb, (unsigned char *)pThisPack, sizeof(struct ipcpacket) );
				memcpy( pBuf, pThisPack->payload, pThisPack->payloadnum );
			}
		}
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
/*	
	if( -1 == iRet )
		spQMSG( "spIPCPackBuffOUT fail !!\n" );
*/
	return iRet;
}


static int tcpSockgetData( int newSock )
{
	#define	BUFSIZE	512
	int iRet = -1;
	int ndo = 0;
	char buffer[BUFSIZE];



	SETZERO( buffer, BUFSIZE );
	ndo = recv( newSock, buffer, BUFSIZE, 0 );
	if( ndo < 0 )
		spERR( "read fail return !!" );

	/*
	printf( "[%s]\r\n", buffer ); fflush( stdout );
	*/
	spQMSG( "CLIENT:%d bytes CRC:%s \r\n", ndo, ((0 == spIPCPacketCRCvalid((struct ipcpacket *)buffer))?"ok":"fail") );
	spIPCPacketDump( (struct ipcpacket *)buffer );
	spIPCPackBuffADD( (struct ipcpacket *)buffer );
/*	
	spIPCPackBuffADD( (struct ipcpacket *)buffer );
*/
/* 
	spIPCPackBuffDUMP();
*/
	

#if 0	
	ndo = send( newSock, "Got your message", 16, 0 );
	if( ndo < 0 )
		spERR( "write fail return !!" );
#endif

	iRet = 0;
	return iRet;
}


static int tcpSockListenWait( int iSocket, int iPort )
{
	int iRet = 0;
	int clilen = 0;
	int ServSock = 0;
	int newSock = 0;
	int ndo = 0;
	struct sockaddr_in cli_addr;

	
	ServSock = iSocket;
	clilen = sizeof( cli_addr );

waitLoop:
	spQMSG( "[wait client data~][0x%08x][port:%d]\r\n", tcpserv_thread_id, iPort );
	newSock = accept( ServSock, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen );

	if( newSock < 0 )
		spERR( "\r\n  accept fail return !!" );

	spQMSG( "[got client data~][0x%08x][port:%d]\r\n", tcpserv_thread_id, iPort );
	
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
	portno = spIPCgetMgrPort( serverType );

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

	tcpSockListenWait( ServSock, portno );

	return piRet;
}


int tcpSockSend( char *hostname, int portnum, char *pData, int iSize )
{
	int iRet = (-1);
	int clntSock = 0;
	int ndo = 0;
	struct sockaddr_in serv_addr;
	
	spQMSG( "tcpSockSend: %s:%d 0x%08x %d !!! \n", hostname, portnum, pData, iSize );
	
	SETZERO( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	inet_aton( hostname, &serv_addr.sin_addr.s_addr );
	serv_addr.sin_port = htons( portnum );

	/* create socket */
	clntSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( clntSock < 0 ) 
		spERR( "ERROR!! tcpSockSend: opening socket" );

	/* connect socket to specified address */
	ndo = sizeof( serv_addr );
	if( connect( clntSock, (struct sockaddr *)&serv_addr, ndo ) < 0 ) 
		spERR("ERROR!! tcpSockSend: connecting");

	/* send out data with socket */
	ndo = send( clntSock, pData, iSize, 0 );
	if( ndo < 0 ) 
		spERR( "ERROR!! tcpSockSend: writing to socket" );
	
	iRet = 0;
	close( clntSock );
	
	return iRet;
}


static int spIPCsendEx( char *pData, int iLen, int iSrcID, int iSrcPort, int iTarID, int iTarPort )
{
	int iRet = -1;
	struct ipcpacket ipcPak;
	
	spQMSG( "spIPCsendEx: 0x%08x %d [%d:%d]->[%d:%d]!!! \n", pData, iLen, iSrcID, iSrcPort, iTarID, iTarPort );
	
	/* clean packet */
	spIPCPacketInit( &ipcPak );
	
	/* setup packet */
	ipcPak.userID = iSrcID;
	memcpy( ipcPak.srcip, "127.0.0.1", 10 );
	ipcPak.srcport = iSrcPort;
	memcpy( ipcPak.tarip, "127.0.0.1", 10 );
	ipcPak.tarport = iTarPort;
	ipcPak.serialnum = 0;
	ipcPak.packetnum = 0;
	ipcPak.payloadnum = iLen;
	memcpy( ipcPak.payload, pData, iLen );

	/* add CRC sign */
	spIPCPacketCRCsign( &ipcPak );
	spIPCPacketDump( &ipcPak );
	
	/* send out with tcp socket */
	iRet = tcpSockSend( ipcPak.tarip, ipcPak.tarport, (char *)&ipcPak, sizeof(struct ipcpacket) );

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
	
	iRet = spIPCsendEx( pData, iLen, srcID, srcPort, tarID, tarPort );	

	return iRet;
}


/*
int spIPCrecv( char *pData, int *piLen, int iSrcID, int iTarID )
*/
int spIPCrecv( char *pData, int *piLen, tSRVMGRTYP type  )
{
	int iRet = -1;

	iRet = spIPCrecvEx( pData, piLen, 0, 0 );

	return iRet;
}


int spIPCrequest( char *pData, int *piLen, tSRVMGRTYP type )
{
	int iRet = -1;

	/* send request */
	iRet = spIPCsend( pData, *piLen, type );

	/* wait response */


	/* get feedback */
	iRet = spIPCrecv( pData, piLen, type );


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


int spIPCinitServer( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	serverType = servertype;
	iRet = spIPCsetCallback( pCB );
	pthread_create( &tcpserv_thread_id, NULL, &tcpServer, NULL );

	return iRet;
}






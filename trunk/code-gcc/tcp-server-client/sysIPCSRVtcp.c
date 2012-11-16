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
/*
#include <sys/types.h>
*/
#include <netinet/in.h>
#include "toolhelps.h"
#include "ipcpacket.h"
#include "sysIPCSRV.h"
#include "sysIPCSRVbuffer.h"
#include "spRingBuf.h"



static tSRVMGRTYP serverTypeTCP = NONEMGR;
static PFNIPCCALLBACK ipcCallbackTCP = NULL;
static pthread_t tcpserv_thread_id;




static pthread_mutex_t mutex;
static int mutexINITED = 0;




static int tcpSockgetData( int newSock )
{
	#define	BUFSIZE	512
	int iRet = -1;
	int ndo = 0;
	char buffer[BUFSIZE];



	SETZERO( buffer, BUFSIZE );
	ndo = recv( newSock, buffer, BUFSIZE, 0 );
	if( ndo < 0 )
		spERR( "socket read fail return !!" );

	spQMSG( "%s:%s: %d bytes CRC:%s \r\n", __FILE__, __FUNCTION__, ndo, ((0 == spIPCPacketCRCvalid((struct ipcpacket *)buffer))?"ok":"fail") );
	spIPCPacketDump( (struct ipcpacket *)buffer );
	spIPCPackBuffADD( (struct ipcpacket *)buffer );

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

	spQMSG( "%s:%s: [wait client data~][0x%08x][port:%d]\r\n", __FILE__, __FUNCTION__, tcpserv_thread_id, iPort );
	newSock = accept( ServSock, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen );

	if( newSock < 0 )
		spERR( "\r\n  accept fail return !!" );

	spQMSG( "%s:%s: [got client data~][0x%08x][port:%d]\r\n", __FILE__, __FUNCTION__, tcpserv_thread_id, iPort );
	
	ndo = tcpSockgetData( newSock ); 
	if( ipcCallbackTCP )
		ipcCallbackTCP();
	
	goto waitLoop;
	
	
	return iRet;
}


int tcpSockSend( char *hostname, int portnum, char *pData, int iSize )
{
	int iRet = (-1);
	int clntSock = 0;
	int ndo = 0;
	struct sockaddr_in serv_addr;
	
	spQMSG( "%s:%s: %s:%d 0x%08x %d !!! \n", __FILE__, __FUNCTION__, hostname, portnum, pData, iSize );
	spMxL( &mutex, &mutexINITED );
	
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
	
	spMxU( &mutex, &mutexINITED );
	
	return iRet;
}


static int tcpSockRecv( char *hostname, int portnum, char *buffer, int buflen )
{
	int iRet = (-1);
	int clntSock = 0;
	int ndo = 0;
	struct sockaddr_in serv_addr;

	spMxL( &mutex, &mutexINITED );
	
	SETZERO( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	inet_aton( hostname, &serv_addr.sin_addr.s_addr );
	serv_addr.sin_port = htons( portnum );

	clntSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( clntSock < 0 ) 
		spERR( "ERROR tcpSockRecv opening socket" );

	ndo = sizeof( serv_addr );
	if( connect( clntSock, (struct sockaddr *)&serv_addr, ndo ) < 0 ) 
		spERR("ERROR tcpSockRecv connecting");

	ndo = recv( clntSock, buffer, buflen, 0 );
	if( ndo < 0 ) 
		spERR( "ERROR tcpSockRecv writing to socket" );
	
	iRet = ndo;
	close( clntSock );
	
	spMxU( &mutex, &mutexINITED );
	
	return iRet;
}


int spIPCrecvEx( int newSock, struct ipcpacket *precvPack )
{
	#define	BUFSIZE	512
	int iRet = -1;
	int ndo = 0;
	char buffer[BUFSIZE];


	SETZERO( buffer, BUFSIZE );
	ndo = recv( newSock, buffer, BUFSIZE, 0 );
	if( ndo < 0 )
		spERR( "spIPCrecvEx: read fail return !!\n" );

	iRet = (0 == spIPCPacketCRCvalid((struct ipcpacket *)buffer)?1:0);
	spQMSG( "HOST:%d bytes CRC:%s \r\n", ndo, iRet?"ok":"fail" );
	if( iRet )
	{
		spIPCPacketDump( (struct ipcpacket *)buffer );
		iRet = sizeof( struct ipcpacket );
		memcpy( precvPack, buffer, iRet );
	}

	/*
	spIPCPackBuffADD( (struct ipcpacket *)buffer );
	*/ 

	return iRet;
}


int spIPCsendEx( char *pData, int iLen, int iSrcID, int iSrcPort, int iTarID, int iTarPort )
{
	int iRet = -1;
	struct ipcpacket ipcPak;
	
	spQMSG( "%s:%s: 0x%08x %d [%d:%d]->[%d:%d]!!! \n", __FILE__, __FUNCTION__, pData, iLen, iSrcID, iSrcPort, iTarID, iTarPort );
	
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


int spIPCrecvWait( int iSocket, int iPort, struct ipcpacket *precvPack )
{
	int iRet = 0;
	int clilen = 0;
	int ServSock = 0;
	int newSock = 0;
	int ndo = 0;
	struct sockaddr_in cli_addr;

	
	ServSock = iSocket;
	clilen = sizeof( cli_addr );

	for(;;)
	{
		spQMSG( "[wait receive data~][port:%d]\r\n", iPort );
		newSock = accept( ServSock, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen );

		if( newSock < 0 )
			spERR( "\r\n  accept fail return !!" );

		spQMSG( "[got receive data~][0x%08x][port:%d]\r\n", tcpserv_thread_id, iPort );
	
		ndo = spIPCrecvEx( newSock, precvPack ); 
		if( 0 < ndo )
			break;
	}
	
	close( newSock );
	return iRet;
}


int tcpClient( int port )
{
	int iRet = 0;
	int ServSock = 0;
	int portno = 0;
	struct sockaddr_in serv_addr;

	spMxL( &mutex, &mutexINITED );
	
	/* create socket */
	ServSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( ServSock < 0 )
		spERR( "\r\n  tcpClient: socket() fail return !!" );
	
	portno = port;

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
		spERR( "\r\n  tcpClient: bind fail return !!" );

	/*
	 * listen to the socket with queue deepth in 5
	 */
	if( listen( ServSock, 5 ) < 0 )
		spERR( "\r\n  tcpClient: listen fail return !!" );

	spMxU( &mutex, &mutexINITED );

	iRet = ServSock;
	return iRet;
}


static void *tcpServer( void *argv )
{
	unsigned int *piRet = NULL;
	int ServSock = 0;
	int portno = 0;
	struct sockaddr_in serv_addr;

	spMxL( &mutex, &mutexINITED );

	/* create socket */
	ServSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( ServSock < 0 )
		spERR( "\r\n  tcpServer: socket() fail return !!" );
	
	if(1) /* work around for issue "Address already in use" */
	{
		/* What you need to do is add the following two line to your code */
		unsigned value = 1;
		setsockopt(ServSock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
	}
	
	/*
	portno = SERVERPORTNO;
	*/
	portno = spIPCgetMgrPort( serverTypeTCP );

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
		spERR( "\r\n  tcpServer: bind fail return !!" );

	/*
	 * listen to the socket with queue deepth in 5
	 */
	if( listen( ServSock, 5 ) < 0 )
		spERR( "\r\n  tcpServer: listen fail return !!" );

	spMxU( &mutex, &mutexINITED );

	tcpSockListenWait( ServSock, portno );

	return piRet;
}


int spIPCrequestTCP( char *pData, int *piLen, tSRVMGRTYP type )
{
	int iRet = -1;
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
	
	return iRet;
}

int spIPCPackResponseTCP( struct ipcpacket *pBuf, char *pData, int iLen )
{
	int iRet = -1;
	struct ipcpacket ipcPak;


	/* init packet */
	spIPCPacketInit( &ipcPak );
	
	if( pBuf )
	{
		memcpy( &ipcPak, pBuf, sizeof(struct ipcpacket) ); 
	}
	
	spQMSG( "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	
	if( pBuf && pData && (iLen<=255) )
	{
		/* setup packet */
		/* ipcPak.userID = iSrcID; */
		memcpy( ipcPak.srcip, pBuf->tarip, 10 );
		ipcPak.srcport = pBuf->tarport;
		memcpy( ipcPak.tarip, pBuf->srcip, 10 );
		ipcPak.tarport = pBuf->srcport;
		/* ipcPak.serialnum = 0; */
		ipcPak.packetnum = 0;
		
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
	iRet = tcpSockSend( ipcPak.tarip, ipcPak.tarport, (char *)&ipcPak, sizeof(struct ipcpacket) );

	return iRet;
}


int spIPCsetCallbackTCP( PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	if( pCB )
	{
		ipcCallbackTCP = pCB;
		iRet = 0;
	}

	return iRet;
}


int spIPCinitServerTCP( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	serverTypeTCP = servertype;
	iRet = spIPCsetCallbackTCP( pCB );
	pthread_create( &tcpserv_thread_id, NULL, &tcpServer, NULL );

	return iRet;
}


int spIPCInitTCP( void )
{
	spMxI( &mutex, &mutexINITED );
	return 0;
}


int spIPCDeinitTCP( void )
{
	spMxD( &mutex, &mutexINITED );
	return 0;
}

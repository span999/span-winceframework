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
#include "sysIPCSRV.h"
#include "sysIPCSRVtcp.h"
#include "spRingBuf.h"


#if 0
int spIPCgetMgrPort( tSRVMGRTYP type );
#endif






static tSRVMGRTYP serverType = 0;
static PFNIPCCALLBACK ipcCallback = NULL;
#if 0
static pthread_t tcpserv_thread_id;
#endif

#if 0
static pthread_mutex_t mutex;
static int mutexON = 0;

static int mutex_INIT( void )
{
	int iRet = -1;
	
	if( !mutexON )
	{
		if( pthread_mutex_init( &mutex, NULL ) != 0 )
		{
			spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
		}
		else
		{
			mutexON = 1;
			iRet = 0;
		}
	}
	else
		iRet = 0;
	
	return iRet;
}


static int mutex_DESTROY( void )
{
	int iRet = -1;

	if( mutexON )
	{
		pthread_mutex_destroy( &mutex );
		spQMSG( "%s:%s: done !!\n", __FILE__, __FUNCTION__ );
		mutexON = 0;
		iRet = 0;
	}
	
	return iRet;
}


static int mutex_LOCK( void )
{
	int iRet = -1;
	
	mutex_INIT();
	if( mutexON )
		pthread_mutex_lock( &mutex );
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


static int mutex_UNLOCK( void )
{
	int iRet = -1;
	
	mutex_INIT();
	if( mutexON )
		pthread_mutex_unlock( &mutex );
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}
#endif



static CircularBuffer cb;
static CircularBuffer *pcb = NULL;
static int RingBufferSize = 1024;
static void spIPCPackBuffINIT( void )
{
	if( NULL == pcb )
	{
		cbInit(&cb, RingBufferSize, sizeof(struct ipcpacket));
		pcb = &cb;
	}

	if( NULL == pcb )
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
	
	return;
}


int spIPCPackBuffADD( struct ipcpacket *pBuf )
{
	int iRet = -1;
	
	if( pBuf )
	{
		
		spIPCPackBuffINIT();
		spQMSG( "%s:%s: +++\n", __FILE__, __FUNCTION__ );
		cbWrite( pcb, (unsigned char *)pBuf, sizeof(struct ipcpacket) );
		spQMSG( "%s:%s: ---\n", __FILE__, __FUNCTION__ );
	}
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int spIPCPackBuffGET( struct ipcpacket *pBuf )
{
	int iRet = -1;
	
	if( pBuf )
	{
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
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );

	return iRet;
}


int spIPCPackBuffDUMP( void )
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
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );

	return iRet;
}


#if 0
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
#endif

#if 0
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
	if( ipcCallback )
		ipcCallback();
	
	goto waitLoop;
	
	
	return iRet;
}
#endif

#if 0
static void *tcpServer( void *argv )
{
	unsigned int *piRet = NULL;
	int ServSock = 0;
	int portno = 0;
	struct sockaddr_in serv_addr;

	mutex_LOCK();

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
		spERR( "\r\n  tcpServer: bind fail return !!" );

	/*
	 * listen to the socket with queue deepth in 5
	 */
	if( listen( ServSock, 5 ) < 0 )
		spERR( "\r\n  tcpServer: listen fail return !!" );

	mutex_UNLOCK();

	tcpSockListenWait( ServSock, portno );

	return piRet;
}
#endif


#if 0
int tcpSockSend( char *hostname, int portnum, char *pData, int iSize )
{
	int iRet = (-1);
	int clntSock = 0;
	int ndo = 0;
	struct sockaddr_in serv_addr;
	
	spQMSG( "%s:%s: %s:%d 0x%08x %d !!! \n", __FILE__, __FUNCTION__, hostname, portnum, pData, iSize );
	mutex_LOCK();
	
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
	
	mutex_UNLOCK();
	
	return iRet;
}
#endif

static int spIPCsendEx( char *pData, int iLen, int iSrcID, int iSrcPort, int iTarID, int iTarPort )
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

#if 0
static int tcpSockRecv( char *hostname, int portnum, char *buffer, int buflen )
{
	int iRet = (-1);
	int clntSock = 0;
	int ndo = 0;
	struct sockaddr_in serv_addr;

	mutex_LOCK();
	
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
	
	mutex_UNLOCK();
	
	return iRet;
}
#endif

#if 0
static int tcpClient( int port )
{
	int iRet = 0;
	int ServSock = 0;
	int portno = 0;
	struct sockaddr_in serv_addr;

	mutex_LOCK();
	
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

	mutex_UNLOCK();

	iRet = ServSock;
	return iRet;
}
#endif

#if 0
static int spIPCrecvEx( int newSock, struct ipcpacket *precvPack )
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
#endif

#if 0
static int spIPCrecvWait( int iSocket, int iPort, struct ipcpacket *precvPack )
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
#endif

static int spIPCgetMgrID( tSRVMGRTYP type )
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
/*
	iRet = spIPCrecvEx( pData, piLen, 0, 0 );
*/
	return iRet;
}


int spIPCrequest( char *pData, int *piLen, tSRVMGRTYP type )
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


int spIPCPackResponse( struct ipcpacket *pBuf, char *pData, int iLen )
{
	int iRet = -1;
	
#if 0
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
#else
	iRet = spIPCPackResponseTCP( pBuf, pData, iLen );
#endif

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
#if 0	
	pthread_create( &tcpserv_thread_id, NULL, &tcpServer, NULL );
#else
	iRet = spIPCinitServerTCP( servertype, pCB );
#endif

	return iRet;
}






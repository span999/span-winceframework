/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "toolhelps.h"

/*
int ServSock = 0;
*/

#define SERVERPORTNO 77777


static int tcpSockListenWait( int iSocket )
{
	int iRet = 0;
	int clilen = 0;
	int ServSock = 0;
	int newSock = 0;
	int ndo = 0;
	struct sockaddr_in cli_addr;
	char buffer[256];
	
	ServSock = iSocket;
	clilen = sizeof( cli_addr );

waitLoop:
	/*
	printf( "[wait client data~]\r\n" ); fflush( stdout );
	*/
	spQMSG( "[wait client data~]\r\n" );
	newSock = accept( ServSock, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen );

	if( newSock < 0 )
		spERR( "\r\n  accept fail return !!" );

	/*
	printf( "[got client data~]\r\n" ); fflush( stdout );
	*/
	spQMSG( "[got client data~]\r\n" );
	
	
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

	goto waitLoop;
	
	
	return iRet;
}


int tcpServer( int iParam )
{
	int iRet = 0;
	int ServSock = 0;
	int portno = 0;
	struct sockaddr_in serv_addr;

	/* create socket */
	ServSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( ServSock < 0 )
		spERR( "\r\n  socket() fail return !!" );
	
	portno = SERVERPORTNO;

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

	return iRet;
}












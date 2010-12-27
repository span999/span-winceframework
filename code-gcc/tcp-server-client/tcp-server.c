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


/*
int ServSock = 0;
*/

#define SERVERPORTNO 77777

#define SPMSG(X) printf("->X\r\n"); fflush(stdout);


void myerr( char *msg );




void myerr( char *msg )
{
	perror( msg );
	SPMSG(123)
	exit( 1 );
}


int tcpServer( int iPara )
{
	int iRet = 0;
	int ServSock = 0;
	int clilen = 0;
	int newSock = 0;
	int portno = 0;
	int ndo = 0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	char buffer[256];


	ServSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( ServSock < 0 )
		myerr( "\r\n  socket() fail return !!" );
	
	portno = SERVERPORTNO;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if( bind( ServSock, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) < 0 )
		myerr( "\r\n  bind fail return !!" );

	
	listen( ServSock, 5 );

	clilen = sizeof( cli_addr );

waitLoop:
	/*
	SPMSG(wait client data~)
	*/
	printf( "[wait client data~]\r\n" ); fflush( stdout );
	newSock = accept( ServSock, (struct sockaddr *)&cli_addr, &clilen );

	if( newSock < 0 )
		myerr( "\r\naccept fail return !!" );

	printf( "" );
	/*
	SPMSG(got client data~)
	*/
	printf( "[got client data~]\r\n" ); fflush( stdout );
	bzero( buffer, 256 );

	ndo = read( newSock, buffer, 255 );
	if( ndo < 0 )
		myerr( "read fail return !!" );

	printf( "[%s]\r\n", buffer ); fflush( stdout );
	ndo = write( newSock, "Got your message", 16 );
	if( ndo < 0 )
		myerr( "write fail return !!" );

	goto waitLoop;

	return iRet;
}












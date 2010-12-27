/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "toolhelps.h"




int main( int argc, char *argv[] )
{
	int clntSock = 0;
	int portno = 0;
	int ndo = 0;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
    
	if( argc < 3 ) 
	{
		fprintf( stderr, "usage %s hostname port\n\r", argv[0] );
		exit(0);
    }

	portno = atoi( argv[2] );

repeatLoop:
	clntSock = socket( AF_INET, SOCK_STREAM, 0 );

	if( clntSock < 0 ) 
		myerr( "ERROR opening socket" );

	server = gethostbyname( argv[1], strlen(argv[1]) );
    	
	if( server == NULL )
	{
		fprintf( stderr, "ERROR, no such host\n\r" );
		exit( 0 );
	}

	SETZERO( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
/*
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
*/
	serv_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	serv_addr.sin_port = htons( portno );


	ndo = sizeof( serv_addr );
	if( connect( clntSock, (struct sockaddr *)&serv_addr, ndo ) < 0 ) 
		myerr("ERROR connecting");

	printf( "Please enter the message: " );
	SETZERO( buffer, 256 );

	fgets( buffer, 255, stdin );
	ndo = write( clntSock, buffer, strlen(buffer) );
	if( ndo < 0 ) 
		myerr( "ERROR writing to socket" );

	SETZERO( buffer, 256 );
	ndo = read( clntSock, buffer, 255 );

	if( ndo < 0 ) 
 		myerr( "ERROR reading from socket" );
	printf("SERV: %s\n",buffer);
	
	close( clntSock );

	goto repeatLoop;
    	
	return 0;
}








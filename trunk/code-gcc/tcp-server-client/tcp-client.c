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

	/*
	server = gethostbyname( (char *)argv[1], (int)strlen(argv[1]) );
	*/
	server = gethostbyname( argv[1] );
    	
	if( server == NULL )
	{
		fprintf( stderr, "ERROR, no such host\n\r" );
		exit( 0 );
	}

	portno = atoi( argv[2] );


	/*
	 * prepare socket address data
	 * 
	 * reference link, 
	 * http://www.umiacs.com/sockaddr_inman.html
	 */
	SETZERO( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
/*
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
*/
#if 0
	serv_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
#else	
	inet_aton("127.0.0.1", &serv_addr.sin_addr.s_addr);
#endif
	serv_addr.sin_port = htons( portno );



repeatLoop:
	spQMSG( "Please enter the message: " );	
	SETZERO( buffer, 256 );
	fgets( buffer, 255, stdin );

	clntSock = socket( AF_INET, SOCK_STREAM, 0 );

	if( clntSock < 0 ) 
		spERR( "ERROR opening socket" );

	ndo = sizeof( serv_addr );
	if( connect( clntSock, (struct sockaddr *)&serv_addr, ndo ) < 0 ) 
		spERR("ERROR connecting");

	ndo = write( clntSock, buffer, strlen(buffer) );
	if( ndo < 0 ) 
		spERR( "ERROR writing to socket" );

	SETZERO( buffer, 256 );
	ndo = read( clntSock, buffer, 255 );

	if( ndo < 0 ) 
 		spERR( "ERROR reading from socket" );
 	else
		spQMSG("SERV: %s\n",buffer);
	
	close( clntSock );

	goto repeatLoop;
    	
	return 0;
}








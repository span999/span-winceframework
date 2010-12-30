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



static int spSocketID[10];
static int spSocketIDIndex = 0;



int spClientSocketOpen( char *hostname, int portnum )
{
	int iRet = (-1);
	int ndo = 0;
	int clntSock = 0;
	struct sockaddr_in serv_addr;
	
	
	SETZERO( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	inet_aton( hostname, &serv_addr.sin_addr.s_addr );
	serv_addr.sin_port = htons( portnum );

	clntSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( clntSock < 0 ) 
		spERR( "ERROR spSocketOpen opening socket" );
		
	ndo = sizeof( serv_addr );
	if( connect( clntSock, (struct sockaddr *)&serv_addr, ndo ) < 0 ) 
		spERR("ERROR spSocketOpen connecting");
	
	iRet = clntSock;
	spSocketID[spSocketIDIndex] = clntSock;
	spSocketIDIndex++;
	
	return iRet;
}


int spClientSocketClose( int spSocketID )
{
	int iRet = (-1);
	
	close( spSocketID );
	spSocketIDIndex--;
	/*
	spSocketID[spSocketIDIndex] = 0;
	*/
	iRet = 0;
	return iRet;
}


int spClientSocketSend( int spSocketID, char *string )
{
	int iRet = (-1);
	int ndo = 0;

	ndo = send( spSocketID, string, strlen(string), 0 );
	if( ndo < 0 ) 
		spERR( "ERROR spClientSocketSend writing to socket" );
	else
		iRet = 0;
	
	return iRet;
}



int spSocketSend( char *hostname, int portnum, char *string )
{
	int iRet = (-1);
	int clntSock = 0;
	int ndo = 0;
	struct sockaddr_in serv_addr;
	
	
	SETZERO( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	inet_aton( hostname, &serv_addr.sin_addr.s_addr );
	serv_addr.sin_port = htons( portnum );

	clntSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( clntSock < 0 ) 
		spERR( "ERROR spSocketSend opening socket" );

	ndo = sizeof( serv_addr );
	if( connect( clntSock, (struct sockaddr *)&serv_addr, ndo ) < 0 ) 
		spERR("ERROR spSocketSend connecting");

	ndo = send( clntSock, string, strlen(string), 0 );
	if( ndo < 0 ) 
		spERR( "ERROR spSocketSend writing to socket" );
	
	close( clntSock );
	
	return iRet;
}


int spSocketRecv( char *hostname, int portnum, char *buffer, int buflen )
{
	int iRet = (-1);
	int clntSock = 0;
	int ndo = 0;
	struct sockaddr_in serv_addr;
	/*
	char buffer[256];
	*/
	
	SETZERO( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	inet_aton( hostname, &serv_addr.sin_addr.s_addr );
	serv_addr.sin_port = htons( portnum );

	clntSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( clntSock < 0 ) 
		spERR( "ERROR spSocketRecv opening socket" );

	ndo = sizeof( serv_addr );
	if( connect( clntSock, (struct sockaddr *)&serv_addr, ndo ) < 0 ) 
		spERR("ERROR spSocketRecv connecting");

	ndo = recv( clntSock, buffer, buflen, 0 );
	if( ndo < 0 ) 
		spERR( "ERROR spSocketRecv writing to socket" );
	
	close( clntSock );
	
	return iRet;
}



int main( int argc, char *argv[] )
{
#if 0
	int clntSock = 0;
	int portno = 0;
	int ndo = 0;
	struct sockaddr_in serv_addr;
	struct hostent *server;
#endif
	char buffer[256];
	int ClientSocketId = 0;
#if 0    
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
#endif


repeatLoop:
	spQMSG( "Please enter the message: " );	
	SETZERO( buffer, 256 );
	fgets( buffer, 255, stdin );
#if 0
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
#endif	

#if 0
	spSocketSend( "127.0.0.1", 77777, buffer );
	SETZERO( buffer, 256 );
	spSocketRecv( "127.0.0.1", 77777, buffer, 255 );
	spQMSG("SERV: %s\n",buffer);
#else
	ClientSocketId = spClientSocketOpen( "127.0.0.1", 77777 );
	spClientSocketSend( ClientSocketId, buffer );
	spClientSocketClose( ClientSocketId );
#endif

	goto repeatLoop;
    	
	return 0;
}








/*
 *
 *
 *
 *
 *
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "tools.h"



#ifdef DEBUGMSG_FILE
FILE *phDebugmsgfile;
#endif

#ifndef MPLAB_PIC24_PORT
void SPPRINTF( char* pString, ... )
{
	va_list ap;
	char buf[512];

#ifdef DEBUGMSG_FILE
	phDebugmsgfile = fopen( DEBUGMSG_FILENAME, "a" );
#endif

	va_start( ap, pString );

	vsprintf( buf, pString, ap );

#ifdef DEBUGMSG_CONSOLE
	printf( buf );
#endif
#ifdef DEBUGMSG_FILE
	fprintf( phDebugmsgfile, buf );
#endif

	va_end( ap );


#ifdef DEBUGMSG_FILE
	fclose( phDebugmsgfile );
#endif


}
#endif	///#ifndef MPLAB_PIC24_PORT

#if 0
static void psrprintf( char* msg )
{
	UARTprintf( msg );
}
#else
///move to header
///#define 	psrprintf		SPPRINTF
#endif


int pointerValid( void* pPointer, char* pcErrorMsg )
{
	int iRet = 0;

	if( NULL == pPointer )
	{
		psrprintf( pcErrorMsg );
		iRet = (-1);
	}

	return iRet;
}


int Clean_pointerlength( void* pPointer, unsigned uLen )
{
	int iRet = (-1);

	if( NULL != pPointer )
	{
		memset( pPointer, 0, uLen );
		iRet = 0;
	}

	return iRet;
}


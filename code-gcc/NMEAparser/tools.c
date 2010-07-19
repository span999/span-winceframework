/*
 *
 *
 *
 *
 *
 *
 */


#include <stdio.h>
#include <stdarg.h>
#include "tools.h"



#ifdef DEBUGMSG_FILE
FILE *phDebugmsgfile;
#endif


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

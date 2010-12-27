/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>


void myerr( char *msg )
{
	perror( msg );
	exit( 1 );
}


void millisleep( int milliseconds )
{
	int iRet = 0;
	iRet = usleep( (unsigned int)(milliseconds * 1000) );
}
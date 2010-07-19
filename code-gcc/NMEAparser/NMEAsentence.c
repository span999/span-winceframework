/*
 *
 *
 *
 *
 *
 *
 */


#include <stdio.h>
///#include <stdarg.h>





int IsNMEAsentenceTag( char* string, char* strTag, int nCount )
{
	int iRet = (-1);

	if( strncmp( string, strTag, nCount ) == 0 )
		iRet = 0;

	return iRet;
}


int IsNMEAsentence( char* str )
{
	///return IsNMEAsentenceTag( str, "$", 1 );
	return IsNMEAsentenceTag( str, "$GP", 3 );
}


int IsNMEAsentenceRMC( char* str )
{
	return IsNMEAsentenceTag( str, "$GPRMC", 6 );
}


int IsNMEAsentenceGGA( char* str )
{
	return IsNMEAsentenceTag( str, "$GPGGA", 6 );
}


int IsNMEAsentenceGSA( char* str )
{
	return IsNMEAsentenceTag( str, "$GPGSA", 6 );
}


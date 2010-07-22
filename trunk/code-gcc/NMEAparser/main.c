/*
 *
 *
 *
 *
 *
 *
 */


#include <stdio.h>
#include "tools.h"
#include "NMEAsentence.h"
#include "NMEAparser.h"





#define GPSLOG_FILE
#define GPSLOG_FILENAME		"GPSLOG.txt"


/* prototype declare */



#ifdef GPSLOG_FILE
FILE *phGPSlogfile;
#endif





void GPSlogfileDump( void )
{
	char buf[512];
	char* pbuf = NULL;
	phGPSlogfile = fopen( GPSLOG_FILENAME, "r" );


	while( !feof(phGPSlogfile) )
	{
		pbuf = fgets( buf, 256, phGPSlogfile );

		/// check if NMEA sentence
		if( 0 == IsNMEAsentence( buf ) )
		{
			///check if sentence we want
			if(
				0 == IsNMEAsentenceRMC( buf )  ||
				0 == IsNMEAsentenceGGA( buf )  ||
				0 == IsNMEAsentenceGSA( buf )
			)
			{
				psrprintf( pbuf );
			}
		}
	}

	fclose( phGPSlogfile );
}


void GPSlogfileParse( void )
{
	char buf[256];
	char* pbuf = NULL;
	int iRet = (-1);
	gps_NMEA_session session_get;


	phGPSlogfile = fopen( GPSLOG_FILENAME, "r" );

	while( !feof(phGPSlogfile) )
	{
		pbuf = fgets( buf, 256, phGPSlogfile );

		if( pbuf )
		{
			if( 0 == NMEAparser( pbuf, &session_get ) )
			{
				iRet = Handle_NMEA_session( &session_get );
			}
			else
			{
				psrprintf( "NMEAparser fail!!\n" );
			}
		}
		else
		{
			psrprintf( "fgets fail!!\n" );
		}

		if( 0 == iRet )
			;
	}

	fclose( phGPSlogfile );
}



int main( void )
{
	int iRet = 0;

	SPPRINTF( "This is a test project %d !!\r\n", iRet );
	///GPSlogfileDump();

	GPSlogfileParse();

	return iRet;
}



/*
 *
 *
 *
 *
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
///#include "tools.h"
///#include "NMEAsentence.h"
///#include "NMEAparser.h"





#define GPSLOG_FILE
#define GPSLOG_FILENAME		"GPSLOG.txt"


#define DEBUGMSG_CONSOLE


/* prototype declare */
#define HUFFMAN_NODE_MAX	256


typedef 	unsigned char 	tBYTE;


typedef struct {
	tBYTE nodeNum;	///0~255
	char nodeSymbol;
	unsigned count;
} tHUFFMAN_NODE;


typedef struct {
	tBYTE nodeTotle;	///0~255
	tHUFFMAN_NODE nodes[HUFFMAN_NODE_MAX];
	tBYTE SortList[HUFFMAN_NODE_MAX];	///SortList[0] content the node number of the most count node.
} tHUFFMAN_TBL;


static tHUFFMAN_TBL HuffManTable;


#ifdef GPSLOG_FILE
FILE *phGPSlogfile;
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


int HuffmanTableInit( tHUFFMAN_TBL *pHTbl )
{
	int iRet = (-1);

	if( pHTbl )
	{
		int iLoop = 0;

		memset( pHTbl, 0, sizeof( tHUFFMAN_TBL ) );

		for(iLoop = 0; iLoop < HUFFMAN_NODE_MAX; iLoop++ )
			pHTbl->nodes[iLoop].nodeNum = iLoop;

		iRet = 0;
	}

	return iRet;
}


int HuffmanTableDump( tHUFFMAN_TBL *pHTbl )
{
	int iRet = (-1);

	if( pHTbl )
	{
		int iLoop = 0;

		SPPRINTF( "Huffman table dump %d !!\n", pHTbl->nodeTotle );
		for( iLoop = 0; iLoop < pHTbl->nodeTotle; iLoop++ )
		{
			if( 0x0a == pHTbl->nodes[iLoop].nodeSymbol )
				SPPRINTF( "[%d][LF/0x%02x]=[%d]\n", pHTbl->nodes[iLoop].nodeNum, pHTbl->nodes[iLoop].nodeSymbol, pHTbl->nodes[iLoop].count );
			else
			if( 0x0d == pHTbl->nodes[iLoop].nodeSymbol )
				SPPRINTF( "[%d][CR/0x%02x]=[%d]\n", pHTbl->nodes[iLoop].nodeNum, pHTbl->nodes[iLoop].nodeSymbol, pHTbl->nodes[iLoop].count );
			else
				SPPRINTF( "[%d][%c/0x%02x]=[%d]\n", pHTbl->nodes[iLoop].nodeNum, pHTbl->nodes[iLoop].nodeSymbol, pHTbl->nodes[iLoop].nodeSymbol, pHTbl->nodes[iLoop].count );
		}

		SPPRINTF( "Sorting list:\n" );
		for( iLoop = 0; iLoop < pHTbl->nodeTotle; iLoop++ )
		{
			SPPRINTF( "[%6d] ", iLoop );
		}
		SPPRINTF( "\n" );
		for( iLoop = 0; iLoop < pHTbl->nodeTotle; iLoop++ )
		{
			SPPRINTF( "[%6d] ", pHTbl->SortList[iLoop] );
		}
		SPPRINTF( "\n" );
		for( iLoop = 0; iLoop < pHTbl->nodeTotle; iLoop++ )
		{
			SPPRINTF( "[%6d] ", pHTbl->nodes[pHTbl->SortList[iLoop]].count );
		}
		SPPRINTF( "\n" );
	}

	return iRet;
}


int HuffmanTableSortingArrange( tHUFFMAN_TBL *pHTbl, tBYTE nodeNum )
{
	int iRet = 0;
	int iLoop = 0;

	for( iLoop = 0; iLoop < pHTbl->nodeTotle; iLoop++ )	///total number needs to sorted
	{
		if( pHTbl->nodes[pHTbl->SortList[iLoop]].count <= pHTbl->nodes[nodeNum].count )
		{
			int iLp = pHTbl->nodeTotle - 2;
			while( iLp >= iLoop )
			{
				pHTbl->SortList[iLp+1] = pHTbl->SortList[iLp];
				iLp--;
			}
			pHTbl->SortList[iLoop] = nodeNum;
			break;
		}
	}

	return iRet;
}


int HuffmanTableSorting( tHUFFMAN_TBL *pHTbl )
{
	int iRet = (-1);

	if( pHTbl )
	{
		int iLoop = 0;
		int iSortCount = 0;

		SPPRINTF( "Huffman table sorting %d !!\n", pHTbl->nodeTotle );
		for( iLoop = 0; iLoop < pHTbl->nodeTotle; iLoop++ )	///total number needs to sorted
		{

			HuffmanTableSortingArrange( pHTbl, pHTbl->nodes[iLoop].nodeNum );

			SPPRINTF( "[%d]-[%d]\n", pHTbl->nodes[iLoop].nodeNum, pHTbl->nodes[pHTbl->nodes[iLoop].nodeNum].count );
///			SPPRINTF( "[%d][%c/0x%02x]=[%d]\n", pHTbl->nodes[iLoop].nodeNum, pHTbl->nodes[iLoop].nodeSymbol, pHTbl->nodes[iLoop].nodeSymbol, pHTbl->nodes[iLoop].count );
		}
	}

	return iRet;
}


int HuffmanTableFoundInNodes( tHUFFMAN_TBL *pHTbl, char *pChar )
{
	int iRet = (-1);

	if( pHTbl && pChar )
	{
		int iLoop = 0;

		for( iLoop = 0; iLoop < pHTbl->nodeTotle; iLoop++ )
		{
			if( *pChar == pHTbl->nodes[iLoop].nodeSymbol )
			{

				iRet = 0;
				break;
			}
		}
	}

	return iRet;
}


int HuffmanTableFoundInNodesAddCount( tHUFFMAN_TBL *pHTbl, char *pChar )
{
	int iRet = (-1);

	if( pHTbl && pChar )
	{
		int iLoop = 0;

		for( iLoop = 0; iLoop < pHTbl->nodeTotle; iLoop++ )
		{
			if( *pChar == pHTbl->nodes[iLoop].nodeSymbol )
			{
				pHTbl->nodes[iLoop].count++;
				iRet = 0;
				break;
			}
		}
	}

	return iRet;
}


int HuffmanTableAddNewNodesSymbol( tHUFFMAN_TBL *pHTbl, char *pChar )
{
	int iRet = (-1);

	if( pHTbl && pChar )
	{
		pHTbl->nodes[pHTbl->nodeTotle].nodeSymbol = *pChar;
		pHTbl->nodes[pHTbl->nodeTotle].count++;
		pHTbl->nodeTotle++;
		iRet = 0;
	}

	return iRet;
}


void GPSlogfileDump( void )
{
	char buf[512];
	char* pbuf = NULL;
	phGPSlogfile = fopen( GPSLOG_FILENAME, "r" );


	while( !feof(phGPSlogfile) )
	{
		pbuf = fgets( buf, 256, phGPSlogfile );

#if 0
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
#else
		SPPRINTF( pbuf );
#endif
	}

	fclose( phGPSlogfile );
}





void GPSlogfileHuffmanAnalysis( void )
{
///	char buf[256];
///	char* pbuf = NULL;
	int iRet = (-1);
	int iGetC = (-1);
///	gps_NMEA_session session_get;


	phGPSlogfile = fopen( GPSLOG_FILENAME, "r" );

	while( !feof(phGPSlogfile) )
	{
		iGetC = fgetc( phGPSlogfile );
		if( EOF != iGetC )
		{
			///TODO:
			if( 0 != HuffmanTableFoundInNodesAddCount( &HuffManTable, &iGetC ) )
				HuffmanTableAddNewNodesSymbol( &HuffManTable, &iGetC );

			SPPRINTF( "%c", iGetC );
		}
		else
		{
			SPPRINTF( "fgetc fail!!\n" );
		}

		if( 0 == iRet )
			;
	}

	fclose( phGPSlogfile );
}



int main( void )
{
	int iRet = 0;

	SPPRINTF( "This is a Huffman text project %d !!\r\n", iRet );

	HuffmanTableInit( &HuffManTable );

	///GPSlogfileDump();

	GPSlogfileHuffmanAnalysis();
	HuffmanTableSorting( &HuffManTable );
	HuffmanTableDump( &HuffManTable );

	return iRet;
}



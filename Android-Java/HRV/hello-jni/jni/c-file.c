
#include "c-common.h"

static FILE *fLog;

int CloseLogfile( void )
{
	int iRet = 0;

	if( !fLog )
		iRet = fclose( fLog );

	return iRet;
}


int WriteLog( unsigned char *pChar, unsigned uSize )
{
	int iRet = uSize;

	if( !fLog )
		fLog = fopen("RAWlog.log", "w+");

	///fwrite( pChar, uSize, 1, fLog );
	fwrite( pChar, sizeof(unsigned char), uSize, fLog );
	fputc( '\n', fLog );

	return iRet;
}

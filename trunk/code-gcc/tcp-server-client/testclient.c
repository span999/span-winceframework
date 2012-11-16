/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toolhelps.h"
#include "sysPowerSRV.h"



int main( int argc, char *argv[] )
{
	char buffer[256];
	int iLoop = 0;
	int iRet = 0;

	sysPowerSRVInit();

	while( iLoop < 200 )
	{
		/*
		spQMSG( "Please enter the message: " );	
		SETZERO( buffer, 256 );
		fgets( buffer, 255, stdin );
		*/

		/*
		getCPUActivatedNum();
		*/
		/*	
		setCPUActivatedNum( 1 );
		*/
		iRet = loopbackTest( iLoop );
		spQMSG( "loopback test num:%d %s \n", iLoop, (iLoop==iRet)?"Ok !":"Fail !!" );	
	
		iLoop++;
	}

	sysPowerSRVDeinit();
    	
	return 0;
}

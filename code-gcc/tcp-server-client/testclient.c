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


repeatLoop:
	spQMSG( "Please enter the message: " );	
	SETZERO( buffer, 256 );
	fgets( buffer, 255, stdin );


	getCPUActivatedNum();
	setCPUActivatedNum( 1 );
	getCPUActivatedNum();
	setCPUActivatedNum( 1 );
	getCPUActivatedNum();
	setCPUActivatedNum( 1 );
	getCPUActivatedNum();
	setCPUActivatedNum( 1 );
	getCPUActivatedNum();
	setCPUActivatedNum( 1 );
	getCPUActivatedNum();
	setCPUActivatedNum( 1 );
	getCPUActivatedNum();
	setCPUActivatedNum( 1 );

	goto repeatLoop;
    	
	return 0;
}

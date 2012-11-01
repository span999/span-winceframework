/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>

#include "ipcpacket.h"
#include "sysIPCSRV.h"


static char PROGRAMNAME[] = "sysPowerMGR";
static pthread_t thread_id;


void *mainPowerMGR( void *argv )
{
	int iLoop = 0;

	while( 1 )
	{
		sleep( 1 );
		if( ++iLoop % 5 == 0 )
			spQMSG( "%s is here ... \n", "mainPowerMGR" );
	}

	spQMSG( "Exit %s !!! \n", PROGRAMNAME );

	return;
}


void IPCCallBack( void )
{

	spQMSG( "IPCCallBack in %s !!! \n", PROGRAMNAME );
	return;
}


int main( int argc, char *argv )
{
	int iRet = -1;
	int iLoop = 0;


	/* setup ipc communication routine */
	/* set callback */
	spIPCinitServer( IPCCallBack );
	
	/* create routine for power manager */
	pthread_create( &thread_id, NULL, &mainPowerMGR, NULL );

	/* wait until loop end */
	while( 1 )
	{
		sleep( 1 );
		if( ++iLoop % 5 == 0 )
			spQMSG( "%s is here ... \n", PROGRAMNAME );
	}

	spQMSG( "Exit %s !!! \n", PROGRAMNAME );
	return iRet;
}

/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ipcpacket.h"
#include "tcp-server.h"
#include "tcp-client.h"

static char PROGRAMNAME[] = "sysIPCMGR";


int main( int argc, char *argv )
{
	int iRet = -1;
	int iLoop = 0;


	/* setup ipc communication routine */
	/* set callback */

	
	/* create routine for ipc manager */
	

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

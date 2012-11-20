/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toolhelps.h"
#include "sysPowerSRV.h"




/*
#define COMMANDNUM		5
char *commands[COMMANDNUM] = { "loopback", "fullpower", "null", "null" ,"null" };
*/
char *commands[] = { \
"loopback", \
"corenumact", \
"corenumset", \
"fullpower", \
"lowpower", \
"null", \
"null" \
};

char *descript[] = { \
"command loopback", \
"get activate core number", \
"set activate core number", \
"fullpower (set all core ON)", \
"lowpower (set 1 core ON only)", \
"null", \
"null" \
};


int main( int argc, char *argv[] )
{
	char buffer[256];
	int iLoop = 0;
	int iRet = 0;

#if 0
	spQMSG( "%s:%s: argc:%d!!!\n", __FILE__, __FUNCTION__, argc );
	for( ; iLoop < argc; iLoop++ )
		spQMSG( "%s:%s: argv[%d]=%s\n", __FILE__, __FUNCTION__, argc, argv[iLoop] );
#endif

	if( 1 == argc )
	{
		spQMSG( "\n%s: use \"%s -h\" for help ~\n\n", argv[0], argv[0] );
		goto _EXIT;
	}
	else
	if( (2 == argc) && (0 == strcmp("-h", argv[1])) )
	{
		spQMSG( "\n" );
		spQMSG( "------------------------------------------------------------------\n" );
		spQMSG( "%s: help for commands in %s.\n", argv[0], argv[0] );
		iLoop = 0;
		while( (0 != strcmp(commands[iLoop],"null")) )
		{
			spQMSG( "%s: use \"%s %s\" for %s testing.\n", argv[0], argv[0], commands[iLoop], descript[iLoop] );
			iLoop++;
		}
		spQMSG( "------------------------------------------------------------------\n" );
		spQMSG( "\n" );
		goto _EXIT;
	}
	else
	{
		int validcmd = 0;
		/* check if command valid */
		iLoop = 0;
		while( (0 != strcmp(commands[iLoop],"null")) && (0 == validcmd) )
		{
			if( 0 == strcmp(commands[iLoop], argv[1]) )
				validcmd = 1;
			iLoop++;
		}

		if( 0 == validcmd )
		{
			spQMSG( "\n%s:%s: unknown command \"%s\" !!\n", __FILE__, __FUNCTION__, argv[1] );
			goto _EXIT;
		}
		else
		{
			spQMSG( "\n%s:%s: start test on command \"%s\" !!\n\n", __FILE__, __FUNCTION__, argv[1] );
		}
	}
	
	

	sysPowerSRVInit();


	if( 0 == strcmp(commands[0], argv[1]) )
	{	/* command index 0, */
		int iloops = 200;

		/* change loop count if user input */
		if( argc > 2 )
			iloops = atoi(argv[2]);

		iLoop = 0;
		while( iLoop++ < iloops )
		{
			iRet = sPSloopbackTest( iLoop );
			spQMSG( "%s loopback test num:%d\n", (iLoop==iRet)?"====>Ok !":"=>Fail !!", iLoop );	
		}
	}
	else
	if( 0 == strcmp(commands[1], argv[1]) )
	{	/* command index 1, */
		iRet = -1;
		iRet = sPSgetCPUActivatedNum();
		spQMSG( "%s get current CPU activate core num:%d\n", (iRet>0)?"====>Ok !":"=>Fail !!", iRet );	
	}
	else
	if( 0 == strcmp(commands[2], argv[1]) )
	{	/* command index 2, */
		int icore;

		iRet = -1;
		if( argc > 2 )
		{
			icore = atoi(argv[2]);
			iRet = sPSsetCPUActivatedNum( icore );
			if( 0 != iRet )
			{
				spQMSG( "%s sPSsetCPUActivatedNum(%d)\n", (iRet>=0)?"====>Ok !":"=>Fail !!", icore );
			}
			else
			{
				iRet = sPSgetCPUActivatedNum();
				spQMSG( "%s set CPU activate core num:%d\n", (iRet==icore)?"====>Ok !":"=>Fail !!", icore );	
			}
		}
		else
		{
			spQMSG( "%s set CPU activate core num: What's your input ?!?!\n", (iRet>0)?"====>Ok !":"=>Fail !!" );	
		}
	}
	else
	if( 0 == strcmp(commands[3], argv[1]) )
	{	/* command index 3, */
		int icore = 4;

		iRet = -1;
		if( 1 )
		{
			iRet = sPSsetCPUActivatedNum( icore );
			if( 0 != iRet )
			{
				spQMSG( "%s sPSsetCPUActivatedNum(%d)\n", (iRet>=0)?"====>Ok !":"=>Fail !!", icore );
			}
			else
			{
				iRet = sPSgetCPUActivatedNum();
				spQMSG( "%s set CPU activate core num:%d\n", (iRet==icore)?"====>Ok !":"=>Fail !!", icore );	
			}
		}
		else
		{
			spQMSG( "%s set CPU activate core num: What's your input ?!?!\n", (iRet>0)?"====>Ok !":"=>Fail !!" );	
		}
	}
	else
	if( 0 == strcmp(commands[4], argv[1]) )
	{	/* command index 4, */
		int icore = 1;

		iRet = -1;
		if( 1 )
		{
			iRet = sPSsetCPUActivatedNum( icore );
			if( 0 != iRet )
			{
				spQMSG( "%s sPSsetCPUActivatedNum(%d)\n", (iRet>=0)?"====>Ok !":"=>Fail !!", icore );
			}
			else
			{
				iRet = sPSgetCPUActivatedNum();
				spQMSG( "%s set CPU activate core num:%d\n", (iRet==icore)?"====>Ok !":"=>Fail !!", icore );	
			}
		}
		else
		{
			spQMSG( "%s set CPU activate core num: What's your input ?!?!\n", (iRet>0)?"====>Ok !":"=>Fail !!" );	
		}
	}



	sysPowerSRVDeinit();

   
_EXIT:	
	return 0;
}

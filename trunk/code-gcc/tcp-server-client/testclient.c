/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "toolhelps.h"
#include "sysPowerSRV.h"





static char verStr[] = "v2.2";

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
"dvfsset", \
"fullspeed", \
"lowspeed", \
"speedset", \
"null", \
"null" \
};

char *descript[] = { \
"command loopback", \
"get activate core number (1,2,3,4)", \
"set activate core number (1,2,3,4)", \
"set fullpower mode (all core ON)", \
"set lowpower mode (1 core ON only)", \
"set DVFS mode (on/off)", \
"set fullspeed mode (1GHz)", \
"set lowspeed mode (400MHz)", \
"set cpu speed (400,800,1000 MHz)", \
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
		spQMSG( "   help for commands in %s %s.\n", argv[0], verStr );
		iLoop = 0;
		while( (0 != strcmp(commands[iLoop],"null")) )
		{
			spQMSG( "   use \"%s %s\" for %s testing.\n", argv[0], commands[iLoop], descript[iLoop] );
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
	else
	if( 0 == strcmp(commands[5], argv[1]) )
	{	/* command index 5, */
		iRet = -1;
		
		if( 0 == strcmp( "on", argv[2]) )
		{
			iRet = sPSsetCPUDVFS( 1 );
		}
		else
		if( 0 == strcmp( "off", argv[2]) )
		{
			iRet = sPSsetCPUDVFS( 0 );
		}
		else
		{
			spQMSG( "%s set CPU DVFS: What's your input ?!?! (on/off)\n", (iRet>0)?"====>Ok !":"=>Fail !!" );	
		}		
	}
	else
	if( 0 == strcmp(commands[6], argv[1]) )
	{	/* command index 6, */
		int iSpeed = 0;
		
		iRet = -1;
		iSpeed = 1000;

		iRet = sPSsetCPUspeed( iSpeed );

		if( -2 == iRet )
		{
			spQMSG( "%s set CPU speed: What's your input ?!?! (200~1000)\n", (iRet>0)?"====>Ok !":"=>Fail !!" );	
		}
	}
	else
	if( 0 == strcmp(commands[7], argv[1]) )
	{	/* command index 7, */
		int iSpeed = 0;
		
		iRet = -1;
		iSpeed = 400;

		iRet = sPSsetCPUspeed( iSpeed );

		if( -2 == iRet )
		{
			spQMSG( "%s set CPU speed: What's your input ?!?! (200~1000)\n", (iRet>0)?"====>Ok !":"=>Fail !!" );	
		}
	}
	else
	if( 0 == strcmp(commands[8], argv[1]) )
	{	/* command index 8, */
		int iSpeed = 0;
		
		iRet = -1;
		iSpeed = atoi(argv[2]);;

		iRet = sPSsetCPUspeed( iSpeed );

		if( -2 == iRet )
		{
			spQMSG( "%s set CPU speed: What's your input ?!?! (200~1000)\n", (iRet>0)?"====>Ok !":"=>Fail !!" );	
		}
	}



	sysPowerSRVDeinit();

   
_EXIT:	
	return 0;
}

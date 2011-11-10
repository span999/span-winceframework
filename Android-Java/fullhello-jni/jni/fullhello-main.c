/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <stdio.h>
#include "libsIO-help.h"
#include "libsIO-common.h"




static void printhelp( void )
{
	printf("%s\r\n",helpstr);
}


static int chartoint( char ch )
{
	int iRet = 0;
	
	switch( ch )
	{
		case '1':
			iRet = 1;
			break;
		case '2':
			iRet = 2;
			break;
		case '3':
			iRet = 3;
			break;
		case '4':
			iRet = 4;
			break;
		case '5':
			iRet = 5;
			break;
		case '6':
			iRet = 6;
			break;
		case '7':
			iRet = 7;
			break;
		case '8':
			iRet = 8;
			break;
		case '9':
			iRet = 9;
			break;
		case 'a':
			iRet = 10;
			break;
		case 'b':
			iRet = 11;
			break;
		case 'c':
			iRet = 12;
			break;
		case 'd':
			iRet = 13;
			break;
		case 'e':
			iRet = 14;
			break;
		case 'f':
			iRet = 15;
			break;
		default:
			iRet = 0;
			break;		
	}

	return iRet;
}

static int strtoint( char *str )
{
	int iRet = -1;
	int iCnt = 0;
	int iNum[2];
	
	if( 0 != str )
	{
		while( *str )
		{
			///printf( "%c", *str );
			iNum[iCnt] = chartoint( *str );
			///printf("[%d]", iNum[iCnt] );
			str++;
			iCnt++;
		}
		
	}

	if( 1 == iCnt )	///1 digit only
	{
		iRet = iNum[0];
	}
	else
	if( 2 == iCnt )	///2 digit
	{
		iRet = (iNum[0]*16) + (iNum[1]);
	}
	
	///printf("{%d}", iRet );
	///printf("\r\n");
	return iRet;
}


int main( int argc, char *argv[] )
{
	int iRet = 0;

	printf("test program start ...%d \r\n", argc);
	
	if( argc > 1 )
	{
		if( 0 == strcmp( argv[1], "help" ) )
			goto _HELP;
		else
		if( 0 == strcmp( argv[1], "cpu" ) )
			libsdumpcupinfo();
		else
		if( 0 == strcmp( argv[1], "mem" ) )
			libsdumpmeminfo();
		else
		if( 0 == strcmp( argv[1], "iic" ) )
			if( 0 == strcmp( argv[2], "search" ) )
				libsi2csearch();
			else
			if( 2 == argc )
				libsi2csearch();
			else
			if( 0 == strcmp( argv[2], "list" ) )
				libsi2clist();
			else
			if( 0 == strcmp( argv[2], "dump" ) )
				libsi2cdump();
			else
			if( 0 == strcmp( argv[2], "get" ) )
			{
				if( 6 > argc )
				{
					goto _HELP;
				}
				else
				{
					printf("Bus=%s, ID=0x%s, reg=0x%s \r\n", argv[3], argv[4], argv[5] );
					strtoint( argv[3] );
					strtoint( argv[4] );
					strtoint( argv[5] );

					libsi2cget( strtoint( argv[3] ), strtoint( argv[4] ), strtoint( argv[5] ) );
					///libsi2cget( 2, 0x1e, 0x00 );
					///libsi2cget( 2, 0x1e, 0x02 );
					///libsi2cget( 2, 0x1e, 0x04 );
					///libsi2cget( 2, 0x1e, 0x06 );
				}
			}
			else
			if( 0 == strcmp( argv[2], "set" ) )
				;///libsi2csearch();
			else
				goto _HELP;
		else
		if( 0 == strcmp( argv[1], "gpio" ) )
			goto _HELP;
		else
			goto _HELP;
	}
	else
	{
		goto _HELP;
	}

	///libsdumpcupinfo();
	///libsdumpmeminfo();

	///libsi2csearch();
	goto _EXIT;
	
_HELP:
	printhelp();
_EXIT:	

	return iRet;
}

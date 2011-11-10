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




void printhelp( void )
{
	printf("%s\r\n",helpstr);
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
				;///libsi2csearch();
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

/*
 *
 *
 *
 *
 *
 *
 *
 *
 */

#ifndef __TOOLS_H__
#define __TOOLS_H__


#define DEBUGMSG_CONSOLE
#define DEBUGMSG_FILE
#define DEBUGMSG_FILENAME	"debugmsg.log"


void SPPRINTF( char* pString, ... );
#define 	psrprintf		SPPRINTF
int pointerValid( void* pPointer, char* pcErrorMsg );
int Clean_pointerlength( void* pPointer, unsigned uLen );


#endif	///#ifndef __TOOLS_H__

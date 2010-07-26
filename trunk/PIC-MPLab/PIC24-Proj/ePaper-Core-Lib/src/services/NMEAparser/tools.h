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



#ifndef MPLAB_PIC24_PORT 
	#define DEBUGMSG_CONSOLE
	#define DEBUGMSG_FILE
	#define DEBUGMSG_FILENAME	"debugmsg.log"
#else
	#include "mTypeDef.h"
	#include "..\Toolbox\Toolbox.h"
#endif	


#ifndef MPLAB_PIC24_PORT
void SPPRINTF( char* pString, ... );
#else
///in Toolbox.h
/// void SPPRINTF( char* pString, ... );
#endif	///#ifndef MPLAB_PIC24_PORT

#define 	psrprintf		SPPRINTF

int pointerValid( void* pPointer, char* pcErrorMsg );
int Clean_pointerlength( void* pPointer, unsigned uLen );


#endif	///#ifndef __TOOLS_H__

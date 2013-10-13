/*


*/
#ifndef __HELPER_H__
#define __HELPER_H__

#include "config.h"



#define 	_MSGHEAD_ 				"SPD:"


#if SP_DEBUG_MESSAGE
/* debug flag sets */
#define	dTIME			0x00004000
#define	dDBG			0x00001000
#define	dBEGN			0x00000100
#define	dDUTY			0x00000080
#define	dEND			0x00000040
#define	dISR			0x00000020
#define	dIST			0x00000010
#define	dDUMP			0x00000004
#define	dINFO			0x00000002
#define	dINIT			0x00000001
#define	dERR			0x00010000

/* #define	DBGFSET			(dDBG|dBEGN|dERR) */
/* #define	DBGFSET			(dDBG|dINFO|dINIT|dERR) */
/* #define	DBGFSET			(dINFO|dDUMP|dERR) */
/* #define	DBGFSET			(dDUMP|dERR) */
/* #define	DBGFSET			(dTIME|dDUTY|dIST|dDUMP|dERR) */
#define	DBGFSET			(dTIME|dBEGN|dDUTY|dIST|dERR)

#define	_dF_(x)			(DBGFSET&x)
#endif	/* #if SP_DEBUG_MESSAGE */

#if SP_DEBUG_MESSAGE
void __SPMSG( unsigned int iFlag, char *msgout, ... );
#define 	_SPMSG 					__SPMSG
#else
void __NODEBUG(void);
#define 	_SPMSG(a,b,...) 		__NODEBUG()
#endif	/* #if SP_DEBUG_MESSAGE */




void MsSleep( int iVal );

#if SP_GETTIME_TICK
long _GetTick( int iDiff );
#define _GetTime()		_GetTick(0)
#define _GetTimeDiff()	_GetTick(1)
#else
long _GetMicroSec( int iDiff );
#define _GetTime()		_GetMicroSec(0)
#define _GetTimeDiff()	_GetMicroSec(1)
#endif


#endif /* #ifndef __HELPER_H__ */





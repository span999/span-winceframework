/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "helper.h"



void MsSleep( int iVal ) {
	usleep(iVal*1000);
}

#if SP_GETTIME_TICK
long _GetTick( int iDiff ) {
	static long priTick = 0;
	long Tick = 0;
	long tmp = 0;
	int iRet = (-1);
    struct timespec now;

	///iRet = clock_gettime(CLOCK_MONOTONIC, &now);
	///iRet = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
	iRet = clock_gettime(CLOCK_REALTIME, &now);
	if( -1 < iRet )
		tmp = Tick = now.tv_nsec;
	else
		_SPMSG(dERR, "%s:%s clock_gettime() failed.\r\n", _MSGHEAD_, __func__);

	if( iDiff > 0 )
	{
		if(Tick > priTick)
			Tick = Tick - priTick;
		else
			Tick = (Tick+1000000000) - priTick;
	}
	priTick = tmp;
    return Tick;
}
#else
long _GetMicroSec( int iDiff ) {
	static long priTick = 0;
	long Tick = 0;
	long tmp = 0;
	int iRet = (-1);
    struct timespec now;

	///iRet = clock_gettime(CLOCK_MONOTONIC, &now);
	///iRet = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
	iRet = clock_gettime(CLOCK_REALTIME, &now);
	if( -1 < iRet )
		tmp = Tick = (now.tv_sec*1000000) + (now.tv_nsec/1000);
	else
		_SPMSG(dERR, "%s:%s clock_gettime() failed.\r\n", _MSGHEAD_, __func__);

	if( iDiff > 0 )
	{
		if(Tick > priTick)
			Tick = Tick - priTick;
		else
			Tick = (Tick+1000000000) - priTick;
	}
	priTick = tmp;
    return Tick;
}
#endif	/* #if SP_GETTIME_TICK */

#if SP_DEBUG_MESSAGE
void __SPMSG( unsigned int iFlag, char *msgout, ... ) {
	int n, size = 64;
	char *p;
	va_list ap;

	if( !_dF_(iFlag) )
		return;

	if((p = malloc(size)) == NULL)
		return;

	va_start(ap, msgout);
	n = vsnprintf(p, size, msgout, ap);
	va_end(ap);

	fprintf( stderr, "%s", p );
	
	free(p);
}
#else
void __NODEBUG(void) {}
#endif

/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "helper.h"


void MsSleep( int iVal ) {
	usleep(iVal*1000);
}


void _SPMSG( unsigned int iFlag, char *msgout, ... ) {
	int n, size = 64;
	char *p;
	va_list ap;

	if( _dF_(iFlag) )
		return;

	if((p = malloc(size)) == NULL)
		return;

	va_start(ap, msgout);
	n = vsnprintf(p, size, msgout, ap);
	va_end(ap);

	fprintf( stderr, "%s", p );
	
	free(p);
}
/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>




char * make_message(const char *fmt, ...)
{
	/* Guess we need no more than 100 bytes. */
	int n, size = 100;
	char *p, *np;
	va_list ap;

	if ((p = malloc (size)) == NULL)
		return NULL;

	while (1) {
		/* Try to print in the allocated space. */
		va_start(ap, fmt);
		n = vsnprintf (p, size, fmt, ap);
		va_end(ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size)
			return p;
		/* Else try again with more space. */
		if (n > -1)    /* glibc 2.1 */
			size = n+1; /* precisely what is needed */
		else           /* glibc 2.0 */
			size *= 2;  /* twice the old size */
		if ((np = realloc (p, size)) == NULL) {
			free(p);
			return NULL;
		} else {
			p = np;
		}
	}
}


void spQMSG( char *msgout, ... )
{
	int n, size = 128;
	char *p;
	va_list ap;

	if((p = malloc(size)) == NULL)
		return;

	va_start(ap, msgout);
	n = vsnprintf(p, size, msgout, ap);
	va_end(ap);

	fprintf( stderr, "%s", p );
	
	free(p);
}


void myerr( char *msg )
{
	perror( msg );
	exit( 1 );
}


void spERR( char *msg )
{
	perror( msg );
	exit( 0 );
}


void millisleep( int milliseconds )
{
#if 0
	int iRet = 0;
	iRet = usleep( (unsigned int)(milliseconds * 1000) );
#else
	usleep( (unsigned int)(milliseconds * 1000) );
#endif
}


long spGetTimetick( void )
{
#if 0	
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec*1000000 + now.tv_nsec/1000;
#endif
#if 1
	long lRet = 0;
	struct timeval tv;
	
	gettimeofday( &tv,NULL );
	#if 0
	while( tv.tv_usec > 1000000 )
	{
		tv.tv_sec++;
		tv.tv_usec -= 1000000;
	}
	#else
	lRet = (tv.tv_sec * 1000000) + tv.tv_usec;
	#endif
	
	return lRet;
#endif 
}

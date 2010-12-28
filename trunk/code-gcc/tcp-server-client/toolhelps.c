/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>




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

	if ((p = malloc (size)) == NULL)
		return NULL;

	va_start(ap, msgout);
	n = vsnprintf (p, size, msgout, ap);
	va_end(ap);

	fprintf( stderr, "%s", p );
	
	free(p);
}

void myerr( char *msg )
{
	perror( msg );
	exit( 1 );
}


void millisleep( int milliseconds )
{
	int iRet = 0;
	iRet = usleep( (unsigned int)(milliseconds * 1000) );
}

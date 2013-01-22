/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
///#include <sys/time.h>


/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
/* #define	DBGFSET		(dDBG|dINFO|dERR) */
#define	DBGFSET		(dINFO|dERR)
#define	dF(x)		(DBGFSET&x)





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


void spMSG( int iFlag, char *msgout, ... )
{
	int n, size = 128;
	char *p;
	va_list ap;

	if( iFlag <= 0 )
		return;

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



/*
 * use code below to initial mutex key.
 * static pthread_mutex_t mutex;
 * static int mutexINITED = 0;
*/
int sp_mutex_INIT( pthread_mutex_t *pKey, int *pInt )
{
	int iRet = -1;

	if( *pInt == 0 )
	{
		iRet = pthread_mutex_init( pKey, NULL );
		if( 0 == iRet )
		{
			spMSG( dF(dDBG), "%s:%s: done !!\n", __FILE__, __FUNCTION__ );
			*pInt = 1;
		}
		else
		{
			perror( "ERROR!! pthread_mutex_init fail:" );
		}
	}
	else
		spMSG( dF(dDBG), "%s:%s: done already !!\n", __FILE__, __FUNCTION__ );

	return iRet;
}


int sp_mutex_DESTROY( pthread_mutex_t *pKey, int *pInt )
{
	int iRet = -1;

	if( *pInt == 1 )
	{
		iRet = pthread_mutex_destroy( pKey );
		if( 0 == iRet )
		{
			spMSG( dF(dDBG), "%s:%s: done !!\n", __FILE__, __FUNCTION__ );
			*pInt = 0;
		}
		else
		{
			perror( "ERROR!! pthread_mutex_destroy fail:" );
		}
	}
	else
		spMSG( dF(dERR), "%s:%s: not valid key !!\n", __FILE__, __FUNCTION__ );

	return iRet;
}


int sp_mutex_LOCK( pthread_mutex_t *pKey, int *pInt )
{
	int iRet = -1;

	sp_mutex_INIT( pKey, pInt );

	if( *pInt == 1 )
	{
		iRet = pthread_mutex_lock( pKey );
		if( 0 == iRet )
		{
			spMSG( dF(dDBG), "%s:%s: done !!\n", __FILE__, __FUNCTION__ );
		}
		else
		{
			perror( "ERROR!! pthread_mutex_lock fail:" );
		}
	}

	return iRet;
}


int sp_mutex_UNLOCK( pthread_mutex_t *pKey, int *pInt )
{
	int iRet = -1;

	sp_mutex_INIT( pKey, pInt );

	if( *pInt == 1 )
	{
		iRet = pthread_mutex_unlock( pKey );
		if( 0 == iRet )
		{
			spMSG( dF(dDBG), "%s:%s: done !!\n", __FILE__, __FUNCTION__ );
		}
		else
		{
			perror( "ERROR!! pthread_mutex_unlock fail:" );
		}
	}

	return iRet;
}


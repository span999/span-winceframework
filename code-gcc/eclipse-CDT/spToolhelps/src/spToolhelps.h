/*
 *
 *
 *
 *
 */

#ifndef __SPTOOLHELPS_H__
#define __SPTOOLHELPS_H__


#include <pthread.h>




#define SETZERO(t, s) memset(t, 0, s)
#define QMSG( X ) fprintf(stderr, X)

void spQMSG( char *msgout, ... );
void spMSG( int iFlag, char *msgout, ... );
void myerr( char *msg );
void spERR( char *msg );
void millisleep( int milliseconds );
long spGetTimetick( void );


/* for mutex function */
/*
static pthread_mutex_t mutex;
static int mutexINITED = 0;
*/

int sp_mutex_INIT( pthread_mutex_t *pKey, int *pInt );
int sp_mutex_DESTROY( pthread_mutex_t *pKey, int *pInt );
int sp_mutex_LOCK( pthread_mutex_t *pKey, int *pInt );
int sp_mutex_UNLOCK( pthread_mutex_t *pKey, int *pInt );


#define		spMxI(x,y)	sp_mutex_INIT(x,y)
#define		spMxD(x,y)	sp_mutex_DESTROY(x,y)
#define		spMxL(x,y)	sp_mutex_LOCK(x,y)
#define		spMxU(x,y)	sp_mutex_UNLOCK(x,y)



#if 0 /* example */
to use spMSG() with flags set as below

/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
/* #define	DBGFSET		(dDBG|dINFO|dERR) */
#define	DBGFSET		(dINFO|dERR)
#define	dF(x)		(DBGFSET&x)

#endif



#endif /* __SPTOOLHELPS_H__ */

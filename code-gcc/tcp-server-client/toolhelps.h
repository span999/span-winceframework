/*
 *
 *
 */
#ifndef __TOOLHELPS_H__
#define __TOOLHELPS_H__


#define SETZERO(t, s) memset(t, 0, s)
#define QMSG( X ) fprintf(stderr, X)

void spQMSG( char *msgout, ... );
void spMSG( int iFlag, char *msgout, ... );
void myerr( char *msg );
void spERR( char *msg );
void millisleep( int milliseconds );
long spGetTimetick( void );


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




#endif /* #ifndef __TOOLHELPS_H__ */

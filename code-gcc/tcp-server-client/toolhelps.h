/*
 *
 *
 */
#ifndef __TOOLHELPS_H__
#define __TOOLHELPS_H__


#define SETZERO(t, s) memset(t, 0, s)
#define QMSG( X ) fprintf(stderr, X)

void spQMSG( char *msgout, ... );
void myerr( char *msg );
void spERR( char *msg );
void millisleep( int milliseconds );
long spGetTimetick( void );



#endif /* #ifndef __TOOLHELPS_H__ */

/*
 *
 *
 */
#ifndef __TOOLHELPS_H__
#define __TOOLHELPS_H__


#define SETZERO(t, s) memset(t, 0, s)
#define QMSG( X ) fprintf(stderr, X)

extern void spQMSG( char *msgout, ... );
extern void myerr( char *msg );
extern void millisleep( int milliseconds );



#endif /* #ifndef __TOOLHELPS_H__ */

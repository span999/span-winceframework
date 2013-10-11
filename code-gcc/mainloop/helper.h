/*


*/
#ifndef __HELPER_H__
#define __HELPER_H__


#define 	_MSGHEAD_ 		"SPD:"

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

/* #define	DBGFSET		(dDBG|dBEGN|dERR) */
/* #define	DBGFSET		(dDBG|dINFO|dINIT|dERR) */
/* #define	DBGFSET			(dINFO|dDUMP|dERR) */
/* #define	DBGFSET			(dDUMP|dERR) */
#define	DBGFSET			(dTIME|dDUTY|dIST|dDUMP|dERR)

#define	_dF_(x)			(!(DBGFSET&x))





void MsSleep( int iVal );
void _SPMSG( unsigned int iFlag, char *msgout, ... );


#endif /* #ifndef __HELPER_H__ */





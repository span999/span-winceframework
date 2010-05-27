#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


///#define DBGMSG			///
#define DBGMSG			printf
#define INFOMSG			printf


#define CHECKBYTECODE	0x0D
#define	BYTERATE_SEC	300


int GetRandomStr( unsigned *pStr );
int DataStore( unsigned char *upChar );
int GetCurrArraySize( void );
int DataCarry( void );
int DumpArrayStatus( void );
int CloseLogfile( void );
bool RequestArrayAddDone( void );
int WriteLog( unsigned char *pChar, unsigned uSize );

#endif	///#ifndef __COMMON_H__




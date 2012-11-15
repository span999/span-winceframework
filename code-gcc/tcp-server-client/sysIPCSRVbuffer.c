/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
/*
#include <stdarg.h>
*/
#include <pthread.h>
/*
#include <string.h>
*/
/*
#include <sys/socket.h>
*/
/*
#include <sys/types.h>
*/
/*
#include <netinet/in.h>
*/
#include "toolhelps.h"

#include "ipcpacket.h"
/*
#include "sysIPCSRV.h"
*/
#include "spRingBuf.h"






#if 0 /* leave ir unused for now */
static pthread_mutex_t mutex;
static int mutexON = 0;

static int mutex_INIT( void )
{
	int iRet = -1;
	
	if( !mutexON )
	{
		if( pthread_mutex_init( &mutex, NULL ) != 0 )
		{
			spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
		}
		else
		{
			mutexON = 1;
			iRet = 0;
		}
	}
	else
		iRet = 0;
	
	return iRet;
}


static int mutex_DESTROY( void )
{
	int iRet = -1;

	if( mutexON )
	{
		pthread_mutex_destroy( &mutex );
		spQMSG( "%s:%s: done !!\n", __FILE__, __FUNCTION__ );
		mutexON = 0;
		iRet = 0;
	}
	
	return iRet;
}


static int mutex_LOCK( void )
{
	int iRet = -1;
	
	mutex_INIT();
	if( mutexON )
		pthread_mutex_lock( &mutex );
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


static int mutex_UNLOCK( void )
{
	int iRet = -1;
	
	mutex_INIT();
	if( mutexON )
		pthread_mutex_unlock( &mutex );
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}
#endif


static CircularBuffer cb;
static CircularBuffer *pcb = NULL;
static int RingBufferSize = 1024;
static void spIPCPackBuffINIT( void )
{
	if( NULL == pcb )
	{
		cbInit(&cb, RingBufferSize, sizeof(struct ipcpacket));
		pcb = &cb;
	}

	if( NULL == pcb )
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
	
	return;
}


int spIPCPackBuffADD( struct ipcpacket *pBuf )
{
	int iRet = -1;
	
	if( pBuf )
	{
		
		spIPCPackBuffINIT();
		spQMSG( "%s:%s: +++\n", __FILE__, __FUNCTION__ );
		cbWrite( pcb, (unsigned char *)pBuf, sizeof(struct ipcpacket) );
		spQMSG( "%s:%s: ---\n", __FILE__, __FUNCTION__ );
	}
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int spIPCPackBuffGET( struct ipcpacket *pBuf )
{
	int iRet = -1;
	
	if( pBuf )
	{
		if( !cbIsEmpty( pcb ) )
		{
			cbRead( pcb, (unsigned char *)pBuf, sizeof(struct ipcpacket) );
			iRet = 0;
		}
/*		
		else
			spQMSG( "spIPCPackBuffGET cbIsEmpty fail !!\n" );
*/
	}
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );

	return iRet;
}


int spIPCPackBuffPEEK( struct ipcpacket *pBuf )
{
	int iRet = -1;
	
	if( pBuf )
	{
		if( !cbIsEmpty( pcb ) )
		{
			cbCopy( pcb, (unsigned char *)pBuf, sizeof(struct ipcpacket) );
			iRet = 0;
		}
/*		
		else
			spQMSG( "spIPCPackBuffGET cbIsEmpty fail !!\n" );
*/
	}
	else
		spQMSG( "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );

	return iRet;
}


int spIPCPackBuffDUMP( void )
{
	int iRet = -1;
	
	if( pcb )
	{
		struct ipcpacket elem;
		
		spQMSG( "RingBuffer dump:\n");
		/* Remove and print all elements */
		while( !cbIsEmpty( pcb ) ) {
			cbRead( pcb, (unsigned char *)&elem, sizeof(struct ipcpacket) );
			spIPCPacketDump( &elem );
		}
	}
	
	return iRet;
}


int spIPCPackBuffpayloadGet( char *pBuf, int *pLen )
{
	int iRet = -1;
	
	if( !cbIsEmpty( pcb ) )
	{
		struct ipcpacket elem;
		struct ipcpacket *pThisPack = NULL;
		
		pThisPack = &elem;
		cbCopy( pcb, (unsigned char *)pThisPack, sizeof(struct ipcpacket) );

		iRet = 0;
		if( pBuf && *pLen >= pThisPack->payloadnum )
		{
			iRet = pThisPack->payloadnum;
			cbRead( pcb, (unsigned char *)pThisPack, sizeof(struct ipcpacket) );
			memcpy( pBuf, pThisPack->payload, pThisPack->payloadnum );
			iRet = 0;
		}
	}
	
	return iRet;
}

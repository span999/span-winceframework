/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "toolhelps.h"
#include "ipcpacket.h"
#include "spRingBuf.h"




/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
/* #define	DBGFSET		(dDBG|dINFO|dERR) */
#define	DBGFSET		(dINFO|dERR)
#define	dF(x)		(DBGFSET&x)





#define		RINGBUFFERLEVEL		64
#define		RINGBUFFERSIZE		(sizeof(struct ipcpacket))


#if 0 /* leave ir unused for now */
static pthread_mutex_t mutex;
static int mutexINITED = 0;
#endif


static CircularBuffer cb;
static CircularBuffer *pcb = NULL;
static int RingBufferLevel = RINGBUFFERLEVEL;
static int RingBufferSize = RINGBUFFERSIZE;


static void spIPCPackBuffINIT( void )
{
	if( NULL == pcb )
	{
		cbInit( &cb, RingBufferLevel, RingBufferSize );
		pcb = &cb;
	}

	if( NULL == pcb )
		spMSG( dF(dERR), "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
	
	return;
}


int spIPCPackBuffADD( struct ipcpacket *pBuf )
{
	int iRet = -1;
	
	if( pBuf )
	{
		
		spIPCPackBuffINIT();
		spMSG( dF(dDBG), "%s:%s: +++\n", __FILE__, __FUNCTION__ );
		cbWrite( pcb, (unsigned char *)pBuf, sizeof(struct ipcpacket) );
		spMSG( dF(dDBG), "%s:%s: ---\n", __FILE__, __FUNCTION__ );
	}
	else
		spMSG( dF(dERR), "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );
	
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
		spMSG( dF(dERR), "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );

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
		spMSG( dF(dERR), "%s:%s: failed !!\n", __FILE__, __FUNCTION__ );

	return iRet;
}


int spIPCPackBuffDUMP( void )
{
	int iRet = -1;
	
	if( pcb )
	{
		struct ipcpacket elem;
		
		spMSG( dF(dDBG), "%s:%s: RingBuffer dump:\n");
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

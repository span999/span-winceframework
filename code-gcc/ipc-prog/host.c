/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
/*
#include <signal.h>
#include <sys/time.h>
*/


#include "ipc_common.h"
#include "so_ipc.h"
#include "spRingBuf.h"



#define		RINGBUFLEVEL	64
#define		RINGBUFSIZE		1024
static CircularBuffer cb;
static CircularBuffer *spRingBufINIT( int BufSize )
{
	static CircularBuffer *pcb = NULL;
	int RingBuffLevel = RINGBUFLEVEL;
	
	if( NULL == pcb )	/* first use, do inital */
	{
		if( 0 ==  BufSize )
			BufSize = RINGBUFSIZE; 
		cbInit( &cb, RingBuffLevel, BufSize );
		pcb = &cb;
	}
	
	return pcb;
}


static int spRingBufADD( char *pData, int Size )
{
	int iRet = -1;
	
	if( pData )
	{
		if( RINGBUFLEVEL <= Size )
		{
			printf("%s:%s: cbWrite +++\n", __FILE__, __FUNCTION__);
			cbWrite( spRingBufINIT(0), (unsigned char *)pData, Size );
			printf("%s:%s: cbWrite ---\n", __FILE__, __FUNCTION__);
			iRet = 0;
		}
		else
			printf("%s:%s: ERROR!! data over flow !\n", __FILE__, __FUNCTION__);
	}
	else
		printf("%s:%s: ERROR!! null pointer !\n", __FILE__, __FUNCTION__);
	
	return iRet;
}


static int spRingBufGET( char *pData, int Size )
{
	int iRet = -1;
	
	if( pData )
	{
		if( RINGBUFLEVEL <= Size )
		{
			if( !cbIsEmpty( spRingBufINIT(0) ) )
			{
				cbRead( spRingBufINIT(0), (unsigned char *)pData, Size );
				iRet = 0;
			}
			else
				printf("%s:%s: ERROR!! data empty !\n", __FILE__, __FUNCTION__);
		}
		else
			printf("%s:%s: ERROR!! data not available !\n", __FILE__, __FUNCTION__);
	}
	else
		printf("%s:%s: ERROR!! null pointer !\n", __FILE__, __FUNCTION__);

	return iRet;
}


static int spRingBufPEEK( char *pData, int Size )
{
	int iRet = -1;
	
	if( pData )
	{
		if( RINGBUFLEVEL <= Size )
		{
			if( !cbIsEmpty( spRingBufINIT(0) ) )
			{
				cbCopy( spRingBufINIT(0), (unsigned char *)pData, Size );
				iRet = 0;
			}
			else
				printf("%s:%s: ERROR!! data over flow !\n", __FILE__, __FUNCTION__);
		}
		else
			printf("%s:%s: ERROR!! data not available !\n", __FILE__, __FUNCTION__);
	}
	else
		printf("%s:%s: ERROR!! null pointer !\n", __FILE__, __FUNCTION__);

	return iRet;
}



static int hostpoolReadySEM_ID = 0;
static int hostpoolAvailableSEM_ID = 0;
static int hostSHMpool_ID = 0;


static void *sharedmemoryHandler( void* unused )
{
	char *pPool = NULL;
	
	for(;;)
	{
		/* wait data available */
		printf("%s:%s: wait data available +++\n", __FILE__, __FUNCTION__);
		semLOCK(hostpoolAvailableSEM_ID);
		printf("%s:%s: wait data available ---\n", __FILE__, __FUNCTION__);
		/* data available, lock pool with NOT ready */
		semLOCK(hostpoolReadySEM_ID);
		printf("%s:%s: pending for data copy +++\n", __FILE__, __FUNCTION__);
		
		/* copy data to ring buffer */
		SharedMemoryIDinit( hostSHMpool_ID, &pPool );
		if( NULL != pPool )
		{
			spRingBufADD( pPool, RINGBUFSIZE );
			printf( "%s:%s: add [%c] to ringbuffer\n", __FILE__, __FUNCTION__, *(char *)pPool );
		}
		else
			printf( "%s:%s: ERROR!! SharedMemoryIDinit return null pointer !\n", __FILE__, __FUNCTION__ );
		SharedMemoryIDdeinit( pPool );
		
		/* copy done, unlock pool with ready */
		printf("%s:%s: pending for data copy ---\n", __FILE__, __FUNCTION__);
		semUNLOCK(hostpoolReadySEM_ID);
	}

	return NULL;
}


static void *ringbufferHandler( void* unused )
{
	int iRet = -1;
	char tmpBuf[2048];
	char *pPool;

	
	for(;;)
	{
		usleep( (iRet == -1)?500000:10000 );
		/* check ring buffer data available */
		iRet = spRingBufGET( tmpBuf, 2048 );
		if( 0 == iRet )
		{
			struct ipcpacket *pipc;
			struct sysPowerCmd *pCmd;
			int clientSHMid = -1;
			int clientSEMid = -1;
			
			pipc = (struct ipcpacket *)tmpBuf;
			pCmd = (struct sysPowerCmd *)pipc->payload;
			clientSHMid = pipc->srcSHMid;
			clientSEMid = pipc->srcSEMid;
			printf( "%s:%s: SHM:%d SEM:%d size:%d\n", __FILE__, __FUNCTION__, clientSHMid, clientSEMid, pipc->payloadnum );
			if( pipc->payloadnum > 0 )
			{
				pCmd->rspReturn = pCmd->cmdID;
				
				/* copy data to pool */
				iRet = SharedMemoryIDinit( clientSHMid, &pPool );
				///if( NULL != pPool )
				if( (pPool > 0) && (0 == iRet) )
				{
					memcpy( pPool, tmpBuf, sizeof(struct ipcpacket) );
				}
				else
					printf("%s:%s: ERROR!! SharedMemoryIDinit return null pointer !\n", __FILE__, __FUNCTION__);
				SharedMemoryIDdeinit( pPool );
				
				/* release */
				semUNLOCK(clientSEMid);
			}
			else
				printf( "%s:%s: ERROR!! payloadnum = %d \n", __FILE__, __FUNCTION__, pipc->payloadnum );

		}
		else
			printf("%s:%s: ERROR!! spRingBufGET \n", __FILE__, __FUNCTION__);
	}

	return NULL;
}




int main()
{
	int iRet = 0;
	pthread_t thread_id1;
	pthread_t thread_id2;


	/* create shared memory */
	hostSHMpool_ID = getNamedSharedMemoryID( NAMEDHOSTSHMPATH, RINGBUFSIZE );

	/* create semaphore for memory pool available */
	hostpoolAvailableSEM_ID = getNamedSemaphoresID( NAMEDHOSTSEMAVAPATH );
	/* init semaphore as pending */
	SemaphoresIDinitwait( hostpoolAvailableSEM_ID );

	/* create semaphore for memory pool ready */
	hostpoolReadySEM_ID = getNamedSemaphoresID( NAMEDHOSTSeMRDYPATH );
	/* init semaphore as free */
	SemaphoresIDinit( hostpoolReadySEM_ID );



	printf( "The process ID is %d\n", (int)getpid() );
	printf( "The parent process ID is %d\n", (int)getppid() );

	pthread_create( &thread_id1, NULL, &ringbufferHandler, NULL );
	pthread_create( &thread_id2, NULL, &sharedmemoryHandler, NULL );



	sleep(0); /* leave some time for thread */
	getchar();
	SemaphoresIDdestroy( hostpoolReadySEM_ID );
	SemaphoresIDdestroy( hostpoolAvailableSEM_ID );
	SharedMemoryIDdestroy(hostSHMpool_ID );

	printf( "\ndone \n" );
	return iRet;
}


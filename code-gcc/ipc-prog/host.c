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
				printf("%s:%s: ERROR!! data over flow !\n", __FILE__, __FUNCTION__);
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





int main()
{
	int iRet = 0;
	pthread_t thread_id;


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

	pthread_create( &thread_id, NULL, &sharedmemoryHandler, NULL );


/*
	if(1)
	{
		char *pChar;
		
		SharedMemoryIDinit( iSharedMemory, &pChar );
		
		strncpy( pChar, "This is a test string!", 24 );
		pChar[0] = 'T';
		pChar[1] = 'h';
		pChar[2] = 'a';
		pChar[3] = 't';
		
		printf( "set ==>%s [%c][%c][%c][%c]\n", pChar, pChar[0], pChar[1], pChar[2], pChar[3] );
		SharedMemoryIDdeinit( pChar );		
	}

	if(1)
	{
		char *pChar2;
		
		SharedMemoryIDinit( iSharedMemory, &pChar2 );
		printf( "get ==>%s [%c][%c][%c][%c]\n", pChar2, pChar2[0], pChar2[1], pChar2[2], pChar2[3] );
		SharedMemoryIDdeinit( pChar2 );
	}
*/

	sleep(0); /* leave some time for thread */
	getchar();
	SemaphoresIDdestroy( hostpoolReadySEM_ID );
	SemaphoresIDdestroy( hostpoolAvailableSEM_ID );
	SharedMemoryIDdestroy(hostSHMpool_ID );

	printf( "\ndone \n" );
	return iRet;
}


/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*
#include <pthread.h>

#include <signal.h>
#include <sys/time.h>
*/

#include "ipc_common.h"
#include "so_ipc.h"


#define		RINGBUFSIZE		1024


static int hostpoolReadySEM_ID = 0;
static int hostpoolAvailableSEM_ID = 0;
static int hostSHMpool_ID = 0;

static int SendData( char *pData, unsigned int len )
{
	int iRet = -1;
	char *pPool = NULL;
	
	/* wait for pool ready */
	semLOCK(hostpoolReadySEM_ID);

	/* wait for pool available */
	/* semLOCK(hostpoolAvailableSEM_ID); */
	
	/* copy data to pool */
	SharedMemoryIDinit( hostSHMpool_ID, &pPool );
	if( NULL != pPool )
		memcpy( pPool, pData, len );
	else
		printf("%s:%s: ERROR!! SharedMemoryIDinit return null pointer !\n", __FILE__, __FUNCTION__);
	SharedMemoryIDdeinit( pPool );

	/* set for pool available */
	semUNLOCK(hostpoolAvailableSEM_ID);
	/* set for pool ready */
	semUNLOCK(hostpoolReadySEM_ID);

	return iRet;
}


int main()
{
	int return_value = 0;
	pthread_t thread_id;
	char testchar = 'A';
	int iLoop = 10;

	/* create shared memory */
	hostSHMpool_ID = getNamedSharedMemoryIDs( NAMEDHOSTSHMPATH, RINGBUFSIZE );

	/* create semaphore for memory pool available */
	hostpoolAvailableSEM_ID = getNamedSemaphoresIDs( NAMEDHOSTSEMAVAPATH );
	/* init semaphore as pending */
	/* SemaphoresIDinitwait( hostpoolAvailableSEM_ID ); */

	/* create semaphore for memory pool ready */
	hostpoolReadySEM_ID = getNamedSemaphoresIDs( NAMEDHOSTSeMRDYPATH );
	/* init semaphore as free */
	/* SemaphoresIDinit( hostpoolReadySEM_ID ); */


	printf("The process ID is %d\n", (int)getpid() );
	printf("The parent process ID is %d\n", (int)getppid() );

	
	while( iLoop > 0 )
	{
		printf("send data [%c] to shared memory\n", testchar );
		SendData( &testchar, sizeof(testchar) );
		testchar = testchar + 1;
		iLoop--;
	}


	sleep(0); /* leave some time for thread */
	getchar();

	printf( "\ndone \n" );
	return 0;
}


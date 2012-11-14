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


/* spIPCsend */
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
	{
		memcpy( pPool, pData, len );
		iRet = 0;
	}
	else
		printf("%s:%s: ERROR!! SharedMemoryIDinit return null pointer !\n", __FILE__, __FUNCTION__);
	SharedMemoryIDdeinit( pPool );

	/* set for pool available */
	semUNLOCK(hostpoolAvailableSEM_ID);
	/* set for pool ready */
	semUNLOCK(hostpoolReadySEM_ID);

	return iRet;
}


#define		NAMEDCLINTSHMPATH			("/dev/fb0")
#define		NAMEDCLIENTSEMAVAPATH		("/dev/loop0")


/* spIPCrequest */
static int RequestData( char *pData, unsigned int len )
{
	int iRet = -1;
	char *pPool = NULL;
	static int clientpoolAvailableSEM_ID = 0;
	static int clientSHMpool_ID = 0;
	struct ipcpacket *pipc;
	
	/* create shared memory */
	clientSHMpool_ID = getNamedSharedMemoryID( NAMEDCLINTSHMPATH, RINGBUFSIZE );

	/* create semaphore for memory pool available */
	clientpoolAvailableSEM_ID = getNamedSemaphoresID( NAMEDCLIENTSEMAVAPATH );
	/* init semaphore as pending */
	SemaphoresIDinitwait( clientpoolAvailableSEM_ID );

	pipc = (struct ipcpacket *)pData;
	pipc->srcSHMid = clientSHMpool_ID;
	pipc->srcSEMid = clientpoolAvailableSEM_ID;

	iRet = SendData( pData, len );
	
	/* wait response after data out*/
	printf("%s:%s: wait for response !\n", __FILE__, __FUNCTION__);
	semLOCK( clientpoolAvailableSEM_ID );
	
	
	/* copy data from pool */
	SharedMemoryIDinit( clientSHMpool_ID, &pPool );
	if( NULL != pPool )
	{
		memcpy( pData, pPool, len );
	}
	else
		printf("%s:%s: ERROR!! SharedMemoryIDinit return null pointer !\n", __FILE__, __FUNCTION__);
	SharedMemoryIDdeinit( pPool );


	SemaphoresIDdestroy( clientpoolAvailableSEM_ID );
	SharedMemoryIDdestroy( clientSHMpool_ID );


	return iRet;
}



int main()
{
	int return_value = 0;
	pthread_t thread_id;
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

	if(1)
	{
		struct ipcpacket ipc;
		struct sysPowerCmd *pCmd;
		int iRet = -1;
		
		iLoop = 0;
		while( iLoop < 1000 )
		{
			pCmd = (struct sysPowerCmd *)ipc.payload;
			ipc.payloadnum = sizeof(struct sysPowerCmd);
			
			pCmd->cmdID = iLoop;
			printf("send cmd data [%d] to shared memory\n", iLoop );
			#if 0
			SendData( (char *)&ipc, sizeof(struct ipcpacket) );
			#else
			iRet = RequestData( (char *)&ipc, sizeof(struct ipcpacket) );
			printf("get cmd resp [%d]\n", pCmd->rspReturn );
			#endif
			iLoop++;
		}
	}


	sleep(0); /* leave some time for thread */
	/* getchar(); */

	printf( "\ndone \n" );
	return 0;
}


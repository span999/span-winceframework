/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <string.h>

#include "toolhelps.h"
#include "ipcpacket.h"
#include "sysIPCSRV.h"
#include "sysIPCSRVbuffer.h"
#include "spRingBuf.h"
#include "spSemShm.h"



/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
/* #define	DBGFSET		(dDBG|dINFO|dERR) */
#define	DBGFSET		(dINFO|dERR)
#define	dF(x)		(DBGFSET&x)





static tSRVMGRTYP serverTypeSHM = NONEMGR;
static PFNIPCCALLBACK ipcCallbackSHM = NULL;
static pthread_t shmserv_thread_id;


static int hostpoolReadySEM_ID = 0;
static int hostpoolAvailableSEM_ID = 0;
static int hostSHMpool_ID = 0;


static pthread_mutex_t mutex;
static int mutexINITED = 0;




#define		NAMEDHOSTSHMPATH		("/dev/null")
#define		NAMEDHOSTSEMAVAPATH		("/dev/zero")
#define		NAMEDHOSTSeMRDYPATH		("/dev/rtc")

#define		semLOCK(x)		SemaphoresIDset(x,1)
#define		semUNLOCK(x)	SemaphoresIDset(x,0)

/*#define		RINGBUFSIZE		1024*/
#define		RINGBUFSIZE		(sizeof(struct ipcpacket))

static void *shmServer( void *argv )
{
	unsigned int *piRet = NULL;
	char *pPool = NULL;

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

	spMSG( dF(dINFO), "%s:%s: The process ID is %d\n", __FILE__, __FUNCTION__, (int)getpid() );
	spMSG( dF(dINFO), "%s:%s: The parent process ID is %d\n", __FILE__, __FUNCTION__, (int)getppid() );


	for(;;)
	{
		/* wait data available */ /* wait for SEM/SHM */
		spMSG( dF(dDBG), "%s:%s: wait data available +++\n", __FILE__, __FUNCTION__);
		semLOCK(hostpoolAvailableSEM_ID);
		spMSG( dF(dDBG), "%s:%s: wait data available ---\n", __FILE__, __FUNCTION__);
		/* data available, lock pool with NOT ready */
		semLOCK(hostpoolReadySEM_ID);
		spMSG( dF(dDBG), "%s:%s: pending for data copy +++\n", __FILE__, __FUNCTION__);
	
		spMxL( &mutex, &mutexINITED );
		
		/* copy data to ring buffer */ /* get data to ring buffer */
		SharedMemoryIDinit( hostSHMpool_ID, &pPool );
		if( NULL != pPool )
		{
			spIPCPackBuffADD( (struct ipcpacket *)pPool );
			spMSG( dF(dDBG), "%s:%s: add [%c] to ringbuffer\n", __FILE__, __FUNCTION__, *(char *)pPool );
		}
		else
			spMSG( dF(dERR), "%s:%s: ERROR!! SharedMemoryIDinit return null pointer !\n", __FILE__, __FUNCTION__ );
		SharedMemoryIDdeinit( pPool );
		
		spMxU( &mutex, &mutexINITED );
		
		/* copy done, unlock pool with ready */
		spMSG( dF(dDBG), "%s:%s: pending for data copy ---\n", __FILE__, __FUNCTION__);
		semUNLOCK(hostpoolReadySEM_ID);
		
		/* callback to user/server */
		if( ipcCallbackSHM )
			ipcCallbackSHM();
	}


	return piRet;
}


static int SendData2host( char *pData, int ilen )
{
	int iRet = -1;
	char *pPool = NULL;

	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	
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
	
	
	/* wait for pool ready */
	semLOCK(hostpoolReadySEM_ID);

	/* wait for pool available */
	/* semLOCK(hostpoolAvailableSEM_ID); */
	
	/* copy data to pool */
	SharedMemoryIDinit( hostSHMpool_ID, &pPool );
	if( NULL != pPool )
	{
		memcpy( pPool, pData, ilen );
		iRet = 0;
	}
	else
		spMSG( dF(dERR), "%s:%s: ERROR!! SharedMemoryIDinit return null pointer !\n", __FILE__, __FUNCTION__);
	SharedMemoryIDdeinit( pPool );

	/* set for pool available */
	semUNLOCK(hostpoolAvailableSEM_ID);
	/* set for pool ready */
	semUNLOCK(hostpoolReadySEM_ID);

	return iRet;
}


static int SendData2client( char *pData, int ilen )
{
	int iRet = -1;
	char *pPool = NULL;
	struct ipcpacket *pipc;
	int iSHM;
	int iSEM;
	
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	pipc = (struct ipcpacket *)pData;

	iSHM = pipc->srcSHMid;
	iSEM = pipc->srcSEMid;

	/* clean client SHM/SEM info */
	pipc->srcSHMid = -1;
	pipc->srcSEMid = -1;
	
	/* wait for pool ready */
	/* semLOCK(hostpoolReadySEM_ID); */

	/* wait for pool available */
	/* semLOCK(hostpoolAvailableSEM_ID); */
	
	/* copy data to pool */
	SharedMemoryIDinit( iSHM, &pPool );
	if( NULL != pPool )
	{
		memcpy( pPool, pData, ilen );
		iRet = 0;
	}
	else
		spMSG( dF(dERR), "%s:%s: ERROR!! SharedMemoryIDinit return null pointer !\n", __FILE__, __FUNCTION__);
	SharedMemoryIDdeinit( pPool );

	/* set for pool available */
	semUNLOCK(iSEM);

	return iRet;
}


#define		NAMEDCLINTSHMPATH			("/dev/fb0")
#define		NAMEDCLIENTSEMAVAPATH		("/dev/loop0")

int spIPCrequestSHM( char *pData, int *piLen, tSRVMGRTYP type )
{
	int iRet = -1;

	char *pPool = NULL;
	static int clientpoolAvailableSEM_ID = 0;
	static int clientSHMpool_ID = 0;
	struct ipcpacket *pipc;
	struct ipcpacket ipcPak;
	
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	
	/* prepare recv memory pool */
	/* create shared memory */
	clientSHMpool_ID = getNamedSharedMemoryID( NAMEDCLINTSHMPATH, RINGBUFSIZE );

	/* create semaphore for memory pool available */
	clientpoolAvailableSEM_ID = getNamedSemaphoresID( NAMEDCLIENTSEMAVAPATH );
	/* init semaphore as pending */
	SemaphoresIDinitwait( clientpoolAvailableSEM_ID );

	/* prepare send data struct */
	/* clean packet */
	spIPCPacketInit( &ipcPak );

	/* setup client info */
	ipcPak.srcSHMid = clientSHMpool_ID;
	ipcPak.srcSEMid = clientpoolAvailableSEM_ID;

	ipcPak.serialnum = 0;
	ipcPak.packetnum = 0;
	ipcPak.payloadnum = *piLen;
	memcpy( ipcPak.payload, pData, *piLen );

	/* add CRC sign */
	spIPCPacketCRCsign( &ipcPak );
	if( dF(dDBG) ) spIPCPacketDump( &ipcPak );

	iRet = SendData2host( (char *)&ipcPak, sizeof(struct ipcpacket) );


	/* wait response after data out*/
	spMSG( dF(dDBG), "%s:%s: wait for response !\n", __FILE__, __FUNCTION__);
	semLOCK( clientpoolAvailableSEM_ID );
	
	
	/* copy payload data from pool */
	SharedMemoryIDinit( clientSHMpool_ID, &pPool );
	if( NULL != pPool )
	{
		/* memcpy( pData, pPool, *piLen ); */
		pipc = (struct ipcpacket *)pPool;
		memcpy( pData, pipc->payload, pipc->payloadnum );
	}
	else
		spMSG( dF(dERR), "%s:%s: ERROR!! SharedMemoryIDinit return null pointer !\n", __FILE__, __FUNCTION__);
	SharedMemoryIDdeinit( pPool );


	SemaphoresIDdestroy( clientpoolAvailableSEM_ID );
	SharedMemoryIDdestroy( clientSHMpool_ID );


	return iRet;
}


int spIPCPackResponseSHM( struct ipcpacket *pBuf, char *pData, int iLen )
{
	int iRet = -1;
	struct ipcpacket ipcPak;


	/* init packet */
	spIPCPacketInit( &ipcPak );
	
	if( pBuf )
	{
		memcpy( &ipcPak, pBuf, sizeof(struct ipcpacket) ); 
	}
	
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	
	if( pBuf && pData && (iLen<=255) )
	{
		/* setup packet */

		/* copy payload data */
		ipcPak.payloadnum = iLen;
		memcpy( ipcPak.payload, pData, iLen );
	}
	else
		spMSG( dF(dERR), "%s:%s: ERROR!! spIPCPackResponse: error !!! \n", __FILE__, __FUNCTION__ );
		
	/* add CRC sign */
	spIPCPacketCRCsign( &ipcPak );
	if( dF(dDBG) ) spIPCPacketDump( &ipcPak );
	
	/* send out */
	iRet = SendData2client( (char *)&ipcPak, sizeof(struct ipcpacket) );

	return iRet;
}


int spIPCsetCallbackSHM( PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	if( pCB )
	{
		ipcCallbackSHM = pCB;
		iRet = 0;
	}

	return iRet;
}


int spIPCinitServerSHM( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB )
{
	int iRet = -1;

	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );

	serverTypeSHM = servertype;
	iRet = spIPCsetCallbackSHM( pCB );
	pthread_create( &shmserv_thread_id, NULL, &shmServer, NULL );

	return iRet;
}


int spIPCInitSHM( void )
{
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	
	spMxI( &mutex, &mutexINITED );

	return 0;
}


int spIPCDeinitSHM( void )
{
	spMSG( dF(dDBG), "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	
	if( serverTypeSHM != NONEMGR )
	{
		/* release resource from server service */
		SemaphoresIDdestroy( hostpoolReadySEM_ID );
		SemaphoresIDdestroy( hostpoolAvailableSEM_ID );
		SharedMemoryIDdestroy(hostSHMpool_ID );
	}	
	
	spMxD( &mutex, &mutexINITED );
	return 0;
}

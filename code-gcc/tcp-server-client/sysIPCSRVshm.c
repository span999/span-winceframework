/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <string.h>

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include "toolhelps.h"
#include "ipcpacket.h"
#include "sysIPCSRV.h"
#include "sysIPCSRVbuffer.h"
#include "spRingBuf.h"



/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
#define	DBGFSET		(dDBG|dINFO|dERR)
/* #define	DBGFSET		(dINFO|dERR) */
#define	dF(x)		(DBGFSET&x)





static tSRVMGRTYP serverTypeSHM = NONEMGR;
static PFNIPCCALLBACK ipcCallbackSHM = NULL;
static pthread_t shmserv_thread_id;


static int hostpoolReadySEM_ID = 0;
static int hostpoolAvailableSEM_ID = 0;
static int hostSHMpool_ID = 0;


static pthread_mutex_t mutex;
static int mutexINITED = 0;


union semun {
	int val;
	struct semid_ds *buf;
	unsigned short  *array;
};




int getSemaphoresID( void )
{
	int iRet = -1;
	key_t key = -1;
	int semid = -1;

	/* create semaphores with 1 resource */
	key = ftok("/dev/null", 'E');
	if( -1 != key )
	{
		semid = semget( key, 1, 0666 | IPC_CREAT );

		if( semid != -1 )
		{
			iRet = semid;
			spMSG( dF(dDBG), "%s:%s:OK! semget [%d] \n", __FILE__, __FUNCTION__, semid );
		}
		else
		{
			spMSG( dF(dERR), "%s:%s:ERROR! semget fail [%d] \n", __FILE__, __FUNCTION__, semid );
			/*
			perror( "ERROR!! " );
			*/ 
		}
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! ftok fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


int getNamedSemaphoresID( char *Name )
{
	int iRet = -1;
	key_t key = -1;
	int semid = -1;

	/* create semaphores with 1 resource */
	key = ftok( Name, 'E');
	if( -1 != key )
	{
		semid = semget( key, 1, 0666 | IPC_CREAT );

		if( semid != -1 )
		{
			iRet = semid;
			spMSG( dF(dDBG), "%s:%s:OK! semget [%d] \n", __FILE__, __FUNCTION__, semid );
		}
		else
		{
			spMSG( dF(dERR), "%s:%s:ERROR! semget fail [%d] \n", __FILE__, __FUNCTION__, semid );
			/*
			perror( "ERROR!! " );
			*/ 
		}
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! ftok fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


int getNamedSemaphoresIDs( char *Name )
{
	int iRet = -1;
	key_t key = -1;
	int semid = -1;

	/* create semaphores with 1 resource */
	key = ftok( Name, 'E');
	if( -1 != key )
	{
		semid = semget( key, 1, 0 );

		if( semid != -1 )
		{
			iRet = semid;
			spMSG( dF(dDBG), "%s:%s:OK! semget [%d] \n", __FILE__, __FUNCTION__, semid );
		}
		else
		{
			spMSG( dF(dERR), "%s:%s:ERROR! semget fail [%d] \n", __FILE__, __FUNCTION__, semid );
			/*
			perror( "ERROR!! " );
			*/ 
		}
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! ftok fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


int SemaphoresIDinit( int semid )
{
	int iRet = 0;
	union semun sem_arg;
	
	if( semid > -1 )
	{
		
		/* set 1 to #0 semaphores */
		/* means semaphores #0 is available */
		sem_arg.val = 1;
		iRet = semctl( semid, 0, SETVAL, sem_arg );
		if( iRet == -1 )
			spMSG( dF(dERR), "%s:%s:ERROR! semctl fail\n", __FILE__, __FUNCTION__ );
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int SemaphoresIDinitwait( int semid )
{
	int iRet = 0;
	union semun sem_arg;
	
	if( semid > -1 )
	{
		
		/* set 0 to #0 semaphores */
		/* means semaphores #0 is NOT available */
		sem_arg.val = 0;
		iRet = semctl( semid, 0, SETVAL, sem_arg );
		if( iRet == -1 )
			spMSG( dF(dERR), "%s:%s:ERROR! semctl fail\n", __FILE__, __FUNCTION__ );
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int SemaphoresIDdestroy( int semid )
{
	int iRet = 0;
	union semun sem_arg;
	
	if( semid > -1 )
	{
		
		/* set 1 to #0 semaphores */
		/* means semaphores #0 is available */
		sem_arg.val = 1;
		iRet = semctl( semid, 0, IPC_RMID, sem_arg );
		if( iRet == -1 )
			spMSG( dF(dERR), "%s:%s:ERROR! semctl fail\n", __FILE__, __FUNCTION__ );
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int SemaphoresIDset( int semid, int op )
{
	/* op: 0=unlock, 1=lock */
	int iRet = -1;
	struct sembuf sb;
	
	sb.sem_num = 0;	/* resource #0 in semaphore */
	if( 0 == op )
		sb.sem_op = 1;
	else
		sb.sem_op = -1;
	sb.sem_flg = 0;
 	
 	
 	spMSG( dF(dDBG), "\n%s:%s:TRY! semop +++ %s ....\n", __FILE__, __FUNCTION__, (0==op)?"unlock":"lock" );
	/*                numbers of sembuf */
	/*                sembuf  |         */
	/*                    v   v         */
	iRet = semop( semid, &sb, 1 );
	
	if( iRet == -1 )
		spMSG( dF(dERR), "%s:%s:ERROR! semop %s fail\n", __FILE__, __FUNCTION__, (0==op)?"unlock":"lock" );
	else
	{
		spMSG( dF(dDBG), "\n%s:%s:TRY! semop --- %s ....\n", __FILE__, __FUNCTION__, (0==op)?"unlock":"lock" );
	}
	
	return iRet;
}





int getNamedSharedMemoryID( char *Name, int iSize )
{
	int iRet = -1;
	key_t key = -1;
	int shmid = -1;

	/* create shared memory with key */
	key = ftok( Name, 'M');
	if( -1 != key )
	{
		#if 1
		shmid = shmget( key, iSize, 0666 | IPC_CREAT );
		#else
		shmid = shmget( key, iSize, 0644 | IPC_CREAT );
		#endif

		if( shmid != -1 )
		{
			iRet = shmid;
			spMSG( dF(dDBG), "%s:%s:OK! shmget [%d] \n", __FILE__, __FUNCTION__, shmid );
		}
		else
		{
			spMSG( dF(dERR), "%s:%s:ERROR! shmget fail [%d] \n", __FILE__, __FUNCTION__, shmid );
			/*
			perror( "ERROR!! " );
			*/ 
		}
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! ftok fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


int getNamedSharedMemoryIDs( char *Name, int iSize )
{
	int iRet = -1;
	key_t key = -1;
	int shmid = -1;

	/* create shared memory with key */
	key = ftok( Name, 'M');
	if( -1 != key )
	{
		#if 1
		shmid = shmget( key, iSize, 0 );
		#else
		shmid = shmget( key, iSize, 0 );
		#endif

		if( shmid != -1 )
		{
			iRet = shmid;
			spMSG( dF(dDBG), "%s:%s:OK! shmget [%d] \n", __FILE__, __FUNCTION__, shmid );
		}
		else
		{
			spMSG( dF(dERR), "%s:%s:ERROR! shmget fail [%d] \n", __FILE__, __FUNCTION__, shmid );
			/*
			perror( "ERROR!! " );
			*/ 
		}
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! ftok fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


int SharedMemoryIDinit( int shmid, char **This )
{
	int iRet = 0;
	void *pV = (void *)-1;
	
	if( (shmid > -1) && This )
	{
		/* attach memory address */
		pV = shmat( shmid, NULL, 0 );
		if( pV == NULL || pV == -1 )
		{
			spMSG( dF(dERR), "%s:%s:ERROR! shmat fail [0x%x]->[0x%x]\n", __FILE__, __FUNCTION__, *This, pV );
			iRet = -1;
		}
		else
		{
			spMSG( dF(dDBG), "%s:%s:OK! shmat [0x%x]\n", __FILE__, __FUNCTION__, *This );
			*This = (char *)pV;
			spMSG( dF(dDBG), "->[0x%x]\n", *This );
		}
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


int SharedMemoryIDdeinit( char *This )
{
	int iRet = -1;
	
	if( This )
	{
		/* de-attach memory address */
		iRet = shmdt( This );
		if( iRet == -1 )
			spMSG( dF(dERR), "%s:%s:ERROR! shmdt [0x%x] fail\n", __FILE__, __FUNCTION__, This );
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


int SharedMemoryIDdestroy( int shmid )
{
	int iRet = 0;
	
	if( shmid > -1 )
	{
		iRet = shmctl( shmid, IPC_RMID, NULL );
		if( iRet == -1 )
			spMSG( dF(dERR), "%s:%s:ERROR! shmctl fail\n", __FILE__, __FUNCTION__ );
	}
	else
		spMSG( dF(dERR), "%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}




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
	/* set for pool ready */
	/* semUNLOCK(hostpoolReadySEM_ID); */

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
	
	spQMSG( "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	
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
	spIPCPacketDump( &ipcPak );

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
	
	spQMSG( "%s:%s: !!! \n", __FILE__, __FUNCTION__ );
	
	if( pBuf && pData && (iLen<=255) )
	{
		/* setup packet */
	#if 0	
		/* ipcPak.userID = iSrcID; */
		memcpy( ipcPak.srcip, pBuf->tarip, 10 );
		ipcPak.srcport = pBuf->tarport;
		memcpy( ipcPak.tarip, pBuf->srcip, 10 );
		ipcPak.tarport = pBuf->srcport;
		/* ipcPak.serialnum = 0; */
		ipcPak.packetnum = 0;
	#else
	#endif	
		/* copy payload data */
		ipcPak.payloadnum = iLen;
		memcpy( ipcPak.payload, pData, iLen );
	}
	else
		spQMSG( "spIPCPackResponse: error !!! \n" );
		
	/* add CRC sign */
	spIPCPacketCRCsign( &ipcPak );
	spIPCPacketDump( &ipcPak );
	
	/* send out with tcp socket */
	#if 0
	iRet = tcpSockSend( ipcPak.tarip, ipcPak.tarport, (char *)&ipcPak, sizeof(struct ipcpacket) );
	#else
	iRet = SendData2client( (char *)&ipcPak, sizeof(struct ipcpacket) );
	#endif

	return iRet;
}


int spIPCsetCallbackSHM( PFNIPCCALLBACK pCB )
{
	int iRet = -1;

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

	serverTypeSHM = servertype;
	iRet = spIPCsetCallbackSHM( pCB );
	pthread_create( &shmserv_thread_id, NULL, &shmServer, NULL );

	return iRet;
}


int spIPCInitSHM( void )
{
	spMxI( &mutex, &mutexINITED );

	return 0;
}


int spIPCDeinitSHM( void )
{
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

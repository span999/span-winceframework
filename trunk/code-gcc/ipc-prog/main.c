/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <sys/time.h>

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include "so_test.h"


static int iSemaphore = 0;
static int iSharedMemory = 0;


static int getSemaphoresID( void )
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
			printf("%s:%s:OK! semget [%d] \n", __FILE__, __FUNCTION__, semid );
		}
		else
		{
			printf("%s:%s:ERROR! semget fail [%d] \n", __FILE__, __FUNCTION__, semid );
			/*
			perror( "ERROR!! " );
			*/ 
		}
	}
	else
		printf("%s:%s:ERROR! ftok fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


static int getNamedSemaphoresID( char *Name )
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
			printf("%s:%s:OK! semget [%d] \n", __FILE__, __FUNCTION__, semid );
		}
		else
		{
			printf("%s:%s:ERROR! semget fail [%d] \n", __FILE__, __FUNCTION__, semid );
			/*
			perror( "ERROR!! " );
			*/ 
		}
	}
	else
		printf("%s:%s:ERROR! ftok fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


union semun {
	int val;
	struct semid_ds *buf;
	unsigned short  *array;
};

static int SemaphoresIDinit( int semid )
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
			printf("%s:%s:ERROR! semctl fail\n", __FILE__, __FUNCTION__ );
	}
	else
		printf("%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


static int SemaphoresIDdestroy( int semid )
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
			printf("%s:%s:ERROR! semctl fail\n", __FILE__, __FUNCTION__ );
	}
	else
		printf("%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


static int SemaphoresIDset( int semid, int op )
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
 	
 	
 	printf("\n%s:%s:TRY! semop +++ %s ....\n", __FILE__, __FUNCTION__, (0==op)?"unlock":"lock" );
	/*                numbers of sembuf */
	/*                sembuf  |         */
	/*                    v   v         */
	iRet = semop( semid, &sb, 1 );
	
	if( iRet == -1 )
		printf("%s:%s:ERROR! semop %s fail\n", __FILE__, __FUNCTION__, (0==op)?"unlock":"lock" );
	else
		printf("\n%s:%s:TRY! semop --- %s ....\n", __FILE__, __FUNCTION__, (0==op)?"unlock":"lock" );
	
	return iRet;
}


#define		semLOCK(x)		SemaphoresIDset(x,1)
#define		semUNLOCK(x)	SemaphoresIDset(x,0)
#if 1
#define USE_SEMAPHOE
#endif




static int getNamedSharedMemoryID( char *Name, int iSize )
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
			printf("%s:%s:OK! shmget [%d] \n", __FILE__, __FUNCTION__, shmid );
		}
		else
		{
			printf("%s:%s:ERROR! shmget fail [%d] \n", __FILE__, __FUNCTION__, shmid );
			/*
			perror( "ERROR!! " );
			*/ 
		}
	}
	else
		printf("%s:%s:ERROR! ftok fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


static int SharedMemoryIDinit( int shmid, char **This )
{
	int iRet = 0;
	void *pV = (void *)-1;
	
	if( (shmid > -1) && This )
	{
		/* attach memory address */
		pV = shmat( shmid, NULL, 0 );
		if( pV == NULL )
			printf("%s:%s:ERROR! shmat fail\n", __FILE__, __FUNCTION__ );
		else
		{
			printf("%s:%s:OK! shmat [0x%x]", __FILE__, __FUNCTION__, *This );
			*This = (char *)pV;
			printf("->[0x%x]\n", *This );
		}
	}
	else
		printf("%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}


static int SharedMemoryIDdeinit( char *This )
{
	int iRet = -1;
	
	if( This )
	{
		/* de-attach memory address */
		iRet = shmdt( This );
		if( iRet == -1 )
			printf("%s:%s:ERROR! shmdt [0x%x] fail\n", __FILE__, __FUNCTION__, This );
	}
	else
		printf("%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


static int SharedMemoryIDdestroy( int shmid )
{
	int iRet = 0;
	
	if( shmid > -1 )
	{
		iRet = shmctl( shmid, IPC_RMID, NULL );
		if( iRet == -1 )
			printf("%s:%s:ERROR! shmctl fail\n", __FILE__, __FUNCTION__ );
	}
	else
		printf("%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
	return iRet;
}











static void* print_chars( void* in )
{
	struct char_print_parms args;
	
	args.character = 'x';
	args.count = 1000;

	if( *(char*)in > 0 )
		args.character = *(char*)in;

#ifdef USE_SEMAPHOE
	semLOCK( iSemaphore );
#endif
	/* call library */
	lib_char_print( &args );
#ifdef USE_SEMAPHOE
	semUNLOCK( iSemaphore );
#endif

	return NULL;
}


static void* print_xs( void* unused)
{
	char cshow;

	cshow = 'x';

	if(0)
	{
		char *pChar;
		
		SharedMemoryIDinit( iSharedMemory, &pChar );
		
		strncpy( pChar, "This is a test string!", 24 );
		pChar[0] = 'a';
		pChar[1] = 'b';
		pChar[2] = 'c';

		SharedMemoryIDdeinit( pChar );
	}
	
	print_chars( &cshow );

	if(1)
	{
		char *pChar;
		
		SharedMemoryIDinit( iSharedMemory, &pChar );
		
		printf( "==>%s [%c][%c][%c]\n", pChar, pChar[0], pChar[1], pChar[2] );

		SharedMemoryIDdeinit( pChar );
	}


	return NULL;
}


static void* print_ys( void* unused)
{
	char cshow;

	cshow = 'y';

	if(1)
	{
		char *pChar;
		
		SharedMemoryIDinit( iSharedMemory, &pChar );
		
		strncpy( pChar, "This is a test string!", 24 );
		pChar[0] = 'a';
		pChar[1] = 'b';
		pChar[2] = 'c';

		SharedMemoryIDdeinit( pChar );
	}

	print_chars( &cshow );

	if(0)
	{
		char *pChar;
		
		SharedMemoryIDinit( iSharedMemory, &pChar );
		
		printf( "==>%s [%c][%c][%c]\n", pChar, pChar[0], pChar[1], pChar[2] );
		
		sleep(1);
		SharedMemoryIDdeinit( pChar );
	}

	return NULL;
}


int main()
{
	int return_value = 0;
	pthread_t thread_id;

	#if 0
	iSemaphore = getSemaphoresID();
	#else
	iSemaphore = getNamedSemaphoresID( "/dev/null" );
	#endif
	SemaphoresIDinit( iSemaphore );

#define	SHM_SIZE	(1024*4)
	iSharedMemory = getNamedSharedMemoryID( "/dev/zero", SHM_SIZE );


#if 0	
	return_value = system("ls -al ./");

	return_value = system("cat /proc/version");
	return_value = system("cat /proc/devices");
	return_value = system("cat /proc/driver/rtc | grep rtc_time");
	return_value = system("cat /proc/driver/rtc | grep rtc_date");
	return_value = system("cat /proc/driver/rtc | grep alrm_time");
	return_value = system("cat /proc/driver/rtc | grep alrm_date");

	return_value = system("cat /proc/cpuinfo | grep processor");
	return_value = system("cat /proc/cpuinfo | grep MHz");
#endif

	printf("The process ID is %d\n", (int)getpid() );
	printf("The parent process ID is %d\n", (int)getppid() );

	pthread_create( &thread_id, NULL, &print_xs, NULL );

	print_ys( NULL );


	sleep(1);
#ifdef USE_SEMAPHOE
	semLOCK( iSemaphore );
#endif
	if(1)
	{
		char *pChar;
		
		SharedMemoryIDinit( iSharedMemory, &pChar );
		
		strncpy( pChar, "This is a test string!", 24 );
		pChar[0] = 'T';
		pChar[1] = 'h';
		pChar[2] = 'a';
		pChar[3] = 't';
		
		#if 1
		printf( "==>%s [%c][%c][%c][%c]\n", pChar, pChar[0], pChar[1], pChar[2], pChar[3] );
		#else
		printf( "==>[%c][%c][%c][%c]\n", pChar[0], pChar[1], pChar[2], pChar[3] );
		#endif
		SharedMemoryIDdeinit( pChar );
		
		///SharedMemoryIDinit( iSharedMemory, pChar );
		///printf( "==>%s [%c][%c][%c][%c]\n", pChar, pChar[0], pChar[1], pChar[2], pChar[3] );
		///SharedMemoryIDdeinit( pChar );
	}
#ifdef USE_SEMAPHOE
	semUNLOCK( iSemaphore );
#endif

	sleep(1);
#ifdef USE_SEMAPHOE
	semLOCK( iSemaphore );
#endif
	if(1)
	{
		char *pChar2;
		
		SharedMemoryIDinit( iSharedMemory, &pChar2 );
		#if 1
		printf( "==>%s [%c][%c][%c][%c]\n", pChar2, pChar2[0], pChar2[1], pChar2[2], pChar2[3] );
		#else
		printf( "==>[%c][%c][%c][%c]\n", pChar2[0], pChar2[1], pChar2[2], pChar2[3] );
		#endif
		
		SharedMemoryIDdeinit( pChar2 );
	}
#ifdef USE_SEMAPHOE
	semUNLOCK( iSemaphore );
#endif


	sleep(1); /* leave some time for thread */
	SemaphoresIDdestroy( iSemaphore );
	SharedMemoryIDdestroy(iSharedMemory );

	printf( "\ndone \n" );
	return 0;
}


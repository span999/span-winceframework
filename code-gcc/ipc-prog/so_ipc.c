/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>




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
			printf("%s:%s:ERROR! semctl fail\n", __FILE__, __FUNCTION__ );
	}
	else
		printf("%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
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
			printf("%s:%s:ERROR! semctl fail\n", __FILE__, __FUNCTION__ );
	}
	else
		printf("%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
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
			printf("%s:%s:ERROR! semctl fail\n", __FILE__, __FUNCTION__ );
	}
	else
		printf("%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
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
			printf("%s:%s:ERROR! shmat fail [0x%x]->[0x%x]\n", __FILE__, __FUNCTION__, *This, pV );
			iRet = -1;
		}
		else
		{
			printf("%s:%s:OK! shmat [0x%x]\n", __FILE__, __FUNCTION__, *This );
			*This = (char *)pV;
			printf("->[0x%x]\n", *This );
		}
	}
	else
		printf("%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
	
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
			printf("%s:%s:ERROR! shmdt [0x%x] fail\n", __FILE__, __FUNCTION__, This );
	}
	else
		printf("%s:%s:ERROR! fail\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


int SharedMemoryIDdestroy( int shmid )
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

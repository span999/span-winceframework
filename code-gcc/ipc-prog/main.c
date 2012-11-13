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
#include <sys/ipc.h>

#include "so_test.h"


static int iSemaphore = 0;


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
	
	print_chars( &cshow );

	return NULL;
}


static void* print_ys( void* unused)
{
	char cshow;

	cshow = 'y';
	
	print_chars( &cshow );

	return NULL;
}


int main()
{
	int return_value = 0;
	pthread_t thread_id;

	iSemaphore = getSemaphoresID();
	SemaphoresIDinit( iSemaphore );

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

	///sleep(1);

	print_ys( NULL );

	sleep(1); /* leave some time for thread */
	SemaphoresIDdestroy( iSemaphore );
	printf( "\ndone \n" );
	return 0;
}


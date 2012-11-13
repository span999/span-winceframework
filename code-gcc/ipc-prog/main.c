/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <sys/time.h>



#include "so_test.h"
#include "so_ipc.h"



static int iSemaphore = 0;
static int iSharedMemory = 0;





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


	sleep(1); /* leave some time for thread */
	SemaphoresIDdestroy( iSemaphore );
	SharedMemoryIDdestroy(iSharedMemory );

	printf( "\ndone \n" );
	return 0;
}


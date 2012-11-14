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



static int iSemaphore = 0;
static int iSharedMemory = 0;




static int SendData( char *pData, unsigned int len )
{
	int iRet = -1;
	
	
	return iRet;
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




	printf("The process ID is %d\n", (int)getpid() );
	printf("The parent process ID is %d\n", (int)getppid() );




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


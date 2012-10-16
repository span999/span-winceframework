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



static void* print_chars( void* in )
{
	struct char_print_parms args;
	
	args.character = 'x';
	args.count = 100;

	if( *(char*)in > 0 )
		args.character = *(char*)in;

	/* call library */
	lib_char_print( &args );

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
	
	return_value = system("ls -al ./");

	printf("The process ID is %d\n", (int)getpid() );
	printf("The parent process ID is %d\n", (int)getppid() );

	pthread_create( &thread_id, NULL, &print_xs, NULL );

	///sleep(1);

	print_ys( NULL );

	printf( "\ndone \n" );
	return 0;
}


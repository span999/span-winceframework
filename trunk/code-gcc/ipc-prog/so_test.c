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






/* 
Prints a number of characters to stderr, as given by PARAMETERS,
which is a pointer to a struct char_print_parms. 
*/
void* lib_char_print( void* parameters )
{
	/* Cast the cookie pointer to the right type. */
	struct char_print_parms* p = (struct char_print_parms*)parameters;
	int i;

	for( i = 0; i < p->count; ++i )
		fputc( p->character, stderr );
	
	return NULL;
}



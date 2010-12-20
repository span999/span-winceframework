/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void banner( void );
void millisleep( int milliseconds );
void rollingsign( void );
void pointssign( void );
void counterup( int iCnt );



int main( int argc, char *argv[] )
{
	int iRet = 0;
	int iCount = 0;

/*
#define COUNTTOP 100
*/
#define COUNTTOP 5

	banner();

	while( iCount < COUNTTOP )
	{
		/*
		///printf(".");
		///banner();
		///rollingsign();
		*/
		pointssign();
		/*
		///counterup( 100 );
		*/
		iCount++;
		/*
		///usleep(100000);
		*/
		millisleep( 1000 );
	}
	printf("\r\n");


	return iRet;
}



void millisleep( int milliseconds )
{
	int iRet = 0;
	iRet = usleep( (unsigned int)(milliseconds * 1000) );
}


void banner( void )
{

	printf("\r\n");
	printf("   =======================\r\n");
	printf("   This is a test program.\r\n");
	printf("   =======================\r\n");
	printf("\r\n");
	fprintf(stderr,"#");
}


/*
///#define M100M millisleep( 100 ); printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r");
///#define M100M printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"); millisleep( 200 );
*/
#define M100M millisleep( 200 ); printf("\r"); fflush(stdout);

void rollingsign( void )
{
	printf("\\");
	M100M
	printf("\r");
	printf("--");
	M100M
	printf("\r");
	printf("/");
	M100M
	printf("\r");
	printf("|");
	M100M
	printf("\r");
}



void pointssign( void )
{
	printf(".........."); fflush(stdout);
	M100M
	printf("\r                                         \r");
	printf("...................."); fflush(stdout);
	M100M
	printf("\r                                         \r");
	printf(".............................."); fflush(stdout);
	M100M
	printf("\r                                         \r");
	printf("........................................"); fflush(stdout);
	M100M
	printf("\r                                         \r");
	M100M
}


void counterup( int iTop )
{
	int iCnt = 0;
	for( iCnt = 0; iCnt < iTop; iCnt++ )
	{
		printf("%d", iCnt);
		printf("\r");
	}
	printf("\r\n");
}

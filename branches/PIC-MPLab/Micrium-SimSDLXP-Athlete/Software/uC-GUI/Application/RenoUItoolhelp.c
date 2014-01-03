
/*
	body of tool help functions. non-uc-GUI related
*/

#ifdef RENOUI_PROJ
/* ********************************************************* */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


int spGetRandNum( int iBtm, int iTop )
{
	static int Sc = -1;
	int iRet = 0;
	
	if( -1 == Sc )
		Sc = iBtm;
	if( iTop == Sc )
		Sc = iBtm;

	Sc = rand() % iTop;
	iRet = Sc;
	Sc++;
	
	return iRet;
}


int IsSamekey( int iSrc, int iTar )
{
	if( iSrc == 'o' )
		printf("  UP key hold~~\n");
	else
	if( iSrc == 'i' )
		printf("  UP key press~~\n");
	else
	if( iSrc == 'l' )
		printf("  DOWN key hold~~\n");
	else
	if( iSrc == 'k' )
		printf("  DOWN key press~~\n");
	else
	if( iSrc == 'y' )
		printf("  BACK key hold~~\n");
	else
	if( iSrc == 'u' )
		printf("  BACK key press~~\n");
	else
	if( iSrc == 'h' )
		printf("  ENTER key hold~~\n");
	else
	if( iSrc == 'j' )
		printf("  ENTER key press~~\n");
	else
		printf("  unknow key press~~!!\n");
	
	return (iSrc == iTar)?1:0;		
} 

#define	DEBUGMSG_CONSOLE
void SPPRINTF( char* pString, ... )
{
	va_list ap;
	char buf[128];

#ifdef DEBUGMSG_FILE
	phDebugmsgfile = fopen( DEBUGMSG_FILENAME, "a" );
#endif

	va_start( ap, pString );

	vsprintf( buf, pString, ap );

#ifdef DEBUGMSG_CONSOLE
	printf( buf );
#endif
#ifdef DEBUGMSG_FILE
	fprintf( phDebugmsgfile, buf );
#endif

	va_end( ap );


#ifdef DEBUGMSG_FILE
	fclose( phDebugmsgfile );
#endif


}













/* ********************************************************* */
#endif	/* RENOUI_PROJ */

#include "c-common.h"
#include <pthread.h>

static bool bDataParsor_thread_done = false;
static bool bGetString_thread_done = false;

long GetMicroSec()
{
	int iRet;
	long lRet = 0;
	static struct timeval stv;
	struct timeval tv;

	DBGMSG("SPD@GetMicroSec +++\n");

	iRet = gettimeofday( &tv, NULL );
	if( (0 == stv.tv_sec) && (0 == stv.tv_usec) )
	{
		stv.tv_sec = tv.tv_sec;
		stv.tv_usec = tv.tv_usec;
	}
	else
	{
		DBGMSG("SPD@GetMicroSec Sec=%ld \n", tv.tv_sec-stv.tv_sec );
		DBGMSG("SPD@GetMicroSec uSec=%ld \n", tv.tv_usec-stv.tv_usec );
		lRet = (tv.tv_sec-stv.tv_sec)*1000000 + (tv.tv_usec-stv.tv_usec);

		stv.tv_sec = tv.tv_sec;
		stv.tv_usec = tv.tv_usec;
	}

	DBGMSG("SPD@GetMicroSec ---%ld\n", lRet );
	return lRet;
}


static void* Thread_GetString( void* cookie )
{
	int iRet = 0;
	int iLoop = 0;
	unsigned upTemp;
	long ltime = 0;
	long rtime = 0;

	DBGMSG("SPD@Thread_GetString +++\n");
	GetMicroSec();

	for( iLoop = 0; iLoop < 30; iLoop++ )
	{
		///ask for data string
		iRet = GetRandomStr( &upTemp );
		INFOMSG( "SPD@GetString:%04d-total:%d=>%s\n", iLoop, iRet, (char *)upTemp );
		///parsor
		DataStore( (unsigned char *)upTemp );
		///get micro sec till now
		ltime = GetMicroSec();
		///count char rate 
		rtime = ((1000000 * iRet) / BYTERATE_SEC);
		INFOMSG( "SPD@GetString:%ld %ld\n", ltime, rtime );
		///sleep for certain 
		usleep( (rtime>ltime)?(rtime-ltime):0 );
		if( bGetString_thread_done )
			break;
	}	


	DBGMSG("SPD@Thread_GetString ---\n");
	return NULL;
}


static void* Thread_DataParsor( void* cookie )
{
	int iRet = 0;
	int iLoop = 0;
	unsigned upTemp;

	DBGMSG("SPD@Thread_DataParsor +++\n");
	
	for( iLoop = 0; iLoop < 40; iLoop++ )
	{
		iRet = GetCurrArraySize();
		if( iRet > 0 )
		{
			INFOMSG( "SPD@Thread_DataParsor-%04d %04d\n", iLoop, iRet );
			DataCarry();
			///usleep(100000);
		}
		else
		{
			INFOMSG( "SPD@Thread_DataParsor- NO DATA !!!! -%04d %04d\n", iLoop, iRet );
			usleep(100000);
		}
		///DumpArray();
		///DumpArrayStatus();
		if( bDataParsor_thread_done )
			break;
	}	


	DBGMSG("SPD@Thread_DataParsor ---\n");
	return NULL;
}



///int main( int argc, char **argv )
int main123()
{
	int iRet = 0;
	unsigned	upTemp = 0;

	pthread_t t;
	
	DBGMSG("SPD@main +++\n");
	
	GetMicroSec();
///	INFOMSG( "Hello !!!%d %s\n", iRet, cTagIHRtmp );

	pthread_create( &t, NULL, Thread_GetString, NULL );
	sleep(1);
	pthread_create( &t, NULL, Thread_DataParsor, NULL );
	sleep(1);

	
	while( 0x78 != iRet )
	{ 
		INFOMSG("Input 'x' key to stop! -0x%x \n", iRet );
		iRet = getchar();
		upTemp++;
		if( upTemp > 1000)
			break;
	}

	///DumpArray();
	DumpArrayStatus();
	///sleep(2);
	
	CloseLogfile();
	DBGMSG("SPD@main %d \n", 31%32 );
	DBGMSG("SPD@main ---\n");
    return iRet;
}

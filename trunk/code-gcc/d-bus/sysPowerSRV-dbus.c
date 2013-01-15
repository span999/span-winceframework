/*
 *
 *
 */

#define DBUS_API_SUBJECT_TO_CHANGE
#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//#include <pthread.h>
//#include <string.h>

#include "toolhelps.h"
//#include "ipcpacket.h"
//#include "sysIPCSRV.h"
//#include "sysPowerCOMM.h"
//#include "sysPowerSRV.h"



/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
#if 1
	#define	DBGFSET		(dDBG|dINFO|dERR)
#else
	#define	DBGFSET		(dINFO|dERR)
#endif
#define	dF(x)		(DBGFSET&x)




#define DBUSSRV_METHOD_SERVER		"test.method.server"
#define DBUSSRV_METHOD_OBJECT		"/test/method/Object"
#define DBUSSRV_METHOD_TYPE			"test.method.Type"


#define spMxON \
		spMxI( &mutex, &mutexINITED ); \
		spMxL( &mutex, &mutexINITED );
		
#define spMxOFF \
		spMxU( &mutex, &mutexINITED ); \
		spMxD( &mutex, &mutexINITED );

/*
	these function routine should be placed at sysPowerSRV.so / sysPowerSRV.a
*/






/* for mutex */
static pthread_mutex_t mutex;
static int mutexINITED = 0;




int dbus_method_call( int *piRet, int iIn1, char *pFn )
{
	int iRet = -1;
	
	spMSG( dF(dINFO), "%s:%s: pFn=%s iIn1=%d\n", __FILE__, __FUNCTION__, pFn, iIn1 );
	
	return iRet;
}



int sPSgetCPUActivatedNum( void )
{
	int iRet = -1;
	int dRet = -1;
	int iIn1 = 0;

	spMxON
	
	dRet = dbus_method_call( &iRet, iIn1, (char *)__FUNCTION__ ); 

	spMxOFF
	
	return iRet;
}


int sPSsetCPUActivatedNum( int num )
{
	int iRet = -1;
	int dRet = -1;
	int iIn1 = num;

	spMxON
	
	dRet = dbus_method_call( &iRet, iIn1, (char *)__FUNCTION__ ); 

	spMxOFF
	
	return iRet;
}


int sPSsetCPUDVFS( int num )
{
	int iRet = -1;
	int dRet = -1;
	int iIn1 = num;

	spMxON
	
	dRet = dbus_method_call( &iRet, iIn1, (char *)__FUNCTION__ ); 

	spMxOFF
	
	return iRet;
}


int sPSsetCPUspeed( int nMHz )
{
	int iRet = -1;
	int dRet = -1;
	int iIn1 = nMHz;

	/* set limitation here */
	if( (nMHz != 400) && (nMHz != 800) && (nMHz != 1000) )
	{
		spMSG( dF(dERR), "%s:%s: parameter invalid [%d] !!\n", __FILE__, __FUNCTION__, nMHz );
		iRet = -2;
		return iRet;
	}

	spMxON
	
	dRet = dbus_method_call( &iRet, iIn1, (char *)__FUNCTION__ ); 
	
	spMxOFF

	return iRet;
}


int sPSsetCPUsuspend( void )
{
	int iRet = -1;
	int dRet = -1;
	int iIn1 = 0;

	spMxON
	
	dRet = dbus_method_call( &iRet, iIn1, (char *)__FUNCTION__ ); 

	spMxOFF
	
	return iRet;
}


int sPSloopbackTest( int test )
{
	int iRet = -1;
	int dRet = -1;
	int iIn1 = test;

	spMxON
	
	dRet = dbus_method_call( &iRet, iIn1, (char *)__FUNCTION__ ); 

	spMxOFF
	
	if( iRet != test )
		spMSG( dF(dERR), "%s:%s: fail !!\n", __FILE__, __FUNCTION__ );
		
	return iRet;
}


int sysPowerSRVInit( void )
{
	spMxI( &mutex, &mutexINITED );
	return 0;
}


int sysPowerSRVDeinit( void )
{
	spMxD( &mutex, &mutexINITED );
	return 0;
}



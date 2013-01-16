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
		spMxL( &mutex, &mutexINITED );
		
#define spMxOFF \
		spMxU( &mutex, &mutexINITED );



struct sysPowerCallMethod
{
	int				iID;			/* id */
	int				iType;			/* method type */
	char*			userName;		/* user function name */
	char*			dbusName;		/* dbus function name */
};

static struct sysPowerCallMethod methodList[] = {
	{	0,	0,	"sPSgetCPUActivatedNum", "getCPUcoreActivatedNumber"	},
	{	1,	0,	"sPSsetCPUActivatedNum", "setCPUcoreActivatedNumber"	},
	{	2,	0,	"sPSsetCPUDVFS", "setCPUdvfs"	},
	{	3,	0,	"sPSsetCPUspeed", "setCPUspeed"	},
	{	4,	0,	"sPSsetCPUsuspend", "setCPUsuspend"	},
	{	-1,	-1,	NULL, NULL	}
};


/*
	these function routine should be placed at sysPowerSRV.so / sysPowerSRV.a
*/


/* for mutex */
static pthread_mutex_t mutex;
static int mutexINITED = 0;



static char **getDbusNameFromUserName( char *UserName )
{
	char **pDbusName = NULL;
	int iTmp = 0;
	
	for( iTmp = 0; iTmp != -1; iTmp++ )
	{
		if( 0 == strcmp( methodList[iTmp].userName, UserName) )
		{
			pDbusName = &methodList[iTmp].dbusName;
			spMSG( dF(dINFO), "%s:%s: user function name [%s] found!\n", __FILE__, __FUNCTION__, UserName );
			spMSG( dF(dINFO), "%s:%s: mapping function name [%s]\n", __FILE__, __FUNCTION__, *pDbusName );
			break;
		}
	}

	return pDbusName;
}


int dbus_method_call_ex( int *piRet, int iIn1, char *pFn )
{
	int iRet = -1;
	DBusMessage* msg;
	DBusMessageIter args;
	DBusConnection* conn;
	DBusError err;
	DBusPendingCall* pending;
	int ret;
	bool stat;
	dbus_int32_t level;

	printf("Calling remote method with %s(%d)\n", pFn, iIn1 );

	// initialiset the errors
	dbus_error_init(&err);

	// connect to the system bus and check for errors
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) { 
		fprintf(stderr, "Connection Error (%s)\n", err.message); 
		dbus_error_free(&err);
	}
	if (NULL == conn) { 
		exit(1); 
	}

	// request our name on the bus
	ret = dbus_bus_request_name(conn, "test.method.caller", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
	if (dbus_error_is_set(&err)) { 
		fprintf(stderr, "Name Error (%s)\n", err.message); 
		dbus_error_free(&err);
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
		exit(1);
	}

	// create a new method call and check for errors
	msg = dbus_message_new_method_call(DBUSSRV_METHOD_SERVER, // target for the method call
										DBUSSRV_METHOD_OBJECT, // object to call on
										DBUSSRV_METHOD_TYPE, // interface to call on
										pFn); // method name
									
	/*
	 * # dbus-send --system --print-reply --type=method_call --dest='test.method.server' /test/method/Object 'test.method.Type.Method' string:’hello world’
	 * 
	     dbus-send --dest=org.freedesktop.ExampleName               \
                   /org/freedesktop/sample/object/name              \
                   org.freedesktop.ExampleInterface.ExampleMethod   \
                   int32:47 string:’hello world’ double:65.32       \
                   array:string:"1st item","next item","last item"  \
                   dict:string:int32:"one",1,"two",2,"three",3      \
                   variant:int32:-8                                 \
                   objpath:/org/freedesktop/sample/object/name
     *
	 */
	if (NULL == msg) { 
		fprintf(stderr, "Message Null\n");
		exit(1);
	}

	// append arguments
	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &iIn1)) {
		fprintf(stderr, "Out Of Memory!\n"); 
		exit(1);
	}
   
	// send message and get a handle for a reply
	if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
		fprintf(stderr, "Out Of Memory!\n"); 
		exit(1);
	}
	if (NULL == pending) { 
		fprintf(stderr, "Pending Call Null\n"); 
		exit(1); 
	}
	dbus_connection_flush(conn);
   
	printf("Request Sent\n");
   
	// free message
	dbus_message_unref(msg);
   
	// block until we recieve a reply
	dbus_pending_call_block(pending);

	// get the reply message
	msg = dbus_pending_call_steal_reply(pending);
	if (NULL == msg) {
		fprintf(stderr, "Reply Null\n"); 
		exit(1); 
	}
	// free the pending message handle
	dbus_pending_call_unref(pending);

	// read the parameters
	if (!dbus_message_iter_init(msg, &args))
		fprintf(stderr, "Message has no arguments!\n"); 
	else if (DBUS_TYPE_BOOLEAN != dbus_message_iter_get_arg_type(&args)) 
		fprintf(stderr, "Argument is not boolean!\n"); 
	else
		dbus_message_iter_get_basic(&args, &stat);

	if (!dbus_message_iter_next(&args))
		fprintf(stderr, "Message has too few arguments!\n"); 
	else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) 
		fprintf(stderr, "Argument is not int32!\n"); 
	else
		dbus_message_iter_get_basic(&args, &level);

	printf("Got Reply: %s, %d\n", (stat>0)?"true":"false", level);
   
	// free reply and close connection
	dbus_message_unref(msg);   
	dbus_connection_close(conn);

	
	return iRet;
}


int dbus_method_call( int *piRet, int iIn1, char *pFn )
{
	int iRet = -1;
	char **pDbusName = NULL;
	
	spMSG( dF(dINFO), "%s:%s: pFn=%s iIn1=%d\n", __FILE__, __FUNCTION__, pFn, iIn1 );
	pDbusName = getDbusNameFromUserName( pFn );
	
	if( NULL != pDbusName )
	{
		spMSG( dF(dINFO), "%s:%s: use function name [%s]\n", __FILE__, __FUNCTION__, *pDbusName );
		dbus_method_call_ex( piRet, iIn1, *pDbusName );
	}
	else
		spMSG( dF(dERR), "%s:%s: mapping function name NOT found!!\n", __FILE__, __FUNCTION__ );
	
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



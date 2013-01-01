/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#if 0
#include <dbus/dbus-glib.h>
#endif

	

static DBusConnection* g_conn = NULL;

static DBusConnection* getDBusConn( void )
{
	if( NULL != g_conn )
		return g_conn;
	else
		return NULL;
} 


static DBusConnection** setDBusConn( void )
{
	return &g_conn;
} 


int _dbus_init( void )
{
	int iRet = -1;
	DBusError err;
	int ret;

	printf("SPD@%s\n", __FUNCTION__);
	// initialise the errors
	dbus_error_init(&err);


	// connect to the bus
   	*setDBusConn() = dbus_bus_get(DBUS_BUS_SESSION, &err);
   	if (dbus_error_is_set(&err)) { 
      		fprintf(stderr, "Connection Error (%s)\n", err.message); 
      		dbus_error_free(&err); 
  	}
   	if (NULL == getDBusConn()) { 
      		exit(1); 
   	}

	// request a name on the bus
   	ret = dbus_bus_request_name(getDBusConn(), "test.method.server", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
   	if (dbus_error_is_set(&err)) { 
      		fprintf(stderr, "Name Error (%s)\n", err.message); 
      		dbus_error_free(&err); 
   	}
   	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
      		exit(1);
   	}


	return iRet;
}


int _dbus_deinit( void )
{
	int iRet = -1;

	printf("SPD@%s\n", __FUNCTION__);

	// dis-connect to the bus
	dbus_connection_close(getDBusConn());

	return iRet;
}


int send_sig( void )
{
	dbus_uint32_t serial = 0; // unique number to associate replies with requests
	DBusMessage* msg;
	DBusMessageIter args;
	char sigstr[] = "123456789";
	void *sigvalue = NULL;
     
	sigvalue = sigstr;
	 
	// create a signal and check for errors 
	msg = dbus_message_new_signal("/test/signal/Object", // object name of the signal
								"test.signal.Type", // interface name of the signal
								"Test"); // name of the signal
	if (NULL == msg) 
	{ 
		fprintf(stderr, "Message Null\n"); 
		exit(1); 
	}

	// append arguments onto signal
	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &sigvalue)) { 
		fprintf(stderr, "Out Of Memory!\n"); 
		exit(1);
	}

	// send the message and flush the connection
	if (!dbus_connection_send(getDBusConn(), msg, &serial)) { 
		fprintf(stderr, "Out Of Memory!\n"); 
		exit(1);
	}
	dbus_connection_flush(getDBusConn());
   
	// free the message 
	dbus_message_unref(msg);

}


int main( int argc, char *argv[] )
{
	int iRet = -1;
//	DBusConnection* conn;

	printf("SPD@%s +++\n", __FUNCTION__);

	_dbus_init();


	_dbus_deinit();

	printf("SPD@%s ---\n", __FUNCTION__);
	
	return iRet;
}



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
static int iType = 0;

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
	 
	printf("SPD@%s\n", __FUNCTION__);
	// create a signal and check for errors 
	msg = dbus_message_new_signal("/test/signal/Object", // object name of the signal
								"test.signal.Type", // interface name of the signal
								"Test"); // name of the signal
	if (NULL == msg) 
	{ 
		fprintf(stderr, "Message Null\n");
		printf("SPD@%s Message Null\n", __FUNCTION__);
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

#ifndef true
	#define	true 1
#endif
int wait_sig( void )
{
	DBusError err;
	dbus_uint32_t serial = 0; // unique number to associate replies with requests
	DBusMessage* msg;
	DBusMessageIter args;
	char sigstr[] = "123456789";
	void *sigvalue = NULL;
     
	sigvalue = sigstr;
	 
	printf("SPD@%s\n", __FUNCTION__);
	// add a rule for which messages we want to see
	dbus_bus_add_match(getDBusConn(), 
					"type='signal',interface='test.signal.Type'", 
					&err); // see signals from the given interface
	
	dbus_connection_flush(getDBusConn());
	if (dbus_error_is_set(&err)) { 
		fprintf(stderr, "Match Error (%s)\n", err.message);
		///printf("SPD@%s Match Error (%s)\n", __FUNCTION__, err.message);
		exit(1); 
	}

	// loop listening for signals being emmitted
	while (true) {

		// non blocking read of the next available message
		dbus_connection_read_write(getDBusConn(), 0);
		msg = dbus_connection_pop_message(getDBusConn());

		// loop again if we haven't read a message
		if (NULL == msg) { 
			sleep(1);
			continue;
		}

		// check if the message is a signal from the correct interface and with the correct name
		if (dbus_message_is_signal(msg, "test.signal.Type", "Test")) {
			// read the parameters
			if (!dbus_message_iter_init(msg, &args))
			{
				fprintf(stderr, "Message has no arguments!\n");
				///printf("SPD@%s Message has no arguments!\n", __FUNCTION__);
			}
			else
			if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
			{
				fprintf(stderr, "Argument is not string!\n"); 
				///printf("SPD@%s Argument is not string!\n", __FUNCTION__);
			}
			else 
			{
				dbus_message_iter_get_basic(&args, &sigvalue);
				///printf("Got Signal with value %s\n", (char *)sigvalue);
			}
		}

		// free the message
		dbus_message_unref(msg);
	}

}


int main( int argc, char *argv[] )
{
	int iRet = -1;
//	DBusConnection* conn;

	printf("SPD@%s +++\n", __FUNCTION__);


	if( argc > 1 )
	{
		if( (0 == strcmp("-host", argv[1])) )
		{
			printf("SPD@%s parameter! [%s] \n", __FUNCTION__, argv[1]);
			iType = 1;
		}
		else
		if( (0 == strcmp("-client", argv[1])) )
		{
			printf("SPD@%s parameter! [%s] \n", __FUNCTION__, argv[1]);
			iType = 2;
		}	
		else
		{
			printf("SPD@%s unknow parameter! [%s] \n", __FUNCTION__, argv[1]);
			goto _EXIT;
		}	
	}
	else
	{
		printf("SPD@%s parameter needed![%d] \n", __FUNCTION__, argc);
		goto _EXIT;
	}

	_dbus_init();

	if( 1 == iType )
	{
		wait_sig();
	}
	else
	if( 2 == iType )
	{
		send_sig();
		
	}

	_dbus_deinit();

	printf("SPD@%s ---\n", __FUNCTION__);

_EXIT:	
	return iRet;
}



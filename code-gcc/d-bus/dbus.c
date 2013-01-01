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

	

static DBusConnection* g_conn;




int _dbus_init( void )
{
	int iRet = -1;
	DBusError err;
	int ret;

	printf("SPD@%s\n", __FUNCTION__);
	// initialise the errors
	dbus_error_init(&err);


	// connect to the bus
   	g_conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
   	if (dbus_error_is_set(&err)) { 
      		fprintf(stderr, "Connection Error (%s)\n", err.message); 
      		dbus_error_free(&err); 
  	}
   	if (NULL == g_conn) { 
      		exit(1); 
   	}

	// request a name on the bus
   	ret = dbus_bus_request_name(g_conn, "test.method.server", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
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
	dbus_connection_close(g_conn);

	return iRet;
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



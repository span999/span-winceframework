#define DBUS_API_SUBJECT_TO_CHANGE
#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "sysPowerSRV-dbus.h"
#include "sysPowerMethod-dbus.h"




/**
 * Connect to the DBUS bus and send a broadcast signal
 */
void sendsignal(char* sigvalue)
{
	DBusMessage* msg;
	DBusMessageIter args;
	DBusConnection* conn;
	DBusError err;
	int ret;
	dbus_uint32_t serial = 0;

	printf("Sending signal with value %s\n", sigvalue);

	// initialise the error value
	dbus_error_init(&err);

	// connect to the DBUS system bus, and check for errors
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) { 
		fprintf(stderr, "Connection Error (%s)\n", err.message); 
		dbus_error_free(&err); 
	}
	if (NULL == conn) { 
		exit(1); 
	}

	// register our name on the bus, and check for errors
	ret = dbus_bus_request_name(conn, "test.signal.source", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
	if (dbus_error_is_set(&err)) { 
		fprintf(stderr, "Name Error (%s)\n", err.message); 
		dbus_error_free(&err); 
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
		exit(1);
	}

	// create a signal & check for errors 
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
   if (!dbus_connection_send(conn, msg, &serial)) {
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
   }
   dbus_connection_flush(conn);
   
   printf("Signal Sent\n");
   
   // free the message and close the connection
   dbus_message_unref(msg);
   dbus_connection_close(conn);
}

/**
 * Call a method on a remote object
 */
void query(char* param) 
{
   DBusMessage* msg;
   DBusMessageIter args;
   DBusConnection* conn;
   DBusError err;
   DBusPendingCall* pending;
   int ret;
   bool stat;
   dbus_uint32_t level;

   printf("Calling remote method with %s\n", param);

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
   msg = dbus_message_new_method_call("test.method.server", // target for the method call
                                      "/test/method/Object", // object to call on
                                      "test.method.Type", // interface to call on
                                      "Method"); // method name
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
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &param)) {
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
   else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) 
      fprintf(stderr, "Argument is not int!\n"); 
   else
      dbus_message_iter_get_basic(&args, &level);

   printf("Got Reply: %d, %d\n", stat, level);
   
   // free reply and close connection
   dbus_message_unref(msg);   
   dbus_connection_close(conn);
}


/**
 * Call a method on a remote object
 */
void calls( int iID, int iParam ) 
{
   DBusMessage* msg;
   DBusMessageIter args;
   DBusConnection* conn;
   DBusError err;
   DBusPendingCall* pending;
   int ret;
   bool stat;
   dbus_int32_t level;

   printf("Calling remote method with %d:%d\n", iID, iParam );

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

	switch( iID )
	{
		case 0:
	// create a new method call and check for errors
	msg = dbus_message_new_method_call(DBUSSRV_METHOD_SERVER, // target for the method call
										DBUSSRV_METHOD_OBJECT, // object to call on
										DBUSSRV_METHOD_TYPE, // interface to call on
										"Method"); // method name
			break;
		
		case 1:
	// create a new method call and check for errors
	msg = dbus_message_new_method_call(DBUSSRV_METHOD_SERVER, // target for the method call
										DBUSSRV_METHOD_OBJECT, // object to call on
										DBUSSRV_METHOD_TYPE, // interface to call on
										"m_CPUcoreUP"); // method name
			break;
			
		case 2:
	// create a new method call and check for errors
	msg = dbus_message_new_method_call(DBUSSRV_METHOD_SERVER, // target for the method call
										DBUSSRV_METHOD_OBJECT, // object to call on
										DBUSSRV_METHOD_TYPE, // interface to call on
										"m_CPUcoreDOWN"); // method name
			break;
		
		default:
			break;
	}
									
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
   if (!dbus_message_iter_append_basic(&args, (iID==0)?DBUS_TYPE_STRING:DBUS_TYPE_INT32, &iParam)) {
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
}


void reply_to_method_call(DBusMessage* msg, DBusConnection* conn)
{
   DBusMessage* reply;
   DBusMessageIter args;
   bool stat = true;
   dbus_uint32_t level = 21614;
   dbus_uint32_t serial = 0;
   char* param = "";

   // read the arguments
   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n"); 
   else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
      fprintf(stderr, "Argument is not string!\n"); 
   else 
      dbus_message_iter_get_basic(&args, &param);

   printf("Method called with %s\n", param);

   // create a reply from the message
   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &stat)) { 
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
   }
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &level)) { 
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
   }

   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);
}

/**
 * Server that exposes a method call and waits for it to be called
 */
void listen() 
{
   DBusMessage* msg;
   DBusMessage* reply;
   DBusMessageIter args;
   DBusConnection* conn;
   DBusError err;
   int ret;
   char* param;

   printf("Listening for method calls\n");

   // initialise the error
   dbus_error_init(&err);
   
   // connect to the bus and check for errors
   conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err); 
   }
   if (NULL == conn) {
      fprintf(stderr, "Connection Null\n"); 
      exit(1); 
   }
   
   // request our name on the bus and check for errors
   ret = dbus_bus_request_name(conn, "test.method.server", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Name Error (%s)\n", err.message); 
      dbus_error_free(&err);
   }
   if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
      fprintf(stderr, "Not Primary Owner (%d)\n", ret);
      exit(1); 
   }

   // loop, testing for new messages
   while (true) {
      // non blocking read of the next available message
      dbus_connection_read_write(conn, 0);
      msg = dbus_connection_pop_message(conn);

      // loop again if we haven't got a message
      if (NULL == msg) { 
         sleep(1); 
         continue; 
      }
      
      // check this is a method call for the right interface & method
      if (dbus_message_is_method_call(msg, "test.method.Type", "Method"))
         reply_to_method_call(msg, conn);

      // free the message
      dbus_message_unref(msg);
   }

   // close the connection
   dbus_connection_close(conn);
}

/**
 * Listens for signals on the bus
 */
void receive()
{
   DBusMessage* msg;
   DBusMessageIter args;
   DBusConnection* conn;
   DBusError err;
   int ret;
   char* sigvalue;

   printf("Listening for signals\n");

   // initialise the errors
   dbus_error_init(&err);
   
   // connect to the bus and check for errors
   conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message);
      dbus_error_free(&err); 
   }
   if (NULL == conn) { 
      exit(1);
   }
   
   // request our name on the bus and check for errors
   ret = dbus_bus_request_name(conn, "test.signal.sink", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Name Error (%s)\n", err.message);
      dbus_error_free(&err); 
   }
   if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
      exit(1);
   }

   // add a rule for which messages we want to see
   dbus_bus_add_match(conn, "type='signal',interface='test.signal.Type'", &err); // see signals from the given interface
   dbus_connection_flush(conn);
   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Match Error (%s)\n", err.message);
      exit(1); 
   }
   printf("Match rule sent\n");

   // loop listening for signals being emmitted
   while (true) {

      // non blocking read of the next available message
      dbus_connection_read_write(conn, 0);
      msg = dbus_connection_pop_message(conn);

      // loop again if we haven't read a message
      if (NULL == msg) { 
         sleep(1);
         continue;
      }

      // check if the message is a signal from the correct interface and with the correct name
      if (dbus_message_is_signal(msg, "test.signal.Type", "Test")) {
         
         // read the parameters
         if (!dbus_message_iter_init(msg, &args))
            fprintf(stderr, "Message Has No Parameters\n");
         else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
            fprintf(stderr, "Argument is not string!\n"); 
         else
            dbus_message_iter_get_basic(&args, &sigvalue);
         
         printf("Got Signal with value %s\n", sigvalue);
      }

      // free the message
      dbus_message_unref(msg);
   }
   // close the connection
   dbus_connection_close(conn);
}

int main(int argc, char** argv)
{
	int iCnt = 0;
	
	sysPowerSRVInit();
	while( iCnt++ < 1000 )
	{
		sPSgetCPUActivatedNum();
		sPSsetCPUActivatedNum(2);
		sPSgetCPUActivatedNum();
		sPSsetCPUActivatedNum(2);
		///sPSsetCPUDVFS(1);
	}
	sysPowerSRVDeinit();
	return 0;
	
	
	if (2 > argc) {
		printf ("Syntax: dbus-example [send|receive|listen|query|call] [<param>]\n");
		return 1;
	}
	char* param = "no param";
	if (3 >= argc && NULL != argv[2]) 
		param = argv[2];
	if (0 == strcmp(argv[1], "send"))
		sendsignal(param);
	else if (0 == strcmp(argv[1], "receive"))
		receive();
	else if (0 == strcmp(argv[1], "listen"))
		listen();
	else if (0 == strcmp(argv[1], "query"))
		query(param);
	else if (0 == strcmp(argv[1], "call"))
	{
		int iID = 0;
		int iParam = 0;
		
		if (0 != strcmp(argv[2], ""))
			iID = atoi(argv[2]);
		else
			printf ("Error: call function without ID\n");
			
		if (0 != strcmp(argv[3], ""))
			iParam = atoi(argv[3]);
		else
			printf ("Error: call function without param\n");
			
		calls( iID, iParam );
	}
	else {
		printf ("Syntax: dbus-example [send|receive|listen|query] [<param>]\n");
		return 1;
	}
	return 0;
}
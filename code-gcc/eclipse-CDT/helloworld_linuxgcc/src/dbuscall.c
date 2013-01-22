

/*
 *
 * add `pkg-config --cflags dbus-1` to compiler
 * add `pkg-config --libs dbus-1` to linker
 *
*/

#define DBUS_API_SUBJECT_TO_CHANGE
#include <glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <stdbool.h>
///#include <Xlib.h>
#include <X11/extensions/XTest.h>

///#include "toolhelps.h"



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




static void spgMSG( int iFlag, char *msgout, ... )
{
	int n, size = 128;
	char *p;
	va_list ap;

	if( iFlag <= 0 )
		return;

	if((p = malloc(size)) == NULL)
		return;

	va_start(ap, msgout);
	n = vsnprintf(p, size, msgout, ap);
	va_end(ap);

	///fprintf( stderr, "%s", p );
	g_printf("%s", p);

	free(p);
}


static void overridekeycode( int keyCode, int *pkc )
{
	if( keyCode == 0xff0d )
		*pkc = 36;
	else
	if( keyCode == 0xff09 )
		*pkc = 23;
	else
	if( keyCode == 0xffbe )
		*pkc = 67;
	else
	if( keyCode == 0xff56 )
		*pkc = 117;
	else
	if( keyCode == 0xff55 )
		*pkc = 112;
	else
	if( keyCode == 0xff52 )
		*pkc = 111;
	else
	if( keyCode == 0xff54 )
		*pkc = 116;
	else
	if( keyCode == 0xff51 )
		*pkc = 113;
	else
	if( keyCode == 0xff53 )
		*pkc = 114;
	else
	if( keyCode == 0xffff )
		*pkc = 119;
	else
	if( keyCode == 0xff57 )
		*pkc = 115;
	else
		*pkc = 0;
}

#if 0
void sendXkeycode( unsigned int keyCode )
{
	Display* disp = NULL;
	char *display_name;
	int kc = 0;

	display_name = XDisplayName(NULL);
	disp = XOpenDisplay( display_name );
	///disp = XOpenDisplay( ":0.0" );
	///disp = XOpenDisplay( NULL );
	if( disp == NULL )
	{
		spgMSG( dF(dERR), "%s:%s: XOpenDisplay fail !!!\n", __FILE__, __FUNCTION__ );
		return;
	}
	else
	{
		if( display_name )
			spgMSG( dF(dINFO), "%s:%s: XDisplayName [%s] !!!\n", __FILE__, __FUNCTION__, display_name );
	}

	kc = XKeysymToKeycode( disp, keyCode );

	if( 0 == kc )
	{
		overridekeycode( keyCode, &kc );
		if( kc != 0 )
			spgMSG( dF(dINFO), "%s:%s: keycode override [0->%d] !!!\n", __FILE__, __FUNCTION__, kc );
	}

	if( 0 != kc )
	{
		XTestFakeKeyEvent( disp, kc, True, CurrentTime );
		spgMSG( dF(dINFO), "%s:%s: keySym=%d[0x%08x] keyCode=%d[0x%08x] ON\n", __FILE__, __FUNCTION__, keyCode, keyCode, kc, kc );
		XFlush(disp);
		XTestFakeKeyEvent( disp, kc, False, CurrentTime );
		spgMSG( dF(dINFO), "%s:%s: keySym=%d[0x%08x] keyCode=%d[0x%08x] OFF\n", __FILE__, __FUNCTION__, keyCode, keyCode, kc, kc );
		XFlush(disp);
	}
	else
		spgMSG( dF(dERR), "%s:%s: keySym=%d[0x%08x] keyCode=%d[0x%08x] fail !!\n", __FILE__, __FUNCTION__, keyCode, keyCode, kc, kc );

	XCloseDisplay( disp );

}
#else
void sendXkeycode( unsigned int keyCode )
{
	Display* disp = NULL;
	char *display_name;
	int kc = 0;
	int Junk;
	Window window = 0;
	XKeyEvent event;
	int meta_mask = 0;


	display_name = XDisplayName(NULL);
	disp = XOpenDisplay( display_name );
	///disp = XOpenDisplay( ":0.0" );
	///disp = XOpenDisplay( NULL );
	if( disp == NULL )
	{
		spgMSG( dF(dERR), "%s:%s: XOpenDisplay fail !!!\n", __FILE__, __FUNCTION__ );
		return;
	}
	else
	{
		if( display_name )
			spgMSG( dF(dINFO), "%s:%s: XDisplayName [%s] !!!\n", __FILE__, __FUNCTION__, display_name );
	}

    if(window == 0)
        XGetInputFocus(disp, &window, &Junk);

    if(window == (Window)-1)
        window = RootWindow(disp, 0); // XXX nonzero screens?

    event.display	= disp;
    event.window	= window;
    event.root		= RootWindow(disp, 0); // XXX nonzero screens?
    event.subwindow	= None;
    event.time		= CurrentTime;
    event.x		= 1;
    event.y		= 1;
    event.x_root	= 1;
    event.y_root	= 1;
    event.same_screen	= True;
    event.type		= KeyPress;
    event.state		= 0;

	kc = XKeysymToKeycode( disp, keyCode );

	if( 0 == kc )
	{
		overridekeycode( keyCode, &kc );
		if( kc != 0 )
			spgMSG( dF(dINFO), "%s:%s: keycode override [0->%d] !!!\n", __FILE__, __FUNCTION__, kc );
	}

	event.keycode = kc;

	///if( 0 != kc )
	if(kc != NoSymbol)
	{
	    XSync(disp, False);
	    //XSetErrorHandler(MyErrorHandler);
	    XSendEvent(disp, window, True, KeyPressMask, (XEvent*)&event);
	    XSync(disp, False);
	    //XSetErrorHandler(NULL);
	}
	else
		spgMSG( dF(dERR), "%s:%s: keySym=%d[0x%08x] keyCode=%d[0x%08x] fail !!\n", __FILE__, __FUNCTION__, keyCode, keyCode, kc, kc );

	XCloseDisplay( disp );

}
#endif


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
		spgMSG( dF(dERR), "%s:%s: Message has no arguments!\n", __FILE__, __FUNCTION__ );
	else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
		spgMSG( dF(dERR), "%s:%s: Argument is not string!\n", __FILE__, __FUNCTION__ );
	else
		dbus_message_iter_get_basic(&args, &param);

	spgMSG( dF(dINFO), "%s:%s: Method called with %s\n", __FILE__, __FUNCTION__, param );

	// create a reply from the message
	reply = dbus_message_new_method_return(msg);

	// add the arguments to the reply
	dbus_message_iter_init_append(reply, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &stat)) {
		spgMSG( dF(dERR), "%s:%s: Out Of Memory!\n", __FILE__, __FUNCTION__ );
		exit(1);
	}
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &level)) {
		spgMSG( dF(dERR), "%s:%s: Out Of Memory!\n", __FILE__, __FUNCTION__ );
		exit(1);
	}

	// send the reply && flush the connection
	if (!dbus_connection_send(conn, reply, &serial)) {
		spgMSG( dF(dERR), "%s:%s: Out Of Memory!\n", __FILE__, __FUNCTION__ );
		exit(1);
	}
	dbus_connection_flush(conn);

	// free the reply
	dbus_message_unref(reply);
}


void reply_to_method_call2(DBusMessage* msg, DBusConnection* conn)
{
	DBusMessage* reply;
	DBusMessageIter args;
	bool stat = true;
	dbus_int32_t level = 999;
	dbus_uint32_t serial = 0;
	char* param = "";
	int iParam = 0;

	// read the arguments
	if (!dbus_message_iter_init(msg, &args))
		spgMSG( dF(dERR), "%s:%s: Message has no arguments!\n", __FILE__, __FUNCTION__ );
	else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
		spgMSG( dF(dERR), "%s:%s: Argument is not int32!\n", __FILE__, __FUNCTION__ );
	else
		dbus_message_iter_get_basic(&args, &iParam);

	spgMSG( dF(dINFO), "%s:%s: Method called with %d[0x%08x]\n", __FILE__, __FUNCTION__, iParam, iParam );
	sendXkeycode( iParam );

	// create a reply from the message
	reply = dbus_message_new_method_return(msg);

	// add the arguments to the reply
	dbus_message_iter_init_append(reply, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &stat)) {
		spgMSG( dF(dERR), "%s:%s: Out Of Memory!\n", __FILE__, __FUNCTION__ );
		exit(1);
	}
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &level)) {
		spgMSG( dF(dERR), "%s:%s: Out Of Memory!\n", __FILE__, __FUNCTION__ );
		exit(1);
	}

	// send the reply && flush the connection
	if (!dbus_connection_send(conn, reply, &serial)) {
		spgMSG( dF(dERR), "%s:%s: Out Of Memory!\n", __FILE__, __FUNCTION__ );
		exit(1);
	}
	dbus_connection_flush(conn);

	// free the reply
	dbus_message_unref(reply);
}




#define	_USE_USLEEP_
#define DBUS_INTERVAL 300

/**
 * Server that exposes a method call and waits for it to be called
 */
void *FakeKeyCall_listen( void *argv )
{
	DBusMessage* msg;
	DBusMessage* reply;
	DBusMessageIter args;
	DBusConnection* conn;
	DBusError err;
	int ret;
	char* param;

	spgMSG( dF(dINFO), "%s:%s: Listening for method calls ... \n", __FILE__, __FUNCTION__ );

	// initialise the error
	dbus_error_init(&err);

	// connect to the bus and check for errors
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) {
		spgMSG( dF(dERR), "%s:%s: Connection Error (%s)\n", __FILE__, __FUNCTION__, err.message );
		dbus_error_free(&err);
	}
	if (NULL == conn) {
		spgMSG( dF(dERR), "%s:%s: Connection Null!!\n", __FILE__, __FUNCTION__ );
		exit(1);
	}

	// request our name on the bus and check for errors
	ret = dbus_bus_request_name(conn, "test.method.server", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
	if (dbus_error_is_set(&err)) {
		spgMSG( dF(dERR), "%s:%s: Name Error (%s)\n", __FILE__, __FUNCTION__, err.message );
		dbus_error_free(&err);
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		spgMSG( dF(dERR), "%s:%s: Not Primary Owner (%d)\n", __FILE__, __FUNCTION__, ret );
		exit(1);
	}

	// loop, testing for new messages
	while (true) {
		// non blocking read of the next available message
		dbus_connection_read_write(conn, 0);
		msg = dbus_connection_pop_message(conn);

		// loop again if we haven't got a message
		if (NULL == msg) {
			#ifdef _USE_USLEEP_
			usleep( ((NULL == msg)?DBUS_INTERVAL:100) );
			#else
			sleep( ((NULL == msg)?1:0) );
			#endif
			continue;
		}

		// check this is a method call for the right interface & method
		if (dbus_message_is_method_call(msg, "test.method.Type", "sendFakeKey"))
			reply_to_method_call2(msg, conn);

		// free the message
		dbus_message_unref(msg);
	}

	// close the connection
	dbus_connection_close(conn);
}

/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>

#define DBUS_API_SUBJECT_TO_CHANGE
#include <dbus/dbus.h>
#include <stdbool.h>



#include "toolhelps.h"
#include "sysPowerCOMM.h"
#include "sysPower-lowlevel.h"
///#include "ipcpacket.h"
///#include "sysPowerSRV.h"
///#include "sysIPCSRV.h"



/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
#if 0
	#define	DBGFSET		(dDBG|dINFO|dERR)
#else
	#define	DBGFSET		(dINFO|dERR)
#endif
#define	dF(x)		(DBGFSET&x)






static pthread_t thread_id;
static char verStr[] = "v0.1";

/* for mutex */
static pthread_mutex_t mutex;
static int mutexINITED = 0;




#if 0
static int ifCoreNumValid( int iCore )
{
	int iRet = -1;
	
	if( (iCore > 0) && (iCore < 5) )
	{
		iRet = 0;
	}
	
	return iRet;
}


static int PowerCmdParser( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	int iParam1 = -1;
	
	if( pCmd )
	{
		/* get power command */
		iRet = getPowerCmdID( pCmd );
		spMSG( dF(dDBG), "%s:%s: get power command [%d] \n", __FILE__, __FUNCTION__, iRet );
		
		if( 0 != getPowerCmdRsptime( pCmd ) )
		{
			spMSG( dF(dERR), "%s:%s: ERROR!! get power command [%d] with Rsptime!! \n", __FILE__, __FUNCTION__, iRet );
			return iRet;
		}
		
		/* proceed the command */
		switch( iRet )
		{
			case GETCPUACTIVATEDNUM:				
				setPowerCmdReturn( pCmd, CPUcoreActivated );
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;

			case SETCPUACTIVATEDNUM:
				iParam1 = getPowerCmdParam1( pCmd );
				if( 0 == ifCoreNumValid( iParam1 ) )
				{
					iRet = setCPUcoreActivatedNumber( iParam1 );
					setPowerCmdReturn( pCmd, iRet );
					spMSG( dF(dDBG), "%s:%s: get power command iParam1[%d] %s !!\n", __FILE__, __FUNCTION__, iParam1, (0==iRet)?"OK":"Fail" );
				}
				else
				{
					setPowerCmdReturn( pCmd, -1 );
					spMSG( dF(dERR), "%s:%s: get power command iParam1[%d] over specification !!\n", __FILE__, __FUNCTION__, iParam1 );
				}
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;

			case SETCPUDVFS:
				iParam1 = getPowerCmdParam1( pCmd );
				iRet = setCPUdvfs( iParam1 );
				setPowerCmdReturn( pCmd, iRet );
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;

			case SETCPUSPEED:
				iParam1 = getPowerCmdParam1( pCmd );
				setCPUdvfs( 0 );
				iRet = setCPUspeed( iParam1 );
				setPowerCmdReturn( pCmd, iRet );
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;

			case SETCPUSUSPEND:
				iParam1 = getPowerCmdParam1( pCmd );
				/* iRet = setCPUsuspend(); */
				iRet = 0;
				setPowerCmdReturn( pCmd, iRet );
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;
				
			case LOOPBACKTEST:
				setPowerCmdReturn( pCmd, getPowerCmdParam1(pCmd) );
				setPowerCmdRsptime( pCmd, spGetTimetick() );
				break;
				
			default:
				break;
		}	/* switch */
		iRet = 0;
	}

	return iRet;
}
#else
static int PowerCmdParser( struct sysPowerCmd *pCmd )
{
	int iRet = -1;
	int iParam1 = -1;
	
	if( pCmd )
	{
	}

	return iRet;
}
#endif


#define	_USE_USLEEP_
#define CMD_INTERVAL	200		/* 200 ms polling */
#define US_MS			1000000
#define	HEREMSGTIME		45


#if 0
void *mainPowerMGR( void *argv )
{
	int iLoop = 0;
	struct sysPowerCmd pwrCmd;
	struct ipcpacket ipcPack;
	int Len = 0;
	int iRet = -1;

	while( 1 )
	{
		#ifdef _USE_USLEEP_
		usleep( ((0>iRet)?CMD_INTERVAL:0) );
		#else
		sleep( ((0>iRet)?1:0) );
		#endif


		#ifdef _USE_USLEEP_
		if( ( ++iLoop % ((HEREMSGTIME*US_MS)/CMD_INTERVAL) ) == 0 )
			spMSG( dF(dINFO), "%s:%s: %s is here ... %d \n", __FILE__, __FUNCTION__, verStr, iLoop );
		#else
		if( ++iLoop % HEREMSGTIME == 0 )
			spMSG( dF(dINFO), "%s:%s: %s is here ... %d \n", __FILE__, __FUNCTION__, verStr, iLoop );
		#endif
			
		/*  */
	#if 0
		Len = sizeof(struct sysPowerCmd);
		iRet = spIPCpayloadGet( NULL, (char *)&pwrCmd, &Len );
		if( iRet > 0 )
		{
			spMSG( dF(dDBG), "%s:%s: get data %d bytes... \n", __FILE__, __FUNCTION__, iRet );
			PowerCmdDump( &pwrCmd );	
		}
	#else
		/* try to get data pack */
		iRet = spIPCPackBuffOUT( &ipcPack );
		if( 0 == iRet )
		{
			/* parse the payload */
			Len = sizeof(struct sysPowerCmd);
			iRet = spIPCpayloadGet( &ipcPack, (char *)&pwrCmd, &Len );
			if( iRet > 0 )
			{
				spMSG( dF(dDBG), "%s:%s: get data %d bytes... \n", __FILE__, __FUNCTION__, iRet );
				if( dF(dDBG) ) PowerCmdDump( &pwrCmd );
				
				iRet = PowerCmdParser( &pwrCmd );
				
				if( 0 == iRet )
					iRet = spIPCPackResponse( &ipcPack, (char *)&pwrCmd, sizeof(struct sysPowerCmd) );

				/* work around for shm crash after suspend/resume */
				if( (SETCPUSUSPEND == getPowerCmdID( &pwrCmd )) && (0 == iRet) )
				{
					sleep(1);
					setCPUsuspend();
				}

			}
			else
				spMSG( dF(dERR), "%s:%s: get payload data %d bytes... fail !! \n", __FILE__, __FUNCTION__, iRet );
		}
	#endif	
	}

	spMSG( dF(dINFO), "%s:%s: Exit !!! \n", __FILE__, __FUNCTION__ );

	return 0;
}
#else
void *mainPowerMGR( void *argv )
{
	int iLoop = 0;
	struct sysPowerCmd pwrCmd;
	int Len = 0;
	int iRet = -1;

	while( 1 )
	{
	}

	spMSG( dF(dINFO), "%s:%s: Exit !!! \n", __FILE__, __FUNCTION__ );

	return 0;
}
#endif


#if 0
void IPCCallBack( void )
{

	spMSG( dF(dDBG), "IPCCallBack in !!! \n", __FILE__, __FUNCTION__ );
	return;
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
void PowerCall_listen() 
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



#if 0
int main( int argc, char *argv[] )
{
	int iRet = -1;

	/* setup ipc communication routine */
	spIPCInit();
	/* set server type & callback */
	spIPCinitServer( POWERMGR, IPCCallBack );
	
	spMxI( &mutex, &mutexINITED );
	
	/* create routine for power manager */
	pthread_create( &thread_id, NULL, &mainPowerMGR, NULL );

#ifdef __ARM_CODE__
	/* loop forever */
	while(1)
		sleep(3);
#else
	getchar();
#endif	/* #ifdef __ARM_CODE__ */

	spMxD( &mutex, &mutexINITED );
	
	spIPCDeinit();
	spMSG( dF(dINFO), "%s:%s: Exit !!! \n", __FILE__, __FUNCTION__ );
	return iRet;
}
#else
int main( int argc, char *argv[] )
{
	int iRet = -1;

	spMxI( &mutex, &mutexINITED );
	
	/* create routine for power manager */
	pthread_create( &thread_id, NULL, &mainPowerMGR, NULL );

#ifdef __ARM_CODE__
	/* loop forever */
	while(1)
		sleep(3);
#else
	spMSG( dF(dINFO), "%s:%s\n", __FILE__, __FUNCTION__ );
	spMSG( dF(dINFO), "%s:%s: any key exit !!! \n", __FILE__, __FUNCTION__ );
	getchar();
#endif	/* #ifdef __ARM_CODE__ */

	spMxD( &mutex, &mutexINITED );

	spMSG( dF(dINFO), "%s:%s: Exit !!! \n", __FILE__, __FUNCTION__ );
	return iRet;
}
#endif

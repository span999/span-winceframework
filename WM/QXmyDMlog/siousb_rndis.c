
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            S I O _ R N D I S

GENERAL DESCRIPTION
  This module contains the interface layer for SIO that applies to the rndis driver.  
  
EXTERNALIZED FUNCTIONS

  siousb_rndis_dev_init()
    Initializes the rndis driver.
                                                     
  siousb_rndis_open_stream()
    This procedure opens a stream across the serial link.
    
  siousb_rndis_close_stream()
    This procedure closes a stream.

  siousb_rndis_dev_transmit()
    Transmit data.

  siousb_rndis_ioctl()
    Control open stream.
         
  
INITIALIZATION AND SEQUENCING REQUIREMENTS

  siousb_rndis_dev_init()       must be called by a task upon power-up. 
                 
  siousb_rndis_open_stream()    must be called by each caller that wants to use 
                             SIO services over the rndis bus.  
                 
  siousb_rndis_close_stream()   needs to be called when the caller wants to give up the use
                             of SIO services
  Copyright (c) 2006-2008, Qualcomm Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Path:   $
  $Header: //depot/asic/Third_Party_OS/wince/WMSHARED/MAIN/csp/amss/drivers/siousb/siousb_rndis.c#12 $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/12/08   jmm     Use DIAG priority instead of default, change default to 250
04/30/08   jmm     Wait for 1st Rx packet before DTR notification; fix ioctl bug.
04/02/08   jmm     Wait for a USB connect event before trying to open a connection to
                   the PC;  closes an opened connection when a USB disconnect occurs, 
                   and waits for a USB connect.
                   event before reconnecting again.
02/01/08   jmm     Added DTR handling
01/11/08   jmm     Revised debug msgs, update min/max logic
12/04/07   jmm     Added sleep call if dsm item not available when needed.  
                   Fixed an edit bug introduced in the last check in.  Removed
                   unneeded msgs and code.
11/09/07   jmm     Fixed a device transmit problem where partial writes were
                   not handled correctly.  Revised select statement to
                   include error descriptor.  Added checks for function
                   arguments and other sanity checks.  Cleaned up
                   comments, changed ethr to rndis throughout, added
                   statistics, added function headers where needed,
                   removed TX thread and other obsolete code.
                   Initial Release.
===========================================================================*/


/*===========================================================================

                            INCLUDE FILES

===========================================================================*/


#include "target.h"                 /* Target specific definitions          */
#include "comdef.h"                 /* Definitions for byte, word, etc.     */
#include <winsock2.h>

#include "sio.h"                    /* Serial I/O Services.                 */
#include "siousb.h"                 /* SIO USB driver layer.                */
#include "siousb_rndis.h"
#include "siousb_util.h"

#include "..\..\..\..\shared\spCElog\spCElog.h"		///span090520,  add for QXDM log
///extern DWORD log_inp( PUCHAR pucInput, DWORD dwSize );

/*===========================================================================

                     CONSTANT DEFINES

===========================================================================*/
#define MAX_IP_STR_LEN 50

/* Path to the registry key containing settings. */
#define gwsz_RegKeyPath TEXT("Drivers\\BuiltIn\\WINDIAG\\Settings")

/* IP address of QPST server. */
#define gwsz_RegDiagIP TEXT("ServerIP")
#define gwsz_RegDiagIP_Default TEXT("169.254.2.2")

/* Port that QPST server listens. */
#define gwsz_RegDiagPort TEXT("ServerPort")
#define g_RegDiagPort_Default 2500

/* Number of times to try re-connecting. */
#define gwsz_RegMaxRetryCount TEXT("MaxRetryCount")
#define g_RegMaxRetryCount_Default 100

/* Number of milliseconds to wait between re-connection attempts. */
#define gwsz_RegRetrySleepMs TEXT("RetrySleepMs")
#define g_RegRetrySleepMs_Default 10000

/* Number of transmitted bytes to send before sleeping the Diag thread
   to yield the CPU to other tasks on the system. */
#define gwsz_RegTxThresholdBytes TEXT("TxThresholdBytes")
#define g_RegTxThresholdBytes_Default (1024 * 100)

/* Number of milliseconds to sleep when TX threshold is reached. */
#define gwsz_RegTxYieldMs TEXT("TxYieldMs")
#define g_RegTxYieldMs_Default (50)

/* Priority of the Rx thread for Diag. */
#define gwsz_RegRxThreadPriority TEXT("RxThreadPriority")
#define g_RegRxThreadPriority_Default (250)

/* Timeout for select */
#define SIOUSB_RNDIS_SELECT_TIMEOUT_SECONDS 5

/* Number of milliseconds to sleep when rx buffer isn't available. */
#define SIOUSB_DIAG_YIELD_MS (10)

/*===========================================================================

                     TYPEDEFS

===========================================================================*/

siousb_stats_type g_siousb_stats;

/*===========================================================================

                     STATIC VARIABLES

===========================================================================*/

static  volatile SOCKET  g_diagsocket = INVALID_SOCKET;           /* global socket handle */
static  volatile boolean g_diagConnected = FALSE;
static  volatile boolean dtr_callback_is_required = FALSE;

static  sio_usb_data_stream_info_type g_siousb_info;    /* Holds stream parameters */
static  HANDLE g_hRxThread;                /* Tx thread handle */

static  char gsz_QPSTServerAddr[MAX_IP_STR_LEN];
static  int  g_QPSTPort;
static  uint32 g_MaxAttempts;
static  uint32 g_RetrySleepValMs;
static  uint32 g_TxThresholdBytes;
static  uint32 g_TxYieldMs;
static  uint32 g_RxThreadPriority;

static  siousb_stream_type  siousb_stream = SIOUSB_STREAM_UNOPENED;
static  sio_port_id_type    g_port_id = SIO_PORT_MAX;
static volatile DWORD dwPrevTick;

/*===========================================================================

       Prototypes

===========================================================================*/
static siousb_status_type siousb_rndis_setup_open_stream(void);
static siousb_status_type siousb_rndis_setup_rx(void);
static siousb_status_type siousb_rndis_init_registry(void);
static void siousb_rndis_socket_cleanup(void);

/*===========================================================================

FUNCTION siousb_rndis_dev_init                               EXTERNALIZED FUNCTION

DESCRIPTION
  Initializes the siousb_rndis client.  
  There is no h/w initialization requried, but if there
  was it would be done here.
  
DEPENDENCIES
  Should be called just once.
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
void siousb_rndis_dev_init( 
	void 
)
{
  static BOOL dev_init_called = FALSE;

  g_siousb_stats.dev_init_calls++;
  if (FALSE == dev_init_called)
  {
    RETAILMSG(1, (L"siousb_rndis_dev_init:\r\n"));
    dev_init_called = TRUE;
    dwPrevTick = GetTickCount();
    g_siousb_stats.rx_min_pkt_size = MAX_INT32;
    g_siousb_stats.tx_min_pkt_size = MAX_INT32;
	
	__log_init();	///span090611,  add for QXDM log
  }
} /* siousb_rndis_dev_init */

/*===========================================================================

FUNCTION siousb_rndis_open_stream                            EXTERNALIZED FUNCTION

DESCRIPTION
  This function opens a stream across the rndis port.
      
DEPENDENCIES
  None
      
RETURN VALUE
  SIO_DONE_S:  SIO stream as specified in open_ptr has been successfully 
               opened.
  SIO_BADP_S:  Bad Parameter
    
SIDE EFFECTS
  None
  
===========================================================================*/

sio_status_type siousb_rndis_open_stream( 
  sio_open_type *open_ptr                       /* SIO Open structure      */
)
{
  sio_status_type           status;

  g_siousb_stats.open_stream_calls++;

  RETAILMSG(1, (L"siousb_rndis_open_stream:\r\n"));

  /* Make sure this is called */
  siousb_rndis_dev_init();

  /* Check for null & needed pointers */
  if (NULL == open_ptr)
  {
    RETAILMSG(1, (L"siousb_rndis_open_stream: NULL open_ptr \r\n"));
    return SIO_BADP_S;      
  }

  if (NULL == open_ptr->tx_queue)
  {
    RETAILMSG(1, (L"siousb_rndis_open_stream: NULL tx_queue pointer \r\n"));
  }

  if ((NULL == open_ptr->rx_queue) && (NULL == open_ptr->rx_func_ptr))
  {
    RETAILMSG(1, (L"siousb_rndis_open_stream: rx_queue && rx_func pointers are NULL\r\n"));
  }

  /* Check for proper stream ID */
  if (open_ptr->stream_id >= SIO_MAX_STREAM)
  {
    RETAILMSG(1, (L"siousb_rndis_open_stream: bad stream_id %d \r\n", open_ptr->stream_id));
    return SIO_BADP_S;      
  }

  /* Check that stream is not already open */
  if (siousb_stream != SIOUSB_STREAM_UNOPENED)
  {
    RETAILMSG(1, (L"siousb_rndis_open_stream: stream_id %d already open\r\n", open_ptr->stream_id));
    return SIO_BADP_S;      
  }
  RETAILMSG(1, (L"siousb_rndis_open_stream: stream_id=%d, port_id=%d \r\n", 
                                                    open_ptr->stream_id, open_ptr->port_id));

  /* Make sure the client only wants certain ports */
  switch (open_ptr->port_id)
  {
    #if (defined(FEATURE_USB_DIAG) || defined(FEATURE_USB_DIAG_NMEA))
    case SIO_PORT_USB_SER1:
      /* Setup the stream for rx/tx */
      siousb_stream = SIOUSB_STREAM_DIAG;
      break;
    #endif

    default:
      RETAILMSG(1, (L"siousb_rndis_open_stream, bad port %d\r\n", open_ptr->port_id));
      return SIO_BADP_S;      
  }

  g_siousb_info.tx_watermark_queue_ptr  = open_ptr->tx_queue;       /* Store tx queue */
  g_siousb_info.rx_watermark_queue_ptr  = open_ptr->rx_queue;       /* Store Rx queue */
  g_siousb_info.rx_callback_fn          = open_ptr->rx_func_ptr;    /* Store Rx notifier */
  g_siousb_info.sio_stream_id           = open_ptr->stream_id;      /* Store stream id */
  g_siousb_info.stream_mode             = open_ptr->stream_mode;    /* Store stream mode */

  siousb_rndis_init_registry();

  status = siousb_rndis_setup_open_stream();
  g_port_id = open_ptr->port_id;
  return status;
} /* siousb_rndis_open_stream */


/*===========================================================================

FUNCTION siousb_rndis_finish_close_stream                          
                                                                             
DESCRIPTION
  Complete close on the stream.  
                  
DEPENDENCIES
  None
      
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
static void siousb_rndis_finish_close_stream( 
  void
)
{
  g_siousb_stats.close_stream_calls++;

  RETAILMSG(1, (L"siousb_rndis_finish_close_stream\r\n"));

  /* Close the stream */
  if(g_diagsocket != INVALID_SOCKET)
  {
    closesocket(g_diagsocket);
    g_diagsocket = INVALID_SOCKET;
  }
} /* siousb_rndis_finish_close_stream */


/*===========================================================================

FUNCTION siousb_rndis_close_stream                  EXTERNALIZED FUNCTION
                                                                             
DESCRIPTION
  This procedure does all the book-keeping necessary to close a stream.  
                  
DEPENDENCIES
  None
      
RETURN VALUE
  None
  
SIDE EFFECTS
  close_func_ptr will be called when all pending data has left transmitter.
  
===========================================================================*/

void siousb_rndis_close_stream(
  sio_stream_id_type stream_id,             /* Stream ID                   */
  sio_port_id_type   port_id,               /* Port ID                     */
  void             (*close_func_ptr)(void)  /* Function to call when 
                                               transmission is complete.   */
)
{
  g_siousb_stats.close_stream_calls++;

  RETAILMSG(1, (L"siousb_rndis_close_stream: stream_id=%d, port_id=%d\r\n", stream_id, port_id));

  /* Check for valid arguments */
  if ((stream_id < 0) || (stream_id >= SIO_MAX_STREAM) || (g_port_id != port_id))
  {
    RETAILMSG(1, (L"siousb_rndis_close_stream: invalid stream_id %d or port %d\r\n", stream_id, port_id));
    return;
  }

  if (siousb_stream != SIOUSB_STREAM_DIAG)
  {
    RETAILMSG(1, (L"siousb_rndis_close_stream: stream not open.\r\n"));
    return;
  }

  /* Flush the Tx stream */
  siousb_rndis_flush_tx(stream_id, port_id, NULL);

  siousb_rndis_finish_close_stream();

  if (close_func_ptr)
  {   
    close_func_ptr();
  }
  siousb_stream = SIOUSB_STREAM_UNOPENED;
  g_port_id = SIO_PORT_MAX;
} /* siousb_rndis_close_stream */  

/*===========================================================================

FUNCTION siousb_rndis_flush_tx                           EXTERNALIZED FUNCTION
                                                                             
DESCRIPTION
  This function flushes the TX.

DEPENDENCIES
 User should not queue more data until after the registered callback
 is executed.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void siousb_rndis_flush_tx( 
  sio_stream_id_type stream_id,             /* Stream ID                   */
  sio_port_id_type   port_id,               /* Port ID                     */
  void             (*flush_func_ptr)(void)  /* Function to call when 
                                               pending transmission is 
                                               complete                    */
)
{
  g_siousb_stats.tx_flush_calls++;
  RETAILMSG(1, (L"siousb_rndis_flush_tx: stream_id=%d, port_id=%d\r\n", stream_id, port_id));

  /* Check for valid arguments */
  if ((stream_id < 0) || (stream_id >= SIO_MAX_STREAM) || (g_port_id != port_id))
  {
    RETAILMSG(1, (L"siousb_rndis_close_stream: invalid stream_id %d port %d\r\n", stream_id, port_id));
    return;
  }

  if (siousb_stream != SIOUSB_STREAM_DIAG)
  {
    RETAILMSG(1, (L"siousb_rndis_flush_tx: ERR, stream_id %d not open.\r\n", stream_id));
    return;
  }

  /* Makes sure the function exists. */
  if (flush_func_ptr)
  {
    flush_func_ptr();
  }
} /* siousb_rndis_flush_tx */   

/*===========================================================================

FUNCTION siousb_rndis_dev_transmit                           EXTERNALIZED FUNCTION

DESCRIPTION
  This function will transmit a packet.

DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
void siousb_rndis_dev_transmit( 
  sio_stream_id_type stream_id,           /* Stream ID                     */
  sio_port_id_type   port_id,             /* SIO Port ID              */
  dsm_item_type      *tx_ptr              /* Packet for transmission       */
)
{
  DWORD dwThisTick;
  dsm_item_type* chain_item_ptr = NULL;
  int pkts;

  int bytesWritten=0;
  int bytesRemaining;
  static uint32 tx_cnt = 0;
  uint8 *data_ptr;

  /* Check for valid arguments */
  if ((stream_id < 0) || (stream_id >= SIO_MAX_STREAM) || (g_port_id != port_id))
  {
    RETAILMSG(1, (L"siousb_rndis_dev_transmit: invalid stream_id %d or port_id %d\r\n", stream_id, port_id));
    return;
  }

  if (siousb_stream != SIOUSB_STREAM_DIAG)
  {
    RETAILMSG(1, (L"siousb_rndis_dev_transmit: stream_id %d is not open\r\n", stream_id));
    return;
  }

  /* Stats: Determine how many DSM items are being requested, and
     how many are chained.  A single dsm item is not chained.  
     If N packets are linked together, we count N-1 as chained. */
  pkts = 0;
  chain_item_ptr = tx_ptr;
  while (chain_item_ptr && chain_item_ptr->used)
  {
    pkts++;
    chain_item_ptr = chain_item_ptr->pkt_ptr;
  }
  if (pkts > 1)
  {
    g_siousb_stats.tx_dsm_items_chained += pkts - 1;
  }

  if (g_diagConnected)
  {
    chain_item_ptr = tx_ptr;
     
    while (chain_item_ptr && chain_item_ptr->used)
    {
      bytesRemaining = chain_item_ptr->used;
      data_ptr = chain_item_ptr->data_ptr; 

      /* Update Tx statistics */
      g_siousb_stats.tx_dsm_byte_count += bytesRemaining;
      g_siousb_stats.tx_max_pkt_size = max(bytesRemaining, g_siousb_stats.tx_max_pkt_size);
      g_siousb_stats.tx_min_pkt_size = min(bytesRemaining, g_siousb_stats.tx_min_pkt_size);

      while (bytesRemaining > 0)
      {
		__log_inp1( data_ptr, bytesRemaining );		///span090520,  add for QXDM log. get response from device
		
		if( __log_get_FakeHostEnabled() )	///span090611,  add for QXDM log
		{
			RETAILMSG(1, (L"SPD@siousb_rndis_dev_transmit: FakeHost take response, 0x%08x %d \r\n", data_ptr, bytesRemaining ));
			bytesWritten = bytesRemaining;
		}
		else
        bytesWritten = send(g_diagsocket, data_ptr, bytesRemaining, 0);

        if (SOCKET_ERROR == bytesWritten)
        {
          int send_error = WSAGetLastError();
          g_siousb_stats.tx_write_error_count++;
          if ((WSAECONNRESET == send_error) || (WSAEHOSTUNREACH == send_error))
          {
            RETAILMSG(1, (L"siousb_rndis_dev_transmit: host unreachable or connection reset (error %d)\r\n", send_error));
          }
          else
          {
            RETAILMSG(1, (L"siousb_rndis_dev_transmit: send failure, error = %d\r\n", send_error));
          }
          break;
        }

        tx_cnt += bytesWritten;
        bytesRemaining -= bytesWritten;
        if(bytesRemaining > 0)
        {
          g_siousb_stats.tx_dsm_items_partially_written++;
          data_ptr += bytesWritten;
        }
      }

      if (SOCKET_ERROR == bytesWritten)
      {
        /* Yield CPU to Rx thread, then break out of while loop */
        g_siousb_stats.tx_defers_X_to_rx_count++;
        Sleep(g_TxYieldMs);
        tx_cnt = 0;
        break;
      }

      g_siousb_stats.tx_dsm_items_written++;
      chain_item_ptr = chain_item_ptr->pkt_ptr;

      /* Yield the CPU to the Rx Thread that might have an incoming packet to process. */
      if (tx_cnt >= g_TxThresholdBytes)
      {
        g_siousb_stats.tx_defers_X_to_rx_count++;
        Sleep(g_TxYieldMs);
        tx_cnt = 0;
      }
      else
      {
        /* Yield to Rx Thread in case it has an incoming packet waiting to process. */
        g_siousb_stats.tx_defers_0_to_rx_count++;
        Sleep(0);
      }
    }
  }
  else
  {
    g_siousb_stats.tx_pkts_dropped_conn_down++;
  }

  if (tx_ptr != NULL)
  {
    dsm_free_packet(&tx_ptr);
  }

  /* Display statistics periodically.
   * This requires that we get regular transmit requests. */
  dwThisTick = GetTickCount();
  if ((dwThisTick - dwPrevTick) > SIOUSB_STATS_DISPLAY_INTERVAL_MS)
  {
    siousb_output_stats();
    dwPrevTick = dwThisTick;
  }
} /* siousb_rndis_dev_transmit */ 

/*===========================================================================

FUNCTION siousb_rndis_dtr_event

DESCRIPTION
  Make required DTR callbacks.

DEPENDENCIES
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
void siousb_rndis_dtr_event(
  boolean dtr_value
)
{
  /* Close the socket */
  if (!dtr_value)
  {
    RETAILMSG(1, (L"siousb_rndis_dtr_event: DTR is false, closing socket\r\n"));
    if (closesocket(g_diagsocket) != 0)
    {
      RETAILMSG(1, (L"siousb_rndis_dtr_event: closesocket error = %d\r\n", WSAGetLastError()));
    }
  }

  /* Make the callback as needed*/
  if (dtr_value)
  {
    /* Defer DTR callbacks untill Rx packets start to arrive */
    dtr_callback_is_required = TRUE;
  }
  else
  {
    /* Notify immediately */
    siousb_util_make_dtr_callbacks(&g_siousb_info, FALSE);
  }
} /* siousb_rndis_dtr_event */

/*===========================================================================

FUNCTION siousb_rndis_ioctl
                                                                             
DESCRIPTION
  Execute an IOCTL command.
  This function is present to preserve the AMSS interface, and
  is not currently not implemented in the WinMob version.
                  
DEPENDENCIES
  None
      
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
void siousb_rndis_ioctl(
  sio_stream_id_type    stream_id,
  sio_port_id_type      port_id,            /* SIO Port ID              */
  sio_ioctl_cmd_type    cmd,
  sio_ioctl_param_type  *param
)
{
  g_siousb_stats.ioctl_calls++;

  /* Check for valid arguments */
  if ((stream_id < 0) || (stream_id >= SIO_MAX_STREAM) || (g_port_id != port_id))
  {
    RETAILMSG(1, (L"siousb_rndis_ioctl: invalid stream_id %d or port_id %d\r\n", stream_id, port_id));
    return;
  }

  if (siousb_stream != SIOUSB_STREAM_DIAG)
  {
    RETAILMSG(1, (L"siousb_rndis_ioctl: stream_id %d is not open\r\n", stream_id));
    return;
  }

  RETAILMSG(1, (L"siousb_rndis_ioctl: stream=%d, port=%d, cmd=%d\r\n", stream_id, port_id, cmd));
  siousb_util_handle_ioctl_cmd(&g_siousb_info, cmd, param);
}

/*===========================================================================

FUNCTION siousb_rndis_disable_device
                                                                             
DESCRIPTION
  Disable the RNDIS usb device.
  This function is present to preserve the AMSS interface, and
  is not currently not implemented in the WinMob version.
                  
DEPENDENCIES
  None
      
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
void siousb_rndis_disable_device(
  sio_port_id_type port
)
{
  g_siousb_stats.disable_device_calls++;
}


/*===========================================================================

FUNCTION siousb_rndis_control_open_stream
                                                                             
DESCRIPTION
  Open the RNDIS control stream
  This function is present to preserve the AMSS interface, and
  is not currently not implemented in the WinMob version.
                  
DEPENDENCIES
  None
      
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
sio_status_type siousb_rndis_control_open_stream( 
  sio_open_type *open_ptr                       /* SIO Open structure      */
)
{
  g_siousb_stats.ctrl_open_stream_calls++;
  return SIO_DONE_S;
}


/*===========================================================================

FUNCTION siousb_rndis_control_close_stream
                                                                             
DESCRIPTION
  Close the RNDIS control stream
  This function is present to preserve the AMSS interface, and
  is not currently not implemented in the WinMob version.
                  
DEPENDENCIES
  None
      
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
void siousb_rndis_control_close_stream( 
  sio_stream_id_type stream_id,             /* Stream ID                   */
  void             (*close_func_ptr)(void)  /* Function to call when 
                                               transmission is complete.   */
)
{
  g_siousb_stats.ctrl_close_stream_calls++;
}


/*===========================================================================

FUNCTION siousb_rndis_control_transmit
                                                                             
DESCRIPTION
  Send data across the RNDIS control stream
  This function is present to preserve the AMSS interface, and
  is not currently not implemented in the WinMob version.
                  
DEPENDENCIES
  None
      
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
void siousb_rndis_control_transmit( 
  sio_stream_id_type stream_id,           /* Stream ID                     */
  dsm_item_type     *tx_ptr               /* Packet for transmission       */
)
{
  g_siousb_stats.ctrl_transmit_calls++;
}

/*===========================================================================

FUNCTION siousb_rndis_setup_open_stream           

DESCRIPTION
  Setup the stream for tx/rx
  
DEPENDENCIES
  
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
static siousb_status_type siousb_rndis_setup_open_stream(
  void
)
{
  WSADATA wsaData;

  RETAILMSG(1, (L"siousb_rndis_setup_open_stream: Opening for DIAG\r\n"));

  /* Startup winsock */
  if(WSAStartup(MAKEWORD(2,2), &wsaData))
  {
     /* WSAStartup failed */
    RETAILMSG(1, (L"siousb_rndis_setup_open_stream: Failure starting winsock, error = %d\r\n", WSAGetLastError()));
    return SIOUSB_FAILURE;
  }

  /* Setup the stream for Rx. 
   * The connection itself will be made in the receive thread.
   */
  if (siousb_rndis_setup_rx() != SIOUSB_SUCCESS)
  {
    siousb_rndis_socket_cleanup();
    return SIOUSB_FAILURE;
  }

  return SIOUSB_SUCCESS;
}

extern BOOL IsAPIReady( DWORD hAPI );	///span90518, add lof file


/*===========================================================================

FUNCTION siousb_rndis_connect_socket           

DESCRIPTION
  Create and connect the socket to the server.
  
DEPENDENCIES
  
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
static siousb_status_type siousb_rndis_connect_socket(
  void
)
{
  int rc;
  int flag = 1;
  siousb_status_type retVal = SIOUSB_FAILURE;
  uint32 nConnectAttempts = 0;
  SOCKADDR_IN saRemote;

  RETAILMSG(1, (L"siousb_rndis_connect_socket\r\n"));

  memset(&saRemote, 0, sizeof(saRemote));

  /* Create socket */
  g_diagsocket = socket(AF_INET, SOCK_STREAM, 0);
  if (INVALID_SOCKET == g_diagsocket) 
  {
    RETAILMSG(1, (L"siousb_rndis_connect_socket: Failure creating socket, error = %d\r\n", WSAGetLastError()));
    return SIOUSB_FAILURE;
  }

  /* Turn off nagle for the socket */
  rc = setsockopt(g_diagsocket, IPPROTO_TCP, TCP_NODELAY, 
               (char *) &flag, sizeof(int));  

  if (rc < 0)
  {
    RETAILMSG(1, (L"siousb_rndis_connect_socket: Failure disabling nagle, error = %d\r\n", WSAGetLastError()));
  }

  /* Connect to QPST */
  saRemote.sin_family = AF_INET;
  saRemote.sin_port = htons(g_QPSTPort);
  saRemote.sin_addr.s_addr = inet_addr(gsz_QPSTServerAddr);

  while(TRUE)
  {
    nConnectAttempts++;

    rc = connect(g_diagsocket, (SOCKADDR*)&saRemote, sizeof(saRemote));
    if (0 == rc)
    {
			CHAR ucTest[] = "This is siousb input : Established connection to QPST server^^^^^^^^^^^^################";	///span90518, add lof file
			if( IsAPIReady( SH_SHELL|SH_GDI|SH_WMGR ) )	///span90518, add lof file
				__log_inp( (PUCHAR)ucTest, strlen(ucTest) );	///span90518, add lof file
				
		g_siousb_stats.connect_count++;

      RETAILMSG(1, (L"siousb_rndis_connect_socket: Established connection to QPST server.\r\n"));

      g_diagConnected = TRUE;
	  __log_set_USB_connected( TRUE );	///span90611, add for QXDM log. QPST connection ok

      retVal = SIOUSB_SUCCESS;

      break;
    }

	__log_set_USB_connected( FALSE );	///span90611, add for QXDM log. No QPST connection
	
    if ( (nConnectAttempts <= g_MaxAttempts) || (0 == g_MaxAttempts) )
    {
      /* The failure could be caused by the USB cable not being connected (DTR is FALSE), or 
       * something else, such as QPST not running on the host. 
       */
      if (sio_usb_get_dtr_value())
      {
			CHAR ucTest[] = "This is siousb input : Failure connecting to QPST server^^^^^^^^^^^^################";	///span90518, add lof file
			if( IsAPIReady( SH_SHELL|SH_GDI|SH_WMGR ) )
				__log_inp( (PUCHAR)ucTest, strlen(ucTest) );	///span90518, add lof file
				
        RETAILMSG(1, (L"siousb_rndis_connect_socket: Failure connecting to QPST server, error = %d\r\n",
          WSAGetLastError()));
      }
      else
      {
        RETAILMSG(1, (L"siousb_rndis_connect_socket: USB cable not connected\r\n"));
        g_diagsocket = INVALID_SOCKET;
        retVal = SIOUSB_FAILURE;
        break;
      }

      /* Sleep before attempting a re-connection to the server. */
      g_siousb_stats.connect_reattempts++;
      Sleep(g_RetrySleepValMs);
    }
    else
    {
      g_siousb_stats.connect_max_attempts++;
      RETAILMSG(1, (L"siousb_rndis_connect_socket: Maximum connection attempts reached, aborting.\r\n"));

      closesocket(g_diagsocket);
      g_diagsocket = INVALID_SOCKET;
      retVal = SIOUSB_FAILURE;
      break;
    }
  }
  return retVal;
} /* siousb_rndis_connect_socket */


extern HANDLE g_hConnectEventArray[2];///span090611,  add for QXDM log

/*===========================================================================

FUNCTION siousb_rndis_rx_thread           

DESCRIPTION
  Thread that waits for read requests.

DEPENDENCIES
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None

===========================================================================*/
DWORD siousb_rndis_rx_thread(LPVOID param)
{
  static HANDLE hDtrEvent = NULL;
  int bytesRead;
  dsm_item_type *item_ptr = NULL;

  fd_set rfd, efd;
  struct timeval timer;

  timer.tv_sec  = SIOUSB_RNDIS_SELECT_TIMEOUT_SECONDS;
  timer.tv_usec = 0;

  RETAILMSG(1, (L"siousb_rndis_rx_thread: starting Rx thread for DIAG\r\n"));

  /* Set priority to required value */
  if (CeSetThreadPriority(GetCurrentThread(), g_RxThreadPriority) != TRUE)
  {
    RETAILMSG(1, (L"siousb_rndis_rx_thread: set thread priority err=%d\r\n", GetLastError()));
  }

  /* Create the event to sync on */
  if (NULL == (hDtrEvent = CreateEvent(NULL, TRUE, FALSE, SIO_USB_SIGNAL_DTR)))
  {
    RETAILMSG(1, (L"siousb_rndis_rx_thread: Can't create event. Error = %d\r\n", GetLastError()));
  }

  __log_set_USB_DTR_EventHandler( hDtrEvent );	///span090611,  add for QXDM log
  __log_set_USB_Connect_EventHandler( g_hConnectEventArray[0] );	///span090611,  add for QXDM log
  __log_set_USB_Disconnect_EventHandler( g_hConnectEventArray[1] );	///span090611,  add for QXDM log

  while(TRUE)
  {
    int select_results = 0;

    FD_ZERO(&rfd);
    FD_ZERO(&efd);
    FD_SET(g_diagsocket, &rfd);
    FD_SET(g_diagsocket, &efd);

    /* Wait until notification is received that DTR is asserted */
    if (!sio_usb_get_dtr_value())
    {
      RETAILMSG(1, (L"siousb_rndis_rx_thread: waiting for DTR\r\n"));
      if (WaitForSingleObject(hDtrEvent, INFINITE) != WAIT_OBJECT_0)
      {
        RETAILMSG(1, (L"siousb_rndis_rx_thread: Can't wait obj error = %d\r\n", GetLastError()));
      }
      RETAILMSG(1, (L"siousb_rndis_rx_thread: DTR is signalled\r\n"));
    }

	if( __log_get_FakeHostEnabled() )	///span090611,  add for QXDM log
	{
		if( !g_diagConnected )
		{	///first time here, make flag g_diagConnected TRUE
		g_diagConnected = TRUE;
		}
		else
		{	///wait here! until next command ready
			while( __log_get_WaitForCmd() )
			{
				Sleep(100);
				RETAILMSG(1, (L"SPD@siousb_rndis_rx_thread: Wait for new command ready !!!! \r\n"));
				if( !__log_get_FakeHostEnabled() )
				{
					RETAILMSG(1, (L"SPD@siousb_rndis_rx_thread: Fake host stop !!!! \r\n"));
					break;
				}	
			}	
		}
		g_siousb_stats.connect_count++;
		select_results = 1;			
		goto FAKE_HOST_RX;
	}	
	
    /* If not connected yet, connect.
     * The first time through it it won't be connected.  In addition, it
     * won't be connected if the cable was disconnected and then reconnected.
     */
    if (!g_diagConnected)
    {
      if (siousb_rndis_connect_socket() != SIOUSB_SUCCESS)
      {
        g_siousb_stats.rx_reconnect_failures++;
      }
    }

    /* Using select() shouldn't be necessary, but frequently recv()
     * does not get an error return when the USB cable is removed, and
     * recovery doesn't happen.  With the select() statement below, we 
     * either get an error return from select() itself, or from the 
     * FD_ISSET() macro.
     */

    /* Poll until there is data to read or an error occurs. */
    while (0 == (select_results = select(0, &rfd, NULL, &efd, &timer)))
      ;

    /* if select_results > 0, data is ready or an error occurred.  */
    bytesRead = -1;
    if (select_results > 0)
    {
      if (FD_ISSET(g_diagsocket, &efd))
      {
        // Zero variable in order to skip recv below
        select_results = 0;
        RETAILMSG(1, (L"siousb_rndis_rx_thread: FD_ISSET error=%d\r\n", WSAGetLastError()));
      }
    }
    else 
    {
      /* Must be socket error, skip the recv */
      RETAILMSG(1, (L"siousb_rndis_rx_thread: select error=%d\r\n", WSAGetLastError()));
    }

FAKE_HOST_RX:	///span090611,  add for QXDM log

    if (select_results > 0)
    {
		///RETAILMSG(1, (L"siousb_rndis_rx_thread: select_results is %d\r\n", select_results ));
      /* Get a dsm item, sleep if there are none. */
      while (NULL == (item_ptr = sio_usb_get_rx_buffer()))
      {
        /* Allow other threads to run, then try again. */
        Sleep(SIOUSB_DIAG_YIELD_MS);
        g_siousb_stats.rx_out_of_dsm_items++;
      }

	  if( __log_get_FakeHostEnabled() )	///span090611,  add for QXDM log
	  {
			bytesRead = __log_out( item_ptr->data_ptr, item_ptr->size );		///span090520,  add for QXDM log
			RETAILMSG(1, (L"SPD@siousb_rndis_rx_thread: FakeHost takefake command, 0x%08x %d \r\n", item_ptr->data_ptr, item_ptr->size ));
	  }
	  else
	  {
      bytesRead = recv(g_diagsocket, item_ptr->data_ptr, item_ptr->size, 0);
	  }
	  
	  __log_inp2( item_ptr->data_ptr, bytesRead );		///span090520,  add for QXDM log
	  
	if( __log_get_FakeHostEnabled() )	///span090611,  add for QXDM log
	{
		g_siousb_stats.connect_count--;
		///ResetEvent( hDtrEvent );
		///sio_usb_dtr_event( FALSE );
	}	
	  
      if (bytesRead < 0)
      {
        g_siousb_stats.rx_errors++;
        RETAILMSG(1, (L"siousb_rndis_rx_thread: Failed to Rx data, error = %d\r\n", WSAGetLastError()));
      }
      else if (0 == bytesRead)
      {
        g_siousb_stats.closed_socket_count++;
        RETAILMSG(1, (L"siousb_rndis_rx_thread: Remote side closed socket\r\n"));
      }
    }

    if (bytesRead > 0)
    {
      /* Update rx statistics */
      g_siousb_stats.rx_pkts_rcvd++;
      g_siousb_stats.rx_bytes_rcvd += bytesRead;
      g_siousb_stats.rx_max_pkt_size = max(bytesRead, g_siousb_stats.rx_max_pkt_size);
      g_siousb_stats.rx_min_pkt_size = min(bytesRead, g_siousb_stats.rx_min_pkt_size);

      item_ptr->used = (uint16) bytesRead;

      sio_usb_process_rx_buffer(&g_siousb_info, &item_ptr);

      /* If DTR notification is required, make it now since we're starting to get
       * incoming requests.
       */
       if (dtr_callback_is_required)
       {
         dtr_callback_is_required = FALSE;
         siousb_util_make_dtr_callbacks(&g_siousb_info, TRUE);
       }
    }
    else
    {
      g_diagConnected = FALSE;

      /* Close socket and attempt re-connection. */

      closesocket(g_diagsocket);
      g_diagsocket = INVALID_SOCKET;

      dsm_free_packet(&item_ptr);

      /* If DTR is still asserted, delay slightly then attempt a reconnect */
      if (sio_usb_get_dtr_value())
      {
        Sleep(g_RetrySleepValMs);
        if (siousb_rndis_connect_socket() != SIOUSB_SUCCESS)
        {
          g_siousb_stats.rx_reconnect_failures++;
        }
      }
    }
  }
  return -1;
} /* siousb_rndis_rx_thread */

/*===========================================================================

FUNCTION siousb_rndis_setup_rx           

DESCRIPTION
  Setup rx
  
DEPENDENCIES
  
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
static siousb_status_type siousb_rndis_setup_rx(
  void
)
{
  /* Create Rx thread */
  RETAILMSG(1, (L"siousb_rndis_setup_rx: Creating Rx thread for DIAG\r\n"));

  /* Create Rx thread */
  g_hRxThread = CreateThread(NULL,
                  0,
                  siousb_rndis_rx_thread,
                  (LPVOID)0,
                  0,
                  NULL);

  if (NULL == g_hRxThread)
  {
    RETAILMSG(1, (L"siousb_rndis_setup_rx: Failed to create Rx thread\r\n"));
    return SIOUSB_FAILURE;
  }
  return SIOUSB_SUCCESS;
} /* siousb_rndis_setup_rx */

/*===========================================================================

FUNCTION siousb_rndis_socket_cleanup

DESCRIPTION
  Cleanup
  
DEPENDENCIES
  
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
static void siousb_rndis_socket_cleanup(
  void
)
{
  RETAILMSG(1, (L"siousb_rndis_socket_cleanup: Entered socket cleanup.\r\n"));

  siousb_rndis_finish_close_stream();

  if(g_hRxThread)
  {
    if (!CloseHandle(g_hRxThread))
    {
      RETAILMSG(1, (L"siousb_rndis_socket_cleanup: closing thread handle error %d\r\n", GetLastError()));
    }
    g_hRxThread = NULL;
  }
  WSACleanup();
}

/*===========================================================================

FUNCTION siousb_rndis_init_registry

DESCRIPTION
  Read registry settings for Diag port, or initialize with default values.
  
DEPENDENCIES
  
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
static siousb_status_type siousb_rndis_init_registry(void)
{
    HKEY hKey = NULL;
    LONG result;
    DWORD dwType;
    DWORD dwSize;
    WCHAR szVal[100];
    LPWCH pszVal;
    DWORD dwVal;
    DWORD i;

    /* Open or create key if it does not exist. */
    if ((result = RegCreateKeyEx(
        HKEY_LOCAL_MACHINE, gwsz_RegKeyPath, 0, NULL, 0, 0, NULL,
         &hKey, &dwVal)) != ERROR_SUCCESS)
    {
      RETAILMSG(1, (L"siousb_rndis_init_registry: Failed RegCreateKeyEx, error = %d\r\n", result));

      /* TODO: Clean-up and set defaults. For the moment just continue and set defaults. */
    }

    /* Read/initialize value for QPST server IP. */

    dwType = 0;
    dwSize = sizeof(szVal);

    if ((RegQueryValueEx(hKey, gwsz_RegDiagIP, NULL, &dwType, (LPBYTE)szVal, &dwSize)
      != ERROR_SUCCESS) || (dwType != REG_SZ))
    {
      pszVal = gwsz_RegDiagIP_Default;
      dwType = REG_SZ;
      dwSize = (wcslen(pszVal)+1) * sizeof(WCHAR);

      if ((result = RegSetValueEx(hKey, gwsz_RegDiagIP, 0, dwType,
          (const BYTE*)pszVal, dwSize)) != ERROR_SUCCESS)
      {
          RETAILMSG(1, (L"siousb_rndis_init_registry: Failed RegSetKeyEx ServerIP, error = %d\r\n", result));
      }
    }
    else
    {
      pszVal = szVal;
    }

    dwSize /= sizeof(WCHAR);

    for (i = 0; i < dwSize && i < MAX_IP_STR_LEN-1; i++)
    {
      gsz_QPSTServerAddr[i] = (char)pszVal[i];
    }

    gsz_QPSTServerAddr[i] = '\0';

    /* TODO: Validate szVal contains a good IP address. */

    /* Read/initialize value for QPST server port. */

    dwType = 0;
    dwSize = sizeof(dwVal);

    if ((RegQueryValueEx(hKey, gwsz_RegDiagPort, NULL, &dwType, (LPBYTE)&dwVal, &dwSize)
      != ERROR_SUCCESS) || (dwType != REG_DWORD))
    {
      dwVal = g_RegDiagPort_Default;
      dwType = REG_DWORD;
      dwSize = sizeof(DWORD);

      if ((result = RegSetValueEx(hKey, gwsz_RegDiagPort, 0, dwType,
          (const BYTE*)&dwVal, dwSize)) != ERROR_SUCCESS)
      {
          RETAILMSG(1, (L"siousb_rndis_init_registry: Failed RegSetKeyEx ServerPort, error = %d\r\n", result));
      }
    }

    g_QPSTPort = dwVal;    
    RETAILMSG(1, (L"siousb_rndis_init_registry: QPSTPort = %d, IP = %s\r\n", g_QPSTPort, pszVal));

    /* Read/initialize value for maximum retry count. */

    dwType = 0;
    dwSize = sizeof(dwVal);

    if ((RegQueryValueEx(hKey, gwsz_RegMaxRetryCount, NULL, &dwType, (LPBYTE)&dwVal, &dwSize)
      != ERROR_SUCCESS) || (dwType != REG_DWORD))
    {
      dwVal = g_RegMaxRetryCount_Default;
      dwType = REG_DWORD;
      dwSize = sizeof(DWORD);

      if ((result = RegSetValueEx(hKey, gwsz_RegMaxRetryCount, 0, dwType,
          (const BYTE*)&dwVal, dwSize)) != ERROR_SUCCESS)
      {
          RETAILMSG(1, (L"siousb_rndis_init_registry: Failed RegSetKeyEx MaxRetryCount, error = %d\r\n", result));
      }
    }

    g_MaxAttempts = dwVal;    

    /* Read/initialize value for retry sleep value. */

    dwType = 0;
    dwSize = sizeof(dwVal);

    if ((RegQueryValueEx(hKey, gwsz_RegRetrySleepMs, NULL, &dwType, (LPBYTE)&dwVal, &dwSize)
      != ERROR_SUCCESS) || (dwType != REG_DWORD))
    {
      dwVal = g_RegRetrySleepMs_Default;
      dwType = REG_DWORD;
      dwSize = sizeof(DWORD);

      if ((result = RegSetValueEx(hKey, gwsz_RegRetrySleepMs, 0, dwType,
          (const BYTE*)&dwVal, dwSize)) != ERROR_SUCCESS)
      {
          RETAILMSG(1, (L"siousb_rndis_init_registry: Failed RegSetKeyEx RetrySleepMs, error = %d\r\n", result));
      }
    }

    g_RetrySleepValMs = dwVal;    
    RETAILMSG(1, (L"siousb_rndis_init_registry: MaxRetryCount = %d, SleepVal = %d msec\r\n", g_MaxAttempts, g_RetrySleepValMs));

    /* Read/initialize value for Tx Threadhold Bytes. */

    dwType = 0;
    dwSize = sizeof(dwVal);

    if ((RegQueryValueEx(hKey, gwsz_RegTxThresholdBytes, NULL, &dwType, (LPBYTE)&dwVal, &dwSize)
      != ERROR_SUCCESS) || (dwType != REG_DWORD))
    {
      dwVal = g_RegTxThresholdBytes_Default;
      dwType = REG_DWORD;
      dwSize = sizeof(DWORD);

      if ((result = RegSetValueEx(hKey, gwsz_RegTxThresholdBytes, 0, dwType,
          (const BYTE*)&dwVal, dwSize)) != ERROR_SUCCESS)
      {
          RETAILMSG(1, (L"siousb_rndis_init_registry: Failed RegSetKeyEx TxThreadholdBytes, error = %d\r\n", result));
      }
    }

    g_TxThresholdBytes = dwVal;    

    /* Read/initialize value for Tx Yield Ms. */

    dwType = 0;
    dwSize = sizeof(dwVal);

    if ((RegQueryValueEx(hKey, gwsz_RegTxYieldMs, NULL, &dwType, (LPBYTE)&dwVal, &dwSize)
      != ERROR_SUCCESS) || (dwType != REG_DWORD))
    {
      dwVal = g_RegTxYieldMs_Default;
      dwType = REG_DWORD;
      dwSize = sizeof(DWORD);

      if ((result = RegSetValueEx(hKey, gwsz_RegTxYieldMs, 0, dwType,
          (const BYTE*)&dwVal, dwSize)) != ERROR_SUCCESS)
      {
          RETAILMSG(1, (L"siousb_rndis_init_registry: Failed RegSetKeyEx TxYieldMs, error = %d\r\n", result));
      }
    }

    g_TxYieldMs = dwVal;    
    RETAILMSG(1, (L"siousb_rndis_init_registry: TxThreshold value = %d bytes, YieldVal = %d msec\r\n",
                          g_TxThresholdBytes, g_TxYieldMs));

    /* Read/initialize value for Rx thread priority. */

    dwType = 0;
    dwSize = sizeof(dwVal);

    if ((RegQueryValueEx(hKey, gwsz_RegRxThreadPriority, NULL, &dwType, (LPBYTE)&dwVal, &dwSize)
      != ERROR_SUCCESS) || (dwType != REG_DWORD))
    {
      /* If no entry for priority, use the priority of the calling thread.  */
      dwVal = CeGetThreadPriority(GetCurrentThread());
      if (THREAD_PRIORITY_ERROR_RETURN == dwVal)
      {
        /* If this fails, use a default value. */
        RETAILMSG(1, (L"siousb_rndis_init_registry: using default priority\r\n"));
        dwVal = g_RegRxThreadPriority_Default;
      }
      else
      {
        RETAILMSG(1, (L"siousb_rndis_init_registry: using callers priority\r\n"));
      }

      dwType = REG_DWORD;
      dwSize = sizeof(DWORD);

      if ((result = RegSetValueEx(hKey, gwsz_RegRxThreadPriority, 0, dwType,
          (const BYTE*)&dwVal, dwSize)) != ERROR_SUCCESS)
      {
          RETAILMSG(1, (L"siousb_rndis_init_registry: Failed RegSetKeyEx RxThreadPriority, error = %d\r\n", result));
      }
    }

    g_RxThreadPriority = dwVal;    
    RETAILMSG(1, (L"siousb_rndis_init_registry: RxThreadPriority = %d\r\n", g_RxThreadPriority));


    /* Close key */
    RegCloseKey(hKey);

    return SIOUSB_SUCCESS;
} /* siousb_rndis_init_registry */
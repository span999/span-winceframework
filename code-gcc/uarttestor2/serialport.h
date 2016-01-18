 /**
 * Serial port interface
 *
 * Usage:
 *  serial_port_t port;
 *  serial_port_init(&port);
 *  strncpy(port.file, "/dev/ttyXXX", sizeof(port.file));
 *  port.baudrate = 115200;     or other valid rates
 *  port.parity = 'n';          or , 'e', or 'o'
 *  port.databits = 8           or 7 or 6 or 5
 *  port.stopbits = 1;          or 2
 *  port.read_timeout_us=1000;  or other timeouts or -1 for "read blocking"
 *  port.local = 1;             or 0 or -1 for 'don't change'.
 *  port.set_dtr = -1;          or 0 or 1. -1== don't change
 *  port.set_rts = -1;          or 0 or 1. -1== don't change
 *
 *  Meaning of port.local: "Ignore modem status lines"
 *  Meaning of set_dtr   : "Set DTR directly when opening"
 *  Meaning of set_rts   : "Set RTS directly when opening"
 *
 * @file serialport.h
 * @author stfwi
 * @license Free, non-/commercial use granted.
 */
#ifndef SERIALPORT_H
#define SERIALPORT_H
 
#include <termios.h>
#ifdef  __cplusplus
extern "C" {
#endif
 
/**
 * Errors returned by serial port functions
 */
typedef enum {
  E_SERIAL_OK = 0,
  E_SERIAL_NULLPOINTER = -1,
  E_SERIAL_OPEN = -2,
  E_SERIAL_NOTTY = -3,
  E_SERIAL_GETATTR = -4,
  E_SERIAL_SETATTR = -5,
  E_SERIAL_GETIOC = -6,
  E_SERIAL_SETIOC = -7,
  E_SERIAL_SETNONBLOCK = -8,
  E_SERIAL_SELECT = -9,
  E_SERIAL_READ = -10,
  E_SERIAL_BAUDRATE = -11,
  E_SERIAL_IOCTL = - 12,
  E_SERIAL_WRITE = -13
} serial_port_error_t;
 
 
/**
 * Port definition and state structure.
 */
typedef struct {
  char file[128];                   /* Port file to open, e.g. /dev/ttyUSB      */
  int  baudrate;                    /* A valid baud rate                        */
  char parity;                      /* Parity, either 'n', 'e', or 'o'          */
  char stopbits;                    /* Stop bits: 0=1bit, 1=2 bits              */
  char databits;                    /* Data bits 5 to 8                         */
  char local;                       /* 1= modem status lines are ignored        */
  char flush;                       /* 1= clear buffers after opening           */
  char set_dtr;                     /* Init DTR, 0=clear, 1=set, -1=dont change */
  char set_rts;                     /* Init RTS, 0=clear, 1=set, -1=dont change */
  long long timeout_us;             /* <0=read blocking, >=0: timeout in us     */
  struct {                          /* State structure (dynamically changed)    */
    serial_port_error_t error_code; /* Last error code                          */
    char error_string[128];         /* Last error string                        */
    int handle;                     /* Handle of the open port file             */
    struct termios attr;            /* Applied terminal io attributes           */
    struct termios attr_original;   /* Terminal io attributes before opening    */
    int mdlns;                      /* Modem line states                        */
    int mdlns_original;             /* Modem line states before opening         */
  } _state;
} serial_port_t;
 
 
 
/**
 * Returns a list of valid baudrates. The end of the list is marked with the
 * value 0
 *
 * @return const int*
 */
const unsigned long* serial_port_get_valid_baudrates(void);
 
/**
 * Initialises the structure variable, resets all settings and the state to
 * 0 or the defaults, respectively.
 * @param serial_port_t *port
 * @return serial_port_error_t
 */
serial_port_error_t serial_port_init(serial_port_t *port);
 
 
/**
 * Opens and applies the settings defined in port. Returns 0 on success,
 * or another serial_port_error_t.
 *
 * @param serial_port_t *port
 * @return serial_port_error_t
 */
serial_port_error_t serial_port_open(serial_port_t *port);
 
 
/**
 * Close the port
 *
 * serial_port_t *port
 * @return serial_port_error_t
 */
serial_port_error_t serial_port_close(serial_port_t *port);
 
/**
 * Wait until a port input/output/error flag is set, timout is port.timeout_us.
 * Arguments 'r', 'w' and 'e' are boolean flags what events shall be checked
 * (r=have received, w=send buffer empty, e=error somewhere).
 *
 * Return value: bit0: r is set, bit1: w is set, bit2: e is set.
 *
 * @param serial_port_t *port
 * @param int r
 * @param int w
 * @param int e
 * @return int
 */
int serial_port_wait_for_events(serial_port_t *port, int r, int w, int e);
 
/**
 * Read from the port. Returns the number of bytes read or,
 * if <0 a serial_port_error_t.
 *
 * @param serial_port_t *port
 * @param char* buffer
 * @param unsigned sz
 * @return int
 */
int serial_port_read(serial_port_t *port, char* buffer, unsigned sz);
 
/**
 * Write sz bytes. Returns number of bytes written or <0 on error
 *
 * @param serial_port_t *port
 * @param const char* buffer
 * @param unsigned sz
 * @return int
 */
int serial_port_write(serial_port_t *port, const char* buffer, unsigned sz);
 
/**
 * Clears RX and TX buffers ("flush"). It wait_until_all_output_sent==1, waits
 * until TX buffer is completely sent.
 * @param serial_port_t *port
 * @param int wait_until_all_output_sent
 */
int serial_port_clear_buffers(serial_port_t *port, int wait_until_all_output_sent);
 
/**
 * Returns 1 if set, 0 if not set <0 on error
 *
 * @param serial_port_t *port
 * @return int
 */
int serial_port_get_cts(serial_port_t *port);
 
/**
 * Returns 1 if set, 0 if not set <0 on error
 *
 * @param serial_port_t *port
 * @return int
 */
int serial_port_get_dsr(serial_port_t *port);
 
/**
 * Returns 1 if set, 0 if not set <0 on error
 *
 * @param serial_port_t *port
 * @return int
 */
int serial_port_get_dtr(serial_port_t *port);
 
/**
 * Returns 1 if set, 0 if not set <0 on error
 *
 * @param serial_port_t *port
 * @return int
 */
int serial_port_get_rts(serial_port_t *port);
 
/**
 * Returns 0 on success <0 on error
 *
 * @param serial_port_t *port
 * @return int
 */
int serial_port_set_dtr(serial_port_t *port, int DTR);
 
/**
 * Returns 0 on success <0 on error
 *
 * @param serial_port_t *port
 * @return int
 */
int serial_port_set_rts(serial_port_t *port, int RTS);
 
/**
 * Print port settings and state to stderr.
 * @param serial_port_t *port
 */
void serial_port_dump(serial_port_t *port);
 
#ifdef  __cplusplus
}
#endif
#endif

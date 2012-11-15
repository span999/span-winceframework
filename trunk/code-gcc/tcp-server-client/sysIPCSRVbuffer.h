#ifndef __SYSIPCSRVBUFFER_H__
#define __SYSIPCSRVBUFFER_H__


int spIPCPackBuffADD( struct ipcpacket *pBuf );
int spIPCPackBuffGET( struct ipcpacket *pBuf );
int spIPCPackBuffPEEK( struct ipcpacket *pBuf );
int spIPCPackBuffDUMP( void );
int spIPCPackBuffpayloadGet( char *pBuf, int *pLen );


#endif /* ##ifndef __SYSIPCSRVBUFFER_H__ */

#ifndef __SYSIPCSRVTCP_H__
#define __SYSIPCSRVTCP_H__


int spIPCrequestTCP( char *pData, int *piLen, tSRVMGRTYP type );
int spIPCPackResponseTCP( struct ipcpacket *pBuf, char *pData, int iLen );
int spIPCsetCallbackTCP( PFNIPCCALLBACK pCB );
int spIPCinitServerTCP( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB );

int spIPCInitTCP( void );
int spIPCDeinitTCP( void );



#endif /* ##ifndef __SYSIPCSRVTCP_H__ */

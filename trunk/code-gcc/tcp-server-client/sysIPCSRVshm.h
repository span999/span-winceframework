#ifndef __SYSIPCSRVSHM_H__
#define __SYSIPCSRVSHM_H__


int spIPCrequestSHM( char *pData, int *piLen, tSRVMGRTYP type );
int spIPCPackResponseSHM( struct ipcpacket *pBuf, char *pData, int iLen );
int spIPCsetCallbackSHM( PFNIPCCALLBACK pCB );
int spIPCinitServerSHM( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB );

int spIPCInitSHM( void );
int spIPCDeinitSHM( void );



#endif /* ##ifndef __SYSIPCSRVSHM_H__ */

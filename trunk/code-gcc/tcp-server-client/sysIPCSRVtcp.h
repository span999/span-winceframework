#ifndef __SYSIPCSRVTCP_H__
#define __SYSIPCSRVTCP_H__


int tcpClient( int port );
int spIPCrecvEx( int newSock, struct ipcpacket *precvPack );
int spIPCsendEx( char *pData, int iLen, int iSrcID, int iSrcPort, int iTarID, int iTarPort );
int spIPCrecvWait( int iSocket, int iPort, struct ipcpacket *precvPack );
int tcpSockSend( char *hostname, int portnum, char *pData, int iSize );
int spIPCPackResponseTCP( struct ipcpacket *pBuf, char *pData, int iLen );
int spIPCsetCallbackTCP( PFNIPCCALLBACK pCB );
int spIPCinitServerTCP( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB );


#endif /* ##ifndef __SYSIPCSRVTCP_H__ */

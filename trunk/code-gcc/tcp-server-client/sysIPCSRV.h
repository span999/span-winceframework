#ifndef __SYSIPCSRV_H__
#define __SYSIPCSRV_H__




/* */
typedef enum {
	IPCMGR = 0,
	POWERMGR,
	HEALTHMGR,
} tSRVMGRTYP;


#define INITIPCCLIENTPORTNUM 	77777		/* use this default port number for client initial, common clinet for sysIPCMGR */
#define INITIPCHOSTPORTNUM 		88888		/* use this default port number for host initial, fixed host for sysIPCMGR */
/* we should get ipc host port number in boardcase way later ... */


/* callback define */
typedef void (*PFNIPCCALLBACK)( void );



int spIPCsend( char *pData, int iLen, tSRVMGRTYP type );
/*
int spIPCrecv( char *pData, int *piLen, int iSrcID, int iTarID );
*/
int spIPCrecv( char *pData, int *piLen, tSRVMGRTYP type  );
int spIPCrequest( char *pData, int *piLen, tSRVMGRTYP type );
int spIPCsetCallback( PFNIPCCALLBACK pCB );
int spIPCinitServer( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB );
int spIPCpayloadGet( struct ipcpacket *pPack, char *pBuf, int *pLen );
int spIPCPackBuffOUT( struct ipcpacket *pBuf );


#endif /* ##ifndef __SYSIPCSRV_H__ */

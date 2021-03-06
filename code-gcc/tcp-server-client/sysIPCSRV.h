#ifndef __SYSIPCSRV_H__
#define __SYSIPCSRV_H__




/* */
typedef enum {
	NONEMGR = 0,
	IPCMGR,
	POWERMGR,
	HEALTHMGR,
} tSRVMGRTYP;


#define INITIPCCLIENTPORTNUM 	77777		/* use this default port number for client initial, common clinet for sysIPCMGR */
#define INITIPCHOSTPORTNUM 		88888		/* use this default port number for host initial, fixed host for sysIPCMGR */
/* we should get ipc host port number in boardcase way later ... */


/* callback define */
typedef void (*PFNIPCCALLBACK)( void );


/* common function call */
/*
 * send data to specified server
 */
int spIPCsend( char *pData, int iLen, tSRVMGRTYP type );

/*
 * get data from specified server
 */
int spIPCrecv( char *pData, int *piLen, tSRVMGRTYP type  );

/*
 * send & get feedback for specified server
 */
int spIPCrequest( char *pData, int *piLen, tSRVMGRTYP type );

/* server funciton call */
/*
 * intial server service
 */
int spIPCinitServer( tSRVMGRTYP servertype, PFNIPCCALLBACK pCB );

/*
 * setup server callback
 */
int spIPCsetCallback( PFNIPCCALLBACK pCB );

/* packet handling function call */
/*
 * parse payload out from specified IPC packet, or from IPC queue if not specified
 */
int spIPCpayloadGet( struct ipcpacket *pPack, char *pBuf, int *pLen );

/*
 * get the oldest packet from IPC queue
 */
int spIPCPackBuffOUT( struct ipcpacket *pPack );

/*
 * send response base on origonal IPC packet
 */
int spIPCPackResponse( struct ipcpacket *pOrgPack, char *pData, int iLen );


int spIPCgetMgrID( tSRVMGRTYP type );
int spIPCgetMgrPort( tSRVMGRTYP type );
int spIPCgetClientID( void );
int spIPCgetClientPort( void );


int spIPCInit( void );
int spIPCDeinit( void );



#endif /* ##ifndef __SYSIPCSRV_H__ */

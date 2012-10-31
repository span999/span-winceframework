#ifndef __SYSIPCSRV_H__
#define __SYSIPCSRV_H__




/* */
typedef enum {
	IPCMGR = 0,
	POWERMGR,
	HEALTHMGR,
} tSRVMGRTYP;


#define INITIPCCLIENTPORTNUM 	77777		/* use this default port number for client initial */
#define INITIPCHOSTPORTNUM 		88888		/* use this default port number for host initial */
/* we should get ipc host port number in boardcase way later ... */





int spIPCsend( char *pData, int iLen );
int spIPCrecv( char *pData, int *piLen, int iSrcID, int iTarID );



#endif /* ##ifndef __SYSIPCSRV_H__ */

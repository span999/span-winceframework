#ifndef __SPLIBDBGMSGBUF_H__
#define __SPLIBDBGMSGBUF_H__


/*
*/


typedef struct _SPDBG_MSG
{
	BOOL  			bValid;
	DWORD 			dwTotal;
	DWORD 			dwSeqNum;
	unsigned short  DbgMsg[256];
}SPDBG_MSG, *PSPDBG_MSG;

typedef struct _SPDBG_MSG_INFO
{
	DWORD dwTotal;
	DWORD dwBufIndex;
	SPDBG_MSG	dbgMsg1;
	SPDBG_MSG	dbgMsg2;
	SPDBG_MSG	dbgMsg3;
	SPDBG_MSG	dbgMsg4;
	SPDBG_MSG	dbgMsg5;
	SPDBG_MSG	dbgMsg6;
}SPDBG_MSG_INFO, *PSPDBG_MSG_INFO;



DWORD spLibSysHALIoctl_GetDbgMsgBuf( PVOID pBuf );



#endif	///#ifndef __SPLIBDBGMSGBUF_H__
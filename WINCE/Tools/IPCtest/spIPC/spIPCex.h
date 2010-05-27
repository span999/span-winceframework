//
// Copyright (c) Mitac Corporation.  All rights reserved.
//
//

#ifndef __SPIPCEX_H__
#define __SPIPCEX_H__
/*--- start of file ---------------------------------------------------------*/


#define REG_PATH_OEM_IPC_SETTING            TEXT("Software\\Mitac\\spIPC")


///spIPC info for each IPC pair
typedef struct T_SPIPC_INFO_STRUCT
{
    DWORD dwIndex;          ///index, reserved for host
    DWORD dwBulkSize;       ///tansfer bulk size, reserved for host
    HANDLE  hHostTxDone;    ///handle for host Tx ok, reserved for IPC
    HANDLE  hHostRxDone;    ///handle for host Rx ok, reserved for IPC
    HANDLE  hClientTxDone;  ///handle for client Tx ok, reserved for IPC
    HANDLE  hClientRxDone;  ///handle for clinet Rx ok, reserved for IPC
    void* pIPCobject;      ///object pointer

} SPIPC_INFO, *PSPIPC_INFO;


///init
BOOL spIPCInitHost( SPIPC_INFO spIPCinfo );
BOOL spIPCInitClient( SPIPC_INFO spIPCinfo );
BOOL spIPCDeInitHost( SPIPC_INFO spIPCinfo );
BOOL spIPCDeInitClient( SPIPC_INFO spIPCinfo );

///data exchange
BOOL spIPCHostTx( DWORD dwIndex, PBYTE pbDataTx, DWORD dwDataLenght );
DWORD spIPCHostRx( DWORD dwIndex, PBYTE pbBufferRx, DWORD dwBufferLenght );
BOOL spIPCClientTx( DWORD dwIndex, PBYTE pbDataTx, DWORD dwDataLenght );
DWORD spIPCClientRx( DWORD dwIndex, PBYTE pbBufferRx, DWORD dwBufferLenght );

///IPC info
BOOL spListIPCindex( DWORD *pdwIndex, DWORD dwListCount );
SPIPC_INFO spGetIPCinfo( DWORD dwIndex );


/*--- end of file -----------------------------------------------------------*/
#endif  ///#ifndef __SPIPCEX_H__
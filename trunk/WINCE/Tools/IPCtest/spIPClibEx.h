//
// Copyright (c) Mitac Corporation.  All rights reserved.
//
//

#ifndef __SPIPCLIBEX_H__
#define __SPIPCLIBEX_H__
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


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

///init
BOOL spIPClibInitHost( SPIPC_INFO spIPCinfo );
BOOL spIPClibInitClient( SPIPC_INFO spIPCinfo );
BOOL spIPClibDeInitHost( SPIPC_INFO spIPCinfo );
BOOL spIPClibDeInitClient( SPIPC_INFO spIPCinfo );

///data exchange
BOOL spIPClibHostTx( DWORD dwIndex, PBYTE pbDataTx, DWORD dwDataLenght );
DWORD spIPClibHostRx( DWORD dwIndex, PBYTE pbBufferRx, DWORD dwBufferLenght );
BOOL spIPClibClientTx( DWORD dwIndex, PBYTE pbDataTx, DWORD dwDataLenght );
DWORD spIPClibClientRx( DWORD dwIndex, PBYTE pbBufferRx, DWORD dwBufferLenght );

///IPC info
BOOL spListIPClibindex( DWORD *pdwIndex, DWORD dwListCount );
SPIPC_INFO spGetIPClibinfo( DWORD dwIndex );

#ifdef __cplusplus
}
#endif  /* __cplusplus */


/*--- end of file -----------------------------------------------------------*/
#endif  ///#ifndef __SPIPCLIBEX_H__
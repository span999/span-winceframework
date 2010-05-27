//
// Copyright (c) Mitac Corporation.  All rights reserved.
//
//

#ifndef __SPIPCCLASS_H__
#define __SPIPCCLASS_H__
/*--- start of file ---------------------------------------------------------*/

#include "SBuffer.h"
#include "spIPCex.h"
#include "spIPC.h"



#define IPCLISTMAXSIZE  16

class SIPCList
{
public:
    SIPCList();
    ~SIPCList();
    BOOL AddItem( SPIPC_INFO ipcInfo );
    BOOL DeleteItem( SPIPC_INFO ipcInfo );
    SPIPC_INFO* GetInfoWithIndex( DWORD dwIndex );
    void* GetIPCWithIndex( DWORD dwIndex );
    DWORD CurrentListSize();
    
protected:
    SPIPC_INFO* m_pIPCInfoList[IPCLISTMAXSIZE];  ///max 16 pairs IPC
    DWORD m_dwCurrentListSize;
    
private:

};


#define IPCDEFAULTBULKBYTESIZE  (4*1024)

// typedefs for APIs that require the "nkmapfile" component
typedef WINBASEAPI HANDLE (WINAPI *PFN_CreateFileMapping) (
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCWSTR lpName
    );
typedef LPVOID (WINAPI *PFN_MapViewOfFile) (
    HANDLE hFileMappingObject,
    DWORD dwDesiredAccess,
    DWORD dwFileOffsetHigh,
    DWORD dwFileOffsetLow,
    DWORD dwNumberOfBytesToMap
    );
typedef BOOL (WINAPI *PFN_UnmapViewOfFile) (
    LPCVOID lpBaseAddress
    );



class SIPC
{
public:
    SIPC();
    SIPC( DWORD dwBulkSize );
    ~SIPC();
    SPIPC_INFO* GetIPCinfo();
    BOOL IPCHostTx( PBYTE pbDataTx, DWORD dwDataLenght );
    DWORD IPCHostRx( PBYTE pbBufferRx, DWORD dwBufferLenght );
    BOOL IPCClientTx( PBYTE pbDataTx, DWORD dwDataLenght );
    DWORD IPCClientRx( PBYTE pbBufferRx, DWORD dwBufferLenght );
    
protected:
    BOOL SIPCInit( DWORD dwBulkSize );
    DWORD FIFO2MemMapFile();
    BOOL MemMapFile2Out( PBYTE pBuffer, DWORD dwBufferSize );
    static DWORD WINAPI HostTxThreadStub(LPVOID lpvParam);
    DWORD HostTxThread();
    
protected:    
    PFN_CreateFileMapping gpfnCreateFileMapping;
    PFN_MapViewOfFile gpfnMapViewOfFile;
    PFN_UnmapViewOfFile gpfnUnmapViewOfFile;
    HANDLE hMemFile;
    BYTE *pbMemFile;
    HANDLE hHostTxThread;
    HANDLE hFIFOFilled;
    
    SPIPC_INFO m_IPCinfo;
    SFInFOut* pFIFO;
    
private:

};








/*--- end of file -----------------------------------------------------------*/
#endif  ///#ifndef __SPIPCCLASS_H__
//
// Copyright (c) Mitac Corporation.  All rights reserved.
//
//

#ifndef __SPIPCLIB_H__
#define __SPIPCLIB_H__
/*--- start of file ---------------------------------------------------------*/


///span1218, add debug define
#define DEBUG_CODE

#ifdef DEBUG_CODE
    #define     DBGMSGPREFIX    TEXT("IPClib")

    #define		dINIT		0x00000001
    #define		dWARN		0x00000010
    #define		dINFO		0x00000100
    #define		dDBUG		0x00001000
    #define		dFAIL		0x00010000
    #define		dCLAS		0x00100000

    ///#define		DBGMSG_LEVEL	(dCLAS|dWARN|dFAIL)
    #define		DBGMSG_LEVEL	(dINIT|dWARN|dINFO|dDBUG|dFAIL|dCLAS)
    ///#define		DBGMSG_LEVEL	(dINFO|dFAIL)

    #define     IPCDBGMSG(level, Message)	((level&DBGMSG_LEVEL)?(NKDbgPrintfW Message):0)
#else    
    #define     IPCDBGMSG      (void)
#endif  ///#ifdef DEBUG_CODE
///span1218, add debug define




/// typedefs for APIs that require the "IPC" component
typedef BOOL (*PFN_spIPCInitHost) (
    SPIPC_INFO spIPCinfo
    );

typedef BOOL (*PFN_spIPCInitClient) (
    SPIPC_INFO spIPCinfo
    );
    
typedef BOOL (*PFN_spIPCDeInitHost) (
    SPIPC_INFO spIPCinfo
    );

typedef BOOL (*PFN_spIPCDeInitClient) (
    SPIPC_INFO spIPCinfo
    );
    
typedef BOOL (*PFN_spIPCHostTx) (
    DWORD dwIndex,
    PBYTE pbDataTx,
    DWORD dwDataLenght
    );

typedef DWORD (*PFN_spIPCHostRx) (
	DWORD dwIndex,
	PBYTE pbBufferRx,
	DWORD dwBufferLenght
    );

typedef BOOL (*PFN_spIPCClientTx) (
    DWORD dwIndex,
    PBYTE pbDataTx,
    DWORD dwDataLenght
    );

typedef DWORD (*PFN_spIPCClientRx) (
    DWORD dwIndex,
    PBYTE pbBufferRx,
    DWORD dwBufferLenght
    );

typedef BOOL (*PFN_spListIPCindex) (
    DWORD *pdwIndex,
    DWORD dwListCount
    );

typedef SPIPC_INFO (*PFN_spGetIPCinfo) (
    DWORD dwIndex
    );





static PFN_spIPCInitHost gpfnspIPCInitHost = NULL;
static PFN_spIPCInitClient gpfnspIPCInitClient = NULL;
static PFN_spIPCInitHost gpfnspIPCDeInitHost = NULL;
static PFN_spIPCInitClient gpfnspIPCDeInitClient = NULL;
static PFN_spIPCHostTx gpfnspIPCHostTx = NULL;
static PFN_spIPCHostRx gpfnspIPCHostRx = NULL;
static PFN_spIPCClientTx gpfnspIPCClientTx = NULL;
static PFN_spIPCClientRx gpfnspIPCClientRx = NULL;
static PFN_spListIPCindex gpfnspListIPCindex = NULL;
static PFN_spGetIPCinfo gpfnspGetIPCinfo = NULL;





///init
BOOL spIPClibInitDll( void );
BOOL spIPClibDeInit( void );





/*--- end of file -----------------------------------------------------------*/
#endif  ///#ifndef __SPIPC_H__
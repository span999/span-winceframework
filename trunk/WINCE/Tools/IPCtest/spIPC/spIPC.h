//
// Copyright (c) Mitac Corporation.  All rights reserved.
//
//

#ifndef __SPIPC_H__
#define __SPIPC_H__
/*--- start of file ---------------------------------------------------------*/
#include "spIPCex.h"

///span1218, add debug define
#define DEBUG_CODE

#ifdef DEBUG_CODE
    #define     DBGMSGPREFIX    TEXT("IPC")

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


BOOL InitIPCList();
BOOL DeInitIPCList();
BOOL IsNewIPCinfoIndexValid( SPIPC_INFO spIPCinfo );






/*--- end of file -----------------------------------------------------------*/
#endif  ///#ifndef __SPIPC_H__
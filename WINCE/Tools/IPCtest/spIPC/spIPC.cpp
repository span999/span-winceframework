/* MiTAC Int'l Corp. 2001~2006

================================================================================
Description:

================================================================================
Release notes

Modifier	        sign	        date		    Description
--------------------------------------------------------------------------------

*/

#include <windows.h>
///#include "spIPCex.h"
#include "spIPC.h"
#include "spIPCClass.h"

static SIPCList* pIPCList = NULL;




/* -----------------------------------------------------------------------------
    DllMain
    description:
*/
BOOL WINAPI DllMain(HANDLE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    switch(dwReason) {
	    case DLL_PROCESS_ATTACH: {
	        IPCDBGMSG( dDBUG, ( L"%s:DllMain DLL_PROCESS_ATTACH\r\n", DBGMSGPREFIX) );
			break;
	    }
	    case DLL_PROCESS_DETACH: {
	        IPCDBGMSG( dDBUG, ( L"%s:DllMain DLL_PROCESS_DETACH\r\n", DBGMSGPREFIX) );
			break;
	    }
	    case DLL_THREAD_ATTACH: {
	        IPCDBGMSG( dDBUG, ( L"%s:DllMain DLL_THREAD_ATTACH\r\n", DBGMSGPREFIX) );
			break;
	    }
	    case DLL_THREAD_DETACH: {
	        IPCDBGMSG( dDBUG, ( L"%s:DllMain DLL_THREAD_DETACH\r\n", DBGMSGPREFIX) );
			break;
	    }
    	default:
    	    IPCDBGMSG( (dFAIL|dDBUG), ( L"%s:DllMain default\r\n", DBGMSGPREFIX) );
        	break;
    }
    return TRUE;
}


BOOL InitIPCList()
{
    BOOL bRet = TRUE;
    
    if( pIPCList == NULL )
        pIPCList = new SIPCList;
        
    return bRet;
}

BOOL DeInitIPCList()
{
    BOOL bRet = FALSE;
    
    if( (pIPCList != NULL) && (pIPCList->CurrentListSize() == 0) ) {
        delete pIPCList;
        pIPCList = NULL;
    } else {
        IPCDBGMSG( dDBUG, ( L"%s:DeInitIPCList failure \r\n", DBGMSGPREFIX) );
    }
        
    return bRet;
}

BOOL IsNewIPCinfoIndexValid( SPIPC_INFO spIPCinfo )
{
    BOOL bRet = FALSE;
    
    if( 0 != spIPCinfo.dwIndex ) {  ///valid value
        bRet = InitIPCList();
        if( bRet )
            if( NULL == pIPCList->GetInfoWithIndex( spIPCinfo.dwIndex ) ) ///not in list
                bRet = TRUE;
            else
                bRet = FALSE;
    }
    IPCDBGMSG( dDBUG, ( L"%s:IsNewIPCinfoIndexValid %s \r\n", DBGMSGPREFIX, bRet?L"valid":L"invalid") );
    return bRet;
}


///Init
BOOL spIPCInitHost( SPIPC_INFO spIPCinfo )
{
    BOOL bRet = FALSE;
    
    bRet = InitIPCList();
    if( bRet )
        bRet = IsNewIPCinfoIndexValid( spIPCinfo );

    if( bRet ) {
        bRet = pIPCList->AddItem( spIPCinfo );
    } else {
        IPCDBGMSG( dFAIL, ( L"%s:spIPCInitHost not valid \r\n", DBGMSGPREFIX) );
    }    
    
    IPCDBGMSG( dDBUG, ( L"%s:spIPCInitHost %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

BOOL spIPCInitClient( SPIPC_INFO spIPCinfo )
{
    BOOL bRet = FALSE;

    bRet = InitIPCList();
    if( bRet )
        bRet = IsNewIPCinfoIndexValid( spIPCinfo );

    if( !bRet ) {
        bRet = pIPCList->AddItem( spIPCinfo );
    } else {
        IPCDBGMSG( dFAIL, ( L"%s:spIPCInitClient No host found ! \r\n", DBGMSGPREFIX) );
    }    
    
    IPCDBGMSG( dDBUG, ( L"%s:spIPCInitClient %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

BOOL spIPCDeInitHost( SPIPC_INFO spIPCinfo )
{
    BOOL bRet = FALSE;
    

    IPCDBGMSG( dDBUG, ( L"%s:spIPCDeInitHost %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

BOOL spIPCDeInitClient( SPIPC_INFO spIPCinfo )
{
    BOOL bRet = FALSE;
    
    IPCDBGMSG( dDBUG, ( L"%s:spIPCDeInitClient %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}


///data exchange
BOOL spIPCHostTx( DWORD dwIndex, PBYTE pbDataTx, DWORD dwDataLenght )
{
    BOOL bRet = FALSE;
    SIPC *pIPC = NULL;
    
    if( NULL != pIPCList )  ///find the IPC with index
        pIPC = (SIPC *)pIPCList->GetIPCWithIndex( dwIndex );
        
    if( NULL != pIPC )
        bRet = pIPC->IPCHostTx( pbDataTx, dwDataLenght );
            
    IPCDBGMSG( dDBUG, ( L"%s:spIPCHostTx %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

DWORD spIPCHostRx( DWORD dwIndex, PBYTE pbBufferRx, DWORD dwBufferLenght )
{
    DWORD dwRet = 0;
    
    IPCDBGMSG( dDBUG, ( L"%s:spIPCHostRx %s\r\n", DBGMSGPREFIX, (dwRet>0)?L"Ok":L"Fail") );
    return dwRet;
}

BOOL spIPCClientTx( DWORD dwIndex, PBYTE pbDataTx, DWORD dwDataLenght )
{
    BOOL bRet = FALSE;
    
    IPCDBGMSG( dDBUG, ( L"%s:spIPCClientTx %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

DWORD spIPCClientRx( DWORD dwIndex, PBYTE pbBufferRx, DWORD dwBufferLenght )
{
    DWORD dwRet = 0;
    SIPC *pIPC = NULL;
    
    if( NULL != pIPCList )  ///find the IPC with index
        pIPC = (SIPC *)pIPCList->GetIPCWithIndex( dwIndex );
        
    if( NULL != pIPC )
        dwRet = pIPC->IPCClientRx( pbBufferRx, dwBufferLenght );
    
    IPCDBGMSG( dDBUG, ( L"%s:spIPCClientRx %s\r\n", DBGMSGPREFIX, (dwRet>0)?L"Ok":L"Fail") );
    return dwRet;
}

///IPC info
BOOL spListIPCindex( DWORD *pdwIndex, DWORD dwListCount )
{
    BOOL bRet = FALSE;
    
    IPCDBGMSG( dDBUG, ( L"%s:spListIPCindex %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

SPIPC_INFO spGetIPCinfo( DWORD dwIndex )
{
    BOOL bRet = FALSE;
    SPIPC_INFO infoRet;
    SPIPC_INFO *pIPCinfo = NULL;
    SIPC *pIPC = NULL;
    
    infoRet.dwIndex = 0;
    
    if( NULL != pIPCList )  ///find the IPC with index
        pIPC = (SIPC *)pIPCList->GetIPCWithIndex( dwIndex );
        
    if( NULL != pIPC )      ///find the IPCinfo with index
        pIPCinfo = pIPCList->GetInfoWithIndex( dwIndex );
        
    if( NULL != pIPCinfo ) {
        infoRet.dwIndex = pIPCinfo->dwIndex;
        infoRet.dwBulkSize = pIPCinfo->dwBulkSize;
        infoRet.hHostTxDone = pIPCinfo->hHostTxDone;
        infoRet.hHostRxDone = pIPCinfo->hHostRxDone;
        infoRet.hClientTxDone = pIPCinfo->hClientTxDone;
        infoRet.hClientRxDone = pIPCinfo->hClientRxDone;
        infoRet.pIPCobject = pIPCinfo->pIPCobject;        
    }

    IPCDBGMSG( dDBUG, ( L"%s:spGetIPCinfo %s\r\n", DBGMSGPREFIX, ( 0 != infoRet.dwIndex )?L"Ok":L"Fail") );
    return infoRet;
}
/* MiTAC Int'l Corp. 2001~2006

================================================================================
Description:

================================================================================
Release notes

Modifier	        sign	        date		    Description
--------------------------------------------------------------------------------

*/

#include <windows.h>
#include "spIPClibEx.h"
#include "spIPClib.h"


static BOOL bIPCdllLoaded = FALSE;
static HINSTANCE hiCoreDll = NULL; 

BOOL spIPClibInitDll( void )
{
    BOOL bRet = FALSE;
    
    if( bIPCdllLoaded )	{	///dll already load 
    	bRet = TRUE;
	} else {
		///load dll
        ///HINSTANCE hiCoreDll = NULL; 

        // get pointers to IPC functions
        hiCoreDll = LoadLibrary(_T("spIPC.dll"));
        if(hiCoreDll != NULL) {
            gpfnspIPCInitHost = (PFN_spIPCInitHost) GetProcAddress((HMODULE) hiCoreDll, _T("spIPCInitHost"));
            gpfnspIPCInitClient = (PFN_spIPCInitClient) GetProcAddress((HMODULE) hiCoreDll, _T("spIPCInitClient"));
            gpfnspIPCDeInitHost = (PFN_spIPCDeInitHost) GetProcAddress((HMODULE) hiCoreDll, _T("spIPCDeInitHost"));
            gpfnspIPCDeInitClient = (PFN_spIPCDeInitClient) GetProcAddress((HMODULE) hiCoreDll, _T("spIPCDeInitClient"));
            gpfnspIPCHostTx = (PFN_spIPCHostTx) GetProcAddress((HMODULE) hiCoreDll, _T("spIPCHostTx"));
            gpfnspIPCHostRx = (PFN_spIPCHostRx) GetProcAddress((HMODULE) hiCoreDll, _T("spIPCHostRx"));
            gpfnspIPCClientTx = (PFN_spIPCClientTx) GetProcAddress((HMODULE) hiCoreDll, _T("spIPCClientTx"));
            gpfnspIPCClientRx = (PFN_spIPCClientRx) GetProcAddress((HMODULE) hiCoreDll, _T("spIPCClientRx"));
            gpfnspListIPCindex = (PFN_spListIPCindex) GetProcAddress((HMODULE) hiCoreDll, _T("spListIPCindex"));
            gpfnspGetIPCinfo = (PFN_spGetIPCinfo) GetProcAddress((HMODULE) hiCoreDll, _T("spGetIPCinfo"));
        }
        ///FreeLibrary(hiCoreDll);		// we're already linked to coredll
		
		
		if(
			( NULL != gpfnspIPCInitHost ) &&
			( NULL != gpfnspIPCInitClient ) &&
			( NULL != gpfnspIPCDeInitHost ) &&
			( NULL != gpfnspIPCDeInitClient ) &&
			( NULL != gpfnspIPCHostTx ) &&
			( NULL != gpfnspIPCHostRx ) &&
			( NULL != gpfnspIPCClientTx ) &&
			( NULL != gpfnspIPCClientRx ) &&
			( NULL != gpfnspListIPCindex ) &&
			( NULL != gpfnspGetIPCinfo )
		) {
			bIPCdllLoaded = TRUE;
			bRet = TRUE;
		} else {
			bIPCdllLoaded = FALSE;
			bRet = FALSE;
		}	
	}

    IPCDBGMSG( dDBUG, ( L"%s:spIPClibInitDll %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

BOOL spIPClibDeInit()
{
    BOOL bRet = FALSE;
	
	bRet = FreeLibrary(hiCoreDll);
    if( bRet ) {
		gpfnspIPCInitHost = NULL;
		gpfnspIPCInitClient = NULL;
		gpfnspIPCDeInitHost = NULL;
		gpfnspIPCDeInitClient = NULL;
		gpfnspIPCHostTx = NULL;
		gpfnspIPCHostRx = NULL;
		gpfnspIPCClientTx = NULL;
		gpfnspIPCClientRx = NULL;
		gpfnspListIPCindex = NULL;
		gpfnspGetIPCinfo = NULL;
	} else {
    	IPCDBGMSG( dFAIL, ( L"%s:spIPClibDeInit FreeLibrary failure\r\n", DBGMSGPREFIX) );
    }
    
    IPCDBGMSG( dDBUG, ( L"%s:spIPClibDeInit %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}


///init
BOOL spIPClibInitHost( SPIPC_INFO spIPCinfo )
{
    BOOL bRet = FALSE;
    
    if( spIPClibInitDll() ) {
    	bRet = gpfnspIPCInitHost( spIPCinfo );
    } else {
    	IPCDBGMSG( dFAIL, ( L"%s:spIPClibInitHost spIPClibInitDll failure\r\n", DBGMSGPREFIX) );
    }

    IPCDBGMSG( dDBUG, ( L"%s:spIPClibInitHost %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

BOOL spIPClibInitClient( SPIPC_INFO spIPCinfo )
{
    BOOL bRet = FALSE;

    if( spIPClibInitDll() ) {
		bRet = gpfnspIPCInitClient( spIPCinfo );
	} else {
    	IPCDBGMSG( dFAIL, ( L"%s:spIPClibInitClient spIPClibInitDll failure\r\n", DBGMSGPREFIX) );
    }
    
    IPCDBGMSG( dDBUG, ( L"%s:spIPClibInitClient %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

BOOL spIPClibDeInitHost( SPIPC_INFO spIPCinfo )
{
    BOOL bRet = FALSE;
    
    if( gpfnspIPCDeInitHost( spIPCinfo ) ) {
    	bRet = spIPClibDeInit();
    } else {
    	IPCDBGMSG( dFAIL, ( L"%s:spIPClibDeInitHost gpfnspIPCDeInitHost failure\r\n", DBGMSGPREFIX) );
    }

    IPCDBGMSG( dDBUG, ( L"%s:spIPClibDeInitHost %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

BOOL spIPClibDeInitClient( SPIPC_INFO spIPCinfo )
{
    BOOL bRet = FALSE;

    if( gpfnspIPCDeInitClient( spIPCinfo ) ) {
		bRet = spIPClibDeInit();
	} else {
    	IPCDBGMSG( dFAIL, ( L"%s:spIPClibDeInitClient gpfnspIPCDeInitClient failure\r\n", DBGMSGPREFIX) );
    }
    
    IPCDBGMSG( dDBUG, ( L"%s:spIPClibDeInitClient %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}


///data exchange
BOOL spIPClibHostTx( DWORD dwIndex, PBYTE pbDataTx, DWORD dwDataLenght )
{
    BOOL bRet = FALSE;
    
    if( NULL != gpfnspIPCHostTx ) {
    	bRet = gpfnspIPCHostTx( dwIndex, pbDataTx, dwDataLenght );
	} else {
		IPCDBGMSG( dFAIL, ( L"%s:spIPClibHostTx gpfnspIPCHostTx failure\r\n", DBGMSGPREFIX) );
	}
    	
    IPCDBGMSG( dDBUG, ( L"%s:spIPClibHostTx %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

DWORD spIPClibHostRx( DWORD dwIndex, PBYTE pbBufferRx, DWORD dwBufferLenght )
{
    DWORD dwRet = 0;

    if( NULL != gpfnspIPCHostRx ) {
    	dwRet = gpfnspIPCHostRx( dwIndex, pbBufferRx, dwBufferLenght );
	} else {
		IPCDBGMSG( dFAIL, ( L"%s:spIPClibHostRx gpfnspIPCHostRx failure\r\n", DBGMSGPREFIX) );
	}
    
    IPCDBGMSG( dDBUG, ( L"%s:spIPClibHostRx %s\r\n", DBGMSGPREFIX, (dwRet>0)?L"Ok":L"Fail") );
    return dwRet;
}

BOOL spIPClibClientTx( DWORD dwIndex, PBYTE pbDataTx, DWORD dwDataLenght )
{
    BOOL bRet = FALSE;

    if( NULL != gpfnspIPCClientTx ) {
    	bRet = gpfnspIPCClientTx( dwIndex, pbDataTx, dwDataLenght );
	} else {
		IPCDBGMSG( dFAIL, ( L"%s:spIPClibClientTx gpfnspIPCClientTx failure\r\n", DBGMSGPREFIX) );
	}
	    
    IPCDBGMSG( dDBUG, ( L"%s:spIPClibClientTx %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

DWORD spIPClibClientRx( DWORD dwIndex, PBYTE pbBufferRx, DWORD dwBufferLenght )
{
    DWORD dwRet = 0;

    if( NULL != gpfnspIPCClientTx ) {
    	dwRet = gpfnspIPCClientTx( dwIndex, pbBufferRx, dwBufferLenght );
	} else {
		IPCDBGMSG( dFAIL, ( L"%s:spIPClibClientRx gpfnspIPCClientTx failure\r\n", DBGMSGPREFIX) );
	}

    IPCDBGMSG( dDBUG, ( L"%s:spIPClibClientRx %s\r\n", DBGMSGPREFIX, (dwRet>0)?L"Ok":L"Fail") );
    return dwRet;
}

///IPC info
BOOL spListIPClibindex( DWORD *pdwIndex, DWORD dwListCount )
{
    BOOL bRet = FALSE;
    
    if( NULL != gpfnspListIPCindex ) {
    	bRet = gpfnspListIPCindex( pdwIndex, dwListCount );
	} else {
		IPCDBGMSG( dFAIL, ( L"%s:spListIPClibindex gpfnspListIPCindex failure\r\n", DBGMSGPREFIX) );
	}

    IPCDBGMSG( dDBUG, ( L"%s:spListIPClibindex %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return bRet;
}

SPIPC_INFO spGetIPClibinfo( DWORD dwIndex )
{
    BOOL bRet = FALSE;
    SPIPC_INFO infoRet;

    if( NULL != gpfnspGetIPCinfo ) {
    	infoRet = gpfnspGetIPCinfo( dwIndex );
    	if( 0 == infoRet.dwIndex )
    		bRet = FALSE;
    	else
    		bRet = TRUE;
	} else {
		IPCDBGMSG( dFAIL, ( L"%s:spGetIPClibinfo gpfnspGetIPCinfo failure\r\n", DBGMSGPREFIX) );
	}

    IPCDBGMSG( dDBUG, ( L"%s:spGetIPClibinfo %s\r\n", DBGMSGPREFIX, bRet?L"Ok":L"Fail") );
    return infoRet;
}
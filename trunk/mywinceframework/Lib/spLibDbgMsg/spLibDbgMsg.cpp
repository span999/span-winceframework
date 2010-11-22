//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//

///#include <windows.h>
///#include <drvlib.h>
#include "spOS.h"
#include "spPlatform.h"

#include "spLibErrCodeDef.h"
#include "SPDebugDef.h"



typedef DWORD (*PFN_GetIdleTime)(void);

///phototype
static BOOL spLibInitContent( DWORD dwParam );
static BOOL spLibLoadMyModule( void );
static BOOL spLibFirstLoad( void );


static PFN_GetIdleTime gpfGetIdleTime;

static TCHAR szDbgBuff[256];
static DWORD dwDefaultLevel = ( dDBGFLAGOUTPMASK | dDBGFLAGSPECMASK | dDBGFLAGCOMMMASK );


/// *****************************************
/// interface functions
/// *****************************************

DWORD spLibDbgMsg_Init( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDNKMSG( dINIT, (L"$$$spLibDbgMsg_Init: %d \r\n", 1) );
	
	dwDefaultLevel = dwParam;
		
	SPDNKMSG( dINIT, (L"$$$spLibDbgMsg_Init: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibDbgMsg_Deinit( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDNKMSG( dINIT, (L"$$$spLibDbgMsg_Deinit: %d \r\n", 1) );
	

	
	SPDNKMSG( dINIT, (L"$$$spLibDbgMsg_Deinit: %d \r\n", 0) );
	return dwRet;
}

DWORD spLibDbgMsg( DWORD dwFlag, LPCTSTR szError,... )
{
	DWORD dwRet = 0;
	va_list vl;
	
	SPDNKMSG( dINFO, (L"$$$spLibDbgMsg: %d \r\n", 1) );
	
	if( dwFlag & (dwDefaultLevel & (dDBGFLAGSPECMASK | dDBGFLAGCOMMMASK)) ) ///check if any degub message flag enabled
	{
		va_start(vl, szError);

		StringCchVPrintf(szDbgBuff, (256), szError, vl);
	
		if( dwFlag & (dwDefaultLevel & dDBGFLAGOUTPMASK) )
		{
			if( dwFlag & dNOUT )	///nkprint
				NKDbgPrintfW( szDbgBuff );
		
			if( dwFlag & dDOUT )	///dailogue
				MessageBox( NULL, szDbgBuff, TEXT("Dbg Msg:"), MB_OK|MB_SETFOREGROUND );
				
			if( dwFlag & dFOUT )	///log file
				NKDbgPrintfW( szDbgBuff );

		}	
	
		va_end(vl);
	}	
	
	
	SPDNKMSG( dINFO, (L"$$$spLibDbgMsg: %d \r\n", 0) );
	
	return dwRet;
}

DWORD spLibDbgMsg_Dlg( LPCTSTR szError,... )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
    va_list vl;
	
	SPDNKMSG( dINFO, (L"$$$spLibDbgMsg_Dlg: %d \r\n", 1) );

	if( (dwDefaultLevel & (dDBGFLAGSPECMASK | dDBGFLAGCOMMMASK)) ) ///check if any degub message flag enabled
	{
		va_start(vl, szError);

		StringCchVPrintf(szDbgBuff, (256), szError, vl);
	
		if( 1 )
		{
			if( dwDefaultLevel & dDOUT )	///dailogue
				MessageBox( NULL, szDbgBuff, TEXT("Dbg Msg:"), MB_OK|MB_SETFOREGROUND );
		}	
	
		va_end(vl);
	}	
	
	
	SPDNKMSG( dINFO, (L"$$$spLibDbgMsg_Dlg: %d \r\n", 0) );
	
	return dwRet;
}


DWORD spLibDbgMsg_NK( LPCTSTR szError,... )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	va_list vl;
	
	SPDNKMSG( dINFO, (L"$$$spLibDbgMsg_NK: %d \r\n", 1) );

	if( (dwDefaultLevel & (dDBGFLAGSPECMASK | dDBGFLAGCOMMMASK)) ) ///check if any degub message flag enabled
	{
		va_start(vl, szError);

		StringCchVPrintf(szDbgBuff, (256), szError, vl);
	
		if( 1 )
		{
			if( dwDefaultLevel & dNOUT )	///nkprint
				NKDbgPrintfW( szDbgBuff );
		}	
	
		va_end(vl);
	}	

	
	SPDNKMSG( dINFO, (L"$$$spLibDbgMsg_NK: %d \r\n", 0) );
	
	return dwRet;
}

DWORD spLibDbgMsg_Log( LPCTSTR szError,... )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	va_list vl;
	
	SPDNKMSG( dINFO, (L"$$$spLibDbgMsg_Log: %d \r\n", 1) );

	if( (dwDefaultLevel & (dDBGFLAGSPECMASK | dDBGFLAGCOMMMASK)) ) ///check if any degub message flag enabled
	{
		va_start(vl, szError);

		StringCchVPrintf(szDbgBuff, (256), szError, vl);
	
		if( 1 )
		{
			if( dwDefaultLevel & dFOUT )	///log file
				NKDbgPrintfW( szDbgBuff );
		}	
	
		va_end(vl);
	}	
	
	SPDNKMSG( dINFO, (L"$$$spLibDbgMsg_Log: %d \r\n", 0) );

	return dwRet;
}




/// *****************************************
/// internal functions
/// *****************************************
static BOOL spLibFirstLoad( void )
{
	BOOL bRet = TRUE;
	
	///if( (0 == pThisContent->dwStartTick) || (0 == pThisContent->dwStartTick) )
	if( 1 )
		bRet = TRUE;
	else
		bRet = FALSE;
		
	return bRet;
}


static BOOL spLibInitContent( DWORD dwParam )
{
	BOOL bRet = TRUE;

	    ///TCHAR                       szBuff[128];


	
	return bRet;
}



static BOOL spLibLoadMyModule(void)
{
    HMODULE hCore;	
    BOOL bRet = FALSE;
	
    /// init system function call
    hCore = (HMODULE)LoadLibrary( L"coredll.dll" );
    if ( hCore ) {
        gpfGetIdleTime = (PFN_GetIdleTime)GetProcAddress( hCore, L"GetIdleTime" );
        if ( !gpfGetIdleTime )
        {
            FreeLibrary(hCore);
            bRet = FALSE;
		}
        else
            bRet = TRUE;
    }
    
    return bRet;	
}

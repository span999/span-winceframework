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
#include <windows.h>
#include <drvlib.h>

#include "..\..\Inc\spLibErrCodeDef.h"
#include "SPDebugDef.h"


///

typedef struct _LibSysInfoMemoryContent { 
	DWORD dwLength;
	DWORD dwMemoryLoad;
	DWORD dwTotalPhys;
	DWORD dwAvailPhys;
	int iDiffPhys;
	DWORD dwTotalPageFile;
	DWORD dwAvailPageFile;
	int iDiffPageFile;
	DWORD dwTotalVirtual;
	DWORD dwAvailVirtual;
	int iDiffVirtual;
} LibSysInfoMemoryContent, *PLibSysInfoMemoryContent; 



typedef void (*PFN_GlobalMemoryStatus)( MEMORYSTATUS *pBuffer );


///phototype
static BOOL spLibInitContent( LibSysInfoMemoryContent* pThis );
static BOOL spLibLoadMyModule( void );
static BOOL spLibFirstLoad( void );
static BOOL spLibLoadMemoryInfo( MEMORYSTATUS *pmmi );
static BOOL spLibProcessMemoryInfo( LibSysInfoMemoryContent* pThis, MEMORYSTATUS *pmmi );
static BOOL spLibCopyMemoryInfo( LibSysInfoMemoryContent* pTar, LibSysInfoMemoryContent* pSrc );

///
static PFN_GlobalMemoryStatus gpfGlobalMemoryStatus;

static LibSysInfoMemoryContent ThisContent;
static LibSysInfoMemoryContent *pThisContent = &ThisContent;
static MEMORYSTATUS mmi;
static MEMORYSTATUS *pmmi = &mmi;


/// *****************************************
/// interface functions
/// *****************************************

DWORD spLibSysInfoMemory_Init( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibSysInfoMemory_Init: %d \r\n", 1) );
	
	
	if( spLibInitContent( pThisContent ) && spLibLoadMyModule() )
		dwRet = 0;
	else
		dwRet = (-1);

		
	SPDMSG( dINIT, (L"$$$spLibSysInfoMemory_Init: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibSysInfoMemory_Deinit( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibSysInfoMemory_Deinit: %d \r\n", 1) );
	
	spLibInitContent( pThisContent );
	
	SPDMSG( dINIT, (L"$$$spLibSysInfoMemory_Deinit: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibSysInfoMemory_Get( LibSysInfoMemoryContent *pOut )
{
	DWORD dwRet = -1;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibSysInfoMemory_Get: %d \r\n", dwRet) );
	
	if( spLibLoadMemoryInfo( pmmi ) && pOut )
	{
		///go process
		if( spLibProcessMemoryInfo( pThisContent, pmmi ) )
		{
			spLibCopyMemoryInfo( pOut, pThisContent );
			dwRet = 0;
		}
	}
	
	SPDMSG( dINFO, (L"$$$spLibSysInfoMemory_Get: %d \r\n", dwRet) );
	return dwRet;
}


DWORD spLibSysInfoMemory_GetAvg( void )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibSysInfoMemory_GetAvg: %d \r\n", 1) );
	
	
	SPDMSG( dINFO, (L"$$$spLibSysInfoMemory_GetAvg: %d \r\n", 0) );
	return dwRet;
}



/// *****************************************
/// internal functions
/// *****************************************
static BOOL spLibFirstLoad( void )
{
	BOOL bRet = TRUE;
	
	if( (0 == pThisContent->dwLength) || (0 == pThisContent->dwMemoryLoad) )
		bRet = TRUE;
	else
		bRet = FALSE;
		
	return bRet;
}


static BOOL spLibInitContent( LibSysInfoMemoryContent* pThis )
{
	BOOL bRet = TRUE;

	pThis->dwLength = 0; 
	pThis->dwMemoryLoad = 0; 
	pThis->dwTotalPhys = 0; 
	pThis->dwAvailPhys = 0; 
	pThis->iDiffPhys = 0; 
	pThis->dwTotalPageFile = 0; 
	pThis->dwAvailPageFile = 0; 
	pThis->iDiffPageFile = 0; 
	pThis->dwTotalVirtual = 0; 
	pThis->dwAvailVirtual = 0; 	
	pThis->iDiffVirtual = 0; 	
	
	return bRet;
}



static BOOL spLibLoadMyModule(void)
{
    HMODULE hCore;	
    BOOL bRet = FALSE;
	
    /// init system function call
    hCore = (HMODULE)LoadLibrary( L"coredll.dll" );
    if ( hCore ) {
        gpfGlobalMemoryStatus = (PFN_GlobalMemoryStatus)GetProcAddress( hCore, L"GlobalMemoryStatus" );
        if ( !gpfGlobalMemoryStatus )
        {
            FreeLibrary(hCore);
            bRet = FALSE;
		}
        else
            bRet = TRUE;
    }
    
    return bRet;	
}


static BOOL spLibLoadMemoryInfo( MEMORYSTATUS *pmmi )
{
	BOOL bRet = FALSE;
	///MEMORYSTATUS mmi;
	
	if( gpfGlobalMemoryStatus && pmmi )
	{
		pmmi->dwLength = sizeof( MEMORYSTATUS );
		gpfGlobalMemoryStatus( pmmi );
	
		bRet = TRUE;
	}
	
	return bRet;
}


static BOOL spLibProcessMemoryInfo( LibSysInfoMemoryContent* pThis, MEMORYSTATUS *pmmi )
{
	BOOL bRet = FALSE;
	int iTmp = 0;
	
	if( pThis && pmmi )
	{
		pThis->dwLength = sizeof(LibSysInfoMemoryContent); 
		pThis->dwMemoryLoad = pmmi->dwMemoryLoad;
		iTmp = pmmi->dwAvailPhys - pThis->dwAvailPhys;
		pThis->iDiffPhys = (0 == iTmp)?(pThis->iDiffPhys):(iTmp);
		pThis->dwTotalPhys = pmmi->dwTotalPhys; 
		pThis->dwAvailPhys = pmmi->dwAvailPhys;
		iTmp = pmmi->dwAvailPageFile - pThis->dwAvailPageFile;
		pThis->iDiffPageFile = (0 == iTmp)?(pThis->iDiffPageFile):(iTmp);
		pThis->dwTotalPageFile = pmmi->dwTotalPageFile; 
		pThis->dwAvailPageFile = pmmi->dwAvailPageFile;
		iTmp = pmmi->dwAvailVirtual - pThis->dwAvailVirtual;
		pThis->iDiffVirtual = (0 == iTmp)?(pThis->iDiffVirtual):(iTmp);
		pThis->dwTotalVirtual = pmmi->dwTotalVirtual; 
		pThis->dwAvailVirtual = pmmi->dwAvailVirtual;
		
		if( pThis->iDiffPhys == pThis->dwAvailPhys )
		{	///first time!!
			pThis->iDiffPhys = 0;
			pThis->iDiffPageFile = 0;
			pThis->iDiffVirtual = 0;
		}
		
		bRet = TRUE;
	}
	
	return bRet;
}



static BOOL spLibCopyMemoryInfo( LibSysInfoMemoryContent* pTar, LibSysInfoMemoryContent* pSrc )
{
	BOOL bRet = FALSE;
	
	if( pTar && pSrc )
	{
		pTar->dwLength = pSrc->dwLength; 
		pTar->dwMemoryLoad = pSrc->dwMemoryLoad; 
		pTar->dwTotalPhys = pSrc->dwTotalPhys; 
		pTar->dwAvailPhys = pSrc->dwAvailPhys; 
		pTar->iDiffPhys = pSrc->iDiffPhys; 
		pTar->dwTotalPageFile = pSrc->dwTotalPageFile; 
		pTar->dwAvailPageFile = pSrc->dwAvailPageFile; 
		pTar->iDiffPageFile = pSrc->iDiffPageFile; 
		pTar->dwTotalVirtual = pSrc->dwTotalVirtual; 
		pTar->dwAvailVirtual = pSrc->dwAvailVirtual; 	
		pTar->iDiffVirtual = pSrc->iDiffVirtual; 

		bRet = TRUE;
	}
	
	return bRet;
}
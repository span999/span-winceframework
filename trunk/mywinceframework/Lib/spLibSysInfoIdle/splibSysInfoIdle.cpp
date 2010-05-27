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
typedef struct LibSysInfoIdleContent
{
	HANDLE hWDTimer;
	DWORD dwInterval;	///every second
	DWORD dwStartTick;
	DWORD dwStopTick;
	DWORD dwIdleSt;
	DWORD dwIdleEd;
	DWORD PercentIdle;
	DWORD totalPercentIdle;
	DWORD totalCount;
} LibSysInfoIdleContent;


typedef DWORD (*PFN_GetIdleTime)(void);

///phototype
static BOOL spLibInitContent( LibSysInfoIdleContent* pThis );
static BOOL spLibLoadMyModule( void );
static BOOL spLibFirstLoad( void );
static DWORD spLibCalcIdlePercent( LibSysInfoIdleContent* pThis );
static DWORD spLibCalcIdlePercentEx( LibSysInfoIdleContent* pThis );
static DWORD spLibCalcIdleAvgPercent( LibSysInfoIdleContent* pThis );



///
static PFN_GetIdleTime gpfGetIdleTime;

static LibSysInfoIdleContent ThisContent;
static LibSysInfoIdleContent *pThisContent = &ThisContent;


/// *****************************************
/// interface functions
/// *****************************************

DWORD spLibSysInfoIdle_Init( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibSysInfoIdle_Init: %d \r\n", 1) );
	
	
	if( spLibInitContent( pThisContent ) && spLibLoadMyModule() )
		dwRet = 0;
	else
		dwRet = (-1);

		
	SPDMSG( dINIT, (L"$$$spLibSysInfoIdle_Init: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibSysInfoIdle_Deinit( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibSysInfoIdle_Deinit: %d \r\n", 1) );
	
	spLibInitContent( pThisContent );
	
	SPDMSG( dINIT, (L"$$$spLibSysInfoIdle_Deinit: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibSysInfoIdle_Get( void )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibSysInfoIdle_Get: %d \r\n", 1) );
	
	dwRet = spLibCalcIdlePercent( pThisContent );
	
	SPDMSG( dINFO, (L"$$$spLibSysInfoIdle_Get: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibSysInfoIdle_GetAvg( void )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibSysInfoIdle_GetAvg: %d \r\n", 1) );
	
	dwRet = spLibCalcIdlePercent( pThisContent );
	dwRet = spLibCalcIdleAvgPercent( pThisContent );
	
	SPDMSG( dINFO, (L"$$$spLibSysInfoIdle_GetAvg: %d \r\n", 0) );
	return dwRet;
}



/// *****************************************
/// internal functions
/// *****************************************
static BOOL spLibFirstLoad( void )
{
	BOOL bRet = TRUE;
	
	if( (0 == pThisContent->dwStartTick) || (0 == pThisContent->dwStartTick) )
		bRet = TRUE;
	else
		bRet = FALSE;
		
	return bRet;
}


static BOOL spLibInitContent( LibSysInfoIdleContent* pThis )
{
	BOOL bRet = TRUE;

	pThis->hWDTimer = NULL;
	pThis->dwInterval = 0;	///every second
	pThis->dwStartTick = 0;
	pThis->dwStopTick = 0;
	pThis->dwIdleSt = 0;
	pThis->dwIdleEd = 0;
	pThis->PercentIdle = 0;
	pThis->totalPercentIdle = 0;
	pThis->totalCount = 0;
	
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


static DWORD spLibCalcIdlePercent( LibSysInfoIdleContent* pThis )
{
	DWORD dwRet = 0;
	
	if( spLibFirstLoad() )
	{
		pThis->dwStartTick = GetTickCount();
		pThis->dwIdleSt = gpfGetIdleTime();

		dwRet = 0;
	}
	else
	{
		pThis->dwStopTick = GetTickCount();
		pThis->dwIdleEd = gpfGetIdleTime();
		
		dwRet = spLibCalcIdlePercentEx( pThis );

		pThis->dwStartTick = GetTickCount();
		pThis->dwIdleSt = gpfGetIdleTime();
		
	}
	
	return dwRet;
}


static DWORD spLibCalcIdlePercentEx( LibSysInfoIdleContent* pThis )
{
	DWORD dwRet = 0;
	
	pThis->PercentIdle = ( (100*(pThis->dwIdleEd - pThis->dwIdleSt) ) / (pThis->dwStopTick - pThis->dwStartTick));
	
	dwRet = pThis->PercentIdle;

	pThis->totalPercentIdle = pThis->totalPercentIdle + pThis->PercentIdle;
	pThis->totalCount++;
	
	return dwRet;
}


static DWORD spLibCalcIdleAvgPercent( LibSysInfoIdleContent* pThis )
{
	DWORD dwRet = 0;
	
	dwRet = 100 - (pThis->totalPercentIdle/pThis->totalCount);
	if( (dwRet > 100) || (dwRet < 0) )
		dwRet = 0;
       				
	/// clear all
	pThis->totalPercentIdle = 0;
	pThis->totalCount = 0;
	
	return dwRet;
}

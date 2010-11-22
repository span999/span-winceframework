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

#include "..\..\Inc\spLibErrCodeDef.h"
#include "SPDebugDef.h"


///
typedef struct LibProgAliveInfoContent
{
	HANDLE hCheckEvent;
	HANDLE hResponseEvent;
} LibProgAliveInfoContent;





typedef DWORD (*PFN_GetIdleTime)(void);

///phototype
static BOOL spLibInitContent( LibProgAliveInfoContent* pThis );
///static BOOL spLibLoadMyModule( void );
static BOOL spLibFirstLoad( void );
static BOOL spLibDeInitContent( LibProgAliveInfoContent* pThis );
static BOOL spLibInitEvent( LibProgAliveInfoContent* pThis );

///
///static PFN_GetIdleTime gpfGetIdleTime;

static LibProgAliveInfoContent ThisContent;
static LibProgAliveInfoContent *pThisContent = &ThisContent;


/// *****************************************
/// interface functions
/// *****************************************

DWORD spLibProgAliveInfo_Init( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibProgAliveInfo_Init: %d \r\n", 1) );
	
	
	if( spLibInitContent( pThisContent ) && spLibInitEvent( pThisContent ) )
		dwRet = 0;
	else
		dwRet = (-1);

		
	SPDMSG( dINIT, (L"$$$spLibProgAliveInfo_Init: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibProgAliveInfo_Deinit( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibProgAliveInfo_Deinit: %d \r\n", 1) );
	
	spLibDeInitContent( pThisContent );
	
	SPDMSG( dINIT, (L"$$$spLibProgAliveInfo_Deinit: %d \r\n", 0) );
	return dwRet;
}


HANDLE spLibProgAliveInfo_GetCheckEvent( void )
{
	HANDLE hRet = NULL;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibProgAliveInfo_GetCheckEvent: %d \r\n", 1) );
	
	hRet = pThisContent->hCheckEvent;
	
	SPDMSG( dINFO, (L"$$$spLibProgAliveInfo_GetCheckEvent: %d \r\n", 0) );
	return hRet;
}

HANDLE spLibProgAliveInfo_GetResponseEvent( void )
{
	HANDLE hRet = NULL;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibProgAliveInfo_GetCheckEvent: %d \r\n", 1) );
	
	hRet = pThisContent->hResponseEvent;
	
	SPDMSG( dINFO, (L"$$$spLibProgAliveInfo_GetCheckEvent: %d \r\n", 0) );
	return hRet;
}




/// *****************************************
/// internal functions
/// *****************************************
static BOOL spLibFirstLoad( void )
{
	BOOL bRet = TRUE;
	
	if( (NULL == pThisContent->hCheckEvent) || (NULL == pThisContent->hResponseEvent) )
		bRet = TRUE;
	else
		bRet = FALSE;
		
	return bRet;
}


static BOOL spLibInitContent( LibProgAliveInfoContent* pThis )
{
	BOOL bRet = TRUE;

	pThis->hCheckEvent = NULL;
	pThis->hResponseEvent = NULL;
	
	return bRet;
}

static BOOL spLibDeInitContent( LibProgAliveInfoContent* pThis )
{
	BOOL bRet = TRUE;

	CloseHandle( pThis->hCheckEvent );
	CloseHandle( pThis->hResponseEvent );
	
	pThis->hCheckEvent = NULL;
	pThis->hResponseEvent = NULL;
	
	return bRet;
}


static BOOL spLibInitEvent( LibProgAliveInfoContent* pThis )
{
	BOOL bRet = TRUE;

	if( NULL != pThis )
	{	
		pThis->hCheckEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		pThis->hResponseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	else
		bRet = FALSE;
	
	return bRet;
}


/*
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
*/


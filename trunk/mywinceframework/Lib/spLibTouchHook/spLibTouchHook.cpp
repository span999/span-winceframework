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
#include "spOS.h"
#include "spPlatform.h"
#include "spCommon.h"
#include "SPDebugDef.h"
#include "spDllTouchHook.h"
#include "spLibTouchHook.h"


///
typedef struct _LibTouchHookContent
{
	HANDLE 							hTouchEventDown;
	HANDLE 							hTouchEventMove;
	HANDLE 							hTouchEventUp;
	HANDLE 							hTouchEventControl;
	PFN_TOUCH_HOOK_LIB_CALLBACK		pfnTouchHookLibCallback;
	HANDLE							hMonitorThread;
	DWORD							dwInitStep;
} LibTouchHookContent;


#define		SPPREFIX			TEXT("THLib:")
#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
#define		LIBMSGFLAG_NK		(dNOUT|0x0FFFFFFF)		///nk msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only



///phototype
static BOOL spLibInitContent( LibTouchHookContent* pThis );
///static BOOL spLibLoadMyModule( void );
///static BOOL spLibFirstLoad( void );
static BOOL spLibDeInitContent( LibTouchHookContent* pThis );
static BOOL spLibInitEvent( LibTouchHookContent* pThis );
static BOOL spLibInitCreateThread( LibTouchHookContent* pThis );
static DWORD WINAPI TouchEventThread( LPVOID  pContext );
static void spGetEventIn( HANDLE hEvent, TOUCH_EVENT_DATA *pteD );


static LibTouchHookContent ThisContent;
static LibTouchHookContent *pThisContent = &ThisContent;
static TOUCH_EVENT_DATA teData;
static TOUCH_EVENT_DATA *pteData = &teData;



/// *****************************************
/// interface functions
/// *****************************************

DWORD spLibTouchHook_Init( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibTouchHook_Init: %d \r\n", 1) );
	
	if( (1 != pThisContent->dwInitStep) && (2 != pThisContent->dwInitStep) && (3 != pThisContent->dwInitStep) )
	{
		if( spLibInitContent( pThisContent ) && spLibInitEvent( pThisContent ) )
		{
			///create thread
			if( spLibInitCreateThread( pThisContent ) )
				dwRet = 0;
			else
				dwRet = (-1);

			dwRet = 0;
		}
		else
			dwRet = (-1);
	}

	if( 0 != dwRet )
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibTouchHook_Init fail !!!"), SPPREFIX );
		
	SPDMSG( dINIT, (L"$$$spLibTouchHook_Init: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibTouchHook_Deinit( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibTouchHook_Deinit: %d \r\n", 1) );
	
	spLibDeInitContent( pThisContent );
	
	SPDMSG( dINIT, (L"$$$spLibTouchHook_Deinit: %d \r\n", 0) );
	return dwRet;
}


BOOL spLibTouchHook_SetCallback( PFN_TOUCH_HOOK_LIB_CALLBACK pfnTouchHookLibCB )
{
	BOOL bRet = FALSE;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibTouchHook_SetCallback: %d \r\n", 1) );
	
	if( pfnTouchHookLibCB )
	{
		pThisContent->pfnTouchHookLibCallback = pfnTouchHookLibCB;
		bRet = TRUE;
	}
	
	SPDMSG( dINFO, (L"$$$spLibTouchHook_SetCallback: %d \r\n", 0) );
	return bRet;
}





/// *****************************************
/// internal functions
/// *****************************************
/*
static BOOL spLibFirstLoad( void )
{
	BOOL bRet = TRUE;
	
	if( (NULL == pThisContent->hCheckEvent) || (NULL == pThisContent->hResponseEvent) )
		bRet = TRUE;
	else
		bRet = FALSE;
		
	return bRet;
}
*/


static BOOL spLibInitContent( LibTouchHookContent* pThis )
{
	BOOL bRet = TRUE;

	pThis->hTouchEventDown = NULL;
	pThis->hTouchEventMove = NULL;
	pThis->hTouchEventUp = NULL;
	pThis->hTouchEventControl = NULL;
	pThis->pfnTouchHookLibCallback = NULL;
	pThis->hMonitorThread = NULL;
	
	pteData->teType = TOUCH_NONE_EVENT;
	pteData->tiX = 0;
	pteData->tiY = 0;

	pThis->dwInitStep = 1;
	
	return bRet;
}



static BOOL spLibDeInitContent( LibTouchHookContent* pThis )
{
	BOOL bRet = TRUE;

	SetEvent( pThis->hTouchEventControl );
	Sleep( 500 );
	
	CloseHandle( pThis->hTouchEventDown );
	CloseHandle( pThis->hTouchEventMove );
	CloseHandle( pThis->hTouchEventUp );
	CloseHandle( pThis->hTouchEventControl );	
	
	pThis->hTouchEventDown = NULL;
	pThis->hTouchEventMove = NULL;
	pThis->hTouchEventUp = NULL;
	pThis->hTouchEventControl = NULL;
	pThis->pfnTouchHookLibCallback = NULL;
	
	pThis->dwInitStep = 0;
	
	return bRet;
}


static BOOL spLibInitEvent( LibTouchHookContent* pThis )
{
	BOOL bRet = TRUE;

	if( NULL != pThis )
	{	
		pThis->hTouchEventDown = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_DOWN_EVENT_NAME );
		pThis->hTouchEventMove = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_MOVE_EVENT_NAME );
		pThis->hTouchEventUp = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_UP_EVENT_NAME );
		pThis->hTouchEventControl = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_CONTROL_EVENT_NAME );
		pThis->dwInitStep = 2;
	}
	else
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitEvent fail !!!"), SPPREFIX );
		bRet = FALSE;
	}
	
	return bRet;
}


static BOOL spLibInitCreateThread( LibTouchHookContent* pThis )
{
	BOOL bRet = TRUE;
	DWORD dwThreadId = 0;
	DWORD dwThreadPararm = 0;
	
	pThis->hMonitorThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)TouchEventThread, (LPVOID)&dwThreadPararm, 0, (LPDWORD)&dwThreadId );
	
	if( NULL == pThis->hMonitorThread )
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitCreateThread fail !!!"), SPPREFIX );
		pThis->dwInitStep = 0;
	}
	else	
		pThis->dwInitStep = 3;
	
	return bRet;
}


static DWORD WINAPI TouchEventThread( LPVOID pContext )
{
#define		WAITEVENT_NUM		4

	DWORD dwRet = 0;
	DWORD dwReturns = 0;
	BOOL bExitMonitor = FALSE;
	HANDLE hWaitEvents[WAITEVENT_NUM];
	DWORD dwWaitMS = INFINITE;
	
	hWaitEvents[0] = pThisContent->hTouchEventDown;
	hWaitEvents[1] = pThisContent->hTouchEventMove;
	hWaitEvents[2] = pThisContent->hTouchEventUp;
	hWaitEvents[3] = pThisContent->hTouchEventControl;
	
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("%s TouchEventThread start !!!"), SPPREFIX );
	
	while( !bExitMonitor )
	{
		dwReturns = WaitForMultipleObjects( WAITEVENT_NUM, hWaitEvents, FALSE, dwWaitMS );
		
		switch( dwReturns )
		{
			case WAIT_OBJECT_0 + 0:
				/// got touch DOWN event
				pteData->teType = TOUCH_DOWN_EVENT;
				spGetEventIn( hWaitEvents[0], pteData );
				break;
			case WAIT_OBJECT_0 + 1:
				/// got touch MOVE event
				pteData->teType = TOUCH_MOVE_EVENT;
				spGetEventIn( hWaitEvents[1], pteData );
				break;
			case WAIT_OBJECT_0 + 2:
				/// got touch UP event
				pteData->teType = TOUCH_UP_EVENT;
				spGetEventIn( hWaitEvents[2], pteData );
				break;
			case WAIT_OBJECT_0 + 3:
				/// got control event
				pteData->teType = TOUCH_NONE_EVENT;
				spGetEventIn( hWaitEvents[3], pteData );
				bExitMonitor = TRUE;
				break;
			case WAIT_TIMEOUT:
				break;
			default:
				break;
				
		}	///switch
		
	}	///while
	
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("%s TouchEventThread exit !!!"), SPPREFIX );
	
	return dwRet;
}


static void spGetEventIn( HANDLE hEvent, TOUCH_EVENT_DATA *pteD )
{
	DWORD dwData = 0;
	TOUCH_EVENT_DATA Data;
	
	dwData = GetEventData( hEvent );
	
	Data.teType = pteD->teType;
	Data.tiX = pteD->tiX = (INT)(dwData & 0x0000FFFF);
	Data.tiY = pteD->tiY = (INT)((dwData>>16) & 0x0000FFFF);
	
	if( pThisContent->pfnTouchHookLibCallback )
		pThisContent->pfnTouchHookLibCallback( Data );
		
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


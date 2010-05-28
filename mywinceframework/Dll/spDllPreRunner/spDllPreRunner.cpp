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


#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
#define		LIBMSGFLAG_NK		(dNOUT|0x0FFFFFFF)		///NK msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only



static DWORD MainRoutine( DWORD dwPararm );
static DWORD WINAPI StoreageMsgWaitThread( LPVOID  pContext );

static BOOL IsAllThreadStop( void );





/// *****************************************
/// routine
/// *****************************************


BOOL WINAPI  DllEntry(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DEBUGREGISTER(hinstDll);
        SPDMSG(dINIT, (_T("SPD@%s() : Process Attach\r\n"), _T(__FUNCTION__)));
		///spLibDbgMsg_Dlg( TEXT("spDllPreRunner DLL_PROCESS_ATTACH!!!") );
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        SPDMSG(dINIT, (_T("SPD@%s() : Process Detach\r\n"), _T(__FUNCTION__)));
		///spLibDbgMsg_Dlg( TEXT("spDllPreRunner DLL_PROCESS_DETACH!!!") );
    }

    return TRUE;
}


DWORD spDllHook_Start( DWORD dwParam )
{
	DWORD dwRet;
	
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("spDllPreRunner_Start 1!!!") );
	dwRet = MainRoutine( dwParam );
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("spDllPreRunner_Start 2!!!") );
	
	return dwRet;
}


///BLOCK_DRIVER_GUID
///DEVCLASS_CARDSERV_GUID
///FATFS_MOUNT_GUID
///STORE_MOUNT_GUID
///STOREMGR_DRIVER_GUID
///CE_DRIVER_SD_BUS_GUID


static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;
	HANDLE hStoreageMsgWaitThread = NULL;
	DWORD dwStoreageMsgWaitThreadId = 0;
///	hStoreageMsgWaitThread = NULL;
///	DWORD dwStoreageMsgWaitThreadId = 0;
	DWORD dwThreadPararm = 0;
	
	///hStoreageMsgWaitThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)StoreageMsgWaitThread, (LPVOID)&dwThreadPararm, 0, (LPDWORD)&dwStoreageMsgWaitThreadId );

	
	return dwRet;
}


static DWORD WINAPI StoreageMsgWaitThread( LPVOID  pContext )
{
	DWORD dwRet = 0;

	return dwRet;
}

static BOOL IsAllThreadStop( void )
{
	BOOL bRet = FALSE; 
	
	
	return bRet;
}



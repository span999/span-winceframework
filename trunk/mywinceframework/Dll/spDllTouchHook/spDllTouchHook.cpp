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

#include <Tchddi.h>


#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only
#define		SPPREFIX			TEXT("TCHHDll:")
#define		TOUCHDLLNAME		TEXT("touch.dll")


typedef BOOL (*PFN_TOUCH_PANEL_ENABLE)( PFN_TOUCH_PANEL_CALLBACK pfnCallback );



static PFN_TOUCH_PANEL_ENABLE pfnTouchPanelEnable = NULL;



BOOL spLoadHook( HMODULE hTouchDll );

static HMODULE hTouchDll = 0;

BOOL TouchIOControl(	
					DWORD dwIoControlCode, 
					LPVOID lpInBuffer, 
					DWORD nInBufferSize,
					LPVOID lpOutBuffer, 
					DWORD nOutBufferSize, 
					LPDWORD lpBytesReturned
)
{
	BOOL bRet = FALSE;
	
	return bRet;
}


BOOL TouchPanelEnable( PFN_TOUCH_PANEL_CALLBACK    pfnCallback )
{
	BOOL bRet = FALSE;
	
	return bRet;
}


VOID TouchPanelDisable( VOID )
{
	return;
}


BOOL TouchPanelGetDeviceCaps( INT iIndex, LPVOID lpOutput )
{
	BOOL bRet = FALSE;
	
	return bRet;
}


void TouchPanelPowerHandler( BOOL bOff )
{
    return;
}


BOOL TouchPanelReadCalibrationPoint( INT *pRawX, INT *pRawY )
{
	BOOL bRet = FALSE;
	
	return bRet;
}


VOID TouchPanelReadCalibrationAbort( VOID )
{
	return;
}


BOOL TouchPanelSetMode( INT iIndex, LPVOID lpInput )
{
	BOOL bRet = FALSE;
	
	return bRet;
}


BOOL TouchPanelSetCalibration(
								INT32 cCalibrationPoints,
								INT32* pScreenXBuffer,
								INT32* pScreenYBuffer,
								INT32* pUncalXBuffer,
								INT32* pUncalYBuffer 
)
{
	BOOL bRet = FALSE;
	
	return bRet;
}


VOID TouchPanelCalibrateAPoint(
								INT32 UncalX,
								INT32 UncalY,
								INT32* pCalX,
								INT32* pCalY 
)
{
	return;
}





BOOL APIENTRY TouchPanelDllEntry( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
 )
{
	switch ( ul_reason_for_call )
	{

		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls((HMODULE) hModule);
			hTouchDll = LoadLibrary( TOUCHDLLNAME );
			if( NULL != hTouchDll )
				spLoadHook( hTouchDll );
			break;
		case DLL_PROCESS_DETACH:
			if( NULL != hTouchDll )
				FreeLibrary( hTouchDll );
			break;
	}
    return TRUE;

}


BOOL spLoadHook( HMODULE hTouchDll )
{
	BOOL bRet = FALSE;

	if( hTouchDll )
	{
		if (NULL == pfnTouchPanelEnable)
		{
			pfnTouchPanelEnable = (PFN_TOUCH_PANEL_ENABLE)GetProcAddress(hTouchDll, TEXT("TouchPanelEnable"));
		}

	}
	
	return bRet;
}
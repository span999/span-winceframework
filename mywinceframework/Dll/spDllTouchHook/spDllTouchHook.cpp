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
#include "spLibTouchHook.h"

#include <Tchddi.h>


#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only
#define		SPPREFIX			TEXT("TCHHDll:")
#define		TOUCHDLLNAME		TEXT("touch.dll")


typedef BOOL (*PFN_TOUCH_PANEL_ENABLE)( PFN_TOUCH_PANEL_CALLBACK pfnCallback );
typedef VOID (*PFN_TOUCH_PANEL_DISABLE)( VOID );
typedef BOOL (*PFN_TOUCH_PANEL_GET_DEVICE_CAPS)( INT iIndex, LPVOID lpOutput );
typedef void (*PFN_TOUCH_PANEL_POWER_HANDLER)( BOOL bOff );
typedef BOOL (*PFN_TOUCH_PANEL_READ_CALIBRATION_POINT)( INT *pRawX, INT *pRawY );
typedef VOID (*PFN_TOUCH_PANEL_READ_CALIBRATION_ABORT)( VOID );
typedef BOOL (*PFN_TOUCH_PANEL_SET_MODE)( INT iIndex, LPVOID lpInput );
typedef BOOL (*PFN_TOUCH_PANEL_SET_CALIBRATION)( INT32 cCalibrationPoints, INT32* pScreenXBuffer, INT32* pScreenYBuffer, INT32* pUncalXBuffer, INT32* pUncalYBuffer );
typedef VOID (*PFN_TOUCH_PANEL_CALIBRATEA_POINT)( INT32 UncalX, INT32 UncalY, INT32* pCalX, INT32* pCalY );
typedef BOOL (*PFN_TOUCH_IO_CONTROL)( DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned );


static PFN_TOUCH_PANEL_ENABLE 					pfnTouchPanelEnable = NULL;
static PFN_TOUCH_PANEL_DISABLE 					pfnTouchPanelDisable = NULL;
static PFN_TOUCH_PANEL_GET_DEVICE_CAPS 			pfnTouchPanelGetDeviceCaps = NULL;
static PFN_TOUCH_PANEL_POWER_HANDLER 			pfnTouchPanelPowerHandler = NULL;
static PFN_TOUCH_PANEL_READ_CALIBRATION_POINT 	pfnTouchPanelReadCalibrationPoint = NULL;
static PFN_TOUCH_PANEL_READ_CALIBRATION_ABORT 	pfnTouchPanelReadCalibrationAbort = NULL;
static PFN_TOUCH_PANEL_SET_MODE 				pfnTouchPanelSetMode = NULL;
static PFN_TOUCH_PANEL_SET_CALIBRATION 			pfnTouchPanelSetCalibration = NULL;
static PFN_TOUCH_PANEL_CALIBRATEA_POINT 		pfnTouchPanelCalibrateAPoint = NULL;
static PFN_TOUCH_IO_CONTROL 					pfnTouchIOControl = NULL;

static PFN_TOUCH_PANEL_CALLBACK					pfnOrgTouchPanelCallback = NULL;

static BOOL spLoadHook( HMODULE hTouchDll );
static BOOL spCreateHookEvent( void );
static BOOL spCloseHookEvent( void );
static void spSendEventOut( HANDLE hEvent, DWORD dwData );



static HMODULE hTouchDll = 0;
static HANDLE hTouchHookEvents[3];
static INT xSaved = 0;
static INT ySaved = 0;
static int iMinX = 4;
static int iMinY = 4;


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
	
	bRet = pfnTouchIOControl( dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned );
	
	return bRet;
}

/*
LPARAM MAKELPARAM(
    WORD wLow,
    WORD wHigh
);
*/

BOOL myTouchPanelCallback( TOUCH_PANEL_SAMPLE_FLAGS Flags, INT X, INT Y )
{
	BOOL bRet = FALSE;
	
	if( Flags == (TouchSampleDownFlag | TouchSampleIsCalibratedFlag | TouchSampleValidFlag) )
	{	///down
		///SendMessage(g_hwWnd, WM_LBUTTONDOWN, 0, MAKELPARAM(X,Y));
		spSendEventOut( hTouchHookEvents[0], MAKELPARAM(X,Y) );
	}
	else
	if( Flags == (TouchSampleDownFlag | TouchSamplePreviousDownFlag | TouchSampleIsCalibratedFlag | TouchSampleValidFlag) &&
		((xSaved - X > iMinX) || (X - xSaved > iMinX)) &&
		((ySaved - Y > iMinY) || (Y - ySaved > iMinY))
	)
	{	///move
		///SendMessage(g_hwWnd, WM_MOUSEMOVE, 0, MAKELPARAM(X,Y));
		spSendEventOut( hTouchHookEvents[1], MAKELPARAM(X,Y) );
	}
	else
	if( Flags ==(TouchSampleIsCalibratedFlag | TouchSampleValidFlag | TouchSamplePreviousDownFlag) )
	{	///up
		///SendMessage(g_hwWnd, WM_LBUTTONUP, 0, MAKELPARAM(X,Y));
		spSendEventOut( hTouchHookEvents[2], MAKELPARAM(X,Y) );
	}

	///update stored
	xSaved = X;
	ySaved = Y;
	
	///original callback
	if( pfnOrgTouchPanelCallback )
		bRet = pfnOrgTouchPanelCallback( Flags, X, Y );
	
	return bRet;
}


BOOL TouchPanelEnable( PFN_TOUCH_PANEL_CALLBACK pfnCallback )
{
	BOOL bRet = FALSE;
	
	///store callback
	pfnOrgTouchPanelCallback = pfnCallback;
	
	///replace with my callback function
	bRet = pfnTouchPanelEnable( myTouchPanelCallback );
	
	return bRet;
}


VOID TouchPanelDisable( VOID )
{
	pfnTouchPanelDisable();
	return;
}


BOOL TouchPanelGetDeviceCaps( INT iIndex, LPVOID lpOutput )
{
	BOOL bRet = FALSE;
	
	bRet = pfnTouchPanelGetDeviceCaps( iIndex, lpOutput );
	
	return bRet;
}


void TouchPanelPowerHandler( BOOL bOff )
{
	pfnTouchPanelPowerHandler( bOff );
    return;
}


BOOL TouchPanelReadCalibrationPoint( INT *pRawX, INT *pRawY )
{
	BOOL bRet = FALSE;
	
	bRet = pfnTouchPanelReadCalibrationPoint( pRawX, pRawY );
	
	return bRet;
}


VOID TouchPanelReadCalibrationAbort( VOID )
{
	pfnTouchPanelReadCalibrationAbort();
	return;
}


BOOL TouchPanelSetMode( INT iIndex, LPVOID lpInput )
{
	BOOL bRet = FALSE;
	
	bRet = pfnTouchPanelSetMode( iIndex, lpInput );
	
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
	
	bRet = pfnTouchPanelSetCalibration( cCalibrationPoints, pScreenXBuffer, pScreenYBuffer, pUncalXBuffer, pUncalYBuffer );
	
	return bRet;
}


VOID TouchPanelCalibrateAPoint(
								INT32 UncalX,
								INT32 UncalY,
								INT32* pCalX,
								INT32* pCalY 
)
{
	pfnTouchPanelCalibrateAPoint( UncalX, UncalY, pCalX, pCalY );
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
			spCreateHookEvent();
			break;
		case DLL_PROCESS_DETACH:
			if( NULL != hTouchDll )
				FreeLibrary( hTouchDll );
			spCloseHookEvent();	
			break;
	}
    return TRUE;

}


static BOOL spLoadHook( HMODULE hTouchDll )
{
	BOOL bRet = FALSE;

	if( hTouchDll )
	{
		if( NULL == pfnTouchPanelEnable )
		{
			pfnTouchPanelEnable = (PFN_TOUCH_PANEL_ENABLE)GetProcAddress(hTouchDll, TEXT("TouchPanelEnable"));
		}
		if( NULL == pfnTouchPanelDisable )
		{
			pfnTouchPanelDisable = (PFN_TOUCH_PANEL_DISABLE)GetProcAddress(hTouchDll, TEXT("TouchPanelDisable"));
		}
		if( NULL == pfnTouchPanelGetDeviceCaps )
		{
			pfnTouchPanelGetDeviceCaps = (PFN_TOUCH_PANEL_GET_DEVICE_CAPS)GetProcAddress(hTouchDll, TEXT("TouchPanelGetDeviceCaps"));
		}
		if( NULL == pfnTouchPanelPowerHandler )
		{
			pfnTouchPanelPowerHandler = (PFN_TOUCH_PANEL_POWER_HANDLER)GetProcAddress(hTouchDll, TEXT("TouchPanelPowerHandler"));
		}
		if( NULL == pfnTouchPanelReadCalibrationPoint )
		{
			pfnTouchPanelReadCalibrationPoint = (PFN_TOUCH_PANEL_READ_CALIBRATION_POINT)GetProcAddress(hTouchDll, TEXT("TouchPanelReadCalibrationPoint"));
		}
		if( NULL == pfnTouchPanelReadCalibrationAbort )
		{
			pfnTouchPanelReadCalibrationAbort = (PFN_TOUCH_PANEL_READ_CALIBRATION_ABORT)GetProcAddress(hTouchDll, TEXT("TouchPanelReadCalibrationAbort"));
		}
		if( NULL == pfnTouchPanelSetMode )
		{
			pfnTouchPanelSetMode = (PFN_TOUCH_PANEL_SET_MODE)GetProcAddress(hTouchDll, TEXT("TouchPanelSetMode"));
		}
		if( NULL == pfnTouchPanelSetCalibration )
		{
			pfnTouchPanelSetCalibration = (PFN_TOUCH_PANEL_SET_CALIBRATION)GetProcAddress(hTouchDll, TEXT("TouchPanelSetCalibration"));
		}
		if( NULL == pfnTouchPanelCalibrateAPoint )
		{
			pfnTouchPanelCalibrateAPoint = (PFN_TOUCH_PANEL_CALIBRATEA_POINT)GetProcAddress(hTouchDll, TEXT("TouchPanelCalibrateAPoint"));
		}
		if( NULL == pfnTouchIOControl )
		{
			pfnTouchIOControl = (PFN_TOUCH_IO_CONTROL)GetProcAddress(hTouchDll, TEXT("TouchIOControl"));
		}

	}
	
	return bRet;
}


static BOOL spCreateHookEvent( void )
{
	static bInited = FALSE;
	BOOL bRet = FALSE;
	
	if( !bInited )
	{
		hTouchHookEvents[0] = NULL;
		hTouchHookEvents[1] = NULL;
		hTouchHookEvents[2] = NULL;
		
		///down
		hTouchHookEvents[0] = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_DOWN_EVENT_NAME );
		///move
		hTouchHookEvents[1] = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_MOVE_EVENT_NAME );
		///up
		hTouchHookEvents[2] = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_UP_EVENT_NAME );
	
		if( NULL != hTouchHookEvents[0] && NULL != hTouchHookEvents[1] && NULL != hTouchHookEvents[2] )
			bInited = TRUE;
		else
			bInited = FALSE;
			
		bRet = bInited;
	}
	
	return bRet;
}


static BOOL spCloseHookEvent( void )
{
	BOOL bRet = TRUE;
	
	CloseHandle( hTouchHookEvents[0] );
	CloseHandle( hTouchHookEvents[1] );
	CloseHandle( hTouchHookEvents[2] );
		
	return bRet;
}


static void spSendEventOut( HANDLE hEvent, DWORD dwData )
{
	SetEventData( hEvent, dwData );
	SetEvent( hEvent );
}
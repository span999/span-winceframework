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
#include "..\..\Inc\spLibSysInfoIdle.h"

#include "SPDebugDef.h"


#define 	START_SPEXEKEEPCPUBUSY_EVENT_NAME			TEXT("named start spExeKeepCPUBusy event")
#define 	EXIT_SPEXEKEEPCPUBUSY_EVENT_NAME			TEXT("named exit spExeKeepCPUBusy event")
#define 	RESET_SPEXEKEEPCPUBUSY_EVENT_NAME			TEXT("named reset spExeKeepCPUBusy event")



static DWORD MainRoutine( DWORD dwPararm );
static void ReleaseAlls( void );
static void ModifyCPULoading( DWORD dwValue );


static HANDLE	hStartEvent = NULL;
static HANDLE	hExitEvent = NULL;
static HANDLE	hResetEvent = NULL;
static BOOL 	bExitRoutine = FALSE;


/// *****************************************
/// routine
/// *****************************************




int WINAPI WinMain (
              HINSTANCE hInstance,    // Handle to the current instance
              HINSTANCE hPrevInstance,// Handle to the previous instance
              LPTSTR lpCmdLine,       // Pointer to the command line
              int nCmdShow)           // Show state of the window
{
	DWORD dwErrCode = 0;

	hStartEvent = CreateEvent( NULL, FALSE, FALSE, START_SPEXEKEEPCPUBUSY_EVENT_NAME );
	dwErrCode = GetLastError();
	
	if( NULL != hStartEvent )
	{
		hExitEvent = CreateEvent( NULL, FALSE, FALSE, EXIT_SPEXEKEEPCPUBUSY_EVENT_NAME );
		
		if( ERROR_ALREADY_EXISTS == dwErrCode )
		{
			int RetMsgBox = 0;
			
			RetMsgBox = MessageBox( NULL, TEXT("2nd Launch will exit CPU Busy !!!"), TEXT("MainRoutine"), MB_ICONQUESTION|MB_OKCANCEL );
			
			if( IDOK == RetMsgBox )
			{
				///cpu utilization program already running, get it down!!
				SetEvent( hExitEvent );
				SPDMSG( dINIT, (L"SPD@spExeKeepCPUBusy: set exit event-%d \r\n", 0 ) );
				///wait 3 seconds
				Sleep( 3000 );
			
				ReleaseAlls();
				SPDMSG( dINIT, (L"SPD@spExeKeepCPUBusy: set exit event done-%d \r\n", 1 ) );
			}	
		}
		else
		{
			///init
			spLibSysInfoIdle_Init( 0 );
			
			
			///start cpu utilization program
			MainRoutine( 0 );
			
			ReleaseAlls();
			SPDMSG( dINIT, (L"SPD@spExeKeepCPUBusy: exit done-%d \r\n", 0 ) );
		}	
	}
	else
	{
		SPDMSG( dFAIL, (L"SPD@spExeKeepCPUBusy: CreateEvent fail-%d \r\n", GetLastError() ) );
	}

	return 0;
}


static void ReleaseAlls( void )
{

	if( NULL != hStartEvent )
		CloseHandle( hStartEvent );
		
	if( NULL != hExitEvent )
		CloseHandle( hExitEvent );

	if( NULL != hResetEvent )
		CloseHandle( hResetEvent );

	spLibSysInfoIdle_Deinit( 0 );
	

}



static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;
	DWORD dwReturns = 0;
	
	
	#define WAITEVENT_NUM	3
	HANDLE hWaitEvents[WAITEVENT_NUM];
///	DWORD dwWaitMS = INFINITE;
	DWORD dwWaitMS = 3;

	hResetEvent = CreateEvent( NULL, FALSE, FALSE, RESET_SPEXEKEEPCPUBUSY_EVENT_NAME );
	
	hWaitEvents[0] = hStartEvent;
	hWaitEvents[1] = hExitEvent;
	hWaitEvents[2] = hResetEvent;

	
	MessageBox( NULL, TEXT("start CPU Busy !!!"), TEXT("MainRoutine"), MB_OK|MB_ICONINFORMATION );

	while( !bExitRoutine )
	{
		dwReturns = WaitForMultipleObjects( WAITEVENT_NUM, hWaitEvents, FALSE, dwWaitMS );
		
		switch( dwReturns )
		{
			case WAIT_OBJECT_0 + 0:
				/// got start event
				break;
			case WAIT_OBJECT_0 + 1:
				/// got exit event
				bExitRoutine = TRUE;
				break;
			case WAIT_OBJECT_0 + 2:
				/// got reset event
				break;
			case WAIT_TIMEOUT:
				ModifyCPULoading( spLibSysInfoIdle_Get() );
				break;
			default:
				break;
				
		}	///switch
		
	}	///while


	return dwRet;
}


static void ModifyCPULoading( DWORD dwValue )
{
	DWORD dwLoop = 0;
	///static DWORD dwTimes = 0;
	///RECT rcDest;
	///DWORD dwNum = 0;
	double dbNum = 0.0;

	///spMessageBoxOut( dINIT, TEXT("DrawCPULoading+++") );	
	SPDMSG( dINIT, (TEXT("ModifyCPULoading+++")) );	
	
	if( dwValue > 5 )
	{
		for( dwLoop = 0; dwLoop < 2000; dwLoop++ )
		{
			dbNum = 1234564.45646 * 126.15646 / 999;
		}
	}

	///spMessageBoxOut( dINIT, TEXT("DrawCPULoading---") );	
	SPDMSG( dINIT, (TEXT("ModifyCPULoading---")) );	
}



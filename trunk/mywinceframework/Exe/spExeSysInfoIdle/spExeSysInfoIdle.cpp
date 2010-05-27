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
#include "..\..\Inc\spLibSysInfoIdle.h"
#include "..\..\Inc\spLibProgAliveInfo.h"
#include "SPDebugDef.h"


///
// Debugging defines and functions for KITL output
#define MSG_INFO 0
#define MSG_TRACE 0
#define MSG_WARNING 0
#define MSG_ERROR 1


#define NAMED_SYSTEM_IDLE_CHECK_EXIT_EVENT		TEXT("named system idle check exit event")

typedef enum ExeSysInfoState
{
	tINIT,
	tCHECKEXIST,
	tNORMAL,
} ExeSysInfoState, *PExeSysInfoState;



///phototype
static DWORD MainRoutine( DWORD dwPararm );
static DWORD GetWaitMS( PExeSysInfoState pThisstat );
static DWORD GetCheckEventHandles( DWORD dwParam );
static DWORD GetResponseEventHandles( DWORD dwParam );
static DWORD GetExitEventHandles( DWORD dwParam );
static DWORD TimeoutEventHandles( DWORD dwParam );
static void Lock( void );
static void UnLock( void );
static void EnterStateNormal( void );
static void EnterStateCheckExist( void );
static void EnterStateExit( void );
static DWORD DoRoutine( DWORD dwParam );



///
static ExeSysInfoState progstat = tINIT;
PExeSysInfoState pprogstat = &progstat;
static CRITICAL_SECTION CriticalSection;
static BOOL bExitRoutine = FALSE;
static TCHAR szString_[64];         	// Temperary string


/// *****************************************
/// routine
/// *****************************************

inline void DebugOutput(int out_en, char* fmt, ...)
{
	if (out_en)
	{
		WCHAR out[1024];
		WCHAR wfmt[256];
		va_list body;
		va_start(body, fmt);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fmt, -1, wfmt, 256);
		vswprintf(out, wfmt, body);
		va_end(body);
		RETAILMSG(1, (out));
	}
}



int WINAPI WinMain (
              HINSTANCE hInstance,    // Handle to the current instance
              HINSTANCE hPrevInstance,// Handle to the previous instance
              LPTSTR lpCmdLine,       // Pointer to the command line
              int nCmdShow)           // Show state of the window
{

	MainRoutine( 0 );

	return 0;
}



static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;
	DWORD dwReturns = 0;

	#define WAITEVENT_NUM	3
	HANDLE hWaitEvents[WAITEVENT_NUM];
	DWORD dwWaitMS = INFINITE;

	
	progstat = tINIT;
	InitializeCriticalSection( &CriticalSection );
	spLibSysInfoIdle_Init( 0 );
	spLibProgAliveInfo_Init( 0 );

	hWaitEvents[0] = spLibProgAliveInfo_GetCheckEvent();
	hWaitEvents[1] = spLibProgAliveInfo_GetResponseEvent();
	hWaitEvents[2] = CreateEvent( NULL, FALSE, FALSE, NAMED_SYSTEM_IDLE_CHECK_EXIT_EVENT );

	EnterStateCheckExist();
	
	
	
	
	DebugOutput(MSG_ERROR, "Failed to open COM2 port, exiting\n");
	MessageBox( NULL, TEXT("????"), TEXT("MainRoutine"), MB_OK );
	
	///

	
	while( !bExitRoutine )
	{
		dwReturns = WaitForMultipleObjects( WAITEVENT_NUM, hWaitEvents, FALSE, GetWaitMS( pprogstat ) );
		
		switch( dwReturns )
		{
			case WAIT_OBJECT_0 + 0:
				GetCheckEventHandles( 0 );
				break;
			case WAIT_OBJECT_0 + 1:
				GetResponseEventHandles( 0 );
				break;
			case WAIT_OBJECT_0 + 2:
				GetExitEventHandles( 0 );
				break;
			case WAIT_TIMEOUT:
				TimeoutEventHandles( 0 );
				break;
			default:
				break;
				
		}	///switch
		
	}	///while
	


	DeleteCriticalSection( &CriticalSection );	
	spLibProgAliveInfo_Deinit( 0 );
	spLibSysInfoIdle_Deinit( 0 );
	CloseHandle( hWaitEvents[2] );
	progstat = tINIT;
	bExitRoutine = FALSE;

	
	return dwRet;
}


static DWORD GetWaitMS( PExeSysInfoState pThisstat )
{
	DWORD dwRet = 1000;
	
	if( *pThisstat == tINIT )
	{
		dwRet = 600;
	}
	else
	if( *pThisstat == tCHECKEXIST )
	{
		dwRet = 600;
	}
	else
	if( *pThisstat == tNORMAL )
	{
		///dwRet = INFINITE;
		dwRet = 1000;
	}
	else
	{
		dwRet = 600;
	}
	
	return dwRet;
}


static DWORD GetCheckEventHandles( DWORD dwParam )
{
	DWORD dwRet = 0;
	static DWORD dwCount = 0;
	
	if( *pprogstat == tINIT )
	{
		SPDMSG( dWARN, (L"$$$GetCheckEventHandles: %d \r\n", *pprogstat) );
	}
	else
	if( *pprogstat == tCHECKEXIST )
	{
		SPDMSG( dWARN, (L"$$$GetCheckEventHandles: %d \r\n", *pprogstat) );

		///SetEvent( spLibProgAliveInfo_GetResponseEvent() );
	}
	else
	if( *pprogstat == tNORMAL )
	{
		SPDMSG( dWARN, (L"$$$GetCheckEventHandles: %d \r\n", *pprogstat) );
		
		SetEvent( spLibProgAliveInfo_GetResponseEvent() );
	}
	else
	{
		SPDMSG( dWARN, (L"$$$GetCheckEventHandles: %d \r\n", *pprogstat) );
	}
	
	
	return dwRet;
}


static DWORD GetResponseEventHandles( DWORD dwParam )
{
	DWORD dwRet = 0;
	static DWORD dwCount = 0;
	
	if( *pprogstat == tINIT )
	{
		SPDMSG( dWARN, (L"$$$GetResponseEventHandles: %d \r\n", *pprogstat) );
	}
	else
	if( *pprogstat == tCHECKEXIST )
	{
		SPDMSG( dWARN, (L"$$$GetResponseEventHandles: %d \r\n", *pprogstat) );
		
		EnterStateExit();
	}
	else
	if( *pprogstat == tNORMAL )
	{
		SPDMSG( dWARN, (L"$$$GetResponseEventHandles: %d \r\n", *pprogstat) );
		
		EnterStateExit();
	}
	else
	{
		SPDMSG( dWARN, (L"$$$GetResponseEventHandles: %d \r\n", *pprogstat) );
	}
	
	
	return dwRet;
}


static DWORD GetExitEventHandles( DWORD dwParam )
{
	DWORD dwRet = 0;
	static DWORD dwCount = 0;
	
	if( *pprogstat == tINIT )
	{
		SPDMSG( dWARN, (L"$$$GetExitEventHandles: %d \r\n", *pprogstat) );
	}
	else
	if( *pprogstat == tCHECKEXIST )
	{
		SPDMSG( dWARN, (L"$$$GetExitEventHandles: %d \r\n", *pprogstat) );
		
		EnterStateExit();
	}
	else
	if( *pprogstat == tNORMAL )
	{
		SPDMSG( dWARN, (L"$$$GetExitEventHandles: %d \r\n", *pprogstat) );
		
		EnterStateExit();
	}
	else
	{
		SPDMSG( dWARN, (L"$$$GetExitEventHandles: %d \r\n", *pprogstat) );
	}
	
	
	return dwRet;
}


static DWORD TimeoutEventHandles( DWORD dwParam )
{
	DWORD dwRet = 0;
	static DWORD dwCount = 0;
	
	if( *pprogstat == tINIT )
	{
		SPDMSG( dWARN, (L"$$$TimeoutEventHandles: %d \r\n", *pprogstat) );
	}
	else
	if( *pprogstat == tCHECKEXIST )
	{
		SPDMSG( dWARN, (L"$$$TimeoutEventHandles: %d \r\n", *pprogstat) );
		dwCount++;
		
		if( dwCount > 5 )
		{
			dwCount = 0;	///RESET
			EnterStateNormal();
		}
		else
		{
			SetEvent( spLibProgAliveInfo_GetCheckEvent() );
		}
	}
	else
	if( *pprogstat == tNORMAL )
	{
		SPDMSG( dWARN, (L"$$$TimeoutEventHandles: %d \r\n", *pprogstat) );
		
		/// Todo:
		DoRoutine( 0 );
	}
	else
	{
		SPDMSG( dWARN, (L"$$$TimeoutEventHandles: %d \r\n", *pprogstat) );
	}
	
	
	return dwRet;
}


static void Lock( void )
{
	EnterCriticalSection( &CriticalSection );
}

static void UnLock( void )
{
	LeaveCriticalSection( &CriticalSection );
}


static void EnterStateNormal( void )
{
	Lock();
	progstat = tNORMAL;
	UnLock();
}


static void EnterStateCheckExist( void )
{
	Lock();
	progstat = tCHECKEXIST;
	UnLock();
}


static void EnterStateExit( void )
{
	bExitRoutine = TRUE;
	SPDMSG( dWARN, (L"$$$EnterStateExit: %d \r\n", 0) );
	
	MessageBox( NULL, TEXT("Exit Program!!"), TEXT("MainRoutine"), MB_OK );
}


static DWORD DoRoutine( DWORD dwParam )
{
	DWORD dwGet = 0;
	DWORD dwLoop = 10;
///

#if 0	
	for( ; dwLoop > 0; dwLoop-- )
	{
		Sleep( 1000 );
		dwGet = spLibSysInfoIdle_Get();
		wsprintf(szString_, TEXT("%02d%%"), dwGet);
		MessageBox( NULL, szString_, TEXT("MainRoutine"), MB_OK );
	}
#else
		dwGet = spLibSysInfoIdle_Get();
		wsprintf(szString_, TEXT("%02d%%"), dwGet);
		MessageBox( NULL, szString_, TEXT("MainRoutine"), MB_OK );

#endif	
	
	return dwGet;
}
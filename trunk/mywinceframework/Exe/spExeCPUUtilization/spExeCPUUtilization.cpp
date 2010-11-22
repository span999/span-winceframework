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
#include "..\..\Inc\spLibDDrawClass.h"
#include "..\..\Inc\spLibSysInfoIdle.h"
#ifndef SHOW_CPU_ONLY
#include "..\..\Inc\spLibSysInfoMemory.h"
#endif

#include "SPDebugDef.h"


#define 	START_SPEXECPUUTILIZATION_EVENT_NAME			TEXT("named start spExeCPUUtilization event")
#define 	EXIT_SPEXECPUUTILIZATION_EVENT_NAME				TEXT("named exit spExeCPUUtilization event")
#define 	RESET_SPEXECPUUTILIZATION_EVENT_NAME			TEXT("named reset spExeCPUUtilization event")


#define CPULOADDRAW_X_START		50
#define CPULOADDRAW_Y_START		0

#define CPULOADDRAW_WIDTH		100
#define CPULOADDRAW_HIGHT		4

///#define CPULOADDRAW_X_SCALE		5
#define CPULOADDRAW_X_SCALE		4
#define CPULOADDRAW_Y_SCALE		3

#define CPULOADDRAW_X_END		(CPULOADDRAW_X_START+(CPULOADDRAW_WIDTH*CPULOADDRAW_X_SCALE))
#define CPULOADDRAW_Y_END		(CPULOADDRAW_Y_START+(CPULOADDRAW_HIGHT*CPULOADDRAW_Y_SCALE))


#define CPULOADDRAW_R_BACKGROUND	0x0
#define CPULOADDRAW_G_BACKGROUND	0xFF
#define CPULOADDRAW_B_BACKGROUND	0x0

#define CPULOADDRAW_R_FOREGROUND	0xFF
#define CPULOADDRAW_G_FOREGROUND	0x0
#define CPULOADDRAW_B_FOREGROUND	0x0

#define CPULOADDRAW_R_SCALEDOT		0x0
#define CPULOADDRAW_G_SCALEDOT		0x0
#define CPULOADDRAW_B_SCALEDOT		0x0





typedef struct _CPUUtilizationDrawArea
{
    DWORD	dwXbase;
    DWORD	dwYbase;
    DWORD	dwStartX;
    DWORD	dwStartY;
    DWORD	dwXscale;
    DWORD	dwYscale;
    DWORD	dwEndX;
    DWORD	dwEndY;
} CPUUtilizationDrawArea, *PCPUUtilizationDrawArea;


static DWORD MainRoutine( DWORD dwPararm );
static void ReleaseAlls( void );
#ifdef SHOW_CPU_ONLY
static void DrawCPULoading( DWORD dwValue );
#else
static void DrawCPUMemLoading( DWORD dwValue );
#endif	///#ifdef SHOW_CPU_ONLY
static HRESULT InitDDraw( HWND hWnd );
static void InitDrawArea( void );


static CPUUtilizationDrawArea	DrawArea;
static PCPUUtilizationDrawArea	pDrawArea = &DrawArea;
static HANDLE	hStartEvent = NULL;
static HANDLE	hExitEvent = NULL;
static HANDLE	hResetEvent = NULL;
static BOOL 	bExitRoutine = FALSE;
static spLibDDraw	*pmyDD = NULL;


/// *****************************************
/// routine
/// *****************************************

static void InitDrawArea( void )
{
	pDrawArea = &DrawArea;

	memset( pDrawArea, 0, sizeof(DrawArea) );
	
	pDrawArea->dwXbase = CPULOADDRAW_WIDTH;
	pDrawArea->dwYbase = CPULOADDRAW_HIGHT;
	pDrawArea->dwStartX = CPULOADDRAW_X_START;
	pDrawArea->dwStartY = CPULOADDRAW_Y_START;
	pDrawArea->dwXscale = CPULOADDRAW_X_SCALE;
	pDrawArea->dwYscale = CPULOADDRAW_Y_SCALE;
	pDrawArea->dwEndX = CPULOADDRAW_X_END;
	pDrawArea->dwEndY = CPULOADDRAW_Y_END;

	if( 500 < GetSystemMetrics( SM_CXSCREEN ) )
		pDrawArea->dwXscale = 5;
				
	pDrawArea->dwEndX = (pDrawArea->dwStartX+(pDrawArea->dwXbase*pDrawArea->dwXscale));
	pDrawArea->dwEndY = (pDrawArea->dwStartY+(pDrawArea->dwYbase*pDrawArea->dwYscale));;

}




int WINAPI WinMain (
              HINSTANCE hInstance,    // Handle to the current instance
              HINSTANCE hPrevInstance,// Handle to the previous instance
              LPTSTR lpCmdLine,       // Pointer to the command line
              int nCmdShow)           // Show state of the window
{
	DWORD dwErrCode = 0;

	hStartEvent = CreateEvent( NULL, FALSE, FALSE, START_SPEXECPUUTILIZATION_EVENT_NAME );
	dwErrCode = GetLastError();
	
	if( NULL != hStartEvent )
	{
		hExitEvent = CreateEvent( NULL, FALSE, FALSE, EXIT_SPEXECPUUTILIZATION_EVENT_NAME );
		
		if( ERROR_ALREADY_EXISTS == dwErrCode )
		{
			int RetMsgBox = 0;
			
			RetMsgBox = MessageBox( NULL, TEXT("2nd Launch will exit CPU Utilization !!!"), TEXT("MainRoutine"), MB_ICONQUESTION|MB_OKCANCEL );
			
			if( IDOK == RetMsgBox )
			{
				///cpu utilization program already running, get it down!!
				SetEvent( hExitEvent );
				SPDMSG( dINIT, (L"SPD@spExeCPUUtilization: set exit event-%d \r\n", 0 ) );
				///wait 3 seconds
				Sleep( 3000 );
			
				ReleaseAlls();
				SPDMSG( dINIT, (L"SPD@spExeCPUUtilization: set exit event done-%d \r\n", 1 ) );
			}	
		}
		else
		{
			///init
			InitDrawArea();

			///init
			spLibSysInfoIdle_Init( 0 );
	#ifndef SHOW_CPU_ONLY
			spLibSysInfoMemory_Init( 0 );
	#endif
			
			///init 
			InitDDraw( NULL );
			
			///start cpu utilization program
			MainRoutine( 0 );
			
			ReleaseAlls();
			SPDMSG( dINIT, (L"SPD@spExeCPUUtilization: exit done-%d \r\n", 0 ) );
		}	
	}
	else
	{
		SPDMSG( dFAIL, (L"SPD@spExeCPUUtilization: CreateEvent fail-%d \r\n", GetLastError() ) );
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
	
	if( NULL != pmyDD )
	{
		delete pmyDD;
		pmyDD = NULL;
	}
	
}



static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;
	DWORD dwReturns = 0;
	
	
	#define WAITEVENT_NUM	3
	HANDLE hWaitEvents[WAITEVENT_NUM];
///	DWORD dwWaitMS = INFINITE;
	DWORD dwWaitMS = 1000;

	hResetEvent = CreateEvent( NULL, FALSE, FALSE, RESET_SPEXECPUUTILIZATION_EVENT_NAME );
	
	hWaitEvents[0] = hStartEvent;
	hWaitEvents[1] = hExitEvent;
	hWaitEvents[2] = hResetEvent;

	
	MessageBox( NULL, TEXT("start CPU Utilization !!!"), TEXT("MainRoutine"), MB_OK|MB_ICONINFORMATION );

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
	#ifdef SHOW_CPU_ONLY
				DrawCPULoading( 100 - spLibSysInfoIdle_Get() );
	#else
				DrawCPUMemLoading( 100 - spLibSysInfoIdle_Get() );
	#endif
				break;
			default:
				break;
				
		}	///switch
		
	}	///while


	return dwRet;
}

#ifdef SHOW_CPU_ONLY

static void DrawCPULoading( DWORD dwValue )
{
	DWORD dwLoop = 0;
	static DWORD dwTimes = 0;
	RECT rcDest;
	DWORD dwNum = 0;

	///spMessageBoxOut( dINIT, TEXT("DrawCPULoading+++") );	
	SPDMSG( dINIT, (TEXT("DrawCPULoading+++")) );	
	
	if( NULL != pmyDD && NULL != pDrawArea )
	{
	
		///draw backgound
		SetRect( &rcDest, pDrawArea->dwStartX, pDrawArea->dwStartY, pDrawArea->dwEndX, pDrawArea->dwEndY );
		pmyDD->spLibBltDDraw( &rcDest, CPULOADDRAW_R_BACKGROUND, CPULOADDRAW_G_BACKGROUND, CPULOADDRAW_B_BACKGROUND );		///green as Backgound

		///draw percentage
		if( dwValue > 0 )	///only draw when we have!!
		{
			SetRect( &rcDest, pDrawArea->dwStartX, pDrawArea->dwStartY, (pDrawArea->dwStartX+(dwValue*pDrawArea->dwXscale)), pDrawArea->dwEndY );
			pmyDD->spLibBltDDraw( &rcDest, CPULOADDRAW_R_FOREGROUND, CPULOADDRAW_G_FOREGROUND, CPULOADDRAW_B_FOREGROUND );		///red as Backgound
		}

		for( dwLoop = 1; dwLoop < 20; dwLoop++ )
		{
			if( 0 == dwLoop % 2 ) 
				SetRect( &rcDest, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale), pDrawArea->dwStartY, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale)+2, pDrawArea->dwStartY+(1*pDrawArea->dwYscale) );
			else
				SetRect( &rcDest, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale), pDrawArea->dwStartY, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale)+1, pDrawArea->dwStartY+(1*pDrawArea->dwYscale) );
	
			pmyDD->spLibBltDDraw( &rcDest, CPULOADDRAW_R_SCALEDOT, CPULOADDRAW_G_SCALEDOT, CPULOADDRAW_B_SCALEDOT );		///black as dot
		}
		
		pmyDD->spLibTextDraw( pDrawArea->dwStartX, pDrawArea->dwEndY+pDrawArea->dwYbase, TEXT("CPU utilization => %4d%%, Avg=> %4d%%!!!"), dwValue, spLibSysInfoIdle_GetAvg() );		
	}

	///spMessageBoxOut( dINIT, TEXT("DrawCPULoading---") );	
	SPDMSG( dINIT, (TEXT("DrawCPULoading---")) );	
}

#else	///#ifdef SHOW_CPU_ONLY

static void DrawCPUMemLoading( DWORD dwValue )
{
	DWORD dwLoop = 0;
	static DWORD dwTimes = 0;
	RECT rcDest;
	DWORD dwNum = 0;

	///spMessageBoxOut( dINIT, TEXT("DrawCPULoading+++") );	
	SPDMSG( dINIT, (TEXT("DrawCPUMemLoading+++")) );	
	
	if( NULL != pmyDD && NULL != pDrawArea )
	{
		LibSysInfoMemoryContent mmi;
		DWORD dwRet;
		
		memset( &mmi, 0, sizeof(LibSysInfoMemoryContent) );
		dwRet = spLibSysInfoMemory_Get( &mmi );
		
		///draw backgound
		SetRect( &rcDest, pDrawArea->dwStartX, pDrawArea->dwStartY, pDrawArea->dwEndX, pDrawArea->dwEndY );
		pmyDD->spLibBltDDraw( &rcDest, CPULOADDRAW_R_BACKGROUND, CPULOADDRAW_G_BACKGROUND, CPULOADDRAW_B_BACKGROUND );		///green as Backgound

		///draw percentage
		if( dwValue > 0 )	///only draw when we have!!
		{
			SetRect( &rcDest, pDrawArea->dwStartX, pDrawArea->dwStartY, (pDrawArea->dwStartX+(dwValue*pDrawArea->dwXscale)), pDrawArea->dwEndY );
			pmyDD->spLibBltDDraw( &rcDest, CPULOADDRAW_R_FOREGROUND, CPULOADDRAW_G_FOREGROUND, CPULOADDRAW_B_FOREGROUND );		///red as Backgound
		}

		for( dwLoop = 1; dwLoop < 20; dwLoop++ )
		{
			if( 0 == dwLoop % 2 ) 
				SetRect( &rcDest, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale), pDrawArea->dwStartY, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale)+2, pDrawArea->dwStartY+(1*pDrawArea->dwYscale) );
			else
				SetRect( &rcDest, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale), pDrawArea->dwStartY, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale)+1, pDrawArea->dwStartY+(1*pDrawArea->dwYscale) );
	
			pmyDD->spLibBltDDraw( &rcDest, CPULOADDRAW_R_SCALEDOT, CPULOADDRAW_G_SCALEDOT, CPULOADDRAW_B_SCALEDOT );		///black as dot
		}
		
	#if 0	///two lines, put memory info in second line
		pmyDD->spLibTextDraw( pDrawArea->dwStartX, pDrawArea->dwEndY+pDrawArea->dwYbase, TEXT("CPU utilization => %4d%%, Avg=> %4d%%!!!"), dwValue, spLibSysInfoIdle_GetAvg() );
		if( (-1) == dwRet )
			pmyDD->spLibTextDraw( pDrawArea->dwStartX, pDrawArea->dwEndY+(pDrawArea->dwYbase*5), TEXT("Memory get fail !!!") );		
		else	
			pmyDD->spLibTextDraw( pDrawArea->dwStartX, pDrawArea->dwEndY+(pDrawArea->dwYbase*5), TEXT("Memory %d%%=> P[%d/%d]k, V[%d/%d]k, F[%d/%d]k"), mmi.dwMemoryLoad, mmi.dwAvailPhys/1024, mmi.dwTotalPhys/1024, mmi.dwAvailVirtual/1024, mmi.dwTotalVirtual/1024, mmi.dwAvailPageFile/1024, mmi.dwTotalPageFile/1024 );
	#else	///one line for both CPU & memory
		pmyDD->spLibTextDraw( pDrawArea->dwStartX, pDrawArea->dwEndY+pDrawArea->dwYbase, TEXT("CPU =>%4d%%, Avg=>%4d%%!! Phy Memory =>%4d%% [%d/%d/%d]k!!"), dwValue, spLibSysInfoIdle_GetAvg(), mmi.dwMemoryLoad, mmi.dwAvailPhys/1024, mmi.dwTotalPhys/1024, mmi.iDiffPhys/1024 );
	#endif
	}

	///spMessageBoxOut( dINIT, TEXT("DrawCPULoading---") );	
	SPDMSG( dINIT, (TEXT("DrawCPUMemLoading---")) );	
}

#endif	///#ifdef SHOW_CPU_ONLY

static HRESULT InitDDraw( HWND hWnd )
{
    HRESULT	hRet = 0;
	
	///spMessageBoxOut( dINIT, TEXT("Create spLibDDRaw object !") );
	SPDMSG( dINIT, (TEXT("Create spLibDDRaw object !")) );
	
	pmyDD = new spLibDDraw( hWnd );
	///pmyDD = new spLibDDraw();
	
	if( pmyDD == NULL )
	{
		///spMessageBoxOut( dFAIL,  TEXT("Create spLibDDRaw object fail !") );	
		SPDMSG( dFAIL,  (TEXT("Create spLibDDRaw object fail !")) );	
	}
	else
	{
		DWORD dwRet = 0;
		
		///spMessageBoxOut( dINFO, TEXT("Start Init DDraw ") );	
		///spMessageBoxOut( dINFO, TEXT("Start Init DDraw ") );	
		SPDMSG( dINFO, (TEXT("Start Init DDraw ")) );	
		///init primary with 1 back
		///pmyDD->spLibInitDDrawBack( 0 );
		///pmyDD->spLibInitDDrawBack( 0,  EnumFunction );
		dwRet = pmyDD->spLibInitDDraw();
		
		
		if( dwRet == 0 )
		{

		}
		else
		{
			static TCHAR szString_[64];         	// Temperary string
			
			wsprintf(szString_, TEXT("Init DDraw error %02d%%"), dwRet);
			MessageBox( NULL, szString_, TEXT("MainRoutine"), MB_OK );
		}
	}		
	
	///spMessageBoxOut( dINIT, TEXT("spLibDDRaw done !!") );
	SPDMSG( dINIT, (TEXT("spLibDDRaw done !!")) );
	
	return hRet;
}
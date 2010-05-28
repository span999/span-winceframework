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
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only
#define		SPPREFIX			TEXT("ARExe:")
#define		FIRSTMODULENAME		TEXT("spDllPreRunner.dll")
#define		SECONDMODULENAME	TEXT("spDllAutoRunner.dll")


typedef DWORD (*PFN_FirstModule_Start)( DWORD dwParam );
typedef DWORD (*PFN_SecondModule_Start)( DWORD dwParam );


static DWORD MainRoutine( DWORD dwPararm );
static BOOL spExeLoadMyModule1( void );
static BOOL spExeLoadMyModule2( void );
static BOOL WaitSystemReady( DWORD dwFlag, DWORD dwTimeout );
static void spExeReleaseAll( void );


static PFN_FirstModule_Start gpfFirstModuleStart = NULL;
static PFN_SecondModule_Start gpfSecondModuleStart = NULL;
static HMODULE hCoreFirst = 0;
static HMODULE hCoreSecond = 0;	



/// *****************************************
/// routine
/// *****************************************

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

	DWORD dwCount = 0;	
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s go Auto Runnner3 !!!"), SPPREFIX );
	
	spExeLoadMyModule1();
	
	if( gpfFirstModuleStart )
	{
		gpfFirstModuleStart( 0 );
	}
	else
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s gpfFirstModuleStart fail !!!"), SPPREFIX );
	}

	WaitSystemReady( SH_WMGR, 60000 );
	
	spExeLoadMyModule2();
	
	if( gpfSecondModuleStart )
	{
		gpfSecondModuleStart( 0 );
	}
	else
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s gpfSecondModuleStart fail !!!"), SPPREFIX );
	}
	
	spExeReleaseAll();
	return dwRet;
}


static BOOL WaitSystemReady( DWORD dwFlag, DWORD dwTimeout )
{
	BOOL bRet = FALSE;
	DWORD dwCount = 0;
	
	if( (0 == dwTimeout) || (dwTimeout > (5*60*1000)) )	///default 5000ms
		dwTimeout = 5000;
	
	/// Flag : SH_SHELL, SH_GDI, SH_WMGR
	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s WaitSystemReady wait %d !!!"), SPPREFIX, dwTimeout );
	
	while( 1 )
	{
		Sleep( 100 );
		bRet = IsAPIReady( dwFlag );
		dwCount = dwCount + 100;
		
		if( bRet || (dwCount >= dwTimeout) )
			break;
	}	

	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s WaitSystemReady exit %d !!!"), SPPREFIX, dwTimeout );
	return bRet;
}


static BOOL spExeLoadMyModule1( void )
{
    ///HMODULE hCore = 0;	
    BOOL bRet = FALSE;
	
    /// init system function call
    hCoreFirst = (HMODULE)LoadLibrary( FIRSTMODULENAME );
    if ( hCoreFirst ) {
        gpfFirstModuleStart = (PFN_FirstModule_Start)GetProcAddress( hCoreFirst, L"spDllHook_Start" );
        if ( !gpfFirstModuleStart )
        {
            FreeLibrary(hCoreFirst);
            bRet = FALSE;
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s GetProcAddress1 fail !!!"), SPPREFIX );
		}
        else
            bRet = TRUE;
    }
	else
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s LoadLibrary1 fail !!!"), SPPREFIX );
	}
	
    
    return bRet;	
}


static BOOL spExeLoadMyModule2( void )
{
    ///HMODULE hCore = 0;	
    BOOL bRet = FALSE;
	
    /// init system function call
    hCoreSecond = (HMODULE)LoadLibrary( SECONDMODULENAME );
    if ( hCoreSecond ) {
        gpfSecondModuleStart = (PFN_SecondModule_Start)GetProcAddress( hCoreSecond, L"spDllHook_Start" );
        if ( !gpfSecondModuleStart )
        {
            FreeLibrary(hCoreSecond);
            bRet = FALSE;
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s GetProcAddress1 fail2 !!!"), SPPREFIX );
		}
        else
            bRet = TRUE;
    }
	else
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s LoadLibrary1 fail2 !!!"), SPPREFIX );
	}
	
    
    return bRet;	
}


static void spExeReleaseAll( void )
{
	if( NULL != hCoreFirst )
		FreeLibrary(hCoreFirst);

	if( NULL != hCoreSecond )
		FreeLibrary(hCoreSecond);
}
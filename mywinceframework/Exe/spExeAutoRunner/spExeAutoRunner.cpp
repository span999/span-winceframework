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
#include <pnp.h>
#include <storemgr.h>


#include "spLibErrCodeDef.h"
#include "SPDebugDef.h"
#include "spLibDbgMsg.h"

#define		SPPREFIX			TEXT("ARExe:")
#define		FIRSTMODULENAME		TEXT("spDllAutoRunner.dll")

typedef DWORD (*PFN_FirstModule_Start)( DWORD dwParam );


static DWORD MainRoutine( DWORD dwPararm );
static BOOL spExeLoadMyModule( void );

static PFN_FirstModule_Start gpfFirstModuleStart = NULL;

///static TCHAR szString[128];         // Temperary string


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
	
	spLibDbgMsg_Dlg( TEXT("%s go Auto Runnner3 !!!"), SPPREFIX );
	
	spExeLoadMyModule();
	
	if( gpfFirstModuleStart )
	{
		gpfFirstModuleStart( 0 );
	}
	else
	{
		spLibDbgMsg_Dlg( TEXT("%s gpfFirstModuleStart fail !!!"), SPPREFIX );
	}
	
	return dwRet;
}

static BOOL spExeLoadMyModule( void )
{
    HMODULE hCore = 0;	
    BOOL bRet = FALSE;
	
    /// init system function call
    hCore = (HMODULE)LoadLibrary( FIRSTMODULENAME );
    if ( hCore ) {
        gpfFirstModuleStart = (PFN_FirstModule_Start)GetProcAddress( hCore, L"spDllAutoRunner_Start" );
        if ( !gpfFirstModuleStart )
        {
            FreeLibrary(hCore);
            bRet = FALSE;
			spLibDbgMsg_Dlg( TEXT("%s GetProcAddress fail !!!"), SPPREFIX );
		}
        else
            bRet = TRUE;
    }
	else
	{
		spLibDbgMsg_Dlg( TEXT("%s LoadLibrary fail !!!"), SPPREFIX );
	}
	
    
    return bRet;	
}


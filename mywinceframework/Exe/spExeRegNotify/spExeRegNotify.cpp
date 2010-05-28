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
#include "spLibDDrawClass.h"
#include "spLibRegNotify.h"



#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only
#define		SPPREFIX			TEXT("RNExe:")



static DWORD MainRoutine( DWORD dwPararm );
static void spExeReleaseAll( void );



static spLibDDraw	*pmyDD = NULL;
static DWORD dwCount = 0;

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


BOOL RegNotifyExeCallback( DWORD dwByteSize )
{
	BOOL bRet = TRUE;
	DWORD dwLoop = 0;
	
	dwCount++;
	pmyDD->spLibTextDraw( 100, 200, TEXT(" %4d, %d !!"), dwByteSize, dwCount );
	
///	for( dwLoop = 0; dwLoop < dwByteSize; dwLoop++ )
///		pmyDD->spLibTextDraw( 100+(dwLoop*20), 220, TEXT("%02x"), *((BYTE *)pData+dwLoop) );
	
	
	return bRet;
}

static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;

	DWORD dwCount = 0;	
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s go RegNotify.exe!!!"), SPPREFIX );
	
	///init ddraw
	pmyDD = new spLibDDraw();
	pmyDD->spLibInitDDraw();
	
	spLibRegNotify_Init( 0 );
	spLibRegNotify_SetCallback( RegNotifyExeCallback );
	
	Sleep( 120000 );
	
	spLibRegNotify_Deinit( 0 );
	spExeReleaseAll();
	
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s exit RegNotify.exe!!!"), SPPREFIX );
	return dwRet;
}



static void spExeReleaseAll( void )
{
	if( NULL != pmyDD )
	{
		delete pmyDD;
		pmyDD = NULL;
	}
}
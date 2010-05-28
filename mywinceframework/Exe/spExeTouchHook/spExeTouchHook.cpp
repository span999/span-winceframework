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
#include "spLibTouchHook.h"
#include "spLibNumPadParser.h"



#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only
#define		SPPREFIX			TEXT("THExe:")



static DWORD MainRoutine( DWORD dwPararm );
static void spExeReleaseAll( void );



static spLibDDraw	*pmyDD = NULL;


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


BOOL TouchHookExeCallback( TOUCH_EVENT_DATA ThisTouchData )
{
	BOOL bRet = TRUE;
	
	pmyDD->spLibTextDraw( 100, 100, TEXT("Type=>%2d, X =>%4d, Y=>%4d!!"), ThisTouchData.teType, ThisTouchData.tiX, ThisTouchData.tiY );
	
	if( TOUCH_UP_EVENT == ThisTouchData.teType )
		spLibParseSequenceCondition( spLibParseAreaCondition( ThisTouchData.tiX, ThisTouchData.tiY ) );
	
	return bRet;
}

static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;

	DWORD dwCount = 0;	
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s go TouchHook.exe!!!"), SPPREFIX );
	
	///init ddraw
	pmyDD = new spLibDDraw();
	pmyDD->spLibInitDDraw();
	
	///init touch hook
	spLibTouchHook_Init( 0 );
	///init number pad parser
	spLibParseAreaInit();

	/// hook callback
	spLibTouchHook_SetCallback( TouchHookExeCallback );
	
	Sleep( 60000 );
	
	spLibParseAreaDeInit();
	spLibTouchHook_Deinit( 0 );
	
	spExeReleaseAll();
	
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s exit TouchHook.exe!!!"), SPPREFIX );
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
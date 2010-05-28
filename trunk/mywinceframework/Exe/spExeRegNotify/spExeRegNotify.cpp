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
#include "spLibDbgMsgBuf.h"


#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
#define		LIBMSGFLAG_NK		(dNOUT|0x0FFFFFFF)		///nk msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only
#define		SPPREFIX			TEXT("RNExe:")

#define		UI_USER_END_CANCER		0x00001234
#define		UI_USER_END_OK			0x00001278


static DWORD MainRoutine( DWORD dwPararm );
static void spExeReleaseAll( void );
static void spExeInitAll( void );
static DWORD CheckUserEndUI( DWORD dwInterval, DWORD dwTryTimes, DWORD dwUITimeout );
static DWORD WINAPI UserEndUIThread( LPVOID pContext );



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


BOOL RegNotifyExeCallback( DWORD dwByteSize )
{
	BOOL bRet = TRUE;
	DWORD dwLoop = 0;
	SPDBG_MSG_INFO HalDbgMsgBuf;
	DWORD dwRet = 0;
	static DWORD dwLastShowNum = 0;
	static DWORD dwLastShowIdx = 0;
	static DWORD dwCallbackCount = 0;
	
	dwCallbackCount++;
	
	///memset( &HalDbgMsgBuf, 0, sizeof(SPDBG_MSG_INFO) );		///clear buffer
	dwRet = spLibSysHALIoctl_GetDbgMsgBuf( &HalDbgMsgBuf );		///get msg with buffer
	
	if( 0 < dwRet )
	{
		SPDBG_MSG *pMsg = NULL;
		
		for( dwLoop = 0; dwLoop < 6; dwLoop++ )
		{
			pMsg = &HalDbgMsgBuf.dbgMsg1;
			pMsg = pMsg + dwLoop;

			if( pMsg->dwSeqNum > dwLastShowNum )	///check if new msg
			{	
				pmyDD->spLibTextDraw( 20, 40+(dwLastShowIdx*14), TEXT(" %ld, %ld, %ld, %s!< "), dwByteSize, dwCallbackCount, pMsg->dwSeqNum, pMsg->DbgMsg );
				dwLastShowNum = pMsg->dwSeqNum;
				dwLastShowIdx++;
				if( dwLastShowIdx > 12 )
					dwLastShowIdx = 0;
			}
			
			if( (-1) == dwLastShowNum )
				dwLastShowNum = 0;
			
		}	///for

	}
	else
		pmyDD->spLibTextDraw( 100, 200, TEXT(" %4d, %d !!"), dwByteSize, dwCallbackCount );

	
	return bRet;
}

static DWORD WINAPI UserEndUIThread( LPVOID pContext )
{
	DWORD dwRet = UI_USER_END_CANCER;
	
	if( IDOK == MessageBox( NULL, TEXT("Do you want to exit this program??"), TEXT("OAL Dbg Msg Info:"), MB_OKCANCEL|MB_SETFOREGROUND ) )
		dwRet = UI_USER_END_OK;
	
	ExitThread( dwRet );
	return dwRet;
}



static DWORD CheckUserEndUI( DWORD dwInterval, DWORD dwTryTimes, DWORD dwUITimeout )
{
	DWORD 	dwRet = 0;
	DWORD 	dwLoop = 0;
	DWORD 	dwThreadPararm = dwUITimeout;
	HANDLE	hUIthread = NULL;
	DWORD 	dwThreadExitCode = 0;

	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s CheckUserEndUI+++"), SPPREFIX );
	
	do
	{
		Sleep( dwInterval );
		
		///create thread for UI
		hUIthread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)UserEndUIThread, (LPVOID)&dwThreadPararm, 0, NULL );
		Sleep( dwUITimeout );
		
		if( GetExitCodeThread( hUIthread, &dwThreadExitCode ) )
		{
			if( STILL_ACTIVE == dwThreadExitCode )
			{	///force terminate
				spLibDbgMsg( LIBMSGFLAG_NK, TEXT("%s force terminate CheckUserEndUI !!"), SPPREFIX );
				TerminateThread( hUIthread, UI_USER_END_CANCER );
			}
			else
			if( UI_USER_END_OK == dwThreadExitCode )
			{	///user check ok button
				spLibDbgMsg( LIBMSGFLAG_NK, TEXT("%s User check OK !!"), SPPREFIX );
				break;
			}
			
		}
		else
		{
			spLibDbgMsg( LIBMSGFLAG_NK, TEXT("%s Error! force terminate CheckUserEndUI !!"), SPPREFIX );
			TerminateThread( hUIthread, UI_USER_END_CANCER );
		}
		CloseHandle( hUIthread );
		
		dwLoop++;
	}
	while( dwLoop != dwTryTimes );
	
	return dwRet;
}


static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s go RegNotify.exe!!!"), SPPREFIX );
	
	spExeInitAll();
	
	spLibRegNotify_Init( 0 );
	spLibRegNotify_SetCallback( RegNotifyExeCallback );
	
	CheckUserEndUI( 60000, 0, 5000 );
	
	spLibRegNotify_Deinit( 0 );
	spExeReleaseAll();
	
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s exit RegNotify.exe!!!"), SPPREFIX );
	return dwRet;
}


static void spExeInitAll( void )
{
	///init ddraw
	pmyDD = new spLibDDraw();
	pmyDD->spLibInitDDraw();
}


static void spExeReleaseAll( void )
{
	if( NULL != pmyDD )
	{
		delete pmyDD;
		pmyDD = NULL;
	}
}
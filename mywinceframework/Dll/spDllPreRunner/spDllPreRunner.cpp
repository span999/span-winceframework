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
#include "spLibRegNotify.h"
#include "spLibDbgMsgBuf.h"



#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
#define		LIBMSGFLAG_NK		(dNOUT|0x0FFFFFFF)		///NK msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only

#define		SPPREFIX			TEXT("ARPre:")



extern void DrawCPUMem( BOOL bDraw );
extern void InitSysInfoDrawArea( spLibDDraw *pDD );
extern void DeInitSysInfoDrawArea( void );


static DWORD MainRoutine( DWORD dwPararm );
static DWORD WINAPI PreRunnerThread( LPVOID  pContext );

static BOOL IsAllThreadStop( void );

static BOOL bPreRunnerThreadStop = FALSE;
static spLibDDraw	*pmyDD = NULL;
static BOOL bOALMsgShow = FALSE;
static BOOL bCPUMsgShow = FALSE;


/// *****************************************
/// routine
/// *****************************************


BOOL WINAPI  DllEntry(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DEBUGREGISTER(hinstDll);
        SPDMSG(dINIT, (_T("SPD@%s() : Process Attach\r\n"), _T(__FUNCTION__)));
		///spLibDbgMsg_Dlg( TEXT("spDllPreRunner DLL_PROCESS_ATTACH!!!") );
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        SPDMSG(dINIT, (_T("SPD@%s() : Process Detach\r\n"), _T(__FUNCTION__)));
		///spLibDbgMsg_Dlg( TEXT("spDllPreRunner DLL_PROCESS_DETACH!!!") );
    }

    return TRUE;
}


DWORD spDllHook_Start( DWORD dwParam )
{
	DWORD dwRet;
	
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("spDllPreRunner_Start 1!!!") );
	dwRet = MainRoutine( dwParam );
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("spDllPreRunner_Start 2!!!") );
	
	return dwRet;
}



BOOL TouchHookExeCallback( TOUCH_EVENT_DATA ThisTouchData )
{
	BOOL bRet = TRUE;
	DWORD dwRet = (-1);
	
	///pmyDD->spLibTextDraw( 250, 240, TEXT("Type=>%2d, X =>%4d, Y=>%4d!!"), ThisTouchData.teType, ThisTouchData.tiX, ThisTouchData.tiY );
	
	if( TOUCH_UP_EVENT == ThisTouchData.teType )
	{
		///pmyDD->spLibTextDraw( 250, 240, TEXT("X =>%4d, Y=>%4d!!"), ThisTouchData.tiX, ThisTouchData.tiY );
		dwRet = spLibParseSequenceCondition( spLibParseAreaCondition( ThisTouchData.tiX, ThisTouchData.tiY ) );
		
		if( -1 != dwRet )
		{
			pmyDD->spLibTextDraw( 250, 240, TEXT("X =>%4d, Y=>%4d!!"), ThisTouchData.tiX, ThisTouchData.tiY );

			if( 99 == dwRet )
			{
			}
			else
			if( 1 == dwRet )
			{	/// on screen message show control
				bOALMsgShow = !bOALMsgShow;
				pmyDD->spLibTextDraw( 10, 10, TEXT("OALMsg %s"), bOALMsgShow?TEXT("On"):TEXT("Off") );
			}
			else
			if( 2 == dwRet )
			{	/// on screen CPU/Mem show control
				bCPUMsgShow = !bCPUMsgShow;
				pmyDD->spLibTextDraw( 10, 10, TEXT("CPUMsg %s"), bCPUMsgShow?TEXT("On"):TEXT("Off") );
			}
		}
	}
	
	return bRet;
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
	
	if( !bOALMsgShow )
	{
		///Sleep( 100 );
		return FALSE;
	}	
	
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


BOOL DDrawShowInit( void )
{
	BOOL bRet = TRUE;
	
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("%s go DDrawShowInit!!!"), SPPREFIX );
	
	///init ddraw
	pmyDD = new spLibDDraw();
	pmyDD->spLibInitDDraw();

	InitSysInfoDrawArea( pmyDD );
	
		
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s exit DDrawShowInit!!!"), SPPREFIX );
	
	return bRet;
}


BOOL DDrawShowDeInit( void )
{
	BOOL bRet = TRUE;
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s go DDrawShowDeInit!!!"), SPPREFIX );
	
	DeInitSysInfoDrawArea();
	
	if( NULL != pmyDD )
	{
		delete pmyDD;
		pmyDD = NULL;
	}
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s exit DDrawShowDeInit!!!"), SPPREFIX );
	
	return bRet;
}

BOOL TouchHookInit( void )
{
	BOOL bRet = TRUE;
	
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("%s go TouchHookExeInit!!!"), SPPREFIX );
	
	
	///init number pad parser
	spLibParseAreaInit();

	///init touch hook
	spLibTouchHook_Init( 0 );

	/// hook callback
	spLibTouchHook_SetCallback( TouchHookExeCallback );
		
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s exit TouchHookExeInit!!!"), SPPREFIX );
	
	return bRet;
}


BOOL TouchHookDeInit( void )
{
	BOOL bRet = TRUE;
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s go TouchHookExeDeInit!!!"), SPPREFIX );
	
	///de-init
	spLibTouchHook_Deinit( 0 );
	spLibParseAreaDeInit();
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s exit TouchHookExeDeInit!!!"), SPPREFIX );
	
	return bRet;
}

BOOL RegNotifyInit( void )
{
	BOOL bRet = TRUE;
	
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("%s go RegNotifyInit!!!"), SPPREFIX );
	
	///init reg notify
	spLibRegNotify_Init( 0 );
	bRet = spLibRegNotify_SetCallback( RegNotifyExeCallback );
	if( !bRet )
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibRegNotify_SetCallback fail!!!"), SPPREFIX );
		
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s exit RegNotifyInit!!!"), SPPREFIX );
	
	return bRet;
}


BOOL RegNotifyDeInit( void )
{
	BOOL bRet = TRUE;
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s go RegNotifyDeInit!!!"), SPPREFIX );
	
	//de-init
	spLibRegNotify_Deinit( 0 );
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s exit RegNotifyDeInit!!!"), SPPREFIX );
	
	return bRet;
}



static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;
	HANDLE hPreRunnerThread = NULL;
	DWORD dwPreRunnerThreadId = 0;
	DWORD dwThreadPararm = 0;



	DDrawShowInit();
	Sleep( 100 );
	TouchHookInit();
	Sleep( 100 );
	///RegNotifyInit();
	
	
	hPreRunnerThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)PreRunnerThread, (LPVOID)&dwThreadPararm, 0, (LPDWORD)&dwPreRunnerThreadId );

	if( NULL == hPreRunnerThread )
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("CreateThread PreRunnerThread fail %d !!!"), GetLastError() );
	}	
	else
		bPreRunnerThreadStop = FALSE;

	
	while( 1 )
	{
	
		Sleep( 1000 );
		DrawCPUMem( bCPUMsgShow );
		///spLibDbgMsg( LIBMSGFLAG_NK, TEXT("Pre Runnner running!!!") );
		
	}

	
	///RegNotifyDeInit();
	TouchHookDeInit();
	DDrawShowDeInit();
	
	return dwRet;
}


static DWORD WINAPI PreRunnerThread( LPVOID  pContext )
{
#define		WAITEVENT_NUM		2
	DWORD dwRet = 0;
	HANDLE hWaitEvents[WAITEVENT_NUM];
	///DWORD dwWaitMS = INFINITE;
	DWORD dwWaitMS = 2000;
	DWORD dwReturns = 0;
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("Start Pre Runnner !!!") );

	hWaitEvents[0] = spLibRegNotify_GetNotifyHandle();
	hWaitEvents[1] = CreateEvent( NULL, FALSE, FALSE, NULL );

	
	while( 1 )
	{
		if( bPreRunnerThreadStop )
			break;
		
		dwReturns = WaitForMultipleObjects( WAITEVENT_NUM, hWaitEvents, FALSE, dwWaitMS );

		switch( dwReturns )
		{
			case WAIT_OBJECT_0 + 0:
				/// got notify
				///no message here, or you will get a endless message loop...
				///spLibDbgMsg( LIBMSGFLAG_NULL, TEXT("%s MonitorEventThread 1 !!!"), SPPREFIX );
				RegNotifyExeCallback( 0 );
				CeFindNextRegChange( hWaitEvents[0] );
				break;
			case WAIT_OBJECT_0 + 1:
				/// got control event
				spLibDbgMsg( LIBMSGFLAG, TEXT("%s PreRunnerThread 2 !!!"), SPPREFIX );
					
				break;
			case WAIT_TIMEOUT:
				///Sleep( 1000 );
				///DrawCPUMem( bCPUMsgShow );
				spLibDbgMsg( LIBMSGFLAG_NK, TEXT("Pre Runnner running!!!") );
			break;
	
			default:
				break;
				
			}	///switch		
		
	}
	
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("End Pre Runnner !!!") );
	
	return dwRet;
}

static BOOL IsAllThreadStop( void )
{
	BOOL bRet = FALSE; 
	
	
	return bRet;
}



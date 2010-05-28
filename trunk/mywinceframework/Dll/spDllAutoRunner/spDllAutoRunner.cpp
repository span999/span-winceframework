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
#include <pnp.h>
#include <storemgr.h>
#include "SPDebugDef.h"
#include "spLibARFileParser.h"



#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
#define		LIBMSGFLAG_NK			(dNOUT|0x0FFFFFFF)		///NK msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only

#define		SPPREFIX			TEXT("ARDll:")

#define MIC_AUTORUNNER_REG_KEY					(TEXT("\\Software\\Mitac\\micAutoRunner"))


static DWORD MainRoutine( DWORD dwPararm );
static DWORD WINAPI StoreageMsgWaitThread( LPVOID pContext );
static DWORD ParseCmdFile( DWORD dwParam );
static BOOL IsAllThreadStop( void );
static BOOL spCopyFile( LPCWSTR szwSourceFile, LPCWSTR szwTargetFile, BOOL bFlag );

static bStoreageMsgWaitThreadStop = FALSE;



/// *****************************************
/// routine
/// *****************************************


BOOL WINAPI  DllEntry(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        ///DEBUGREGISTER(hinstDll);
		///DisableThreadLibraryCalls((HMODULE) hModule);
		DisableThreadLibraryCalls((HMODULE) hinstDll);
        SPDMSG(dINIT, (_T("SPD@%s() : Process Attach\r\n"), _T(__FUNCTION__)));
		///spLibDbgMsg_Dlg( TEXT("spDllAutoRunner DLL_PROCESS_ATTACH!!!") );
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        SPDMSG(dINIT, (_T("SPD@%s() : Process Detach\r\n"), _T(__FUNCTION__)));
		///spLibDbgMsg_Dlg( TEXT("spDllAutoRunner DLL_PROCESS_DETACH!!!") );
    }

    return TRUE;
}


DWORD spDllHook_Start( DWORD dwParam )
{
	DWORD dwRet;
	
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("spDllAutoRunner_Start 1!!!") );
	dwRet = MainRoutine( dwParam );
	spLibDbgMsg( LIBMSGFLAG_NK, TEXT("spDllAutoRunner_Start 2!!!") );
	
	return dwRet;
}


///BLOCK_DRIVER_GUID
///DEVCLASS_CARDSERV_GUID
///FATFS_MOUNT_GUID
///STORE_MOUNT_GUID
///STOREMGR_DRIVER_GUID
///CE_DRIVER_SD_BUS_GUID


static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;
	HANDLE hStoreageMsgWaitThread = NULL;
	DWORD dwStoreageMsgWaitThreadId = 0;
	DWORD dwThreadPararm = 0;
	
	hStoreageMsgWaitThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)StoreageMsgWaitThread, (LPVOID)&dwThreadPararm, 0, (LPDWORD)&dwStoreageMsgWaitThreadId );
	///hStoreageMsgWaitThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)StoreageMsgWaitThread, NULL, 0, NULL );
	
	if( NULL == hStoreageMsgWaitThread )
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("CreateThread fail %d !!!"), GetLastError() );
	}	
	else
		bStoreageMsgWaitThreadStop = FALSE;
	
	
	while( !IsAllThreadStop() )
		Sleep(1111);
	
	return dwRet;
}


static DWORD WINAPI StoreageMsgWaitThread( LPVOID  pContext )
{
	DWORD dwRet = 0;

	DWORD dwCount = 0;	
    MSGQUEUEOPTIONS msgQueueOptions;
    HANDLE hDevNotificationQueue = NULL;
    HANDLE hDevNotifications = NULL;
    WCHAR buf[MAX_PATH + sizeof(DEVDETAIL)];
    DWORD dwFlags     = 0;
    DEVDETAIL *lpDevDetail = (DEVDETAIL *) buf;
    DWORD dwBytesRead = sizeof(buf);

	
	spLibDbgMsg_Dlg( TEXT("go Auto Runnner !!!") );

    // register for filesystem mount/unmount notifications
    memset(&msgQueueOptions, 0, sizeof(msgQueueOptions));
    msgQueueOptions.dwSize        = sizeof( msgQueueOptions );
    msgQueueOptions.dwFlags       = 0;
    msgQueueOptions.dwMaxMessages = 0;
    msgQueueOptions.cbMaxMessage  = 4096;
    msgQueueOptions.bReadAccess   = TRUE;	
	
    hDevNotificationQueue = CreateMsgQueue( NULL, &msgQueueOptions );
    ///ASSERT(hDevNotificationQueue);
    if( hDevNotificationQueue )
    {
		BOOL bReadQueueDone = FALSE;
		BOOL bFoundStr = FALSE;
        GUID fatfs = FATFS_MOUNT_GUID;
		
        hDevNotifications = RequestDeviceNotifications( &fatfs, hDevNotificationQueue, TRUE );
        ///ASSERT(hDevNotifications);
		
		while( TRUE )
		{
			do
            {
				bReadQueueDone = FALSE;

				bReadQueueDone = ReadMsgQueue( hDevNotificationQueue, lpDevDetail, dwBytesRead, &dwCount, INFINITE, &dwFlags );
				///bReadQueueDone = ReadMsgQueue( hDevNotificationQueue, lpDevDetail, dwBytesRead, &dwCount, 40000, &dwFlags );
			
				if( FALSE == bReadQueueDone )
				{
					if( ERROR_TIMEOUT == GetLastError() )
						break;
				}
				else
				{
					if( 0 == wcscmp( TEXT("SDMMC"), lpDevDetail->szName ) )
						bFoundStr = TRUE;
					else	
					if( 0 == wcscmp( TEXT("Storage Card"), lpDevDetail->szName ) )
						bFoundStr = TRUE;
					else
						bFoundStr = FALSE;
				}

			}while( !(bReadQueueDone && lpDevDetail->fAttached && bFoundStr ) );
		

			if( bReadQueueDone )
			{
				spLibDbgMsg_Dlg( TEXT("Got storeage mount event2 %d(%s)"), lpDevDetail->cbName, lpDevDetail->szName );
				if( 1 == ParseCmdFile( 0 ) )
					break;
			}	
			else
			{
				spLibDbgMsg( LIBMSGFLAG, TEXT("Timeout2 !!!") );
				break;
			}	
		
		}///while()		
	
		CloseMsgQueue( hDevNotificationQueue );
    }
	
	
	bStoreageMsgWaitThreadStop = TRUE;
	spLibDbgMsg( LIBMSGFLAG, TEXT("End Auto Runnner !!!") );
	return dwRet;
}

static BOOL IsAllThreadStop( void )
{
	BOOL bRet = FALSE; 
	
	if( bStoreageMsgWaitThreadStop )
		bRet = TRUE;
	
	return bRet;
}


static DWORD ParseCmdFile( DWORD dwParam )
{
	DWORD dwRet = 0;
	BOOL bRet = 0;
	
	/// load setting first
	///LoadRegSetting();
	spLibLoadRegCmdSetting();
	
	///dwRet = CmdFileParser( 0 );
	dwRet = spLibCmdFileParser( 0 );
	
	return dwRet;
}




#if 0
#else
BOOL spCopyFile( LPCWSTR szwSourceFile, LPCWSTR szwTargetFile, BOOL bFlag )
{
	#define SPFILEBUFFERSIZE	128
	BOOL bRet = TRUE;
	FILE *hSrcFile = NULL, *hDestFile = NULL;
	BYTE fbuffer[SPFILEBUFFERSIZE];
	int iReadCount = 0;
	
	
	do
	{
		// Open the source file.
		if( ( hSrcFile = _wfopen( szwSourceFile, TEXT("rb") ) ) == NULL ) 
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s open src file fail %s"), SPPREFIX, szwSourceFile );
			bRet = FALSE;
			break;
		}

		// Open the destination file.
		if( ( hDestFile = _wfopen( szwTargetFile, TEXT("wb") ) ) == NULL ) 
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s open targ file fail %s"), SPPREFIX, szwTargetFile );
			bRet = FALSE;
			break;
		}

		/// do the copy
		while( !feof( hSrcFile ) )	///check if EOF
		{
			/* Attempt to read src file in SPFILEBUFFERSIZE bytes: */
			iReadCount = fread( fbuffer, sizeof( BYTE ), SPFILEBUFFERSIZE, hSrcFile );
			if( ferror( hSrcFile ) )
			{	///check if anything wrong
				spLibDbgMsg( LIBMSGFLAG, TEXT("%s fread src file fail %s"), SPPREFIX, szwSourceFile );
				bRet = FALSE;
				break;
			}
			else
			{	///write target file
				fwrite( fbuffer, sizeof( BYTE ), iReadCount, hDestFile );
				if( ferror( hSrcFile ) )
				{	///check if anything wrong
					spLibDbgMsg( LIBMSGFLAG, TEXT("%s fwrite targ file fail %s"), SPPREFIX, szwTargetFile );
					bRet = FALSE;
					break;
				}
			}
		}
	
		fclose( hSrcFile );
		fclose( hDestFile );
	
	}while( 0 );
	
	
	return bRet;
}
#endif
















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



///BLOCK_DRIVER_GUID
///DEVCLASS_CARDSERV_GUID
///FATFS_MOUNT_GUID
///STORE_MOUNT_GUID
///STOREMGR_DRIVER_GUID
///CE_DRIVER_SD_BUS_GUID

#if 0
static DWORD MainRoutine( DWORD dwPararm )
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

	
	///MessageBox( NULL, TEXT("go Auto Runnner !!!"), TEXT("MainRoutine"), MB_ICONQUESTION|MB_OK );
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
			do{
				bReadQueueDone = FALSE;

				///bReadQueueDone = ReadMsgQueue( hDevNotificationQueue, lpDevDetail, dwBytesRead, &dwCount, INFINITE, &dwFlags );
				bReadQueueDone = ReadMsgQueue( hDevNotificationQueue, lpDevDetail, dwBytesRead, &dwCount, 7000, &dwFlags );
			
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
				///MessageBox( NULL, TEXT("Got storeage mount event !!!"), TEXT("MainRoutine"), MB_ICONQUESTION|MB_OK );
				
				///wsprintf( szString, TEXT("Got storeage mount event %d(%s)"), lpDevDetail->cbName, lpDevDetail->szName );
				///MessageBox( NULL, szString, TEXT("MainRoutine"), MB_ICONQUESTION|MB_OK );
				spLibDbgMsg_Dlg( TEXT("Got storeage mount event2 %d(%s)"), lpDevDetail->cbName, lpDevDetail->szName );
			}	
			else
			{
				///MessageBox( NULL, TEXT("Timeout !!!"), TEXT("MainRoutine"), MB_ICONQUESTION|MB_OK );
				spLibDbgMsg_Dlg( TEXT("Timeout2 !!!") );
				break;
			}	
		
		}///while()		
	
		CloseMsgQueue( hDevNotificationQueue );
    }
	
	return dwRet;
}

#else
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
#endif

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


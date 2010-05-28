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


#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only
#define		SPPREFIX			TEXT("ARDll:")

#define MIC_AUTORUNNER_REG_KEY					(TEXT("\\Software\\Mitac\\micAutoRunner"))


static DWORD MainRoutine( DWORD dwPararm );
static DWORD WINAPI StoreageMsgWaitThread( LPVOID  pContext );
static DWORD ParseCmdFile( DWORD dwParam );
static BOOL LoadRegSetting( void );
static BOOL IsAllThreadStop( void );
static BOOL spCopyFile( LPCWSTR szwSourceFile, LPCWSTR szwTargetFile, BOOL bFlag );

static TCHAR szCmdFilePath[32];
static TCHAR szCmdFileName[32];
static TCHAR szRspFileName[32];
static DWORD dwCmdOption = 0;
static TCHAR szCmdSign[4];
static TCHAR szCmdMark[4];
static TCHAR szCmdSource[16];
static TCHAR szCmdTarget[16];
static TCHAR szCmdLine[16];
static TCHAR szCmdRun[16];
static TCHAR szCmdCopy[16];
static TCHAR szCmdMove[16];
static TCHAR szCmdDelete[16];
static TCHAR szCmdEnd[16];	
static TCHAR szSourceFile[256];
static TCHAR szTargetFile[256];
static TCHAR szRunCmdLine[256];
static TCHAR szCmdHide[16];
static TCHAR szCmdShow[16];
static TCHAR szCmdWait[16];
static TCHAR szCmdKeepCmd[32];

static BOOL	bKeepCmdFile = FALSE;
static bStoreageMsgWaitThreadStop = FALSE;



/// *****************************************
/// routine
/// *****************************************


BOOL WINAPI  DllEntry(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DEBUGREGISTER(hinstDll);
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
	
	///spLibDbgMsg_Dlg( TEXT("spDllAutoRunner_Start 1!!!") );
	dwRet = MainRoutine( dwParam );
	///spLibDbgMsg_Dlg( TEXT("spDllAutoRunner_Start 2!!!") );
	
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
///	hStoreageMsgWaitThread = NULL;
///	DWORD dwStoreageMsgWaitThreadId = 0;
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
			do{
				bReadQueueDone = FALSE;

				///bReadQueueDone = ReadMsgQueue( hDevNotificationQueue, lpDevDetail, dwBytesRead, &dwCount, INFINITE, &dwFlags );
				bReadQueueDone = ReadMsgQueue( hDevNotificationQueue, lpDevDetail, dwBytesRead, &dwCount, 40000, &dwFlags );
			
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
				spLibDbgMsg_Dlg( TEXT("Timeout2 !!!") );
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
	LoadRegSetting();
	
	
		if( 1 )
		{
		    FILE *fp = NULL;
			BYTE pPathFileData[256];
			PWSTR pTemp = (PWSTR)pPathFileData;
			UINT uiReTry = 5;
			
			wcscpy( (PWSTR)pPathFileData, (PWSTR)szCmdFilePath );
			pTemp = pTemp + (wcslen((PWSTR)pPathFileData));	///move to end
			wcscpy( (PWSTR)pTemp, (PWSTR)szCmdFileName );
			
			///spLibDbgMsg_Dlg( TEXT("Try reach cmd file %s"), pPathFileData );

			for( uiReTry = 4; uiReTry > 0; uiReTry-- ) 
			{
				Sleep(500);
				///try three path
				///first
				fp = _wfopen( (PWSTR)pPathFileData , _T("rt") );	///read,text mode
				
				if( fp == NULL )	///no, try second
				{
					memset( pPathFileData, 0, 256 );	///clear
					wcscpy( (PWSTR)pPathFileData, TEXT("\\SDMMC\\") );
					pTemp = (PWSTR)pPathFileData;
					pTemp = pTemp + (wcslen((PWSTR)pPathFileData));	///move to end
					wcscpy( (PWSTR)pTemp, (PWSTR)szCmdFileName );
					
					fp = _wfopen( (PWSTR)pPathFileData , _T("rt") );	///read,text mode
				}	

				if( fp == NULL )	///no, try third
				{
					memset( pPathFileData, 0, 256 );	///clear
					wcscpy( (PWSTR)pPathFileData, TEXT("\\Storage Card\\") );
					pTemp = (PWSTR)pPathFileData;
					pTemp = pTemp + (wcslen((PWSTR)pPathFileData));	///move to end
					wcscpy( (PWSTR)pTemp, (PWSTR)szCmdFileName );
					
					fp = _wfopen( (PWSTR)pPathFileData , _T("rt") );	///read,text mode
				}
					
				if( fp != NULL )
					break; 
			}	

			if( fp == NULL ) 
			{
				spLibDbgMsg( LIBMSGFLAG, TEXT("No such file %s"), pPathFileData );
			}
			else
			{
				#define MAX_LINE_LENGTH		256
				CHAR ucTempA[MAX_LINE_LENGTH];
				UINT uiIndex = 0;
				
				spLibDbgMsg( LIBMSGFLAG, TEXT("%s file found"), pPathFileData );

				///Parsing file
			    while(1)
			    {
			    	///clean buffer
			    	for( uiIndex = 0; uiIndex < MAX_LINE_LENGTH; uiIndex++ )
			    		ucTempA[MAX_LINE_LENGTH] = '\0';

			    	///get a line
			        if( fgets( ucTempA, MAX_LINE_LENGTH, fp) == NULL )	///NULL indicate error or EOF.
			        {
						///spLibDbgMsg_Dlg( TEXT("EOF or Error !!!") );
			        	break;
			        }
			        	
			        if( (';' == ucTempA[0]) )	///';' ignore char
			        {
						///wsprintf (szString, TEXT("Ignore sign!!!") );
						///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			        	continue;	
			        }
			        else
			        if( ('#' == ucTempA[0]) )	///'#' command
			        {
			        	UINT uiIdx = 1;
			        	TCHAR szTempStr[12];         // Temperary string
			        	TCHAR szTempStr1[128];         // Temperary string
			        	TCHAR *pTempStrPointer;
			        	TCHAR *pTempStrPointer1;
			        	
			        	bKeepCmdFile = FALSE;
			        	
						///wsprintf (szString, TEXT("Command sign!!!") );
						///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						
						///copy cmd to buffer
						pTempStrPointer = szTempStr;
						while( (':' != ucTempA[uiIdx]) )
						{
							///wsprintf (szString, TEXT("Cmd->%c") , ucTempA[uiIdx] );
							///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							
							*pTempStrPointer = (TCHAR)ucTempA[uiIdx];
							uiIdx++;
							pTempStrPointer++;
						}	
						*pTempStrPointer = (TCHAR)':';	///add ':'
						pTempStrPointer++;
						*pTempStrPointer = (TCHAR)'\0';	///add '\0'
						pTempStrPointer = szTempStr;


						///copy cmd parameter to buffer
						pTempStrPointer1 = szTempStr1;
						uiIdx++;
						while( ('\0' != ucTempA[uiIdx]) )
						{
							///wsprintf (szString, TEXT("Cmd->%c") , ucTempA[uiIdx] );
							///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							
							*pTempStrPointer1 = (TCHAR)ucTempA[uiIdx];
							uiIdx++;
							pTempStrPointer1++;
						}
						*pTempStrPointer1 = (TCHAR)'\0';	///add '\0'
						pTempStrPointer1--;
						*pTempStrPointer1 = (TCHAR)'\0';	///add '\0'
						///wsprintf (szString, TEXT("Cmd->%s") , szTempStr1 );
						///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

						
						///now, compare with cmd list
						if( 0 == wcscmp( szCmdSource, pTempStrPointer ) )
						{
							///spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s"), pTempStrPointer );
							wcscpy( szSourceFile, szTempStr1 );
							spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s"), pTempStrPointer, szSourceFile );

						}
						else
						if( 0 == wcscmp( szCmdTarget, pTempStrPointer ) )
						{
							///spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							wcscpy( szTargetFile, szTempStr1 );
							spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s"), pTempStrPointer, szTargetFile );
						}
						else
						if( 0 == wcscmp( szCmdLine, pTempStrPointer ) )
						{
							///spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							wcscpy( szRunCmdLine, szTempStr1 );
							spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s"), pTempStrPointer, szRunCmdLine );
						}
						else
						if( 0 == wcscmp( szCmdRun, pTempStrPointer ) )
						{
							///spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							///spLibDbgMsg_Dlg( TEXT("Run->%s"), szSourceFile );
							spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s"), pTempStrPointer, szSourceFile );

							///CreateProcess( szSourceFile, NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
							CreateProcess( szSourceFile, szRunCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);							
						}
						else
						if( 0 == wcscmp( szCmdCopy, pTempStrPointer ) )
						{
							///spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							///spLibDbgMsg_Dlg( TEXT("Copy %s->%s"), szSourceFile, szTargetFile );
							spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s->%s"), pTempStrPointer, szSourceFile, szTargetFile );

							bRet = spCopyFile( szSourceFile, szTargetFile, TRUE );
							///bRet = CopyFile( szSourceFile, szTargetFile, TRUE );
							if( 0 == bRet )
								spLibDbgMsg( LIBMSGFLAG, TEXT("Copy fail %d"), GetLastError() );
						}
						else
						if( 0 == wcscmp( szCmdMove, pTempStrPointer ) )
						{
							///spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							///spLibDbgMsg_Dlg( TEXT("Move %s->%s"), szSourceFile, szTargetFile );
							spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s->%s"), pTempStrPointer, szSourceFile, szTargetFile );

							bRet = MoveFile( szSourceFile, szTargetFile );
							if( 0 == bRet )
								spLibDbgMsg( LIBMSGFLAG, TEXT("Move fail %d"), GetLastError() );				
						}
						else
						if( 0 == wcscmp( szCmdDelete, pTempStrPointer ) )
						{
							///spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							///spLibDbgMsg_Dlg( TEXT("delete %s"), szSourceFile );
							spLibDbgMsg_Dlg( TEXT("Cmd->%s:%s"), pTempStrPointer, szSourceFile );
							
							bRet = DeleteFile( szSourceFile );
							if( 0 == bRet )
								spLibDbgMsg( LIBMSGFLAG, TEXT("delete fail %d"), GetLastError() );
						}
						else
						if( 0 == wcscmp( szCmdEnd, pTempStrPointer ) )
						{
							spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							
							///to end the parser !!!
							dwRet = 1;
							
							///move the screen to the last message		
							///DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_GETCOUNT, 0, 0);							
							///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
							
							///if( !bKeepCmdFile )
							///	;
							
							break;
						}
						else
						if( 0 == wcscmp( szCmdShow, pTempStrPointer ) )
						{
							spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							
							///set the option show autorunner dialog flag
							///dwCmdOption = dwCmdOption | CMDOPTION_SHOWAUTORUNNERDLG;
							///Do it
							///ShowAutoRunerDlg();
						}
						else
						if( 0 == wcscmp( szCmdHide, pTempStrPointer ) )
						{
							spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							
							///set the option show autorunner dialog flag
							///dwCmdOption = dwCmdOption & ~CMDOPTION_SHOWAUTORUNNERDLG;
							///Do it
							///ShowAutoRunerDlg();
						}
						else
						if( 0 == wcscmp( szCmdWait, pTempStrPointer ) )
						{
							spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							
							Sleep(1000);
						}
						else
						if( 0 == wcscmp( szCmdKeepCmd, pTempStrPointer ) )
						{
							spLibDbgMsg_Dlg( TEXT("Cmd->%s"), pTempStrPointer );
							
							bKeepCmdFile = TRUE;
						}
						else
						{
							spLibDbgMsg_Dlg( TEXT("??? unknow Cmd->%s"), pTempStrPointer );
						}
						
						///move the screen to the last message		
						///DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_GETCOUNT, 0, 0);							
						///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

			        	continue;
			        }	

			    }

				
				fclose( fp );
			}

			
		}
		else
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("wait unknow !!"));
		}

	
	return dwRet;
}


static BOOL LoadRegSetting()
{
	BOOL bRet = FALSE;
///	TCHAR szString[256];         // Temperary string
	HKEY hKey;
	BYTE pValueData[256];
	DWORD dwValueType = 0;
	DWORD dwValueLen = 0;
	DWORD dwValueData = 0;
	
	/// default at [HKEY_LOCAL_MACHINE\Software\Mitac\micAutoRunner]

	if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, MIC_AUTORUNNER_REG_KEY, 0, 0, &hKey ) )
	{		
		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdFilePath", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdFilePath=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdFilePath, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdFilePath=%s"), szCmdFilePath );
	    }
	    else
	    {	
	    	///wcscpy( (PWSTR)szCmdFilePath, (PWSTR)L"\\SDMMC Card\\" );
			wcscpy( (PWSTR)szCmdFilePath, (PWSTR)L"\\Storage Card\\" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdFilePath=%s"), szCmdFilePath );
	    }
	    
		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdFileName", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdFileName=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdFileName, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdFileName=%s"), szCmdFileName );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdFileName, (PWSTR)L"micAR.cmd" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdFileName=%s"), szCmdFileName );
	    }
	    
		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"RspFileName", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load RspFileName=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szRspFileName, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load RspFileName=%s"), szRspFileName );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szRspFileName, (PWSTR)L"micAR.rsp" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set RspFileName=%s"), szRspFileName );
	    }

		dwValueType = REG_DWORD;
		dwValueLen = sizeof( dwValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdOption", NULL, &dwValueType, (LPBYTE)&dwValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdOption=0x%08x"), dwValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	///wcscpy( (PWSTR)szRspFileName, (PWSTR)pValueData );
	    	dwCmdOption = dwValueData;
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdOption=0x%08x"), dwCmdOption );
	    }
	    else
	    {	
	    	///wcscpy( (PWSTR)szRspFileName, (PWSTR)L"micAR.rsp" );
	    	dwCmdOption = 0;
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdOption=0x%08x"), dwCmdOption );
	    }


		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdSign", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdSign=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdSign, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdSign=%s"), szCmdSign );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdSign, (PWSTR)L"#" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdSign=%s"), szCmdSign );
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdMark", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdMark=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdMark, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdMark=%s"), szCmdMark );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdMark, (PWSTR)L";" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdMark=%s"), szCmdMark );
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdSource", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdSource=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdSource, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdSource=%s"), szCmdSource );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdSource, (PWSTR)L"SRC:" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdSource=%s"), szCmdMark );
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdTarget", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdTarget=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdTarget, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdTarget=%s"), szCmdTarget );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdTarget, (PWSTR)L"TARG:" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdTarget=%s"), szCmdTarget );
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdLine", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdLine=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdLine, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdLine=%s"), szCmdLine );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdLine, (PWSTR)L"CMDL:" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdLine=%s"), szCmdLine );
	    }


		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdRun", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdRun=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdRun, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdRun=%s"), szCmdRun );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdRun, (PWSTR)L"RUN:" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdRun=%s"), szCmdRun );
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdCopy", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdCopy=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdCopy, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdCopy=%s"), szCmdCopy );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdCopy, (PWSTR)L"COPY:" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdCopy=%s"), szCmdCopy );
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdMove", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdMove=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdMove, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdMove=%s"), szCmdMove );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdMove, (PWSTR)L"MOVE:" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdMove=%s"), szCmdMove );
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdDelete", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdDelete=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdDelete, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdDelete=%s"), szCmdDelete );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdDelete, (PWSTR)L"DEL:" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdDelete=%s"), szCmdDelete );
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdEnd", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdEnd=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdEnd, (PWSTR)pValueData );
			
			///spLibDbgMsg_Dlg( TEXT("Load CmdEnd=%s"), szCmdEnd );
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdEnd, (PWSTR)L"END:" );
	    	
			///spLibDbgMsg_Dlg( TEXT("Set CmdEnd=%s"), szCmdEnd );
	    }

    	wcscpy( (PWSTR)szCmdShow, (PWSTR)L"SHOW:" );
		spLibDbgMsg_Dlg( TEXT("Set CmdShow=%s"), szCmdShow );

    	wcscpy( (PWSTR)szCmdHide, (PWSTR)L"HIDE:" );
		spLibDbgMsg_Dlg( TEXT("Set CmdHide=%s"), szCmdHide );

    	wcscpy( (PWSTR)szCmdWait, (PWSTR)L"WAIT:" );
		spLibDbgMsg_Dlg( TEXT("Set CmdWait=%s"), szCmdWait );

    	wcscpy( (PWSTR)szCmdKeepCmd, (PWSTR)L"KEEPCMD:" );
		spLibDbgMsg_Dlg( TEXT("Set CmdKeepCmd=%s"), szCmdKeepCmd );
	}
	else
	{		
		///load default
    	///wcscpy( (PWSTR)szCmdFilePath, (PWSTR)L"\\SDMMC Card\\" );
		wcscpy( (PWSTR)szCmdFilePath, (PWSTR)L"\\Storage Card\\" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdFilePath=%s"), szCmdFilePath );

    	wcscpy( (PWSTR)szCmdFileName, (PWSTR)L"micAR.cmd" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdFileName=%s"), szCmdFileName );

    	wcscpy( (PWSTR)szRspFileName, (PWSTR)L"micAR.rsp" );
		///spLibDbgMsg_Dlg( TEXT("Set RspFileName=%s"), szRspFileName );

    	wcscpy( (PWSTR)szCmdSign, (PWSTR)L"#" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdSign=%s"), szCmdSign );

    	wcscpy( (PWSTR)szCmdMark, (PWSTR)L";" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdMark=%s"), szCmdMark );

    	wcscpy( (PWSTR)szCmdSource, (PWSTR)L"SRC:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdSource=%s"), szCmdSource );

    	wcscpy( (PWSTR)szCmdTarget, (PWSTR)L"TARG:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdTarget=%s"), szCmdTarget );

    	wcscpy( (PWSTR)szCmdRun, (PWSTR)L"RUN:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdRun=%s"), szCmdRun );

    	wcscpy( (PWSTR)szCmdCopy, (PWSTR)L"COPY:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdCopy=%s"), szCmdCopy );

    	wcscpy( (PWSTR)szCmdMove, (PWSTR)L"MOVE:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdMove=%s"), szCmdMove );

    	wcscpy( (PWSTR)szCmdDelete, (PWSTR)L"DEL:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdDelete=%s"), szCmdDelete );

    	wcscpy( (PWSTR)szCmdEnd, (PWSTR)L"END:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdEnd=%s"), szCmdEnd );


    	wcscpy( (PWSTR)szCmdShow, (PWSTR)L"SHOW:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdShow=%s"), szCmdShow );

    	wcscpy( (PWSTR)szCmdHide, (PWSTR)L"HIDE:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdHide=%s"), szCmdHide );

    	wcscpy( (PWSTR)szCmdWait, (PWSTR)L"WAIT:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdWait=%s"), szCmdWait );

    	wcscpy( (PWSTR)szCmdKeepCmd, (PWSTR)L"KEEPCMD:" );
		///spLibDbgMsg_Dlg( TEXT("Set CmdKeepCmd=%s"), szCmdKeepCmd );
		
	}	
	
	return bRet;
}


BOOL spWstr2Tstr( LPCWSTR szwStr, LPCTSTR sztStr )
{
	BOOL bRet = FALSE;
	
	if( szwStr && sztStr )
	{
		WCHAR *pW = NULL;
		TCHAR *pT = NULL;
		DWORD dwLen = 0;
		DWORD dwLoop = 0;

		dwLen = wcslen((PWSTR)szwStr);
		pW = (WCHAR *)szwStr;
		pT = (TCHAR *)sztStr;
		
		while( dwLoop < dwLen )
		{
			*pT = (TCHAR)*pW;
			dwLoop++;
			pW++;
			pT++;
		}
		
		
		bRet = TRUE;
	}
	
	return bRet;
}


BOOL spCopyFile( LPCWSTR szwSourceFile, LPCWSTR szwTargetFile, BOOL bFlag )
{
	BOOL bRet = FALSE;
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s TCHAR size=%d"), SPPREFIX, sizeof(TCHAR) );
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s WCHAR size=%d"), SPPREFIX, sizeof(WCHAR) );
	
	///if( 1 == sizeof(TCHAR) )
	if( 1 )
	{
		TCHAR sztSourceFile[128];
		TCHAR sztTargetFile[128];
		
		spWstr2Tstr( szwSourceFile, sztSourceFile );
		spWstr2Tstr( szwTargetFile, sztTargetFile );

		bRet = CopyFile( sztSourceFile, sztTargetFile, bFlag );
	}
	else
	{
		bRet = CopyFile( szwSourceFile, szwTargetFile, bFlag );
	}
	
	return bRet;
}

















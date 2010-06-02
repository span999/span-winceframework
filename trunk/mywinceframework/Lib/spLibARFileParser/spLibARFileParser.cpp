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
#include "spLibARFileParser.h"

///


#define		SPPREFIX			TEXT("ARLib:")
#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
#define		LIBMSGFLAG_NK		(dNOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only


#define MIC_AUTORUNNER_REG_KEY					(TEXT("\\Software\\Mitac\\micAutoRunner"))


///phototype
static ARLIBCMDPAR_RSP spLibCmdParser( TCHAR *ptcCmd, TCHAR *ptcParam );



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




/// *****************************************
/// interface functions
/// *****************************************

ARLIBCMDPAR_RSP spLibCmdFileParser( DWORD dwParam )
{
	DWORD dwRet = 0;
	BOOL bRet = 0;
	ARLIBCMDPAR_RSP arRet = AR_OK;
	
    FILE *fp = NULL;
	BYTE pPathFileData[256];
	PWSTR pTemp = (PWSTR)pPathFileData;
	UINT uiReTry = 5;
		
	///spLibDbgMsg( LIBMSGFLAG, TEXT("Try reach cmd file %s"), pPathFileData );

	///try to find command file in storage card
	for( uiReTry = 4; uiReTry > 0; uiReTry-- ) 
	{
		Sleep(500);

		///try three path
		///first
		if( fp == NULL )
		{
			memset( pPathFileData, 0, 256 );	///clear
			wcscpy( (PWSTR)pPathFileData, (PWSTR)szCmdFilePath );	///load command file path
			pTemp = (PWSTR)pPathFileData;
			pTemp = pTemp + (wcslen((PWSTR)pPathFileData));	///move to end
			wcscpy( (PWSTR)pTemp, (PWSTR)szCmdFileName );	///load command file name just after path
		
			fp = _wfopen( (PWSTR)pPathFileData , _T("rt") );	///read,text mode
		}
			
		if( fp == NULL )	///no, try second
		{
			memset( pPathFileData, 0, 256 );	///clear
			wcscpy( (PWSTR)pPathFileData, TEXT("\\SDMMC\\") );
			pTemp = (PWSTR)pPathFileData;
			pTemp = pTemp + ( wcslen( (PWSTR)pPathFileData ) );	///move to end
			wcscpy( (PWSTR)pTemp, (PWSTR)szCmdFileName );
				
			fp = _wfopen( (PWSTR)pPathFileData , _T("rt") );	///read,text mode
		}	

		if( fp == NULL )	///no, try third
		{
			memset( pPathFileData, 0, 256 );	///clear
			wcscpy( (PWSTR)pPathFileData, TEXT("\\Storage Card\\") );
			pTemp = (PWSTR)pPathFileData;
			pTemp = pTemp + ( wcslen( (PWSTR)pPathFileData ) );	///move to end
			wcscpy( (PWSTR)pTemp, (PWSTR)szCmdFileName );
					
			fp = _wfopen( (PWSTR)pPathFileData , _T("rt") );	///read,text mode
		}
					
		if( fp != NULL )	///command file found! break for loop
			break; 
	}	///for	

	
	if( fp == NULL ) 
	{
		arRet = AR_NOCMDFILE;
		spLibDbgMsg( LIBMSGFLAG_NK, TEXT("No such file %s"), pPathFileData );
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
				arRet = AR_CMDFILEERR;
	        	break;
	        }
			        	
	        if( (';' == ucTempA[0]) )	///';' ignore char
	        {
				///wsprintf (szString, TEXT("Ignore sign!!!") );
				///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	        	continue;	
	        }
	        else
	        if( ('#' == ucTempA[0]) )	///'#' command prefix sign
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
				///pTempStrPointer = szTempStr;

				///copy cmd parameter to buffer
				pTempStrPointer1 = szTempStr1;
				uiIdx++;
				while( ('\0' != ucTempA[uiIdx]) )
				{
					///wsprintf (szString, TEXT("Cmd->%c") , ucTempA[uiIdx] );
					///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					
					if( '\\' == ucTempA[uiIdx] )	///in JPN system, we need this work aroud
					{
						uiIdx++;	///get next one
						if( '\\' == ucTempA[uiIdx] )
						{
							*pTempStrPointer1 = (TCHAR)ucTempA[uiIdx];
							uiIdx++;
							pTempStrPointer1++;
						}
					}
					else
					{
						*pTempStrPointer1 = (TCHAR)ucTempA[uiIdx];
						uiIdx++;
						pTempStrPointer1++;
					}
				}
				*pTempStrPointer1 = (TCHAR)'\0';	///add '\0'
			#if 0	
				pTempStrPointer1--;
				*pTempStrPointer1 = (TCHAR)'\0';	///add '\0'
			#else
				pTempStrPointer1--;
				*pTempStrPointer1 = (TCHAR)'\0';	///add '\0'
				pTempStrPointer1++;
				*pTempStrPointer1 = (TCHAR)'\0';	///add '\0'
				pTempStrPointer1++;
				*pTempStrPointer1 = (TCHAR)'\0';	///add '\0'
			#endif
				///wsprintf (szString, TEXT("Cmd->%s") , szTempStr1 );
				///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///dwRet = spLibCmdParser( szTempStr, szTempStr1 );
				arRet = spLibCmdParser( szTempStr, szTempStr1 );
				///if( 1 == dwRet )
				if( AR_EXIT == arRet )
					break;
				///move the screen to the last message		
				///DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_GETCOUNT, 0, 0);							
				///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
		       	continue;
			}	

		} ///while
				
		fclose( fp );
	}
	
	///return dwRet;
	return arRet;
}


static ARLIBCMDPAR_RSP spLibCmdParser( TCHAR *ptcCmd, TCHAR *ptcParam )
{
	DWORD dwRet = 0;
	BOOL bRet = FALSE;
	ARLIBCMDPAR_RSP arRet = AR_OK;

	if(	ptcCmd && ptcParam )
	{
		///now, compare with cmd list
		if( 0 == wcscmp( szCmdSource, ptcCmd ) )
		{	///SRC:
			wcscpy( szSourceFile, ptcParam );
			spLibDbgMsg( LIBMSGFLAG_NK, TEXT("Cmd->%s:%s"), ptcCmd, szSourceFile );
		}
		else
		if( 0 == wcscmp( szCmdTarget, ptcCmd ) )
		{	///TARG:
			wcscpy( szTargetFile, ptcParam );
			spLibDbgMsg( LIBMSGFLAG_NK, TEXT("Cmd->%s:%s"), ptcCmd, szTargetFile );
		}
		else
		if( 0 == wcscmp( szCmdLine, ptcCmd ) )
		{	///CMDL:
			wcscpy( szRunCmdLine, ptcParam );
			spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s"), ptcCmd, szRunCmdLine );
		}
		else
		if( 0 == wcscmp( szCmdRun, ptcCmd ) )
		{	//RUN:
			spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s"), ptcCmd, szSourceFile );

			///CreateProcess( szSourceFile, NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL );
			CreateProcess( szSourceFile, szRunCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL );							
		}
		else
		if( 0 == wcscmp( szCmdCopy, ptcCmd ) )
		{	///COPY:
			spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s->%s"), ptcCmd, szSourceFile, szTargetFile );

			///bRet = spCopyFile( szSourceFile, szTargetFile, TRUE );
			bRet = CopyFile( szSourceFile, szTargetFile, TRUE );
			if( FALSE == bRet )
			{
				spLibDbgMsg( LIBMSGFLAG, TEXT("Copy fail %d"), GetLastError() );
				arRet = AR_COPYFILEERR;
			}
		}
		else
		if( 0 == wcscmp( szCmdMove, ptcCmd ) )
		{	//MOV:
			spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s->%s"), ptcCmd, szSourceFile, szTargetFile );

			bRet = MoveFile( szSourceFile, szTargetFile );
			if( FALSE == bRet )
				spLibDbgMsg( LIBMSGFLAG, TEXT("Move fail %d"), GetLastError() );				
				arRet = AR_MOVEFILEERR;
			}
		else
		if( 0 == wcscmp( szCmdDelete, ptcCmd ) )
		{	///DEL:
			spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s:%s"), ptcCmd, szSourceFile );
					
			bRet = DeleteFile( szSourceFile );
			if( 0 == bRet )
			{
				spLibDbgMsg( LIBMSGFLAG, TEXT("delete fail %d"), GetLastError() );
				arRet = AR_DELETEFILEERR;
			}	
		}
		else
		if( 0 == wcscmp( szCmdEnd, ptcCmd ) )
		{	///END:
			spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s"), ptcCmd );
					
			///to end the parser !!!
			///dwRet = 1;
			arRet = AR_EXIT;
						
			///move the screen to the last message		
			///DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_GETCOUNT, 0, 0);							
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
			///if( !bKeepCmdFile )
			///	;
							
			///break;
		}
		else
		if( 0 == wcscmp( szCmdShow, ptcCmd ) )
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s"), ptcCmd );
				
			///set the option show autorunner dialog flag
			///dwCmdOption = dwCmdOption | CMDOPTION_SHOWAUTORUNNERDLG;
			///Do it
			///ShowAutoRunerDlg();
		}
		else
		if( 0 == wcscmp( szCmdHide, ptcCmd ) )
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s"), ptcCmd );
							
			///set the option show autorunner dialog flag
			///dwCmdOption = dwCmdOption & ~CMDOPTION_SHOWAUTORUNNERDLG;
			///Do it
			///ShowAutoRunerDlg();
		}
		else
		if( 0 == wcscmp( szCmdWait, ptcCmd ) )
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s"), ptcCmd );
						
			Sleep(1000);
		}
		else
		if( 0 == wcscmp( szCmdKeepCmd, ptcCmd ) )
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("Cmd->%s"), ptcCmd );
			
			bKeepCmdFile = TRUE;
		}
		else
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("??? unknow Cmd->%s"), ptcCmd );
		}
						
	}
	else
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s CmdParser null!!"), SPPREFIX );
	
	///return dwRet;
	return arRet;
}



BOOL spLibLoadRegCmdSetting( void )
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

// spCElogEx.c : Defines the entry point for the application.
//


#include <windows.h>
#include "spCElogCtrl.h"
#include "spCElog2txt.h"
///#include "spCEbuf.h"
#include "spCElog.h"
///#include "spCElogCmdRsp.h"
///#include "spCElogCRC.h"


///
#define SPERR						NKDbgPrintfW
///#define SPDBG						NKDbgPrintfW
#define SPDBG						///void

///#define DEVELOPER_DEBUG

#define APP_TITLE_STRING			TEXT("spLog ...")

///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
static HANDLE	hCtrlEvent = NULL;



///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-




///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
void spCtrl_ShowCtrlCmdRspDbgMsg( DWORD dwData )
{
	///Rsp
	if( SPCELOGCTRL_CTRLRSP_LOG_RUNNING == (dwData&SPCELOGCTRL_CTRLRSP_LOG_RUNNING) )
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlCmdRspDbgMsg: $$$ Logging is running ! $$$ \r\n") );

	if( SPCELOGCTRL_CTRLRSP_LOG_STOP == (dwData&SPCELOGCTRL_CTRLRSP_LOG_STOP) )
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlCmdRspDbgMsg: $$$ Logging is stop ! $$$ \r\n") );

	if( SPCELOGCTRL_CTRLRSP_OK == (dwData&SPCELOGCTRL_CTRLRSP_OK) )
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlCmdRspDbgMsg: $$$ response OK ! $$$ \r\n") );

	if( SPCELOGCTRL_CTRLRSP_FAIL == (dwData&SPCELOGCTRL_CTRLRSP_FAIL) )
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlCmdRspDbgMsg: $$$ response fail ! $$$ \r\n") );

	if( SPCELOGCTRL_CTRLRSP_TIMEOUT == (dwData&SPCELOGCTRL_CTRLRSP_TIMEOUT) )
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlCmdRspDbgMsg: $$$ response timeout ! $$$ \r\n") );

	if( SPCELOGCTRL_CTRLRSP_TIMEOUT == (dwData&SPCELOGCTRL_CTRLRSP_NOT_SUPPORT) )
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlCmdRspDbgMsg: $$$ response not supported ! $$$ \r\n") );
		
}


void spCtrl_ShowCtrlRspOKMsg( DWORD dwData )
{
	if( SPCELOGCTRL_CTRLRSP_OK == dwData )
	{
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlRspOKMsg: $$$ OK ! $$$ \r\n") );
		MessageBox( NULL, TEXT("Command OK !!"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
	}

	if( SPCELOGCTRL_CTRLRSP_FAIL == dwData )
	{
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlRspOKMsg: $$$ Fail ! $$$ \r\n") );
		MessageBox( NULL, TEXT("Command Fail !!"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
	}
	
	if( SPCELOGCTRL_CTRLRSP_TIMEOUT == dwData )
	{
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlRspOKMsg: $$$ Timeout ! $$$ \r\n") );
		MessageBox( NULL, TEXT("Command Timeout !!"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
	}

	if( SPCELOGCTRL_CTRLRSP_NOT_SUPPORT == dwData )
	{
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlRspOKMsg: $$$ Not supported ! $$$ \r\n") );
		MessageBox( NULL, TEXT("Command Not supported !!"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
	}

	if( SPCELOGCTRL_CTRLRSP_LOG_RUNNING == dwData )
	{
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlRspOKMsg: $$$ Log running ! $$$ \r\n") );
		MessageBox( NULL, TEXT("Log is running !!"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
	}

	if( SPCELOGCTRL_CTRLRSP_LOG_STOP == dwData )
	{
		SPDBG( TEXT("SPD@spCtrl_ShowCtrlRspOKMsg: $$$ Log is stop ! $$$ \r\n") );
		MessageBox( NULL, TEXT("Log is stop !!"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
	}
	
}


HANDLE spCtrl_GetCtrlEventHandle()
{
	HANDLE hRet = NULL;
	
	if( NULL == hCtrlEvent )
	{
		hRet = CreateEvent( NULL, FALSE, FALSE, SPCELOGCTRL_EVENT );
		
		if( NULL == hRet )
			SPERR( TEXT("SPD@spGet_CtrlEventHandle: CreateEvent fail !!!\r\n") );
		else
			hCtrlEvent = hRet;
	}
	
	hRet = hCtrlEvent;
	
	return hRet;
}

DWORD spCtrl_Command( DWORD dwCmd )
{
	DWORD dwRet = 0;
	DWORD dwTimeout = 5000;	///5 seconds
	
	if( (dwCmd & SPCELOGCTRL_CTRLCMD_BASE) > 0 )
	{
		dwRet = dwCmd;
		SetEventData( spCtrl_GetCtrlEventHandle(), dwCmd );
		SetEvent( spCtrl_GetCtrlEventHandle() );
		
		while( dwRet == dwCmd )
		{
			Sleep( 100 );
			dwRet = GetEventData( spCtrl_GetCtrlEventHandle() );
			SPDBG( TEXT("SPD@spCtrl_Command: GetEventdata =0x%08x \r\n"), dwRet );
			
			dwTimeout = dwTimeout - 100;
			if( (dwTimeout > 5000) || (dwTimeout < 0) )
			{
				SPERR( TEXT("SPD@spCtrl_Command: Rsponse timeout !!! 0x%08x \r\n"), dwCmd );
				dwRet = SPCELOGCTRL_CTRLRSP_TIMEOUT;
				break;
			}
		}
		
		SPDBG( TEXT("SPD@spCtrl_Command: SetEvent with Cmd code=0x%08x, Rsp code=0x%08x \r\n"), dwCmd, dwRet );
	}
	else
		SPERR( TEXT("SPD@spCtrl_Command: Not vaild Command code !!! 0x%08x \r\n"), dwCmd );
		
	if( SPCELOGCTRL_CTRLRSP_TIMEOUT == dwRet )
		MessageBox( NULL, TEXT("Command timeout !!! Please make sure logging function is supported in this device !"), APP_TITLE_STRING, (MB_OK|MB_ICONQUESTION|MB_SETFOREGROUND) );
	
	return dwRet;
}

DWORD spCtrl_GetCurLogStatus()
{
	DWORD dwRet = 0;
	
	dwRet = spCtrl_Command( SPCELOGCTRL_CTRLCMD_GETSTATUS );
	
	spCtrl_ShowCtrlCmdRspDbgMsg( dwRet );
	
	return dwRet;
}

DWORD spCtrl_GetFakeHostInit()
{
	DWORD dwRet = 0;
	
	dwRet = spCtrl_Command( SPCELOGCTRL_CTRLCMD_HOSTSINIT );
	
	spCtrl_ShowCtrlCmdRspDbgMsg( dwRet );
	
	return dwRet;
}

DWORD spCtrl_LogStart()
{
	DWORD dwRet = 0;
	
	dwRet = spCtrl_Command( SPCELOGCTRL_CTRLCMD_LOG_START );
	
	spCtrl_ShowCtrlCmdRspDbgMsg( dwRet );
	
	return dwRet;
}

DWORD spCtrl_LogStop()
{
	DWORD dwRet = 0;
	
	dwRet = spCtrl_Command( SPCELOGCTRL_CTRLCMD_LOG_STOP );
	
	spCtrl_ShowCtrlCmdRspDbgMsg( dwRet );
	
	return dwRet;
}

DWORD spCtrl_LogSpyMode()
{
	DWORD dwRet = 0;
	
	dwRet = spCtrl_Command( SPCELOGCTRL_CTRLCMD_LOG_SPY );
	
	spCtrl_ShowCtrlCmdRspDbgMsg( dwRet );
	
	return dwRet;
}

DWORD spCtrl_LogStandalone()
{
	DWORD dwRet = 0;
	
	dwRet = spCtrl_Command( SPCELOGCTRL_CTRLCMD_LOG_STANDALONE );
	
	spCtrl_ShowCtrlCmdRspDbgMsg( dwRet );
	
	return dwRet;
}


DWORD spCtrl_GetLogPackageCount()
{
	DWORD dwRet = 0;
	DWORD dwCount = 0;
	
	dwRet = spCtrl_Command( SPCELOGCTRL_CTRLCMD_GETCURRPKGNUM );
	
	if( dwRet != SPCELOGCTRL_CTRLRSP_FAIL && dwRet != SPCELOGCTRL_CTRLRSP_TIMEOUT )
	{
		dwCount = dwRet & ~SPCELOGCTRL_CTRLRSP_VALUE_BASE;
		dwRet = SPCELOGCTRL_CTRLRSP_OK;
	}
	
	spCtrl_ShowCtrlCmdRspDbgMsg( dwRet );
	
	return dwCount;
}

DWORD spCtrl_LogSaveOnDevice()
{
	DWORD dwRet = 0;
	
	dwRet = spCtrl_Command( SPCELOGCTRL_CTRLCMD_LOG_ON_DEV );
	
	spCtrl_ShowCtrlCmdRspDbgMsg( dwRet );
	
	return dwRet;
}

DWORD spCtrl_LogSaveOnSDcard()
{
	DWORD dwRet = 0;
	
	dwRet = spCtrl_Command( SPCELOGCTRL_CTRLCMD_LOG_ON_SD );
	
	spCtrl_ShowCtrlCmdRspDbgMsg( dwRet );
	
	return dwRet;
}

DWORD spCtrl_LogDebugLog()
{
	DWORD dwRet = 0;
#ifdef DEVELOPER_DEBUG	
	dwRet = spCtrl_Command( SPCELOGCTRL_CTRLCMD_LOG_DEBUGLOG );
	
	spCtrl_ShowCtrlCmdRspDbgMsg( dwRet );
#endif	
	return dwRet;
}

void spCtrl_UIGetTextDebugLog()
{
#ifdef DEVELOPER_DEBUG
	int		iRet = 0;
	
	iRet = MessageBox( NULL, TEXT("Do you want to get text debug log ?!"), APP_TITLE_STRING, (MB_YESNO|MB_ICONQUESTION|MB_SETFOREGROUND) );
	if( IDYES == iRet )
	{
		CopyFile( SPCELOG_LOGFILE_NAME, SPCELOG_STOPFILE_NAME, FALSE );
		///CopyFile( SPCELOG_SDLOGFILE_NAME, SPCELOG_SDSTOPFILE_NAME, FALSE );
		MakeCopyLog2txt();
		spCtrl_ShowCtrlRspOKMsg( SPCELOGCTRL_CTRLRSP_OK );
	}
#endif	
}

DWORD spCtrl_UISetLogOnSystemOrSD()
{
	int		iRet = 0;
	DWORD	dwRsp = 0;
	
	iRet = MessageBox( NULL, TEXT("Do you want to save log in system? or 'NO' to save log on SD card"), APP_TITLE_STRING, (MB_YESNO|MB_ICONQUESTION|MB_SETFOREGROUND) );
	if( IDYES == iRet )
	{
		dwRsp = spCtrl_LogSaveOnDevice();
		spCtrl_ShowCtrlRspOKMsg( dwRsp );
	}
	else
	{
		dwRsp = spCtrl_LogSaveOnSDcard();
		spCtrl_ShowCtrlRspOKMsg( dwRsp );
	}		
			
	if( dwRsp == SPCELOGCTRL_CTRLRSP_FAIL )
	{
		iRet = MessageBox( NULL, TEXT("'Can not save log at this location! please try again!"), APP_TITLE_STRING, (MB_OK|MB_ICONWARNING|MB_SETFOREGROUND) );
		///break;
		///continue;
	}
	
	return dwRsp;
}

DWORD spCtrl_UISetLogWithSpyMode()
{
	int		iRet = 0;
	DWORD	dwRsp = 0;

	iRet = MessageBox( NULL, TEXT("Set to use Spy mode !"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
	
	dwRsp = spCtrl_LogSpyMode();

	spCtrl_ShowCtrlRspOKMsg( dwRsp );
	return dwRsp;
}

DWORD spCtrl_UISetLogWithStandaloneMode()
{
	int		iRet = 0;
	DWORD	dwRsp = 0;
	
	iRet = MessageBox( NULL, TEXT("Set to use Standalone mode !        Will popup message show you if standalone started or not within 1 minute"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
	
	dwRsp = spCtrl_LogStandalone();
	
	spCtrl_ShowCtrlRspOKMsg( dwRsp );
	return dwRsp;
}

void spCtrl_UIGetCurrentPacketCount()
{
	int		iRet = 0;
	DWORD	dwRet = 0;
	
	iRet = MessageBox( NULL, TEXT("Do you want to get current log packet count ?!"), APP_TITLE_STRING, (MB_YESNO|MB_ICONQUESTION|MB_SETFOREGROUND) );
	if( IDYES == iRet )
	{
		TCHAR szTmp[128];
		
		dwRet = spCtrl_GetLogPackageCount();
		///spCtrl_ShowCtrlRspOKMsg( SPCELOGCTRL_CTRLRSP_OK );
		wsprintf( szTmp, TEXT( "Here we have %d packets in log already" ), dwRet );

		MessageBox( NULL, szTmp, APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
	}	
}

DWORD spCtrl_UIWaitFakeHostInit()
{
	DWORD dwRsp = 0;
	DWORD dwTimeout = 10;
					
	do{
		Sleep( 5000 );
		dwRsp = spCtrl_GetFakeHostInit();
		dwTimeout--;
		if( SPCELOGCTRL_CTRLRSP_OK == dwRsp )
			break;
	}while( dwTimeout > 0 );
					
	if( SPCELOGCTRL_CTRLRSP_OK == dwRsp )
		MessageBox( NULL, TEXT("Standalone Logging is starting !!!"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
	else
		MessageBox( NULL, TEXT("StandaloneLogging is not starting somehow !!!"), APP_TITLE_STRING, (MB_OK|MB_ICONERROR|MB_SETFOREGROUND) );

	return dwRsp;
}

BOOL spCtrl_Action( DWORD dwOption )
{
	BOOL 	bRet = TRUE;
	int		iRet = 0;
	DWORD	dwRsp = 0;

	do
	{
		iRet = MessageBox( NULL, TEXT("Are you trying to use this log function now ?!"), APP_TITLE_STRING, (MB_YESNO|MB_ICONQUESTION|MB_SETFOREGROUND) );
	
		if( IDYES == iRet )
		{
			///check if log is running or not
			dwRsp = spCtrl_GetCurLogStatus();
		
			if( SPCELOGCTRL_CTRLRSP_LOG_RUNNING == dwRsp )
			{	///log is running
				iRet = MessageBox( NULL, TEXT("Log is runnung!!        Do you want to stop the logging now ?!"), APP_TITLE_STRING, (MB_YESNO|MB_ICONQUESTION|MB_SETFOREGROUND) );
				if( IDYES == iRet )
				{
					dwRsp = spCtrl_LogStop();
					spCtrl_ShowCtrlRspOKMsg( dwRsp );
					
					spCtrl_UIGetTextDebugLog();
				}
				else
				{
					///spCtrl_UIGetTextDebugLog();
					///get current logging status
					spCtrl_UIGetCurrentPacketCount();
				}
			}
			else
			if( SPCELOGCTRL_CTRLRSP_LOG_STOP == dwRsp )
			{	///log is stop /not running
				dwRsp = spCtrl_UISetLogOnSystemOrSD();
				if( dwRsp == SPCELOGCTRL_CTRLRSP_FAIL )
				{
					continue;
				}

				iRet = MessageBox( NULL, TEXT("Do you want to start the logging now ?!"), APP_TITLE_STRING, (MB_YESNO|MB_ICONQUESTION|MB_SETFOREGROUND) );
				if( IDYES == iRet )
				{
					spCtrl_LogDebugLog();
				
					iRet = MessageBox( NULL, TEXT("Do you want to start logging with Spy mode ?       or 'NO' for Standalone Mode"), APP_TITLE_STRING, (MB_YESNO|MB_ICONQUESTION|MB_SETFOREGROUND) );
					if( IDYES == iRet )
					{
						spCtrl_UISetLogWithSpyMode();
					}
					else
					{
						spCtrl_UISetLogWithStandaloneMode();
					}
					
					if( dwRsp == SPCELOGCTRL_CTRLRSP_OK )
					{
						dwRsp = spCtrl_LogStart();
						spCtrl_ShowCtrlRspOKMsg( dwRsp );
					
						if( IDYES != iRet )
							spCtrl_UIWaitFakeHostInit();
					}
					else
					{
						MessageBox( NULL, TEXT("'Logging mode setting faill !!!"), APP_TITLE_STRING, (MB_OK|MB_ICONERROR|MB_SETFOREGROUND) );
					}

				}
				else
				{
					spCtrl_UIGetTextDebugLog();
				}

			}
			else
				;
			
		}
		else
		{
			///iRet = MessageBox( NULL, TEXT("Your input is 'No' !!"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
			iRet = MessageBox( NULL, TEXT("'No' !? then thank you!"), APP_TITLE_STRING, (MB_OK|MB_SETFOREGROUND) );
			///goto EXIT_ACTION;
			break;
		}
		
	}while( 0 );	
	
///EXIT_ACTION:

	return bRet;
}


//-------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int cmdShow )
{

	spCtrl_Action( 0 );
 
	return 0 ;
}


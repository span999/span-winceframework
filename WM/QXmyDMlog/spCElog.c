// spCElog.c : Defines the entry point for the application.
//

///#include <stdafx.h>
#include <windows.h>
#include "spCElog.h"
#include "spCEbuf.h"
#include "spCElog2txt.h"
#include "spCElogDLF.h"
#include "spCElogCtrl.h"
#include "spCElogFakeHost.h"

///
#define SPERR						NKDbgPrintfW
#define SPDBG						NKDbgPrintfW
///#define SPDBG						///void
#define SPCELOG_STATUS_LOCK			EnterCriticalSection(&spCElog_stat.cs);
#define SPCELOG_STATUS_UNLOCK		LeaveCriticalSection(&spCElog_stat.cs);

#define CTRL_HANDLE_CMD_TIMEOUT_LESS	(1000*60*5)
#define	CTRL_HANDLE_CMD_TIMEOUT			250	///ms
#define	CTRL_HANDLE_CMD_TIMEOUT_CONT	100	///ms


///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
BOOL spCElog_Inited();
BOOL spCElog_Write2File( DWORD dwBufNum );
DWORD WINAPI spCElog_Buffer_Handle_Thread( LPVOID pVoid );
DWORD WINAPI spCElog_Control_Handle_Thread( LPVOID pVoid );
BOOL spCElog_Is_DebugLogOn();



///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
static spCElog_status 				spCElog_stat;
static spCElogFakeHost_status		FakeHost_stat;



///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
BOOL spCElog_Get_Status_Inited()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = spCElog_stat.bInited;
	SPCELOG_STATUS_UNLOCK
	
	return bRet;
}

BOOL spCElog_Set_Status_Inited( BOOL bInit )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	if( TRUE == bInit )
		spCElog_stat.bInited += 1;
	else
		spCElog_stat.bInited = 0;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_Inited();
	
	return bRet;
}

BOOL spCElog_Get_Status_Stoping()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = spCElog_stat.bStoping;
	SPCELOG_STATUS_UNLOCK
	
	SPDBG( TEXT("SPD@spCElog_Get_Status_Stoping: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Set_Status_Stoping( BOOL bStop )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.bStoping = bStop;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_Stoping();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_Stoping: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Get_Status_IsStop()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = spCElog_stat.bIsStop;
	SPCELOG_STATUS_UNLOCK
	
	SPDBG( TEXT("SPD@spCElog_Get_Status_IsStop: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Set_Status_IsStop( BOOL bStop )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.bIsStop = bStop;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_IsStop();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_IsStop: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Get_Status_FakeHostEnabled()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = spCElog_stat.bFakeHostenabled;
	SPCELOG_STATUS_UNLOCK
	
	SPDBG( TEXT("SPD@spCElog_Get_Status_FakeHostEnabled: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Set_Status_FakeHostEnabled( BOOL bEnable )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.bFakeHostenabled = bEnable;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_FakeHostEnabled();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_FakeHostEnabled: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Get_Status_USBconnected()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = spCElog_stat.bUSBconnected;
	SPCELOG_STATUS_UNLOCK
	
	SPDBG( TEXT("SPD@spCElog_Get_Status_USBconnected: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Set_Status_USBconnected( BOOL bSet )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.bUSBconnected = bSet;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_USBconnected();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_USBconnected: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Get_Status_SaveLogOnSD()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = spCElog_stat.bSaveLogOnSD;
	SPCELOG_STATUS_UNLOCK
	
	return bRet;
}

BOOL spCElog_Set_Status_SaveLogOnSD( BOOL bSet )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.bSaveLogOnSD = bSet;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_SaveLogOnSD();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_SaveLogOnSD: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Get_Status_DebugLogEnabled()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = spCElog_stat.bDebugLogEnabled;
	SPCELOG_STATUS_UNLOCK
	
	return bRet;
}

BOOL spCElog_Set_Status_DebugLogEnabled( BOOL bSet )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.bDebugLogEnabled = bSet;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_DebugLogEnabled();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_DebugLogEnabled: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}


DWORD spCElog_Get_Status_LogMode()
{
	DWORD dwRet;
	
	SPCELOG_STATUS_LOCK
	dwRet = spCElog_stat.dwLogModes;
	SPCELOG_STATUS_UNLOCK
	
	return dwRet;
}

DWORD spCElog_Set_Status_InitMode()
{
	DWORD dwRet;
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.dwLogModes = SPCELOG_LOGMODE_INIT;
	SPCELOG_STATUS_UNLOCK	
		
	dwRet = spCElog_Get_Status_LogMode();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_InitMode: %d \r\n"), dwRet );
	return dwRet;
}

DWORD spCElog_Set_Status_SpyMode()
{
	DWORD dwRet;
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.dwLogModes = SPCELOG_LOGMODE_SPY;
	SPCELOG_STATUS_UNLOCK	
		
	dwRet = spCElog_Get_Status_LogMode();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_SpyMode: %d \r\n"), dwRet );
	return dwRet;
}

DWORD spCElog_Set_Status_StandaloneMode()
{
	DWORD dwRet;
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.dwLogModes = SPCELOG_LOGMODE_STANDALONE;
	SPCELOG_STATUS_UNLOCK	
		
	dwRet = spCElog_Get_Status_LogMode();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_StandaloneMode: %d \r\n"), dwRet );
	return dwRet;
}


HANDLE spCElog_Get_Status_USBDTReventHandler()
{
	HANDLE hRet;
	
	SPCELOG_STATUS_LOCK
	hRet = spCElog_stat.hDTREvent;
	SPCELOG_STATUS_UNLOCK
	
	return hRet;
}

HANDLE spCElog_Set_Status_USBDTReventHandler( HANDLE hSet )
{
	HANDLE hRet;
	
	if( NULL == hSet ) 
	{
		goto END_SET_STATUS_DTR;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.hDTREvent = hSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_DTR:	
	hRet = spCElog_Get_Status_USBDTReventHandler();
	
	return hRet;
}

HANDLE spCElog_Get_Status_SIOUSBconnectEventHandler()
{
	HANDLE hRet;
	
	SPCELOG_STATUS_LOCK
	hRet = spCElog_stat.hSIOUSBconnectEvent;
	SPCELOG_STATUS_UNLOCK
	
	return hRet;
}

HANDLE spCElog_Set_Status_SIOUSBconnectEventHandler( HANDLE hSet )
{
	HANDLE hRet;
	
	if( NULL == hSet ) 
	{
		goto END_SET_STATUS_DTR1;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.hSIOUSBconnectEvent = hSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_DTR1:	
	hRet = spCElog_Get_Status_SIOUSBconnectEventHandler();
	
	return hRet;
}

HANDLE spCElog_Get_Status_SIOUSBdisconnectEventHandler()
{
	HANDLE hRet;
	
	SPCELOG_STATUS_LOCK
	hRet = spCElog_stat.hSIOUSBdisconnectEvent;
	SPCELOG_STATUS_UNLOCK
	
	return hRet;
}

HANDLE spCElog_Set_Status_SIOUSBdisconnectEventHandler( HANDLE hSet )
{
	HANDLE hRet;
	
	if( NULL == hSet ) 
	{
		goto END_SET_STATUS_DTR2;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.hSIOUSBdisconnectEvent = hSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_DTR2:	
	hRet = spCElog_Get_Status_USBDTReventHandler();
	
	return hRet;
}

HANDLE spCElog_Get_Status_BufferHandling()
{
	HANDLE hRet;
	
	SPCELOG_STATUS_LOCK
	hRet = spCElog_stat.hBufferHandling;
	SPCELOG_STATUS_UNLOCK
	
	return hRet;
}

HANDLE spCElog_Set_Status_BufferHandling( HANDLE hSet )
{
	HANDLE hRet;
	
	if( NULL == hSet ) 
	{
		goto END_SET_STATUS_BUFFERHANDLE;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.hBufferHandling = hSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_BUFFERHANDLE:	
	hRet = spCElog_Get_Status_BufferHandling();
	
	return hRet;
}

HANDLE spCElog_Get_Status_LogCtrlHandling()
{
	HANDLE hRet;
	
	SPCELOG_STATUS_LOCK
	hRet = spCElog_stat.hLogCtrlHandling;
	SPCELOG_STATUS_UNLOCK
	
	return hRet;
}

HANDLE spCElog_Set_Status_LogCtrlHandling( HANDLE hSet )
{
	HANDLE hRet;
	
	if( NULL == hSet ) 
	{
		goto END_SET_STATUS_LOGCTRLHANDLE;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.hLogCtrlHandling = hSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_LOGCTRLHANDLE:	
	hRet = spCElog_Get_Status_LogCtrlHandling();
	
	return hRet;
}


HANDLE spCElog_Get_Status_BufferEvent()
{
	HANDLE hRet;
	
	SPCELOG_STATUS_LOCK
	hRet = spCElog_stat.hBufferEvent;
	SPCELOG_STATUS_UNLOCK
	
	return hRet;
}

HANDLE spCElog_Set_Status_BufferEvent( HANDLE hSet )
{
	HANDLE hRet;
	
	if( NULL == hSet ) 
	{
		goto END_SET_STATUS_BUFFERHANDLE;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.hBufferEvent = hSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_BUFFERHANDLE:	
	hRet = spCElog_Get_Status_BufferEvent();
	
	return hRet;
}

HANDLE spCElog_Get_Status_CtrlEvent()
{
	HANDLE hRet;
	
	SPCELOG_STATUS_LOCK
	hRet = spCElog_stat.hCtrlEvent;
	SPCELOG_STATUS_UNLOCK
	
	return hRet;
}

HANDLE spCElog_Set_Status_CtrlEvent( HANDLE hSet )
{
	HANDLE hRet;
	
	if( NULL == hSet ) 
	{
		goto END_SET_STATUS_CTRLEVENT;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.hCtrlEvent = hSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_CTRLEVENT:	
	hRet = spCElog_Get_Status_CtrlEvent();
	
	return hRet;
}

DWORD spCElog_Get_Status_BufferWaitTimeout()
{
	DWORD dwRet;
	
	SPCELOG_STATUS_LOCK
	dwRet = spCElog_stat.dwBufferWaitTimeout;
	SPCELOG_STATUS_UNLOCK
	
	return dwRet;
}

DWORD spCElog_Set_Status_BufferWaitTimeout( DWORD dwSet )
{
	DWORD dwRet;
	
	if( 0 == dwSet ) 
	{
		goto END_SET_STATUS_BUFFERHANDLE;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.dwBufferWaitTimeout = dwSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_BUFFERHANDLE:	
	dwRet = spCElog_Get_Status_BufferWaitTimeout();
	
	return dwRet;
}

DWORD spCElog_Get_Status_LastCmdTime()
{
	DWORD dwRet;
	
	SPCELOG_STATUS_LOCK
	dwRet = spCElog_stat.dwLastCmdTime;
	SPCELOG_STATUS_UNLOCK
	
	return dwRet;
}

DWORD spCElog_Set_Status_LastCmdTime( DWORD dwSet )
{
	DWORD dwRet;
	
	if( 0 == dwSet ) 
	{
		goto END_SET_STATUS_LASTCMDTIME;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.dwLastCmdTime = dwSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_LASTCMDTIME:	
	dwRet = spCElog_Get_Status_LastCmdTime();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_LastCmdTime: %d \r\n"), dwRet );
	return dwRet;
}

DWORD spCElog_Get_Status_LastRspTime()
{
	DWORD dwRet;
	
	SPCELOG_STATUS_LOCK
	dwRet = spCElog_stat.dwLastRspTime;
	SPCELOG_STATUS_UNLOCK
	
	return dwRet;
}

DWORD spCElog_Set_Status_LastRspTime( DWORD dwSet )
{
	DWORD dwRet;
	
	if( 0 == dwSet ) 
	{
		goto END_SET_STATUS_LASTRSPTIME;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.dwLastRspTime = dwSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_LASTRSPTIME:	
	dwRet = spCElog_Get_Status_LastRspTime();
	
	SPDBG( TEXT("SPD@spCElog_Set_Status_LastRspTime: %d \r\n"), dwRet );
	return dwRet;
}

DWORD spCElog_Get_Status_LastEventTime()
{
	DWORD dwRet;
	
	SPCELOG_STATUS_LOCK
	dwRet = spCElog_stat.dwLastEventTime;
	SPCELOG_STATUS_UNLOCK
	
	return dwRet;
}

DWORD spCElog_Set_Status_LastEventTime( DWORD dwSet )
{
	DWORD dwRet;
	
	if( 0 == dwSet ) 
	{
		goto END_SET_STATUS_LASTEVENTTIME;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.dwLastEventTime = dwSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_LASTEVENTTIME:	
	dwRet = spCElog_Get_Status_LastEventTime();
	
	return dwRet;
}

DWORD spCElog_Get_Status_CtrlWaitTimeout()
{
	DWORD dwRet;
	
	///SPCELOG_STATUS_LOCK
	///dwRet = spCElog_stat.dwBufferWaitTimeout;
	///SPCELOG_STATUS_UNLOCK
	dwRet = INFINITE;
	
	return dwRet;
}


HANDLE spCElog_Get_Status_FileHandle()
{
	HANDLE hRet;
	
	SPCELOG_STATUS_LOCK
	hRet = spCElog_stat.hFile;
	SPCELOG_STATUS_UNLOCK
	
	return hRet;
}



HANDLE spCElog_Set_Status_FileHandle( HANDLE hSet )
{
	HANDLE hRet;
	
	if( NULL == hSet ) 
	{
		goto END_SET_STATUS_FILEHANDLE;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.hFile = hSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_FILEHANDLE:	
	hRet = spCElog_Get_Status_FileHandle();
	
	return hRet;
}

HANDLE spCElog_Get_Status_DLFFileHandle()
{
	HANDLE hRet;
	
	SPCELOG_STATUS_LOCK
	hRet = spCElog_stat.hDLFFile;
	SPCELOG_STATUS_UNLOCK
	
	return hRet;
}

HANDLE spCElog_Set_Status_DLFFileHandle( HANDLE hSet )
{
	HANDLE hRet;
	
	if( NULL == hSet ) 
	{
		goto END_SET_STATUS_DLFFILEHANDLE;
	}
	
	SPCELOG_STATUS_LOCK
	spCElog_stat.hDLFFile = hSet;
	SPCELOG_STATUS_UNLOCK	

END_SET_STATUS_DLFFILEHANDLE:	
	hRet = spCElog_Get_Status_DLFFileHandle();
	
	return hRet;
}


DWORD spCElog_Get_Status_ExtraInfo()
{
	DWORD dwRet;
	
	SPCELOG_STATUS_LOCK
	dwRet = spCElog_stat.dwExtraInfo;
	SPCELOG_STATUS_UNLOCK
	
	return dwRet;
}

BOOL spCElog_Get_Status_ExtraInfo_Enable()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = (spCElog_stat.dwExtraInfo & SPCELOG_EXTRAINFO_ENABLE_MASK) > 0 ? TRUE : FALSE;
	SPCELOG_STATUS_UNLOCK
	
	return bRet;
}

BOOL spCElog_Set_Status_ExtraInfo_Enable( BOOL bEnable )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	if( TRUE == bEnable )
		spCElog_stat.dwExtraInfo  |= SPCELOG_EXTRAINFO_ENABLE_MASK;
	else
		spCElog_stat.dwExtraInfo  &= ~SPCELOG_EXTRAINFO_ENABLE_MASK;;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_ExtraInfo_Enable();
	
	return bRet;
}

BOOL spCElog_Get_Status_ExtraInfo_TimeStamp_Enable()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = (spCElog_stat.dwExtraInfo & SPCELOG_EXTRAINFO_TIMESTAMP_ENABLE_MASK) > 0 ? TRUE : FALSE;
	SPCELOG_STATUS_UNLOCK
	
	return bRet;
}

BOOL spCElog_Set_Status_ExtraInfo_TimeStamp_Enable( BOOL bEnable )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	if( TRUE == bEnable )
		spCElog_stat.dwExtraInfo  |= SPCELOG_EXTRAINFO_TIMESTAMP_ENABLE_MASK;
	else
		spCElog_stat.dwExtraInfo  &= ~SPCELOG_EXTRAINFO_TIMESTAMP_ENABLE_MASK;;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_ExtraInfo_TimeStamp_Enable();
	
	return bRet;
}

BOOL spCElog_Get_Status_ExtraInfo_Order_Enable()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = (spCElog_stat.dwExtraInfo & SPCELOG_EXTRAINFO_ORDER_ENABLE_MASK) > 0 ? TRUE : FALSE;
	SPCELOG_STATUS_UNLOCK
	
	return bRet;
}

BOOL spCElog_Set_Status_ExtraInfo_Order_Enable( BOOL bEnable )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	if( TRUE == bEnable )
		spCElog_stat.dwExtraInfo  |= SPCELOG_EXTRAINFO_ORDER_ENABLE_MASK;
	else
		spCElog_stat.dwExtraInfo  &= ~SPCELOG_EXTRAINFO_ORDER_ENABLE_MASK;;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_ExtraInfo_Order_Enable();
	
	return bRet;
}

BOOL spCElog_Get_Status_ExtraInfo_Direction_Enable()
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	bRet = (spCElog_stat.dwExtraInfo & SPCELOG_EXTRAINFO_DIRECTION_ENABLE_MASK) > 0 ? TRUE : FALSE;
	SPCELOG_STATUS_UNLOCK
	
	return bRet;
}

BOOL spCElog_Set_Status_ExtraInfo_Direction_Enable( BOOL bEnable )
{
	BOOL bRet;
	
	SPCELOG_STATUS_LOCK
	if( TRUE == bEnable )
		spCElog_stat.dwExtraInfo  |= SPCELOG_EXTRAINFO_DIRECTION_ENABLE_MASK;
	else
		spCElog_stat.dwExtraInfo  &= ~SPCELOG_EXTRAINFO_DIRECTION_ENABLE_MASK;;
	SPCELOG_STATUS_UNLOCK	
		
	bRet = spCElog_Get_Status_ExtraInfo_Direction_Enable();
	
	return bRet;
}

///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
BOOL spCElog_WaitForRsp()
{
	BOOL bRet;
	
	if( spCElog_Get_Status_LastCmdTime() <= spCElog_Get_Status_LastRspTime() )
		bRet = FALSE;
	else
		bRet = TRUE;

	SPDBG( TEXT("SPD@spCElog_WaitForRsp: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_WaitForCmd()
{
	BOOL bRet;
	
	if( __spCElogFakeHost_Get_NextCmdRead() )
	{	///this cmd has been read before, wait for update
		bRet = TRUE;
		if( HOST_CMD_TABLE_SIZE < __spCElogFakeHost_Get_StatusNum() )
			Sleep( 1000 );	///we need less polling after init.
		else
			Sleep( 50 );	///do some wait here
	}
	else
	{
		bRet = FALSE;
	}
	SPDBG( TEXT("SPD@spCElog_WaitForCmd: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

/*
	spCElog_Inited() is not protect by critical section.
	so it could be used while critical sction possiblily not inited.
*/
BOOL spCElog_Inited()
{
	BOOL bRet;
	
	bRet = spCElog_stat.bInited > 0 ? TRUE : FALSE;
	
	return bRet;
}

BOOL spCElog_StopFile_Found()
{
	BOOL bRet = FALSE;
	HANDLE hFile = NULL;
	
	if( spCElog_Get_Status_SaveLogOnSD() )	///save to SD card?? or ?
		hFile = CreateFile( SPCELOG_SDSTOPFILE_NAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	else
		hFile = CreateFile( SPCELOG_STOPFILE_NAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		
	if( hFile == INVALID_HANDLE_VALUE )
		SPDBG( TEXT("SPD@spCElog_StopFile_Found: not found \r\n") );
	else
	{
		SPERR( TEXT("SPD@spCElog_StopFile_Found: found !!!\r\n") );
		CloseHandle( hFile );
		bRet = TRUE;
	}
	
	return bRet;
}

BOOL spCElog_StopSignal_Found()
{
	BOOL bRet;
	
	bRet = spCElog_StopFile_Found();
	if( bRet ) 
		spCElog_Set_Status_Stoping( TRUE );
		
	return bRet;
}

HANDLE spCElog_CreateFile()
{
	HANDLE hFile = NULL;
	
	if( !spCElog_Is_DebugLogOn() )
		return hFile;
	
	if( spCElog_Get_Status_SaveLogOnSD() )	///save to SD card?? or ?
		hFile = CreateFile( SPCELOG_SDLOGFILE_NAME, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	else
		hFile = CreateFile( SPCELOG_LOGFILE_NAME, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		
	if( hFile == INVALID_HANDLE_VALUE )
		SPERR( TEXT("SPD@spCElog_CreateFile:CreateFile() failed \r\n") );
	spCElog_Set_Status_FileHandle( hFile );
	
	SetFilePointer( hFile, 0, 0, FILE_END );

	return hFile;
}

HANDLE spCElog_CreateDLFFile()
{
	HANDLE hFile = NULL;

	if( spCElog_Get_Status_SaveLogOnSD() )	///save to SD card?? or ?
		hFile = CreateFile( SPCELOG_SDDLFFILE_NAME, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	else
		hFile = CreateFile( SPCELOG_DLFFILE_NAME, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	
	if( hFile == INVALID_HANDLE_VALUE )
		SPERR( TEXT("SPD@spCElog_CreateDLFFile:CreateFile() failed \r\n") );
	spCElog_Set_Status_DLFFileHandle( hFile );
	
	SetFilePointer( hFile, 0, 0, FILE_END );

	return hFile;
}


BOOL spCElog_CloseFile()
{
	BOOL bRet;
	
	bRet = CloseHandle( spCElog_Get_Status_FileHandle() );
	///spCElog_Set_Status_FileHandle( NULL );
	
	return bRet;
}

BOOL spCElog_CloseDLFFile()
{
	BOOL bRet;
	
	bRet = CloseHandle( spCElog_Get_Status_DLFFileHandle() );
	
	return bRet;
}


BOOL spCElog_Try_Init()
{
	BOOL bRet = TRUE;
	HANDLE hTmp = NULL;
	
	SPDBG( TEXT("SPD@spCElog_Try_Init:+++ \r\n") );

	if( spCElog_StopSignal_Found() )
	{
		SPERR( TEXT("SPD@spCElog_Try_Init: StopSignal found!\r\n") );
		bRet = FALSE;
		goto EXIT_TRY_INIT;
	}

	///try init the log system
	InitializeCriticalSection( &spCElog_stat.cs );
	
	spCElog_Set_Status_Stoping( TRUE );
	
	///init parameter	
	spCElog_Set_Status_FileHandle( NULL );
	spCElog_Set_Status_DLFFileHandle( NULL );
	spCElog_Set_Status_ExtraInfo_Enable( TRUE );
	spCElog_Set_Status_ExtraInfo_TimeStamp_Enable( TRUE );
	spCElog_Set_Status_ExtraInfo_Order_Enable( TRUE );
	spCElog_Set_Status_ExtraInfo_Direction_Enable( TRUE );
	
	
	hTmp = CreateEvent( NULL, FALSE, FALSE, SPCELOG_BUFFERHANDLE_EVENT_NAME );
	spCElog_Set_Status_BufferEvent( hTmp );

	hTmp = CreateEvent( NULL, FALSE, FALSE, SPCELOGCTRL_EVENT );
	spCElog_Set_Status_CtrlEvent( hTmp );

	
	spCElog_Set_Status_BufferWaitTimeout( 3000 );
	
	///init buffer handle thread
	hTmp = CreateThread( (LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)spCElog_Buffer_Handle_Thread, NULL, 0, 0 );
	spCElog_Set_Status_BufferHandling( hTmp );

	///init log control handling thread
	hTmp = CreateThread( (LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)spCElog_Control_Handle_Thread, NULL, 0, 0 );
	spCElog_Set_Status_LogCtrlHandling( hTmp );	
	
	spCEbuf_Init();
	spCEbuf_Set_Status_ExtraInfo( spCElog_Get_Status_ExtraInfo() );
	
	spCElog_Set_Status_Stoping( FALSE );
	spCElog_Set_Status_USBconnected( FALSE );
	spCElog_Set_Status_FakeHostEnabled( FALSE );
	spCElog_Set_Status_SaveLogOnSD( FALSE );		///by default, save to system storage
	spCElog_Set_Status_InitMode();					///set to not-set
	spCElog_Set_Status_DebugLogEnabled( FALSE );	///by default, no debug log output
	spCElog_Set_Status_Inited( TRUE );
	__spCElogFakeHost_Init();					///init FakeHost status holder

	///span0610, set logging system default stop after init
	spCElog_Set_Status_IsStop( TRUE );
EXIT_TRY_INIT:
	
	SPDBG( TEXT("SPD@spCElog_Try_Init:--- \r\n") );
	return bRet;
}

BOOL spCElog_Try_Stop()
{
	BOOL bRet = TRUE;
	
	///brake input
	spCElog_Set_Status_Stoping( TRUE );
	
	///write all buffer to file
	spCElog_Write2File( spCEbuf_Get_Status_Count() );
	
	CloseHandle( spCElog_Get_Status_BufferHandling() );
	CloseHandle( spCElog_Get_Status_BufferEvent() );
	CloseHandle( spCElog_Get_Status_FileHandle() );
	CloseHandle( spCElog_Get_Status_DLFFileHandle() );
	
	spCElog_Set_Status_Inited( FALSE );
	spCEbuf_DeInit();
	
	SPERR( TEXT("SPD@spCElog_Try_Stop: 0x%08x \r\n"), bRet );
	return bRet;
}

BOOL spCElog_Check_WriteFileHandler()
{
	BOOL bRet = FALSE;
	
	if( NULL == spCElog_Get_Status_FileHandle() )
	{
		spCElog_CreateFile();
		bRet = TRUE;
	}
		
	if( NULL == spCElog_Get_Status_DLFFileHandle() )
	{
		spCElog_CreateDLFFile();
		bRet = TRUE;
	}
	
	return bRet;
}

BOOL spCElog_Write1Buffer2File()
{
	BOOL bRet = TRUE;
	PUCHAR pChar = NULL;
	DWORD ppdata = 0;
	DWORD dwSiz = 0;
	DWORD dwWriten = 0;
	
	///check if file handle created. create it if we still don't have it
	spCElog_Check_WriteFileHandler();
	
	///if( TRUE == spCEbuf_GetBuf( pChar, &dwSiz ) )
	if( TRUE == spCEbuf_GetBuf( &ppdata, &dwSiz ) )
	{
		pChar = (PUCHAR)ppdata;
		
		if( spCElog_Is_DebugLogOn() )
		{
			bRet = WriteFile( spCElog_Get_Status_FileHandle(), pChar, dwSiz, &dwWriten, NULL );
			SPDBG( TEXT("SPD@spCElog_Write1Buffer2File: 0x%08x %d-%d \r\n"), pChar, dwSiz, dwWriten );
		}
		
		///we can still use buffer data here...
		///we need to DLF file here.
		bRet = spCElog_Write1Buffer2DLF( (PUCHAR)ppdata, dwSiz, spCElog_Get_Status_DLFFileHandle() );
		
		free( pChar );	///don't forget to release memory
	}
	
	return bRet;
}

BOOL spCElog_Write2File( DWORD dwBufNum )
{
	BOOL bRet = TRUE;
	
	while( dwBufNum > 0 )
	{
		bRet = spCElog_Write1Buffer2File();
		dwBufNum--;
	}
	
	return bRet;
}

///span0714, add for log file save and rename for next one
BOOL spCElog_Accomplish_Log_File()
{
	BOOL bRet = TRUE;
	DWORD dwCnt = 0;

	dwCnt = spCEbuf_Get_Status_Count();
	SPDBG( TEXT("SPD@spCElog_Accomplish_Log_File: buffer count = %d \r\n"), dwCnt );

	spCElog_Write2File( spCEbuf_Get_Status_Count() );

	spCElog_CloseFile();
	spCElog_CloseDLFFile();	
	
	if( spCElog_Get_Status_SaveLogOnSD() )	///save to SD card?? or ?
	{
		WCHAR SPCELOG_SDDLFFILE_NAME_N[40];		
		SYSTEMTIME st;
			
		memset(&st, 0, sizeof(st));
		GetLocalTime(&st);
       	wsprintf(SPCELOG_SDDLFFILE_NAME_N, L"\\Storage Card\\spCElog-%02d%02d%02d.dlf", st.wHour, st.wMinute, st.wSecond);	

		///rename it
		CopyFile( SPCELOG_SDDLFFILE_NAME, SPCELOG_SDDLFFILE_NAME_N, TRUE );
		DeleteFile( SPCELOG_SDDLFFILE_NAME );
	}
	else
	{

		WCHAR SPCELOG_DLFFILE_NAME_N[20];		
		SYSTEMTIME st;
			
		memset(&st, 0, sizeof(st));
		GetLocalTime(&st);
       	wsprintf(SPCELOG_DLFFILE_NAME_N, L"spCElog-%02d%02d%02d.dlf", st.wHour, st.wMinute, st.wSecond);	
		
		///rename it
		CopyFile( SPCELOG_DLFFILE_NAME, SPCELOG_DLFFILE_NAME_N, TRUE );
		DeleteFile( SPCELOG_DLFFILE_NAME );
	}
	
	
	return bRet;
}

DWORD spCElog_Buffer_Handle_Thread( LPVOID pVoid )
{
	DWORD dwRet = 0;
	DWORD dwWait = 0;
	DWORD dwCnt = 0;
	BOOL bWaitting = TRUE;
	DWORD dwTimeout = 0;
	
	SPDBG( TEXT("SPD@spCElog_Buffer_Handle_Thread:+++ \r\n") );
	
	dwTimeout = spCElog_Get_Status_BufferWaitTimeout();
	
	while( bWaitting )
	{
		dwWait = WaitForSingleObject( spCElog_Get_Status_BufferEvent(), dwTimeout );
		if( dwWait == WAIT_OBJECT_0 )
		{
			dwCnt = spCEbuf_Get_Status_Count();
			///NKDbgPrintfW(TEXT("CellBroadCastInit Start init CB setting\r\n"));
			SPDBG( TEXT("SPD@spCElog_buffer_handle_Thread:WAIT_OBJECT_0 \r\n") );
			if( dwCnt > 2 )
			{
				spCElog_Write1Buffer2File();
				///spCEbuf_DumpAll();
			}
			if( dwCnt > 5 )
			{
				spCElog_Write1Buffer2File();
				///spCEbuf_DumpAll();
			}
			
			if( spCElog_Get_Status_IsStop() || spCElog_Get_Status_Stoping() || spCElog_StopSignal_Found() )
			{	///we got stop command
				SPERR( TEXT("SPD@spCElog_Buffer_Handle_Thread: log system is stop !!!\r\n") );
				spCElog_Accomplish_Log_File();
				dwTimeout = INFINITE;	///no longer timeout unless a new log file start.
			}
			else
			{
				dwTimeout = spCElog_Get_Status_BufferWaitTimeout();
			}

		}
		else
		if( dwWait == WAIT_TIMEOUT )
		{
			HANDLE hTmp = NULL;

			if( spCElog_Get_Status_IsStop() || spCElog_Get_Status_Stoping() || spCElog_StopSignal_Found() )
			{	///we got stop command
				SPERR( TEXT("SPD@spCElog_Buffer_Handle_Thread: log system is stop !!!\r\n") );
				spCElog_Accomplish_Log_File();
				dwTimeout = INFINITE;	///no longer timeout unless a new log file start.
			}
			else
			{	///we could do some writing while not busy
				dwCnt = spCEbuf_Get_Status_Count();
				SPDBG( TEXT("SPD@spCElog_buffer_handle_Thread:WAIT_TIMEOUT buffer count = %d \r\n"), dwCnt );
			
				///spCEbuf_DumpAll();

				spCElog_Write2File( spCEbuf_Get_Status_Count() );
			
				spCElog_CloseFile();
				spCElog_CloseDLFFile();
			
				hTmp = spCElog_CreateFile();
				hTmp = spCElog_CreateDLFFile();

				dwTimeout = spCElog_Get_Status_BufferWaitTimeout();
			}
		}
		else
			;
	}	///while
	
	spCElog_Try_Stop();
	///spCElog_CloseFile();
	
	///translate to txt file
	///MakeLog2txt();
	
	return dwRet;
}


DWORD spCElog_Get_EventCmd()
{	
	DWORD dwRet =0;
	
	dwRet = GetEventData( spCElog_Get_Status_CtrlEvent() );
	SPDBG( TEXT("SPD@spCElog_Get_EventCmd: 0x%08x \r\n"), dwRet );
	
	return dwRet;
}

BOOL spCElog_Set_EventRsp( DWORD dwRsp )
{
	BOOL bRet;
	
	bRet = SetEventData( spCElog_Get_Status_CtrlEvent(), dwRsp );
	SPDBG( TEXT("SPD@spCElog_Set_EventRsp: 0x%08x \r\n"), dwRsp );
	
	return bRet;
}

BOOL spCElog_Set_EventCmd( DWORD dwCmd )
{
	BOOL bRet;
	
	bRet = SetEventData( spCElog_Get_Status_CtrlEvent(), dwCmd );
	SetEvent( spCElog_Get_Status_CtrlEvent() );
	SPDBG( TEXT("SPD@spCElog_Set_EventCmd: 0x%08x \r\n"), dwCmd );
	return bRet;
}

BOOL spCElog_Is_SpyMode()
{
	BOOL bRet;
	DWORD dwRet;
	
	dwRet = spCElog_Get_Status_LogMode();
	if( dwRet == SPCELOG_LOGMODE_SPY )
		bRet = TRUE;
	else
		bRet = FALSE;
	
	SPDBG( TEXT("SPD@spCElog_Is_SpyMode: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Is_StandaloneMode()
{
	BOOL bRet;
	DWORD dwRet;
	
	dwRet = spCElog_Get_Status_LogMode();
	if( dwRet == SPCELOG_LOGMODE_STANDALONE )
		bRet = TRUE;
	else
		bRet = FALSE;
	
	SPDBG( TEXT("SPD@spCElog_Is_StandaloneMode: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

BOOL spCElog_Is_DebugLogOn()
{
	BOOL bRet;

	bRet = spCElog_Get_Status_DebugLogEnabled();

	SPDBG( TEXT("SPD@spCElog_Is_DebugLogOn: %s \r\n"), bRet?TEXT("TRUE"):TEXT("FALSE") );
	return bRet;
}

DWORD spCElog_Control_Handle_Thread( LPVOID pVoid )
{
	DWORD 	dwRet = 0;
	DWORD 	dwWait = 0;
	DWORD 	dwCnt = 0;
	BOOL 	bWaitting = TRUE;
	DWORD 	dwWaitTimeout = 0;
	
	SPDBG( TEXT("SPD@spCElog_Control_Handle_Thread:+++ \r\n") );
	
	dwWaitTimeout = spCElog_Get_Status_CtrlWaitTimeout();
	
	while( bWaitting )
	{
		dwWait = WaitForSingleObject( spCElog_Get_Status_CtrlEvent(), dwWaitTimeout );
		if( dwWait == WAIT_OBJECT_0 )
		{	
			DWORD dwCmd = 0;
			SPDBG( TEXT("SPD@spCElog_Control_Handle_Thread:WAIT_OBJECT_0 \r\n") );
			
			dwCmd = spCElog_Get_EventCmd();
			
			switch( dwCmd )
			{
				case SPCELOGCTRL_CTRLCMD_TIMER_RESET:
					SPDBG( TEXT("SPD@spCElog_Control_Handle_Thread: reset timer! \r\n") );
					break;
					
				case SPCELOGCTRL_CTRLCMD_GETSTATUS:
					if( spCElog_Get_Status_IsStop() )
					{	///stop
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_LOG_STOP );
					}
					else
					{	///running
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_LOG_RUNNING );
					}
					break;
				
				case SPCELOGCTRL_CTRLCMD_HOSTSINIT:
					if( !spCElog_Get_Status_IsStop() && ( HOST_CMD_TABLE_SIZE < __spCElogFakeHost_Get_StatusNum() ) )
					{
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_OK );
					}
					else
					{
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_FAIL );
					}
					break;
				
				case SPCELOGCTRL_CTRLCMD_GETCURRPKGNUM:
					if( !spCElog_Get_Status_IsStop() )
					{
						DWORD dwCurPkgCount = 0;
						///dwCurPkgCount = spCEbuf_Get_Status_Count();
						dwCurPkgCount = spCEbuf_Get_Status_IdCount();
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_VALUE_BASE | (dwCurPkgCount&0x3FFFFFFF) );	
					}
					else
					{
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_FAIL );
					}
					break;
				
				case SPCELOGCTRL_CTRLCMD_LOG_START:
					if( spCElog_Get_Status_IsStop() )
					{	///stop
						spCElog_Set_Status_IsStop( FALSE );
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_OK );
						
						if( spCElog_Is_StandaloneMode() )
						{
							spCElog_Set_Status_FakeHostEnabled( TRUE );
							__spCElogFakeHost_SetupNextCmd();	///prepare cmd first
							///fake USB in
							ResetEvent( spCElog_Get_Status_SIOUSBconnectEventHandler() );
							SetEvent( spCElog_Get_Status_SIOUSBconnectEventHandler() );
						}
					}
					else
					{	///running
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_FAIL );
					}
					break;
					
				case SPCELOGCTRL_CTRLCMD_LOG_STOP:
					if( spCElog_Get_Status_IsStop() )
					{	///stop
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_FAIL );
					}
					else
					{	///running
						spCElog_Set_Status_IsStop( TRUE );
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_OK );
						
						///fake USB out
						if( spCElog_Is_StandaloneMode() )
						{
							ResetEvent( spCElog_Get_Status_SIOUSBdisconnectEventHandler() );
							SetEvent( spCElog_Get_Status_SIOUSBdisconnectEventHandler() );
							spCElog_Set_Status_FakeHostEnabled( FALSE );
							__spCElogFakeHost_Reset_StatusNum();	///span0714, add for reset
						}
					}
					break;

				case SPCELOGCTRL_CTRLCMD_LOG_ON_SD:
					if( spCElog_Get_Status_IsStop() )
					{	///stop
						spCElog_Set_Status_SaveLogOnSD( TRUE );
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_OK );
					}
					else
					{	///running
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_FAIL );
						SPERR( TEXT("SPD@spCElog_Control_Handle_Thread: can not change log location in running time!!! \r\n") );
					}
					break;

				case SPCELOGCTRL_CTRLCMD_LOG_ON_DEV:
					if( spCElog_Get_Status_IsStop() )
					{	///stop
						spCElog_Set_Status_SaveLogOnSD( FALSE );
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_OK );
					}
					else
					{	///running
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_FAIL );
						SPERR( TEXT("SPD@spCElog_Control_Handle_Thread: can not change log location in running time!!! \r\n") );
					}
					break;
					
				case SPCELOGCTRL_CTRLCMD_LOG_SPY:
					if( spCElog_Get_Status_IsStop() )
					{	///stop
						spCElog_Set_Status_SpyMode();
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_OK );
					}
					else
					{	///running
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_FAIL );
						SPERR( TEXT("SPD@spCElog_Control_Handle_Thread: can not change mode in running time!!! \r\n") );
					}
					break;

				case SPCELOGCTRL_CTRLCMD_LOG_STANDALONE:
					if( spCElog_Get_Status_IsStop() )
					{	///stop
						spCElog_Set_Status_StandaloneMode();
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_OK );
					}
					else
					{	///running
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_FAIL );
						SPERR( TEXT("SPD@spCElog_Control_Handle_Thread: can not change mode in running time!!! \r\n") );
					}
					break;

					case SPCELOGCTRL_CTRLCMD_LOG_DEBUGLOG:
					if( spCElog_Get_Status_IsStop() )
					{	///stop
						spCElog_Set_Status_DebugLogEnabled( TRUE );
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_OK );
					}
					else
					{	///running
						spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_FAIL );
						SPERR( TEXT("SPD@spCElog_Control_Handle_Thread: can not change debug mode in running time!!! \r\n") );
					}
					break;

				default:
					spCElog_Set_EventRsp( SPCELOGCTRL_CTRLRSP_NOT_SUPPORT );
					SPERR( TEXT("SPD@spCElog_Control_Handle_Thread: unkonw event command !!! 0x%08x \r\n"), dwCmd );
			}	///switch			
		}
		else
		if( dwWait == WAIT_TIMEOUT )
		{
			SPDBG( TEXT("SPD@spCElog_Control_Handle_Thread:  timeout %d !!\r\n"), dwWaitTimeout );
			if( !spCElog_Get_Status_IsStop() && !spCElog_Get_Status_USBconnected() && spCElog_Is_StandaloneMode() )
			{	///handle fake QXDM host here. command out and wait for response. 

				///if( (spCElog_Get_Status_LastCmdTime() <= spCElog_Get_Status_LastRspTime()) || (1 == __spCElogFakeHost_Get_StatusNum()) )
				///if( !spCElog_WaitForRsp() || __spCElogFakeHost_Get_NextCmdCont() || (1 == __spCElogFakeHost_Get_StatusNum()) )
				if( __spCElogFakeHost_Get_NextCmdRead() )
				{
					if( !spCElog_WaitForRsp() || __spCElogFakeHost_Get_NextCmdCont() )
					{
						///setup command for next one
						__spCElogFakeHost_SetupNextCmd();
					
						if( __spCElogFakeHost_Get_NextCmdCont() )	///check if new cmd has a cont. packet
							dwWaitTimeout = CTRL_HANDLE_CMD_TIMEOUT_CONT;	///it's combine Cmd. we need another Cmd soon.
						else
							dwWaitTimeout = CTRL_HANDLE_CMD_TIMEOUT;
					
						///SetEvent( spCElog_Get_Status_USBDTReventHandler() );	///trigger fake DTR
						PulseEvent( spCElog_Get_Status_USBDTReventHandler() );	///trigger fake DTR
						SPDBG( TEXT("SPD@spCElog_Control_Handle_Thread: PulseEvent !!! 0x%08x \r\n"), spCElog_Get_Status_USBDTReventHandler() );					
					}
					else
					{
						SPERR( TEXT("SPD@spCElog_Control_Handle_Thread: NO response for previous Command !!! Wait!! -%dms pass- !!\r\n"), GetTickCount() - spCElog_Get_Status_LastCmdTime() );
					}
				}
				else
				{
					SPERR( TEXT("SPD@spCElog_Control_Handle_Thread: Current command is new, need NO UPDATE!!\r\n") );
				}
				SPDBG( TEXT("SPD@spCElog_Control_Handle_Thread: spCElog_Get_Status_USBDTReventHandler 0x%08x !!\r\n"), spCElog_Get_Status_USBDTReventHandler() );
			}
			else
				SPERR( TEXT("SPD@spCElog_Control_Handle_Thread:  timeout %d Nothing !!\r\n"), dwWaitTimeout );
		}
		else
			;

		///change timeout regarding to status
		if( !spCElog_Get_Status_IsStop() && !spCElog_Get_Status_USBconnected() && spCElog_Is_StandaloneMode() )
		{
			spCElog_Set_Status_FakeHostEnabled( TRUE );
			if( dwWaitTimeout != CTRL_HANDLE_CMD_TIMEOUT_CONT )
			{
				if( HOST_CMD_TABLE_SIZE < __spCElogFakeHost_Get_StatusNum() )
					dwWaitTimeout = CTRL_HANDLE_CMD_TIMEOUT_LESS;	///we need less polling after init.
				else
					dwWaitTimeout = CTRL_HANDLE_CMD_TIMEOUT;
			}
		}
		else
		{
			dwWaitTimeout = spCElog_Get_Status_CtrlWaitTimeout();
			spCElog_Set_Status_FakeHostEnabled( FALSE );
		}
	}	///while
	
	return dwRet;
}


DWORD spCElog_input( PUCHAR pucInput, DWORD dwSize, DWORD dwDir )
{
	DWORD dwRet = SPCEL_ERR_OK;
	PUCHAR puChar = NULL;
	
	if( !spCElog_Inited() )
	{
		if( !spCElog_Try_Init() )
		{
			dwRet = SPCEL_ERR_INIT;
			SPDBG( TEXT("SPD@spCElog_input:spCElog_Try_Init fail\r\n") );
			goto END_INPUT;
		}
	}

	if( spCElog_Get_Status_IsStop() )
	{
		dwRet = SPCEL_ERR_OK;
		SPERR( TEXT("SPD@spCElog_input: log system is not enabled !!!\r\n") );
		goto END_INPUT;
	}

	if( spCElog_Get_Status_Stoping() )
	{
		dwRet = SPCEL_ERR_OK;
		SPERR( TEXT("SPD@spCElog_input: log system is stopping !!!\r\n") );
		goto END_INPUT;
	}
	
	if( NULL == pucInput)
	{
		dwRet = SPCEL_ERR_POINTER;
		SPERR( TEXT("SPD@spCElog_input: fail\r\n") );
		goto END_INPUT;
	}
	
	if( 0 == dwSize )
	{
		dwRet = SPCEL_ERR_PARAMETER;
		SPERR( TEXT("SPD@spCElog_input: fail size=0 !!! dir=%d \r\n"), dwDir );
		goto END_INPUT;
	}
	
	SPDBG( TEXT("SPD@spCElog_input: = %d \r\n"), dwSize );
	///allocate new buffer
	puChar = spCEbuf_New( dwSize );
	
	if( NULL == puChar )
	{
		dwRet = SPCEL_ERR_OOM;
		goto END_INPUT;
	}
	///copy data to new buffer
	memcpy( puChar, pucInput, dwSize );
	///add this new buffer to system
	spCEbuf_Add( puChar, dwSize, dwDir );

	if( SPCEBUF_EXTRAINFO_DIRECTION_2 == dwDir )	///store command time
		spCElog_Set_Status_LastCmdTime( GetTickCount() );
	else	
	if( SPCEBUF_EXTRAINFO_DIRECTION_1 == dwDir )	///store response time
	{
		if( (0x60 == *pucInput) && (0x00 != *(pucInput+1)) )	///it's a EVENT
		{
			spCElog_Set_Status_LastEventTime( GetTickCount() );
			__log_set_EventCmd_Reset_Timer();	///event could still reset control handler thread wait!
		}
		else
		if( (0x10 == *pucInput) && (0x00 == *(pucInput+1)) )	///it's a LOG
		{
		}
		else
		if( (0x79 == *pucInput) && (0x02 == *(pucInput+1)) )	///it's a MSG
		{
		}
		else
			spCElog_Set_Status_LastRspTime( GetTickCount() );
	}
	else
		;
	
	SetEvent( spCElog_Get_Status_BufferEvent() );
	
END_INPUT:		
	return dwRet;
}

int spCElog_output( PUCHAR pucOutput, int iSize, DWORD dwDir )
{
	int iRet = 0;
	PUCHAR pucPtr = pucOutput;
	static DWORD dwCount = 0;

	SPDBG( TEXT("SPD@spCElog_output: = 0x%08x %d cnt=%d\r\n"), pucOutput, iSize, dwCount );
	
	if( NULL != pucOutput )
	{
		DWORD dwCmdSize = 0;
		DWORD dwTmp = 0;
		PUCHAR pucCmd = NULL;
		
		dwCmdSize = __spCElogFakeHost_Get_NextCmdBufSize();
		pucCmd = __spCElogFakeHost_Get_NextCmdBuffer();
		if( (DWORD)abs(iSize) >= dwCmdSize )
		{
			for( dwTmp = 0; dwTmp<dwCmdSize; dwTmp++ )
			{
				*pucPtr = *pucCmd;
				pucPtr++;
				pucCmd++;
			}
			iRet = (int)dwCmdSize;
			__spCElogFakeHost_Set_NextCmdRead();	///index this cmd has been read from Host and send
			SPDBG( TEXT("SPD@spCElog_output: = 0x%08x %d size=%d\r\n"), __spCElogFakeHost_Get_NextCmdBuffer(), dwTmp, iRet );
		}
		else
			SPERR( TEXT("SPD@spCElog_output: data over size !!! = %d > %d \r\n"), dwCmdSize, iSize );

		dwCount++;
	}

	
	return iRet;
}


BOOL __log_get_WaitForRsp()
{
	return spCElog_WaitForRsp();
}

BOOL __log_get_WaitForCmd()
{
	return spCElog_WaitForCmd();
}

BOOL __log_get_FakeHostEnabled()
{
	return spCElog_Get_Status_FakeHostEnabled();
}

BOOL __log_set_USB_connected( BOOL bConnect )
{
	return spCElog_Set_Status_USBconnected( bConnect );
}

HANDLE __log_set_USB_DTR_EventHandler( HANDLE hDTR )
{
	return spCElog_Set_Status_USBDTReventHandler( hDTR );
}

HANDLE __log_set_USB_Connect_EventHandler( HANDLE hDTR )
{
	return spCElog_Set_Status_SIOUSBconnectEventHandler( hDTR );
}

HANDLE __log_set_USB_Disconnect_EventHandler( HANDLE hDTR )
{
	return spCElog_Set_Status_SIOUSBdisconnectEventHandler( hDTR );
}

void __log_set_EventCmd_Reset_Timer()
{
	BOOL bRet;
	
	bRet = spCElog_Set_EventCmd( SPCELOGCTRL_CTRLCMD_TIMER_RESET );
}

DWORD __log_inp( PUCHAR pucInput, DWORD dwSize )
{
	return spCElog_input( pucInput, dwSize, SPCEBUF_EXTRAINFO_DIRECTION_NO );
}

DWORD __log_inp1( PUCHAR pucInput, DWORD dwSize )
{
	return spCElog_input( pucInput, dwSize, SPCEBUF_EXTRAINFO_DIRECTION_1 );
}

DWORD __log_inp2( PUCHAR pucInput, DWORD dwSize )
{
	return spCElog_input( pucInput, dwSize, SPCEBUF_EXTRAINFO_DIRECTION_2 );
}

DWORD __log_inp3( PUCHAR pucInput, DWORD dwSize )
{
	return spCElog_input( pucInput, dwSize, SPCEBUF_EXTRAINFO_DIRECTION_3 );
}

int __log_out( PUCHAR pucOutput, int iSize )
{
	return spCElog_output( pucOutput, iSize, SPCEBUF_EXTRAINFO_DIRECTION_NO );
}

BOOL __log_init()
{
	BOOL bRet;
	
	bRet = spCElog_Try_Init();
	
	return bRet;
}


BOOL __log_close()
{
	BOOL bRet;
	
	bRet = spCElog_Try_Stop();
	
	return bRet;
}
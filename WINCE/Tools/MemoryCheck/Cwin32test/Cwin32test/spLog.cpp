// Cwin32test.cpp : Defines the entry point for the application.
//

#include <stdafx.h>
#include <windows.h>
#include "spLog.h"


BOOL spLog::Init()
{
	BOOL bRet = FALSE;
	
	bRet = PriInit();
	
	InitializeCriticalSection( &m_csLogAccess );

	m_dwLogStatus |= SPLOG_STATUS_FLAG_INITED;
	return bRet;
}

BOOL spLog::DeInit()
{
	BOOL bRet = FALSE;
	
	DeleteCriticalSection( &m_csLogAccess );

	return bRet;
}

BOOL spLog::PriInit()
{
	BOOL bRet = FALSE;
	
	m_hMainLogFilehandle = INVALID_HANDLE_VALUE;
	m_dwMainLogCharNum = 0;
	m_dwLogStatus = 0;
	m_dwLogStatus |= SPLOG_STATUS_FLAG_SAMEFILE;	///use the same file by default
	///m_csLogAccess = 0;
	
	return bRet;
}


BOOL spLog::LogEnable()
{ 
	BOOL bRet = TRUE;
	
	if( m_dwLogStatus&SPLOG_STATUS_FLAG_INITED )
	{
		if( 0 == (m_dwLogStatus & SPLOG_STATUS_FLAG_LOGREADYE) )
		{
			if( LogPrepare() ) {
				m_dwLogStatus |= SPLOG_STATUS_FLAG_ENABLE;
			}
		}
		else
		{
			m_dwLogStatus |= SPLOG_STATUS_FLAG_ENABLE;
		}
	}
	else
	{
		bRet = FALSE;
	}
	
	return bRet;
}

BOOL spLog::LogDisable()
{
	BOOL bRet = TRUE;

	m_dwLogStatus &= ~SPLOG_STATUS_FLAG_ENABLE;
	
	return bRet;
}

BOOL spLog::LogDone( void )
{
	BOOL bRet = FALSE;

	
	if( IsLogReady() )
	{
		///TakeCS();
		
		///WriteTitle();
		WriteCurrentTime();
		WriteSeparator();
		WriteSeparator();
		bRet = CloseHandle( m_hMainLogFilehandle );
		m_hMainLogFilehandle = INVALID_HANDLE_VALUE;
		m_dwLogStatus &= ~SPLOG_STATUS_FLAG_LOGREADYE;
		m_dwLogStatus &= ~SPLOG_STATUS_FLAG_ENABLE;
		
		///ReleaseCS();
	}
	
	return bRet;
}


BOOL spLog::SetupTitle( LPWSTR lpszTitleString )
{
	BOOL bRet = TRUE;
	
	wcscpy( m_szTitle, lpszTitleString );
	m_dwLogStatus |= SPLOG_STATUS_FLAG_TITLE_OK;
	
	return bRet;
}


BOOL spLog::SetupFileName( LPWSTR lpszFileNameString )
{
	BOOL bRet = TRUE;
	
	wcscpy( m_szPathName, lpszFileNameString );
	m_dwLogStatus |= SPLOG_STATUS_FLAG_NAME_OK;
	
	return bRet;
}


BOOL spLog::SetupInformation( LPWSTR lpszInfoString )
{
	BOOL bRet = TRUE;
	
	wcscpy( m_szInformation, lpszInfoString );
	m_dwLogStatus |= SPLOG_STATUS_FLAG_INFO_OK;
	
	return bRet;
}


BOOL spLog::SetupSeparator( LPWSTR lpszSepString )
{
	BOOL bRet = TRUE;
	
	wcscpy( m_szSeparator, lpszSepString );
	m_dwLogStatus |= SPLOG_STATUS_FLAG_SEPARA_OK;
	
	return bRet;
}

void spLog::WriteTitle( void )
{
	WriteLog( TEXT("!%s\r\n"), m_szTitle );
}

void spLog::WriteInformation( void )
{
	WriteLog( TEXT("!%s\r\n"), m_szInformation );
}

void spLog::WriteSeparator( void )
{
	WriteLog( TEXT("!%s\r\n"), m_szSeparator );
}

void spLog::WriteCurrentTime( void )
{

	SYSTEMTIME st;
	
	GetLocalTime( &st );
	
	WriteLog( 
		TEXT("Current Time Y/M/D H/M/S=%d/%d/%d %d/%d/%d\r\n"),
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond
	);
}


BOOL spLog::CheckLogFileParameter(void)
{
	BOOL bRet = TRUE;
	
	if( 0 == (m_dwLogStatus & SPLOG_STATUS_FLAG_INFO_OK) )
	{
		wsprintf( m_szTempString, TEXT("This is a default Information of this Log file.\0") );
		SetupInformation( m_szTempString );
	}

	if( 0 == (m_dwLogStatus & SPLOG_STATUS_FLAG_SEPARA_OK) )
	{
		wsprintf( m_szTempString, TEXT("------------------------------------------------------------\0") );
		SetupSeparator( m_szTempString );
	}

	if( 0 == (m_dwLogStatus & SPLOG_STATUS_FLAG_NAME_OK) )
	{
		wsprintf( m_szTempString, TEXT("spDefault.log\0") );
		SetupFileName( m_szTempString );
	}

	if( 0 == (m_dwLogStatus & SPLOG_STATUS_FLAG_TITLE_OK) )
	{
		wsprintf( m_szTempString, TEXT("This is a default Title of this Log file.\0") );
		SetupTitle( m_szTempString );
	}

	
	return bRet;
}


BOOL spLog::LogPrepare(void)
{
	BOOL bRet = FALSE;
	
	if( 0 == (m_dwLogStatus & SPLOG_STATUS_FLAG_LOGREADYE) )
	{
		///check log file parameter
		bRet = CheckLogFileParameter();
		
		if( 0 == (m_dwLogStatus & SPLOG_STATUS_FLAG_SAMEFILE) )
		{
		}
		else
		{
			if( m_hMainLogFilehandle == INVALID_HANDLE_VALUE )
			{
				m_hMainLogFilehandle = CreateFile( m_szPathName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
				if( ERROR_ALREADY_EXISTS == GetLastError() )
				{	/// move to the end of file if the file already exist
					SetFilePointer( m_hMainLogFilehandle, 0, NULL, FILE_END );
				}
				
				if( m_hMainLogFilehandle != INVALID_HANDLE_VALUE )
				{
					///write the Log header
					bRet = WriteLogFileHeader();
				}
				else
				{
					bRet = FALSE;
				}
			}
		}

		if( bRet )
			m_dwLogStatus |= SPLOG_STATUS_FLAG_LOGREADYE;
	}

	return bRet;
}


BOOL spLog::WriteLogFileHeader(void)
{
	BOOL bRet = TRUE;

	if( INVALID_HANDLE_VALUE != m_hMainLogFilehandle )
	{
		m_dwLogStatus |= SPLOG_STATUS_FLAG_LOGREADYE;
		m_dwLogStatus |= SPLOG_STATUS_FLAG_ENABLE;	
		WriteSeparator();
		WriteTitle();
		WriteInformation();
		WriteCurrentTime();
		WriteSeparator();
		m_dwLogStatus &= ~SPLOG_STATUS_FLAG_LOGREADYE;
		m_dwLogStatus &= ~SPLOG_STATUS_FLAG_ENABLE;							
	}
	else
	{
		bRet = FALSE;
	}
	
	return bRet;
}

	
DWORD spLog::WriteLog( LPWSTR lpszWriteString, ... )
{
	DWORD dwRet = 0;
	BOOL bRet = FALSE;

	va_list ap;
	int count = 0;
	wchar_t buf[512];
	DWORD num = 0;
	int i;

	if( IsLogReady() && IsLogEnable() )
	{
		TakeCS();
		
		va_start( ap, lpszWriteString );
		count = wvsprintf( buf, lpszWriteString, ap );
		va_end( ap );
		
		for ( i = 0; i<count; i++ )
			((UCHAR *)buf)[i] = (UCHAR)buf[i];

		bRet = WriteFile( m_hMainLogFilehandle, buf, count, &num, 0);
		
		ReleaseCS();
	}
	else
		bRet = FALSE;
		
	if( bRet )
	{
		dwRet = num;
		m_dwMainLogCharNum += dwRet;
	}
	else
		dwRet = 0;
	
	return dwRet;
}
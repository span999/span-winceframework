// TestClass.cpp : implementation of Test class.
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/// WARNING !!! you should not change any code in this file !!!
/// for modification, please create your own class base on this.
/// or contact with development center for detail discussion. Thanks!
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "LogClass.h"


/////////////////////////////////////////////////////////////////////////////
/// SLogClass
/*
SLogClass__________________________________________________________________()
*/
BOOL SLogClass::Init()
{
	BOOL bRet = TRUE;
	
	m_hLogFile = INVALID_HANDLE_VALUE;
	m_bLogEnable = FALSE;
	///m_csLogAccess = 0;
	InitializeCriticalSection(&m_csLogAccess);
	
	return bRet;
}

BOOL SLogClass::DeInit()
{
	BOOL bRet = TRUE;
	
	DeleteCriticalSection(&m_csLogAccess);
	
	return bRet;
}

BOOL SLogClass::SetTitle( LPWSTR lpszTitleString )
{
	BOOL bRet = TRUE;
	
	///wcscpy( szTestTitle, lpszTitleString );
	_tcscpy( szTestTitle, lpszTitleString );
	
	return bRet;
}

BOOL SLogClass::SetInfo( LPWSTR lpszInfoString )
{
	BOOL bRet = TRUE;

	///wcscpy( szTestInfo, lpszInfoString );
	_tcscpy( szTestInfo, lpszInfoString );

	return bRet;
}

BOOL SLogClass::SetLogName( LPWSTR lpszLogNameString )
{
	BOOL bRet = TRUE;
	
	wcscpy( szLogName, lpszLogNameString );
	
	return bRet;
}

BOOL SLogClass::SetSeparator( LPWSTR lpszSeparatorString )
{
	BOOL bRet = TRUE;
	
	wcscpy( szSeparator, lpszSeparatorString );
	
	return bRet;
}

BOOL SLogClass::EnableLog( BOOL bSet )
{
	BOOL bRet = TRUE;
	
	TakeCS();
	m_bLogEnable = bSet;
	ReleaseCS();

	return bRet;
}

BOOL SLogClass::IsLogReady()
{
	BOOL bRet = FALSE;
	DWORD dwWrited = 0;
	
	///TakeCS();
	if( m_bLogEnable )
	{
		if( INVALID_HANDLE_VALUE == m_hLogFile )
		{
			m_hLogFile = CreateFile( szLogName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
			if( ERROR_ALREADY_EXISTS == GetLastError() )
			{	/// move to the end of file
				SetFilePointer( m_hLogFile, 0, NULL, FILE_END );
			}
				
			if( INVALID_HANDLE_VALUE != m_hLogFile )
			{
				///WriteFile( m_hLogFile, szTestTitle, wcslen(szTestTitle), &dwWrited, 0);
				///WriteFile( m_hLogFile, szTestInfo, wcslen(szTestInfo), &dwWrited, 0);
				///WriteFile( m_hLogFile, szTestTitle, _tcslen(szTestTitle), &dwWrited, 0);
				///WriteFile( m_hLogFile, szTestInfo, _tcslen(szTestInfo), &dwWrited, 0);
				privateWriteLog( _T("%s"), szSeparator );
				privateWriteLog( _T("%s"), szTestTitle );
				privateWriteLog( _T("%s"), szTestInfo );
				LogCurrentTime();
				privateWriteLog( _T("%s"), szSeparator );
			}		
		}	
		if( INVALID_HANDLE_VALUE != m_hLogFile )
			bRet = TRUE;
	}		
	///ReleaseCS();

	return bRet;
}

BOOL SLogClass::WriteLog( BOOL bON, LPWSTR lpszWriteString, ... )
{
	BOOL bRet = TRUE;
	va_list ap;
	int count = 0;
	wchar_t buf[512];
	DWORD num;
	int i;
	
	if( !bON )
		return FALSE;

	TakeCS();
	if( IsLogReady() )
	{
		va_start( ap, lpszWriteString );
		count = wvsprintf( buf, lpszWriteString, ap );
		va_end( ap );
		for ( i = 0; i<count; i++ )
			((UCHAR *)buf)[i] = (UCHAR)buf[i];
		WriteFile( m_hLogFile, buf, count, &num, 0);
	}
	else
		bRet = FALSE;
	ReleaseCS();
	
	return bRet;
}

BOOL SLogClass::WriteLog( LPWSTR lpszWriteString, ... )
{
	BOOL bRet = TRUE;
	va_list ap;
	int count = 0;
	wchar_t buf[512];
	DWORD num;
	int i;

	TakeCS();
	if( IsLogReady() )
	{
		va_start( ap, lpszWriteString );
		count = wvsprintf( buf, lpszWriteString, ap );
		va_end( ap );
		for ( i = 0; i<count; i++ )
			((UCHAR *)buf)[i] = (UCHAR)buf[i];
		WriteFile( m_hLogFile, buf, count, &num, 0);
	}
	else
		bRet = FALSE;
	ReleaseCS();
	
	return bRet;
}

BOOL SLogClass::LogCurrentTime()
{
	BOOL bRet = TRUE;
	SYSTEMTIME st;
	
	GetLocalTime( &st );
	privateWriteLog( 
		_T("%d/%d/%d %d/%d/%d \r\n"),
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond
		);

	return bRet;
}

BOOL SLogClass::privateWriteLog( LPWSTR lpszWriteString, ... )
{
	BOOL bRet = TRUE;
	va_list ap;
	int count = 0;
	wchar_t buf[512];
	DWORD num;
	int i;

	va_start( ap, lpszWriteString );
	count = wvsprintf( buf, lpszWriteString, ap );
	va_end( ap );
	for ( i = 0; i<count; i++ )
		((UCHAR *)buf)[i] = (UCHAR)buf[i];
	WriteFile( m_hLogFile, buf, count, &num, 0);
	
	return bRet;
}

BOOL SLogClass::SaveLog()
{
	BOOL bRet = TRUE;

	TakeCS();
	if( INVALID_HANDLE_VALUE != m_hLogFile )
	{
		LogCurrentTime();
		privateWriteLog( _T("%s"), szSeparator );
		CloseHandle( m_hLogFile );
		m_hLogFile = INVALID_HANDLE_VALUE;
	}
	ReleaseCS();
	
	return bRet;
}
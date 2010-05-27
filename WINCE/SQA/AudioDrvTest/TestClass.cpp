// TestClass.cpp : implementation of Test class.
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/// WARNING !!! you should not change any code in this file !!!
/// for modification, please create your own class base on this.
/// or contact with development center for detail discussion. Thanks!
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "TestClass.h"


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


/////////////////////////////////////////////////////////////////////////////
/// SShowMsgClass
/*
SShowMsgClass______________________________________________________________()
*/
BOOL SShowMsgClass::Init()
{
	BOOL bRet = TRUE;

	m_dwDebugLevel = 0;	
	m_hWndFrameWindow = NULL;
	m_hdc = NULL;
	m_dwLineIndex = 0;
	m_dwLineMax = 0;
	m_dwLineHeight = DEFAULT_SHOWLINE_HEIGHT;
	
	InitializeCriticalSection(&m_csShowLine);
		
	return bRet;
}

BOOL SShowMsgClass::Init( HWND hMainWnd, RECT rtSetArea )
{
	BOOL bRet = TRUE;
	
	Init();
	
	SetFrameWindowHwnd( hMainWnd );
	SetShowAreaRect( rtSetArea );
	
	return bRet;
}

BOOL SShowMsgClass::DeInit()
{
	BOOL bRet = TRUE;
	
	DeleteCriticalSection(&m_csShowLine);
	ReleaseDC( m_hWndFrameWindow, m_hdc );
	
	return bRet;
}	

BOOL SShowMsgClass::SetFrameWindowHwnd( HWND hMainWnd )
{
	BOOL bRet = TRUE;
	m_hWndFrameWindow = hMainWnd;
	return bRet;
}

BOOL SShowMsgClass::SetShowAreaRect( RECT rtSetArea )
{
	BOOL bRet = TRUE;
	TakeCS();
	m_rtShowArea = rtSetArea;
	m_dwLineMax = (DWORD)(abs(m_rtShowArea.top - m_rtShowArea.bottom)/m_dwLineHeight);
	m_dwLineIndex = 1;		//set to first line
	ReleaseCS();
	return bRet;
}

RECT SShowMsgClass::GetCurrentLineRect()
{
	RECT rtRet;
	DWORD dwTempIndex;
	
	TakeCS();
	dwTempIndex = m_dwLineIndex;
	m_dwLineIndex++;
	if( m_dwLineIndex > m_dwLineMax )
		m_dwLineIndex = 1;
	ReleaseCS();
	
	rtRet = m_rtShowArea;
	if( m_rtShowArea.top > m_rtShowArea.bottom )
	{	
		rtRet.top = rtRet.bottom + (m_dwLineHeight*dwTempIndex);
		rtRet.bottom = rtRet.top - m_dwLineHeight;
	}	
	else
	{	
		rtRet.bottom = rtRet.top + (m_dwLineHeight*dwTempIndex);
		rtRet.top = rtRet.bottom - m_dwLineHeight;
	}	
	
	return rtRet;
}

///BOOL SShowMsgClass::MsgOut( BOOL bON, LPWSTR lpszMsgstring, ... )
///{
///	BOOL bRet = TRUE;
///	RECT rtTemp;
///	
///	va_list ap;
///	int count = 0;
///	wchar_t buf[512];
///
///	int i;
///
///	if( PrepareHdc() && bON )
///	{
///		va_start( ap, lpszMsgstring );
///		count = wvsprintf( buf, lpszMsgstring, ap );
///		va_end( ap );
///		for ( i = 0; i<count; i++ )
///			((wchar_t *)buf)[i] = (wchar_t)buf[i];
///	}
///	else
///		return FALSE;
///
///	if( m_dwLineIndex == 1 )
///		Rectangle( m_hdc, m_rtShowArea.left, m_rtShowArea.top, m_rtShowArea.right, m_rtShowArea.bottom );	
///
///	rtTemp = GetCurrentLineRect();
///
///	///DrawText( m_hdc, lpszMsgstring, -1, &rtTemp, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
///	DrawText( m_hdc, buf, -1, &rtTemp, DT_SINGLELINE | DT_VCENTER );
///	
///	return bRet;
///}

BOOL SShowMsgClass::MsgOut( DWORD dwFlag, LPWSTR lpszMsgstring, ... )
{
	BOOL bRet = TRUE;
	RECT rtTemp;
	
	va_list ap;
	int count = 0;
	wchar_t buf[512];

	int i;

	if( PrepareHdc() && ((m_dwDebugLevel&dwFlag) != 0) )
	{
		va_start( ap, lpszMsgstring );
		count = wvsprintf( buf, lpszMsgstring, ap );
		va_end( ap );
		for ( i = 0; i<count; i++ )
			((wchar_t *)buf)[i] = (wchar_t)buf[i];
	}
	else
		return FALSE;

	if( m_dwLineIndex == 1 )
		Rectangle( m_hdc, m_rtShowArea.left, m_rtShowArea.top, m_rtShowArea.right, m_rtShowArea.bottom );	

	rtTemp = GetCurrentLineRect();

	///DrawText( m_hdc, lpszMsgstring, -1, &rtTemp, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
	DrawText( m_hdc, buf, -1, &rtTemp, DT_SINGLELINE | DT_VCENTER );
	
	return bRet;
}

BOOL SShowMsgClass::MsgOut( LPWSTR lpszMsgstring, ... )
{
	BOOL bRet = TRUE;
	RECT rtTemp;
	
	va_list ap;
	int count = 0;
	wchar_t buf[512];

	int i;

	if( PrepareHdc() )
	{
		va_start( ap, lpszMsgstring );
		count = wvsprintf( buf, lpszMsgstring, ap );
		va_end( ap );
		for ( i = 0; i<count; i++ )
			((wchar_t *)buf)[i] = (wchar_t)buf[i];
	}
	else
		return FALSE;

	if( m_dwLineIndex == 1 )
		Rectangle( m_hdc, m_rtShowArea.left, m_rtShowArea.top, m_rtShowArea.right, m_rtShowArea.bottom );	

	rtTemp = GetCurrentLineRect();

	///DrawText( m_hdc, lpszMsgstring, -1, &rtTemp, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
	DrawText( m_hdc, buf, -1, &rtTemp, DT_SINGLELINE | DT_VCENTER );
	
	return bRet;
}

BOOL SShowMsgClass::PrepareHdc()
{
	BOOL bRet = FALSE;
	
	if( m_hWndFrameWindow != NULL )
	{
		if( m_hdc == NULL )
			m_hdc = GetDC( m_hWndFrameWindow );

		if( m_hdc != NULL )
			bRet = TRUE;
	}	
	
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
/// STestItemClass
/*
STestItemClass_____________________________________________________________()
*/
STestItemClass::STestItemClass( UINT uiID )
{
	Init();
	SetTestItemID( uiID );
}

void STestItemClass::SetFrameWindowHwnd( HWND hWndIn )
{
	hWndFrameWindow = hWndIn;
	if( pMsg )
		pMsg->SetFrameWindowHwnd( hWndIn );
}

void STestItemClass::SetItemWindowRect( RECT rtIn )
{
	rtItemWindow = rtIn;
	if( pMsg )
		pMsg->SetShowAreaRect( rtIn );
}

BOOL STestItemClass::SetTestItemID( UINT uiID )
{
	BOOL bRet;
	
	if( uiID != 0 )
	{	
		uiTestItemID = uiID;
		TestItemStatus = _TestItemEnable;
		bRet = TRUE;
	}
	else
		bRet = FALSE;

	return bRet;
}

void STestItemClass::Init()
{
	m_dwDebugLevel = 0;
	uiTestItemID = 0;
	TestItemStatus = _TestItemDisable;
	hWndFrameWindow = NULL;
	dwTestItemLoop = 0;
	hTestItemThread = NULL;
	hTestItemThreadEvent = NULL;
	bTestItemThreadRun = FALSE;
	dwTestItemStage = _TestStagePreEVT;
	dwTestItemLoopInterval = 100;
	TestItemStartTick = 0;
	TestItemStopTick = 0;
	bEnableLog = FALSE;
	pLog = NULL;
	pMsg = NULL;

	InitMsg();
	InitLog();
	
	hTestItemThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	hTestItemThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TestItemThreadStub, this, 0, NULL);
	if( NULL == hTestItemThread )
		pMsg->MsgOut( _T("STestItemClass::Init CreateThread fail\0") );
	else
		CloseHandle(hTestItemThread);
}

DWORD STestItemClass::SetDebugLevel( DWORD dwLevel )
{
	m_dwDebugLevel = dwLevel;

	if( pMsg )
		pMsg->SetDebugLevel( m_dwDebugLevel );

	return m_dwDebugLevel;
}

BOOL STestItemClass::InitLog()
{
	BOOL bRet;
	
	if( NULL == pLog )
		pLog = new SLogClass();
		
	if( NULL != pLog )
		bRet = _InitLog();
	else
		bRet = FALSE;

	return bRet;
}

BOOL STestItemClass::InitMsg()
{
	BOOL bRet;
	
	if( NULL == pMsg )
		pMsg = new SShowMsgClass();

	if( NULL == pMsg )
		bRet = FALSE;
	else
		bRet = TRUE;

	return bRet;
}

BOOL STestItemClass::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Title of test item Name.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("Information of test item.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
		
	wsprintf( szTempString, _T("LogfileName.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}

DWORD STestItemClass::TestItemThreadStub( LPVOID lpParam )
{
	DWORD dwRet = 0;
	if( NULL != lpParam )
	{
		STestItemClass *pThisItem = (STestItemClass *)lpParam;
		dwRet = pThisItem->TestItemThread();
	}
	return dwRet;
}

DWORD STestItemClass::TestItemThread()
{
	DWORD dwRet = 0;
	DWORD dwWait = INFINITE;
	DWORD dwLoop = dwTestItemLoop;
	
	pMsg->MsgOut( _T("STestItemClass::TestItemThread strat %d times %d\0"), dwTestItemLoop, uiTestItemID );
	pMsg->MsgOut( _T("ItemLoop=%d ItemID%d\0"), dwTestItemLoop, uiTestItemID );
	
	while(1)
	{
		dwRet = WaitForSingleObject( hTestItemThreadEvent, dwWait );
		
		if( dwRet == WAIT_OBJECT_0 )
		{
			pMsg->MsgOut( _T("STestItemClass::TestItemThread WAIT_OBJECT_0 %d\0"), uiTestItemID );
			dwLoop = dwTestItemLoop;		
		}
		else
		if( dwRet == WAIT_TIMEOUT )
		{
			pMsg->MsgOut( _T("STestItemClass::TestItemThread WAIT_TIMEOUT %d %d\0"), dwLoop, uiTestItemID );
			///
			TestContent( dwLoop );			
		}
		else
			;
			
		if( bTestItemThreadRun )			
			dwWait = dwTestItemLoopInterval;
		else
			dwWait = INFINITE;
		
		if( dwLoop == 0 )
		{	
			Stop();
			pLog->SaveLog();
		}	
			
		dwLoop--;
	}
		
	return dwRet;
}

BOOL STestItemClass::Run()
{
	BOOL bRet;
	
	if( _TestItemActive == GetTestItemStatus() )
	{
		pMsg->MsgOut( _T("STestItemClass::Run %d\0"), uiTestItemID );			
		///ShowDebugMsg( _T("STestItemClass::Run\0") );
		SetGetTestItemStatus( _TestItemRuning );
		
		bTestItemThreadRun = TRUE;
		SetEvent( hTestItemThreadEvent );
		
		bRet = TRUE;
	}
	else
	{
		pMsg->MsgOut( _T("STestItemClass::Run status error %d\0"), uiTestItemID );			
		///pMsg->MsgOut( _T("STestItemClass::Run status error\0") );
		bRet = FALSE;
	}	
		
	return bRet;
}

BOOL STestItemClass::Stop()
{
	BOOL bRet;
	
	if( _TestItemRuning == GetTestItemStatus() )
	{
		pMsg->MsgOut( _T("STestItemClass::Stop %d\0"), uiTestItemID );			
		///ShowDebugMsg( _T("STestItemClass::Stop\0") );
		SetGetTestItemStatus( _TestItemActive );

		bTestItemThreadRun = FALSE;
		SetEvent( hTestItemThreadEvent );
				
		bRet = TRUE;
	}
	else
	{
		pMsg->MsgOut( _T("STestItemClass::Stop status error %d\0"), uiTestItemID );			
		///pMsg->MsgOut( _T("STestItemClass::Stop status error\0") );
		bRet = FALSE;
	}	
		
	return bRet;
}

void STestItemClass::TestContent( DWORD dwIndex )
{
	pMsg->MsgOut( _T("STestItemClass::TestContent %d %d\0"), dwIndex, uiTestItemID );		
}

BOOL STestItemClass::SaveLog()
{
	BOOL bRet = TRUE;

	bRet = pLog->SaveLog();
	pMsg->MsgOut( _T("STestItemClass::SaveLog %s %d\0"), (bRet)?L"OK":L"Fail", uiTestItemID );
	
	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
/// STestClass
/*
STestClass_________________________________________________________________()
*/
STestClass::STestClass()
{
	Init();
}

STestClass::STestClass( DWORD dwNumber, DWORD dwLoop )
{
	Init( dwNumber, dwLoop);
}

void STestClass::Init()
{
	m_dwDebugLevel = 0;
	TestItemEntryArraryIndex = 0;
	hWndFrameWindow = NULL;
	m_dwTestLoopNumber = 100;
	m_TestStage = _TestStagePreEVT;
	m_TestLoopInterval = 100;
	m_TestStartTick = 0;
	m_TestStopTick = 0;
	m_bEnableLog = FALSE;
	pLog = NULL;
	pMsg = NULL;
	
	InitMsg();
	InitLog();
		
	SetTestItemNumber(3);
	SetTestLoopNumber(1000);
}

void STestClass::Init( DWORD dwNumber, DWORD dwLoop )
{
	m_dwDebugLevel = 0;
	TestItemEntryArraryIndex = 0;
	hWndFrameWindow = NULL;
	m_dwTestLoopNumber = 100;
	m_TestStage = _TestStagePreEVT;
	m_TestLoopInterval = 100;
	m_TestStartTick = 0;
	m_TestStopTick = 0;
	m_bEnableLog = FALSE;
	pLog = NULL;
	pMsg = NULL;	
	
	InitMsg();
	InitLog();
	
	SetTestItemNumber(dwNumber); 
	SetTestLoopNumber(dwLoop);
}

DWORD STestClass::SetDebugLevel( DWORD dwLevel )
{
	m_dwDebugLevel = dwLevel;

	if( pMsg )
		pMsg->SetDebugLevel( m_dwDebugLevel );

	return m_dwDebugLevel;
}

void STestClass::InitAllItems()
{
	STestItemClass * pFindItem = NULL;
	RECT rtTemp = rtFrameWindow;
	RECT rtItemTemp = rtFrameWindow;
	LONG lTemp;


	if( m_dwDebugLevel != 0 )
	{	
		if( rtTemp.top > rtTemp.bottom )
			rtTemp.bottom = rtTemp.top - DEFAULT_SHOWAREA_HEIGHT;
		else
			rtTemp.top = rtTemp.bottom - DEFAULT_SHOWAREA_HEIGHT;
				
		if( pMsg )
			pMsg->SetShowAreaRect( rtTemp );
	}	

	rtTemp = rtFrameWindow;
	if( m_dwDebugLevel != 0 )	
		rtTemp.bottom = rtTemp.bottom - DEFAULT_SHOWAREA_HEIGHT;
	lTemp = (rtTemp.bottom - rtTemp.top)/TestItemEntryArraryIndex;
		
	for( UINT iLoop = 0; iLoop < TestItemEntryArraryIndex; iLoop++ )
	{
		pFindItem = TestItemEntryArrary[iLoop];
		pFindItem->SetDebugLevel( m_dwDebugLevel );
		pFindItem->SetFrameWindowHwnd( hWndFrameWindow );
		rtItemTemp.top = rtTemp.top+(lTemp*iLoop);
		rtItemTemp.bottom = rtItemTemp.top + lTemp;
		pFindItem->SetItemWindowRect( rtItemTemp );
		pFindItem->SetTestItemLoop( m_dwTestLoopNumber );
	}	
}

void STestClass::SetFrameWindowHwnd( HWND hWndIn )
{
	hWndFrameWindow = hWndIn;
	if( pMsg )
		pMsg->SetFrameWindowHwnd( hWndIn );	

	InitAllItems();
}

void STestClass::SetFrameWindowRect( RECT rtIn )
{
	RECT rtTemp;
	
	rtFrameWindow = rtIn;
	
	rtTemp = rtFrameWindow;
	
	if( rtTemp.top > rtTemp.bottom )
	{	
		rtTemp.bottom = rtTemp.top - DEFAULT_SHOWAREA_HEIGHT;
	}	
	else
	{	
		rtTemp.top = rtTemp.bottom - DEFAULT_SHOWAREA_HEIGHT;
	}
			
	if( pMsg )
		pMsg->SetShowAreaRect( rtTemp );
		
	InitAllItems();	
}

void STestClass::SetTestLoopNumber( DWORD dwLoop )
{
	STestItemClass * pFindItem = NULL;

	m_dwTestLoopNumber = dwLoop;
	
	pMsg->MsgOut( dINIT|dINFO, _T("STestClass::SetTestLoopNumber %d\0"), m_dwTestLoopNumber );
	
	for( UINT iLoop = 0; iLoop < TestItemEntryArraryIndex; iLoop++ )
	{
		pFindItem = TestItemEntryArrary[iLoop];
		pFindItem->SetTestItemLoop( m_dwTestLoopNumber );
	}	
}

void STestClass::SetTestStage( enumTestStage stage )
{
	STestItemClass * pFindItem = NULL;
	
	m_TestStage = stage;

	pMsg->MsgOut( dINIT|dINFO, _T("STestClass::SetTestStage %d\0"), m_TestStage );
	
	for( UINT iLoop = 0; iLoop < TestItemEntryArraryIndex; iLoop++ )
	{
		pFindItem = TestItemEntryArrary[iLoop];
		pFindItem->SetTestItemStage( stage );
	}		
}
	
void STestClass::SetTestLoopInterval( DWORD dwInterval )
{
	STestItemClass * pFindItem = NULL;
	
	m_TestLoopInterval = dwInterval;

	pMsg->MsgOut( dINIT|dINFO, _T("STestClass::SetTestLoopInterval %d\0"), m_TestLoopInterval );

	for( UINT iLoop = 0; iLoop < TestItemEntryArraryIndex; iLoop++ )
	{
		pFindItem = TestItemEntryArrary[iLoop];
		pFindItem->SetTestItemLoopInterval( dwInterval );
	}		
}

void STestClass::AddTestItem( UINT uiID )
{
	TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemClass( uiID );
	TestItemEntryArraryIndex++;
	pMsg->MsgOut( dINIT|dINFO, _T("STestClass::AddTestItem ID0x%08x Index=%d\0"), uiID, TestItemEntryArraryIndex );
	InitAllItems();
}

UINT STestClass::GetTestItemID( UINT uiIndex )
{
	UINT uiRet;
	
	STestItemClass *pTestItem;
	pTestItem = TestItemEntryArrary[uiIndex];
	uiRet = pTestItem->GetTestItemID();
	
	return uiRet;
}


enumTestItemStatus STestClass::GetTestItemStatus( UINT uiIndex )
{
	enumTestItemStatus Itemstatus;

	STestItemClass *pTestItem;
	pTestItem = TestItemEntryArrary[uiIndex];	
	Itemstatus = pTestItem->GetTestItemStatus();
	
	return Itemstatus;
}

enumTestItemStatus STestClass::IDGetTestItemStatus( UINT uiID )
{
	enumTestItemStatus Itemstatus;
	STestItemClass * pFindItem = NULL;
	
	pFindItem = IDFindTestItem( uiID );
	if( pFindItem != NULL )
		Itemstatus = pFindItem->GetTestItemStatus();
	
	return Itemstatus;
}


STestItemClass *STestClass::IDFindTestItem( UINT uiID )
{
	STestItemClass * pFindItem = NULL;
	BOOL bNotfound = TRUE;
	UINT iLoop = 0;
	
	do{
		pFindItem = TestItemEntryArrary[iLoop];
		if( pFindItem->GetTestItemID() == uiID )
			bNotfound = FALSE;
		
		iLoop++;
	}while( bNotfound && (iLoop <= TestItemEntryArraryIndex) );

	return (bNotfound?NULL:pFindItem);
}		

void STestClass::ActiveTestItem( UINT uiID )
{
	STestItemClass * pFindItem = NULL;

	pMsg->MsgOut( dINFO, _T("STestClass::ActiveTestItem ID0x%08x\0"), uiID );
	
	pFindItem = IDFindTestItem( uiID );
	if( pFindItem != NULL )
		pFindItem->SetTestItemActive();
}

void STestClass::DeActiveTestItem( UINT uiID )
{
	STestItemClass * pFindItem = NULL;

	pMsg->MsgOut( dINFO, _T("STestClass::DeActiveTestItem ID0x%08x\0"), uiID );
	
	pFindItem = IDFindTestItem( uiID );
	if( pFindItem != NULL )
		pFindItem->SetTestItemDeActive();

}

BOOL STestClass::InitLog()
{
	BOOL bRet;
	
	if( NULL == pLog )
		pLog = new SLogClass();
		
	if( NULL != pLog )
		bRet = _InitLog();
	else
		bRet = FALSE;

	return bRet;
}

BOOL STestClass::InitMsg()
{
	BOOL bRet;
	
	if( NULL == pMsg )
		pMsg = new SShowMsgClass();

	if( NULL == pMsg )
		bRet = FALSE;
	else
	{
		SetDebugLevel( ( dINIT | dWARN | dINFO | dDBUG | dFAIL | dCLAS ) );	
		bRet = TRUE;
	}	

	return bRet;
}

BOOL STestClass::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Title of test set Name.\r\n") );
	pMsg->MsgOut( dINIT|dINFO, szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("Information of test set.\r\n") );
	pMsg->MsgOut( dINIT|dINFO, szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( dINIT|dINFO, szTempString );
	pLog->SetSeparator( szTempString );
		
	wsprintf( szTempString, _T("LogfileName.log\0") );
	pMsg->MsgOut( dINIT|dINFO, szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}

void STestClass::Run()
{
	pMsg->MsgOut( dINFO, _T("STestClass::Run\0") );
	m_TestStartTick = GetTickCount();
	pLog->WriteLog( _T("Test set Start\r\n") );	
	RunTestItem();
}

void STestClass::Stop()
{
	pMsg->MsgOut( dINFO, _T("STestClass::Stop\0") );
	StopTestItem();
	m_TestStopTick = GetTickCount();
	pLog->WriteLog( _T("Test set Stop, total tick count =%d\r\n"), GetTotalTestTime());
}

BOOL STestClass::RunTestItem()
{
	BOOL bRet = TRUE;
	STestItemClass * pFindItem = NULL;

	pMsg->MsgOut( dINFO, _T("STestClass::RunTestItem\0") );
	
	for( UINT iLoop = 0; iLoop < TestItemEntryArraryIndex; iLoop++ )
	{
		pFindItem = TestItemEntryArrary[iLoop];
		bRet = pFindItem->Run();
	}
	
	return bRet;	
}

BOOL STestClass::RunTestItem( UINT uiID )
{
	BOOL bRet = FALSE;
	STestItemClass * pFindItem = NULL;

	pMsg->MsgOut( dINFO, _T("STestClass::RunTestItem ID-0x%08x\0"), uiID );

	pFindItem = IDFindTestItem( uiID );
	if( pFindItem != NULL )
		bRet = pFindItem->Run();	
	
	return bRet;	
}

BOOL STestClass::StopTestItem()
{
	BOOL bRet = TRUE;
	STestItemClass * pFindItem = NULL;
	
	pMsg->MsgOut( dINFO, _T("STestClass::StopTestItem\0") );

	for( UINT iLoop = 0; iLoop < TestItemEntryArraryIndex; iLoop++ )
	{
		pFindItem = TestItemEntryArrary[iLoop];
		bRet = pFindItem->Stop();
	}
	
	return bRet;	
}

BOOL STestClass::StopTestItem( UINT uiID )
{
	BOOL bRet = FALSE;
	STestItemClass * pFindItem = NULL;

	pMsg->MsgOut( dINFO, _T("STestClass::StopTestItem ID-0x%08x\0"), uiID );
	
	pFindItem = IDFindTestItem( uiID );
	if( pFindItem != NULL )
		bRet = pFindItem->Stop();	
	
	return bRet;	
}

BOOL STestClass::EnableLog( BOOL bEnable )
{
	STestItemClass * pFindItem = NULL;

	m_bEnableLog = bEnable;
	pLog->EnableLog( m_bEnableLog );

	for( UINT iLoop = 0; iLoop < TestItemEntryArraryIndex; iLoop++ )
	{
		pFindItem = TestItemEntryArrary[iLoop];
		pFindItem->EnableLog( m_bEnableLog );
	}
	
	pMsg->MsgOut( dINFO, _T("STestClass::EnableLog %s\0"), m_bEnableLog?L"On":L"Off" );
	
	return m_bEnableLog;
}

BOOL STestClass::SaveLog()
{
	BOOL bRet = TRUE;
	STestItemClass * pFindItem = NULL;
	
	///tell every test item to save log	
	for( UINT iLoop = 0; iLoop < TestItemEntryArraryIndex; iLoop++ )
	{
		pFindItem = TestItemEntryArrary[iLoop];
		pFindItem->SaveLog();
	}

	bRet = pLog->SaveLog();		/// save test class log
	pMsg->MsgOut( dINFO, _T("STestClass::SaveLog %s\0"), bRet?L"Ok":L"Fail" );

	return bRet;
}

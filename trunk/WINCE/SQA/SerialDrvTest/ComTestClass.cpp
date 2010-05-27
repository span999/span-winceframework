// TestClass.cpp : implementation of Battery Test class.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Windows.h"
#include <commctrl.h>
#include "winbase.h"
#include "winioctl.h"
#include "SerialDrvTest.h"
#include "ComTestClass.h"
#include "spComXClass.h"




/////////////////////////////////////////////////////////////////////////////
/// STestItemGPS
/*
STestItemGPS_______________________________________________________________()
*/

void STestItemGPS::TestContent( DWORD dwIndex )
{
	BOOL bRet = TRUE;
	DWORD dwBufIn = 0;
	DWORD dwBufOut = 0;
	
	if( NULL == pCom )
	{	
		pCom = new SGPSComClass();
#if 1	///debug 	
		pCom->SetDebugLevel( 0xFFFFFFFF );
		RECT rtTemp = rtItemWindow;
		rtTemp.top = (rtTemp.top+rtTemp.bottom)/2;
		pCom->SetShowAreaRect( rtTemp );
		pCom->SetFrameWindowHwnd( hWndFrameWindow );
		
		rtTemp = rtItemWindow;
		rtTemp.bottom = (rtTemp.top+rtTemp.bottom)/2;
		SetItemWindowRect( rtTemp );
#else
		pCom->SetDebugLevel( 0 );
#endif		
	}	

	if( pCom )
	{
		pCom->Disable();
		pCom->Enable();
		Sleep(2000);
		pMsg->MsgOut( _T("[%d]GPS Firmware boot %s "), dwIndex, pCom->IsFWBootOK()?L"Ok":L"Fail" );
		pLog->WriteLog( _T("[%d]GPS Firmware boot %s \0"), dwIndex, pCom->IsFWBootOK()?L"Ok":L"Fail" );
	}	
	///start testing
	
   			
   	
}

BOOL STestItemGPS::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("GPS Com driver API test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("This test will test the access of GPS Com driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("ComTestGPS.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	pCom = NULL;	///

	return bRet;
}




/////////////////////////////////////////////////////////////////////////////
/// SComBasicTest
/*
SComBasicTest______________________________________________________________()
*/

void SComBasicTest::AddTestItem( UINT uiID )
{
	if( uiID == IDM_BASIC_GPS )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemGPS( uiID );
	}
	else
		;	/// error handle	

	///pLog->WriteLog( _T("SComBasicTest::AddTestItem ID=%d Idx=%d\0"), uiID, TestItemEntryArraryIndex);
	pMsg->MsgOut( _T("SComBasicTest::AddTestItem ID=%d Idx=%d\0"), uiID, TestItemEntryArraryIndex);
	
	TestItemEntryArraryIndex++;

	InitAllItems();
}


BOOL SComBasicTest::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Serial driver basic test set.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
	
	wsprintf( szTempString, _T("This test will test the basic function of Serial driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );
	
	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("ComBasicTest.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}
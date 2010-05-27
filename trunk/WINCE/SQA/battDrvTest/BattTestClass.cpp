// TestClass.cpp : implementation of Battery Test class.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Windows.h"
#include <commctrl.h>
///#include "pm.h"
#include "winbase.h"
#include "battDrvTest.h"
#include "BattTestClass.h"



typedef DWORD (WINAPI * PFN_GetSystemPowerStatusEx2)( PSYSTEM_POWER_STATUS_EX2, DWORD, BOOL );


/////////////////////////////////////////////////////////////////////////////
/// STestItemAPI
/*
STestItemAPI_______________________________________________________________()
*/


void STestItemAPI::TestContent( DWORD dwIndex )
{
	DWORD dwRet = 0;
	SYSTEM_POWER_STATUS_EX2 SysPwrSat;
	
	static PFN_GetSystemPowerStatusEx2 gpfnGetSystemPowerStatusEx2 = NULL;
	
	if( NULL == gpfnGetSystemPowerStatusEx2 )
	{
		HMODULE hmCoreDll = LoadLibrary( L"coredll.dll" );
		if( hmCoreDll != NULL )
			gpfnGetSystemPowerStatusEx2 = (PFN_GetSystemPowerStatusEx2)GetProcAddress( hmCoreDll, L"GetSystemPowerStatusEx2" );
		else
		{	
			pMsg->MsgOut( _T("STestItemAPI::TestContent LoadLibrary fail") );
			pLog->WriteLog( _T("STestItemAPI::TestContent LoadLibrary fail") );
			return;
		}	
	}	
	
	if( NULL != gpfnGetSystemPowerStatusEx2 )
	{	
		///dwRet = GetSystemPowerStatusEx2( &SysPwrSat, sizeof(SYSTEM_POWER_STATUS_EX2), TRUE );
		dwRet = gpfnGetSystemPowerStatusEx2( &SysPwrSat, sizeof(SYSTEM_POWER_STATUS_EX2), TRUE );
	}
	else
	{	
		pMsg->MsgOut( _T("STestItemAPI::TestContent GetProcAddress fail") );
		pLog->WriteLog( _T("STestItemAPI::TestContent GetProcAddress fail") );
		return;
	}	
	
	///wsprintf( szTempString, _T("[%d]GetSystemPowerStatusEx2 %s %d\0"), dwIndex, (dwRet==0)?L"Fail":L"Success", uiTestItemID );
	pMsg->MsgOut( _T("[%d]GetSystemPowerStatusEx2 %s %d\0"), dwIndex, (dwRet==0)?L"Fail":L"Success", uiTestItemID );


	DWORD dwNowTick;
	dwNowTick = GetTickCount();

	///new update
	NowPwrSat.Index = dwIndex;
	NowPwrSat.Tick = dwNowTick;
	NowPwrSat.syspwrstatex2 = SysPwrSat;
		
	if( FindPowerStatusDiff( &NowPwrSat, &PrePwrSat ) )		///Log only when change
	{	
		///save update
		PrePwrSat.Index = dwIndex;
		PrePwrSat.Tick = dwNowTick;
		PrePwrSat.syspwrstatex2 = SysPwrSat;
	}
	
}


BOOL STestItemAPI::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Battery driver API test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("This test will test the API of battery driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("BattTestAPI.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}

BOOL STestItemAPI::FindPowerStatusDiff( PWRSTATREC *pNowPwrSat, PWRSTATREC *pPrePwrSat )
{
	BOOL bRet = FALSE;

	if( pNowPwrSat->syspwrstatex2.ACLineStatus != pPrePwrSat->syspwrstatex2.ACLineStatus )
	{	
		pLog->WriteLog( _T("ACLineStatus:[%d@%d]%d->[%d@%d]%d\r\n"), pPrePwrSat->Index, pPrePwrSat->Tick, pPrePwrSat->syspwrstatex2.ACLineStatus, pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.ACLineStatus );
		bRet = TRUE;
	}	

	if( pNowPwrSat->syspwrstatex2.BatteryFlag != pPrePwrSat->syspwrstatex2.BatteryFlag )
	{	
		pLog->WriteLog( _T("BatteryFlag:[%d@%d]%d->[%d@%d]%d\r\n"), pPrePwrSat->Index, pPrePwrSat->Tick, pPrePwrSat->syspwrstatex2.BatteryFlag, pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryFlag );
		bRet = TRUE;
	}	

	if( pNowPwrSat->syspwrstatex2.BatteryLifePercent != pPrePwrSat->syspwrstatex2.BatteryLifePercent )
	{	
		pLog->WriteLog( _T("BatteryLifePercent:[%d@%d]%d->[%d@%d]%d\r\n"), pPrePwrSat->Index, pPrePwrSat->Tick, pPrePwrSat->syspwrstatex2.BatteryLifePercent, pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryLifePercent );
		bRet = TRUE;
	}

	if( pNowPwrSat->syspwrstatex2.BatteryTemperature != pPrePwrSat->syspwrstatex2.BatteryTemperature )
	{	
		pLog->WriteLog( _T("BatteryTemperature:[%d@%d]%d->[%d@%d]%d\r\n"), pPrePwrSat->Index, pPrePwrSat->Tick, pPrePwrSat->syspwrstatex2.BatteryTemperature, pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryTemperature );
		bRet = TRUE;
	}

	if( bRet )
	{	
		pLog->WriteLog( _T("[%d@%d]GetSystemPowerStatusEx2 %s %d\r\n\0"), pNowPwrSat->Index, pNowPwrSat->Tick, L"Success", uiTestItemID );
		pMsg->MsgOut( _T("[%d@%d]GetSystemPowerStatusEx2 %s %d\r\n\0"), pNowPwrSat->Index, pNowPwrSat->Tick, L"Success", uiTestItemID );	
		pLog->WriteLog( _T("AC:%d Flag:%d Batt:%d Temp:%d Typ:%d\r\n\0"), pNowPwrSat->syspwrstatex2.ACLineStatus, pNowPwrSat->syspwrstatex2.BatteryFlag, pNowPwrSat->syspwrstatex2.BatteryLifePercent, pNowPwrSat->syspwrstatex2.BatteryTemperature, pNowPwrSat->syspwrstatex2.BatteryChemistry );
		pMsg->MsgOut( _T("AC:%d Flag:%d Batt:%d Temp:%d Typ:%d\r\n\0"), pNowPwrSat->syspwrstatex2.ACLineStatus, pNowPwrSat->syspwrstatex2.BatteryFlag, pNowPwrSat->syspwrstatex2.BatteryLifePercent, pNowPwrSat->syspwrstatex2.BatteryTemperature, pNowPwrSat->syspwrstatex2.BatteryChemistry );
	}	
	
	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
/// STestItemBattLevel
/*
STestItemBattLevel_________________________________________________________()
*/

void STestItemBattLevel::TestContent( DWORD dwIndex )
{
	DWORD dwRet = 0;
	SYSTEM_POWER_STATUS_EX2 SysPwrSat;
	
	static PFN_GetSystemPowerStatusEx2 gpfnGetSystemPowerStatusEx2 = NULL;
	
	if( NULL == gpfnGetSystemPowerStatusEx2 )
	{
		HMODULE hmCoreDll = LoadLibrary( L"coredll.dll" );
		if( hmCoreDll != NULL )
			gpfnGetSystemPowerStatusEx2 = (PFN_GetSystemPowerStatusEx2)GetProcAddress( hmCoreDll, L"GetSystemPowerStatusEx2" );
		else
		{	
			pMsg->MsgOut( _T("STestItemBattLevel::TestContent LoadLibrary fail") );
			pLog->WriteLog( _T("STestItemBattLevel::TestContent LoadLibrary fail") );
			return;
		}	
	}	
	
	if( NULL != gpfnGetSystemPowerStatusEx2 )
	{	
		///dwRet = GetSystemPowerStatusEx2( &SysPwrSat, sizeof(SYSTEM_POWER_STATUS_EX2), TRUE );
		dwRet = gpfnGetSystemPowerStatusEx2( &SysPwrSat, sizeof(SYSTEM_POWER_STATUS_EX2), TRUE );
	}
	else
	{	
		pMsg->MsgOut( _T("STestItemBattLevel::TestContent GetProcAddress fail") );
		pLog->WriteLog( _T("STestItemBattLevel::TestContent GetProcAddress fail") );
		return;
	}
	
	if( 0 == dwRet )
	{	
		 pMsg->MsgOut( _T("STestItemBattLevel::TestContent GetSystemPowerStatusEx2 fail") );
		return;
	}
		 
	///wsprintf( szTempString, _T("[%d]Batt level = %d%% AC %s %d\0"), dwIndex, SysPwrSat.BatteryLifePercent, (1==SysPwrSat.ACLineStatus)?L"On":L"Off",uiTestItemID );
	pMsg->MsgOut( _T("[%d]Batt level = %d%% AC %s %d\0"), dwIndex, SysPwrSat.BatteryLifePercent, (1==SysPwrSat.ACLineStatus)?L"On":L"Off",uiTestItemID );
	
	///pLog->WriteLog( _T("[%d@%d]Batt level = %d%% AC %s Charge %s %d\r\n\0"), dwIndex, GetTickCount(), SysPwrSat.BatteryLifePercent, (1==SysPwrSat.ACLineStatus)?L"On":L"Off", (BATTERY_FLAG_CHARGING==SysPwrSat.BatteryFlag)?L"On":L"Off", uiTestItemID );
	DWORD dwNowTick;
	dwNowTick = GetTickCount();

	///new update
	NowPwrSat.Index = dwIndex;
	NowPwrSat.Tick = dwNowTick;
	NowPwrSat.syspwrstatex2 = SysPwrSat;
		
	if( FindBattLifeDiff( &NowPwrSat, &PrePwrSat ) )		///Log only when change
	{	
		///save update
		PrePwrSat.Index = dwIndex;
		PrePwrSat.Tick = dwNowTick;
		PrePwrSat.syspwrstatex2 = SysPwrSat;
	}
}


BOOL STestItemBattLevel::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Battery driver level test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("This test will test the battery level of battery driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("BattTestLevel.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}

BOOL STestItemBattLevel::FindBattLifeDiff( PWRSTATREC *pNowPwrSat, PWRSTATREC *pPrePwrSat )
{
	BOOL bRet = FALSE;

	if( pNowPwrSat->syspwrstatex2.BatteryLifePercent != pPrePwrSat->syspwrstatex2.BatteryLifePercent )
	{	
		pLog->WriteLog( _T("BatteryLifePercent:[%d@%d]%d->[%d@%d]%d\r\n"), pPrePwrSat->Index, pPrePwrSat->Tick, pPrePwrSat->syspwrstatex2.BatteryLifePercent, pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryLifePercent );
		pMsg->MsgOut( _T("BatteryLifePercent:[%d@%d]%d->[%d@%d]%d\r\n"), pPrePwrSat->Index, pPrePwrSat->Tick, pPrePwrSat->syspwrstatex2.BatteryLifePercent, pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryLifePercent );
		bRet = TRUE;
	}

	if( bRet )
	{	
		///pLog->WriteLog( _T("[%d@%d]GetSystemPowerStatusEx2 %s %d\r\n\0"), pNowPwrSat->Index, pNowPwrSat->Tick, L"Success", uiTestItemID );
		///pLog->WriteLog( _T("AC:%d Flag:%d Batt:%d Temp:%d Typ:%d\r\n\0"), pNowPwrSat->syspwrstatex2.ACLineStatus, pNowPwrSat->syspwrstatex2.BatteryFlag, pNowPwrSat->syspwrstatex2.BatteryLifePercent, pNowPwrSat->syspwrstatex2.BatteryTemperature, pNowPwrSat->syspwrstatex2.BatteryChemistry );
		pLog->WriteLog( _T("[%d@%d]Batt level = %d%% AC:%s Charge:%s \r\n"), pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryLifePercent, (1==pNowPwrSat->syspwrstatex2.ACLineStatus)?L"On":L"Off", (BATTERY_FLAG_CHARGING==pNowPwrSat->syspwrstatex2.BatteryFlag)?L"On":L"Off" );
		pMsg->MsgOut( _T("[%d@%d]Batt level = %d%% AC:%s Charge:%s \r\n"), pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryLifePercent, (1==pNowPwrSat->syspwrstatex2.ACLineStatus)?L"On":L"Off", (BATTERY_FLAG_CHARGING==pNowPwrSat->syspwrstatex2.BatteryFlag)?L"On":L"Off" );
	}	
	
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
/// STestItemBattTemper
/*
STestItemBattTemper________________________________________________________()
*/

void STestItemBattTemper::TestContent( DWORD dwIndex )
{
	DWORD dwRet = 0;
	SYSTEM_POWER_STATUS_EX2 SysPwrSat;
	LONG lTempera;
	
	static PFN_GetSystemPowerStatusEx2 gpfnGetSystemPowerStatusEx2 = NULL;
	
	if( NULL == gpfnGetSystemPowerStatusEx2 )
	{
		HMODULE hmCoreDll = LoadLibrary( L"coredll.dll" );
		if( hmCoreDll != NULL )
			gpfnGetSystemPowerStatusEx2 = (PFN_GetSystemPowerStatusEx2)GetProcAddress( hmCoreDll, L"GetSystemPowerStatusEx2" );
		else
		{	
			pMsg->MsgOut( _T("STestItemBattTemper::TestContent LoadLibrary fail") );
			pLog->WriteLog( _T("STestItemBattTemper::TestContent LoadLibrary fail") );
			return;
		}	
	}	
	
	if( NULL != gpfnGetSystemPowerStatusEx2 )
	{	
		///dwRet = GetSystemPowerStatusEx2( &SysPwrSat, sizeof(SYSTEM_POWER_STATUS_EX2), TRUE );
		dwRet = gpfnGetSystemPowerStatusEx2( &SysPwrSat, sizeof(SYSTEM_POWER_STATUS_EX2), TRUE );
	}
	else
	{	
		pMsg->MsgOut( _T("STestItemBattTemper::TestContent GetProcAddress fail") );
		pLog->WriteLog( _T("STestItemBattTemper::TestContent GetProcAddress fail") );
		return;
	}
	
	if( 0 == dwRet )
	{	
		pMsg->MsgOut( _T("STestItemBattTemper::TestContent GetSystemPowerStatusEx2 fail") );
		pLog->WriteLog( _T("STestItemBattTemper::TestContent GetSystemPowerStatusEx2 fail") );
		return;
	}
	
	lTempera = SysPwrSat.BatteryTemperature;
	wsprintf( szTempString, _T("[%d]Batt temperature = %d %d\0"), dwIndex, lTempera, uiTestItemID );
	pMsg->MsgOut( szTempString );
	
	///pLog->WriteLog( _T("[%d@%d]Batt temperature = %d %d\r\n\0"), dwIndex, GetTickCount(), lTempera, uiTestItemID );
	DWORD dwNowTick;
	dwNowTick = GetTickCount();

	///new update
	NowPwrSat.Index = dwIndex;
	NowPwrSat.Tick = dwNowTick;
	NowPwrSat.syspwrstatex2 = SysPwrSat;
		
	if( FindBattTemperaDiff( &NowPwrSat, &PrePwrSat ) )		///Log only when change
	{	
		///save update
		PrePwrSat.Index = dwIndex;
		PrePwrSat.Tick = dwNowTick;
		PrePwrSat.syspwrstatex2 = SysPwrSat;
	}	
}


BOOL STestItemBattTemper::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Battery driver temperature test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("This test will test the temperature of battery driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("BattTestTempera.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}

BOOL STestItemBattTemper::FindBattTemperaDiff( PWRSTATREC *pNowPwrSat, PWRSTATREC *pPrePwrSat )
{
	BOOL bRet = FALSE;

	if( pNowPwrSat->syspwrstatex2.BatteryTemperature != pPrePwrSat->syspwrstatex2.BatteryTemperature )
	{	
		pLog->WriteLog( _T("BatteryTemperature:[%d@%d]%d->[%d@%d]%d\r\n"), pPrePwrSat->Index, pPrePwrSat->Tick, pPrePwrSat->syspwrstatex2.BatteryTemperature, pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryTemperature );
		pMsg->MsgOut( _T("BatteryTemperature:[%d@%d]%d->[%d@%d]%d\r\n"), pPrePwrSat->Index, pPrePwrSat->Tick, pPrePwrSat->syspwrstatex2.BatteryTemperature, pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryTemperature );		
		bRet = TRUE;
	}

	if( bRet )
	{	
		///pLog->WriteLog( _T("[%d@%d]GetSystemPowerStatusEx2 %s %d\r\n\0"), pNowPwrSat->Index, pNowPwrSat->Tick, L"Success", uiTestItemID );
		///pLog->WriteLog( _T("AC:%d Flag:%d Batt:%d Temp:%d Typ:%d\r\n\0"), pNowPwrSat->syspwrstatex2.ACLineStatus, pNowPwrSat->syspwrstatex2.BatteryFlag, pNowPwrSat->syspwrstatex2.BatteryLifePercent, pNowPwrSat->syspwrstatex2.BatteryTemperature, pNowPwrSat->syspwrstatex2.BatteryChemistry );
		pLog->WriteLog( _T("[%d@%d]Batt temperature = %d\r\n\0"), pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryTemperature );
		pMsg->MsgOut( _T("[%d@%d]Batt temperature = %d\r\n\0"), pNowPwrSat->Index, pNowPwrSat->Tick, pNowPwrSat->syspwrstatex2.BatteryTemperature );		
	}	
	
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
/// SBattBasicTest
/*
SBattBasicTest_____________________________________________________________()
*/

void SBattBasicTest::AddTestItem( UINT uiID )
{
	if( uiID == IDM_BASIC_API )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemAPI( uiID );
	}
	else		
	if( uiID == IDM_BASIC_BATTERY_LEVEL )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemBattLevel( uiID );
	}
	if( uiID == IDM_BASIC_BATTERY_TEMPERATURE )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemBattTemper( uiID );
	}
	else
		;	/// error handle	

	///pLog->WriteLog( _T("SBattBasicTest::AddTestItem ID=%d Idx=%d\0"), uiID, TestItemEntryArraryIndex);
	
	TestItemEntryArraryIndex++;

	InitAllItems();
}

BOOL SBattBasicTest::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Battery driver basic test set.\r\n") );
	pMsg->MsgOut( dINFO|dINFO, szTempString );
	pLog->SetTitle( szTempString );
	
	wsprintf( szTempString, _T("This test will test the basic function of Battery driver.\r\n") );
	pMsg->MsgOut( dINFO|dINFO, szTempString );
	pLog->SetInfo( szTempString );
	
	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( dINFO|dINFO, szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("BattBasicTest.log\0") );
	pMsg->MsgOut( dINFO|dINFO, szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}
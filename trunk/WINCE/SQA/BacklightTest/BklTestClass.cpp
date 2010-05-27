// TestClass.cpp : implementation of Battery Test class.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Windows.h"
#include <commctrl.h>
///#include "pm.h"
#include "winbase.h"
#include "winioctl.h"
#include "BacklightTest.h"
#include "BklTestClass.h"
#include "Thales_defines.h"




/////////////////////////////////////////////////////////////////////////////
/// STestItemAPI
/*
STestItemAPI_______________________________________________________________()
*/

void STestItemAPI::TestContent( DWORD dwIndex )
{
	BOOL bRet;
	DWORD dwBufIn = 0;
	DWORD dwBufOut = 0;
	
	if( hLGT == INVALID_HANDLE_VALUE )
	{	
		hLGT = CreateFile( _T("LGT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
		if( hLGT == INVALID_HANDLE_VALUE )
		{	
			pMsg->MsgOut( _T("STestItemAPI::TestContent CreateFile fail !!!") );
			pLog->WriteLog( _T("STestItemAPI::TestContent CreateFile fail !!!") );
			return;
		}	
	}
		
	///start testing
	
	pLog->WriteLog( _T("API test Run %d %d\r\n"), dwIndex, uiTestItemID );
	
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_OFF, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_OFF %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_OFF %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );

   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_ON %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );	
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_ON %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );

   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_LCD_BACKLIGHT_OFF, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_LCD_BACKLIGHT_OFF %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_LCD_BACKLIGHT_OFF %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );

   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_LCD_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_LCD_BACKLIGHT_ON %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_LCD_BACKLIGHT_ON %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
   			
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_KEYPAD_BACKLIGHT_OFF, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_KEYPAD_BACKLIGHT_OFF %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_KEYPAD_BACKLIGHT_OFF %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
   			
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_KEYPAD_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_KEYPAD_BACKLIGHT_ON %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_KEYPAD_BACKLIGHT_ON %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
   			
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_UP, &dwBufIn, sizeof(dwBufIn), &dwBufOut, sizeof(dwBufOut), NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_UP %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );

   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_UP %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
   			
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_DOWN, &dwBufIn, sizeof(dwBufIn), &dwBufOut, sizeof(dwBufOut), NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_DOWN %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_DOWN %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );

   	dwBufIn = 100;
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwBufIn, sizeof(dwBufIn), &dwBufOut, sizeof(dwBufOut), NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
   	
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwBufIn, sizeof(dwBufIn), &dwBufOut, sizeof(dwBufOut), NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
   			
   	
}

BOOL STestItemAPI::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Backlight driver API test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("This test will test the API of Backlight driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("BklTestAPI.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
/// STestItemBrightness
/*
STestItemBrightness________________________________________________________()
*/

void STestItemBrightness::TestContent( DWORD dwIndex )
{
	BOOL bRet;
	DWORD dwBufIn = 0;
	DWORD dwBufOut = 0;
	
	if( hLGT == INVALID_HANDLE_VALUE )
	{	
		hLGT = CreateFile( _T("LGT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
		if( hLGT == INVALID_HANDLE_VALUE )
		{	
			pMsg->MsgOut( _T("STestItemBrightness::TestContent CreateFile fail !!!") );
			pLog->WriteLog( _T("STestItemBrightness::TestContent CreateFile fail !!!") );
			return;
		}	
	}
		
	///start testing
	
	pLog->WriteLog( _T("Brightness test Run %d %d\r\n"), dwIndex, uiTestItemID );
	
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_ON %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );  		
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_ON %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );

   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_LCD_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_LCD_BACKLIGHT_ON %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_LCD_BACKLIGHT_ON %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
   			 			
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_KEYPAD_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_KEYPAD_BACKLIGHT_ON %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_KEYPAD_BACKLIGHT_ON %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
   			

	for( dwBufIn = 100; dwBufIn > 0; dwBufIn-- )
	{

	   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwBufIn, sizeof(dwBufIn), &dwBufOut, sizeof(dwBufOut), NULL, NULL);
		pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
	   	if( !bRet )
	   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
	   	if( dwBufOut != dwBufIn )
	   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Wrong", uiTestItemID );   	
	   			
	   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwBufIn, sizeof(dwBufIn), &dwBufOut, sizeof(dwBufOut), NULL, NULL);
		pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
	   	if( !bRet )
	   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
	   	if( dwBufOut != dwBufIn )
	   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Wrong", uiTestItemID );
	   			
	   	Sleep(10);
	}
	   			
	for( dwBufIn = 0; dwBufIn <= 100; dwBufIn++ )
	{

	   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwBufIn, sizeof(dwBufIn), &dwBufOut, sizeof(dwBufOut), NULL, NULL);
		pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
	   	if( !bRet )
	   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
	   	if( dwBufOut != dwBufIn )
	   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Wrong", uiTestItemID );   	
	   			
	   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwBufIn, sizeof(dwBufIn), &dwBufOut, sizeof(dwBufOut), NULL, NULL);
		pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
	   	if( !bRet )
	   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
	   	if( dwBufOut != dwBufIn )
	   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Wrong", uiTestItemID );
	   	
	   	Sleep(10);
	}

   	dwBufIn = 100;
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwBufIn, sizeof(dwBufIn), &dwBufOut, sizeof(dwBufOut), NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
   	
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwBufIn, sizeof(dwBufIn), &dwBufOut, sizeof(dwBufOut), NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );

}


BOOL STestItemBrightness::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Backlight driver Brightness test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("This test will test the Brightness control of Backlight driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("BklTestBright.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
/// STestItemOnOff
/*
STestItemOnOff______________________________________________________________()
*/

void STestItemOnOff::TestContent( DWORD dwIndex )
{
	BOOL bRet;
	DWORD dwBufIn = 0;
	DWORD dwBufOut = 0;
	
	if( hLGT == INVALID_HANDLE_VALUE )
	{	
		hLGT = CreateFile( _T("LGT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
		if( hLGT == INVALID_HANDLE_VALUE )
		{	
			pMsg->MsgOut( _T("STestItemAPI::TestContent CreateFile fail !!!") );
			pLog->WriteLog( _T("STestItemAPI::TestContent CreateFile fail !!!") );
			return;
		}	
	}
		
	///start testing
	
	pLog->WriteLog( _T("On/Off test Run %d %d\r\n"), dwIndex, uiTestItemID );
	
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_OFF, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_OFF %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID ); 		
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_OFF %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
   			
   	bRet = DeviceIoControl( hLGT, IOCTL_OEM_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	pMsg->MsgOut( _T("[%d]IOCTL_OEM_BACKLIGHT_ON %s %d\0"), dwIndex,(bRet)?L"Success":L"Fail", uiTestItemID );
   	if( !bRet )
   		pLog->WriteLog( _T("[%d@%d]IOCTL_OEM_BACKLIGHT_ON %s %d\r\n\0"), dwIndex, GetTickCount(), (bRet)?L"Success":L"Fail", uiTestItemID );
	
}

///LPWSTR STestItemOnOff::GetLogFilePathName()
///{
///	///return ( _T("\\BklTestOnOff.log") );
///	wsprintf( szTempString, _T("\\BklTestOnOff.log\0") );
///	return szTempString;	
///}

BOOL STestItemOnOff::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Backlight driver On/Off test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("This test will test the On/Off control of Backlight driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("BklTestOnOff.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
/// SBattBasicTest
/*
SBattBasicTest______________________________________________________________()
*/

void SBklBasicTest::AddTestItem( UINT uiID )
{
	if( uiID == IDM_BASIC_API )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemAPI( uiID );
	}
	else		
	if( uiID == IDM_BASIC_BRIGHTNESS )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemBrightness( uiID );
	}
	if( uiID == IDM_BASIC_ONOFF )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemOnOff( uiID );
	}
	else
		;	/// error handle	

	///pLog->WriteLog( _T("SBattBasicTest::AddTestItem ID=%d Idx=%d\0"), uiID, TestItemEntryArraryIndex);
	
	TestItemEntryArraryIndex++;

	InitAllItems();
}


BOOL SBklBasicTest::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Backlight driver basic test set.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
	
	wsprintf( szTempString, _T("This test will test the basic function of Backlight driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );
	
	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("BklBasicTest.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}
// TestClass.cpp : implementation of Battery Test class.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Windows.h"
#include <commctrl.h>
#include "winbase.h"
#include "micIOCTLtest.h"
#include "IoctlTestClass.h"


/////////////////////////////////////////////////////////////////////////////
/// STestItemPRX
/*
STestItemPRX_______________________________________________________________()
*/

void STestItemPRX::TestContent( DWORD dwIndex )
{
	BOOL bRet = TRUE;
	DWORD dwLoop = 0;
		
	///pMsg->MsgOut( _T("STestItemPRX::TestContent") );
	
	///start testing
	if( !TryOpenPortName( DRVPRIFIX ) )
	{
		pMsg->MsgOut( dFAIL, _T("STestItemPRX::TestContent TryOpenPortName fail !") );
		return;
	}	
	
	for( dwLoop = 0; dwLoop < DEVICEIOCTLLISTNUM; dwLoop++ )
	{
		DevIoctlList[dwLoop].DevHandle = m_hPRX;
	}


	RunDevIoctlList();
	
 	
}

BOOL STestItemPRX::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("PRX driver API test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("This test will test the access of PRX driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("IoctlTestPRX.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

///	pCom = NULL;	///
	
	return bRet;
}

BOOL STestItemPRX::RunDevIoctlList()
{
	BOOL bRet = TRUE;
	DWORD dwBufRet = 0;
	DWORD dwLoop = 0;
	PVOID pBufIn = NULL;
	PVOID pBufOut = NULL;


	for( dwLoop = 0; dwLoop < DEVICEIOCTLLISTNUM; dwLoop++ )
	{
		///dwBufIn = 60;
		if( DevIoctlList[dwLoop].hasIN )	///allocate for we need 
		{	
			pBufIn = (BYTE *)malloc(DevIoctlList[dwLoop].sizeIN);
			if( pBufIn == NULL )
			{
				pMsg->MsgOut( dFAIL, _T("%s malloc Fail !"), DevIoctlList[dwLoop].Descrip );
				pLog->WriteLog( _T("%s malloc Fail !\r\n"), DevIoctlList[dwLoop].Descrip );
				break;	
			}
			*(DWORD*)pBufIn = DevIoctlList[dwLoop].setIN;
		}	

		if( DevIoctlList[dwLoop].hasOUT )	///allocate for we need 
		{	
			pBufOut = (BYTE *)malloc(DevIoctlList[dwLoop].sizeOUT);
			if( pBufOut == NULL )
			{
				pMsg->MsgOut( dFAIL, _T("%s malloc Fail !"), DevIoctlList[dwLoop].Descrip );
				pLog->WriteLog( _T("%s malloc Fail !\r\n"), DevIoctlList[dwLoop].Descrip );
				break;	
			}
			if( IOCTL_POWER_SET == DevIoctlList[dwLoop].IOcode )
				*(CEDEVICE_POWER_STATE*)pBufOut = D0; 
		}


		if(
			DeviceIoControl( 
							DevIoctlList[dwLoop].DevHandle, 
							DevIoctlList[dwLoop].IOcode, 
							(DevIoctlList[dwLoop].hasIN)?(pBufIn):(NULL),
							(DevIoctlList[dwLoop].hasIN)?(DevIoctlList[dwLoop].sizeIN):(0), 
							(DevIoctlList[dwLoop].hasOUT)?(pBufOut):(NULL), 
							(DevIoctlList[dwLoop].hasOUT)?(DevIoctlList[dwLoop].sizeOUT):(0), 
							(DevIoctlList[dwLoop].hasRET)?(&dwBufRet):(NULL), 
							NULL
							) 
		)
		{
			pMsg->MsgOut( dFAIL, _T("%s success"), DevIoctlList[dwLoop].Descrip );
			pLog->WriteLog( _T("%s success\r\n"), DevIoctlList[dwLoop].Descrip );
		}	
		else
		{
			pMsg->MsgOut( dINFO, _T("%s fail ErrCode=%d"), DevIoctlList[dwLoop].Descrip, GetLastError());
			pLog->WriteLog( _T("%s fail ErrCode=%d\r\n"), DevIoctlList[dwLoop].Descrip, GetLastError());
		}


		if( DevIoctlList[dwLoop].hasIN )	///free
		{	
			free(pBufIn);
			pBufIn = NULL;
		}
		if( DevIoctlList[dwLoop].hasOUT )	///free
		{
			if( IOCTL_POWER_CAPABILITIES == DevIoctlList[dwLoop].IOcode )
			{	
				POWER_CAPABILITIES *pPwrCap = (POWER_CAPABILITIES *)pBufOut;
				pMsg->MsgOut( dINFO, _T("%s out=0x%x"), DevIoctlList[dwLoop].Descrip, pPwrCap->DeviceDx );
				pLog->WriteLog( _T("%s out=0x%x\r\n"), DevIoctlList[dwLoop].Descrip, pPwrCap->DeviceDx );
			}
			else
			if( IOCTL_OEM_GET_POWER_STATUS == DevIoctlList[dwLoop].IOcode || IOCTL_OEM_GET_DEVICE_STATUS == DevIoctlList[dwLoop].IOcode )
			{	
				pMsg->MsgOut( dINFO, _T("%s out=0x%x"), DevIoctlList[dwLoop].Descrip, *(DWORD*)pBufOut );
				pLog->WriteLog( _T("%s out=0x%x\r\n"), DevIoctlList[dwLoop].Descrip, *(DWORD*)pBufOut );
			}
			else
			if( IOCTL_POWER_GET == DevIoctlList[dwLoop].IOcode )
			{	
				pMsg->MsgOut( dINFO, _T("%s out=%d"), DevIoctlList[dwLoop].Descrip, *(DWORD*)pBufOut );
				pLog->WriteLog( _T("%s out=%d\r\n"), DevIoctlList[dwLoop].Descrip, *(DWORD*)pBufOut );
			}
			else
			{	
				pMsg->MsgOut( dINFO, _T("%s out=%d"), DevIoctlList[dwLoop].Descrip, *(DWORD*)pBufOut );
				pLog->WriteLog( _T("%s out=%d\r\n"), DevIoctlList[dwLoop].Descrip, *(DWORD*)pBufOut );
			}	
			free(pBufOut);
			pBufOut = NULL;
		}
		if( DevIoctlList[dwLoop].hasRET )	///
		{
			pMsg->MsgOut( dINFO, _T("%s outsize=%d"), DevIoctlList[dwLoop].Descrip, dwBufRet );
			pLog->WriteLog( _T("%s outsize=%d\r\n"), DevIoctlList[dwLoop].Descrip, dwBufRet );
		}

	}
	
	return bRet;
}

BOOL STestItemPRX::TryOpenPortName( LPTSTR lpszPortName )
{
	BOOL bRet = FALSE;
	
	if( INVALID_HANDLE_VALUE != m_hPRX )
		return TRUE;
	
	m_hPRX = CreateFile( lpszPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
	
	if( INVALID_HANDLE_VALUE == m_hPRX )
	{
		m_hPRX = INVALID_HANDLE_VALUE;	
		bRet = FALSE;
	}
	else
	{	
		bRet = TRUE;
	}	

	pMsg->MsgOut( dDBUG, _T("STestItemPRX::TryOpenPortName %s %s "), lpszPortName, bRet?L"Ok":L"Fail" );
		
	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
/// SIoctlBasicTest
/*
SIoctlBasicTest______________________________________________________________()
*/

void SIoctlBasicTest::AddTestItem( UINT uiID )
{
	if( uiID == IDM_BASIC_PRX )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemPRX( uiID );
	}
	else
		;	/// error handle	

	///pLog->WriteLog( _T("SIoctlBasicTest::AddTestItem ID=%d Idx=%d\0"), uiID, TestItemEntryArraryIndex);
	pMsg->MsgOut( _T("SIoctlBasicTest::AddTestItem ID=%d Idx=%d\0"), uiID, TestItemEntryArraryIndex);
	
	TestItemEntryArraryIndex++;

	InitAllItems();
}


BOOL SIoctlBasicTest::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Ioctl basic test set.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
	
	wsprintf( szTempString, _T("This test will test the basic function of Ioctl.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );
	
	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("IoctlBasicTest.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}
// TestClass.cpp : implementation of Battery Test class.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Windows.h"
#include <commctrl.h>
#include "winbase.h"
#include "winioctl.h"
///#include "ShowMsg.h"
///#include "ComTestClass.h"
#include "spComXClass.h"



/////////////////////////////////////////////////////////////////////////////
/// SComXClass
/*
SComXClass_______________________________________________________________()
*/

SComXClass::~SComXClass()
{
	m_bThreadRUN = FALSE;
   	CloseHandle( m_hComPort );
}

SComXClass::SComXClass()
{
   	Init();
}

SComXClass::SComXClass( DWORD dwComNum )
{
   	Init();
   	SetComNumber( dwComNum );
}

DWORD SComXClass::SetDebugLevel( DWORD dwLevel )
{
	m_dwDebugLevel = dwLevel;
	if( pMsg )
		pMsg->SetDebugLevel( m_dwDebugLevel );	
	return m_dwDebugLevel; 
}

BOOL SComXClass::SetFrameWindowHwnd( HWND hMainWnd )
{
	BOOL bRet = TRUE;
	m_hWndFrameWindow = hMainWnd;
	if( pMsg )
		pMsg->SetFrameWindowHwnd( m_hWndFrameWindow );
	return bRet;
}

BOOL SComXClass::SetShowAreaRect( RECT rtSetArea )
{
	BOOL bRet = TRUE;
	m_rtShowArea = rtSetArea;
	if( pMsg )
		pMsg->SetShowAreaRect( m_rtShowArea );
	return bRet;
}

HANDLE SComXClass::FindComX()
{
	HANDLE hRet = NULL;
	DWORD dwTryPortIdx = 1;
	DWORD dwTryBaudrateIdx = 0;
	BOOL bComXFound = FALSE;
	
	pMsg->MsgOut( dDBUG, _T("SComXClass::FindComX") );
	pMsg->MsgOut( dINFO, _T("Wait ... Try to find out GPS port !") );
	
	///we are try to find out the port with GPS NMEA data

	///is port number known ?
	while( !IsComNumberValid( m_dwComNum ) && (dwTryPortIdx < PORTNUMBERLISTS) )
	{
		if( bComXFound )
			break;

		///try open this port
		if( !TryOpenPortName( PortNumNameList[dwTryPortIdx].szPortName ) )
		{	
			dwTryPortIdx++;
			continue;	///try next one if this port can not open
		}	
		hRet = m_hComPort;
		
		///try baudrate
		while( !IsBaudRateValid( m_dwBaudRate ) && (dwTryBaudrateIdx < BAUDRATELISTS) )
		{
			if( bComXFound )
				break;
				
			if( !InitCom( BaudRateList[dwTryBaudrateIdx] ) )
			{
				dwTryBaudrateIdx++;	
				continue;	///try next one if this can not use
			}
			
			if( NULL == m_hComReadThread )
			{	///create read thread
				m_hComReadThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ComReadThreadStub, this, 0, NULL );
			}
			
			Sleep( 3000 );

			///check if we found the specified data in ReadThread		
			///if( ... )
			///{	///we found the GPS port
			///	bComXFound = TRUE;
			///	m_dwComNum = dwTryPortIdx;
			///	m_dwBaudRate = BaudRateList[dwTryBaudrateIdx];
			///	pMsg->MsgOut( dINFO|dDBUG, _T("SComXClass::FindComX Found !! %s %d"), PortNumNameList[m_dwComNum].szPortName, m_dwBaudRate );
			///	Sleep( 3000 );
			///}	

			CloseHandle( m_hComReadThread );			
			m_hComReadThread = NULL;
			m_bThreadRUN = FALSE;
			Sleep( 1000 );
			m_bThreadRUN = TRUE;
			dwTryBaudrateIdx++;	
		}

		CloseHandle( m_hComPort );
		m_hComPort = INVALID_HANDLE_VALUE;
		
		dwTryPortIdx++;
	}


	if( !bComXFound )
		hRet = NULL;
		
	return hRet;
}

BOOL SComXClass::InitMsg()
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

void SComXClass::Init()
{
	///clear all 
	pMsg = NULL;
	m_hWndFrameWindow = NULL;
	m_hComPort = INVALID_HANDLE_VALUE;
	m_dwComNum = 0;
	m_dwBaudRate = 0;
	m_hComReadThread = NULL;
	m_hComReadThreadEvent = NULL;
	m_bThreadRUN = TRUE;
///	m_bIsNMEA = FALSE;
	
	InitMsg();
}

BOOL SComXClass::SetBaudRate( DWORD dwBaudRate )
{
	BOOL bRet = FALSE;
	
	if( IsBaudRateValid( dwBaudRate ) )
	{
		m_dwBaudRate = dwBaudRate;
		bRet = TRUE;
	}
	pMsg->MsgOut( dDBUG, _T("SComXClass::SetBaudRate = %d"), dwBaudRate );
		
	return bRet;
}

BOOL SComXClass::SetComNumber( DWORD dwComNum )
{
	BOOL bRet = FALSE;
	
	if( IsComNumberValid( dwComNum ) )
	{
		m_dwComNum = dwComNum;
		bRet = TRUE;
	}
	pMsg->MsgOut( dDBUG, _T("SComXClass::SetComNumber = %d"), dwComNum );
		
	return bRet;
}

BOOL  SComXClass::IsBaudRateValid( DWORD dwBaudrate )
{
	BOOL bRet = FALSE;
	
	for( UINT Loop = 0; Loop < BAUDRATELISTS; Loop++ )
	{
		if( dwBaudrate == BaudRateList[Loop] )
		{
			bRet = TRUE;
			break;	
		}
	}
	
	pMsg->MsgOut( dDBUG, _T("SComXClass::IsBaudRateValid %s"),  bRet?L"TRUE":L"FALSE");
	
	return bRet;
}

BOOL  SComXClass::IsComNumberValid( DWORD dwComNum )
{
	BOOL bRet = FALSE;
	
	if( dwComNum > 0 && dwComNum < 16 )		///1~15
		bRet = TRUE;
	
	pMsg->MsgOut( dDBUG, _T("SComXClass::IsComNumberValid %s"),  bRet?L"TRUE":L"FALSE");
		
	return bRet;
}
	
BOOL SComXClass::InitCom()
{
	BOOL bRet = FALSE;
	DCB PortDCB;
	DWORD dwError;
		
	///check if port ok
	if( !IsComReady() )
		return FALSE;

	SetCommMask( m_hComPort, EV_RXCHAR );
	SetupComm( m_hComPort, 512, 512 );
	PurgeComm( m_hComPort, PURGE_TXCLEAR|PURGE_RXCLEAR );

	PortDCB.DCBlength = sizeof(DCB); 

	GetCommState( m_hComPort, &PortDCB );

	PortDCB.BaudRate = m_dwBaudRate;
	PortDCB.fBinary = TRUE; 		///must be true
	PortDCB.fParity = TRUE; 		///parity check
	PortDCB.fOutxCtsFlow = TRUE;
	PortDCB.fOutxDsrFlow = FALSE;
	PortDCB.fDtrControl = DTR_CONTROL_ENABLE;
	PortDCB.fDsrSensitivity = FALSE;
	///PortDCB.fTXContinueOnXoff = TRUE;
	PortDCB.fTXContinueOnXoff = FALSE;
	PortDCB.fOutX = FALSE;
	PortDCB.fInX = FALSE;
	PortDCB.fErrorChar = FALSE;
	PortDCB.fNull = FALSE;
	PortDCB.fRtsControl = RTS_CONTROL_ENABLE;
	PortDCB.fAbortOnError = FALSE;
	PortDCB.ByteSize = 8;
	PortDCB.Parity = NOPARITY;
	PortDCB.StopBits = ONESTOPBIT;


	bRet = SetCommState( m_hComPort, &PortDCB );
	if( !bRet ) 
	{
		dwError = GetLastError();
	}
	
	pMsg->MsgOut( dDBUG, _T("SComXClass::InitCom %s %d"), bRet?L"Ok":L"Fail", dwError);
	
	return bRet;
}

BOOL SComXClass::InitCom( DWORD dwTryBaudRate )
{
	BOOL bRet = FALSE;
	DCB PortDCB;
	DWORD dwError;
		
	///check if port ok
	if( !IsComReady() )
		return FALSE;

	SetCommMask( m_hComPort, EV_RXCHAR );
	SetupComm( m_hComPort, 512, 512 );
	PurgeComm( m_hComPort, PURGE_TXCLEAR|PURGE_RXCLEAR );

	PortDCB.DCBlength = sizeof(DCB); 

	GetCommState( m_hComPort, &PortDCB );

	PortDCB.BaudRate = dwTryBaudRate;
	PortDCB.fBinary = TRUE; 		///must be true
	PortDCB.fParity = TRUE; 		///parity check
	PortDCB.fOutxCtsFlow = TRUE;
	PortDCB.fOutxDsrFlow = FALSE;
	PortDCB.fDtrControl = DTR_CONTROL_ENABLE;
	PortDCB.fDsrSensitivity = FALSE;
	///PortDCB.fTXContinueOnXoff = TRUE;
	PortDCB.fTXContinueOnXoff = FALSE;
	PortDCB.fOutX = FALSE;
	PortDCB.fInX = FALSE;
	PortDCB.fErrorChar = FALSE;
	PortDCB.fNull = FALSE;
	PortDCB.fRtsControl = RTS_CONTROL_ENABLE;
	PortDCB.fAbortOnError = FALSE;
	PortDCB.ByteSize = 8;
	PortDCB.Parity = NOPARITY;
	PortDCB.StopBits = ONESTOPBIT;


	bRet = SetCommState( m_hComPort, &PortDCB );
	if( !bRet ) 
	{
		dwError = GetLastError();
	}
	
	pMsg->MsgOut( dDBUG, _T("SComXClass::InitCom with %d %s %d"), dwTryBaudRate, bRet?L"Ok":L"Fail", dwError);
	
	return bRet;
}

	
BOOL SComXClass::Enable()
{
	BOOL bRet = FALSE;
	
	
	///check if port already open
	if( IsComReady() )
	{
		bRet = TRUE;
	}
	else
	{
		if( !IsComNumberValid( m_dwComNum ) || !IsBaudRateValid( m_dwBaudRate ) )
		{
			pMsg->MsgOut( (IsComNumberValid( m_dwComNum ))?0:dWARN, _T("SComXClass::Enable !unknown Com port number!") );
			pMsg->MsgOut( (IsBaudRateValid( m_dwBaudRate ))?0:dWARN, _T("SComXClass::Enable !unknown baudrate!") );
			pMsg->MsgOut( dINFO|dWARN, _T("SComXClass::Enable !Try find to it out!") );
			
			if( NULL == FindComX() )
				bRet = FALSE;
			else
				bRet = TRUE;
			pMsg->MsgOut( dINFO|dWARN, _T("SComXClass::Enable !unknow com port %s!"), bRet?L"found":L"not found" );	
		}
		else
			bRet = TRUE;
		
		if( bRet )
			bRet = TryOpenPortName( PortNumNameList[m_dwComNum].szPortName );
		else
			return bRet;

		if( bRet )
			bRet = InitCom();
		
		if( NULL == m_hComReadThread )
		{	///create read thread
			m_hComReadThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ComReadThreadStub, this, 0, NULL );
			if( NULL == m_hComReadThread )
			{	
				bRet = FALSE;
			}
			else
				bRet = TRUE;
		}	


	}		
	
	pMsg->MsgOut( dDBUG, _T("SComXClass::Enable %s "), bRet?L"Ok":L"Fail" );
	
	return bRet;
}

BOOL SComXClass::Disable()
{
	BOOL bRet = FALSE;
	
	
	///check if port already close
	if( !IsComReady() )
	{
		bRet = TRUE;
	}
	else
	{
		CloseHandle( m_hComReadThread );			
		m_hComReadThread = NULL;		
		m_bThreadRUN = FALSE;
		Sleep(1000);
		m_bThreadRUN = TRUE;
		EscapeCommFunction( m_hComPort, CLRDTR );
		EscapeCommFunction( m_hComPort, CLRRTS );
		PurgeComm( m_hComPort, PURGE_TXCLEAR|PURGE_RXCLEAR );		
		bRet = CloseHandle( m_hComPort );
		m_hComPort = INVALID_HANDLE_VALUE;
	}		
	
	pMsg->MsgOut( dDBUG, _T("SComXClass::Disable %s "), bRet?L"Ok":L"Fail" );
	
	return bRet;
}


BOOL SComXClass::TryOpenPortName( LPTSTR lpszPortName )
{
	BOOL bRet = FALSE;
	
	m_hComPort = CreateFile( lpszPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
	
	if( !IsComReady() )
	{
		m_hComPort = INVALID_HANDLE_VALUE;	
		bRet = FALSE;
	}
	else
	{	
		bRet = TRUE;
	}	

	pMsg->MsgOut( dDBUG, _T("SComXClass::TryOpenPortName %s %s "), lpszPortName, bRet?L"Ok":L"Fail" );
		
	return bRet;
}

DWORD SComXClass::ComReadThreadStub( LPVOID lpParam )
{
	DWORD dwRet = 0;
	
	if( NULL != lpParam )
	{
		SComXClass *pThisItem = (SComXClass *)lpParam;
		dwRet = pThisItem->ComReadThread( lpParam );
	}

	return dwRet;
}

DWORD SComXClass::ComReadThread( LPVOID lpParam )
{
	DWORD dwRet = 0;

	DWORD dwCommModemStatus;
	DWORD dwComEvent;
	DWORD dwQueLen;
	DWORD dwRetLen;
	COMSTAT ComStat;
	DWORD dwErrorFlags;

	pMsg->MsgOut( dDBUG|dINIT, _T("SComXClass::ComReadThread ") );	

	while( m_hComPort != INVALID_HANDLE_VALUE && m_bThreadRUN )
	{
		/// Wait for an event to occur for the port
		WaitCommEvent( m_hComPort, &dwComEvent, 0 );	///wait for Rx
		
		pMsg->MsgOut( dDBUG, _T("SComXClass::WaitCommEvent =0x%08x "), dwComEvent );
		
		/// Re-specify the set of events to be monitored for the port.
		///SetCommMask( m_hComPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_RING );
		SetCommMask( m_hComPort, EV_RXCHAR );

		if( dwComEvent & EV_RXCHAR ) 
		{
			///Get the comp port status
			ClearCommError( m_hComPort, &dwErrorFlags, &ComStat );

			dwQueLen = ComStat.cbInQue;

			if( dwQueLen > 0 )
			{
				UCHAR buf[512];
				
				if( !ReadFile( m_hComPort, buf, dwQueLen, &dwRetLen , NULL ) )
				{
					pMsg->MsgOut( dFAIL, _T("SComXClass::ComReadThread ReadFile %s "), L"Fail" );
				}
				else
				{
					WCHAR wBuf[512];
					
					pMsg->MsgOut( dDBUG, _T("SComXClass::ComReadThread ReadFile %d bytes"), dwQueLen );

					for( DWORD i = 0; i < dwQueLen; i++ )
					{
						wBuf[i] = (WCHAR)buf[i];
						
						///Try to find out the specified data
						///if( (buf[i] == '$') && (buf[i+1] == 'G') && (buf[i+2] == 'P') )
						///	m_bIsNMEA = TRUE;
					}
					wBuf[i++] = '\0';
					pMsg->MsgOut( dINFO, _T("%s"), wBuf );
				}
			}
			else
			{
			}
		}
		
		GetCommModemStatus( m_hComPort, &dwCommModemStatus );
	}///while()

	pMsg->MsgOut( dFAIL, _T("SComXClass::ComReadThread exit") );
///	m_bIsNMEA = FALSE;
	
	return dwRet;
}

/////////////////////////////////////////////////////////////////////////////
/// SGPSComClass
/*
SGPSComClass______________________________________________________________()
*/

SGPSComClass::~SGPSComClass()
{
	m_bThreadRUN = FALSE;
   	CloseHandle( m_hComPort );
}

SGPSComClass::SGPSComClass()
{
	m_bIsFWBootOK = FALSE;
	m_bIsFWVersionOK = FALSE;
	m_bIsNMEA = FALSE;
   	Init();
}

SGPSComClass::SGPSComClass( DWORD dwComNum )
{
	m_bIsFWBootOK = FALSE;
	m_bIsFWVersionOK = FALSE;
	m_bIsNMEA = FALSE;
   	Init();
   	SetComNumber( dwComNum );
}

HANDLE SGPSComClass::FindComX()
{
	HANDLE hRet = NULL;
	DWORD dwTryPortIdx = 1;
	DWORD dwTryBaudrateIdx = 0;
	BOOL bComXFound = FALSE;
	
	pMsg->MsgOut( dDBUG, _T("SGPSComClass::FindComX") );
	pMsg->MsgOut( dINFO, _T("Wait ... Try to find out GPS port !") );
	
	///we are try to find out the port with GPS NMEA data
	///is port number known ?

	while( !IsComNumberValid( m_dwComNum ) && (dwTryPortIdx < PORTNUMBERLISTS) )
	{
		if( bComXFound )
			break;

		///try open this port
		if( !TryOpenPortName( PortNumNameList[dwTryPortIdx].szPortName ) )
		{	
			dwTryPortIdx++;
			///Sleep( 5000 );
			continue;	///try next one if this port can not open
		}	
		hRet = m_hComPort;
		
		///try baudrate
		while( !IsBaudRateValid( m_dwBaudRate ) && (dwTryBaudrateIdx < BAUDRATELISTS) )
		{
			if( bComXFound )
				break;
				
			if( !InitCom( BaudRateList[dwTryBaudrateIdx] ) )
			{
				dwTryBaudrateIdx++;	
				///Sleep( 5000 );
				continue;	///try next one if this can not use
			}
			
			///find NMEA command
			if( NULL == m_hComReadThread )
			{	///create read thread
				m_hComReadThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ComReadThreadStub, this, 0, NULL );
			}
			
			///Sleep( 5000 );
			Sleep( 3000 );
			
			if( m_bIsNMEA )
			{	///we found the GPS port
				bComXFound = TRUE;
				m_dwComNum = dwTryPortIdx;
				m_dwBaudRate = BaudRateList[dwTryBaudrateIdx];
				pMsg->MsgOut( dINFO|dDBUG, _T("SGPSComClass::FindComX Found !! %s %d"), PortNumNameList[m_dwComNum].szPortName, m_dwBaudRate );
				Sleep( 3000 );
			}	
			
			CloseHandle( m_hComReadThread );			
			m_hComReadThread = NULL;
			m_bThreadRUN = FALSE;
			Sleep( 1000 );
			m_bThreadRUN = TRUE;
			dwTryBaudrateIdx++;	
		}

		CloseHandle( m_hComPort );
		m_hComPort = INVALID_HANDLE_VALUE;
		
		dwTryPortIdx++;
	}


	if( !bComXFound )
		hRet = NULL;
		
	return hRet;
}

BOOL SGPSComClass::InitMsg()
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

BOOL SGPSComClass::InitCom()
{
	BOOL bRet = FALSE;
	DCB PortDCB;
	DWORD dwError;
		
	///check if port ok
	if( !IsComReady() )
		return FALSE;

	SetCommMask( m_hComPort, EV_RXCHAR );
	SetupComm( m_hComPort, 512, 512 );
	PurgeComm( m_hComPort, PURGE_TXCLEAR|PURGE_RXCLEAR );

	PortDCB.DCBlength = sizeof(DCB); 

	GetCommState( m_hComPort, &PortDCB );

	PortDCB.BaudRate = m_dwBaudRate;
	PortDCB.fBinary = TRUE; 		///must be true
	PortDCB.fParity = TRUE; 		///parity check
	PortDCB.fOutxCtsFlow = TRUE;
	PortDCB.fOutxDsrFlow = FALSE;
	PortDCB.fDtrControl = DTR_CONTROL_ENABLE;
	PortDCB.fDsrSensitivity = FALSE;
	///PortDCB.fTXContinueOnXoff = TRUE;
	PortDCB.fTXContinueOnXoff = FALSE;
	PortDCB.fOutX = FALSE;
	PortDCB.fInX = FALSE;
	PortDCB.fErrorChar = FALSE;
	PortDCB.fNull = FALSE;
	PortDCB.fRtsControl = RTS_CONTROL_ENABLE;
	PortDCB.fAbortOnError = FALSE;
	PortDCB.ByteSize = 8;
	PortDCB.Parity = NOPARITY;
	PortDCB.StopBits = ONESTOPBIT;


	bRet = SetCommState( m_hComPort, &PortDCB );
	if( !bRet ) 
	{
		dwError = GetLastError();
	}
	
	pMsg->MsgOut( dDBUG, _T("SGPSComClass::InitCom %s %d"), bRet?L"Ok":L"Fail", dwError);

	m_bIsFWVersionOK = FALSE;
	m_bIsFWBootOK = FALSE;

	return bRet;
}

BOOL SGPSComClass::InitCom( DWORD dwTryBaudRate )
{
	BOOL bRet = FALSE;
	DCB PortDCB;
	DWORD dwError;
		
	///check if port ok
	if( !IsComReady() )
		return FALSE;


	///SetCommMask( m_hComPort, EV_RXCHAR );
	SetCommMask( m_hComPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_RING | EV_RXFLAG | EV_RLSD | EV_POWER | EV_ERR | EV_BREAK | EV_TXEMPTY );
	SetupComm( m_hComPort, 512, 512 );
	PurgeComm( m_hComPort, PURGE_TXCLEAR|PURGE_RXCLEAR );

	PortDCB.DCBlength = sizeof(DCB); 

	GetCommState( m_hComPort, &PortDCB );

	PortDCB.BaudRate = dwTryBaudRate;
	PortDCB.fBinary = TRUE; 		///must be true
	PortDCB.fParity = TRUE; 		///parity check
	PortDCB.fOutxCtsFlow = TRUE;
	PortDCB.fOutxDsrFlow = FALSE;
	PortDCB.fDtrControl = DTR_CONTROL_ENABLE;
	PortDCB.fDsrSensitivity = FALSE;
	///PortDCB.fTXContinueOnXoff = TRUE;
	PortDCB.fTXContinueOnXoff = FALSE;
	PortDCB.fOutX = FALSE;
	PortDCB.fInX = FALSE;
	PortDCB.fErrorChar = FALSE;
	PortDCB.fNull = FALSE;
	PortDCB.fRtsControl = RTS_CONTROL_ENABLE;
	PortDCB.fAbortOnError = FALSE;
	PortDCB.ByteSize = 8;
	PortDCB.Parity = NOPARITY;
	PortDCB.StopBits = ONESTOPBIT;


	bRet = SetCommState( m_hComPort, &PortDCB );
	if( !bRet ) 
	{
		dwError = GetLastError();
	}
	
	pMsg->MsgOut( dDBUG, _T("SGPSComClass::InitCom with %d %s %d"), dwTryBaudRate, bRet?L"Ok":L"Fail", dwError);
		
	m_bIsFWVersionOK = FALSE;		
	m_bIsFWBootOK = FALSE;
	
	return bRet;
}

BOOL SGPSComClass::CheckAnyNMEACode( UCHAR ucThis )
{
	BOOL bRet = FALSE;
	UCHAR CompareStr[] = { '$', 'G', 'P' };
	static DWORD dwRightChar = 0;

	///NKDbgPrintfW( TEXT("%c=%c"), ucThis, CompareStr[dwRightChar] );
	if( ucThis == CompareStr[dwRightChar] )
	{
		if( dwRightChar == 2 )
		{	
			dwRightChar = 0;
			bRet = TRUE;
		}	
		else	
			dwRightChar++;
		///NKDbgPrintfW( TEXT("[%c]"), ucThis );
	}	
	else
		dwRightChar = 0;

	return bRet;
}

BOOL SGPSComClass::CheckBootCode( UCHAR ucThis )
{
	BOOL bRet = FALSE;
	UCHAR CompareStr[] = { 'c', 'l', 'o', 'c', 'k', ':' };
	static DWORD dwRightChar = 0;

	///NKDbgPrintfW( TEXT("%c=%c"), ucThis, CompareStr[dwRightChar] );
	if( ucThis == CompareStr[dwRightChar] )
	{
		if( dwRightChar == 5 )
		{	
			dwRightChar = 0;
			bRet = TRUE;
		}	
		else	
			dwRightChar++;
		///NKDbgPrintfW( TEXT("[%c]"), ucThis );
	}	
	else
		dwRightChar = 0;

	return bRet;
}

DWORD SGPSComClass::ComReadThread( LPVOID lpParam )
{
	DWORD dwRet = 0;

	DWORD dwCommModemStatus;
	DWORD dwComEvent;
	DWORD dwQueLen;
	DWORD dwRetLen;
	COMSTAT ComStat;
	DWORD dwErrorFlags;

	pMsg->MsgOut( dDBUG|dINIT, _T("SGPSComClass::ComReadThread ") );	

	while( m_hComPort != INVALID_HANDLE_VALUE && m_bThreadRUN )
	{
		/// Wait for an event to occur for the port
		WaitCommEvent( m_hComPort, &dwComEvent, 0 );	///wait for Rx
		
		pMsg->MsgOut( dDBUG, _T("SGPSComClass::WaitCommEvent =0x%08x "), dwComEvent );
		
		/// Re-specify the set of events to be monitored for the port.
		///SetCommMask( m_hComPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_RING );
		///SetCommMask( m_hComPort, EV_RXCHAR );
		SetCommMask( m_hComPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_RING | EV_RXFLAG | EV_RLSD | EV_POWER | EV_ERR | EV_BREAK | EV_TXEMPTY );

		if( dwComEvent & EV_RXCHAR ) 
		{
			///Get the comp port status
			ClearCommError( m_hComPort, &dwErrorFlags, &ComStat );

			dwQueLen = ComStat.cbInQue;

			if( dwQueLen > 0 )
			{
				UCHAR buf[512];
				
				if( !ReadFile( m_hComPort, buf, dwQueLen, &dwRetLen , NULL ) )
				{
					pMsg->MsgOut( dFAIL, _T("SGPSComClass::ComReadThread ReadFile %s "), L"Fail" );
				}
				else
				{
					WCHAR wBuf[512];
					
					pMsg->MsgOut( dDBUG, _T("SGPSComClass::ComReadThread ReadFile %d bytes"), dwQueLen );

					for( DWORD i = 0; i < dwQueLen; i++ )
					{
						wBuf[i] = (WCHAR)buf[i];
#if 0						
						///check NMEA code
						if( (buf[i] == '$') && (buf[i+1] == 'G') && (buf[i+2] == 'P') )
							m_bIsNMEA = TRUE;
						///check boot code
						///if( (buf[i] == '$') && (buf[i+1] == 'P') && (buf[i+2] == 'S') && (buf[i+3] == 'R') && (buf[i+4] == 'F')&& (buf[i+5] == 'T'))
						if( (buf[i] == 'c') && (buf[i+1] == 'l') && (buf[i+2] == 'o') && (buf[i+3] == 'c') && (buf[i+4] == 'k')&& (buf[i+5] == ':'))
							m_bIsFWBootOK = TRUE;
#else
						if( CheckAnyNMEACode( buf[i] ) )
							m_bIsNMEA = TRUE;
						if( CheckBootCode( buf[i] ) )
							m_bIsFWBootOK = TRUE;

#endif							
							
					}
					wBuf[i++] = '\0';
					pMsg->MsgOut( dINFO, _T("%s"), wBuf );
					///NKDbgPrintfW( TEXT("%s\r\n"), wBuf );
				}
			}
			else
			{
				pMsg->MsgOut( dWARN, _T("SGPSComClass::ComReadThread ReadFile %d bytes"), dwQueLen );
			}
		}
		else
			pMsg->MsgOut( dWARN, _T("SGPSComClass::ComReadThread Comm event 0x%08x"), dwComEvent );
		
		GetCommModemStatus( m_hComPort, &dwCommModemStatus );
	}///while()

	pMsg->MsgOut( dFAIL, _T("SGPSComClass::ComReadThread exit") );
	
	m_bIsNMEA = FALSE;
	
	return dwRet;
}
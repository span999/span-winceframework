// BattTestClass.h : interface of the Battery Test class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __SPCOMXCLASS_H__
#define __SPCOMXCLASS_H__

#include "TestClass.h"


typedef struct
{
	UINT 	uiPortNumber;		///port number 0~15
	WCHAR	szPortName[8];
}PortNumIndex, *PPortNumIndex;

#define PORTNUMBERLISTS		16

static PortNumIndex PortNumNameList[PORTNUMBERLISTS] =
{
	{  0, _T("COM0:") },
	{  1, _T("COM1:") },
	{  2, _T("COM2:") },
	{  3, _T("COM3:") },
	{  4, _T("COM4:") },
	{  5, _T("COM5:") },
	{  6, _T("COM6:") },
	{  7, _T("COM7:") },
	{  8, _T("COM8:") },
	{  9, _T("COM9:") },
	{ 10, _T("COM10:") },
	{ 11, _T("COM11:") },
	{ 12, _T("COM12:") },
	{ 13, _T("COM13:") },
	{ 14, _T("COM14:") },
	{ 15, _T("COM15:") }
};

#define BAUDRATELISTS		8

static DWORD BaudRateList[BAUDRATELISTS] = 
{
	1200,
	4800,
	9600,
	14400,
	19200,
	38400,
	57600,
	115200
};



/////////////////////////////////////////////////////////////////////////////
/// SComXClass
/*
SComXClass()
*/

class SComXClass
{
/// Function Member
	/// Overrides
public:
protected:
	///find com port in your way
	virtual HANDLE FindComX();
	///init Message	
	virtual BOOL InitMsg();
	///Com port init
	virtual BOOL InitCom();
	///Com port init
	virtual BOOL InitCom( DWORD dwTryBaudRate );
	///read thread
	virtual DWORD ComReadThread( LPVOID lpParam );
	
	/// Implementation 	/// interface
public:
	///de-constructor
	~SComXClass();
	///constructor
	SComXClass();
	///constructor with specified Com number
	SComXClass( DWORD dwComNum );
	///
	DWORD SetDebugLevel( DWORD dwLevel );
	///
	BOOL SetFrameWindowHwnd( HWND hWndIn );
	///
	BOOL SetShowAreaRect( RECT rtIn );
	///set baudrate
	BOOL SetBaudRate( DWORD dwBaudRate );
	///set Com number
	BOOL SetComNumber( DWORD dwComNum );
	///enable Com port
	BOOL Enable();
	///disable Com port
	BOOL Disable();
	///send data
	BOOL ComSend( PBYTE pData, DWORD dwDataLength );
	///receive data
	BOOL ComReceive( PBYTE *ppData, DWORD *pdwDataLength );
	///set the event handle to reactive
	void SetInterruptEvent( HANDLE hEvent );
	
	
	/// Implementation 	/// Operations	
protected:
	///basic init
	void Init();
	///
	BOOL IsBaudRateValid( DWORD dwBaudrate );
	///
	BOOL IsComNumberValid( DWORD dwComNum );
	///try to open the port by name
	BOOL TryOpenPortName( LPTSTR lpszPortName );
	///check if Com ready
	BOOL IsComReady(){ return( (m_hComPort == INVALID_HANDLE_VALUE)?FALSE:TRUE ); }
	///stub of read thread
	static DWORD WINAPI ComReadThreadStub( LPVOID lpParam );

/// Data Member
public:
protected:
	DWORD m_dwDebugLevel;			///Debug level index		
	SShowMsgClass *pMsg;
	HWND m_hWndFrameWindow;
	RECT m_rtShowArea;
	HANDLE m_hComPort;
	DWORD m_dwComNum;
	DWORD m_dwBaudRate;
	HANDLE m_hComReadThread;
	HANDLE m_hComReadThreadEvent;
	BOOL m_bThreadRUN;	
	///BOOL m_bIsNMEA;
};





/////////////////////////////////////////////////////////////////////////////
/// SGPSComClass
/*
SGPSComClass()
*/


class SGPSComClass : public SComXClass
{
/// Function Member
	/// Overrides
public:
protected:
	///find com port in your way
	virtual HANDLE FindComX();
	///init Message	
	virtual BOOL InitMsg();
	///Com port init
	virtual BOOL InitCom();
	///Com port init
	virtual BOOL InitCom( DWORD dwTryBaudRate );
	///read thread
	virtual DWORD ComReadThread( LPVOID lpParam );
	
	/// Implementation 	/// interface
public:
	///de-constructor
	~SGPSComClass();
	///constructor
	SGPSComClass();
	///constructor with specified Com number
	SGPSComClass( DWORD dwComNum );
	
	///if got version
	BOOL IsFWVersionOK(){ return m_bIsFWVersionOK; }
	///
	void GetFWVersion(){ ; }
	///if got version
	BOOL IsFWBootOK(){ return m_bIsFWBootOK; }
	///check NMEA code
	BOOL CheckAnyNMEACode( UCHAR ucThis );
	///check boot code
	BOOL CheckBootCode( UCHAR ucThis );

	/// Implementation 	/// Operations
protected:

/// Data Member
public:
protected:
	BOOL m_bIsNMEA;
	BOOL m_bIsFWVersionOK;
	BOOL m_bIsFWBootOK;	
};

/////////////////////////////////////////////////////////////////////////////


#endif ///#ifndef __SPCOMXCLASS_H__

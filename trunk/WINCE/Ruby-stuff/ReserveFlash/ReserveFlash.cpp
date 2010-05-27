// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ReserveFlash.h"
#include <commctrl.h>
#include <winuserm.h>
///#include <dbt.h>
#if 0
#include "ioctl.h"
#else
#include "Thales_defines.h"
#endif
#include "rFC_ioctl.h"
///#include "Virtual_API.h"
#include "winioctl.h"
#include "DiskIO.h"
#include "PM.h"
#include "Notify.h"
#include "CODEC_Lib.h"
#include "EQ_Dlg.h"
#include "Audio_Dlg.h"
#include "Volume_Dlg.h"
#include "SWvolume_Dlg.h"
#include "Touch_Dlg.h"
#include "About_Dlg.h"
#include "HWVersion_Dlg.h"
#include "Vdbg_Dlg.h"
#include "PerformanceCount_Dlg.h"
#include "Echo_Dlg.h"
#include "Backlight_Dlg.h"
#include "BT_Dlg.h"
#include "AV_Dlg.h"


///extern "C" BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);
extern "C" LPVOID MapPtrToProcess ( LPVOID lpv, HANDLE hProc );

#define IOCTL_HAL_REBOOT		CTL_CODE(FILE_DEVICE_HAL, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define NAMED_EVENT_HDD_COMMAND		TEXT("named event hdd command")

///#define OEM_CELOG_FILLEVENT_NAME		TEXT("SYSTEM/CeLog Fill")
#define OEM_CELOG_FILLEVENT_NAME		TEXT("SYSTEM/OSCaptureTrigger")

/*
typedef struct {
    DWORD PortContext;
    DWORD PortID;
} PORT_IN_COMBO;
*/


#define MAX_LOADSTRING 100

#define TEST_SIZE_BYTE			(1024*1024*5)
///#define HDD_PATH_TEST_FILE		_T("\\HDD4\\Test\\Test.tmp")
///#define HDD_PATH_TEST_FILE1		_T("\\HDD4\\Test\\Test1.tmp")

#define HDD_PATH_TEST_FILE		_T("\\HDD2\\Test.tmp")
#define HDD_PATH_TEST_FILE1		_T("\\HDD2\\Test1.tmp")


///#define HDD_PATH_TEST_FILE		_T("\\Hard Disk4\\Test\\Test.tmp")
///#define HDD_PATH_TEST_FILE1		_T("\\Hard Disk4\\Test\\Test1.tmp")


#define SDMMC_PATH_TEST_FILE		_T("\\SDMMC Card\\Test.tmp")
#define SDMMC_PATH_TEST_FILE1		_T("\\SDMMC Card\\Test1.tmp")


// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle

///WNDPROC				gfMsgProc;
DWORD				dwBaudSelet = 0; 
DWORD				dwComSelet = 0;
DWORD				dwHDDCommand = 0;
BOOL				bHDDCmdRun = FALSE;
HANDLE				hComThread = NULL;
HANDLE				hAutoSuspendThread = NULL;
HANDLE				hHDDCommandThread = NULL;
HANDLE				hHDDCommandEvent = NULL;
HANDLE				ghComPort = NULL;
BOOL				bPortOpened = FALSE;
BOOL				bScreenPause = FALSE;
TCHAR				tcByte;
BOOL				bForcePortClose = FALSE;
BOOL				gbAutoSuspend = FALSE;
BOOL				gbThreadTest = FALSE;

///HANDLE				hVDBGThread = NULL;
///HANDLE				ghVDBGPort = NULL;
///BOOL				bVDBGPortOpened = FALSE;
///BOOL				bVDBGForcePortClose = FALSE;
///BOOL				bVDBGScreenPause = FALSE;
///DWORD				dwVPortSelet = 0;
///static PORT_IN_COMBO PortInCombo[MAX_PORT_LIST];
static DWORD dwSECTOR_PER_CHUNK[10] = {2,4,8,16,32,64,128,256,512,1024};


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
///LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);
///LRESULT CALLBACK	HWVersion		(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	FlashContent	(HWND, UINT, WPARAM, LPARAM);
///LRESULT CALLBACK	Touch			(HWND, UINT, WPARAM, LPARAM);
///LRESULT CALLBACK	Backlight		(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	HardDrive		(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	SDMMC			(HWND, UINT, WPARAM, LPARAM);
///LRESULT CALLBACK	ECHO			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Gps				(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK   	EditBoxSubProc	(HWND, UINT, WPARAM, LPARAM);
///LRESULT CALLBACK	Vdbg			(HWND, UINT, WPARAM, LPARAM);
///LRESULT CALLBACK 	PerformanceCount(HWND, UINT, WPARAM, LPARAM);
///LRESULT CALLBACK 	AUDIO           (HWND, UINT, WPARAM, LPARAM);
///LRESULT CALLBACK 	Volume          (HWND, UINT, WPARAM, LPARAM);
///LRESULT CALLBACK 	EQ		        (HWND, UINT, WPARAM, LPARAM);

///void 				BACKLIGHT_AUTO	( HWND );
///void 				BACKLIGHT_AUTO2	( HWND );
///void				BACKLIGHT_SETTING( HWND hDlg , DWORD dwSelect);
BOOL 				HDD_COPY (DWORD *);
BOOL 				HDD_READ (DWORD *);
BOOL 				HDD_READ2 (DWORD *);
BOOL 				HDD_WRITE2 (DWORD *, BOOL );
BOOL 				HDD_ON_OFF ( BOOL );
BOOL 				SDMMC_COPY (DWORD *);
BOOL 				SDMMC_READ (DWORD *);
BOOL 				SDMMC_READ2 (DWORD *);
BOOL 				SDMMC_WRITE2 (DWORD *, BOOL );
BOOL 				SDMMC_ON_OFF ( BOOL );




int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_RESERVEFLASH);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    It is important to call this function so that the application 
//    will get 'well formed' small icons associated with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS	wc;

    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= (WNDPROC) WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RESERVEFLASH));
    wc.hCursor			= 0;
    wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName		= 0;
    wc.lpszClassName	= szWindowClass;

	return RegisterClass(&wc);
}

//
//  FUNCTION: InitInstance(HANDLE, int)
//
//  PURPOSE: Saves instance handle and creates main window
//
//  COMMENTS:
//
//    In this function, we save the instance handle in a global variable and
//    create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND	hWnd;
	TCHAR	szTitle[MAX_LOADSTRING];			// The title bar text
	TCHAR	szWindowClass[MAX_LOADSTRING];		// The window class name

	hInst = hInstance;		// Store instance handle in our global variable
	// Initialize global strings
	LoadString(hInstance, IDC_RESERVEFLASH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	if (hwndCB)
		CommandBar_Show(hwndCB, TRUE);

	return TRUE;
}

#if 0
//
//
//
//
//
//
WCHAR* spCheckBattery(void)
{
	WCHAR *szBattery = L"??????????????\0\0";	///30
	DWORD dwStatus, dwType, dwSize;
	DWORD dwError;
	HKEY hKey;
	
	///Get the registry for the battery drvier.
	///SPMPMINFO( L"!!SPD@spCheckBattery():change the Battery driver~~\r\n" );	 
	dwError = RegOpenKeyEx( HKEY_LOCAL_MACHINE, TEXT("Drivers\\BuiltIn\\Battery"), 0, 0, &hKey);
	if((ERROR_SUCCESS == dwError) && hKey)
	{
       	dwSize = 30;
       	dwStatus = RegQueryValueEx(hKey, TEXT("Dll"), NULL, &dwType, (LPBYTE)szBattery, &dwSize);
       	if(dwStatus == ERROR_SUCCESS && dwType == REG_SZ) {
   	        NKDbgPrintfW( L"SPD@spCheckBattery() : %s.\r\n", szBattery);
       	}
       	else
       	{
       		///dwHWver2A = 0x00000036;
   	        NKDbgPrintfW( L"SPD@spCheckBattery() : Fail !!.\r\n");
       	}
		
       	// release resources
       	RegCloseKey(hKey);  					 
	}
	else
		NKDbgPrintfW( L"!!SPD@spCheckBattery:RegOpenKeyEx failed Error=%08X\r\n", GetLastError() );
	
	return szBattery; 
}
#endif



void ParseCommMask(DWORD *pdwMask)
{
	if(pdwMask)
	{
		NKDbgPrintfW( L"!!SPD@ParseCommMask: " );

		if( (*pdwMask & EV_BREAK) )
			NKDbgPrintfW( L"EV_BREAK " );		
		if( (*pdwMask & EV_CTS) )
			NKDbgPrintfW( L"EV_CTS " );		
		if( (*pdwMask & EV_DSR) )
			NKDbgPrintfW( L"EV_DSR " );		
		if( (*pdwMask & EV_ERR) )
			NKDbgPrintfW( L"EV_ERR " );		
		if( (*pdwMask & EV_RING) )
			NKDbgPrintfW( L"EV_RING " );		
		if( (*pdwMask & EV_RLSD) )
			NKDbgPrintfW( L"EV_RLSD " );		
		if( (*pdwMask & EV_RXCHAR) )
			NKDbgPrintfW( L"EV_RXCHAR " );		
		if( (*pdwMask & EV_RXFLAG) )
			NKDbgPrintfW( L"EV_RXFLAG " );		
		if( (*pdwMask & EV_TXEMPTY) )
			NKDbgPrintfW( L"EV_TXEMPTY " );		

		NKDbgPrintfW( L"\r\n" );
	}
	else
	{
		NKDbgPrintfW( L"!!SPD@ParseCommMask: failed !! pdwMask=NULL\r\n" );
	}	
}


void ParseDCB(DCB *pDCB)
{
	if(pDCB)
	{
		NKDbgPrintfW( L"!!SPD@ParseDCB: DCBlength=%d(0x%x)\r\n", pDCB->DCBlength, pDCB->DCBlength );
		NKDbgPrintfW( L"!!SPD@ParseDCB: BaudRate=%d(0x%x)\r\n", pDCB->BaudRate, pDCB->BaudRate );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fBinary=%d(0x%x)\r\n", pDCB->fBinary, pDCB->fBinary );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fParity=%d(0x%x)\r\n", pDCB->fParity, pDCB->fParity );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fOutxCtsFlow=%d(0x%x)\r\n", pDCB->fOutxCtsFlow, pDCB->fOutxCtsFlow );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fOutxDsrFlow=%d(0x%x)\r\n", pDCB->fOutxDsrFlow, pDCB->fOutxDsrFlow );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fDtrControl=%d(0x%x)\r\n", pDCB->fDtrControl, pDCB->fDtrControl );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fDsrSensitivity=%d(0x%x)\r\n", pDCB->fDsrSensitivity, pDCB->fDsrSensitivity );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fTXContinueOnXoff=%d(0x%x)\r\n", pDCB->fTXContinueOnXoff, pDCB->fTXContinueOnXoff );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fOutX=%d(0x%x)\r\n", pDCB->fOutX, pDCB->fOutX );
		NKDbgPrintfW( L"!!SPD@ParseDCB: fInX=%d(0x%x)\r\n", pDCB->fInX, pDCB->fInX );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fErrorChar=%d(0x%x)\r\n", pDCB->fErrorChar, pDCB->fErrorChar );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fNull=%d(0x%x)\r\n", pDCB->fNull, pDCB->fNull );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fRtsControl=%d(0x%x)\r\n", pDCB->fRtsControl, pDCB->fRtsControl );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fAbortOnError=%d(0x%x)\r\n", pDCB->fAbortOnError, pDCB->fAbortOnError );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: fDummy2=%d(0x%x)\r\n", pDCB->fDummy2, pDCB->fDummy2 );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: wReserved=%d(0x%x)\r\n", pDCB->wReserved, pDCB->wReserved );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: XonLim=%d(0x%x)\r\n", pDCB->XonLim, pDCB->XonLim );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: XoffLim=%d(0x%x)\r\n", pDCB->XoffLim, pDCB->XoffLim );
		NKDbgPrintfW( L"!!SPD@ParseDCB: ByteSize=%d(0x%x)\r\n", pDCB->ByteSize, pDCB->ByteSize );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: Parity=%d(0x%x)\r\n", pDCB->Parity, pDCB->Parity );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: StopBits=%d(0x%x)\r\n", pDCB->StopBits, pDCB->StopBits );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: XonChar=%d(0x%x)\r\n", pDCB->XonChar, pDCB->XonChar );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: XoffChar=%d(0x%x)\r\n", pDCB->XoffChar, pDCB->XoffChar );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: ErrorChar=%d(0x%x)\r\n", pDCB->ErrorChar, pDCB->ErrorChar );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: EofChar=%d(0x%x)\r\n", pDCB->EofChar, pDCB->EofChar );		
		NKDbgPrintfW( L"!!SPD@ParseDCB: EvtChar=%d(0x%x)\r\n", pDCB->EvtChar, pDCB->EvtChar );
		NKDbgPrintfW( L"!!SPD@ParseDCB: wReserved1=%d(0x%x)\r\n", pDCB->wReserved1, pDCB->wReserved1 );
	}
	else
	{
		NKDbgPrintfW( L"!!SPD@ParseDCB: failed !! pDCB=NULL\r\n" );
	}	
}


void ParseCommProperties(COMMPROP *pCOMMPROP)
{
	if(pCOMMPROP)
	{
		NKDbgPrintfW( L"!!SPD@ParseCommProp: wPacketLength=%d(0x%x)\r\n", pCOMMPROP->wPacketLength, pCOMMPROP->wPacketLength );
		NKDbgPrintfW( L"!!SPD@ParseCommProp: wPacketVersion=%d(0x%x)\r\n", pCOMMPROP->wPacketVersion, pCOMMPROP->wPacketVersion );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwServiceMask=%d(0x%x)\r\n", pCOMMPROP->dwServiceMask, pCOMMPROP->dwServiceMask );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwReserved1=%d(0x%x)\r\n", pCOMMPROP->dwReserved1, pCOMMPROP->dwReserved1 );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwMaxTxQueue=%d(0x%x)\r\n", pCOMMPROP->dwMaxTxQueue, pCOMMPROP->dwMaxTxQueue );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwMaxRxQueue=%d(0x%x)\r\n", pCOMMPROP->dwMaxRxQueue, pCOMMPROP->dwMaxRxQueue );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwMaxBaud=%d(0x%x)\r\n", pCOMMPROP->dwMaxBaud, pCOMMPROP->dwMaxBaud );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwProvSubType=%d(0x%x)\r\n", pCOMMPROP->dwProvSubType, pCOMMPROP->dwProvSubType );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwProvCapabilities=%d(0x%x)\r\n", pCOMMPROP->dwProvCapabilities, pCOMMPROP->dwProvCapabilities );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwSettableParams=%d(0x%x)\r\n", pCOMMPROP->dwSettableParams, pCOMMPROP->dwSettableParams );
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwSettableBaud=%d(0x%x)\r\n", pCOMMPROP->dwSettableBaud, pCOMMPROP->dwSettableBaud );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: wSettableData=%d(0x%x)\r\n", pCOMMPROP->wSettableData, pCOMMPROP->wSettableData );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: wSettableStopParity=%d(0x%x)\r\n", pCOMMPROP->wSettableStopParity, pCOMMPROP->wSettableStopParity );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwCurrentTxQueue=%d(0x%x)\r\n", pCOMMPROP->dwCurrentTxQueue, pCOMMPROP->dwCurrentTxQueue );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwCurrentRxQueue=%d(0x%x)\r\n", pCOMMPROP->dwCurrentRxQueue, pCOMMPROP->dwCurrentRxQueue );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwProvSpec1=%d(0x%x)\r\n", pCOMMPROP->dwProvSpec1, pCOMMPROP->dwProvSpec1 );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: dwProvSpec2=%d(0x%x)\r\n", pCOMMPROP->dwProvSpec2, pCOMMPROP->dwProvSpec2 );		
		NKDbgPrintfW( L"!!SPD@ParseCommProp: wcProvChar=%d(0x%x)\r\n", pCOMMPROP->wcProvChar, pCOMMPROP->wcProvChar );		
	}
	else
	{
		NKDbgPrintfW( L"!!SPD@ParseCommProp: failed !! pCOMMPROP=NULL\r\n" );
	}	
}


static HRESULT ScheduleRunApp(
	LPCTSTR szExeName,
	LPCTSTR szArgs,
	///DWORD dwAfterSeconds
	USHORT usAfterSeconds
)
{
	HRESULT hr = S_OK;
	HANDLE hNotify = NULL;
  
	// set a CE_NOTIFICATION_TRIGGER
	CE_NOTIFICATION_TRIGGER notifTrigger;
	memset(&notifTrigger, 0, sizeof(CE_NOTIFICATION_TRIGGER));
	notifTrigger.dwSize = sizeof(CE_NOTIFICATION_TRIGGER);
  
	// calculate time
	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	
	st.wSecond += usAfterSeconds;	/// seconds later
///	hr = SecondsAfter(&st, dwAfterSeconds);
///	if (FAILED(hr)) {
///		return hr;
///	}
  
	notifTrigger.dwType = CNT_TIME;
	notifTrigger.stStartTime = st;
  
	// timer: execute an exe at specified time
	notifTrigger.lpszApplication = (LPTSTR)szExeName;
	notifTrigger.lpszArguments = (LPTSTR)szArgs;
  
	hNotify = CeSetUserNotificationEx(0, &notifTrigger, NULL);
	
	// NULL because we do not care the action
	if (!hNotify) {
		// close the handle as we do not need to use it further
		CloseHandle(hNotify);
		hr = E_FAIL;
	}

	return hr;
}


void WINAPI AutoSuspendThread()
{
	Sleep(10000);
	while(1)
	{
		if( gbAutoSuspend )
		{	
			///set alarm five seconds later
			ScheduleRunApp( NULL, NULL, 5);
			///ScheduleRunApp( _T("explorer.exe"), NULL, 5);
			///set system suspend
			SetSystemPowerState( NULL, POWER_STATE_SUSPEND, POWER_FORCE );
		}
		///wait 5 seconds
		Sleep(10000);
	}
}


void WINAPI ThreadTestThread()
{
	DWORD dwTest = 0;
	
	Sleep(3000);
	while(1)
	{
		if( gbThreadTest )
		{	
			dwTest = (((3214^9534)/5974312)^(4000+485));
		}
		else
		{
			break;
		}	
		///wait 5 seconds
		Sleep(0);
	}
	
}


/*******************************************************************************
		AccessComPort(void)
********************************************************************************/
void WINAPI AccessComPort(HWND hwnd)
{
	BYTE Byte;
	DWORD dwBytesTransferred;
	HANDLE hTestComm = INVALID_HANDLE_VALUE;
	DWORD dwCommModemStatus;
	DCB dcb;
	COMMTIMEOUTS CommTimeouts;
	WCHAR *szComPort = L"COM?:\0\0";	///12
	DWORD dwSetting[9] = {CBR_1200,CBR_2400,CBR_4800,CBR_9600,CBR_19200,CBR_38400,CBR_57600,CBR_115200,921600};
	TCHAR szString[128];         // Temperary string
	DWORD dwLoop = 0;
	DWORD dwTemp = 0;
	COMMPROP ComProp;
	

	NKDbgPrintfW( TEXT("\r\nSPD@AccessComPort():start !! \r\n") );
	///MessageBox(hwnd, L"Com Port Already Open !!", L"Error", MB_OK);
	
	// Open CommPort
	wsprintf (szComPort, TEXT("COM%d:"), (dwComSelet+1) );
	hTestComm = CreateFile ( szComPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if ( INVALID_HANDLE_VALUE == hTestComm )
	{
		NKDbgPrintfW( TEXT("SPD@AccessComPort():start CreateFile Fail !! \r\n") );
		MessageBox(hwnd, L"Com Port Open failed!!", L"Error", MB_OK);
		return;
	}
	else
		NKDbgPrintfW( L"SPD@AccessComPort():start CreateFile %s !! \r\n", szComPort );
	
	ghComPort = hTestComm;
	/// set flag port open
	bPortOpened = TRUE;

	///config COM port
	memset(&dcb,0,sizeof(dcb));

	dcb.DCBlength = sizeof (DCB);	
	dcb.BaudRate = dwSetting[dwBaudSelet];
    dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fTXContinueOnXoff = TRUE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fAbortOnError = FALSE;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

	SetCommState(hTestComm, &dcb);
	///span debug
	memset(&dcb,0,sizeof(dcb));
	GetCommState(hTestComm, &dcb);
	ParseDCB(&dcb);
	///span debug
	GetCommTimeouts (hTestComm, &CommTimeouts);

	// Change the COMMTIMEOUTS structure settings.
	CommTimeouts.ReadIntervalTimeout = MAXDWORD;  
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;  
	CommTimeouts.ReadTotalTimeoutConstant = 0;    
	CommTimeouts.WriteTotalTimeoutMultiplier = 10;  
	CommTimeouts.WriteTotalTimeoutConstant = 1000;    
	
	// Set the time-out parameters for all read and write operations
	// on the port. 
	if (!SetCommTimeouts (hTestComm, &CommTimeouts))
	{
		// Could not set the time-out parameters.
		NKDbgPrintfW( TEXT("Unable to set the time-out parameters.\r\n") );
		MessageBox(hwnd, L"Unable to set the time-out parameters!!", L"Error", MB_OK);
	}

	// Specify a set of events to be monitored for the port.
	SetCommMask (hTestComm, EV_RXCHAR | EV_RLSD | EV_RXFLAG |EV_ERR);

	///span debug
	dwTemp = 0;
	GetCommMask(hTestComm, &dwTemp);
	ParseCommMask(&dwTemp);
	///span debug

	///span debug
	memset(&ComProp,0,sizeof(ComProp));	
	GetCommProperties(hTestComm, &ComProp);
	ParseCommProperties(&ComProp);
	///span debug

	while (hTestComm != INVALID_HANDLE_VALUE) 
	{
		// Wait for an event to occur for the port.
		WaitCommEvent (hTestComm, &dwCommModemStatus, 0);
		
///		NKDbgPrintfW( TEXT("\r\nComEvent: ") );
		NKDbgPrintfW( TEXT("\r\n>>>") );
		
		// Re-specify the set of events to be monitored for the port.
		SetCommMask (hTestComm, EV_RXCHAR | EV_RLSD | EV_RXFLAG | EV_ERR);

		if (dwCommModemStatus & EV_RXCHAR) 
		{
			int iCount = 0;
			// Loop for waiting for the data.
			do 
			{
				
				// Read the data from the serial port.
				ReadFile (hTestComm, &Byte, 1, &dwBytesTransferred, NULL);

				// Display the data read.
				if (dwBytesTransferred == 1)
				{
					if( Byte == '$')
					{
						/// show to screen !!
						if( !bScreenPause )
						{	
							///Sleep(1);///ShowToScreen(hwnd);
							///wsprintf (szString, TEXT("%c"), tcByte);
							szString[dwLoop++] = '\0';
							///szString[dwLoop++] = '\0';
							SendDlgItemMessage(hwnd, IDC_GPS_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							DWORD dwIndex = SendDlgItemMessage(hwnd, IDC_GPS_LIST1, LB_GETCOUNT, 0, 0);
							
							SendDlgItemMessage(hwnd, IDC_GPS_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
						}	

						dwLoop = 0;	
						NKDbgPrintfW(TEXT("\r\n"));
					}	
						
					NKDbgPrintfW( TEXT("%c"), Byte);				
					tcByte = 0x00ff&(TCHAR)Byte;
					szString[dwLoop] = 0x00ff&(TCHAR)Byte;
					dwLoop++;
					
				}
				Sleep(0);
				iCount++;
			} while (dwBytesTransferred == 1);		/// read until no data in it !!
			NKDbgPrintfW( TEXT("(Rx%d)"), iCount );
		}
		if (dwCommModemStatus & EV_RLSD) 
		{
			NKDbgPrintfW( TEXT("SPD@AccessComPort(): EV_RLSD event !! \r\n") );
		}
		if (dwCommModemStatus & EV_RXFLAG) 
		{
			NKDbgPrintfW( TEXT("SPD@AccessComPort(): EV_RLSD event !! \r\n") );
			ReadFile (hTestComm, &Byte, 1, &dwBytesTransferred, 0);
				if (dwBytesTransferred == 1)
					NKDbgPrintfW( TEXT("\r\n:%c.\r\n"), Byte);

		}
		if (dwCommModemStatus & EV_ERR) 
		{
			NKDbgPrintfW( TEXT("SPD@AccessComPort(): EV_ERR event !! \r\n") );
			
		}

		if ( TRUE == bForcePortClose )	///force close ComPort
			break;

	}/// end of while()

	CloseHandle(hTestComm);
	bPortOpened = FALSE;

}



//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	TCHAR szHello[MAX_LOADSTRING];
	HANDLE hCELOGtrigger = NULL;
	
	if( hCELOGtrigger == NULL )
		hCELOGtrigger = CreateEvent(NULL, FALSE, FALSE, OEM_CELOG_FILLEVENT_NAME);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_HELP_ABOUT:
				   ///DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   AboutDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_HW_VERSION:
				   ///DialogBox(hInst, (LPCTSTR)IDD_HWVERSIONBOX, hWnd, (DLGPROC)HWVersion);
				   HWVersionDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_OSCAPTURE_RUN:
				   CreateProcess(_T("\\Windows\\OSCapture.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
				   NKDbgPrintfW( TEXT("SPD@WndProc():start OSCapture !! \r\n") );
				   break;
				case IDM_TRIGGER_OSCAPTURE:
				   if( hCELOGtrigger != NULL )	
				   		SetEvent(hCELOGtrigger);
				   NKDbgPrintfW( TEXT("SPD@WndProc():trigger OSCapture !! hCELOGtrigger=0x%08x\r\n"), hCELOGtrigger );
				   break;
				case IDM_THREAD_TEST:
					if( hwndCB )
					{	
						HMENU	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
						///check if it's checked
						if( gbThreadTest )
						{
							gbThreadTest = FALSE;
							///unchecked the menu item
							CheckMenuItem( hMenuBar, IDM_THREAD_TEST, MF_UNCHECKED );
						}
						else
						{
							int iLoop = 1000;
							gbThreadTest = TRUE;
																				
							///checked the menu item
							CheckMenuItem( hMenuBar, IDM_THREAD_TEST, MF_CHECKED );
	
							while(iLoop>0)
							{
								hAutoSuspendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadTestThread, (LPVOID)NULL, 0, NULL );
								if ( !hAutoSuspendThread )
									;
								else
									CloseHandle(hAutoSuspendThread);
								iLoop--;
							}		
						}
					}	
				   break;   				   
				case IDM_AUTO_SUSPEND:
					if( hwndCB )
					{	
						HMENU	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
						HANDLE hMPM = INVALID_HANDLE_VALUE;		/// span0111
						hMPM = CreateFile( _T("MPM1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
						DWORD dwAuto = 0;

						///check if it's checked
						if( gbAutoSuspend )
						{
							gbAutoSuspend = FALSE;
							///unchecked the menu item
							CheckMenuItem( hMenuBar, IDM_AUTO_SUSPEND, MF_UNCHECKED );
							///info system driver
							dwAuto = 4321;
							if( hMPM && !DeviceIoControl(hMPM, IOCTL_OEM_IBOOT_RESET_DEVICE, &dwAuto, sizeof(DWORD), NULL, 0, NULL, NULL) )
							{
				    			///SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("IOCTL_HAL_REBOOT Fail !!")); 
							}
							
						}
						else
						{
							gbAutoSuspend = TRUE;
							///checked the menu item
							CheckMenuItem( hMenuBar, IDM_AUTO_SUSPEND, MF_CHECKED );
							///info system driver
							dwAuto = 1234;
							if( hMPM && !DeviceIoControl(hMPM, IOCTL_OEM_IBOOT_RESET_DEVICE, &dwAuto, sizeof(DWORD), NULL, 0, NULL, NULL) )
							{
				    			///SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("IOCTL_HAL_REBOOT Fail !!")); 
							}

						}
						///if autosuspend thread is not run
						if( NULL == hAutoSuspendThread )
						{
							hAutoSuspendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AutoSuspendThread, (LPVOID)NULL, 0, NULL );
							if ( !hAutoSuspendThread )
								;
							else
								CloseHandle(hAutoSuspendThread);							
						}	
					}			
				   break;				  
				case IDM_FLASH_CONTENT:
				   DialogBox(hInst, (LPCTSTR)IDD_FLASHCONTENTBOX, hWnd, (DLGPROC)FlashContent);
				   break;
				case IDM_TOUCH:
				   ///DialogBox(hInst, (LPCTSTR)IDD_TOUCHBOX, hWnd, (DLGPROC)Touch);
				   TouchDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_BT:
				   ///DialogBox(hInst, (LPCTSTR)IDD_TOUCHBOX, hWnd, (DLGPROC)Touch);
				   BTDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_BACKLIGHT:
				   ///DialogBox(hInst, (LPCTSTR)IDD_BACKLIGHTBOX, hWnd, (DLGPROC)Backlight);
				   BacklightDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_VOLUME:
				   ///DialogBox(hInst, (LPCTSTR)IDD_VOLUMEBOX, hWnd, (DLGPROC)Volume);
				   VolumeDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_SWVOLUME:
				   SWvolumeDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_EQ:
				   ///DialogBox(hInst, (LPCTSTR)IDD_EQBOX, hWnd, (DLGPROC)EQ);
				   EQDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_AV:
				   AVDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_HARDDRIVE:
				   DialogBox(hInst, (LPCTSTR)IDD_HARDDRIVEBOX, hWnd, (DLGPROC)HardDrive);				   
				   break;
				case IDM_SDMMC:
				   DialogBox(hInst, (LPCTSTR)IDD_SDMMCBOX, hWnd, (DLGPROC)SDMMC);
				   break;
				case IDM_AUDIO:
				   ///DialogBox(hInst, (LPCTSTR)IDD_AUDIOBOX, hWnd, (DLGPROC)AUDIO);
				   AudioDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_ECHO:
				   ///DialogBox(hInst, (LPCTSTR)IDD_ECHOBOX, hWnd, (DLGPROC)ECHO);
				   EchoDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_PERFORMANCE_COUNT:   
				   ///DialogBox(hInst, (LPCTSTR)IDD_PERFCOUNTBOX, hWnd, (DLGPROC)PerformanceCount);
				   PerformanceCountDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_GPS:
				   DialogBox(hInst, (LPCTSTR)IDD_GPSBOX, hWnd, (DLGPROC)Gps);
				   break;
				case IDM_VDBG:
				   ///DialogBox(hInst, (LPCTSTR)IDD_VDBGBOX, hWnd, (DLGPROC)Vdbg);
				   VdbgDialogBoxHook( hInst, hWnd );
				   break;
				case IDM_FILE_EXIT:
				   DestroyWindow(hWnd);
				   break;
				case IDM_FILE_REBOOT:
				   if( !KernelIoControl( IOCTL_HAL_REBOOT, NULL, 0, NULL, 0, NULL ) )
				   {
				    	///SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("IOCTL_HAL_REBOOT Fail !!")); 
				   }						
				   break;
				case IDM_FILE_REIBOOT:
					{
						HANDLE hMPM = INVALID_HANDLE_VALUE;		/// span0111
						hMPM = CreateFile( _T("MPM1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
						if( hMPM && !DeviceIoControl(hMPM, IOCTL_OEM_IBOOT_RESET_DEVICE, NULL, 0, NULL, 0, NULL, NULL) )
						{
				    	///SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("IOCTL_HAL_REBOOT Fail !!")); 
						}
///						if( !KernelIoControl( IOCTL_OEM_HAL_IBOOT, NULL, 0, NULL, 0, NULL ) )
///						{
///				    		///SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("IOCTL_HAL_REBOOT Fail !!")); 
///						}
					}	
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_CREATE:
			hwndCB = CommandBar_Create(hInst, hWnd, 1);			
			CommandBar_InsertMenubar(hwndCB, hInst, IDM_MENU, 0);
			CommandBar_AddAdornments(hwndCB, 0, 0);
			break;
		case WM_PAINT:
			RECT rt;
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rt);
			LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
			DrawText(hdc, szHello, _tcslen(szHello), &rt, 
				DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			NKDbgPrintfW( L"SPD@WM_KEYDOWN\r\n");
			break;			

		case WM_KEYUP:
			NKDbgPrintfW( L"SPD@WM_KEYUP\r\n");			
			break;
			
		case WM_DESTROY:
			CommandBar_Destroy(hwndCB);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

#if 0
// Mesage handler for the About box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
#endif

#if 0
// Mesage handler for the HW Version box.
LRESULT CALLBACK HWVersion(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	int index = 0;
	TCHAR szString[64];         // Temperary string
	HWND hwndCombo = NULL;      // Handle to the combobox control
	DWORD hwVersion[4];
	DWORD dwRet = 0;
	int iItemNumber = 0;

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			
			SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("Click Read button")); 
			SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, TEXT("Click Read button")); 
			
			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_HWVERSION_COMBO);
			// Add items to the combo box.
			for (index = 1; index < 10; index++)
			{
				wsprintf (szString, TEXT("HW Version [%d](0x%x)"), index, index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			for (index = 65; index < 91; index++)
			{
				wsprintf (szString, TEXT("HW Version [%c](0x%x)"), index, index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage(hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO1);
			// Add items to the combo box.
			wsprintf (szString, TEXT("0700-Ruby"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("0703-Pluto1"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("0704-Pluto2"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			
			
			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO2);
			// Add items to the combo box.
			wsprintf (szString, TEXT("00-Tulsa"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("20-San Dimas"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("30-Thailand"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("40-China"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("50-Taiwan"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("60-Nantes"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)3, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO3);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO4);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO5);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO6);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO7);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO8);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			


			SetDlgItemText(hDlg, IDC_CURRDRIVER_EDIT, spCheckBattery() );

			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_HWVERSION_READ )
			{
				if( KernelIoControl( IOCTL_HAL_READ_HW_VERSION, NULL, 0, (LPVOID)hwVersion, 16, (LPDWORD)&dwRet ) )
				{
					wsprintf (szString, TEXT("HW Version [%c](0x%x)"), hwVersion[0], hwVersion[0]);
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, szString); 
				}
				else
				{
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("Get version Fail !!")); 
				}						
				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_HWVERSION_WRITE )
			{
				// Get the selected item.
				hwndCombo = GetDlgItem(hDlg, IDC_HWVERSION_COMBO);
				if( hwndCombo != NULL )
					iItemNumber = SendMessage (hwndCombo, CB_GETCURSEL, (WPARAM)0, 0);
				else
				{		
					wsprintf (szString, TEXT("Error !!"));
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, szString); 
					return FALSE;
				}	
				
				if( iItemNumber < 0 )
				{
					wsprintf (szString, TEXT("Error !!!"));
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, szString); 
					return FALSE;
				}
				else
				if( iItemNumber < 9 )
				{
					hwVersion[0] = iItemNumber+0x31;
				}			
				else
				if( iItemNumber < 36 )
				{
					hwVersion[0] = (iItemNumber-9)+0x41;
				}
				else			
				{
					wsprintf (szString, TEXT("Error !!!!"));
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, szString); 
					return FALSE;
				}
				
				if(	KernelIoControl( IOCTL_HAL_WRITE_HW_VERSION, (LPVOID)hwVersion, 16, NULL, 0, (LPDWORD)&dwRet ) )
				{
					wsprintf (szString, TEXT("Write [%c](0x%x)"), hwVersion[0], hwVersion[0]);
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, szString); 
				}
				else
				{
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("Write version Fail !!")); 
				}						
				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_SERIALNUMBER_READ )
			{
				BYTE SerialNumber[16];
				if( KernelIoControl( IOCTL_HAL_READ_SERIAL_NO, NULL, 0, (LPVOID)SerialNumber, 16, (LPDWORD)&dwRet ) )
				{
					if( 
						( (SerialNumber[0] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[1] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[2] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[3] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[4] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[5] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[6] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[7] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[8] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[9] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[10] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[11] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[12] & 0xF0 ) != 0x30 )
					)	
					{
						NKDbgPrintfW( L"SPD@HWVersion() 000.\r\n" );
						wsprintf (szString, TEXT("invalid Serial number"));
						SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, szString);  
					}		
					else
					{																																												
						wsprintf (szString, TEXT("%d%d%d%d%d%d%d%d%d%d%d%d%d"), 
							(SerialNumber[0]&0x0F), (SerialNumber[1]&0x0F),
							(SerialNumber[2]&0x0F), (SerialNumber[3]&0x0F),
							(SerialNumber[4]&0x0F), (SerialNumber[5]&0x0F),
							(SerialNumber[6]&0x0F), (SerialNumber[7]&0x0F),
							(SerialNumber[8]&0x0F), (SerialNumber[9]&0x0F),
							(SerialNumber[10]&0x0F), (SerialNumber[11]&0x0F),
							(SerialNumber[12]&0x0F)
							);
						SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, szString); 
					}	
				}
				else
				{
					SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, TEXT("Get serial number Fail !!")); 
				}						
				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_SERIALNUMBER_WRITE )
			{
				BYTE SerialNumber[16];
				DWORD dwResult = LB_ERR;
				int index;
				
				for( index = 0; index<16; index++ )	///clear it
					SerialNumber[index] = 0;
				
				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[0] = 0x30;
					SerialNumber[1] = 0x37;
					SerialNumber[2] = 0x30;
					SerialNumber[3] = 0x30;															
				}	
				else
				{	
					SerialNumber[0] = 0x30;
					SerialNumber[1] = 0x37;
					SerialNumber[2] = 0x30;
					if( dwResult == 0 )		
						SerialNumber[3] = 0x30;
					else
					if( dwResult == 1 )
						SerialNumber[3] = 0x33;
					else
					if( dwResult == 2 )
						SerialNumber[3] = 0x34;						
					else
						SerialNumber[3] = 0x30;
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[4] = 0x34;
					SerialNumber[5] = 0x30;
				}	
				else
				{	
					if( dwResult == 0 )
						SerialNumber[4] = 0x30;
					else
					if( dwResult == 1 )
						SerialNumber[4] = 0x32;
					else
					if( dwResult == 2 )
						SerialNumber[4] = 0x33;
					else
					if( dwResult == 3 )
						SerialNumber[4] = 0x34;
					else
					if( dwResult == 4 )
						SerialNumber[4] = 0x35;
					else
					if( dwResult == 5 )
						SerialNumber[4] = 0x36;
					else						
						SerialNumber[4] = 0x34;
					SerialNumber[5] = 0x30;						
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO3, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[6] = 0x30;
				}	
				else
				{
					SerialNumber[6] = (0x30|(((BYTE)dwResult)&0x0F));
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO4, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[7] = 0x30;
				}	
				else
				{
					SerialNumber[7] = (0x30|(((BYTE)dwResult)&0x0F));
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO5, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[8] = 0x30;
				}	
				else
				{
					SerialNumber[8] = (0x30|(((BYTE)dwResult)&0x0F));
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO6, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[9] = 0x30;
				}	
				else
				{
					SerialNumber[9] = (0x30|(((BYTE)dwResult)&0x0F));
				}	
				
				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO7, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[10] = 0x30;
				}	
				else
				{
					SerialNumber[10] = (0x30|(((BYTE)dwResult)&0x0F));
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO8, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[11] = 0x30;
				}	
				else
				{
					SerialNumber[11] = (0x30|(((BYTE)dwResult)&0x0F));
				}	

				SerialNumber[12] = 0x30 | (10-(((((SerialNumber[1]&0x0F)+(SerialNumber[3]&0x0F)+(SerialNumber[5]&0x0F)+\
									(SerialNumber[7]&0x0F)+(SerialNumber[9]&0x0F)+(SerialNumber[11]&0x0F))*3)+\
									((SerialNumber[0]&0x0F)+(SerialNumber[2]&0x0F)+(SerialNumber[4]&0x0F)+\
									(SerialNumber[6]&0x0F)+(SerialNumber[8]&0x0F)+(SerialNumber[10]&0x0F)))&0x0F));
				
				if(	KernelIoControl( IOCTL_HAL_WRITE_SERIAL_NO, (LPVOID)SerialNumber, 16, NULL, 0, (LPDWORD)&dwRet ) )
				{
					wsprintf (szString, TEXT("Write done!!!"));
					SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, szString); 
				}
				else
				{
					SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, TEXT("Write Fail !!")); 
				}						
				return TRUE;
			}

			break;
	}
    return FALSE;
}
#endif

// Mesage handler for the Flash content box.
LRESULT CALLBACK FlashContent(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;

	TCHAR szString[64];         // Temperary string
	static rFC_COMMAND rFCCmd;
	static rFC_RECORD rFCRecord;
	static rFC_COMMAND *prFCCmd = NULL;
	static rFC_RECORD *prFCRecord = NULL;
	DWORD dwOutbuff = 0;
	DWORD dwReturn = 0;
	DWORD dwRsponse = 0;
	DWORD dwByteRsponse = 0;
	DWORD dwData1;
	DWORD dwData2;	
	DWORD dwData3;		
	LPTSTR aRec = TEXT("A_record");
	LPTSTR bRec = TEXT("B_record");
	LPTSTR cRec = TEXT("C_record");
	BOOL bRet = TRUE;
	static DWORD dwTotalRecord = 0;	
	static DWORD dwCurrRecord = 0;	


	if( NULL == prFCCmd )
	{
		prFCCmd = &rFCCmd;
		
		///prFCCmd = (rFC_COMMAND *)MapPtrToProcess( &rFCCmd , (HANDLE)GetCurrentProcessId() );
		NKDbgPrintfW( L"SPD@FlashContent() Map command pointer 0x%08x to 0x%08x.\r\n", &rFCCmd, prFCCmd);
	}

	if( NULL == prFCRecord )
	{
		prFCRecord = &rFCRecord;
		
		///prFCRecord = (rFC_RECORD *)MapPtrToProcess( &rFCRecord , (HANDLE)GetCurrentProcessId() );
		NKDbgPrintfW( L"SPD@FlashContent() Map record pointer 0x%08x to 0x%08x.\r\n", &rFCRecord, prFCRecord);
	}


	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}

			SetDlgItemText(hDlg, IDC_EDIT_TATALRECORD, TEXT("0/0")); 
			SetDlgItemText(hDlg, IDC_EDIT_ID, TEXT("Empty")); 
			SetDlgItemText(hDlg, IDC_EDIT_NAME, TEXT("Empty")); 
			SetDlgItemText(hDlg, IDC_EDIT_DATA, TEXT("Empty")); 			

			rFCCmd.lpResponse = (LPVOID)&dwRsponse;
			rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;				
			rFCCmd.dwCommand = rFC_LOCK;
			KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
///
///
///			rFCCmd.lpResponse = (LPVOID)&dwRsponse;
///			rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;		
///		
///			rFCCmd.dwCommand = rFC_LIST;
///			KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
///			dwTotalRecord = *(DWORD *)(rFCCmd.lpResponse);
///			
///			wsprintf (szString, TEXT("0/%d"), dwTotalRecord);
///			SetDlgItemText(hDlg, IDC_EDIT_TATALRECORD, szString); 
///		
///			rFCCmd.dwCommand = rFC_NEW;
///			rFCCmd.lpName = aRec;
///			rFCCmd.dwSizeName = wcslen(aRec);
///			dwData1 = 0x12345678;
///			rFCCmd.lpData = (LPVOID)&dwData1;
///			rFCCmd.dwSizeData = sizeof(dwData1);
///			rFCCmd.lpResponse = (LPVOID)&dwRsponse;
///			rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;		
///			KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
///			///SPMPMFATAL(L"SPD@MPM: NEW rFC record ID=[0x%08x] size=%d\r\n", rFCRecord.dwRecordNum, dwReturn);
///			
///			rFCCmd.dwCommand = rFC_NEW;
///			rFCCmd.lpName = bRec;
///			rFCCmd.dwSizeName = wcslen(bRec);
///			dwData2 = 0x87654321;
///			rFCCmd.lpData = (LPVOID)&dwData2;
///			rFCCmd.dwSizeData = sizeof(dwData2);
///			rFCCmd.lpResponse = (LPVOID)&dwRsponse;
///			rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;		
///			KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
///			///SPMPMFATAL(L"SPD@MPM: NEW rFC record ID=[0x%08x] size=%d\r\n", rFCRecord.dwRecordNum, dwReturn);
///
///			rFCCmd.dwCommand = rFC_NEW;
///			rFCCmd.lpName = cRec;
///			rFCCmd.dwSizeName = wcslen(cRec);
///			dwData3 = 0x88888888;
///			rFCCmd.lpData = (LPVOID)&dwData3;
///			rFCCmd.dwSizeData = sizeof(dwData3);
///			rFCCmd.lpResponse = (LPVOID)&dwRsponse;
///			rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;					
///			KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
///			///SPMPMFATAL(L"SPD@MPM: NEW rFC record ID=[0x%08x] size=%d\r\n", rFCRecord.dwRecordNum, dwReturn);
///		
///			rFCCmd.dwCommand = rFC_LIST;
///			KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
///			dwTotalRecord = *(DWORD *)(rFCCmd.lpResponse);			
///			
///			wsprintf (szString, TEXT("0/%d"), dwTotalRecord);
///			SetDlgItemText(hDlg, IDC_EDIT_TATALRECORD, szString); 

///			rFCCmd.dwCommand = rFC_SEEK;
///			rFCCmd.dwRecordID = 0xd0000001;
///			KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
///			///SPMPMFATAL(L"SPD@MPM: SEEK rFC record ID=[0x%08x] size=%d\r\n", rFCRecord.dwRecordNum, dwReturn);
///
///			wsprintf (szString, TEXT("0x%x"), rFCRecord.dwRecordNum);
///			SetDlgItemText(hDlg, IDC_EDIT_ID, szString); 
///			SetDlgItemText(hDlg, IDC_EDIT_NAME, rFCRecord.cRecordName); 
///			wsprintf (szString, TEXT("0x%x"), rFCRecord.dwData);
///			SetDlgItemText(hDlg, IDC_EDIT_DATA, szString); 			
			
			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{

				///rFCCmd.lpResponse = (LPVOID)&dwRsponse;
				///rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;				
				///rFCCmd.dwCommand = rFC_UNLOCK;
				///KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
				
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_FLASHCONTENT_UP )
			{
				if( dwCurrRecord == 0 )
					dwCurrRecord = 1;
				else
				if( dwCurrRecord == 1 )
					dwCurrRecord = 1;
				else
					dwCurrRecord = dwCurrRecord - 1;
						
				rFCCmd.dwCommand = rFC_SEEK;
				///rFCCmd.dwRecordID = 0xd0000001;
				rFCCmd.dwRecordID = dwCurrRecord | rFC_ID_VALID_TAG;
				bRet = KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
				///SPMPMFATAL(L"SPD@MPM: SEEK rFC record ID=[0x%08x] size=%d\r\n", rFCRecord.dwRecordNum, dwReturn);
				if( bRet )
				{	
					wsprintf (szString, TEXT("0x%lx"), rFCRecord.dwRecordNum);
					SetDlgItemText(hDlg, IDC_EDIT_ID, szString);
					NKDbgPrintfW( L"SPD@FlashContent() dwRecordNum = 0x%08x.\r\n", rFCRecord.dwRecordNum); 
					SetDlgItemText(hDlg, IDC_EDIT_NAME, rFCRecord.cRecordName); 
					wsprintf (szString, TEXT("0x%lx"), rFCRecord.dwData);
					SetDlgItemText(hDlg, IDC_EDIT_DATA, szString);
					NKDbgPrintfW( L"SPD@FlashContent() dwData = 0x%08x.\r\n", rFCRecord.dwData); 
					wsprintf (szString, TEXT("%d/%d"), dwCurrRecord, dwTotalRecord);
					SetDlgItemText(hDlg, IDC_EDIT_TATALRECORD, szString); 					 			
				}
				else
				{
					SetDlgItemText(hDlg, IDC_EDIT_ID, TEXT("find record fail !!"));
				}	

				///wsprintf (szString, TEXT("%d"), sizeof(rFCCmd));
				///SetDlgItemText(hDlg, IDC_EDIT_ID, szString); 
				///wsprintf (szString, TEXT("%d"), sizeof(rFCRecord));
				///SetDlgItemText(hDlg, IDC_EDIT_NAME, szString); 
				///wsprintf (szString, TEXT("0x%x"), prFCRecord);
				///SetDlgItemText(hDlg, IDC_EDIT_DATA, szString); 			

				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_FLASHCONTENT_DOWN )
			{
				if( dwCurrRecord == 0 )
					dwCurrRecord = 1;
				else
				if( dwTotalRecord == dwCurrRecord )
					dwCurrRecord = dwCurrRecord;
				else
					dwCurrRecord = dwCurrRecord + 1;

				rFCCmd.dwCommand = rFC_SEEK;
				///rFCCmd.dwRecordID = 0xd0000002;
				rFCCmd.dwRecordID = dwCurrRecord | rFC_ID_VALID_TAG;
				bRet = KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
				///SPMPMFATAL(L"SPD@MPM: SEEK rFC record ID=[0x%08x] size=%d\r\n", rFCRecord.dwRecordNum, dwReturn);
				if( bRet )
				{	
					wsprintf (szString, TEXT("0x%lx"), rFCRecord.dwRecordNum);
					SetDlgItemText(hDlg, IDC_EDIT_ID, szString);
					NKDbgPrintfW( L"SPD@FlashContent() dwRecordNum = 0x%08x.\r\n", rFCRecord.dwRecordNum); 
					SetDlgItemText(hDlg, IDC_EDIT_NAME, rFCRecord.cRecordName); 
					wsprintf (szString, TEXT("0x%lx"), rFCRecord.dwData);
					SetDlgItemText(hDlg, IDC_EDIT_DATA, szString);
					NKDbgPrintfW( L"SPD@FlashContent() dwData = 0x%08x.\r\n", rFCRecord.dwData); 			
					wsprintf (szString, TEXT("%d/%d"), dwCurrRecord, dwTotalRecord);
					SetDlgItemText(hDlg, IDC_EDIT_TATALRECORD, szString); 					 			
				}
				else
				{
					SetDlgItemText(hDlg, IDC_EDIT_ID, TEXT("find record fail !!"));
				}	

				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_FLASHCONTENT_DELETE )
			{
				rFCCmd.dwCommand = rFC_DELET;
				rFCCmd.dwRecordID = dwCurrRecord | rFC_ID_VALID_TAG;
				bRet = KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );				

				rFCCmd.dwCommand = rFC_LIST;
				KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
				dwTotalRecord = *(DWORD *)(rFCCmd.lpResponse);			
				
				wsprintf (szString, TEXT("0/%d"), dwTotalRecord);
				SetDlgItemText(hDlg, IDC_EDIT_TATALRECORD, szString); 		
				
			}	
			else
			if ( LOWORD(wParam) == IDC_FLASHCONTENT_LOAD )				
			{
				
///				rFCCmd.lpResponse = (LPVOID)&dwRsponse;
///				rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;				
///				rFCCmd.dwCommand = rFC_LOCK;
///				KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
	
	
				rFCCmd.lpResponse = (LPVOID)&dwRsponse;
				rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;		
			
				rFCCmd.dwCommand = rFC_LIST;
				KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
				dwTotalRecord = *(DWORD *)(rFCCmd.lpResponse);
				
				wsprintf (szString, TEXT("0/%d"), dwTotalRecord);
				SetDlgItemText(hDlg, IDC_EDIT_TATALRECORD, szString); 
			
				rFCCmd.dwCommand = rFC_NEW;
				rFCCmd.lpName = aRec;
				rFCCmd.dwSizeName = wcslen(aRec);
				dwData1 = 0x12345678;
				rFCCmd.lpData = (LPVOID)&dwData1;
				rFCCmd.dwSizeData = sizeof(dwData1);
				rFCCmd.lpResponse = (LPVOID)&dwRsponse;
				rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;		
				KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
				///SPMPMFATAL(L"SPD@MPM: NEW rFC record ID=[0x%08x] size=%d\r\n", rFCRecord.dwRecordNum, dwReturn);
				
				rFCCmd.dwCommand = rFC_NEW;
				rFCCmd.lpName = bRec;
				rFCCmd.dwSizeName = wcslen(bRec);
				dwData2 = 0x87654321;
				rFCCmd.lpData = (LPVOID)&dwData2;
				rFCCmd.dwSizeData = sizeof(dwData2);
				rFCCmd.lpResponse = (LPVOID)&dwRsponse;
				rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;		
				KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
				///SPMPMFATAL(L"SPD@MPM: NEW rFC record ID=[0x%08x] size=%d\r\n", rFCRecord.dwRecordNum, dwReturn);
	
				rFCCmd.dwCommand = rFC_NEW;
				rFCCmd.lpName = cRec;
				rFCCmd.dwSizeName = wcslen(cRec);
				dwData3 = 0x88888888;
				rFCCmd.lpData = (LPVOID)&dwData3;
				rFCCmd.dwSizeData = sizeof(dwData3);
				rFCCmd.lpResponse = (LPVOID)&dwRsponse;
				rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;					
				KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
				///SPMPMFATAL(L"SPD@MPM: NEW rFC record ID=[0x%08x] size=%d\r\n", rFCRecord.dwRecordNum, dwReturn);
			
				rFCCmd.dwCommand = rFC_LIST;
				KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
				dwTotalRecord = *(DWORD *)(rFCCmd.lpResponse);			
				
				wsprintf (szString, TEXT("0/%d"), dwTotalRecord);
				SetDlgItemText(hDlg, IDC_EDIT_TATALRECORD, szString); 		
				
			}
			else
			if ( LOWORD(wParam) == IDC_FLASHCONTENT_FLASH )
			{
				rFCCmd.lpResponse = (LPVOID)&dwRsponse;
				rFCCmd.lpBytesResponse = (LPDWORD)&dwByteRsponse;				
				rFCCmd.dwCommand = rFC_UNLOCK;
				KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
			}
			else
			if ( LOWORD(wParam) == IDC_FLASHCONTENT_FLUSH )
			{
				
				rFCCmd.dwCommand = rFC_FLUSHMIRROR;
				KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );		

				rFCCmd.dwCommand = rFC_LIST;
				KernelIoControl( IOCTL_HAL_RESERVED_FLASH_CONTAIN, (LPVOID)prFCCmd, sizeof(rFCCmd), (LPVOID)prFCRecord, sizeof(rFCRecord), (LPDWORD)&dwReturn );
				dwTotalRecord = *(DWORD *)(rFCCmd.lpResponse);			
				
				wsprintf (szString, TEXT("0/%d"), dwTotalRecord);
				SetDlgItemText(hDlg, IDC_EDIT_TATALRECORD, szString); 		
			}

			break;
	}
    return FALSE;
}


#if 0
// Mesage handler for the Touch box.
LRESULT CALLBACK Touch(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			return TRUE;

		case WM_COMMAND:
			///IDC_RADIO_A1
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
#endif

// Mesage handler for the GPS box.
LRESULT CALLBACK Gps(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	HWND hwndPortCombo = NULL;      // Handle to the combobox control
	HWND hwndBaudCombo = NULL;      // Handle to the combobox control
	int index = 0;
	DWORD dwBAUDList[9] = {1200,2400,4800,9600,19200,38400,57600,115200,921600};
	TCHAR szString[64];				// Temperary string		
	
	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			
			hwndPortCombo = GetDlgItem(hDlg, IDC_GPS_PORT_COMBO);
			// Add items to the combo box.
			for (index = 1; index < 10; index++)
			{
				wsprintf (szString, TEXT("COM%d:"), index);
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)1, 0);			


			hwndBaudCombo = GetDlgItem(hDlg, IDC_GPS_BAUD_COMBO);
			// Add items to the combo box.
			for (index = 0; index < 9; index++)
			{
				wsprintf (szString, TEXT("%d-%d"), index, dwBAUDList[index]);
				SendMessage (hwndBaudCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndBaudCombo, CB_SETCURSEL, (WPARAM)2, 0);			

			CloseHandle(hwndPortCombo);
			CloseHandle(hwndBaudCombo);
			
			return TRUE;

		case WM_COMMAND:
			///IDC_RADIO_A1
			///CBN_SELCHANGE 
			if ((HIWORD(wParam) == CBN_SELCHANGE))
			{
				NKDbgPrintfW( TEXT("SPD@GPS():CBN_SELCHANGE idComboBox=0x%08x, hwndComboBox=0x%08x\r\n"), wParam, lParam );
				///NKDbgPrintfW( TEXT("SPD@GPS():IDC_GPS_PORT_COMBO=0x%08x IDC_GPS_BAUD_COMBO=0x%08x\r\n"), IDC_GPS_PORT_COMBO, IDC_GPS_BAUD_COMBO );				
				if ((LOWORD(wParam) == IDC_GPS_PORT_COMBO))
				{
					NKDbgPrintfW( TEXT("SPD@GPS():IDC_GPS_PORT_COMBO=0x%08x\r\n"), IDC_GPS_PORT_COMBO );				
				}
				else
				if ((LOWORD(wParam) == IDC_GPS_BAUD_COMBO))
				{
					NKDbgPrintfW( TEXT("SPD@GPS():IDC_GPS_BAUD_COMBO=0x%08x\r\n"), IDC_GPS_BAUD_COMBO );				
				}
			}
								
			if ((LOWORD(wParam) == IDC_GPS_START))
			{
				DWORD	dwResult;
				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_GPS_PORT_COMBO, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwComSelet = 1;		///index "1" -> COM2:
				else
					dwComSelet = dwResult;

				/// get baud setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_GPS_BAUD_COMBO, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwBaudSelet = 2;		///index "2" -> 4800
				else
					dwBaudSelet = dwResult;
				
				NKDbgPrintfW( TEXT("SPD@AccessComPort():dwComSelet=%d, dwBaudSelet=%d"), dwComSelet, dwBaudSelet );
				/// start com port with setting
				hComThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AccessComPort, (LPVOID)hDlg, 0, NULL );
				if ( !hComThread )
					;
				else
					CloseHandle(hComThread);

				Sleep( 300 );				
				
				return TRUE;
			}

			if ((LOWORD(wParam) == IDC_GPS_STOP))
			{
				if( hComThread != NULL )
				{	
					bForcePortClose = TRUE;
					Sleep( 300 );
					CloseHandle(hComThread);
					CloseHandle(ghComPort);
					hComThread = NULL;
				}	
				return TRUE;
			}

			if ((LOWORD(wParam) == IDC_GPS_CLS))
			{
				///pause it ~
				if( !bScreenPause )
					bScreenPause = TRUE;
				
				SendDlgItemMessage(hDlg, IDC_GPS_LIST1, LB_RESETCONTENT, 0, 0);
				///clear screen
				do
				{
					SendDlgItemMessage(hDlg, IDC_GPS_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_GPS_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );
				
				///resume it ~
				if( bScreenPause )
					bScreenPause = FALSE;
				
				return TRUE;
			}

			if ((LOWORD(wParam) == IDC_GPS_PAUSE))
			{
				if( bScreenPause )
					bScreenPause = FALSE;
				else
					bScreenPause = TRUE;
				return TRUE;
			}

			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


#if 0
/*******************************************************************************
		AccessComPort(void)
********************************************************************************/
void WINAPI AccessVDBGPort(HWND hwnd)
{
	HANDLE hTestComm = INVALID_HANDLE_VALUE;
	DWORD dwSetting[9] = {CBR_1200,CBR_2400,CBR_4800,CBR_9600,CBR_19200,CBR_38400,CBR_57600,CBR_115200,921600};
	TCHAR szTstring[VDBG_MESSAGE_MAX_SIZE];		// Temperary string
	TCHAR szTstrtmp[VDBG_MESSAGE_MAX_SIZE];		// Temperary string
	CHAR  szCstring[VDBG_MESSAGE_MAX_SIZE];		// Temperary string
	DWORD dwLoop = 0;
	DWORD dwLoop1 = 0;
	

	NKDbgPrintfW( TEXT("\r\nSPD@AccessVDBGPort():start !! \r\n") );
	///MessageBox(hwnd, L"Com Port Already Open !!", L"Error", MB_OK);
	
	// Open VDBGPort
	if( NULL == ghVDBGPort )
	{	
		hTestComm = CreateFile ( VDBG_DRIVER_HANDLE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    	
		if ( INVALID_HANDLE_VALUE == hTestComm )
		{
			NKDbgPrintfW( TEXT("SPD@AccessVDBGPort():start CreateFile Fail !! \r\n") );
			MessageBox(hwnd, L"Com Port Open failed!!", L"Error", MB_OK);
			return;
		}
		ghVDBGPort = hTestComm;
	}
	else
		hTestComm = ghVDBGPort;	
	
	
	/// set flag port open
	bVDBGPortOpened = TRUE;

	DWORD dwVDBGPortID = 0x12345678;///dwVPortSelet
	DWORD dwVDBGEventHandle = NULL;
	DWORD dwByteRet = 0;
	
	
	dwVDBGPortID = PortInCombo[dwVPortSelet].PortID;
	//init VDBG port
   	DeviceIoControl(hTestComm, IOCTL_OEM_VDBG_PORT_OPEN, &dwVDBGPortID, sizeof(dwVDBGPortID), &dwVDBGEventHandle, sizeof(dwVDBGEventHandle), &dwByteRet, NULL);	


	while( hTestComm != INVALID_HANDLE_VALUE && dwVDBGEventHandle != NULL ) 
	{
		// Wait for an event to occur for the port.
		WaitForSingleObject( (HANDLE)dwVDBGEventHandle, INFINITE );
		dwLoop1 = 0;
		// Read the data from the VDBG port.
		DeviceIoControl(hTestComm, IOCTL_OEM_VDBG_PORT_GETDATA, NULL, 0, szTstrtmp, 128/*sizeof(String[128])*/, &dwByteRet, NULL);
		
		NKDbgPrintfW( L"IOCTL_OEM_VDBG_PORT_GETDATA:[%s] %d char\r\n", szCstring, dwByteRet );
		
		/// show to screen !!
		if( !bVDBGScreenPause )
		{
			///while( dwLoop1<VDBG_MESSAGE_MAX_SIZE )
			///{
			///	szTstrtmp[dwLoop1] = szCstring[dwLoop1];
			///	dwLoop1++;
			///}
			///wsprintf (szString, TEXT("%c"), tcByte);
			///szString[dwLoop++] = '\0';
			wsprintf (szTstring, TEXT("%d:%s"), dwLoop, szTstrtmp);
			
			///SendDlgItemMessage(hwnd, IDC_VDBG_LIST1, LB_ADDSTRING, 0, (LPARAM)(TEXT("ABC")) );
			SendDlgItemMessage(hwnd, IDC_VDBG_LIST1, LB_ADDSTRING, 0, (LPARAM)szTstring);
			///move the screen to the last message		
			DWORD dwIndex = SendDlgItemMessage(hwnd, IDC_VDBG_LIST1, LB_GETCOUNT, 0, 0);							
			SendDlgItemMessage(hwnd, IDC_VDBG_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
		}	


		Sleep(0);
		dwLoop++;

		if ( TRUE == bVDBGForcePortClose )	///force close ComPort
			break;

	}/// end of while()

	CloseHandle(hTestComm);
	bVDBGPortOpened = FALSE;

}
#endif

#if 0
DWORD spAccessAudioCodec(BOOL bRead, DWORD* pdwAddr, DWORD* pdwVal, DWORD* pdwRet, DWORD* pdwSuccess )
{
	DWORD dwRet = 0;
	
	AUDIO_TEST audioTest;

	audioTest.dwAddress = *pdwAddr;
	audioTest.dwValue = *pdwVal;
	audioTest.dwResult = *pdwRet;
	audioTest.dwSucc = *pdwSuccess;

	if( bRead )					
		waveOutMessage( 0, WIDM_GET_REGISTER, (DWORD)&audioTest, 0 );
	else	
		waveOutMessage( 0, WIDM_SET_REGISTER, (DWORD)&audioTest, 0 );
		
	*pdwAddr = audioTest.dwAddress;
	*pdwVal = audioTest.dwValue;
	*pdwRet = audioTest.dwResult;
	*pdwSuccess = audioTest.dwSucc;

	return dwRet;
}
#endif

#if 0
// Message handler for the Audio box.
LRESULT CALLBACK AUDIO(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	TCHAR szString[64];         // Temperary string


	HWND hwndPortCombo = NULL;      // Handle to the combobox control
	UINT index = 0;
	DWORD dwAddr = 0;
	DWORD dwVal = 0;
	DWORD dwRet = 0;
	DWORD dwSuccess = 0;
	static DWORD DiffA[64];
	static DWORD IDC_CHECK_ARRARY[16];
	static DWORD g_dwCODECValue = 0;
	///DWORD DiffB[64];

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_ADDR_COMBO1);
			// Add items to the combo box.
			for (index = 0; index < 8; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_ADDR_COMBO2);
			// Add items to the combo box.
			for (index = 0; index < 16; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_VALUE_COMBO1);
			// Add items to the combo box.
			for (index = 0; index < 16; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_VALUE_COMBO2);
			// Add items to the combo box.
			for (index = 0; index < 16; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_VALUE_COMBO3);
			// Add items to the combo box.
			for (index = 0; index < 16; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_VALUE_COMBO4);
			// Add items to the combo box.
			for (index = 0; index < 16; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			IDC_CHECK_ARRARY[0] = IDC_AUDIO_CHECK0;
			IDC_CHECK_ARRARY[1] = IDC_AUDIO_CHECK1;
			IDC_CHECK_ARRARY[2] = IDC_AUDIO_CHECK2;
			IDC_CHECK_ARRARY[3] = IDC_AUDIO_CHECK3;
			IDC_CHECK_ARRARY[4] = IDC_AUDIO_CHECK4;
			IDC_CHECK_ARRARY[5] = IDC_AUDIO_CHECK5;
			IDC_CHECK_ARRARY[6] = IDC_AUDIO_CHECK6;
			IDC_CHECK_ARRARY[7] = IDC_AUDIO_CHECK7;
			IDC_CHECK_ARRARY[8] = IDC_AUDIO_CHECK8;
			IDC_CHECK_ARRARY[9] = IDC_AUDIO_CHECK9;
			IDC_CHECK_ARRARY[10] = IDC_AUDIO_CHECK10;
			IDC_CHECK_ARRARY[11] = IDC_AUDIO_CHECK11;
			IDC_CHECK_ARRARY[12] = IDC_AUDIO_CHECK12;
			IDC_CHECK_ARRARY[13] = IDC_AUDIO_CHECK13;
			IDC_CHECK_ARRARY[14] = IDC_AUDIO_CHECK14;
			IDC_CHECK_ARRARY[15] = IDC_AUDIO_CHECK15;

			return TRUE;
		
		case WM_COMMAND:
			if ((HIWORD(wParam) == CBN_SELCHANGE))
			{
				///if ((LOWORD(wParam) == IDC_VDBG_PORT_COMBO))
				///g_dwCODECValue
				DWORD	dwValue1000;
				DWORD	dwValue100;
				DWORD	dwValue10;
				DWORD	dwValue1;				
				DWORD 	dwResult;
				DWORD	dwLoop;
				
				/// get Value in combo list
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue1000 = 0;		///set default
				else
					dwValue1000 = dwResult;
				
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue100 = 0;		///set default
				else
					dwValue100 = dwResult;

				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO3, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue10 = 0;		///set default
				else
					dwValue10 = dwResult;

				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO4, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue1 = 0;		///set default
				else
					dwValue1 = dwResult;
				
				g_dwCODECValue = (dwValue1000*0x1000)+(dwValue100*0x100)+(dwValue10*0x10)+(dwValue1);
					
				///set check box with bit setting
				for( dwLoop = 0; dwLoop < 16; dwLoop++)
				{
					if( g_dwCODECValue & (0x0001<<dwLoop) )
						SendDlgItemMessage(hDlg, IDC_CHECK_ARRARY[dwLoop], BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0 );
					else
						SendDlgItemMessage(hDlg, IDC_CHECK_ARRARY[dwLoop], BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0 );
				}		
				
				return TRUE;	
			}
			
			if ((LOWORD(wParam) == IDC_AUDIO_DUMP) )
			{
				DWORD dwCount = 0;
						
				while( dwCount < WM_REGISTER_MAX_ADDRESS )
				{
					dwAddr = dwCount;
					spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
										
					wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
					SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					
					if( dwAddr == 0x7E )
					{
						if( dwRet == 0x4C13 )
						{	
							wsprintf (szString, TEXT("WM_9713L"));
							SetDlgItemText(hDlg, IDC_AUDIO_EDIT1, szString);
						}	
						else
						if( dwRet == 0x4C12 )
						{	
							wsprintf (szString, TEXT("WM_9712L"));
							SetDlgItemText(hDlg, IDC_AUDIO_EDIT1, szString);
						}	
						else
						{
							wsprintf (szString, TEXT("WM_xxxx"));
							SetDlgItemText(hDlg, IDC_AUDIO_EDIT1, szString);					
						}	
					}
					if( (dwAddr == 0x7C) && ( dwRet != 0x574D ))
					{		
						wsprintf (szString, TEXT("UNKNOW CODEC"));
						SetDlgItemText(hDlg, IDC_AUDIO_EDIT1, szString);
					}						
					dwCount = dwCount+2; 
				}
				
				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}
			
			if ((LOWORD(wParam) == IDC_AUDIO_READ) )
			{
				DWORD	dwResult;
				DWORD	dwAddr10;
				DWORD	dwAddr1;
				DWORD	dwLoop;
				
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_ADDR_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwAddr10 = 0;		///error! set default
				else
					dwAddr10 = dwResult;

				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_ADDR_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwAddr1 = 0;		///error! set default
				else
					dwAddr1 = dwResult;

				/// access CODEC
				dwAddr = (dwAddr10*0x10)+dwAddr1;
				spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
										
				wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///set check box with bit setting
				for( dwLoop = 0; dwLoop < 16; dwLoop++)
				{
					if( dwRet & (0x0001<<dwLoop) )
						SendDlgItemMessage(hDlg, IDC_CHECK_ARRARY[dwLoop], BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0 );
					else
						SendDlgItemMessage(hDlg, IDC_CHECK_ARRARY[dwLoop], BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0 );	
				}		
					
				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
								
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_AUDIO_WRITE) )
			{
				DWORD	dwResult;
				DWORD	dwAddr10;
				DWORD	dwAddr1;
#if 0
				DWORD	dwValue1000;
				DWORD	dwValue100;
				DWORD	dwValue10;
				DWORD	dwValue1;
#endif
				DWORD	dwLoop;
				
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_ADDR_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwAddr10 = 0;		///error! set default
				else
					dwAddr10 = dwResult;
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_ADDR_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwAddr1 = 0;		///error! set default
				else
					dwAddr1 = dwResult;
#if 0
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue1000 = 0;		///error! set default
				else
					dwValue1000 = dwResult;
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue100 = 0;		///error! set default
				else
					dwValue100 = dwResult;
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO3, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue10 = 0;		///error! set default
				else
					dwValue10 = dwResult;
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO4, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue1 = 0;		///error! set default
				else
					dwValue1 = dwResult;
					
				g_dwCODECValue = (dwValue1000*0x1000)+(dwValue100*0x100)+(dwValue10*0x10)+(dwValue1);	
#else
				///get check box with bit setting
				for( dwLoop = 0; dwLoop < 16; dwLoop++)
				{
					if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_CHECK_ARRARY[dwLoop], BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
						g_dwCODECValue |= (0x0001<<dwLoop);	///set bit
					else
						g_dwCODECValue &= ~(0x0001<<dwLoop);	///clear bit
				}		

#endif

				/// access CODEC
				dwAddr = (dwAddr10*0x10)+dwAddr1;
				dwVal = g_dwCODECValue;
				
				///write to CODEC
				spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("W:0x%02X-#:0x%04X %s"), dwAddr, dwVal, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				
				///read from CODEC
				spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_AUDIO_DIFF_A) )
			{
				DWORD dwCount = 0;
				DWORD dwIndex = 0;
						
				while( dwCount < WM_REGISTER_MAX_ADDRESS )
				{
					dwAddr = dwCount;
					spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
										
					///wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
					///SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					
					DiffA[dwIndex] = dwRet;
					dwIndex++;
					dwCount = dwCount+2; 
				}
				
				///move the screen to the last message		
				dwIndex = SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}				

			if ((LOWORD(wParam) == IDC_AUDIO_DIFF_B) )
			{
				DWORD dwCount = 0;
				DWORD dwIndex = 0;
						
				while( dwCount < WM_REGISTER_MAX_ADDRESS )
				{
					dwAddr = dwCount;
					spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
										
					///wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
					///SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					
					if( DiffA[dwIndex] != dwRet )
					{
						wsprintf (szString, TEXT("R:0x%02X-#:0x%04X->0x%04X %s"), dwAddr, DiffA[dwIndex], dwRet, (dwSuccess==0)?L"Change":L"Fail" );
						SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
					}	
					dwIndex++;
					dwCount = dwCount+2; 
				}
				
				///move the screen to the last message		
				dwIndex = SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}				
	
			if ((LOWORD(wParam) == IDC_AUDIO_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}							
			break;
	}
    return FALSE;
}
#endif

#if 0
// Message handler for the EQ box.
LRESULT CALLBACK EQ(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	TCHAR szString[64];         // Temperary string

///#define WM_REGISTER_MAX_ADDRESS		0x80
///#define WM_REGISTER_MIN_ADDRESS		0x00

	HWND hwndPortCombo = NULL;      // Handle to the combobox control
	UINT index = 0;
	DWORD dwAddr = 0;
	DWORD dwVal = 0;
	DWORD dwRet = 0;
	DWORD dwSuccess = 0;
	static DWORD DiffA[64];
	static DWORD IDC_CHECK_ARRARY[16];
	static DWORD g_dwCODECValue = 0;
	///DWORD DiffB[64];

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_EQ_COMBO1);
			// Add items to the combo box.
			wsprintf(szString, TEXT("00-Default/Flat"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("01-Rock"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("02-POP"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("03-Jazz"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("04-Classical"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("05-Dance"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("06-Heavy"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("07-Disco"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("08-Soft"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("09-3D Stere/Live"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("10-Hall"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);

			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			return TRUE;
		
		case WM_COMMAND:
			if ((HIWORD(wParam) == CBN_SELCHANGE))
			{
				DWORD	dwModeSelect;
				DWORD 	dwResult;
				DWORD dwAddr, dwVal, dwRet, dwSuccess;
				DWORD dwVal40, dwVal20, dwVal1E;
#define WM_BIT_0	0x0001
#define WM_BIT_1	0x0002
#define WM_BIT_2	0x0004
#define WM_BIT_3	0x0008
#define WM_BIT_4	0x0010
#define WM_BIT_5	0x0020
#define WM_BIT_6	0x0040
#define WM_BIT_7	0x0080
#define WM_BIT_8	0x0100
#define WM_BIT_9	0x0200
#define WM_BIT_10	0x0400
#define WM_BIT_11	0x0800
#define WM_BIT_12	0x1000
#define WM_BIT_13	0x2000
#define WM_BIT_14	0x4000
#define WM_BIT_15	0x8000

				/// get Value in combo list
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_EQ_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwModeSelect = 0;		///set default
				else
					dwModeSelect = dwResult;
				
				///read from CODEC
				dwAddr = 0x40;
				spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwVal40 = dwRet;

				///read from CODEC
				dwAddr = 0x20;
				spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwVal20 = dwRet;

				///read from CODEC
				dwAddr = 0x1E;
				spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwVal1E = dwRet;
				
				switch(dwModeSelect)
				{
					case 0:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						///dwVal1E |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						break;
					case 1:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						///dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						dwVal20 &= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_10);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_2|WM_BIT_1);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_2|WM_BIT_0);	///3D level
						break;
					case 2:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_11|WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_2|WM_BIT_1);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						break;
					case 3:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						///dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_9|WM_BIT_8);		///Bass level
						///dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_2);	///Treble level
						
						///dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_2|WM_BIT_0);	///3D level
						
						break;
					case 4:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_1);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_1|WM_BIT_0);	///3D level

						break;
					case 5:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						///dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						dwVal20 &= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_1|WM_BIT_0);	///Treble level
						
						///dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_3|WM_BIT_2);	///3D level

						break;
					case 6:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_9|WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_1|WM_BIT_0);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						///dwVal1E |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level

						break;
					case 7:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						///dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						///dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						dwVal20 &= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_10);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_2);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						///dwVal1E |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level

						break;
					case 8:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						///dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						///dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						dwVal20 &= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_10);		///Bass level
						///dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_3|WM_BIT_1);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						///dwVal1E |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level

						break;
					case 9:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_3|WM_BIT_1|WM_BIT_0);	///3D level

						break;
					case 10:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_10);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_1);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_3|WM_BIT_1|WM_BIT_0);	///3D level

						break;
					default:
						break;
				}///switch(dwModeSelect)

				///write to CODEC
				dwAddr = 0x40;
				dwVal = dwVal40;
				spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("W:0x%02X-#:0x%04X %s"), dwAddr, dwVal, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				
				///write to CODEC
				dwAddr = 0x20;
				dwVal = dwVal20;
				spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("W:0x%02X-#:0x%04X %s"), dwAddr, dwVal, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///write to CODEC
				dwAddr = 0x1E;
				dwVal = dwVal1E;
				spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("W:0x%02X-#:0x%04X %s"), dwAddr, dwVal, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				
				return TRUE;	
			}
			else
			if ((LOWORD(wParam) == IDC_EQ_DUMP) )
			{
				DWORD dwCount = 0;
						
				while( dwCount < WM_REGISTER_MAX_ADDRESS )
				{
					dwAddr = dwCount;
					spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
										
					wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
					SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					
					if( dwAddr == 0x7E )
					{
						if( dwRet == 0x4C13 )
						{	
							wsprintf (szString, TEXT("WM_9713L"));
							SetDlgItemText(hDlg, IDC_EQ_EDIT1, szString);
						}	
						else
						if( dwRet == 0x4C12 )
						{	
							wsprintf (szString, TEXT("WM_9712L"));
							SetDlgItemText(hDlg, IDC_EQ_EDIT1, szString);
						}	
						else
						{
							wsprintf (szString, TEXT("WM_xxxx"));
							SetDlgItemText(hDlg, IDC_EQ_EDIT1, szString);					
						}	
					}
					if( (dwAddr == 0x7C) && ( dwRet != 0x574D ))
					{		
						wsprintf (szString, TEXT("UNKNOW CODEC"));
						SetDlgItemText(hDlg, IDC_EQ_EDIT1, szString);
					}						
					dwCount = dwCount+2; 
				}
				
				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_EQ_READ) )
			{				
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_EQ_WRITE) )
			{
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_EQ_CHECK_DEVICE) )
			{
				DWORD dwNumDevs, dwLoop;
				WAVEOUTCAPS    woc;
				WAVEINCAPS     wic;

				dwNumDevs = waveOutGetNumDevs();
				wsprintf (szString, TEXT("waveOutGetNumDevs=%d"), dwNumDevs );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				if( dwNumDevs == 0 )
					return TRUE;

				for (dwLoop = 0; dwLoop < dwNumDevs; dwLoop++)
				{
				    /* Get info about the next device */
				    if (MMSYSERR_NOERROR == waveOutGetDevCaps(dwLoop, &woc, sizeof(WAVEOUTCAPS)))
				    {
				        /* Display its Device ID and name */
				        ///printf("Device ID #%u: %s\r\n", i, wic.szPname);
						wsprintf (szString, TEXT("Device ID #%u: %s"), dwLoop, woc.szPname );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						wsprintf (szString, TEXT("MID:0x%04x PID:0x%04x"), woc.wMid, woc.wPid );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						wsprintf (szString, TEXT("DrvVer:0x%08x Channels:%d"), woc.vDriverVersion , woc.wChannels );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						if( woc.dwFormats & WAVE_FORMAT_1M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_1M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_1S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_1S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_2M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_2M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_2S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_2S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_4M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_4M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_4S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_4S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
							 

						if( woc.dwSupport & WAVECAPS_LRVOLUME )
						{
							wsprintf (szString, TEXT("WAVECAPS_LRVOLUME supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwSupport & WAVECAPS_PITCH )
						{
							wsprintf (szString, TEXT("WAVECAPS_PITCH supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwSupport & WAVECAPS_PLAYBACKRATE )
						{
							wsprintf (szString, TEXT("WAVECAPS_PLAYBACKRATE supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwSupport & WAVECAPS_VOLUME )
						{
							wsprintf (szString, TEXT("WAVECAPS_VOLUME supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwSupport & WAVECAPS_SAMPLEACCURATE )
						{
							wsprintf (szString, TEXT("WAVECAPS_SAMPLEACCURATE supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	

				    }
				}

				dwNumDevs = waveInGetNumDevs();
				wsprintf (szString, TEXT("waveInGetNumDevs=%d"), dwNumDevs );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				if( dwNumDevs == 0 )
					return TRUE;
				
				for (dwLoop = 0; dwLoop < dwNumDevs; dwLoop++)
				{
				    /* Get info about the next device */
				    if (MMSYSERR_NOERROR == waveInGetDevCaps(dwLoop, &wic, sizeof(WAVEINCAPS)))
				    {
				        /* Display its Device ID and name */
				        ///printf("Device ID #%u: %s\r\n", i, wic.szPname);
						wsprintf (szString, TEXT("Device ID #%u: %s"), dwLoop, wic.szPname );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						wsprintf (szString, TEXT("MID:0x%04x PID:0x%04x"), wic.wMid, wic.wPid );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						wsprintf (szString, TEXT("DrvVer:0x%08x Channels:%d"), wic.vDriverVersion , wic.wChannels );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						if( wic.dwFormats & WAVE_FORMAT_1M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_1M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_1S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_1S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_2M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_2M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_2S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_2S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_4M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_4M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_4S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_4S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
				    }
				}

				return TRUE;					
			}				
			else
			if ((LOWORD(wParam) == IDC_EQ_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			else
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}							
			break;
	}
    return FALSE;
}
#endif

#if 0
// Mesage handler for the VDBG box.
LRESULT CALLBACK Vdbg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	HWND hwndPortCombo = NULL;      // Handle to the combobox control
///	HWND hwndBaudCombo = NULL;      // Handle to the combobox control
	int index = 0;
///	DWORD dwBAUDList[9] = {1200,2400,4800,9600,19200,38400,57600,115200,921600};
	TCHAR szString[64];				// Temperary string
	TCHAR szString2[64];				// Temperary string
	static DWORD dwPortList[MAX_PORT_LIST];
	///static PORT_IN_COMBO PortInCombo[MAX_PORT_LIST];
	DWORD dwPortIDtemp = 0;
	DWORD dwByteRet;
	
	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			
			if( ghVDBGPort == NULL )
			{
				ghVDBGPort = CreateFile ( VDBG_DRIVER_HANDLE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		    	
				if ( INVALID_HANDLE_VALUE == ghVDBGPort )
				{
					NKDbgPrintfW( TEXT("SPD@Vdbg(): CreateFile Fail !! \r\n") );
				}
				else
				{
					///get the laa port list in VDBG
					DeviceIoControl(ghVDBGPort, IOCTL_OEM_VDBG_PORT_LIST, NULL, 0, &dwPortList, (MAX_PORT_LIST*(sizeof(DWORD))), &dwByteRet, NULL);	
				}					
			}	
			
			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_VDBG_PORT_COMBO);
			// Add items to the combo box.
			for (index = 0; index < MAX_PORT_LIST; index++)
			{
				if( dwPortList[index] != 0x0 )
				{	
					DWORD dwResult;
					///wsprintf (szString, TEXT("COM%d:"), index);
					dwPortIDtemp = dwPortList[index];
					NKDbgPrintfW( L"0x%08x ", dwPortIDtemp );
					DeviceIoControl(ghVDBGPort, IOCTL_OEM_VDBG_PORT_GETIDNAME, &dwPortIDtemp, sizeof(DWORD), &szString, 128, &dwByteRet, NULL);	
					NKDbgPrintfW( L"IOCTL_OEM_VDBG_PORT_GETIDNAME:[%s] %d char\r\n", szString, dwByteRet );
					///dwPortID[index] = dwPortIDtemp;
					wsprintf (szString2, TEXT("%d:%s"), index+1, szString);			
					SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString2);
					dwResult = SendMessage (hwndPortCombo, CB_GETCOUNT, 0, 0);
					PortInCombo[dwResult-1].PortContext = dwPortList[index];
					PortInCombo[dwResult-1].PortID = dwPortIDtemp;
				}	
			}

			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)1, 0);			

			CloseHandle(hwndPortCombo);
			
			return TRUE;

		case WM_COMMAND:
			///IDC_RADIO_A1
			///CBN_SELCHANGE 
			if ((HIWORD(wParam) == CBN_SELCHANGE))
			{
				NKDbgPrintfW( TEXT("SPD@VDBG():CBN_SELCHANGE idComboBox=0x%08x, hwndComboBox=0x%08x\r\n"), wParam, lParam );

				if ((LOWORD(wParam) == IDC_VDBG_PORT_COMBO))
				{
					NKDbgPrintfW( TEXT("SPD@VDBG():IDC_VDBG_PORT_COMBO=0x%08x\r\n"), IDC_VDBG_PORT_COMBO );				
				}
			}
								
			if ((LOWORD(wParam) == IDC_VDBG_START))
			{
				DWORD	dwResult;
				
				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_VDBG_PORT_COMBO, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwVPortSelet = 0;		///
				else
					dwVPortSelet = dwResult;
				
				NKDbgPrintfW( TEXT("SPD@AccessVDBGPort():dwVPortSelet=%d\r\n"), dwVPortSelet );
				
				/// start com port with setting

				hVDBGThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AccessVDBGPort, (LPVOID)hDlg, 0, NULL );
				if ( !hVDBGThread )
					;
				else
					CloseHandle(hVDBGThread);

				Sleep( 300 );				
				
				return TRUE;
			}

			if ((LOWORD(wParam) == IDC_VDBG_STOP))
			{
				if( hVDBGThread != NULL )
				{	
					bVDBGForcePortClose = TRUE;
					Sleep( 300 );
					CloseHandle(hVDBGThread);
					CloseHandle(ghVDBGPort);
					hVDBGThread = NULL;
				}	
				return TRUE;
			}

			if ((LOWORD(wParam) == IDC_VDBG_CLS))
			{
				///pause it ~
				if( !bVDBGScreenPause )
					bVDBGScreenPause = TRUE;
				
				SendDlgItemMessage(hDlg, IDC_VDBG_LIST1, LB_RESETCONTENT, 0, 0);
				///clear screen
				do
				{
					SendDlgItemMessage(hDlg, IDC_VDBG_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_VDBG_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );
				
				///resume it ~
				if( bVDBGScreenPause )
					bVDBGScreenPause = FALSE;
				
				return TRUE;
			}

			if ((LOWORD(wParam) == IDC_VDBG_PAUSE))
			{
				if( bVDBGScreenPause )
					bVDBGScreenPause = FALSE;
				else
					bVDBGScreenPause = TRUE;
				return TRUE;
			}

			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
#endif

#if 0
// ***************************************************************************
// Function Name: EditBoxSubProc
// 
// Purpose: Subclassed Message Handler for IDC_MESSAGE Edit Box
//
// Arguments: Standard Window Procedure Arguments
//
// Return Values:
//	The LRESULT for a normal message to the Edit Box, ANDed with ~DLGC_WANTMESSAGE
//	if the message is an up arrow and the cursor is on the first line.
//
// Side Effects:
//	Sends an up arrow command to the dialog box
//	
// Description:
//	Intercepts the message from the parent dialog box, asking if the edit control
//	wants to accept the message, and if the cursor is on the first line and the
//	up arrow has been pressed, it rejects the message.

LRESULT CALLBACK EditBoxSubProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	
	switch(uMsg)
	{
		// this is the message that is sent to the Edit Box asking if the Edit Box
		// wants a particular message.  In the case of an up arrow on the top line,
		// we actually want the Edit Box to deny the message and send it back to
		// the dialog box.  This way, focus will return to the previous control.
		case WM_GETDLGCODE:
			// Get the default response
			lResult = CallWindowProc(gfMsgProc, hwnd, uMsg, wParam, lParam);

			if(lParam)
			{
				LPMSG pmsg = (LPMSG)lParam;

				if((pmsg->message == WM_KEYDOWN || pmsg->message == WM_KEYUP))
				{
					switch (pmsg->wParam)
					{
						case VK_F19:
							NKDbgPrintfW( L"@VK_F19\r\n");
							break;
						case VK_F18:
							NKDbgPrintfW( L"@VK_F18\r\n");
							break;
						case VK_APP1:
							NKDbgPrintfW( L"@VK_APP1\r\n");
							break;
						case VK_APP2:
							NKDbgPrintfW( L"@VK_APP2\r\n");
							break;
						case VK_APP3:
							NKDbgPrintfW( L"@VK_APP3\r\n");
							break;
						case VK_APP4:
							NKDbgPrintfW( L"@VK_APP4\r\n");
							break;
						case VK_APP5:
							NKDbgPrintfW( L"@VK_APP5\r\n");
							break;
						case VK_APP6:
							NKDbgPrintfW( L"@VK_APP6\r\n");
							break;
						case VK_ESCAPE:
							NKDbgPrintfW( L"@VK_ESCAPE\r\n");
							break;
						case VK_VOLUME_MUTE:
							NKDbgPrintfW( L"@VK_VOLUME_MUTE\r\n");
							break;
						case VK_VOLUME_UP:
							NKDbgPrintfW( L"@VK_VOLUME_UP\r\n");
							break;
						case VK_VOLUME_DOWN:
							NKDbgPrintfW( L"@VK_VOLUME_DOWN\r\n");
							break;
						case VK_UP:
							NKDbgPrintfW( L"@VK_UP\r\n");
							break;
						case VK_DOWN:
							NKDbgPrintfW( L"@VK_DOWN\r\n");
							break;							
						case VK_LEFT:
							NKDbgPrintfW( L"@VK_LEFT\r\n");
							break;
						case VK_RIGHT:
							NKDbgPrintfW( L"@VK_RIGHT\r\n");							
							break;
						default:
							break;
					}
				}
			}

			break;

		default:
			// respond to the messages with the default action
			lResult = CallWindowProc(gfMsgProc, hwnd, uMsg, wParam, lParam);
			break;
	}
	
	return lResult;
}
#endif

#if 0
// Message handler for the PerformanceCount box.
LRESULT CALLBACK PerformanceCount(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	TCHAR szString[64];         // Temperary string

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}

			return TRUE;
		
		case WM_COMMAND:
			if ((LOWORD(wParam) == IDC_PERFCOUNT_CHECK) )
			{
				LARGE_INTEGER PerformanceCheck;
				DWORD dwFLowPart = 0;
				static DWORD dwLastLowPart = 0;
				DWORD dwCount = 1; 

				if( TRUE == QueryPerformanceCounter(&PerformanceCheck) )
				{
					NKDbgPrintfW(L"!!!QueryPerformanceCounter: HighPart=0x%08x LowPart=0x%08x\r\n", PerformanceCheck.HighPart, PerformanceCheck.LowPart );
					dwLastLowPart = PerformanceCheck.LowPart;
				}
				else
				{
					NKDbgPrintfW(L"!!!QueryPerformanceCounter: fail\r\n");
				}
				
				while(dwCount<6)
				{
					wsprintf (szString, TEXT("Check QueryPerformanceCounter in 3 second"));
					SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
					
					Sleep(3000);					
					if( TRUE == QueryPerformanceFrequency(&PerformanceCheck) )
					{
						NKDbgPrintfW(L"!!!QueryPerformanceFrequency: HighPart=0x%08x LowPart=0x%08x\r\n", PerformanceCheck.HighPart, PerformanceCheck.LowPart );
						wsprintf (szString, TEXT("QueryPerformanceFrequency %d"), dwCount);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						wsprintf (szString, TEXT("HighPart=0x%08x LowPart=0x%08x"), PerformanceCheck.HighPart, PerformanceCheck.LowPart);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						wsprintf (szString, TEXT("HighPart=%08d LowPart=%08d"), PerformanceCheck.HighPart, PerformanceCheck.LowPart);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						
						dwFLowPart = PerformanceCheck.LowPart;
					}
					else
					{
						NKDbgPrintfW(L"!!!QueryPerformanceFrequency: fail\r\n");
						wsprintf (szString, TEXT("QueryPerformanceFrequency %d fail!!"), dwCount);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);												
					}
					
					if( TRUE == QueryPerformanceCounter(&PerformanceCheck) )
					{
						NKDbgPrintfW(L"!!!QueryPerformanceCounter: HighPart=0x%08x LowPart=0x%08x\r\n", PerformanceCheck.HighPart, PerformanceCheck.LowPart );
						NKDbgPrintfW(L"!!!QueryPerformanceCounter: =%d sec \r\n", (PerformanceCheck.LowPart-dwLastLowPart)/dwFLowPart );
						wsprintf (szString, TEXT("QueryPerformanceCounter %d"), dwCount);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						wsprintf (szString, TEXT("HighPart=0x%08x LowPart=0x%08x"), PerformanceCheck.HighPart, PerformanceCheck.LowPart);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						wsprintf (szString, TEXT("HighPart=%08d LowPart=%08d"), PerformanceCheck.HighPart, PerformanceCheck.LowPart);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						wsprintf (szString, TEXT("=> %d seconds"), (PerformanceCheck.LowPart-dwLastLowPart)/dwFLowPart);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						
						dwLastLowPart = PerformanceCheck.LowPart;
					}
					else
					{
						NKDbgPrintfW(L"!!!QueryPerformanceCounter: fail\r\n");
						wsprintf (szString, TEXT("QueryPerformanceCounter %d fail!!"), dwCount);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
					}
					dwCount++;
					
					///move the screen to the last message		
					DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_GETCOUNT, 0, 0);							
					SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
					
				}
				return TRUE;					
			}
			if ((LOWORD(wParam) == IDC_PERFCOUNT_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}							
			break;
	}
    return FALSE;
}
#endif

#if 0
// Message handler for the ECHO box.
LRESULT CALLBACK ECHO(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	TCHAR szString[64];         // Temperary string

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}

			// make sure the dialog box was created
			///if(InitDialog(hDlg))
			{
				HWND hMsgDlg;

				// subclass the edit box
				VERIFY(hMsgDlg = GetDlgItem(hDlg, IDC_ECHO_LIST1));
				if(gfMsgProc = (WNDPROC)GetWindowLong(hMsgDlg, GWL_WNDPROC))
				{
					if(SetWindowLong(hMsgDlg, GWL_WNDPROC, (LONG)EditBoxSubProc))
					{
						// successful
						///break;
					}
				}
			}

			
			return TRUE;
		
		case WM_KEYDOWN:
			wsprintf (szString, TEXT("WM_KEYDOWN %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			switch (wParam)
			{
				case VK_F19:
					wsprintf (szString, TEXT("VK_F19 %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_VOLUME_UP:
					wsprintf (szString, TEXT("VK_VOLUME_UP %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_VOLUME_DOWN:
					wsprintf (szString, TEXT("VK_VOLUME_DOWN %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_UP:
					wsprintf (szString, TEXT("VK_UP %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_DOWN:
					wsprintf (szString, TEXT("VK_DOWN %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;

				default:
					break;
			}
			return TRUE;

		case WM_KEYUP:
			wsprintf (szString, TEXT("WM_KEYUP %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			switch (wParam)
			{
				case VK_F19:
					wsprintf (szString, TEXT("VK_F19 %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_VOLUME_UP:
					wsprintf (szString, TEXT("VK_VOLUME_UP %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_VOLUME_DOWN:
					wsprintf (szString, TEXT("VK_VOLUME_DOWN %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_UP:
					wsprintf (szString, TEXT("VK_UP %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_DOWN:
					wsprintf (szString, TEXT("VK_DOWN %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;

				default:
					break;
			}
			return TRUE;
					
		///case WM_DEIVCECHANGE:
		case 0x0219:
			wsprintf (szString, TEXT("WM_DEIVCECHANGE %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_SD_REMOVED:
			wsprintf (szString, TEXT("WM_OEM_SD_REMOVED %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_SD_INSERT:
			wsprintf (szString, TEXT("WM_OEM_SD_INSERT %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_STARTSYNC:
			wsprintf (szString, TEXT("WM_OEM_STARTSYNC %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_OEM_BREAKSYNC:
			wsprintf (szString, TEXT("WM_OEM_BREAKSYNC %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_OEM_USB_CONNECT:
			wsprintf (szString, TEXT("WM_OEM_USB_CONNECT %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_OEM_USB_DISCONNECT:
			wsprintf (szString, TEXT("WM_OEM_USB_DISCONNECT %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_COMMAND:
			if ((LOWORD(wParam) == IDC_ECHO_IOCTL_PRESET1) )
			{
				HANDLE hEHC = INVALID_HANDLE_VALUE;		/// span0111
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_PRESET_ECHO_1, NULL, 0, NULL, 0, NULL, NULL) )
				{
					wsprintf (szString, TEXT("PRESET_ECHO_1 fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_PRESET_ECHO_1 fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("PRESET_ECHO_1 OK %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_PRESET_ECHO_1 OK. \r\n");
				}
				CloseHandle(hEHC);	
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_IOCTL_PRESET2) )
			{			
				HANDLE hEHC = INVALID_HANDLE_VALUE;		/// span0111
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_PRESET_ECHO_2, NULL, 0, NULL, 0, NULL, NULL) )
				{
					wsprintf (szString, TEXT("PRESET_ECHO_2 fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_PRESET_ECHO_2 fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("PRESET_ECHO_2 OK %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_PRESET_ECHO_2 OK. \r\n");
				}
				CloseHandle(hEHC);	
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_EVENT_PRESET1) )
			{
				HANDLE hECHO = NULL;
				hECHO = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_PRESET_ECHO_1);
				SetEvent(hECHO);
				CloseHandle(hECHO);
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_EVENT_PRESET2) )
			{			
				HANDLE hECHO = NULL;
				hECHO = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_PRESET_ECHO_2);
				SetEvent(hECHO);
				CloseHandle(hECHO);
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_IOCTL_BYPASS) )
			{			
				HANDLE hEHC = INVALID_HANDLE_VALUE;
				BOOL bSet = FALSE;
				BOOL bRet = FALSE;
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_BYPASS, NULL, 0, &bRet, sizeof(bRet), NULL, NULL)
				)
				{
					wsprintf (szString, TEXT("SET_ECHO_BYPASS fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS fail. \r\n");
				}
				else
				{
					bSet = !bRet;
				}	

				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_BYPASS, &bSet, sizeof(bSet), &bRet, sizeof(bRet), NULL, NULL) 
				)
				{
					wsprintf (szString, TEXT("SET_ECHO_BYPASS fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("SET_ECHO_BYPASS OK %d->%d"), bSet, bRet);
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS OK. \r\n");
				}
				CloseHandle(hEHC);
				bSet = !bSet;	
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_IOCTL_POWERDOWN) )
			{
				HANDLE hEHC = INVALID_HANDLE_VALUE;
				BOOL bSet = FALSE;
				BOOL bRet = FALSE;
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare

				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_POWERDOWN, NULL, 0, &bRet, sizeof(bRet), NULL, NULL)
				)
				{
					wsprintf (szString, TEXT("SET_ECHO_BYPASS fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS fail. \r\n");
				}
				else
				{
					bSet = !bRet;
				}	
				
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_POWERDOWN, &bSet, sizeof(bSet), &bRet, sizeof(bRet), NULL, NULL) )
				{
					wsprintf (szString, TEXT("SET_ECHO_POWERDOWN fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_POWERDOWN fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("SET_ECHO_POWERDOWN OK %d->%d"), bSet, bRet);
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_POWERDOWN OK. \r\n");
				}
				CloseHandle(hEHC);
				bSet = !bSet;	
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_EVENT_BYPASS) )
			{
				HANDLE hEHC = INVALID_HANDLE_VALUE;
				BOOL bRet = FALSE;
				
				///setevent
				HANDLE hECHO = NULL;
				hECHO = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SET_ECHO_BYPASS);
				SetEvent(hECHO);
				CloseHandle(hECHO);
				
				Sleep(500);				
				///get current setting
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_BYPASS, NULL, 0, &bRet, sizeof(bRet), NULL, NULL) )
				{
					wsprintf (szString, TEXT("SET_ECHO_BYPASS fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("SET_ECHO_BYPASS OK %d->%d"), bRet, bRet);
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS OK. \r\n");
				}
				CloseHandle(hEHC);
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_EVENT_POWERDOWN) )
			{
				HANDLE hEHC = INVALID_HANDLE_VALUE;
				BOOL bRet = FALSE;
				
				///setevent
				HANDLE hECHO = NULL;
				hECHO = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SET_ECHO_POWERDOWN);
				SetEvent(hECHO);
				CloseHandle(hECHO);
				
				Sleep(500);
				///get current setting
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_POWERDOWN, NULL, 0, &bRet, sizeof(bRet), NULL, NULL) )
				{
					wsprintf (szString, TEXT("SET_ECHO_POWERDOWN fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_POWERDOWN fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("SET_ECHO_POWERDOWN OK %d->%d"), bRet, bRet);
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_POWERDOWN OK. \r\n");
				}
				CloseHandle(hEHC);
				
				return TRUE;					
			}							
			if ((LOWORD(wParam) == IDC_ECHO_CHECK) )
			{			
				HANDLE hEHC = INVALID_HANDLE_VALUE;
				BOOL bRet = FALSE;
				BOOL bRet1 = FALSE;
				
				///get current setting
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare

				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_POWERDOWN, NULL, 0, &bRet, sizeof(bRet), NULL, NULL) )
				{
					wsprintf (szString, TEXT("Get PwrDn fail %d"), GetLastError());
					SetDlgItemText(hDlg, IDC_ECHO_EDIT1, szString);			
				}
				else
				{	
///					wsprintf (szString, TEXT("SET_ECHO_POWERDOWN OK %d->%d"), bRet, bRet);
///					SendDlgItemMessage(hDlg, IDC_ECHO_EDIT1, LB_ADDSTRING, 0, (LPARAM)szString);
///					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_POWERDOWN OK. \r\n");
				}

				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_BYPASS, NULL, 0, &bRet1, sizeof(bRet1), NULL, NULL) )
				{
					wsprintf (szString, TEXT("Get ByPas fail %d"), GetLastError());
					SetDlgItemText(hDlg, IDC_ECHO_EDIT1, szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS fail. \r\n");
				}
				else
				{	
///					wsprintf (szString, TEXT("SET_ECHO_BYPASS OK %d->%d"), bRet1, bRet1);
///					SendDlgItemMessage(hDlg, IDC_ECHO_EDIT1, LB_ADDSTRING, 0, (LPARAM)szString);
///					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS OK. \r\n");
				}

				wsprintf (szString, TEXT("PrD=%d ByP=%d"), bRet, bRet1);
				SetDlgItemText(hDlg, IDC_ECHO_EDIT1, szString);

				CloseHandle(hEHC);
								
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				

			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
#endif

#if 0
void AddToProgressBar(HWND hwnd, int nCnt){
  //get the control window
  HWND hwProgressBar = GetDlgItem(hwnd, IDC_BACKLIGHT_PROGRESS);
  //set progress bar position
  if( 0 == nCnt%10 )
  	SendMessage(hwProgressBar, PBM_SETPOS, (WPARAM)(100-nCnt), 0);
}
#endif


// Message handler for the SDMMC box.
LRESULT CALLBACK SDMMC(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
///	DWORD dwSECTOR_PER_CHUNK[10] = {2,4,8,16,32,64,128,256,512,1024};
	TCHAR szString[64];         // Temperary string

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			return TRUE;
					
		///case WM_DEIVCECHANGE:
		case 0x0219:
			wsprintf (szString, TEXT("WM_DEIVCECHANGE %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_SD_REMOVED:
			wsprintf (szString, TEXT("WM_OEM_SD_REMOVED %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_SD_INSERT:
			wsprintf (szString, TEXT("WM_OEM_SD_INSERT %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_STARTSYNC:
			wsprintf (szString, TEXT("WM_OEM_STARTSYNC %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_OEM_BREAKSYNC:
			wsprintf (szString, TEXT("WM_OEM_BREAKSYNC %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_OEM_USB_CONNECT:
			wsprintf (szString, TEXT("WM_OEM_USB_CONNECT %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_OEM_USB_DISCONNECT:
			wsprintf (szString, TEXT("WM_OEM_USB_DISCONNECT %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_COMMAND:
			///IDC_RADIO_A1
			if ((LOWORD(wParam) == IDC_SDMMC_AUTO_TEST) )
			{			
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_SDMMC_OFF) )
			{			
				SDMMC_ON_OFF(FALSE);
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_SDMMC_ON) )
			{			
				SDMMC_ON_OFF(TRUE);
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_SDMMC_COPY) )
			{	
				DWORD dwSecPerChunk = 0;
				DWORD dwTemp = 0;
				int iLoop = 0;
				
				for( iLoop = 0; iLoop<5; iLoop++ )
				{
					SetCursor(LoadCursor(NULL,IDC_WAIT));
					///Create temp files
					dwSecPerChunk = 512;	///256k
					SDMMC_WRITE2(&dwSecPerChunk, TRUE);
									
					SDMMC_COPY(&dwSecPerChunk);
					dwTemp = dwTemp + dwSecPerChunk;
					NKDbgPrintfW(L"Copy Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
					wsprintf (szString, TEXT("Copy%d:%d KB/s"), iLoop, dwSecPerChunk);
					SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

					///move the screen to the last message		
					DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_GETCOUNT, 0, 0);							
					SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

					UpdateWindow(hDlg);
					SetCursor(LoadCursor(NULL,IDC_SIZEWE));
				}

				NKDbgPrintfW(L"Average Copy Transfer Rate @ %d KBytes/Second. \r\n",dwTemp/5);
				wsprintf (szString, TEXT("Average Copy:%d KB/s"), dwTemp/5);
				SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				UpdateWindow(hDlg);

				return TRUE;					
			}
			
			if ((LOWORD(wParam) == IDC_SDMMC_READ) )
			{
				DWORD dwSecPerChunk;
				DWORD dwTemp;
				DWORD dwTemp2 = 0;
				int iLoop = 0;				
				///TCHAR szString[64];         // Temperary string

				wsprintf (szString, TEXT("Reading !!"));
				SetDlgItemText(hDlg, IDC_SDMMC_EDIT1, szString);
				UpdateWindow(hDlg);
				
				
				for( iLoop = 0; iLoop<10; iLoop++ )
				{
					dwSecPerChunk = dwSECTOR_PER_CHUNK[iLoop];		///1k
					dwTemp = dwSecPerChunk;
					SDMMC_READ(&dwSecPerChunk);
					NKDbgPrintfW(L"Driver Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
					wsprintf (szString, TEXT("R%d:%d KB/s @ %d K bulk."),iLoop, dwSecPerChunk, dwTemp/2);
					SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

					///move the screen to the last message		
					DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_GETCOUNT, 0, 0);							
					SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
					
					UpdateWindow(hDlg);						
				}
				

				///Create temp files
				dwSecPerChunk = 512;	///256k
				dwTemp = dwSecPerChunk;
				SDMMC_WRITE2(&dwSecPerChunk, TRUE);

				for( iLoop = 0; iLoop<10; iLoop++ )
				{
					dwSecPerChunk = dwSECTOR_PER_CHUNK[iLoop];		///1k
					dwTemp = dwSecPerChunk;
					SDMMC_READ2(&dwSecPerChunk);
					NKDbgPrintfW(L"File Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
					wsprintf (szString, TEXT("Rf%d:%d KB/s @ %d K bulk."),iLoop, dwSecPerChunk, dwTemp/2);
					SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

					///move the screen to the last message		
					DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_GETCOUNT, 0, 0);							
					SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

					UpdateWindow(hDlg);
				}

				wsprintf (szString, TEXT("Test done"));
				SetDlgItemText(hDlg, IDC_SDMMC_EDIT1, szString);
				UpdateWindow(hDlg);
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_SDMMC_WRITE) )
			{
				
				DWORD dwSecPerChunk;
				DWORD dwTemp;
				DWORD dwTemp2 = 0;
				int iLoop = 0;						
				///TCHAR szString[64];         // Temperary string

				wsprintf (szString, TEXT("Writing !!"));
				SetDlgItemText(hDlg, IDC_SDMMC_EDIT1, szString);
				UpdateWindow(hDlg);

				for( iLoop = 0; iLoop<10; iLoop++ )
				{
					dwSecPerChunk = dwSECTOR_PER_CHUNK[iLoop];		///1k				
					dwTemp = dwSecPerChunk;
					SDMMC_WRITE2(&dwSecPerChunk, FALSE);
					NKDbgPrintfW(L"Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
					wsprintf (szString, TEXT("Wf%d:%d KB/s @ %d K bulk."),iLoop, dwSecPerChunk, dwTemp/2);
					SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

					///move the screen to the last message		
					DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_GETCOUNT, 0, 0);							
					SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

					UpdateWindow(hDlg);
				}	
				
				wsprintf (szString, TEXT("Test done"));
				SetDlgItemText(hDlg, IDC_SDMMC_EDIT1, szString);
				UpdateWindow(hDlg);
				return TRUE;					
			}					
			if ((LOWORD(wParam) == IDC_SDMMC_CLS) )
			{
				///LONG lResult;
				do
				{
					SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_SDMMC_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


BOOL SDMMC_ON_OFF( BOOL bOn )
{
	BOOL bRet = TRUE;
	HANDLE hStore = NULL;
	
	if(hStore == NULL)
	{
		hStore = CreateFile( TEXT("DSK2:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if(hStore == INVALID_HANDLE_VALUE)
		{  	
			NKDbgPrintfW(L"CreateFile DSK1: fail !!\r\n");		
			CloseHandle(hStore);
			return FALSE;
		}
	}

	if(bOn)
	{	
		DeviceIoControl(hStore, IOCTL_OEM_DISK_POWER_UP, NULL, 0, NULL, 0, NULL, NULL);
		NKDbgPrintfW(L"DSK2: IOCTL_OEM_DISK_POWER_UP !!\r\n");		
	}
	else	
	{	
		DeviceIoControl(hStore, IOCTL_OEM_DISK_POWER_DOWN, NULL, 0, NULL, 0, NULL, NULL);
		NKDbgPrintfW(L"DSK2: IOCTL_OEM_DISK_POWER_DOWN !!\r\n");		
	}
	
	CloseHandle(hStore);	
	
	return bRet;		
}

BOOL SDMMC_COPY(DWORD *pdwTestSector)
{
	BOOL bRet = TRUE;	
	DWORD time1 = 0,time2 = 0,time3 = 0,TransferRate = 0;	
	
	time1 = GetTickCount();
	bRet = CopyFile( SDMMC_PATH_TEST_FILE, SDMMC_PATH_TEST_FILE1, FALSE);
	time2 = GetTickCount();
	time3 = (time2 - time1);
	NKDbgPrintfW(L"time1:%d time2:%d time3:%d \r\n", time1, time2, time3);
	TransferRate = TEST_SIZE_BYTE/time3;
	NKDbgPrintfW(L"TransferRate : %d Bytes/s.\r\n", TransferRate*1000);
	*pdwTestSector = (TransferRate*1000)/1024;	

	DeleteFile( SDMMC_PATH_TEST_FILE );
	DeleteFile( SDMMC_PATH_TEST_FILE1 );
	
	return bRet;	
}


BOOL SDMMC_READ(DWORD *pdwTestSector)
{
#define TEST_CHUNKS		1024

	DWORD time1 = 0,time2 = 0,time3 = 0,TransferRate = 0;
	BOOL bRet = TRUE; 
	HANDLE hStore = NULL;
	SG_REQ sg;
	DWORD	count = 0;
	BYTE *pbBuf = NULL;
	DWORD dwSectorPerChuck = 0;
	DWORD dwBytePerChuck = 0;
	DWORD dwChuckPerTest = 0;
	
	dwSectorPerChuck = (*pdwTestSector);
	dwBytePerChuck = dwSectorPerChuck*512;
	dwChuckPerTest = TEST_CHUNKS;
		
	NKDbgPrintfW(L"SDMMC_READ: Sec/Chunk:%d Byte/Chunk:%d Chunk/Test:%d.\r\n", dwSectorPerChuck, dwBytePerChuck, dwChuckPerTest);
	
	if(hStore == NULL)
	{
		hStore = CreateFile( TEXT("DSK2:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if(hStore == INVALID_HANDLE_VALUE)
		{  	
			NKDbgPrintfW(L"CreateFile DSK2: fail !!\r\n");		
			CloseHandle(hStore);
			return FALSE;
		}
	}

	if(pbBuf == NULL)
	{
		pbBuf = (BYTE *)malloc(dwBytePerChuck);
		if(pbBuf == NULL)
		{
			NKDbgPrintfW(L"malloc fail !! insufficient memory available\r\n");
			return FALSE;
		}
	}		
	
	///DeviceIoControl(hStore, IOCTL_OEM_DISK_POWER_UP, NULL, 0, NULL, 0, NULL, NULL);
		
	time1 = GetTickCount();
	while(1)
	{
		if( count > dwChuckPerTest*dwSectorPerChuck )
		{
			time2 = GetTickCount();
		    time3 = (time2 - time1);
		    NKDbgPrintfW(L"time1:%d time2:%d time3:%d \r\n", time1, time2, time3);
		    TransferRate = (dwChuckPerTest*dwBytePerChuck)/time3;
		    NKDbgPrintfW(L"TransferRate : %d \r\n", TransferRate*1000);
		    *pdwTestSector = (TransferRate*1000)/1024;
			break;
		}
				
	    sg.sr_start = (count+2048);
	    sg.sr_num_sec = dwSectorPerChuck;
	    sg.sr_num_sg = 1;
	    sg.sr_status = ERROR_NOT_SUPPORTED;  // not used by ATADisk
	    sg.sr_callback = NULL;
	    sg.sr_sglist[0].sb_buf = pbBuf;
	    sg.sr_sglist[0].sb_len = dwBytePerChuck;
	
		if (!DeviceIoControl(hStore, DISK_IOCTL_READ, (LPVOID)&sg, sizeof(SG_REQ), NULL, 0, NULL, NULL))
		{
			NKDbgPrintfW(L"ERR:SDMMC_READ()\r\n");
	        return FALSE;
		}
		
		count += dwSectorPerChuck;
	}
	
	if( pbBuf != NULL )
	{	
		free(pbBuf);
		pbBuf = NULL;
	}	
	CloseHandle(hStore);
		
	return bRet;
}


BOOL SDMMC_READ2(DWORD *pdwTestSector)
{
	DWORD dwRemainByte = TEST_SIZE_BYTE;
	DWORD dwTestBlockByte = (*pdwTestSector)*512;
	DWORD dwNumBytes = 0;
	BOOL bRet = TRUE;
	HANDLE	hFile = INVALID_HANDLE_VALUE;
	
	DWORD time1 = 0,time2 = 0,time3 = 0,TransferRate = 0;
	BYTE *pbBuf = NULL;	
	
	hFile = CreateFile( SDMMC_PATH_TEST_FILE, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if( hFile != INVALID_HANDLE_VALUE )
	{

		if(pbBuf == NULL)
		{
			pbBuf = (BYTE *)malloc(dwTestBlockByte);
			if(pbBuf == NULL)
			{
				NKDbgPrintfW(L"malloc fail !! insufficient memory available\r\n");
				return FALSE;
			}
			///memset( pbBuf, 0xA, dwTestBlockByte);

		}		

		time1 = GetTickCount();
		while(1)
		{			
			if( dwRemainByte <= 0 )
			{
				time2 = GetTickCount();
		    	time3 = (time2 - time1);
		    	NKDbgPrintfW(L"time1:%d time2:%d time3:%d \r\n", time1, time2, time3);
		    	TransferRate = TEST_SIZE_BYTE/time3;
		    	NKDbgPrintfW(L"TransferRate : %d Bytes/s.\r\n", TransferRate*1000);
		    	*pdwTestSector = (TransferRate*1000)/1024;	
				break;
			}
			
			bRet = ReadFile(hFile, pbBuf, dwTestBlockByte, &dwNumBytes, NULL);
			dwRemainByte = dwRemainByte - dwTestBlockByte;
		}///while()	

	}
	else
	{
		bRet = FALSE;
	}	

	CloseHandle(hFile);
	
	if( pbBuf != NULL )
	{	
		free(pbBuf);
		pbBuf = NULL;
	}	
	
	return bRet;	
	
}


BOOL SDMMC_WRITE2(DWORD *pdwTestSector, BOOL bCreateFile)
{
	DWORD dwRemainByte = TEST_SIZE_BYTE;
	DWORD dwTestBlockByte = (*pdwTestSector)*512;
	DWORD dwNumBytes = 0;
	BOOL bRet = TRUE;
	HANDLE	hFile = INVALID_HANDLE_VALUE;
	
	DWORD time1 = 0,time2 = 0,time3 = 0,TransferRate = 0;
	BYTE *pbBuf = NULL;	
	
	hFile = CreateFile( SDMMC_PATH_TEST_FILE, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if( hFile != INVALID_HANDLE_VALUE )
	{

		if(pbBuf == NULL)
		{
			pbBuf = (BYTE *)malloc(dwTestBlockByte);
			if(pbBuf == NULL)
			{
				NKDbgPrintfW(L"malloc fail !! insufficient memory available\r\n");
				return FALSE;
			}
			memset( pbBuf, 0xA, dwTestBlockByte);

		}		

		time1 = GetTickCount();
		while(1)
		{			
			if( dwRemainByte <= 0 )
			{
				time2 = GetTickCount();
		    	time3 = (time2 - time1);
		    	NKDbgPrintfW(L"time1:%d time2:%d time3:%d \r\n", time1, time2, time3);
		    	TransferRate = TEST_SIZE_BYTE/time3;
		    	NKDbgPrintfW(L"TransferRate : %d Bytes/s.\r\n", TransferRate*1000);
		    	*pdwTestSector = (TransferRate*1000)/1024;	
				break;
			}
			
			bRet = WriteFile(hFile, pbBuf, dwTestBlockByte, &dwNumBytes, NULL);
			dwRemainByte = dwRemainByte - dwTestBlockByte;
		}///while()	

	}
	else
	{
		bRet = FALSE;
	}	

	CloseHandle(hFile);
	
	if( !bCreateFile )
	{	
		bRet = DeleteFile( SDMMC_PATH_TEST_FILE );
		if( !bRet )
			NKDbgPrintfW(L"Can not deletefile\r\n");
	}	

	if( pbBuf != NULL )
	{	
		free(pbBuf);
		pbBuf = NULL;
	}	
	
	return bRet;	
}	



#if 0
// Mesage handler for the Backlight box.
LRESULT CALLBACK Backlight(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	HANDLE hCreateThread = NULL;
	
	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT, TEXT("Click it!"));
			SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Click it!"));
			SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT3, TEXT("50"));
			return TRUE;

		case WM_COMMAND:
			///IDC_RADIO_A1
			if ((LOWORD(wParam) == IDC_BACKLIGHT_AUTO) )
			{	
				BACKLIGHT_AUTO( hDlg );
				///hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)BACKLIGHT_AUTO, &hDlg, 0, NULL);
				///CloseHandle(hCreateThread);
				return TRUE;
			}
			if ((LOWORD(wParam) == IDC_BACKLIGHT_AUTO2) )
			{
				BACKLIGHT_AUTO2( hDlg );
				///hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)BACKLIGHT_AUTO2, &hDlg, 0, NULL);
				///CloseHandle(hCreateThread);
				return TRUE;					
			}
		
			if ((LOWORD(wParam) == IDC_BACKLIGHT_DOWN) )
			{
				BACKLIGHT_SETTING( hDlg , (DWORD)(LOWORD(wParam)) );
				return TRUE;					
			}
			if ((LOWORD(wParam) == IDC_BACKLIGHT_DOWN2) )
			{
				BACKLIGHT_SETTING( hDlg , (DWORD)(LOWORD(wParam)) );
				return TRUE;					
			}
			if ((LOWORD(wParam) == IDC_BACKLIGHT_UP) )
			{
				BACKLIGHT_SETTING( hDlg , (DWORD)(LOWORD(wParam)) );
				return TRUE;					
			}
			if ((LOWORD(wParam) == IDC_BACKLIGHT_UP2) )
			{
				BACKLIGHT_SETTING( hDlg , (DWORD)(LOWORD(wParam)) );
				return TRUE;					
			}
			if ((LOWORD(wParam) == IDC_BACKLIGHT_SET) )
			{
				BACKLIGHT_SETTING( hDlg , (DWORD)(LOWORD(wParam)) );
				return TRUE;					
			}

			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
#endif 

#if 0
typedef struct {
    HWND hwndSlider;
    DWORD dwRegAddr;
    DWORD dwRegVolMask;
    DWORD dwRegMuteMask;
} Volume_Content;


// Mesage handler for the Volume box.
LRESULT CALLBACK Volume(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	static Volume_Content SliderContentArrary[9];
	
	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			
			///MONO
			SliderContentArrary[0].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER1);
			SliderContentArrary[0].dwRegAddr = 0x0008;
			SliderContentArrary[0].dwRegVolMask = 0x003F;
			SliderContentArrary[0].dwRegMuteMask = 0x0080;
			///DACL
			SliderContentArrary[1].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER2);
			SliderContentArrary[1].dwRegAddr = 0x000C;
			SliderContentArrary[1].dwRegVolMask = 0x1F00;
			SliderContentArrary[1].dwRegMuteMask = 0xE000;
			///DACR
			SliderContentArrary[2].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER3);
			SliderContentArrary[2].dwRegAddr = 0x000C;
			SliderContentArrary[2].dwRegVolMask = 0x001F;
			SliderContentArrary[2].dwRegMuteMask = 0xE000;
			///HPL
			SliderContentArrary[3].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER4);
			SliderContentArrary[3].dwRegAddr = 0x0004;
			SliderContentArrary[3].dwRegVolMask = 0x3F00;
			SliderContentArrary[3].dwRegMuteMask = 0x8000;
			///HPR
			SliderContentArrary[4].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER5);
			SliderContentArrary[4].dwRegAddr = 0x0004;
			SliderContentArrary[4].dwRegVolMask = 0x003F;
			SliderContentArrary[4].dwRegMuteMask = 0x0080;
			///SPKL
			SliderContentArrary[5].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER6);
			SliderContentArrary[5].dwRegAddr = 0x0002;
			SliderContentArrary[5].dwRegVolMask = 0x3F00;
			SliderContentArrary[5].dwRegMuteMask = 0x8000;
			///SPKR
			SliderContentArrary[6].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER7);
			SliderContentArrary[6].dwRegAddr = 0x0002;
			SliderContentArrary[6].dwRegVolMask = 0x003F;
			SliderContentArrary[6].dwRegMuteMask = 0x0080;
			///MICL
			SliderContentArrary[7].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER8);
			SliderContentArrary[7].dwRegAddr = 0x0012;
			SliderContentArrary[7].dwRegVolMask = 0x3F00;
			SliderContentArrary[7].dwRegMuteMask = 0x8000;
			///MICR
			SliderContentArrary[8].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER9);
			SliderContentArrary[8].dwRegAddr = 0x0012;
			SliderContentArrary[8].dwRegVolMask = 0x003F;
			SliderContentArrary[8].dwRegMuteMask = 0x8000;
			
			///adjust slider1
			SendMessage(SliderContentArrary[0].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[0].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[0].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[0].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider2
			SendMessage(SliderContentArrary[1].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 31));	// range
			SendMessage(SliderContentArrary[1].hwndSlider,TBM_SETTICFREQ,2,0L);				// tick freq.
			SendMessage(SliderContentArrary[1].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[1].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider3
			SendMessage(SliderContentArrary[2].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 31));	// range
			SendMessage(SliderContentArrary[2].hwndSlider,TBM_SETTICFREQ,2,0L);				// tick freq.
			SendMessage(SliderContentArrary[2].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[2].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider4
			SendMessage(SliderContentArrary[3].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[3].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[3].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[3].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider5
			SendMessage(SliderContentArrary[4].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[4].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[4].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[4].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider6
			SendMessage(SliderContentArrary[5].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[5].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[5].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[5].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider7
			SendMessage(SliderContentArrary[6].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[6].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[6].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[6].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider8
			SendMessage(SliderContentArrary[7].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[7].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[7].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[7].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider9
			SendMessage(SliderContentArrary[8].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[8].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[8].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[8].hwndSlider,TBM_SETPOS,TRUE,dwValue);   


			SetDlgItemText(hDlg, IDC_VOLUME_EDIT1, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT2, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT3, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT4, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT5, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT6, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT7, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT8, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT9, TEXT("??"));
			
			return TRUE;

		case WM_HSCROLL:            // track bar message
		case WM_VSCROLL:            // track bar message
			DWORD deIndex;
			DWORD dwValue;
			TCHAR szString[64];         // Temperary string
			DWORD dwAddr, dwVal, dwRet, dwSuccess;
			
			switch LOWORD(wParam) 
			{
				case TB_BOTTOM:
				case TB_THUMBPOSITION:
				case TB_LINEUP:
				case TB_LINEDOWN:
				case TB_PAGEUP:
				case TB_PAGEDOWN:
				case TB_TOP:
				case TB_THUMBTRACK:
				case TB_ENDTRACK:
					///switch (lParam)
					{
						if( (HWND)lParam == SliderContentArrary[0].hwndSlider )
						{	
							///it's MONO volume
							deIndex = 0;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK1, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT1, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT1, szString);		
								}
							}
							else
							{
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT1, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT1, szString);		
								}
							}	
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[1].hwndSlider )
						{	
							///it's DACL volume
							deIndex = 1;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK2, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT2, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT2, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT2, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue<<8&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT2, szString);		
								}
							}		
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[2].hwndSlider )	
						{	
							///it's DACR volume
							deIndex = 2;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK3, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT3, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT3, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT3, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT3, szString);		
								}
							}							
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[3].hwndSlider )	
						{	
							///it's HPL volume
							deIndex = 3;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK4, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT4, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT4, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT4, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|((dwValue<<8)&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT4, szString);		
								}
							}							
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[4].hwndSlider )	
						{	
							///it's HPR volume
							deIndex = 4;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK5, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT5, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT5, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT5, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT5, szString);		
								}
							}							
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[5].hwndSlider )	
						{	
							///it's SPKL volume
							deIndex = 5;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK6, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT6, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT6, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT6, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|((dwValue<<8)&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT6, szString);
								}
							}							
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[6].hwndSlider )	
						{	
							///it's SPKR volume
							deIndex = 6;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK7, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT7, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT7, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT7, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT7, szString);		
								}
							}							
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[7].hwndSlider )
						{	
							///it's MICL volume
							deIndex = 7;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK8, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT8, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT8, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT8, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue<<8&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT8, szString);		
								}
							}		
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[8].hwndSlider )	
						{	
							///it's MICR volume
							deIndex = 8;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK9, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT9, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT9, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT9, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT9, szString);		
								}
							}							
							break;
						}
						else
						{		
							;
						}	
					}///switch ((HWND)lParam)
					
///	SendMessage(GetDlgItem(hDlg,IDC_STATIC1),WM_SETTEXT,0,(LPARAM)szStatic);

					return TRUE;
				default:
					// Default case
					return FALSE;
			}
			break;

		case WM_COMMAND:

			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
#endif

void WINAPI HDDCommandThread(HWND hDlg)
{
	DWORD dwWaitRet = WAIT_FAILED;
	while(1)
	{
		dwWaitRet = WaitForSingleObject( hHDDCommandEvent, INFINITE );
		
		if( dwWaitRet == WAIT_OBJECT_0 )
		{
			///parse command
			if( dwHDDCommand == IDC_HDD_AUTO_TEST )
			{
				TCHAR szString[64];         // Temperary string
				BOOL bRet;
				int iLoop = 0;

				while(bHDDCmdRun)
				{
					wsprintf (szString, TEXT("Auto Testing !!"));
					SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
					
					iLoop++;
					
					if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_COPYFILE_CHECK, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
					{
						/// copyfile check box checked
						DWORD dwSecPerChunk;
						
						///prepare source file for copyfile test
						dwSecPerChunk = 512;	///256k
						bRet = HDD_WRITE2(&dwSecPerChunk, TRUE);
						
						if(bRet)
						{	
							///start copyfile test
							bRet = CopyFile( HDD_PATH_TEST_FILE, HDD_PATH_TEST_FILE1, FALSE);
							if( bRet == 0 )
							{	
								NKDbgPrintfW(L"CopyFile fail %d !!!! \r\n", GetLastError() );
								Sleep(3000);
								bRet = CopyFile( HDD_PATH_TEST_FILE, HDD_PATH_TEST_FILE1, FALSE);
								if( bRet == 0 )
								{	
									NKDbgPrintfW(L"CopyFile re-try fail %d!!!! \r\n", GetLastError() );
									wsprintf (szString, TEXT("CopyFile-%d Fail %d"), iLoop, GetLastError());
									SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
									Sleep(1000);
								}	
							}	
							else
							{
								wsprintf (szString, TEXT("CopyFile-%d OK"), iLoop);
								SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);											
							}
						}	
						///move the screen to the last message		
						DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_GETCOUNT, 0, 0);							
						SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

						UpdateWindow(hDlg);
						///delete target file
						DeleteFile( HDD_PATH_TEST_FILE1 );
						///delete source file
						DeleteFile( HDD_PATH_TEST_FILE );						
						
					}	///IDC_COPYFILE_CHECK
					
					if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_READIO_CHECK, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
					{
						int iLoopio;
						DWORD dwSecPerChunk, dwTemp;
						
						wsprintf (szString, TEXT("Read IO !!"));
						SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
						UpdateWindow(hDlg);
						
						for( iLoopio = 0; iLoopio<10; iLoopio++ )
						{
							dwSecPerChunk = dwSECTOR_PER_CHUNK[iLoopio];		///1k
							dwTemp = dwSecPerChunk;
							HDD_READ(&dwSecPerChunk);
							NKDbgPrintfW(L"Driver Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
							///SendDlgItemMessage(hDlg, IDC_HDD_EDIT1, EM_LINEINDEX, &dwTemp2, 0);
							wsprintf (szString, TEXT("R%d:%d KB/s @ %d K bulk."),iLoopio, dwSecPerChunk, dwTemp/2);
							///SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
							SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							///SendMessage(GetDlgItem(hDlg, IDC_HDD_LIST1), LB_ADDSTRING, 0, (LPARAM)szString); 
							///ListBox_AddString(GetDlgItem(hDlg,IDC_HDD_LIST1 ), szString );
		
							///move the screen to the last message		
							DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_GETCOUNT, 0, 0);							
							SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
							
							UpdateWindow(hDlg);						
						}
					}	///IDC_READIO_CHECK
					
					if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_READFILE_CHECK, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
					{
						/// copyfile check box checked
						DWORD dwSecPerChunk;
						int iLoop1;
						DWORD dwTemp;
						
						///prepare source file for read file test
						dwSecPerChunk = 512;	///256k
						HDD_WRITE2(&dwSecPerChunk, TRUE);

						wsprintf (szString, TEXT("Read file !!"));
						SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
						UpdateWindow(hDlg);

						for( iLoop1 = 0; iLoop1<10; iLoop1++ )
						{
							dwSecPerChunk = dwSECTOR_PER_CHUNK[iLoop1];		///1k
							dwTemp = dwSecPerChunk;
							HDD_READ2(&dwSecPerChunk);
							NKDbgPrintfW(L"File Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
							wsprintf (szString, TEXT("Rf%d:%d KB/s @ %d K bulk."),iLoop1, dwSecPerChunk, dwTemp/2);
							SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

							///move the screen to the last message		
							DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_GETCOUNT, 0, 0);							
							SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

							UpdateWindow(hDlg);
						}


						///delete source file
						DeleteFile( HDD_PATH_TEST_FILE );												
					}	///IDC_READFILE_CHECK
					
					if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_WRITEFILE_CHECK, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
					{
						/// write files check box checked
						DWORD dwSecPerChunk;
						DWORD dwTemp;
						///DWORD dwTemp2 = 0;
						int iLoop2 = 0;						
						///TCHAR szString[64];         // Temperary string
		
						wsprintf (szString, TEXT("Write file !!"));
						SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
						UpdateWindow(hDlg);
		
						for( iLoop2 = 0; iLoop2<10; iLoop2++ )
						{
							dwSecPerChunk = dwSECTOR_PER_CHUNK[iLoop2];		///1k				
							dwTemp = dwSecPerChunk;
							HDD_WRITE2(&dwSecPerChunk, FALSE);
							NKDbgPrintfW(L"Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
							///SendDlgItemMessage(hDlg, IDC_HDD_EDIT1, EM_LINEINDEX, &dwTemp2, 0);
							wsprintf (szString, TEXT("Wf%d:%d KB/s @ %d K bulk."),iLoop2, dwSecPerChunk, dwTemp/2);
							///SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
							SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							///SendMessage(GetDlgItem(hDlg, IDC_HDD_LIST1), LB_ADDSTRING, 0, (LPARAM)szString); 
							///ListBox_AddString(GetDlgItem(hDlg,IDC_HDD_LIST1 ), szString );
		
							///move the screen to the last message		
							DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_GETCOUNT, 0, 0);							
							SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
		
							UpdateWindow(hDlg);
						}	

						///delete source file
						DeleteFile( HDD_PATH_TEST_FILE );												

					}	///IDC_WRITEFILE_CHECK

					wsprintf (szString, TEXT("Testing done %d!!"), iLoop);
					SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);				

															
				}	///while(bHDDCmdRun)
				
				Sleep(1000);
				///wsprintf (szString, TEXT("Testing done!!"));
				///SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);				
				
			}	///if( dwHDDCommand == IDC_HDD_AUTO_TEST )
			else
			if( dwHDDCommand == IDC_HDD_COPY )
			{
			}				
			else
			if( dwHDDCommand == IDC_HDD_READ_IO )
			{
			}				
			else
			if( dwHDDCommand == IDC_HDD_READ )
			{
			}				
			else
			if( dwHDDCommand == IDC_HDD_WRITE )
			{
			}	
		}
		else
		if( dwWaitRet == WAIT_TIMEOUT )
		{
		}						
	}
}


// Message handler for the HardDrive box.
LRESULT CALLBACK HardDrive(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	DWORD dwSECTOR_PER_CHUNK[10] = {2,4,8,16,32,64,128,256,512,1024};
	TCHAR szString[64];         // Temperary string

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}


			if( NULL == hHDDCommandEvent )
			{
				hHDDCommandEvent = CreateEvent(NULL, FALSE, FALSE, NAMED_EVENT_HDD_COMMAND);
			}
			///if Command thread is not run
			if( NULL == hHDDCommandThread )
			{
				hHDDCommandThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HDDCommandThread, (LPVOID)hDlg, 0, NULL );
				if ( !hHDDCommandThread )
					;
				else
					CloseHandle(hHDDCommandThread);							
			}	
					
			return TRUE;
		
		///case WM_DEIVCECHANGE:
		case 0x0219:
			wsprintf (szString, TEXT("WM_DEIVCECHANGE %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_SD_REMOVED:
			wsprintf (szString, TEXT("WM_OEM_SD_REMOVED %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_SD_INSERT:
			wsprintf (szString, TEXT("WM_OEM_SD_INSERT %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_COMMAND:

			if ((LOWORD(wParam) == IDC_HDD_AUTOSUSPEND_CHECK) )
			{
				HANDLE hMPM = INVALID_HANDLE_VALUE;		/// span0111
				hMPM = CreateFile( _T("MPM1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				DWORD dwAuto = 0;
				
				///check if the checkbox has been checked
				///if( BST_CHECKED == SendMessage(GetDlgItem(hDlg, IDC_CHECK_CHANGE_GO), BM_GETCHECK , NULL, NULL) )
				if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_HDD_AUTOSUSPEND_CHECK, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
				{	
					gbAutoSuspend = TRUE;
					///info system driver
					dwAuto = 1234;
					if( hMPM && !DeviceIoControl(hMPM, IOCTL_OEM_IBOOT_RESET_DEVICE, &dwAuto, sizeof(DWORD), NULL, 0, NULL, NULL) )
					{
		    			///SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("IOCTL_HAL_REBOOT Fail !!")); 
					}
				}	
				else
				{	
					gbAutoSuspend = FALSE;
					///info system driver
					dwAuto = 4321;
					if( hMPM && !DeviceIoControl(hMPM, IOCTL_OEM_IBOOT_RESET_DEVICE, &dwAuto, sizeof(DWORD), NULL, 0, NULL, NULL) )
					{
		    			///SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("IOCTL_HAL_REBOOT Fail !!")); 
					}
				}	

				///if autosuspend thread is not run
				if(gbAutoSuspend)
				{		
					if( NULL == hAutoSuspendThread )
					{
						hAutoSuspendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AutoSuspendThread, (LPVOID)NULL, 0, NULL );
						if ( !hAutoSuspendThread )
							;
						else
							CloseHandle(hAutoSuspendThread);							
					}
				}	
			}
			else
			if ((LOWORD(wParam) == IDC_COPYFILE_CHECK) )
			{
				///check if the checkbox has been checked
				///if( BST_CHECKED == SendMessage(GetDlgItem(hDlg, IDC_CHECK_CHANGE_GO), BM_GETCHECK , NULL, NULL) )
///				if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_COPYFILE_CHECK, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
///					SendDlgItemMessage(hDlg, IDC_COPYFILE_CHECK, BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)NULL);
///				else
///					SendDlgItemMessage(hDlg, IDC_COPYFILE_CHECK, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)NULL);					
			}
			else
			if ((LOWORD(wParam) == IDC_READIO_CHECK) )
			{
///				if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_READIO_CHECK, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
///					SendDlgItemMessage(hDlg, IDC_READIO_CHECK, BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)NULL);
///				else
///					SendDlgItemMessage(hDlg, IDC_READIO_CHECK, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)NULL);					
			}
			else			
			if ((LOWORD(wParam) == IDC_READFILE_CHECK) )
			{
///				if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_READFILE_CHECK, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
///					SendDlgItemMessage(hDlg, IDC_READFILE_CHECK, BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)NULL);
///				else
///					SendDlgItemMessage(hDlg, IDC_READFILE_CHECK, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)NULL);									
			}
			else			
			if ((LOWORD(wParam) == IDC_WRITEFILE_CHECK) )
			{
///				if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_WRITEFILE_CHECK, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
///					SendDlgItemMessage(hDlg, IDC_WRITEFILE_CHECK, BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)NULL);
///				else
///					SendDlgItemMessage(hDlg, IDC_WRITEFILE_CHECK, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)NULL);									
			}
			else			
			if ((LOWORD(wParam) == IDC_HDD_AUTO_TEST) )
			{
				if( bHDDCmdRun )
				{
					bHDDCmdRun = FALSE;
					///switch button caption
					///wsprintf (szString, TEXT("Start Test"));
					///SendDlgItemMessage(hDlg, IDC_HDD_AUTO_TEST, BM_SETSTATE , (WPARAM)TRUE, (LPARAM)0);
					///enable all check box
					///SendDlgItemMessage(hDlg, IDC_COPYFILE_CHECK, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)NULL);
					///SendDlgItemMessage(hDlg, IDC_READIO_CHECK, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)NULL);
					///SendDlgItemMessage(hDlg, IDC_READFILE_CHECK, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)NULL);
					///SendDlgItemMessage(hDlg, IDC_WRITEFILE_CHECK, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)NULL);
					
				}
				else
				{		
					bHDDCmdRun = TRUE;
					dwHDDCommand = IDC_HDD_AUTO_TEST;					
					SetEvent(hHDDCommandEvent);
					///SendDlgItemMessage(hDlg, IDC_HDD_AUTO_TEST, BM_SETSTATE , (WPARAM)FALSE, (LPARAM)0);
				}	
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_HDD_OFF) )
			{			
				///hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)BACKLIGHT_AUTO2, &hDlg, 0, NULL);
				///CloseHandle(hCreateThread);
				HDD_ON_OFF(FALSE);
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_HDD_ON) )
			{			
				///hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)BACKLIGHT_AUTO2, &hDlg, 0, NULL);
				///CloseHandle(hCreateThread);
				HDD_ON_OFF(TRUE);
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_HDD_COPY) )
			{
					
				DWORD dwSecPerChunk = 0;
				DWORD dwTemp = 0;
				int iLoop = 0;
				
				for( iLoop = 0; iLoop<5; iLoop++ )
				{
					SetCursor(LoadCursor(NULL,IDC_WAIT));
					///Create temp files
					dwSecPerChunk = 512;	///256k
					HDD_WRITE2(&dwSecPerChunk, TRUE);
									
					HDD_COPY(&dwSecPerChunk);
					dwTemp = dwTemp + dwSecPerChunk;
					NKDbgPrintfW(L"Copy Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
					wsprintf (szString, TEXT("Copy%d:%d KB/s"), iLoop, dwSecPerChunk);
					SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

					///move the screen to the last message		
					DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_GETCOUNT, 0, 0);							
					SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

					UpdateWindow(hDlg);
					SetCursor(LoadCursor(NULL,IDC_SIZEWE));
				}

				NKDbgPrintfW(L"Average Copy Transfer Rate @ %d KBytes/Second. \r\n",dwTemp/5);
				wsprintf (szString, TEXT("Average Copy:%d KB/s"), dwTemp/5);
				SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				UpdateWindow(hDlg);

				return TRUE;					
			}
			
			if ((LOWORD(wParam) == IDC_HDD_READ) )
			{
				
				DWORD dwSecPerChunk;
				DWORD dwTemp;
				DWORD dwTemp2 = 0;
				int iLoop = 0;				
				///TCHAR szString[64];         // Temperary string

				wsprintf (szString, TEXT("Reading !!"));
				SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
				UpdateWindow(hDlg);
				
				
				for( iLoop = 0; iLoop<10; iLoop++ )
				{
					dwSecPerChunk = dwSECTOR_PER_CHUNK[iLoop];		///1k
					dwTemp = dwSecPerChunk;
					HDD_READ(&dwSecPerChunk);
					NKDbgPrintfW(L"Driver Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
					///SendDlgItemMessage(hDlg, IDC_HDD_EDIT1, EM_LINEINDEX, &dwTemp2, 0);
					wsprintf (szString, TEXT("R%d:%d KB/s @ %d K bulk."),iLoop, dwSecPerChunk, dwTemp/2);
					///SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
					SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					///SendMessage(GetDlgItem(hDlg, IDC_HDD_LIST1), LB_ADDSTRING, 0, (LPARAM)szString); 
					///ListBox_AddString(GetDlgItem(hDlg,IDC_HDD_LIST1 ), szString );

					///move the screen to the last message		
					DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_GETCOUNT, 0, 0);							
					SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
					
					UpdateWindow(hDlg);						
				}
				

				///Create temp files
				dwSecPerChunk = 512;	///256k
				dwTemp = dwSecPerChunk;
				HDD_WRITE2(&dwSecPerChunk, TRUE);

				for( iLoop = 0; iLoop<10; iLoop++ )
				{
					dwSecPerChunk = dwSECTOR_PER_CHUNK[iLoop];		///1k
					dwTemp = dwSecPerChunk;
					HDD_READ2(&dwSecPerChunk);
					NKDbgPrintfW(L"File Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
					wsprintf (szString, TEXT("Rf%d:%d KB/s @ %d K bulk."),iLoop, dwSecPerChunk, dwTemp/2);
					SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

					///move the screen to the last message		
					DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_GETCOUNT, 0, 0);							
					SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

					UpdateWindow(hDlg);
				}

				wsprintf (szString, TEXT("Test done"));
				SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
				UpdateWindow(hDlg);
				///hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)BACKLIGHT_AUTO2, &hDlg, 0, NULL);
				///CloseHandle(hCreateThread);
				
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_HDD_WRITE) )
			{
				
				DWORD dwSecPerChunk;
				DWORD dwTemp;
				DWORD dwTemp2 = 0;
				int iLoop = 0;						
				///TCHAR szString[64];         // Temperary string

				wsprintf (szString, TEXT("Writing !!"));
				SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
				UpdateWindow(hDlg);

				for( iLoop = 0; iLoop<10; iLoop++ )
				{
					dwSecPerChunk = dwSECTOR_PER_CHUNK[iLoop];		///1k				
					dwTemp = dwSecPerChunk;
					HDD_WRITE2(&dwSecPerChunk, FALSE);
					NKDbgPrintfW(L"Transfer Rate @ %d KBytes/Second. \r\n",dwSecPerChunk);
					///SendDlgItemMessage(hDlg, IDC_HDD_EDIT1, EM_LINEINDEX, &dwTemp2, 0);
					wsprintf (szString, TEXT("Wf%d:%d KB/s @ %d K bulk."),iLoop, dwSecPerChunk, dwTemp/2);
					///SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
					SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					///SendMessage(GetDlgItem(hDlg, IDC_HDD_LIST1), LB_ADDSTRING, 0, (LPARAM)szString); 
					///ListBox_AddString(GetDlgItem(hDlg,IDC_HDD_LIST1 ), szString );

					///move the screen to the last message		
					DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_GETCOUNT, 0, 0);							
					SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

					UpdateWindow(hDlg);
				}	
				
				wsprintf (szString, TEXT("Test done"));
				SetDlgItemText(hDlg, IDC_HDD_EDIT1, szString);
				UpdateWindow(hDlg);
				
				return TRUE;					
			}					
			if ((LOWORD(wParam) == IDC_HDD_CLS) )
			{
				///LONG lResult;
				do
				{
					SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );
				///while((lResult=SendDlgItemMessage(hDlg, IDC_HDD_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0)) != LB_ERR );				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				bHDDCmdRun = FALSE;
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

BOOL HDD_ON_OFF( BOOL bOn )
{
	BOOL bRet = TRUE;
	HANDLE hStore = NULL;
	
	if(hStore == NULL)
	{
		hStore = CreateFile( TEXT("DSK1:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if(hStore == INVALID_HANDLE_VALUE)
		{  	
			NKDbgPrintfW(L"CreateFile DSK1: fail !!\r\n");		
			CloseHandle(hStore);
			return FALSE;
		}
	}

	if(bOn)
	{	
		DeviceIoControl(hStore, IOCTL_OEM_DISK_POWER_UP, NULL, 0, NULL, 0, NULL, NULL);
		NKDbgPrintfW(L"DSK1: IOCTL_OEM_DISK_POWER_UP !!\r\n");		
	}
	else	
	{	
		DeviceIoControl(hStore, IOCTL_OEM_DISK_POWER_DOWN, NULL, 0, NULL, 0, NULL, NULL);
		NKDbgPrintfW(L"DSK1: IOCTL_OEM_DISK_POWER_DOWN !!\r\n");		
	}
	
	CloseHandle(hStore);	
	
	return bRet;		
}

BOOL HDD_COPY(DWORD *pdwTestSector)
{
	BOOL bRet = TRUE;	
	DWORD time1 = 0,time2 = 0,time3 = 0,TransferRate = 0;	
	
	time1 = GetTickCount();
	bRet = CopyFile( HDD_PATH_TEST_FILE, HDD_PATH_TEST_FILE1, FALSE);
	time2 = GetTickCount();
	time3 = (time2 - time1);
	NKDbgPrintfW(L"time1:%d time2:%d time3:%d \r\n", time1, time2, time3);
	TransferRate = TEST_SIZE_BYTE/time3;
	NKDbgPrintfW(L"TransferRate : %d Bytes/s.\r\n", TransferRate*1000);
	*pdwTestSector = (TransferRate*1000)/1024;	

	DeleteFile( HDD_PATH_TEST_FILE );
	DeleteFile( HDD_PATH_TEST_FILE1 );
	
	return bRet;	
}


BOOL HDD_READ(DWORD *pdwTestSector)
{
#define TEST_CHUNKS		1024

	DWORD time1 = 0,time2 = 0,time3 = 0,TransferRate = 0;
	BOOL bRet = TRUE; 
	HANDLE hStore = NULL;
	SG_REQ sg;
	DWORD	count = 0;
	BYTE *pbBuf = NULL;
	DWORD dwSectorPerChuck = 0;
	DWORD dwBytePerChuck = 0;
	DWORD dwChuckPerTest = 0;
	
	dwSectorPerChuck = (*pdwTestSector);
	dwBytePerChuck = dwSectorPerChuck*512;
	dwChuckPerTest = TEST_CHUNKS;
		
	NKDbgPrintfW(L"HDD_READ: Sec/Chunk:%d Byte/Chunk:%d Chunk/Test:%d.\r\n", dwSectorPerChuck, dwBytePerChuck, dwChuckPerTest);
	
	if(hStore == NULL)
	{
		hStore = CreateFile( TEXT("DSK1:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if(hStore == INVALID_HANDLE_VALUE)
		{  	
			NKDbgPrintfW(L"CreateFile DSK1: fail !!\r\n");		
			CloseHandle(hStore);
			return FALSE;
		}
	}

	if(pbBuf == NULL)
	{
		pbBuf = (BYTE *)malloc(dwBytePerChuck);
		if(pbBuf == NULL)
		{
			NKDbgPrintfW(L"malloc fail !! insufficient memory available\r\n");
			return FALSE;
		}
	}		
	
	///DeviceIoControl(hStore, IOCTL_OEM_DISK_POWER_UP, NULL, 0, NULL, 0, NULL, NULL);
		
	time1 = GetTickCount();
	while(1)
	{
		if( count > dwChuckPerTest*dwSectorPerChuck )
		{
			time2 = GetTickCount();
		    time3 = (time2 - time1);
		    NKDbgPrintfW(L"time1:%d time2:%d time3:%d \r\n", time1, time2, time3);
		    TransferRate = (dwChuckPerTest*dwBytePerChuck)/time3;
		    NKDbgPrintfW(L"TransferRate : %d \r\n", TransferRate*1000);
		    *pdwTestSector = (TransferRate*1000)/1024;
			break;
		}
				
	    sg.sr_start = (count+2048);
	    sg.sr_num_sec = dwSectorPerChuck;
	    sg.sr_num_sg = 1;
	    sg.sr_status = ERROR_NOT_SUPPORTED;  // not used by ATADisk
	    sg.sr_callback = NULL;
	    sg.sr_sglist[0].sb_buf = pbBuf;
	    sg.sr_sglist[0].sb_len = dwBytePerChuck;
	
		if (!DeviceIoControl(hStore, DISK_IOCTL_READ, (LPVOID)&sg, sizeof(SG_REQ), NULL, 0, NULL, NULL))
		{
			NKDbgPrintfW(L"ERR:HDD_READ()\r\n");
	        return FALSE;
		}
		
		count += dwSectorPerChuck;
	}
	
	if( pbBuf != NULL )
	{	
		free(pbBuf);
		pbBuf = NULL;
	}	
	CloseHandle(hStore);
		
	return bRet;
}


BOOL HDD_READ2(DWORD *pdwTestSector)
{
	DWORD dwRemainByte = TEST_SIZE_BYTE;
	DWORD dwTestBlockByte = (*pdwTestSector)*512;
	DWORD dwNumBytes = 0;
	BOOL bRet = TRUE;
	HANDLE	hFile = INVALID_HANDLE_VALUE;
	
	DWORD time1 = 0,time2 = 0,time3 = 0,TransferRate = 0;
	BYTE *pbBuf = NULL;	
	
	hFile = CreateFile( HDD_PATH_TEST_FILE, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if( hFile != INVALID_HANDLE_VALUE )
	{

		if(pbBuf == NULL)
		{
			pbBuf = (BYTE *)malloc(dwTestBlockByte);
			if(pbBuf == NULL)
			{
				NKDbgPrintfW(L"malloc fail !! insufficient memory available\r\n");
				return FALSE;
			}
			///memset( pbBuf, 0xA, dwTestBlockByte);

		}		

		time1 = GetTickCount();
		while(1)
		{			
			if( dwRemainByte <= 0 )
			{
				time2 = GetTickCount();
		    	time3 = (time2 - time1);
		    	NKDbgPrintfW(L"time1:%d time2:%d time3:%d \r\n", time1, time2, time3);
		    	TransferRate = TEST_SIZE_BYTE/time3;
		    	NKDbgPrintfW(L"TransferRate : %d Bytes/s.\r\n", TransferRate*1000);
		    	*pdwTestSector = (TransferRate*1000)/1024;	
				break;
			}
			
			bRet = ReadFile(hFile, pbBuf, dwTestBlockByte, &dwNumBytes, NULL);
			dwRemainByte = dwRemainByte - dwTestBlockByte;
		}///while()	

	}
	else
	{
		bRet = FALSE;
	}	

	CloseHandle(hFile);
	
	if( pbBuf != NULL )
	{	
		free(pbBuf);
		pbBuf = NULL;
	}	
	
	return bRet;	
	
}


BOOL HDD_WRITE2(DWORD *pdwTestSector, BOOL bCreateFile)
{
	DWORD dwRemainByte = TEST_SIZE_BYTE;
	DWORD dwTestBlockByte = (*pdwTestSector)*512;
	DWORD dwNumBytes = 0;
	BOOL bRet = TRUE;
	HANDLE	hFile = INVALID_HANDLE_VALUE;
	
	DWORD time1 = 0,time2 = 0,time3 = 0,TransferRate = 0;
	BYTE *pbBuf = NULL;	
	
	hFile = CreateFile( HDD_PATH_TEST_FILE, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if( hFile != INVALID_HANDLE_VALUE )
	{
		if(pbBuf == NULL)
		{
			pbBuf = (BYTE *)malloc(dwTestBlockByte);
			if(pbBuf == NULL)
			{
				NKDbgPrintfW(L"malloc fail !! insufficient memory available\r\n");
				return FALSE;
			}
			memset( pbBuf, 0xA, dwTestBlockByte);
		}		

		time1 = GetTickCount();
		while(1)
		{			
			if( dwRemainByte <= 0 )
			{
				time2 = GetTickCount();
		    	time3 = (time2 - time1);
		    	NKDbgPrintfW(L"time1:%d time2:%d time3:%d \r\n", time1, time2, time3);
		    	TransferRate = TEST_SIZE_BYTE/time3;
		    	NKDbgPrintfW(L"TransferRate : %d Bytes/s.\r\n", TransferRate*1000);
		    	*pdwTestSector = (TransferRate*1000)/1024;	
				break;
			}
			
			bRet = WriteFile(hFile, pbBuf, dwTestBlockByte, &dwNumBytes, NULL);
			if(bRet == 0)
			{	
				///re-try
				NKDbgPrintfW(L"HDD_WRITE2: %d(%d)WriteFile error %d !!!\r\n", dwNumBytes, dwTestBlockByte, GetLastError());
				Sleep(5000);
				bRet = WriteFile(hFile, pbBuf, dwTestBlockByte, &dwNumBytes, NULL);
				if(bRet == 0)
				{	
					NKDbgPrintfW(L"HDD_WRITE2: %d(%d)WriteFile re-try error %d !!!\r\n", dwNumBytes, dwTestBlockByte, GetLastError());
					bRet = FALSE;
					break;
				}	
			}	
		
			dwRemainByte = dwRemainByte - dwTestBlockByte;
		}///while()	

	}
	else
	{
		bRet = FALSE;
		NKDbgPrintfW(L"HDD_WRITE2: CreateFile error %d !!!\r\n", GetLastError());
	}	

	CloseHandle(hFile);
	
	if( !bCreateFile )
	{	
		bRet = DeleteFile( HDD_PATH_TEST_FILE );
		if( !bRet )
			NKDbgPrintfW(L"Can not deletefile\r\n");
	}	

	if( pbBuf != NULL )
	{	
		free(pbBuf);
		pbBuf = NULL;
	}	
	
	return bRet;	
}	

#if 0
void BACKLIGHT_SETTING( HWND hDlg , DWORD dwSelect)
{
	TCHAR szString[64];         // Temperary string
  	HANDLE hLGT = INVALID_HANDLE_VALUE;		/// span0111
  	BOOL bDoHWcontrol = FALSE;
  	static DWORD dwBrightness = 50;
  	///DWORD dwWait = 0;
	///DWORD dwLoop = 0;
	DWORD dwInBuf = 0;
	DWORD dwOutBuf = 0;
	DWORD dwOutLan = sizeof(DWORD);

	HANDLE hHandle = NULL;
	
	///disable the screen off timer
	hHandle = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE);		///span0417
	if( NULL == hHandle )
	{
		NKDbgPrintfW(TEXT("ERR:CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE) failed!!!\r\n"));
	}
	else
	if( 0 == SetEvent(hHandle) )
	{
		NKDbgPrintfW(L"ERR:SetEvent() EVENT_OEM_SCREEN_TIMER_DISABLE failed!!! error code =%d.\r\n", GetLastError());	
	}
	CloseHandle(hHandle);		
	///disable the screen off timer
	
	///get backlight device handle 
	hLGT = CreateFile( _T("LGT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare

	if( INVALID_HANDLE_VALUE != hLGT ) {
	}
	else
	{	
		NKDbgPrintfW( L"ERROR !!! CreateFile LGT1 fail.\r\n" );
		///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("ERROR !!! CreateFile LGT1 fail !!!"));
		SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)TEXT("ERROR1 !!!"));
		UpdateWindow(hDlg);
		return;
	}	
	///get backlight device handle 

	///parsing the command
	switch(dwSelect)
	{
		case IDC_BACKLIGHT_DOWN:
			dwBrightness = dwBrightness - 1;
	   		wsprintf (szString, TEXT("%d"), dwBrightness);
   			SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)szString);

			break;
		case IDC_BACKLIGHT_DOWN2:
			dwBrightness = dwBrightness - 5;			
	   		wsprintf (szString, TEXT("%d"), dwBrightness);
   			SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)szString);

			break;
		case IDC_BACKLIGHT_UP:			
			dwBrightness = dwBrightness + 1;			
	   		wsprintf (szString, TEXT("%d"), dwBrightness);
   			SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)szString);
			
			break;
		case IDC_BACKLIGHT_UP2:
			dwBrightness = dwBrightness + 5;			
	   		wsprintf (szString, TEXT("%d"), dwBrightness);
   			SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)szString);
			
			break;
		case IDC_BACKLIGHT_SET:
			bDoHWcontrol = TRUE;
			break;
		default:
			break;			
	}

	if( !bDoHWcontrol )
	{
		///check if the "change and go" checkbox has been checked
		if( BST_CHECKED == SendMessage(GetDlgItem(hDlg, IDC_CHECK_CHANGE_GO), BM_GETCHECK , NULL, NULL) )
			bDoHWcontrol = TRUE;
	}	
	
	///
	if( bDoHWcontrol )
	{
		dwInBuf = dwBrightness;
	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	Sleep(50);
   		DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
   		wsprintf (szString, TEXT("%d/%d"), dwInBuf, dwOutBuf);
   		SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)szString);
   		UpdateWindow(hDlg);
	}


}


void BACKLIGHT_AUTO( HWND hDlg )
{
	//TCHAR szString[64];         // Temperary string
	HANDLE hLGT = INVALID_HANDLE_VALUE;		/// span0111
	BOOL fTESTING = TRUE;
	DWORD dwWait = 0;

	HANDLE hHandle = NULL;
	hHandle = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE);		///span0417
	if( NULL == hHandle )
	{
		NKDbgPrintfW(TEXT("ERR:CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE) failed!!!\r\n"));
	}
	else
	if( 0 == SetEvent(hHandle) )
	{
		NKDbgPrintfW(L"ERR:SetEvent() EVENT_OEM_SCREEN_TIMER_DISABLE failed!!! error code =%d.\r\n", GetLastError());	
	}
	CloseHandle(hHandle);		

	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT, TEXT("Auto Backlight Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT), WM_SETTEXT, NULL, (LPARAM)TEXT("Auto Backlight Testing !!!"));
	UpdateWindow(hDlg);

	hLGT = CreateFile( _T("LGT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare

	if( INVALID_HANDLE_VALUE != hLGT ) {
	}
	else
	{	
		NKDbgPrintfW( L"CreateFile LGT1 fail.\r\n" );
		SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT, TEXT("ERROR !!! CreateFile LGT1 fail !!!"));
		UpdateWindow(hDlg);
	}	
	
	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT, TEXT("Backlight glitter Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT), WM_SETTEXT, NULL, (LPARAM)TEXT("Backlight glitter Testing !!!"));
	UpdateWindow(hDlg);
	
	dwWait = 1000;
	fTESTING = TRUE;
	while(fTESTING)
	{	
		DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
		Sleep(dwWait);
		DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_OFF, NULL, 0, NULL, 0, NULL, NULL);
		Sleep(dwWait);
		dwWait = dwWait - 100;
		if( dwWait <= 0)
			fTESTING = FALSE;
		AddToProgressBar(hDlg, dwWait/10);	
	}	

	dwWait = 1000;
	fTESTING = TRUE;
	while(fTESTING)
	{	
		DeviceIoControl(hLGT, IOCTL_OEM_LCD_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
		DeviceIoControl(hLGT, IOCTL_OEM_KEYPAD_BACKLIGHT_OFF, NULL, 0, NULL, 0, NULL, NULL);
		Sleep(dwWait);
		DeviceIoControl(hLGT, IOCTL_OEM_LCD_BACKLIGHT_OFF, NULL, 0, NULL, 0, NULL, NULL);
		DeviceIoControl(hLGT, IOCTL_OEM_KEYPAD_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
		Sleep(dwWait);
		dwWait = dwWait - 100;
		if( dwWait <= 0)
			fTESTING = FALSE;
		AddToProgressBar(hDlg, dwWait/10);	
	}
 		    	
	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	CloseHandle(hLGT);
	
	SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT, TEXT("Backlight glitter done!"));
	

}


void BACKLIGHT_AUTO2( HWND hDlg )
{
	TCHAR szString[64];         // Temperary string
  	HANDLE hLGT = INVALID_HANDLE_VALUE;		/// span0111
  	BOOL fTESTING = TRUE;
  	DWORD dwWait = 0;
	DWORD dwLoop = 0;
	DWORD dwInBuf = 0;
	DWORD dwOutBuf = 0;
	DWORD dwOutLan = sizeof(DWORD);

	HANDLE hHandle = NULL;
	hHandle = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE);		///span0417
	if( NULL == hHandle )
	{
		NKDbgPrintfW(TEXT("ERR:CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE) failed!!!\r\n"));
	}
	else
	if( 0 == SetEvent(hHandle) )
	{
		NKDbgPrintfW(L"ERR:SetEvent() EVENT_OEM_SCREEN_TIMER_DISABLE failed!!! error code =%d.\r\n", GetLastError());	
	}
	CloseHandle(hHandle);		

	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Auto brightness Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)TEXT("Auto brightness Testing !!!"));
	UpdateWindow(hDlg);

	hLGT = CreateFile( _T("LGT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare

	if( INVALID_HANDLE_VALUE != hLGT ) {
	}
	else
	{	
		NKDbgPrintfW( L"ERROR !!! CreateFile LGT1 fail.\r\n" );
		///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("ERROR !!! CreateFile LGT1 fail !!!"));
		SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)TEXT("ERROR !!! CreateFile LGT1 fail !!!"));
		UpdateWindow(hDlg);
		return;
	}	

	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Brightness Sequent Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)TEXT("Brightness Sequent Testing !!!"));
	NKDbgPrintfW(L"\r\nBrightness Sequent Testing !!!\r\n");
	Sleep(500);	
	dwWait = 100;
	dwLoop = 200;
	fTESTING = TRUE;
	dwInBuf = 100;
	while(fTESTING)
	{	
	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	Sleep(50);
	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	wsprintf (szString, TEXT("Sequent Testing %d - %d"), dwInBuf, dwOutBuf);
	   	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, szString);
	   	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)szString);
	   	UpdateWindow(hDlg);
	   	Sleep(dwWait);
	   	if( dwLoop <= 100)
	   		dwInBuf++;
	   	else
	   		dwInBuf--;		   		
   		dwLoop--;
	   	if( dwLoop <= 0)
	   		fTESTING = FALSE;
	   	NKDbgPrintfW(L".");
	   	AddToProgressBar(hDlg, dwLoop/2);
	}

	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Brightness Random Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)TEXT("Brightness Random Testing !!!"));
	UpdateWindow(hDlg);
	NKDbgPrintfW(L"\r\nBrightness Random Testing !!!\r\n");
	dwWait = 100;
	dwLoop = 200;
	fTESTING = TRUE;
	while(fTESTING)
	{	
		do{
			dwInBuf = (Random() & 0x800000FF);
			Sleep(1);
		}
		while( dwInBuf<0 || dwInBuf>100);

	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	Sleep(50);
	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	wsprintf (szString, TEXT("Random Testing %d - %d"), dwInBuf, dwOutBuf);
	   	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, szString);
	   	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)szString);
	   	UpdateWindow(hDlg);
	   	Sleep(dwWait);
	   	dwLoop--;
	   	if( dwLoop <= 0)
	   		fTESTING = FALSE;
	   	NKDbgPrintfW(L".");	
	   	AddToProgressBar(hDlg, dwLoop/2);
	}

	
	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Brightness Error Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)TEXT("Brightness Error Testing !!!"));
	UpdateWindow(hDlg);
	NKDbgPrintfW(L"\r\nBrightness Error Testing !!!\r\n");
	dwWait = 100;
	dwLoop = 200;
	fTESTING = TRUE;
	while(fTESTING)
	{	
		dwInBuf = (Random() & 0x800000FF);

	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	Sleep(50);
	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	wsprintf (szString, TEXT("Error Testing %d - %d"), dwInBuf, dwOutBuf);
	   	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, szString);
	   	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)szString);
	   	UpdateWindow(hDlg);
	   	Sleep(dwWait);
	   	dwLoop--;
	   	if( dwLoop <= 0)
	   		fTESTING = FALSE;
	   	NKDbgPrintfW(L".");	
	   	AddToProgressBar(hDlg, dwLoop/2);
	}
 		    	
	dwInBuf = 100;
	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);	
	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	CloseHandle(hLGT);
	
	SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Brightness Test done!"));
	UpdateWindow(hDlg);
	NKDbgPrintfW(L"\r\nBrightness Testing Done!!!\r\n");

}
#endif


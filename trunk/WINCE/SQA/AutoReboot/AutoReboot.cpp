// AutoReboot.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "winioctl.h"
#include "AutoReboot.h"
#include <commctrl.h>
#include "TestClass.h"

extern "C" BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);

#define MAX_LOADSTRING 100

#define IOCTL_HAL_REBOOT		CTL_CODE(FILE_DEVICE_HAL, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define MIC_AUTOREBOOT_REG_KEY			(TEXT("\\Software\\Mitac\\micAutoReboot"))
#define MIC_AUTOREBOOT_REBOOTTIMES		(TEXT("RebootTimes"))
#define MIC_AUTOREBOOT_REBOOTDELAY		(TEXT("RebootDelay"))

#define MIC_STARTER_REG_KEY				(TEXT("\\Software\\Mitac\\micStarter"))
#define MIC_STARTER_AUTOREBOOT_NAME		(TEXT("AutoReboot"))
#define MIC_STARTER_AUTOREBOOT_FILE		(TEXT("AutoReboot.exe"))

#define MIC_STARTER_INIT_VALUENAME		(TEXT("Launch99"))
#define MIC_STARTER_INIT_CLEAR_VALUE	(TEXT("-"))
#define MIC_STARTER_INIT_FILE_VALUE		(TEXT("micStarter.exe"))


#define DEVICEFILE			 _T("\\AutoReboot.log")
#define DEVICEFILE1			 _T("\\AutoReboot.log\0")
///#define STORAGEFILE			 _T("\\SDMMC Card\\AutoReboot.log")
#define STORAGEFILE			 _T("\\Storage Card\\AutoReboot.log")
///#define STARTERFILE			 _T("\\SDMMC Card\\micStarter.exe")
#define STARTERFILE			 _T("\\Storage Card\\micStarter.exe")


// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hWindow;		// The window handle
HWND				hwndCB;			// The command bar handle
SLogClass 			*pLog = NULL;
SShowMsgClass 		*pMsg = NULL;
TCHAR				szTempString[256];
HANDLE				hMonitorThread;
DWORD				dwRebootTimes = 0;		/// query from Reg	[HKEY_LOCAL_MACHINE\Software\Mitac\micAutoReboot]
DWORD				dwRebootDelay = 3000;		/// query from Reg



// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	AutoReboot		(HWND, UINT, WPARAM, LPARAM);

static DWORD DialogBoxThreadProc(LPVOID lpvoid);
static DWORD MonitorThreadProc(LPVOID lpvoid);

BOOL _InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Auto reboot system test\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
	
	wsprintf( szTempString, _T("This test will test system reboot times\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );
	
	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	///wsprintf( szTempString, _T("\\SDMMC Card\\AutoReboot.log\0") );
	wsprintf( szTempString, DEVICEFILE1 );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}

BOOL InitMsg()
{
	BOOL bRet;
	
	if( NULL == pMsg )
		pMsg = new SShowMsgClass();

	if( NULL == pMsg )
		bRet = FALSE;
	else
		bRet = TRUE;

	///NKDbgPrintfW( TEXT("InitMsg %s !\r\n"), bRet?L"Ok":L"Fail" );
	return bRet;
}

BOOL InitLog()
{
	BOOL bRet;
	
	if( NULL == pLog )
		pLog = new SLogClass();
		
	if( NULL != pLog )
		bRet = _InitLog();
	else
		bRet = FALSE;

	///NKDbgPrintfW( TEXT("InitLog %s !\r\n"), bRet?L"Ok":L"Fail" );
	return bRet;
}

void CheckStarterRegistrySetting()
{
	HKEY hKey;
	BYTE pValueData[256];
	DWORD dwValueType = 0;
	DWORD dwValueLen = 0;
	DWORD dwValueData = 0;
	DWORD dwDisp = 0;
	
		///check if we have starter key	
	if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, MIC_STARTER_REG_KEY, 0, 0, &hKey ) )
	{	///add the execution file for starter to run
		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );		
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, MIC_STARTER_AUTOREBOOT_NAME, NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
	    	pMsg->MsgOut( _T("AutoReboot: Load execution file name=%s"), pValueData );		
	    }
	    else
	    {	///add it! if we do not have it.
	    	dwValueType = REG_SZ;
	    	dwValueLen = sizeof(TCHAR)*(1+_tcslen(MIC_STARTER_AUTOREBOOT_FILE));
	    	if( ERROR_SUCCESS != RegSetValueEx( hKey, MIC_STARTER_AUTOREBOOT_NAME, NULL, dwValueType, (BYTE *)MIC_STARTER_AUTOREBOOT_FILE, dwValueLen ) )
	    	{	
	    		pMsg->MsgOut( _T("AutoReboot: Set execution file name=%s fail"), MIC_STARTER_AUTOREBOOT_FILE );
	    		NKDbgPrintfW( TEXT("AutoReboot: Set execution file name=%s fail\r\n"), MIC_STARTER_AUTOREBOOT_FILE );
	    	}	
			else
				pMsg->MsgOut( _T("AutoReboot: Set execution file name=%s Ok"), MIC_STARTER_AUTOREBOOT_FILE );		
	    }

		CloseHandle(hKey);
	}
	else
	{	///starter key can not open ! we do not have starter on system 
		NKDbgPrintfW( TEXT("AutoReboot: system do not support micStarter !\r\n") );
		pMsg->MsgOut( _T("AutoReboot: system do not support micStarter !") );
		
		///TODO: try to add one to system !!
		///create starter key
    	if( ERROR_SUCCESS != RegCreateKeyEx( HKEY_LOCAL_MACHINE, MIC_STARTER_REG_KEY, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp ) )
    	{
    		pMsg->MsgOut( _T("AutoReboot: Create Reg key %s fail!"), MIC_STARTER_REG_KEY );
    		NKDbgPrintfW( TEXT("AutoReboot: Create Reg key %s fail!\r\n"), MIC_STARTER_REG_KEY );
    		return;
    	}    			
		
		///add the execution file for starter to run
    	dwValueType = REG_SZ;
    	dwValueLen = sizeof(TCHAR)*(1+_tcslen(MIC_STARTER_AUTOREBOOT_FILE));
	    if( ERROR_SUCCESS != RegSetValueEx( hKey, MIC_STARTER_AUTOREBOOT_NAME, NULL, dwValueType, (BYTE *)MIC_STARTER_AUTOREBOOT_FILE, dwValueLen ) )
	    {	
	    	pMsg->MsgOut( _T("AutoReboot: Set execution file name=%s fail"), MIC_STARTER_AUTOREBOOT_FILE );
	    	NKDbgPrintfW( TEXT("AutoReboot: Set execution file name=%s fail\r\n"), MIC_STARTER_AUTOREBOOT_FILE );
	    }	
		else
			pMsg->MsgOut( _T("AutoReboot: Set execution file name=%s Ok"), MIC_STARTER_AUTOREBOOT_FILE );		
			
		CloseHandle(hKey);

		///open system init key
		if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, TEXT("init"), 0, 0, &hKey ) )
		{	///add launch in system init
	    	dwValueType = REG_SZ;
    		dwValueLen = sizeof(TCHAR)*(1+_tcslen(MIC_STARTER_INIT_FILE_VALUE));
	    	if( ERROR_SUCCESS != RegSetValueEx( hKey, MIC_STARTER_INIT_VALUENAME, NULL, dwValueType, (BYTE *)MIC_STARTER_INIT_FILE_VALUE, dwValueLen ) )
	    	{	
	    		pMsg->MsgOut( _T("AutoReboot: add %s execution file name=%s fail"), MIC_STARTER_INIT_VALUENAME, MIC_STARTER_INIT_FILE_VALUE );
	    		NKDbgPrintfW( TEXT("AutoReboot: add %s execution file name=%s fail\r\n"), MIC_STARTER_INIT_VALUENAME, MIC_STARTER_INIT_FILE_VALUE );
	    	}	
			else
				pMsg->MsgOut( _T("AutoReboot: add %s execution file name=%s Ok"), MIC_STARTER_INIT_VALUENAME, MIC_STARTER_INIT_FILE_VALUE );
		}
		else
		{
	    	pMsg->MsgOut( _T("AutoReboot: open key %s fail"), TEXT("init") );
	    	NKDbgPrintfW( TEXT("AutoReboot: open key %s fail"), TEXT("init") );
		}
		
		CloseHandle(hKey);
		
		///copy from storage
		CopyFile( STARTERFILE, _T("\\micStarter.exe"), FALSE );
		
	}
	
}

void GetRegistrySetting()
{
	HKEY hKey;
	///BYTE pValueData[256];
	DWORD dwValueType = 0;
	DWORD dwValueLen = 0;
	DWORD dwValueData = 0;
	DWORD dwDisp = 0;
	
	if( ERROR_SUCCESS != RegOpenKeyEx( HKEY_LOCAL_MACHINE, MIC_AUTOREBOOT_REG_KEY, 0, 0, &hKey ) )
    {	///add it! if we do not have it.
    	if( ERROR_SUCCESS != RegCreateKeyEx( HKEY_LOCAL_MACHINE, MIC_AUTOREBOOT_REG_KEY, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp ) )
    	{
    		pMsg->MsgOut( _T("AutoReboot: Set Reg key %s fail!"), MIC_AUTOREBOOT_REG_KEY );
    		NKDbgPrintfW( TEXT("AutoReboot: Set Reg key %s fail!\r\n"), MIC_AUTOREBOOT_REG_KEY );
    		return;
    	}    			
    }

	///check value in key
	dwValueType = REG_DWORD;
	dwValueLen = sizeof( dwValueData );
    if( ERROR_SUCCESS == RegQueryValueEx( hKey, MIC_AUTOREBOOT_REBOOTTIMES, NULL, &dwValueType, (BYTE *)&dwValueData, &dwValueLen ) )
    {
    	pMsg->MsgOut( _T("AutoReboot: Load AutoReboot %s value=%d"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData );
    	dwRebootTimes = dwValueData;
    }
    else
    {	///add value if we do not have it
		dwValueType = REG_DWORD;
		dwValueLen = sizeof( dwValueData );
		dwValueData = dwRebootTimes;
		if( ERROR_SUCCESS != RegSetValueEx( hKey, MIC_AUTOREBOOT_REBOOTTIMES, NULL, dwValueType, (BYTE *)&dwValueData, dwValueLen ) )
	    {	
	    	pMsg->MsgOut( _T("AutoReboot: Set %s value=%d fail"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData );
	    	NKDbgPrintfW( TEXT("AutoReboot: Set %s value=%d fail\r\n"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData  );
	    }	
		else
			pMsg->MsgOut( _T("AutoReboot: Set %s value=%d Ok"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData );	
    }

	///check value in key
	dwValueType = REG_DWORD;
	dwValueLen = sizeof( dwValueData );
    if( ERROR_SUCCESS == RegQueryValueEx( hKey, MIC_AUTOREBOOT_REBOOTDELAY, NULL, &dwValueType, (BYTE *)&dwValueData, &dwValueLen ) )
    {
    	pMsg->MsgOut( _T("AutoReboot: Load AutoReboot %s value=%d"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData );
    	dwRebootDelay = dwValueData;
    }
    else
    {	///add value if we do not have it
		dwValueType = REG_DWORD;
		dwValueLen = sizeof( dwValueData );
		dwValueData = dwRebootDelay;
		if( ERROR_SUCCESS != RegSetValueEx( hKey, MIC_AUTOREBOOT_REBOOTDELAY, NULL, dwValueType, (BYTE *)&dwValueData, dwValueLen ) )
	    {	
	    	pMsg->MsgOut( _T("AutoReboot: Set %s value=%d fail"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData );
	    	NKDbgPrintfW( TEXT("AutoReboot: Set %s value=%d fail\r\n"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData  );
	    }	
		else
			pMsg->MsgOut( _T("AutoReboot: Set %s value=%d Ok"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData );	
    }

	CloseHandle(hKey);
	
}

void UpdateRegistrySetting()
{
	HKEY hKey;
	///BYTE pValueData[256];
	DWORD dwValueType = 0;
	DWORD dwValueLen = 0;
	DWORD dwValueData = 0;
	DWORD dwDisp = 0;
	
	if( ERROR_SUCCESS != RegOpenKeyEx( HKEY_LOCAL_MACHINE, MIC_AUTOREBOOT_REG_KEY, 0, 0, &hKey ) )
    {	///add it! if we do not have it.
    	if( ERROR_SUCCESS != RegCreateKeyEx( HKEY_LOCAL_MACHINE, MIC_AUTOREBOOT_REG_KEY, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp ) )
    	{
    		pMsg->MsgOut( _T("AutoReboot: Set Reg key %s fail!"), MIC_AUTOREBOOT_REG_KEY );
    		NKDbgPrintfW( TEXT("AutoReboot: Set Reg key %s fail!\r\n"), MIC_AUTOREBOOT_REG_KEY );
    		return;
    	}    			
    }

	///check value in key
	dwValueType = REG_DWORD;
	dwValueLen = sizeof( dwValueData );
    if( ERROR_SUCCESS == RegQueryValueEx( hKey, MIC_AUTOREBOOT_REBOOTTIMES, NULL, &dwValueType, (BYTE *)&dwValueData, &dwValueLen ) )
    {
    	pMsg->MsgOut( _T("AutoReboot: Load AutoReboot %s value=%d"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData );
    	///dwRebootTimes = dwValueData;
    	
    	///update it
		dwValueType = REG_DWORD;
		dwValueLen = sizeof( dwValueData );
		dwValueData = dwRebootTimes;
		if( ERROR_SUCCESS != RegSetValueEx( hKey, MIC_AUTOREBOOT_REBOOTTIMES, NULL, dwValueType, (BYTE *)&dwValueData, dwValueLen ) )
	    {	
	    	pMsg->MsgOut( _T("AutoReboot: update %s value=%d fail"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData );
	    	NKDbgPrintfW( TEXT("AutoReboot: update %s value=%d fail\r\n"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData  );
	    }	
		else
			pMsg->MsgOut( _T("AutoReboot: update %s value=%d Ok"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData );	

    }
    else
    {	///add value if we do not have it
		dwValueType = REG_DWORD;
		dwValueLen = sizeof( dwValueData );
		dwValueData = dwRebootTimes;
		if( ERROR_SUCCESS != RegSetValueEx( hKey, MIC_AUTOREBOOT_REBOOTTIMES, NULL, dwValueType, (BYTE *)&dwValueData, dwValueLen ) )
	    {	
	    	pMsg->MsgOut( _T("AutoReboot: Set %s value=%d fail"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData );
	    	NKDbgPrintfW( TEXT("AutoReboot: Set %s value=%d fail\r\n"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData  );
	    }	
		else
			pMsg->MsgOut( _T("AutoReboot: Set %s value=%d Ok"), MIC_AUTOREBOOT_REBOOTTIMES, dwValueData );	
    }

	///check value in key
	dwValueType = REG_DWORD;
	dwValueLen = sizeof( dwValueData );
    if( ERROR_SUCCESS == RegQueryValueEx( hKey, MIC_AUTOREBOOT_REBOOTDELAY, NULL, &dwValueType, (BYTE *)&dwValueData, &dwValueLen ) )
    {
    	pMsg->MsgOut( _T("AutoReboot: Load AutoReboot %s value=%d"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData );
    	///dwRebootDelay = dwValueData;
    	
    	///update it
		dwValueType = REG_DWORD;
		dwValueLen = sizeof( dwValueData );
		dwValueData = dwRebootDelay;
		if( ERROR_SUCCESS != RegSetValueEx( hKey, MIC_AUTOREBOOT_REBOOTDELAY, NULL, dwValueType, (BYTE *)&dwValueData, dwValueLen ) )
	    {	
	    	pMsg->MsgOut( _T("AutoReboot: update %s value=%d fail"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData );
	    	NKDbgPrintfW( TEXT("AutoReboot: update %s value=%d fail\r\n"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData  );
	    }	
		else
			pMsg->MsgOut( _T("AutoReboot: update %s value=%d Ok"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData );	

    }
    else
    {	///add value if we do not have it
		dwValueType = REG_DWORD;
		dwValueLen = sizeof( dwValueData );
		dwValueData = dwRebootDelay;
		if( ERROR_SUCCESS != RegSetValueEx( hKey, MIC_AUTOREBOOT_REBOOTDELAY, NULL, dwValueType, (BYTE *)&dwValueData, dwValueLen ) )
	    {	
	    	pMsg->MsgOut( _T("AutoReboot: Set %s value=%d fail"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData );
	    	NKDbgPrintfW( TEXT("AutoReboot: Set %s value=%d fail\r\n"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData  );
	    }	
		else
			pMsg->MsgOut( _T("AutoReboot: Set %s value=%d Ok"), MIC_AUTOREBOOT_REBOOTDELAY, dwValueData );	
    }

	CloseHandle(hKey);
	
}

///get the info of menu item
static BOOL spGetMenuItemInfo( UINT uiItemID, MENUITEMINFO *pMii )
{
	BOOL bRet = FALSE;
	HMENU hMenuBar = NULL;
	
	if( NULL == hwndCB )	///error handle
	{
		pMsg->MsgOut( dFAIL, _T("!!!spGetMenuItemInfo error hwndCB\0") );	
		return bRet;
	}

	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );

	if( hMenuBar != NULL )
	{
	    memset( pMii, 0, sizeof(MENUITEMINFO));
    	pMii->cbSize = sizeof(MENUITEMINFO);
    	pMii->fMask = (MIIM_CHECKMARKS|MIIM_DATA|MIIM_ID|MIIM_STATE|MIIM_SUBMENU|MIIM_TYPE);
    	
		bRet = GetMenuItemInfo( hMenuBar, uiItemID, FALSE, pMii );
	}	
	else
		pMsg->MsgOut( dFAIL, _T("!!!spGetMenuItemInfo error hMenuBar\0") );	
	
	return bRet;
}

///check if the menu item is checked
static BOOL spIsMenuItemChecked( UINT uiItemID )
{
	BOOL bRet = FALSE;
	MENUITEMINFO menuIteminfo;
		
	if( spGetMenuItemInfo( uiItemID, &menuIteminfo ) )
	{
		bRet = (menuIteminfo.fState & MFS_CHECKED)?TRUE:FALSE;
	}
	else
	{	/// error handle
		pMsg->MsgOut( dFAIL, _T("!!!spIsMenuItemChecked error\0") );
	}	

	return bRet;
}

///invert menu item checked status
static BOOL spInvertMenuItemCheck( UINT uiItemID )
{
	BOOL bRet = FALSE;
	HMENU hMenuBar = NULL;
	
	if( NULL == hwndCB )	///error handle
	{
		pMsg->MsgOut( dFAIL, _T("!!!spInvertMenuItemCheck error hwndCB\0") );	
		return bRet;
	}	

	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
	
	if( hMenuBar != NULL )
	{	
		if( spIsMenuItemChecked( uiItemID ) )
			CheckMenuItem( hMenuBar, uiItemID, MF_BYCOMMAND|MF_UNCHECKED );
		else
			CheckMenuItem( hMenuBar, uiItemID, MF_BYCOMMAND|MF_CHECKED );
		bRet = TRUE;
	}
	else
	{	///error handle
		pMsg->MsgOut( dFAIL, _T("!!!spInvertMenuItemCheck error hMenuBar\0") );
	}
	
	return bRet;
}

static BOOL spUnCheckAllDelayItem()
{
	BOOL bRet = FALSE;
	HMENU hMenuBar = NULL;
	
	if( NULL == hwndCB )	///error handle
	{
		pMsg->MsgOut( dFAIL, _T("!!!spUnCheckAllDelayItem error hwndCB\0") );	
		return bRet;
	}	

	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
	
	if( hMenuBar == NULL )	///error handle
	{
		pMsg->MsgOut( dFAIL, _T("!!!spUnCheckAllDelayItem error hMenuBar\0") );	
		return bRet;
	}
	
	CheckMenuItem( hMenuBar, IDM_OPTION_DELAY_0, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_DELAY_3, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_DELAY_5, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_DELAY_8, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_DELAY_15, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_DELAY_20, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_DELAY_30, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_DELAY_60, MF_BYCOMMAND|MF_UNCHECKED );

	return bRet;	
}


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

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_AUTOREBOOT);

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
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AUTOREBOOT));
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
	LoadString(hInstance, IDC_AUTOREBOOT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}

	hWindow = hWnd;
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	if (hwndCB)
		CommandBar_Show(hwndCB, TRUE);

	///NKDbgPrintfW( TEXT("1\r\n") );
	if( InitMsg() )
	{	
		RECT rtWin;
		GetClientRect( hWnd, &rtWin );
		rtWin.top = rtWin.top + 24;		

		pMsg->SetShowAreaRect( rtWin );

		pMsg->SetFrameWindowHwnd( hWnd );
	}	
	///NKDbgPrintfW( TEXT("3\r\n") );
	if( InitLog() )
		pLog->EnableLog( TRUE );
	else	
		pMsg->MsgOut( _T("InitLog Fail !!") );

	///NKDbgPrintfW( TEXT("4\r\n") );
	CheckStarterRegistrySetting();
	GetRegistrySetting();
	
	return TRUE;
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
///	HDC hdc;
	int wmId, wmEvent;
///	PAINTSTRUCT ps;
///	TCHAR szHello[MAX_LOADSTRING];

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_HELP_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_FILE_START:
				   dwRebootTimes = 1;
				   break;
				case IDM_FILE_RESET:
				   dwRebootTimes = 0;
				   dwRebootDelay = 3000;
				   UpdateRegistrySetting();
				   spUnCheckAllDelayItem();
				   spInvertMenuItemCheck( IDM_OPTION_DELAY_3 );
				   break;
				case IDM_OPTION_DELAY_0:
				   dwRebootDelay = 0;
				   spUnCheckAllDelayItem();
				   spInvertMenuItemCheck( IDM_OPTION_DELAY_0 );
				   break;
				case IDM_OPTION_DELAY_3:
				   dwRebootDelay = 3000;
				   spUnCheckAllDelayItem();
				   spInvertMenuItemCheck( IDM_OPTION_DELAY_3 );
				   break;
				case IDM_OPTION_DELAY_5:
				   dwRebootDelay = 5000;
				   spUnCheckAllDelayItem();
				   spInvertMenuItemCheck( IDM_OPTION_DELAY_5 );
				   break;
				case IDM_OPTION_DELAY_8:
				   dwRebootDelay = 8000;
				   spUnCheckAllDelayItem();
				   spInvertMenuItemCheck( IDM_OPTION_DELAY_8 );
				   break;
				case IDM_OPTION_DELAY_15:
				   dwRebootDelay = 15000;
				   spUnCheckAllDelayItem();
				   spInvertMenuItemCheck( IDM_OPTION_DELAY_15 );
				   break;
				case IDM_OPTION_DELAY_20:
				   dwRebootDelay = 20000;
				   spUnCheckAllDelayItem();
				   spInvertMenuItemCheck( IDM_OPTION_DELAY_20 );
				   break;
				case IDM_OPTION_DELAY_30:
				   dwRebootDelay = 30000;
				   spUnCheckAllDelayItem();
				   spInvertMenuItemCheck( IDM_OPTION_DELAY_30 );
				   break;
				case IDM_OPTION_DELAY_60:
				   dwRebootDelay = 60000;
				   spUnCheckAllDelayItem();
				   spInvertMenuItemCheck( IDM_OPTION_DELAY_60 );
				   break;
				case IDM_FILE_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_CREATE:
			hwndCB = CommandBar_Create(hInst, hWnd, 1);			
			CommandBar_InsertMenubar(hwndCB, hInst, IDM_MENU, 0);
			CommandBar_AddAdornments(hwndCB, 0, 0);
			///NKDbgPrintfW( TEXT("5\r\n") );
			hMonitorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitorThreadProc, NULL, 0, NULL);
			
			break;
///		case WM_PAINT:
///			RECT rt;
///			hdc = BeginPaint(hWnd, &ps);
///			GetClientRect(hWnd, &rt);
///			LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
///			DrawText(hdc, szHello, _tcslen(szHello), &rt, 
///				DT_SINGLELINE | DT_VCENTER | DT_CENTER);
///			EndPaint(hWnd, &ps);
///			break;
		case WM_DESTROY:
			CommandBar_Destroy(hwndCB);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

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

// Mesage handler for the AutoReboot box.
LRESULT CALLBACK AutoReboot(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	TCHAR	szStatic[32];


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
			
			///show reboot times
			wsprintf( szStatic,_T("AutoReboot Times %d"), dwRebootTimes );
			SendMessage( GetDlgItem( hDlg, IDC_AUTOREBOOT_STATIC1 ), WM_SETTEXT ,0 ,(LPARAM)szStatic );
			
			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			else
			if ((LOWORD(wParam) == IDC_AUTOREBOOT_BUTTON1))
			{
				dwRebootTimes = 0;
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}

			break;
	}
    return FALSE;
}

static BOOL DoLog()
{
	BOOL bRet = TRUE;
	
	///copy from storage
	CopyFile( STORAGEFILE, DEVICEFILE, FALSE );
            	
	if( pLog )
	{	
		///pLog->WriteLog( _T("Auto Reboot ... 0x%08x\r\n"), GetTickCount() );
		pLog->WriteLog( _T("Auto Reboot ... %d\r\n"), dwRebootTimes );
		pLog->SaveLog();
		dwRebootTimes++;
	}
	if( pMsg )
		pMsg->MsgOut( _T("Auto Reboot ... %d\0"), dwRebootTimes );
	
	///copy back storage
	CopyFile( DEVICEFILE, STORAGEFILE, FALSE );
		
	///DeleteFile( DEVICEFILE );

	return bRet;
}

static DWORD DialogBoxThreadProc(LPVOID lpvoid)
{
	DialogBox(hInst, (LPCTSTR)IDD_AUTOREBOOTBOX, hWindow, (DLGPROC)AutoReboot);
	return 0;
}

static DWORD MonitorThreadProc(LPVOID lpvoid)
{

	while(1)
	{
		Sleep( 200 );

		if( 0 != dwRebootTimes )
		{	
			///DialogBox(hInst, (LPCTSTR)IDD_AUTOREBOOTBOX, hWindow, (DLGPROC)AutoReboot);
			hMonitorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DialogBoxThreadProc, NULL, 0, NULL);
			Sleep( dwRebootDelay );
		}
				
		if( 0 != dwRebootTimes )
		{
				
			DoLog();
			UpdateRegistrySetting();
			
			///reboot
			if( pMsg )
				pMsg->MsgOut( _T("Do Reboot ... %d\0"), dwRebootTimes );
			NKDbgPrintfW( TEXT("Do Reboot ... %d\r\n"), dwRebootTimes );	
			if( !KernelIoControl( IOCTL_HAL_REBOOT, NULL, 0, NULL, 0, NULL ) )
				NKDbgPrintfW( TEXT("IOCTL_HAL_REBOOT Fail !!! \r\n") );
		}		
	}
	
	return 0;
}
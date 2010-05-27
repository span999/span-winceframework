// PT310TV.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PT310TV.h"
#include "dtv_def.h"
#include <commctrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle


HANDLE hDTV = NULL;
TCHAR szString[128];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);




void Init(HWND hDlg)
{
	hDTV = CreateFile( L"DTV1:", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
	
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 13");///index 0
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 14");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 15");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 16");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 17");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 18");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 19");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 20");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 21");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 22");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 23");///index 10
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 24");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 25");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 26");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 27");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 28");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 29");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 30");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 31");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 32");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 33");///index 20
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 34");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 35");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 36");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 37");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 38");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 39");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 40");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 41");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 42");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 43");///index 30
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 44");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 45");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 46");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 47");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 48");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 49");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 50");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 51");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 52");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 53");///index 40
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 54");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 55");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 56");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 57");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 58");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 59");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 60");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 61");
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"CH 62");///index 49
    SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SETCURSEL, (WPARAM)10, 0);	///set default 23

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

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_PT310TV);

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
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PT310TV));
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
	LoadString(hInstance, IDC_PT310TV, szWindowClass, MAX_LOADSTRING);
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
	DWORD dwIoctlCode = 0;
	DWORD dwActSize = 0;

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
			
			Init(hDlg);
			
			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				dwIoctlCode = IOCTL_DTV_MODULE_VIDEO_ON;
		    	if( DeviceIoControl(hDTV, dwIoctlCode, NULL, 0, NULL, 0, &dwActSize, NULL) ) {
				    wsprintf( szString, TEXT("Video ON OK") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);       	
				} else {
				    wsprintf( szString, TEXT("Video ON Fail") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);       	
				}

				dwIoctlCode = IOCTL_DTV_MODULE_POWER_OFF;
		    	if( DeviceIoControl(hDTV, dwIoctlCode, NULL, 0, NULL, 0, NULL, NULL) ) {
					wsprintf( szString, TEXT("Power OFF OK") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				} else {
					wsprintf( szString, TEXT("Power OFF Fail") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				}

				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			
			if ( LOWORD(wParam) == IDC_BUTTON1 )
			{	///power on
				dwIoctlCode = IOCTL_DTV_MODULE_POWER_ON;
		    	if( DeviceIoControl(hDTV, dwIoctlCode, NULL, 0, NULL, 0, NULL, NULL) ) {
					wsprintf( szString, TEXT("Power ON OK") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				} else {
					wsprintf( szString, TEXT("Power ON Fail") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				}
				return TRUE;
			}
			
			if ( LOWORD(wParam) == IDC_BUTTON2 )
			{	///power off

				dwIoctlCode = IOCTL_DTV_MODULE_VIDEO_ON;
		    	if( DeviceIoControl(hDTV, dwIoctlCode, NULL, 0, NULL, 0, &dwActSize, NULL) ) {
				    wsprintf( szString, TEXT("Video ON OK") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);       	
				} else {
				    wsprintf( szString, TEXT("Video ON Fail") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);       	
				}

				dwIoctlCode = IOCTL_DTV_MODULE_POWER_OFF;
		    	if( DeviceIoControl(hDTV, dwIoctlCode, NULL, 0, NULL, 0, NULL, NULL) ) {
					wsprintf( szString, TEXT("Power OFF OK") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				} else {
					wsprintf( szString, TEXT("Power OFF Fail") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				}
				return TRUE;
			}

			if ( LOWORD(wParam) == IDC_BUTTON3 )
			{	///scan
				DWORD EnsembleIndex = 0;
				CHANNELSCAN ScanChannel;
				CHANNEL_INFO ChannelInfo;

				EnsembleIndex = SendMessage( GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0);
				ScanChannel.dwChannelNum = (EnsembleIndex+13);
				ScanChannel.dwScanTimeout = 4999;

				dwIoctlCode = IOCTL_DTV_CHANNEL_SCAN;
		    	if( DeviceIoControl(hDTV, dwIoctlCode, &ScanChannel, sizeof(ScanChannel), &ChannelInfo, sizeof(ChannelInfo), &dwActSize, NULL) ) {
			        if( ScanChannel.dwChannelNum != ChannelInfo.dwPhyChNum ) {
        				wsprintf( szString, TEXT("Channel %d no service"), (EnsembleIndex+13) );
        				SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			        } else {
        				wsprintf( szString, TEXT("Channel %d sync!"), (EnsembleIndex+13) );
        				SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
        			}
					wsprintf( szString, TEXT("Scan OK") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				} else {
					wsprintf( szString, TEXT("Scan Fail") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				}
				return TRUE;
			}

			if ( LOWORD(wParam) == IDC_BUTTON4 )
			{	///set channel
				DWORD EnsembleIndex = 0;
				ISDB_T_SETCHANNEL ChServ;

				EnsembleIndex = SendMessage( GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0);
    			ChServ.dwPhyChNum = (EnsembleIndex+13);
    			ChServ.dwServiceID = 0;
    			
				dwIoctlCode = IOCTL_DTV_CHANNEL_SET;
		    	if( DeviceIoControl(hDTV, dwIoctlCode, &ChServ, sizeof(ChServ), NULL, 0, &dwActSize, NULL) ) {
				    wsprintf( szString, TEXT("SetChannel %d OK"), (EnsembleIndex+13) );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);       	
				} else {
				    wsprintf( szString, TEXT("SetChannel %d Fail"), (EnsembleIndex+13) );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);       	
				}
				return TRUE;
			}

			if ( LOWORD(wParam) == IDC_BUTTON5 )
			{	///video on
    			DTV_VIDEO_DIM NewSize;
    
    			NewSize.dwX = 15;
    			NewSize.dwY = 10;
    			DeviceIoControl(hDTV, IOCTL_DTV_SET_VIDEO_OUT_POSTION, &NewSize, sizeof(NewSize), NULL, 0, &dwActSize, NULL);
    
    			NewSize.dwX = 240;
    			NewSize.dwY = 136;
				DeviceIoControl(hDTV, IOCTL_DTV_SET_VIDEO_OUT_SIZE, &NewSize, sizeof(NewSize), NULL, 0, &dwActSize, NULL);
				
				dwIoctlCode = IOCTL_DTV_MODULE_VIDEO_ON;
		    	if( DeviceIoControl(hDTV, dwIoctlCode, NULL, 0, NULL, 0, &dwActSize, NULL) ) {
				    wsprintf( szString, TEXT("Video ON OK") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);       	
				} else {
				    wsprintf( szString, TEXT("Video ON Fail") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);       	
				}
				return TRUE;
			}

			if ( LOWORD(wParam) == IDC_BUTTON6 )
			{	///get RF
    			ISDB_T_SIGNALSTATUS SignalStatus;
    			SignalStatus.dwSize = sizeof(SignalStatus);
    			SignalStatus.dwBEN = 0;
    			SignalStatus.dwBEC = 999999;
				dwIoctlCode = IOCTL_DTV_MODULE_SIGNAL_STATUS;
				
		    	if( DeviceIoControl(hDTV, dwIoctlCode, &SignalStatus, sizeof(SignalStatus), NULL, 0, &dwActSize, NULL) ) {
				    wsprintf( szString, TEXT("Get RF OK") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			        
					wsprintf( szString, TEXT("AGC=%d"), SignalStatus.dwAGC );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					wsprintf( szString, TEXT("CN=%lf"), SignalStatus.dbCN );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					wsprintf( szString, TEXT("BEN=%d"), SignalStatus.dwBEN );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					wsprintf( szString, TEXT("BEC=%d"), SignalStatus.dwBEC );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
		
					wsprintf( szString, TEXT("Signal Level=%d"), SignalStatus.dwSignalLevel );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
		
				} else {
				    wsprintf( szString, TEXT("Get RF Fail") );
					SendDlgItemMessage( hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);       	
				}
				return TRUE;
			}
			
			break;
			
	}
    return FALSE;
}

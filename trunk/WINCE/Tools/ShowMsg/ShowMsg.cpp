// ShowMsg.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ShowMsg.h"
#include <commctrl.h>
#include "TestClass.h"
#include "spComXClass.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle
HWND				hMainWnd;
SShowMsgClass		*pMsg;
SGPSComClass		*pCom;

static HANDLE hMonitorThread = NULL;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);

static DWORD MonitorThreadProc(LPVOID lpvoid);


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

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SHOWMSG);

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
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHOWMSG));
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
	LoadString(hInstance, IDC_SHOWMSG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}

	hMainWnd = hWnd;
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	if (hwndCB)
		CommandBar_Show(hwndCB, TRUE);

	RECT rtWin;

	GetClientRect( hMainWnd, &rtWin );
	rtWin.top = rtWin.top + 24;
	///rtWin.left = 100;
	rtWin.top = (rtWin.top+rtWin.bottom)/2;
	///rtWin.right = 200;
	///rtWin.bottom = (rtWin.top+rtWin.bottom)/2;

	pMsg = new SShowMsgClass( hMainWnd, rtWin );
	pMsg->SetDebugLevel( 0xFFFFFFFF );


	GetClientRect( hMainWnd, &rtWin );
	rtWin.top = rtWin.top + 24;
	///rtWin.left = 100;
	///rtWin.top = (rtWin.top+rtWin.bottom)/2;
	///rtWin.right = 200;
	rtWin.bottom = (rtWin.top+rtWin.bottom)/2;

	///pCom = new SComXClass( 2 );
	pCom = new SGPSComClass();
	///pCom->SetDebugLevel( dINFO|dWARN|dFAIL );
	pCom->SetDebugLevel( 0xFFFFFFFF );
	pCom->SetFrameWindowHwnd( hMainWnd );
	pCom->SetShowAreaRect( rtWin );
	pCom->SetComNumber( 2 );	
	///pCom->SetBaudRate( 4800 );
	pCom->SetBaudRate( 9600 );
	///pCom->Enable();
	
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


static DWORD MonitorThreadProc(LPVOID lpvoid)
{
	DWORD dwRet = 0;
	DWORD dwCount = 0;
	RECT rtTemp;
	
	Sleep(1000);
	
	
///	GetClientRect( hMainWnd, &rtTemp );
	///rtWin.left = 100;
	///rtWin.top = 100;
	///rtWin.right = 200;
	///rtWin.bottom = 200;
	
	while(1)
	{
		dwCount++;
///		if( (dwCount%3) == 0 )
///		{	
///			rtTemp.top = 100;
///			rtTemp.bottom = 200;
///		}
///		else	
///		if( (dwCount%3) == 1 )
///		{	
///			rtTemp.top = 250;
///			rtTemp.bottom = 350;
///		}
///		else	
///		{	
///			rtTemp.top = 400;
///			rtTemp.bottom = 500;
///		}

		///pMsg->SetShowAreaRect( rtTemp );
		Sleep(100);
		pMsg->MsgOut( (dwCount%2)?TRUE:FALSE, _T("ssssssssssssssssssssssssssssss  %d  %s "), dwCount, (dwCount%2)?L"Good":L"Bad" );

		pCom->Disable();
		pCom->Enable();
		Sleep(2000);
		pMsg->MsgOut( _T("GPS Firmware boot %s "), pCom->IsFWBootOK()?L"Ok":L"Fail" );
	}

	return dwRet;
}

// spExeBackLightAdjust.cpp : Defines the entry point for the application.
//



#include "stdafx.h"
#include "spExeBackLightAdjust.h"
#include <windows.h>
#include <commctrl.h>

#define TOUCHTWO_SUPPORT
#define TOUCHTWOLIB_SUPPORT


#ifdef TOUCHTWO_SUPPORT
	#pragma comment(lib,"Lib\\spLibSysHALIoctl.lib")
	#include "spLibSysHALIoctl.h"

#ifdef TOUCHTWOLIB_SUPPORT	
	#pragma comment(lib,"Lib\\spLibSysTouchTwo.lib")
	#include "spLibSysTouchTwo.h"
#endif
#endif	///TOUCHTWO_SUPPORT



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			g_hInst;			// current instance
#ifdef SHELL_AYGSHELL
HWND				g_hWndMenuBar;		// menu bar handle
#else // SHELL_AYGSHELL
HWND				g_hWndCommandBar;	// command bar handle
#endif // SHELL_AYGSHELL

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#ifdef TOUCHTWO_SUPPORT

#ifdef TOUCHTWOLIB_SUPPORT
DWORD MonitorThreadProc( LPVOID lpvoid );
HWND g_hWnd;
HANDLE g_hMonitorThread;
#endif
BOOL spLibTouchTwoCtrl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif	///TOUCHTWO_SUPPORT


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPEXEBACKLIGHTADJUST));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPEXEBACKLIGHTADJUST));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    TCHAR szTitle[MAX_LOADSTRING];		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name

    g_hInst = hInstance; // Store instance handle in our global variable

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the device specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_SPEXEBACKLIGHTADJUST, szWindowClass, MAX_LOADSTRING);

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    //If it is already running, then focus on the window, and exit
    hWnd = FindWindow(szWindowClass, szTitle);	
    if (hWnd) 
    {
        // set focus to foremost child window
        // The "| 0x00000001" is used to bring any owned windows to the foreground and
        // activate them.
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    } 
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

    if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return FALSE;
    }

    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

#ifdef WIN32_PLATFORM_PSPC
    // When the main window is created using CW_USEDEFAULT the height of the menubar (if one
    // is created is not taken into account). So we resize the window after creating it
    // if a menubar is present
    if (g_hWndMenuBar)
    {
        RECT rc;
        RECT rcMenuBar;

        GetWindowRect(hWnd, &rc);
        GetWindowRect(g_hWndMenuBar, &rcMenuBar);
        rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
		
        MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
    }
#endif // WIN32_PLATFORM_PSPC

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

#ifndef SHELL_AYGSHELL
    if (g_hWndCommandBar)
    {
        CommandBar_Show(g_hWndCommandBar, TRUE);
    }
#endif // !SHELL_AYGSHELL

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
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
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

#if defined(SHELL_AYGSHELL) && !defined(WIN32_PLATFORM_WFSP)
    static SHACTIVATEINFO s_sai;
#endif // SHELL_AYGSHELL && !WIN32_PLATFORM_WFSP
	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_HELP_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
                    break;
#ifndef SHELL_AYGSHELL
                case IDM_FILE_EXIT:
                    DestroyWindow(hWnd);
                    break;
#endif // !SHELL_AYGSHELL
#ifdef WIN32_PLATFORM_PSPC
                case IDM_OK:
                    SendMessage (hWnd, WM_CLOSE, 0, 0);				
                    break;
#endif // WIN32_PLATFORM_PSPC
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_CREATE:
#ifdef TOUCHTWO_SUPPORT
#ifdef TOUCHTWOLIB_SUPPORT
			g_hWnd = hWnd;
			///create thread
			g_hMonitorThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)MonitorThreadProc, NULL, 0, NULL );
#endif
#endif
			
#ifndef SHELL_AYGSHELL
            g_hWndCommandBar = CommandBar_Create(g_hInst, hWnd, 1);
            CommandBar_InsertMenubar(g_hWndCommandBar, g_hInst, IDR_MENU, 0);
            CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);
#endif // !SHELL_AYGSHELL
#ifdef SHELL_AYGSHELL
            SHMENUBARINFO mbi;

            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hWnd;
            mbi.nToolBarId = IDR_MENU;
            mbi.hInstRes   = g_hInst;

            if (!SHCreateMenuBar(&mbi)) 
            {
                g_hWndMenuBar = NULL;
            }
            else
            {
                g_hWndMenuBar = mbi.hwndMB;
            }

            // Initialize the shell activate info structure
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);
#endif // SHELL_AYGSHELL
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            
            // TODO: Add any drawing code here...
            
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
#ifndef SHELL_AYGSHELL
            CommandBar_Destroy(g_hWndCommandBar);
#endif // !SHELL_AYGSHELL
#ifdef SHELL_AYGSHELL
            CommandBar_Destroy(g_hWndMenuBar);
#endif // SHELL_AYGSHELL
            PostQuitMessage(0);
            break;

#if defined(SHELL_AYGSHELL) && !defined(WIN32_PLATFORM_WFSP)
        case WM_ACTIVATE:
            // Notify shell of our activate message
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
            break;
#endif // SHELL_AYGSHELL && !WIN32_PLATFORM_WFSP

#ifdef TOUCHTWO_SUPPORT
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
	#ifdef TOUCHTWOLIB_SUPPORT
			if( spLibSysTouchTwo_Process( hWnd, message, wParam, lParam ) )
				break;
			else
				return DefWindowProc(hWnd, message, wParam, lParam);
	#else			
			if( spLibTouchTwoCtrl( hWnd, message, wParam, lParam ) )
				break;
			else
				return DefWindowProc(hWnd, message, wParam, lParam);
	#endif
#endif ///TOUCHTWO_SUPPORT
				
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
#ifndef SHELL_AYGSHELL
            RECT rectChild, rectParent;
            int DlgWidth, DlgHeight;	// dialog width and height in pixel units
            int NewPosX, NewPosY;

            // trying to center the About dialog
            if (GetWindowRect(hDlg, &rectChild)) 
            {
                GetClientRect(GetParent(hDlg), &rectParent);
                DlgWidth	= rectChild.right - rectChild.left;
                DlgHeight	= rectChild.bottom - rectChild.top ;
                NewPosX		= (rectParent.right - rectParent.left - DlgWidth) / 2;
                NewPosY		= (rectParent.bottom - rectParent.top - DlgHeight) / 2;
				
                // if the About box is larger than the physical screen 
                if (NewPosX < 0) NewPosX = 0;
                if (NewPosY < 0) NewPosY = 0;
                SetWindowPos(hDlg, 0, NewPosX, NewPosY,
                    0, 0, SWP_NOZORDER | SWP_NOSIZE);
            }
#endif // !SHELL_AYGSHELL
#ifdef SHELL_AYGSHELL
            {
                // Create a Done button and size it.  
                SHINITDLGINFO shidi;
                shidi.dwMask = SHIDIM_FLAGS;
                shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
                shidi.hDlg = hDlg;
                SHInitDialog(&shidi);
            }
#endif // SHELL_AYGSHELL

            return (INT_PTR)TRUE;

        case WM_COMMAND:
#ifndef SHELL_AYGSHELL
            if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
#endif // !SHELL_AYGSHELL
#ifdef SHELL_AYGSHELL
            if (LOWORD(wParam) == IDOK)
#endif
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, message);
            return (INT_PTR)TRUE;

    }
    return (INT_PTR)FALSE;
}


#ifdef TOUCHTWO_SUPPORT

void DrawTEXTtoPanel( HWND hWnd, LPCTSTR szError,... )
{
	static HBRUSH myhBrushBlack = NULL;
	static HBRUSH myhBrushWhite = NULL;
	static HDC myhdc = NULL;
	///RECT rtWin, rtNow;
	RECT rtWin;
	
	TCHAR szDbgBuff[256];
	va_list vl;
	
	if( NULL == myhBrushBlack )
		myhBrushBlack = CreateSolidBrush( 0x00000000 );

	if( NULL == myhBrushWhite )
		myhBrushBlack = CreateSolidBrush( 0x00FFFFFF );

		
	if( NULL == myhdc )
		myhdc = GetDC( hWnd );
		
	GetClientRect( hWnd, &rtWin );

	//clean it !!
	SelectObject( myhdc, (HBRUSH)myhBrushWhite );
	Rectangle( myhdc, rtWin.left, rtWin.top, rtWin.right, rtWin.bottom );
	
	SelectObject( myhdc, (HBRUSH)myhBrushBlack );

	va_start(vl, szError);
	
	StringCchVPrintf(szDbgBuff, (256), szError, vl);
	DrawText( myhdc, szDbgBuff, 30, &rtWin, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
	
	va_end(vl);
		
}


void DrawRECTtoPanel( HWND hWnd, RECT *rtWindow, DWORD dwOperation )
{
	static HBRUSH myhBrushBlack = NULL;
	static HBRUSH myhBrushWhite = NULL;
	static HDC myhdc = NULL;
	RECT rtWin, rtOrg;
	static RECT rtNow;
	
	SetRect( &rtOrg, 50, 250, 150, 400 );
	
	if( NULL == myhBrushBlack )
		myhBrushBlack = CreateSolidBrush( 0x00000000 );

	if( NULL == myhBrushWhite )
		myhBrushWhite = CreateSolidBrush( 0x00FFFFFF );

		
	if( NULL == myhdc )
		myhdc = GetDC( hWnd );
		
	GetClientRect( hWnd, &rtWin );

	//clean it !!
	SelectObject( myhdc, (HBRUSH)myhBrushWhite );
	Rectangle( myhdc, rtWin.left, rtWin.top, rtWin.right, rtWin.bottom );
	
	///
	SelectObject( myhdc, (HBRUSH)myhBrushBlack );
	if( 0 == dwOperation )
	{
		rtNow.left = rtOrg.left;
		rtNow.top = rtOrg.top;
		rtNow.right = rtOrg.right;
		rtNow.bottom = rtOrg.bottom;
	}
	else
	if( 1 == dwOperation )
	{	///bigger
		rtNow.top = rtNow.top - 5;
	}
	else
	if( 2 == dwOperation )
	{	///smaller
		rtNow.top = rtNow.top + 5;
	}
	///Rectangle( myhdc, rtNow.left, rtNow.top, rtNow.right, rtNow.bottom );
	FillRect( myhdc, &rtNow, myhBrushBlack );
	
	
}


DWORD CtrlBacklight( DWORD dwOperation )
{
	DWORD dwRet = 0;
	DWORD dwOutBuf = 0;
	DWORD dwInBuf = 0;
	
	if( dwOutBuf = spLibSysHALIoctl_BacklightGet() )
	{
		if( 1 ==dwOperation )
			dwOutBuf++;
		else
		if( 2 ==dwOperation )
			dwOutBuf--;
		
		dwInBuf = dwOutBuf;
		if( dwInBuf > 12 )
			dwInBuf = 11;

		spLibSysHALIoctl_BacklightSet( dwInBuf );
	}
	else
		NKDbgPrintfW( TEXT("Get backlight valuefails !\r\n ") );

	return dwRet;
}



int spLibTwoPointDistance( int x1, int y1, int x2, int y2 )
{
	///return sqrt( pow(abs(x1-x2),2) + pow(abs(y1-y2),2) );
	return (int)sqrt( pow(abs(x1-x2),2) + pow(abs(y1-y2),2) );
}


#define 	TWOFINGER_MIN_TIME_INTERVAL			13
#define 	TWOFINGER_MIN_DISTANCE				25

BOOL spLibTouchTwoCtrl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = FALSE;

	static int myxPrv, myyPrv;
	static int myxMovO, myyMovO;
	static int myxMovR, myyMovR;
	static int myDistOR, myDistPrv;
	static DWORD dwTickCount = 0;
	static BOOL bTwoFinger = FALSE;
	int myxPos, myyPos, myDist;
	RECT rtTemp;
	
	
	myxPos = LOWORD(lParam);
	myyPos = HIWORD(lParam);
	
	///DrawTEXTtoPanel( hWnd, TEXT("touch~") );
	
	if( 
		WM_LBUTTONDOWN == message ||
		WM_LBUTTONUP == message
	)
	{
		///clear flag
		bTwoFinger = FALSE;
		DrawTEXTtoPanel( hWnd, TEXT("up-down-%d,%d,%d,%d"), myxPos, myyPos, myxPrv, myyPrv );
		bRet = FALSE;
		
		DrawRECTtoPanel( hWnd, &rtTemp, 0 );
	}
	else	///WM_MOUSEMOVE == message
	{	
		///DrawTEXTtoPanel( hWnd, TEXT("mov-%d,%d,%d,%d,%d"), myxPos, myyPos, myxPrv, myyPrv, spLibTwoPointDistance(myxPos,myyPos,myxPrv,myyPrv) );
		
		if( !bTwoFinger )	///not two fingers yet
		{
			myDist = spLibTwoPointDistance(myxPos,myyPos,myxPrv,myyPrv);
			if( 
				((GetTickCount() - dwTickCount) < TWOFINGER_MIN_TIME_INTERVAL) 
				&&
				( myDist > TWOFINGER_MIN_DISTANCE )
///				( abs(myxPrv-myxPos) > 25 || abs(myyPrv-myyPos) > 25 )
///				( spLibTwoPointDistance(myxPos,myyPos,myxPrv,myyPrv) > TWOFINGER_MIN_DISTANCE )
			)
			{
				///we have two fingers now!!
				bTwoFinger = TRUE;
				myxMovR = myxPos;
				myyMovR = myyPos;
				myxMovO = myxPrv;
				myyMovO = myyPrv;
				myDistOR = spLibTwoPointDistance(myxMovR,myyMovR,myxMovO,myyMovO);
				myDistPrv = myDistOR;
				
				DrawTEXTtoPanel( hWnd, TEXT("!!Two fingers!!%d"), myDistOR );
				
				
				bRet = TRUE;
			}
			else
			{
				bTwoFinger = FALSE; 
				bRet = FALSE;
				DrawTEXTtoPanel( hWnd, TEXT("mov-%d,%d,%d,%d,%d"), myxPos, myyPos, myxPrv, myyPrv, myDist );
			}
		}
		else	///two figners on!
		{
			///check two fingers distance
			myDist = spLibTwoPointDistance(myxPos,myyPos,myxMovO,myyMovO);
			
			DrawTEXTtoPanel( hWnd, TEXT("Two move") );
			
			///second finger release
			if( myDist < (TWOFINGER_MIN_DISTANCE/2) )
			{
				bTwoFinger = FALSE;
				bRet = FALSE;
				DrawTEXTtoPanel( hWnd, TEXT("second fingers release") );
			}	
			else	///two fingers operation
			{
				///check zoom operation
				if( (myDist - myDistPrv) > 0 )
				{
					///bigger
					DrawTEXTtoPanel( hWnd, TEXT("Bigger") );
					DrawRECTtoPanel( hWnd, &rtTemp, 1 );
					CtrlBacklight( 1 );
				}
				else
				{
					///smaller
					DrawTEXTtoPanel( hWnd, TEXT("Smaller") );
					DrawRECTtoPanel( hWnd, &rtTemp, 2 );
					CtrlBacklight( 2 );
				}
				
				///check rotation 
				if( myxMovO > myxPos )
				{
					if( myyPos > myyMovO )
					{
						///anti-clockwise
						DrawTEXTtoPanel( hWnd, TEXT("anti-clockwise") );
					}
					else
					{
						///clockwise
						DrawTEXTtoPanel( hWnd, TEXT("clockwise") );
					}
				}
				else
				{
					if( myyPos > myyMovO )
					{
						///clockwise
						DrawTEXTtoPanel( hWnd, TEXT("clockwise") );
					}
					else
					{
						///anti-clockwise
						DrawTEXTtoPanel( hWnd, TEXT("anti-clockwise") );
					}
				}
				
				///update stored value
				myDistPrv = myDist;
				
				bRet = TRUE;
			}
		}

	}

	///update variable
	myxPrv = myxPos;
	myyPrv = myyPos;

	///update tickcount
	dwTickCount = GetTickCount();
	
	return bRet;
}

#ifdef TOUCHTWOLIB_SUPPORT
DWORD MonitorThreadProc( LPVOID lpvoid )
{
	DWORD dwRet = 0;
	DWORD dwWait = INFINITE;
	static HANDLE hEventSizeUp = NULL;
	static HANDLE hEventSizeDown = NULL;
	static HANDLE hEventRotateClockwise = NULL;
	static HANDLE hEventRotateAntiClockwise = NULL;
	static HANDLE hEventArray[4];
	
	RECT rtTemp;
	
	if( NULL == hEventSizeDown )
	{
		hEventSizeUp = CreateEvent( NULL, FALSE, FALSE, NAMED_TOUCHTWO_SIZE_UP );
		hEventSizeDown = CreateEvent( NULL, FALSE, FALSE, NAMED_TOUCHTWO_SIZE_DOWN );
		hEventRotateClockwise = CreateEvent( NULL, FALSE, FALSE, NAMED_TOUCHTWO_ROTATE_CLOCKWISE );
		hEventRotateAntiClockwise = CreateEvent( NULL, FALSE, FALSE, NAMED_TOUCHTWO_ROTATE_ANTICLOCKWISE );
		hEventArray[0] = hEventSizeUp;
		hEventArray[1] = hEventSizeDown;
		hEventArray[2] = hEventRotateClockwise;
		hEventArray[3] = hEventRotateAntiClockwise;
	}
	
	DrawRECTtoPanel( g_hWnd, &rtTemp, 0 );
	
	while(1)
	{
		dwRet = WaitForMultipleObjects( 4, hEventArray, FALSE, dwWait );

		switch( dwRet )
		{
			case WAIT_OBJECT_0 + 0:
				/// got size up event
				DrawTEXTtoPanel( g_hWnd, TEXT("Bigger") );
				DrawRECTtoPanel( g_hWnd, &rtTemp, 1 );
				CtrlBacklight( 1 );
				break;
			case WAIT_OBJECT_0 + 1:
				/// got size down event
				DrawTEXTtoPanel( g_hWnd, TEXT("Smaller") );
				DrawRECTtoPanel( g_hWnd, &rtTemp, 2 );
				CtrlBacklight( 2 );
				break;
			case WAIT_OBJECT_0 + 2:
				/// got rotate clockwise event
				DrawTEXTtoPanel( g_hWnd, TEXT("clockwise") );
				break;
			case WAIT_OBJECT_0 + 3:
				/// got rotate anti-clockwise event
				DrawTEXTtoPanel( g_hWnd, TEXT("anti-clockwise") );
				break;

			case WAIT_TIMEOUT:
				
				break;
			default:
				break;
				
		}	///switch
		
	}

	return dwRet;
}
#endif

#endif	///TOUCHTWO_SUPPORT
// spExeTouchTwo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "spExeTouchTwo.h"
#include <windows.h>
#include <commctrl.h>

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
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPEXETOUCHTWO));

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
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPEXETOUCHTWO));
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
    LoadString(hInstance, IDC_SPEXETOUCHTWO, szWindowClass, MAX_LOADSTRING);

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

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			{
				static int myxPrv, myyPrv;
				static int myxMovO, myyMovO;
				static int myxMovR, myyMovR;
				static DWORD dwTickCount = 0;
				static BOOL bTwoFinger = FALSE;
				int myxPos, myyPos;
				HBRUSH myhBrushRed, myhBrushGreen, myhBrushBlue, myhBrushWhite, myhBrushBlack;
				HDC myhdc;
				RECT rtWin, rtNow;

				myxPos = LOWORD(lParam);
				myyPos = HIWORD(lParam);
				myhBrushRed = CreateSolidBrush( 0x000000FF );
				myhBrushGreen = CreateSolidBrush( 0x0000FF00 );
				myhBrushBlue = CreateSolidBrush( 0x00FF0000 );
				myhBrushBlack = CreateSolidBrush( 0x00000000 );
				myhBrushWhite = CreateSolidBrush( 0x00FFFFFF );

				myhdc = GetDC( hWnd );
				GetClientRect( hWnd, &rtWin );


				SelectObject( myhdc, (HBRUSH)myhBrushWhite );
				Rectangle( myhdc, rtWin.left, rtWin.top, rtWin.right, rtWin.bottom );
				if( 
					WM_LBUTTONDOWN == message
				)
				{
					SelectObject( myhdc, (HBRUSH)myhBrushBlack );
					DrawText( myhdc, TEXT("DOWN"), 4, &rtWin, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
				}
				else
				if( 
					WM_LBUTTONUP == message
				)
				{
					SelectObject( myhdc, (HBRUSH)myhBrushBlack );
					DrawText( myhdc, TEXT("UP"), 2, &rtWin, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
				}
				else
				if( 
					WM_MOUSEMOVE == message
				)
				{
					SelectObject( myhdc, (HBRUSH)myhBrushBlack );
					DrawText( myhdc, TEXT("MOVE"), 4, &rtWin, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
				}



				if( 
					WM_LBUTTONDOWN == message ||
					WM_LBUTTONUP == message
///					WM_MOUSEMOVE == message
				)
				{
					///SelectObject( myhdc, (HBRUSH)myhBrushWhite );
					///Rectangle( myhdc, rtWin.left, rtWin.top, rtWin.right, rtWin.bottom );

					///DrawText( myhdc, TEXT("1234567890"), 10, &rtWin, DT_SINGLELINE | DT_VCENTER | DT_CENTER );
					SetRect( &rtNow, myxPos+30, myyPos+30, myxPos-30, myyPos-30 );
					if( WM_LBUTTONDOWN == message )
						SelectObject( myhdc, (HBRUSH)myhBrushRed );
					else
						SelectObject( myhdc, (HBRUSH)myhBrushGreen );

					Ellipse( myhdc, rtNow.left, rtNow.top, rtNow.right, rtNow.bottom );

					if( 0 == dwTickCount )
						dwTickCount = GetTickCount();

					bTwoFinger = FALSE;
					myxPrv = myxPos;
					myyPrv = myyPos;
				}

				if( 
///					WM_LBUTTONDOWN == message ||
///					WM_LBUTTONUP == message ||
					WM_MOUSEMOVE == message
				)
				{
					///SelectObject( myhdc, (HBRUSH)myhBrushWhite );
					///Rectangle( myhdc, rtWin.left, rtWin.top, rtWin.right, rtWin.bottom );

					///DrawText( myhdc, TEXT("1234567890"), 10, &rtWin, DT_SINGLELINE | DT_VCENTER | DT_CENTER );

					if( 
						!bTwoFinger &&
						((GetTickCount() - dwTickCount) < 25) &&
						( abs(myxPrv-myxPos) > 25 || abs(myyPrv-myyPos) > 25 )
					)
					{
						bTwoFinger = TRUE;
						myxMovR = myxPos;
						myyMovR = myyPos;
						myxMovO = myxPrv;
						myyMovO = myyPrv;

					}
					else
					{
						if( bTwoFinger )
						{
							static int myxPrvDif, myyPrvDif;
///							int myxDiff, myyDiff;
							int myRatio;
							int myR;
							POINT pointArrary[4];

							myxPrvDif = abs(myxPos-myxMovO) - abs(myxMovR-myxMovO);
							myyPrvDif = abs(myyPos-myyMovO) - abs(myyMovR-myyMovO);
							myRatio = (myxPrvDif + myyPrvDif)/4;

					#if 0 
							///SetRect( &rtNow, myxPos+(30+(myxPrvDif)), myyPos+(30+(myyPrvDif)), myxPos-(30+(myxPrvDif)), myyPos-(30+(myyPrvDif)) );
							///SetRect( &rtNow, myxMovO+(30+(myxPrvDif)), myyMovO+(30+(myyPrvDif)), myxMovO-(30+(myxPrvDif)), myyMovO-(30+(myyPrvDif)) );
							SetRect( &rtNow, myxMovO+(30+(myRatio)), myyMovO+(30+(myRatio)), myxMovO-(30+(myRatio)), myyMovO-(30+(myRatio)) );
							SelectObject( myhdc, (HBRUSH)myhBrushBlack );
							Rectangle( myhdc, rtNow.left, rtNow.top, rtNow.right, rtNow.bottom );
					#else
							SetRect( &rtNow, myxMovO+(30+(myRatio)), myyMovO+(30+(myRatio)), myxMovO-(30+(myRatio)), myyMovO-(30+(myRatio)) );
							SelectObject( myhdc, (HBRUSH)myhBrushBlack );

							myR = (abs(rtNow.right-myxMovO))^2 + (abs(rtNow.bottom-myyMovO))^2;

							pointArrary[0].x = rtNow.right;
							pointArrary[0].y = rtNow.bottom;
							pointArrary[1].x = rtNow.left;
							pointArrary[1].y = rtNow.bottom;
							pointArrary[2].x = rtNow.left;
							pointArrary[2].y = rtNow.top;
							pointArrary[3].x = rtNow.right;
							pointArrary[3].y = rtNow.top;

							if( (myyPos - myyMovR) > 0 )	///anti-clockwise
							{
								int xAdd, yAdd;
								yAdd = abs(myyPos - myyMovR)/2;
								pointArrary[0].y = rtNow.bottom + yAdd;
								xAdd = abs( sqrt(myR - (abs(pointArrary[0].y-myyMovO))^2) - myxMovO );
								pointArrary[0].x = rtNow.right - xAdd;

								pointArrary[1].x = rtNow.left - yAdd;
								pointArrary[1].y = rtNow.bottom - xAdd;

								pointArrary[2].x = rtNow.left + xAdd;
								pointArrary[2].y = rtNow.top - yAdd;
								pointArrary[3].x = rtNow.right + yAdd;
								pointArrary[3].y = rtNow.top + xAdd;

							}
							else
							{
								int xAdd, yAdd;
								yAdd = abs(myyPos - myyMovR)/2;
								pointArrary[0].y = rtNow.bottom - yAdd;
								xAdd = abs( sqrt(myR - (abs(pointArrary[0].y-myyMovO))^2) - myxMovO );
								pointArrary[0].x = rtNow.right + xAdd;

								pointArrary[1].x = rtNow.left + yAdd;
								pointArrary[1].y = rtNow.bottom + xAdd;

								pointArrary[2].x = rtNow.left - xAdd;
								pointArrary[2].y = rtNow.top + yAdd;
								pointArrary[3].x = rtNow.right - yAdd;
								pointArrary[3].y = rtNow.top - xAdd;
							}

							if( 
								pointArrary[0].x > 0 &&
								pointArrary[0].y > 0 &&
								pointArrary[1].x > 0 &&
								pointArrary[1].y > 0 &&
								pointArrary[2].x > 0 &&
								pointArrary[2].y > 0 &&
								pointArrary[3].x > 0 &&
								pointArrary[3].y > 0 
							)
							{
								Polygon( myhdc, pointArrary, 4 );
							}
					#endif
						
							///second finger release
							if( (abs(myxMovO-myxPos) < 10) && (abs(myyMovO-myyPos) < 10) )
								bTwoFinger = FALSE;
						}
						else
						{
							SetRect( &rtNow, myxPos+30, myyPos+30, myxPos-30, myyPos-30 );
							SelectObject( myhdc, (HBRUSH)myhBrushBlue );
							Rectangle( myhdc, rtNow.left, rtNow.top, rtNow.right, rtNow.bottom );
						}
					}
					myxPrv = myxPos;
					myyPrv = myyPos;
					dwTickCount = GetTickCount();
				}



			}
			break;


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

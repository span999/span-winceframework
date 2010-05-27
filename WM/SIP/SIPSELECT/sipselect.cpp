//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++

	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
	ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
	THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
	PARTICULAR PURPOSE.

	Module Name:  

	sipselect.cpp

--*/

//*****************************************************************************
//	sipselect.cpp - SipSelect Program for Windows CE
//*****************************************************************************
#include <windows.h>
#include <commctrl.h>
#include <windef.h>
#include <Winuser.h>
#include <windowsx.h>

#include "sipselect.h"
#include "sipdefs.h"

#include "resource.h"

#define	MAGIC_BULLET	0x06d00000
#define	MAGIC_MASK		0x0ff00000

///////////////////////////////////////////////////////////////////////////////
//	Prototype
///////////////////////////////////////////////////////////////////////////////
// Call WinMain
BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

// CallBack
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DmyWndProc(HWND, UINT, WPARAM, LPARAM);

// Message
void OnCreate(HWND, WPARAM, LPARAM);
void OnDestroy(HWND, WPARAM, LPARAM);

// SipMenu
void OnSipMenu(HWND);

BOOL SipShowHide(BOOL show);

///////////////////////////////////////////////////////////////////////////////
//	Global
///////////////////////////////////////////////////////////////////////////////
HINSTANCE ghInst	= NULL;					// Instance
HWND      ghWnd		= NULL;					// window handle
HWND      ghDmyWnd		= NULL;				// dummy window handle
NOTIFYICONDATA tnid;

static BOOL g_bInSipSelect = FALSE;
BOOL g_bSipIMShow = FALSE;
BOOL g_bSipTimer1 = FALSE;
BOOL g_bSipTimer2 = FALSE;

//=============================================================================
//	WinMain
//=============================================================================
int WINAPI WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPTSTR lpszCmdParm,
		int nCmdShow
	)
{
	MSG		msg;
	HWND	one;

	if ((one = FindWindow(SIPSELECT_CLSNAME, NULL))!= NULL){
		SetForegroundWindow(one);
		return(FALSE);
	}

	ghInst = hInstance;

	if (!hPrevInstance) {
		if (!InitApplication(hInstance))
			return FALSE;
	}
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//=============================================================================
//	WindowClass Regist
//=============================================================================
BOOL InitApplication(
		HINSTANCE hInstance
	)
{
	WNDCLASS wc;

	wc.style		 = 0;
	wc.lpfnWndProc	 = MainWndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = NULL;
	wc.hCursor		 = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = SIPSELECT_CLSNAME;

	return RegisterClass(&wc);
}

//=============================================================================
//	CreateWindow & Show
//=============================================================================
BOOL InitInstance(
		HINSTANCE hInstance,
		int nCmdShow
	)
{
	ghWnd = CreateWindowEx(
		WS_EX_NOACTIVATE,
		SIPSELECT_CLSNAME,
		SIPSELECT_APPNAME,
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!ghWnd)
		return FALSE;

//	ShowWindow(ghWnd, nCmdShow);
//	UpdateWindow(ghWnd);

	return TRUE;
}

//=============================================================================
//	DummyCreateWindow & Show
//=============================================================================
BOOL DummyWindowCreate(void)
{
#define DUMMY_CLSNAME TEXT("SipSelecter Dummy Window")
#define DUMMY_APPNAME TEXT("SipSelecter Dummy Window")

	WNDCLASS wc;

	wc.style		 = 0;
	wc.lpfnWndProc	 = DmyWndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = NULL;
	wc.hIcon		 = NULL;
	wc.hCursor		 = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = DUMMY_CLSNAME;
	RegisterClass(&wc);

	ghDmyWnd = CreateWindowEx(
		0,//WS_EX_NOACTIVATE,
		DUMMY_CLSNAME,
		DUMMY_APPNAME,
		WS_CHILD,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		GetForegroundWindow(),
		NULL,
		NULL,
		NULL
	);
	if (!ghDmyWnd)
		return FALSE;

	//Other Popupmenu Close
	SetCapture(ghDmyWnd);
	ReleaseCapture();

//	ShowWindow(ghDmyWnd, SW_SHOW);
//	UpdateWindow(ghDmyWnd);

	return TRUE;
}

//=============================================================================
//	DmyWndProc
//=============================================================================
LRESULT CALLBACK DmyWndProc(
		HWND hWnd,
		UINT uMessage,
		WPARAM wParam,
		LPARAM lParam
	)
{
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

//=============================================================================
//	MainWndProc
//=============================================================================
LRESULT CALLBACK MainWndProc(
		HWND hWnd,
		UINT uMessage,
		WPARAM wParam,
		LPARAM lParam
	)
{
	static BOOL	LBtnDwnFlg = FALSE;
	static BOOL	LBtnDblclkFlg = FALSE;

	switch (uMessage) {
		case WM_CREATE:
			OnCreate(hWnd, wParam, lParam);
			break;
		case WM_DESTROY:
			OnDestroy(hWnd, wParam, lParam);
			break;
		case WM_TIMER:
			if(wParam == 1){
				KillTimer(hWnd, 1);
				g_bSipTimer1 = FALSE;
				if(!LBtnDblclkFlg){
					OnSipMenu(hWnd);
				}
				LBtnDblclkFlg = FALSE;
			}
			else {
				KillTimer(hWnd, 2);
				g_bSipTimer2 = FALSE;
			}
			break;

		case IDM_SIP_TASKBAR:
			if(lParam == WM_LBUTTONDBLCLK){
				if(!g_bSipIMShow){
					SipShowHide(TRUE);
				} else {
					SipShowHide(FALSE);
				}
				if(g_bInSipSelect){
					OnSipMenu(hWnd);
				}
				LBtnDwnFlg = FALSE;
				if(g_bSipTimer1){
					LBtnDblclkFlg = TRUE;
				}
				if(g_bSipTimer2){
					KillTimer(hWnd, 2);
					g_bSipTimer2 = FALSE;
				}
			} else
			if(lParam == WM_LBUTTONDOWN){
				LBtnDwnFlg = TRUE;
			} else
			if(lParam == WM_LBUTTONUP){
				if(g_bSipTimer1){
					KillTimer(hWnd, 1);
					g_bSipTimer1 = FALSE;
					LBtnDblclkFlg = FALSE;
				} else if(g_bSipTimer2){
					KillTimer(hWnd, 2);
					g_bSipTimer2 = FALSE;
					if(!g_bSipIMShow){
						SipShowHide(TRUE);
					} else {
						SipShowHide(FALSE);
					}
					if(g_bInSipSelect){
						OnSipMenu(hWnd);
					}
					LBtnDwnFlg = FALSE;
				} else {
					if(!g_bInSipSelect){
						if(LBtnDwnFlg){
							for(;;){
								if(SetTimer(hWnd, 1, GetDoubleClickTime(), NULL)){
									break;
								}
							}
							g_bSipTimer1 = TRUE;
						} else {
							WCHAR TmpTmp[80];
							TmpTmp[0] = 0;

							GetClassNameW( GetForegroundWindow(), TmpTmp, 79 );
							if( !lstrcmpW(TmpTmp, L"HHTaskBar") || !lstrcmpW(TmpTmp, L"Dialog")){
								for(;;){
									if(SetTimer(hWnd, 2, GetDoubleClickTime(), NULL)){
										break;
									}
								}
								g_bSipTimer2 = TRUE;
							}
						}
					} else {
						OnSipMenu(hWnd);
					}
				}
				LBtnDwnFlg = FALSE;
			}
			break;

		case IDM_SIP_HIDE:
			g_bSipIMShow = FALSE;

			// TaskTray
			tnid.hIcon = (HICON)LoadImage(ghInst, MAKEINTRESOURCE(IDI_MAIN_ICON),
											IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
			Shell_NotifyIcon(NIM_MODIFY, &tnid);
			break;
		case IDM_SIP_SHOW:
			g_bSipIMShow = TRUE;

			// TaskTray
			tnid.hIcon = (HICON)LoadImage(ghInst, MAKEINTRESOURCE(IDI_MAIN2_ICON),
											IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
			Shell_NotifyIcon(NIM_MODIFY, &tnid);
			break;

		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	return 0;
}

//=============================================================================
//	WM_CREATE
//=============================================================================
void OnCreate(
		HWND hWnd,
		WPARAM wParam,
		LPARAM lParam
	)
{
	TCHAR	tiptxt[64];

	// Icon
	HICON hIcon = (HICON)LoadImage(ghInst, MAKEINTRESOURCE(IDI_MAIN_ICON),
		IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	SendMessage(hWnd, WM_SETICON, FALSE, (WPARAM)hIcon);

	// TaskTray
	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = hWnd;
	tnid.uID = 1;
	tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	memset(tiptxt, 0, sizeof(tiptxt));
	LoadString(ghInst, IDS_TIP, tiptxt, sizeof(tiptxt)/sizeof(TCHAR));
	wcsncpy(tnid.szTip, tiptxt, sizeof(tnid.szTip)/sizeof(TCHAR));
	tnid.uCallbackMessage = IDM_SIP_TASKBAR;
	tnid.hIcon = (HICON)LoadImage(ghInst, MAKEINTRESOURCE(IDI_MAIN_ICON),
									IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	Shell_NotifyIcon(NIM_ADD, &tnid);

}

//=============================================================================
//	WM_DESTROY
//=============================================================================
void OnDestroy(
		HWND hWnd,
		WPARAM wParam,
		LPARAM lParam
	)
{
	//Task Tray DELETE
	Shell_NotifyIcon(NIM_DELETE, &tnid);

	PostQuitMessage(0);
}

//=============================================================================
//	SipMenu
//=============================================================================
void OnSipMenu(
		HWND hWnd
	)
{
    RECT rc;
	BOOL ret = TRUE;

	if(0 != GetAsyncKeyState(VK_MENU)){
		if(!g_bSipIMShow){
			SipShowHide(TRUE);
	    } else {
			SipShowHide(FALSE);
		}
#ifdef DEBUG
	} else if(0 != GetAsyncKeyState(VK_SHIFT)){
		SendMessage(hWnd, WM_CLOSE, 0, 0L);
#endif //DEBUG
	} else {
	    if( !g_bInSipSelect ) {
			if(DummyWindowCreate()){
		        g_bInSipSelect = TRUE;
				::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
		        ret = SIP_IMListPopup(
		            ghDmyWnd,
		            TPM_CENTERALIGN | TPM_BOTTOMALIGN,
		            rc.right,
		            rc.bottom,
		            NULL );
				PostMessage(hWnd, WM_NULL, 0, 0);
			    g_bInSipSelect = FALSE;
				if(!g_bSipIMShow && ret){
					SipShowHide(TRUE);
				}
				DestroyWindow(ghDmyWnd);
			}
	    } else {
			SetCapture(hWnd);
			ReleaseCapture();
	    }
	}
}

//=============================================================================
//	SipShowHide()
//=============================================================================
BOOL SipShowHide(BOOL show)
{
	if(show){
		PostMessage(ghWnd, IDM_SIP_SHOW, NULL, NULL);
		return SipShowIM(SIPF_ON | MAGIC_BULLET);
	} else {
		PostMessage(ghWnd, IDM_SIP_HIDE, NULL, NULL);
		return SipShowIM(SIPF_OFF | MAGIC_BULLET);
	}
}


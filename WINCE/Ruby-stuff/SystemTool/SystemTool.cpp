// SystemTool.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SystemTool.h"
#include <commctrl.h>
#include "CPU_Dlg.h"
#include "TOOLHELP_Dlg.h"
#include "OSBench_Dlg.h"
#include "SysPower_Dlg.h"
#include "CUnit_Dlg.h"
#include "IPC_Dlg.h"
#include "WinOverlay_Dlg.h"

#define		MAX_LOADSTRING 100
#define		WM_MAINTRAYICON		(WM_USER + 4097)
#define		GDI_BENCH_EVENT		L"named GDI Bench thread event"

// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle
HWND				g_hWnd;			// 
static BOOL			g_bMainWinHide = FALSE;
static BOOL			g_bMinimiseChecked = FALSE;
static HINSTANCE 	g_hInstance = NULL;
static HANDLE		hGDIBenchThread = NULL;
static HANDLE		hGDIBenchEvent = NULL;
static BOOL 		g_bGDIRunTest = FALSE;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);
static void 		MTrayIconSet	(DWORD);
static DWORD		spGDIBenchTest	(HINSTANCE hInst, HWND hWnd );


void MTrayIconSet(DWORD dwIndex)
{
	static BOOL bTrayOn = FALSE;
	NOTIFYICONDATA cb;
	cb.cbSize = sizeof(cb);
	cb.hWnd = g_hWnd;
	///cb.uFlags = NIF_ICON |NIF_MESSAGE|NIF_TIP;
	cb.uFlags = NIF_ICON|NIF_MESSAGE;
	cb.uCallbackMessage = WM_MAINTRAYICON;

		
	///add new
	cb.hIcon = LoadIcon( g_hInstance, MAKEINTRESOURCE(IDI_ICON_SYSTEMTRAY) );
	cb.uID = IDI_ICON_SYSTEMTRAY;


	if( !bTrayOn ) {
		Shell_NotifyIcon(NIM_ADD,&cb);
		bTrayOn = TRUE;
	}else{
		///Shell_NotifyIcon(NIM_MODIFY,&cb);
		///Shell_NotifyIcon(NIM_ADD,&cb);
		Shell_NotifyIcon(NIM_DELETE,&cb);
		bTrayOn = FALSE;		
	}

	
///	if( dwIndex > 0xFF )
///		Shell_NotifyIcon(NIM_DELETE,&cb);
		///NIM_ADD, NIM_DELETE, NIM_MODIFY
		///m_stcStatus.SetWindowText(msg);	
}


static DWORD spGetRandomColorRef( DWORD dwSeed )
{
	DWORD dwRet = 0;
	BYTE bBlue;
	BYTE bGreen;
	BYTE bRed;
	
	srand( dwSeed );
	
	bRed = (rand()& 0xFF);
	bGreen = (rand()& 0xFF);
	bBlue = (rand()& 0xFF);
	
	dwRet = dwRet | (bRed) | (bGreen<<8) | (bBlue<<16);
	
	return dwRet;	
}


static BOOL spGetRandomPoint( POINT *ppoNew, RECT *pRectBound, DWORD dwSeed )
{
	BOOL bRet = TRUE;
	LONG lTemp;
	BOOL bNotGetPoint = TRUE;
	
	srand( dwSeed );
	
	///get x
	do{
		lTemp = (rand()&0x01FF);

		if( pRectBound->left > pRectBound->right )
		{	
			if( lTemp < pRectBound->left && lTemp > pRectBound->right )
				bNotGetPoint = FALSE;
		}
		else
		{
			if( lTemp > pRectBound->left && lTemp < pRectBound->right )
				bNotGetPoint = FALSE;
		}			
	}while( bNotGetPoint );

	ppoNew->x = lTemp;
	bNotGetPoint = TRUE;
	
	///get y
	do{
		lTemp = (rand()&0x01FF);

		if( pRectBound->top > pRectBound->bottom )
		{	
			if( lTemp < pRectBound->top && lTemp > pRectBound->bottom )
				bNotGetPoint = FALSE;
		}
		else
		{
			if( lTemp > pRectBound->top && lTemp < pRectBound->bottom )
				bNotGetPoint = FALSE;
		}			
	}while( bNotGetPoint );

	ppoNew->y = lTemp;
	bNotGetPoint = TRUE;
	
	return bRet;
}


static BOOL spGetRandomPolyPoint( POINT *pppoNew, int iPolyNum, RECT *pRectBound, DWORD dwSeed)
{
	int iLoop = 0;
	POINT poTemp;
	BOOL bRet = TRUE;
	
	for( iLoop = 0; iLoop < iPolyNum;  iLoop++ )
	{
		spGetRandomPoint( &poTemp, pRectBound, dwSeed );
		pppoNew[iLoop].x = poTemp.x;
		pppoNew[iLoop].y = poTemp.y;
	}
	
	///return pppoNew;
	return bRet;
}

static BOOL spGetRandomRect( RECT *pRectNew, RECT *pRectBound, DWORD dwSeed )
{
	BOOL bRet = TRUE;
	LONG lTemp;
	
	srand( dwSeed );
	
	///NKDbgPrintfW(L"SPD@spGetRandomRect left=%d top=%d right=%d bottom =%d \r\n", pRectBound->left, pRectBound->top, pRectBound->right, pRectBound->bottom );
	
	if( pRectBound->left > pRectBound->right )
	{	
		///for left 
		do{
			lTemp = (rand()&0x01FF);
		}while( lTemp > pRectBound->left || lTemp < pRectBound->right );
	}
	else
	{
		///for left 
		do{
			lTemp = (rand()&0x01FF);
		}while( lTemp < pRectBound->left || lTemp > pRectBound->right );
	}	
	pRectNew->left = lTemp;

	if( pRectBound->top > pRectBound->bottom )
	{		
		///for top
		do{
			lTemp = (rand()&0x01FF);
		}while( lTemp > pRectBound->top || lTemp < pRectBound->bottom );
	}
	else
	{
		///for top
		do{
			lTemp = (rand()&0x01FF);
		}while( lTemp < pRectBound->top || lTemp > pRectBound->bottom );		
	}
	pRectNew->top = lTemp;

	if( pRectBound->left > pRectBound->right )
	{		
		///for right 
		do{
			lTemp = (rand()&0x01FF);
		}while( lTemp > pRectNew->left || lTemp < pRectBound->right );
	}
	else
	{
		///for right 
		do{
			lTemp = (rand()&0x01FF);
		}while( lTemp < pRectNew->left || lTemp > pRectBound->right );
	}	
	pRectNew->right = lTemp;

	if( pRectBound->top > pRectBound->bottom )
	{		    	
		///for bottom
		do{
			lTemp = (rand()&0x01FF);
		}while( lTemp > pRectNew->top || lTemp < pRectBound->bottom );
	}
	else
	{
		///for bottom
		do{
			lTemp = (rand()&0x01FF);
		}while( lTemp < pRectNew->top || lTemp > pRectBound->bottom );
	}		
	pRectNew->bottom = lTemp;
	
	///NKDbgPrintfW(L"SPD@spGetRandomRect new left=%d top=%d right=%d bottom =%d \r\n", pRectNew->left, pRectNew->top, pRectNew->right, pRectNew->bottom );
	
	return bRet;
}


DWORD WINAPI spGDIBenchTestThread( HWND hWnd1 )
{
	DWORD dwRet;
	BOOL bTestRunning = FALSE;
	///BOOL bRunTest = FALSE;
	
	if( hGDIBenchEvent == NULL )
		hGDIBenchEvent = CreateEvent(NULL, FALSE, FALSE, GDI_BENCH_EVENT);
	
	while(1)
	{
		
		dwRet = WaitForSingleObject( hGDIBenchEvent, 3000 );
		
		if( dwRet == WAIT_OBJECT_0 )
			;	///g_bGDIRunTest = (g_bGDIRunTest==TRUE)?FALSE:TRUE;
		else 
		if( dwRet == WAIT_TIMEOUT )
			;
		else
			;	

		if( (FALSE == bTestRunning) && (TRUE == g_bGDIRunTest) )
		{	
			bTestRunning = TRUE;
			spGDIBenchTest( g_hInstance, hWnd1 );
			bTestRunning = FALSE;
		}	
	}	
	
	return dwRet;
}


typedef struct {
    DWORD dwAvgMs;
    DWORD dwAvgUs;
    DWORD dwTotalMs;
} GDIBenchTestResult;

static DWORD		spGDIBenchTest	(HINSTANCE hInst, HWND hWnd1 )
{
#define GDITESTLOOP	10000
#define GDIPOLYNUM	3

	DWORD dwRet = 0;
	HWND hWnd = hWnd1;
	HDC hdc;
	RECT rtWindow;
	RECT rtNew;
	POINT pointArrary[GDIPOLYNUM];
	POINT poTemp, poTemp1;
	///PAINTSTRUCT ps;
	TCHAR szHello[MAX_LOADSTRING];
	DWORD dwBeginTick = 0;
	DWORD dwEndTick = 0;
	DWORD dwTickSum = 0;
	DWORD dwLoop = 0;
	HBRUSH myhBrushArrary[256];
	GDIBenchTestResult myTestResult[5];

	///create brush
	for( dwLoop = 0; dwLoop < 256; dwLoop++ )
	{
		myhBrushArrary[dwLoop] = CreateSolidBrush( spGetRandomColorRef( GetTickCount() ) );
	}
	
	///hdc = BeginPaint(hWnd, &ps);
	hdc = GetDC (hWnd);

	GetClientRect(hWnd, &rtWindow);
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	
///	DrawText(hdc, szHello, _tcslen(szHello), &rtWindow, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	
///	Sleep(2000);
/*-----------------------------------------------------------------------------------------*/
	dwTickSum = 0;
	for( dwLoop = GDITESTLOOP; dwLoop > 0; dwLoop-- )
	{
		spGetRandomRect( &rtNew, &rtWindow, (GetTickCount()+dwLoop) );
		dwBeginTick = GetTickCount();
		DrawText(hdc, szHello, _tcslen(szHello), &rtNew, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dwEndTick = GetTickCount();
		dwTickSum = dwTickSum + (dwEndTick - dwBeginTick);
	}
	
	///MessageBox(NULL, NULL, TEXT("ERROR 00001"), MB_OK);
	myTestResult[0].dwAvgMs = dwTickSum/GDITESTLOOP;
	myTestResult[0].dwAvgUs = dwTickSum*1000/GDITESTLOOP;
	myTestResult[0].dwTotalMs = dwTickSum;
	///NKDbgPrintfW(L"SPD@spGDIBenchTest DrawText = %dms[%d] %dus.\r\n", dwTickSum/GDITESTLOOP, dwTickSum, dwTickSum*1000/GDITESTLOOP );
	NKDbgPrintfW(L"SPD@spGDIBenchTest DrawText = %dms[%d] %dus.\r\n", myTestResult[0].dwAvgMs, myTestResult[0].dwTotalMs, myTestResult[0].dwAvgUs );
/*-----------------------------------------------------------------------------------------*/
	dwTickSum = 0;
	for( dwLoop = GDITESTLOOP; dwLoop > 0; dwLoop-- )
	{
		///spGetRandomRect( &rtNew, &rtWindow, (GetTickCount()+dwLoop) );
		spGetRandomRect( &rtNew, &rtWindow, GetTickCount() );
		///SelectObject(hdc, (HBRUSH)GetStockObject(GRAY_BRUSH));
		SelectObject(hdc, (HBRUSH)myhBrushArrary[dwLoop%255]);
		dwBeginTick = GetTickCount();
		Ellipse(hdc, rtNew.left, rtNew.top, rtNew.right, rtNew.bottom );
		dwEndTick = GetTickCount();
		dwTickSum = dwTickSum + (dwEndTick - dwBeginTick);
	}
	
	///MessageBox(NULL, NULL, TEXT("ERROR 00001"), MB_OK);
	myTestResult[1].dwAvgMs = dwTickSum/GDITESTLOOP;
	myTestResult[1].dwAvgUs = dwTickSum*1000/GDITESTLOOP;
	myTestResult[1].dwTotalMs = dwTickSum;
	///NKDbgPrintfW(L"SPD@spGDIBenchTest Ellipse = %dms[%d]] %dus.\r\n", dwTickSum/GDITESTLOOP, dwTickSum, dwTickSum*1000/GDITESTLOOP );
	NKDbgPrintfW(L"SPD@spGDIBenchTest Ellipse = %dms[%d] %dus.\r\n", myTestResult[1].dwAvgMs, myTestResult[1].dwTotalMs, myTestResult[1].dwAvgUs );
/*-----------------------------------------------------------------------------------------*/
	dwTickSum = 0;
	for( dwLoop = GDITESTLOOP; dwLoop > 0; dwLoop-- )
	{
		///spGetRandomRect( &rtNew, &rtWindow, (GetTickCount()+dwLoop) );
		spGetRandomRect( &rtNew, &rtWindow, GetTickCount() );
		SelectObject(hdc, (HBRUSH)myhBrushArrary[dwLoop%255]);
		dwBeginTick = GetTickCount();
		Rectangle(hdc, rtNew.left, rtNew.top, rtNew.right, rtNew.bottom );
		dwEndTick = GetTickCount();
		dwTickSum = dwTickSum + (dwEndTick - dwBeginTick);
	}

	///MessageBox(NULL, NULL, TEXT("ERROR 00001"), MB_OK);
	myTestResult[2].dwAvgMs = dwTickSum/GDITESTLOOP;
	myTestResult[2].dwAvgUs = dwTickSum*1000/GDITESTLOOP;
	myTestResult[2].dwTotalMs = dwTickSum;	
	///NKDbgPrintfW(L"SPD@spGDIBenchTest Rectangle = %dms[%d]] %dus.\r\n", dwTickSum/GDITESTLOOP, dwTickSum, dwTickSum*1000/GDITESTLOOP );
	NKDbgPrintfW(L"SPD@spGDIBenchTest Rectangle = %dms[%d] %dus.\r\n", myTestResult[2].dwAvgMs, myTestResult[2].dwTotalMs, myTestResult[2].dwAvgUs );
/*-----------------------------------------------------------------------------------------*/
	dwTickSum = 0;
	for( dwLoop = GDITESTLOOP; dwLoop > 0; dwLoop-- )
	{
		///spGetRandomRect( &rtNew, &rtWindow, (GetTickCount()+dwLoop) );
		SelectObject(hdc, (HBRUSH)myhBrushArrary[dwLoop%255]);
#if 0		
		spGetRandomPolyPoint( pointArrary, GDIPOLYNUM, &rtWindow, GetTickCount() );
#else
		spGetRandomPoint( &poTemp, &rtWindow, (GetTickCount()) );
		pointArrary[0].x = poTemp.x;
		pointArrary[0].y = poTemp.y;
		spGetRandomPoint( &poTemp, &rtWindow, (GetTickCount()+100) );		
		pointArrary[1].x = poTemp.x;
		pointArrary[1].y = poTemp.y;
		spGetRandomPoint( &poTemp, &rtWindow, (GetTickCount()+999) );
		pointArrary[2].x = poTemp.x;
		pointArrary[2].y = poTemp.y;
#endif		
		dwBeginTick = GetTickCount();
		///Polygon(hdc, spGetRandomPolyPoint( pointArrary, GDIPOLYNUM, &rtWindow, (GetTickCount()+dwLoop) ), GDIPOLYNUM );										
		Polygon(hdc, pointArrary, GDIPOLYNUM );
		dwEndTick = GetTickCount();
		dwTickSum = dwTickSum + (dwEndTick - dwBeginTick);
	}

	///MessageBox(NULL, NULL, TEXT("ERROR 00001"), MB_OK);
	myTestResult[3].dwAvgMs = dwTickSum/GDITESTLOOP;
	myTestResult[3].dwAvgUs = dwTickSum*1000/GDITESTLOOP;
	myTestResult[3].dwTotalMs = dwTickSum;	
	///NKDbgPrintfW(L"SPD@spGDIBenchTest Polygon = %dms[%d] %dus.\r\n", dwTickSum/GDITESTLOOP, dwTickSum, dwTickSum*1000/GDITESTLOOP );
	NKDbgPrintfW(L"SPD@spGDIBenchTest Polygon = %dms[%d] %dus.\r\n", myTestResult[3].dwAvgMs, myTestResult[3].dwTotalMs, myTestResult[3].dwAvgUs );
/*-----------------------------------------------------------------------------------------*/
	dwTickSum = 0;
	for( dwLoop = GDITESTLOOP; dwLoop > 0; dwLoop-- )
	{
		///spGetRandomPoint( &poTemp, &rtWindow, (GetTickCount()+dwLoop) );
		///spGetRandomPoint( &poTemp1, &rtWindow, (GetTickCount()+dwLoop+999) );		
		spGetRandomPoint( &poTemp, &rtWindow, (dwLoop) );
		spGetRandomPoint( &poTemp1, &rtWindow, (dwLoop+9999) );		

		dwBeginTick = GetTickCount();
		MoveToEx(hdc, poTemp.x, poTemp.y, NULL );
		LineTo(hdc, poTemp1.x, poTemp1.y);
		dwEndTick = GetTickCount();
		dwTickSum = dwTickSum + (dwEndTick - dwBeginTick);
	}

	///MessageBox(NULL, NULL, TEXT("ERROR 00001"), MB_OK);
	myTestResult[4].dwAvgMs = dwTickSum/GDITESTLOOP;
	myTestResult[4].dwAvgUs = dwTickSum*1000/GDITESTLOOP;
	myTestResult[4].dwTotalMs = dwTickSum;	
	///NKDbgPrintfW(L"SPD@spGDIBenchTest Polygon = %dms[%d] %dus.\r\n", dwTickSum/GDITESTLOOP, dwTickSum, dwTickSum*1000/GDITESTLOOP );
	NKDbgPrintfW(L"SPD@spGDIBenchTest LineTo = %dms[%d] %dus.\r\n", myTestResult[4].dwAvgMs, myTestResult[4].dwTotalMs, myTestResult[4].dwAvgUs );
/*-----------------------------------------------------------------------------------------*/


	wsprintf( 
		szHello, 
		TEXT("DrawText = %dms %dus [%d].\r\nEllipse = %dms %dus [%d].\r\nRectangle = %dms %dus [%d].\r\nPolygon = %dms %dus [%d].\r\nLineTo = %dms %dus [%d].\r\n"), 
		myTestResult[0].dwAvgMs, myTestResult[0].dwAvgUs, myTestResult[0].dwTotalMs,
		myTestResult[1].dwAvgMs, myTestResult[1].dwAvgUs, myTestResult[1].dwTotalMs,
		myTestResult[2].dwAvgMs, myTestResult[2].dwAvgUs, myTestResult[2].dwTotalMs,
		myTestResult[3].dwAvgMs, myTestResult[3].dwAvgUs, myTestResult[3].dwTotalMs,
		myTestResult[4].dwAvgMs, myTestResult[4].dwAvgUs, myTestResult[4].dwTotalMs
		 );
	if( rtWindow.top > rtWindow.bottom )
		rtWindow.bottom = rtWindow.bottom + 25;
	else	
		rtWindow.top = rtWindow.top + 25;
	DrawText(hdc, szHello, _tcslen(szHello), &rtWindow, DT_CENTER );

	Sleep(3000);
	
	///EndPaint(hWnd, &ps);
	ReleaseDC (hWnd, hdc);

	///delet brush
	for( dwLoop = 0; dwLoop < 256; dwLoop++ )
	{
		DeleteObject( myhBrushArrary[dwLoop] );
	}

	
	return dwRet;
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

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SYSTEMTOOL);

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
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSTEMTOOL));
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
	LoadString(hInstance, IDC_SYSTEMTOOL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}

	g_hInstance = hInstance;
	
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
#if 0	///debug only
	NKDbgPrintfW(L"SPD@main WndProc 0x%08x!\r\n", message);
	switch (message)
	{
		case WM_ACTIVATE:
			NKDbgPrintfW(L"SPD@main WndProc WM_ACTIVATE!\r\n");
			break;
		case WM_SETFOCUS:
			NKDbgPrintfW(L"SPD@main WndProc WM_SETFOCUS!\r\n");
			break;
		case WM_KILLFOCUS:
			NKDbgPrintfW(L"SPD@main WndProc WM_KILLFOCUS!\r\n");
			break;
		case WM_PAINT:
			NKDbgPrintfW(L"SPD@main WndProc WM_PAINT!\r\n");
			break;
		case WM_WINDOWPOSCHANGED:
			NKDbgPrintfW(L"SPD@main WndProc WM_WINDOWPOSCHANGED!\r\n");
			break;			
		case WM_NOTIFY:
			NKDbgPrintfW(L"SPD@main WndProc WM_NOTIFY!\r\n");
			break;
		case WM_ERASEBKGND:
			NKDbgPrintfW(L"SPD@main WndProc WM_ERASEBKGND!\r\n");
			break;
		case WM_QUERYNEWPALETTE:
			NKDbgPrintfW(L"SPD@main WndProc WM_QUERYNEWPALETTE!\r\n");
			break;
		///case WM_NCACTIVATE:
		case 0x0086:
			NKDbgPrintfW(L"SPD@main WndProc WM_NCACTIVATE!\r\n");
			break;			
		default:
			break;	
	}
#endif
	
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
				case IDM_ITEMS_CPU_USEAGE:
				   CPUusageDialogBoxHook( hInst, hWnd );	
				   break;
				case IDM_ITEMS_TOOLHELP:
				   spToolhelpDialogBoxHook( hInst, hWnd );	
				   break;					   
				case IDM_OPTION_MINIMIZE:
				   if( hwndCB )
				   {	
				   		HMENU	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
				   		if( g_bMinimiseChecked )
				   		{
				   				g_bMinimiseChecked = FALSE;
								///unchecked the menu item
								CheckMenuItem( hMenuBar, IDM_OPTION_MINIMIZE, MF_UNCHECKED );				   		
				   		}
				   		else
				   		{
				   				g_bMinimiseChecked = TRUE;
								///checked the menu item
								CheckMenuItem( hMenuBar, IDM_OPTION_MINIMIZE, MF_CHECKED );
				   		}
				   }			
				   break;
				case IDM_OSBENCH_ALL:
				   OSBenchDialogBoxHook( hInst, hWnd );
				   break;					   
				case IDM_CUNIT_TEST:
				   spCUnitDialogBoxHook( hInst, hWnd );
				   break;					   
				case IDM_IPC_TEST:
				   spIPCDialogBoxHook( hInst, hWnd );
				   break;					   
				case IDM_SYSPOWER_TEST:
				   spSysPowerDialogBoxHook( hInst, hWnd );
				   break;					   
				case IDM_WINOVERLAY_TEST:
				   spWinOverlayDialogBoxHook( hInst, hWnd );
				   break;					   
				case IDM_GDI_TEST:
					///hGDIBenchThread
					if( NULL == hGDIBenchThread )
					{
						hGDIBenchThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)spGDIBenchTestThread, (LPVOID)hWnd, 0, NULL );
						if ( !hGDIBenchThread )
							;
						else
							CloseHandle(hGDIBenchThread);
					}				
					
					if( NULL == hGDIBenchEvent )
				   		hGDIBenchEvent = CreateEvent(NULL, FALSE, FALSE, GDI_BENCH_EVENT);
					
					g_bGDIRunTest = (g_bGDIRunTest==TRUE)?FALSE:TRUE;
						
					if( hwndCB )
					{	
						HMENU	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
						if( g_bGDIRunTest )
							CheckMenuItem( hMenuBar, IDM_GDI_TEST, MF_CHECKED );
					   	else
							CheckMenuItem( hMenuBar, IDM_GDI_TEST, MF_UNCHECKED );
				   	}
					
					if( NULL != hGDIBenchEvent )
				   		SetEvent( hGDIBenchEvent );

				   break;					   
				case IDM_FILE_EXIT:
				   if( g_bMinimiseChecked )
				   {
				   		g_bMainWinHide = TRUE;
			    		ShowWindow(hWnd, SW_HIDE);				   		
				   		MTrayIconSet(1);
				   }
				   else
				   {
				   		MTrayIconSet( (0xFF+1) );
				   		DestroyWindow(hWnd);
				   }
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_MAINTRAYICON:
			
			NKDbgPrintfW(L"SPD@main WM_MAINTRAYICON click !\r\n");
			if (TRUE == g_bMainWinHide)
			{
			    ShowWindow(hWnd, SW_SHOW);
			    SetForegroundWindow(hWnd);	// make us come to the front
			    g_bMainWinHide = FALSE;
			    MTrayIconSet( (0xFF+1) );
			} 
			else 
			{
			    g_bMainWinHide = TRUE;
			    ShowWindow(hWnd, SW_HIDE);
			    MTrayIconSet(1);
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
			g_hWnd = hDlg;
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

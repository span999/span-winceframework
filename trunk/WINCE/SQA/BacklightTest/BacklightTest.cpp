// BacklightTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BacklightTest.h"
#include "BklTestClass.h"
#include <commctrl.h>

#define MAX_LOADSTRING 100


typedef enum
{
    _NORMAL  = 0,		///	no cmd line mode specified
    _STRESS,			///	cmd line stress mode
}
enumCmdLineStatus;


// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle

//////////////////////////////////////////////////////////////////////
/// my Variables
HWND				hMainWnd;		// The main window handle
TCHAR 				szTempString[MAX_LOADSTRING];
enumCmdLineStatus	CmdLineMode;

SBklBasicTest *pMyBasicTest = NULL;
///#define DBGMSGLEVEL	( dINIT | dWARN | dINFO | dDBUG | dFAIL | dCLAS )		///debug message define for this windows
#define DBGMSGLEVEL	( 0 )
#define	DBGMSGAREAHEIGHT	DEFAULT_SHOWAREA_HEIGHT
SShowMsgClass *pMsg = NULL;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);


//////////////////////////////////////////////////////////////////////
/// extern function
EXTERN_C BOOL SetEventData(HANDLE hEvent, DWORD dwData); 
EXTERN_C DWORD GetEventData(HANDLE hEvent);

//////////////////////////////////////////////////////////////////////
/// my basic function
static BOOL InitMsg( HWND hWnd, DWORD dwDbgLevel, DWORD dwAreaHi, DWORD dwLineHi );
static BOOL spGetMenuItemInfo( UINT uiItemID, MENUITEMINFO *pMii );
static BOOL spIsMenuItemChecked( UINT uiItemID );
static BOOL spInvertMenuItemCheck( UINT uiItemID );
static BOOL spCheckMenuItem( UINT uiItemID, BOOL bCheck );
static BOOL spIsAnyBasicItemChecked();
static BOOL spCheckBasicMenu();
static BOOL spSelectRobustModeMenu( UINT uiID );
static BOOL spTestItemActiveSycnMenuCheck( UINT uiItemID );
static BOOL spMenuOptionStageCheck( UINT uiItemID );
static BOOL spMenuOptionLoopCheck( UINT uiItemID );
static BOOL spMenuOptionLoopIntervalCheck( UINT uiItemID );
static enumCmdLineStatus ParseCommandLine( LPTSTR lpCmdLine );
static BOOL RunCommandLine( enumCmdLineStatus enumMode );
static void SetTestSettingDefault();
static void SetTestSettingStress();



int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	CmdLineMode = ParseCommandLine( lpCmdLine );

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_BACKLIGHTTEST);

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
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BACKLIGHTTEST));
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
	LoadString(hInstance, IDC_BACKLIGHTTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}

	hMainWnd = hWnd;		///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///init Msg object
	InitMsg( hMainWnd, DBGMSGLEVEL, DBGMSGAREAHEIGHT, 0 );

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	if (hwndCB)
		CommandBar_Show(hwndCB, TRUE);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///init test object
	pMyBasicTest = new SBklBasicTest( 3, 100 );
	///init test item
	///pMyBasicTest->SetDebugLevel( ( dINIT | dWARN | dINFO | dDBUG | dFAIL | dCLAS ) );	///debug message define for test class
	pMyBasicTest->SetDebugLevel( 0 );
	pMyBasicTest->AddTestItem( IDM_BASIC_API );
	pMyBasicTest->AddTestItem( IDM_BASIC_BRIGHTNESS );
	pMyBasicTest->AddTestItem( IDM_BASIC_ONOFF );
	pMyBasicTest->SetFrameWindowHwnd( hMainWnd );


	{	///setup frame rect for test class
		RECT rt;
		GetClientRect( hMainWnd, &rt );
		if( DBGMSGLEVEL != 0 )
			rt.bottom = rt.bottom - DBGMSGAREAHEIGHT;		///22 for mainframe message
		rt.top = rt.top + 24;			///24 for CommandBar offset
		pMyBasicTest->SetFrameWindowRect( rt );
	}
	
	SetTestSettingDefault();

	RunCommandLine( CmdLineMode );

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
					pMyBasicTest->SaveLog();
				  	DestroyWindow(hWnd);
				   break;
/// File ////////////////////////////////////////////////////////////////////////////////				   	
				case IDM_FILE_SAVELOG:
					pMyBasicTest->SaveLog();
				   	break;
/// File ////////////////////////////////////////////////////////////////////////////////				   					   	
/// Basic test ////////////////////////////////////////////////////////////////////////////////
				case IDM_BASIC_START:
					///spShowMsgString( _T("IDM_BASIC_START\0") );
					if( spIsAnyBasicItemChecked() )
					{	
						spInvertMenuItemCheck( IDM_BASIC_START );
					
						if( spIsMenuItemChecked( IDM_BASIC_START ) )
							pMyBasicTest->Run();
						else
							pMyBasicTest->Stop();
					}	
					
				   	break;
				case IDM_BASIC_API:
					///spShowMsgString( _T("IDM_BASIC_API\0") );					
					if( !spIsMenuItemChecked( IDM_BASIC_START ) )
					{	
						spInvertMenuItemCheck( IDM_BASIC_API );
											
						spTestItemActiveSycnMenuCheck( IDM_BASIC_API );
					}	
					
				   	break;
				case IDM_BASIC_BRIGHTNESS:
					if( !spIsMenuItemChecked( IDM_BASIC_START ) )
					{	
						spInvertMenuItemCheck( IDM_BASIC_BRIGHTNESS );
					
						spTestItemActiveSycnMenuCheck( IDM_BASIC_BRIGHTNESS );
					}	
					
				   	break;
				case IDM_BASIC_ONOFF:
					if( !spIsMenuItemChecked( IDM_BASIC_START ) )
					{	
						spInvertMenuItemCheck( IDM_BASIC_ONOFF );
						
						spTestItemActiveSycnMenuCheck( IDM_BASIC_ONOFF );
					}					
				   	break;				   				   				   
/// Basic test ////////////////////////////////////////////////////////////////////////////////				   
/// Stress test ////////////////////////////////////////////////////////////////////////////////
				case IDM_STRESS_START:
					spInvertMenuItemCheck( IDM_STRESS_START );

					if( spIsMenuItemChecked( IDM_STRESS_START ) )
					{
						SetTestSettingStress();
						pMyBasicTest->Run();
					}	
					else
					{	
						pMyBasicTest->Stop();
						SetTestSettingDefault();
					}	
				   	break;
/// Stress test ////////////////////////////////////////////////////////////////////////////////				   
/// Robust test ////////////////////////////////////////////////////////////////////////////////
				case IDM_ROBUST_START:

				   	break;
/// Robust test ////////////////////////////////////////////////////////////////////////////////
/// Option ////////////////////////////////////////////////////////////////////////////////
				case IDM_OPTION_LOG:
					spInvertMenuItemCheck( IDM_OPTION_LOG );
					pMyBasicTest->EnableLog( spIsMenuItemChecked( IDM_OPTION_LOG ) );
					break;
				case IDM_OPTION_STAGE_EVT:
					spMenuOptionStageCheck( IDM_OPTION_STAGE_EVT );
				   	break;				   					   	
				case IDM_OPTION_STAGE_DVT:
					spMenuOptionStageCheck( IDM_OPTION_STAGE_DVT );
				   	break;				   					   	
				case IDM_OPTION_STAGE_PVT:
					spMenuOptionStageCheck( IDM_OPTION_STAGE_PVT );
				   	break;				   					   	
				case IDM_OPTION_STAGE_MP:
					spMenuOptionStageCheck( IDM_OPTION_STAGE_MP );
				   	break;
				   	
				case IDM_OPTION_LOOPS_FREE:
					spMenuOptionLoopCheck( IDM_OPTION_LOOPS_FREE );
				   	break;				   					   	
				case IDM_OPTION_LOOPS_1:
					spMenuOptionLoopCheck( IDM_OPTION_LOOPS_1 );
				   	break;				   					   	
				case IDM_OPTION_LOOPS_100:
					spMenuOptionLoopCheck( IDM_OPTION_LOOPS_100 );
				   	break;				   					   	
				case IDM_OPTION_LOOPS_10000:
					spMenuOptionLoopCheck( IDM_OPTION_LOOPS_10000 );
				   	break;				   					   					   	

				case IDM_OPTION_INTERVAL_FREE:
					spMenuOptionLoopIntervalCheck( IDM_OPTION_INTERVAL_FREE );
				   	break;				   					   	
				case IDM_OPTION_INTERVAL_10:
					spMenuOptionLoopIntervalCheck( IDM_OPTION_INTERVAL_10 );
				   	break;				   					   	
				case IDM_OPTION_INTERVAL_100:
					spMenuOptionLoopIntervalCheck( IDM_OPTION_INTERVAL_100 );
				   	break;				   					   					   	
				case IDM_OPTION_INTERVAL_1000:
					spMenuOptionLoopIntervalCheck( IDM_OPTION_INTERVAL_1000 );
				   	break;				   					   					   	
				case IDM_OPTION_INTERVAL_10000:
					spMenuOptionLoopIntervalCheck( IDM_OPTION_INTERVAL_10000 );
				   	break;				   					   					   	

/// Option ////////////////////////////////////////////////////////////////////////////////
				   
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_CREATE:
			hwndCB = CommandBar_Create(hInst, hWnd, 1);			
			CommandBar_InsertMenubar(hwndCB, hInst, IDM_MENU, 0);
			CommandBar_AddAdornments(hwndCB, 0, 0);
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
			pMyBasicTest->SaveLog();
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


//////////////////////////////////////////////////////////////////////////////////////////
/// My basic utilities function
///init message object
BOOL InitMsg( HWND hWnd, DWORD dwDbgLevel, DWORD dwAreaHi, DWORD dwLineHi )
{
	BOOL bRet = TRUE;
	RECT rtArea;

	if( pMsg == NULL )
	{	
		pMsg = new SShowMsgClass();
		if( pMsg == NULL )
			return FALSE;
	}	

	if( 0 ==  dwAreaHi )
		dwAreaHi = DEFAULT_SHOWAREA_HEIGHT;		///default
	
	if( 0 ==  dwLineHi )
		dwLineHi = DEFAULT_SHOWLINE_HEIGHT;		///default

	GetClientRect(hWnd, &rtArea);		
	if( rtArea.top > rtArea.bottom )
		rtArea.top = rtArea.bottom + dwAreaHi;
	else
		rtArea.top = rtArea.bottom - dwAreaHi;

	pMsg->SetFrameWindowHwnd( hWnd );
	pMsg->SetDebugLevel( dwDbgLevel );
	pMsg->SetShowAreaRect( rtArea );
	pMsg->SetLineHeight( dwLineHi );

	return bRet;
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


///check if any basic function has been checked
static BOOL spIsAnyBasicItemChecked()
{
	BOOL bRet = FALSE;
	
	if( spIsMenuItemChecked( IDM_BASIC_API ) )
		bRet = TRUE;
	else	
	if( spIsMenuItemChecked( IDM_BASIC_BRIGHTNESS ) )
		bRet = TRUE;
	else	
	if( spIsMenuItemChecked( IDM_BASIC_ONOFF ) )
		bRet = TRUE;
	else
		bRet = FALSE;
	
	return bRet;	
}


///check basic menu status and config it
static BOOL spCheckBasicMenu()
{
	BOOL bRet = FALSE;
	HMENU hMenuBar = NULL;
	
	if( NULL == hwndCB )	///error handle
	{
		pMsg->MsgOut( dFAIL, _T("!!!spCheckBasicMenu error hwndCB\0") );	
		return bRet;
	}	

	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
	
	if( hMenuBar == NULL )	///error handle
	{
		pMsg->MsgOut( dFAIL, _T("!!!spCheckBasicMenu error hMenuBar\0") );	
		return bRet;
	}
	
	///if no function checked then gray the start item
	if( spIsAnyBasicItemChecked() )
		EnableMenuItem( hMenuBar, IDM_BASIC_START, MF_BYCOMMAND|MF_ENABLED );
	else
		EnableMenuItem( hMenuBar, IDM_BASIC_START, MF_BYCOMMAND|MF_GRAYED );		
			
	///if it's already start gray all function item
	if( spIsMenuItemChecked( IDM_BASIC_START ) )
	{
		EnableMenuItem( hMenuBar, IDM_BASIC_API, MF_BYCOMMAND|MF_GRAYED );
		EnableMenuItem( hMenuBar, IDM_BASIC_BRIGHTNESS, MF_BYCOMMAND|MF_GRAYED );
		EnableMenuItem( hMenuBar, IDM_BASIC_ONOFF, MF_BYCOMMAND|MF_GRAYED );		
	}
	else
	{
		EnableMenuItem( hMenuBar, IDM_BASIC_API, MF_BYCOMMAND|MF_ENABLED );
		EnableMenuItem( hMenuBar, IDM_BASIC_BRIGHTNESS, MF_BYCOMMAND|MF_ENABLED );
		EnableMenuItem( hMenuBar, IDM_BASIC_ONOFF, MF_BYCOMMAND|MF_ENABLED );		
	}		
	
	return bRet;	
}


static BOOL spSelectRobustModeMenu( UINT uiID )
{
	BOOL bRet = TRUE;
	

	if( spIsMenuItemChecked( uiID ) )
	{
		///spInvertMenuItemCheck( uiID );
		spCheckMenuItem( uiID, FALSE );
	}
	else	
	{	
		///uncheck all
///		spCheckMenuItem( IDM_ROBUST_MODE_NORMAL, FALSE );
///		spCheckMenuItem( IDM_ROBUST_MODE_BOUNCE, FALSE );
///		spCheckMenuItem( IDM_ROBUST_MODE_UNUSUAL, FALSE );
		
		///check this one
		///spInvertMenuItemCheck( uiID );
		spCheckMenuItem( uiID, TRUE );
	}			

	return bRet;
}


///invert menu item checked status
static BOOL spTestItemActiveSycnMenuCheck( UINT uiItemID )
{
	BOOL bRet = FALSE;
	
	if( NULL != pMyBasicTest )
	{
		if( spIsMenuItemChecked( uiItemID ) )
			pMyBasicTest->ActiveTestItem( uiItemID );
		else
			pMyBasicTest->DeActiveTestItem( uiItemID );
		bRet = TRUE;	
	}
	else
	{	///error handle
		bRet = FALSE;
		pMsg->MsgOut( dFAIL, _T("!!!spTestItemActiveSycnMenuCheck error pMyBattBasicTest\0") );	
	}		

	///wsprintf( szTempString, _T("TestItem ID %d status = %d\0"), uiItemID, pMyBasicTest->IDGetTestItemStatus(uiItemID) );
	///spShowMsgString( szTempString );
	pMsg->MsgOut( dINFO, _T("TestItem ID %d status = %d\0"), uiItemID, pMyBasicTest->IDGetTestItemStatus(uiItemID) );

	return bRet;
}

static BOOL spCheckMenuItem( UINT uiItemID, BOOL bCheck )
{
	BOOL bRet = FALSE;
	HMENU hMenuBar = NULL;


	if( NULL == hwndCB )	///error handle
	{
		pMsg->MsgOut( dFAIL, _T("!!!spCheckMenuItem error hwndCB\0") );	
		return bRet;
	}	

	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
	
	if( hMenuBar == NULL )
	{	///error handle
		pMsg->MsgOut( dFAIL, _T("!!!spCheckMenuItem error hMenuBar\0") );
		return bRet;
	}
	
	if( bCheck )
		CheckMenuItem( hMenuBar, uiItemID, MF_BYCOMMAND|MF_CHECKED );
	else
		CheckMenuItem( hMenuBar, uiItemID, MF_BYCOMMAND|MF_UNCHECKED );
	
	return bRet;
}



static BOOL spMenuOptionStageCheck( UINT uiItemID )
{
	BOOL bRet = FALSE;
	HMENU hMenuBar = NULL;


	if( NULL == hwndCB )	///error handle
	{
		pMsg->MsgOut( dFAIL, _T("!!!spMenuOptionStageCheck error hwndCB\0") );	
		return bRet;
	}	

	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
	
	if( hMenuBar == NULL )
	{	///error handle
		pMsg->MsgOut( dFAIL, _T("!!!spMenuOptionStageCheck error hMenuBar\0") );
		return bRet;
	}
	
	///unchecked all option stage item
	CheckMenuItem( hMenuBar, IDM_OPTION_STAGE_EVT, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_STAGE_DVT, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_STAGE_PVT, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_STAGE_MP, MF_BYCOMMAND|MF_UNCHECKED );

	
	///checked this one
	CheckMenuItem( hMenuBar, uiItemID, MF_BYCOMMAND|MF_CHECKED );
	
	enumTestStage enStageTemp;
	
	switch(uiItemID)
	{
		case IDM_OPTION_STAGE_EVT:
			enStageTemp = _TestStageEVT;
			break;
		case IDM_OPTION_STAGE_DVT:
			enStageTemp = _TestStageDVT;
			break;
		case IDM_OPTION_STAGE_PVT:
			enStageTemp = _TestStagePVT;
			break;
		case IDM_OPTION_STAGE_MP:
			enStageTemp = _TestStageMP;
			break;						
		default:
			enStageTemp = _TestStageEVT;
			break;				
	}
		
	pMyBasicTest->SetTestStage( enStageTemp );
	
	bRet = TRUE;

	return bRet;
}


static BOOL spMenuOptionLoopCheck( UINT uiItemID )
{
	BOOL bRet = FALSE;
	HMENU hMenuBar = NULL;


	if( NULL == hwndCB )	///error handle
	{
		pMsg->MsgOut( dFAIL, _T("!!!spMenuOptionLoopCheck error hwndCB\0") );	
		return bRet;
	}	

	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
	
	if( hMenuBar == NULL )
	{	///error handle
		pMsg->MsgOut( dFAIL, _T("!!!spMenuOptionLoopCheck error hMenuBar\0") );
		return bRet;
	}
	
	///unchecked all option stage item
	CheckMenuItem( hMenuBar, IDM_OPTION_LOOPS_FREE, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_LOOPS_1, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_LOOPS_100, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_LOOPS_10000, MF_BYCOMMAND|MF_UNCHECKED );

	
	///checked this one
	CheckMenuItem( hMenuBar, uiItemID, MF_BYCOMMAND|MF_CHECKED );
	
	DWORD dwLoopTemp;
	
	switch(uiItemID)
	{
		case IDM_OPTION_LOOPS_FREE:
			dwLoopTemp = 0xffffffff;
			break;
		case IDM_OPTION_LOOPS_1:
			dwLoopTemp = 1;
			break;
		case IDM_OPTION_LOOPS_100:
			dwLoopTemp = 100;
			break;
		case IDM_OPTION_LOOPS_10000:
			dwLoopTemp = 10000;
			break;						
		default:
			dwLoopTemp = 100;
			break;				
	}
		
	pMyBasicTest->SetTestLoopNumber( dwLoopTemp );
	
	bRet = TRUE;

	return bRet;
}


static BOOL spMenuOptionLoopIntervalCheck( UINT uiItemID )
{
	BOOL bRet = FALSE;
	HMENU hMenuBar = NULL;


	if( NULL == hwndCB )	///error handle
	{
		pMsg->MsgOut( dFAIL, _T("!!!spMenuOptionLoopIntervalCheck error hwndCB\0") );	
		return bRet;
	}	

	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
	
	if( hMenuBar == NULL )
	{	///error handle
		pMsg->MsgOut( dFAIL, _T("!!!spMenuOptionLoopIntervalCheck error hMenuBar\0") );
		return bRet;
	}
	
	///unchecked all option stage item
	CheckMenuItem( hMenuBar, IDM_OPTION_INTERVAL_FREE, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_INTERVAL_10, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_INTERVAL_100, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_INTERVAL_1000, MF_BYCOMMAND|MF_UNCHECKED );
	CheckMenuItem( hMenuBar, IDM_OPTION_INTERVAL_10000, MF_BYCOMMAND|MF_UNCHECKED );

	
	///checked this one
	CheckMenuItem( hMenuBar, uiItemID, MF_BYCOMMAND|MF_CHECKED );
	
	DWORD dwIntervalTemp;
	
	switch(uiItemID)
	{
		case IDM_OPTION_INTERVAL_FREE:
			dwIntervalTemp = 1;
			break;
		case IDM_OPTION_INTERVAL_10:
			dwIntervalTemp = 10;
			break;
		case IDM_OPTION_INTERVAL_100:
			dwIntervalTemp = 100;
			break;
		case IDM_OPTION_INTERVAL_1000:
			dwIntervalTemp = 1000;
			break;						
		case IDM_OPTION_INTERVAL_10000:
			dwIntervalTemp = 10000;
			break;									
		default:
			dwIntervalTemp = 100;
			break;				
	}
		
	pMyBasicTest->SetTestLoopInterval( dwIntervalTemp );
	
	bRet = TRUE;

	return bRet;
}


static enumCmdLineStatus ParseCommandLine( LPTSTR lpCmdLine )
{
	enumCmdLineStatus enumRet = _NORMAL;
	

    if( !_tcsicmp( lpCmdLine, TEXT("-stress") ) ) 
    {
    	///pMsg->MsgOut( dINFO, _T("ParseCommandLine stress test !!!\0") );
   		NKDbgPrintfW( TEXT("ParseCommandLine stress test !!!\r\n") );
    	enumRet = _STRESS;
    }
    else
    	NKDbgPrintfW( TEXT("ParseCommandLine %s !!!\r\n"), lpCmdLine );

	return enumRet;
}


static BOOL RunCommandLine( enumCmdLineStatus enumMode )
{
	BOOL  bRet = FALSE;

	if( _NORMAL == enumMode )
	{	
		///do nothing ...
		bRet = TRUE;
	}
	else
	if( _STRESS == enumMode )
	{
		if( NULL != pMyBasicTest )
		{	
			pMsg->MsgOut( dINFO, _T("Run stress test from command line !!!\0") );	
			NKDbgPrintfW( TEXT("Run stress test from command line !!!\r\n") );
			///do strees test
			SetTestSettingStress();
			
			if( !spIsMenuItemChecked( IDM_STRESS_START ) )
				spInvertMenuItemCheck( IDM_STRESS_START );
			
			pMyBasicTest->Run();
		}
	}

	return bRet;
}

static void SetTestSettingDefault()
{
	if( NULL != pMyBasicTest )
	{	
		pMyBasicTest->SetTestLoopNumber( 100 );	///set default
		pMyBasicTest->SetTestStage( _TestStageEVT );	///set default
		pMyBasicTest->SetTestLoopInterval( 100 );	///set default	

		pMyBasicTest->EnableLog( FALSE );
		pMyBasicTest->DeActiveTestItem( IDM_BASIC_API );
		pMyBasicTest->DeActiveTestItem( IDM_BASIC_BRIGHTNESS );
		pMyBasicTest->DeActiveTestItem( IDM_BASIC_ONOFF );
	}	
}

static void SetTestSettingStress()
{
	if( NULL != pMyBasicTest )
	{	
		pMyBasicTest->SetTestLoopNumber( 0xffffffff );
		pMyBasicTest->SetTestStage( _TestStageEVT );
		pMyBasicTest->SetTestLoopInterval( 1 );

		pMyBasicTest->EnableLog( TRUE );
		pMyBasicTest->ActiveTestItem( IDM_BASIC_API );
		pMyBasicTest->ActiveTestItem( IDM_BASIC_BRIGHTNESS );
		pMyBasicTest->ActiveTestItem( IDM_BASIC_ONOFF );
	}	
}
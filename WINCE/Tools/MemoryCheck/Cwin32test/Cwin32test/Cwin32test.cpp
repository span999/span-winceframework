// Cwin32test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Cwin32test.h"
#include <windows.h>
#include <commctrl.h>
#include <tlhelp32.h>
#include "spLog.h"
#include "DefHead.h"
#include "spToolHelp.h"

#define MAX_LOADSTRING 100



// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndMenuBar;		// menu bar handle
spLog				*g_pLog;			///log object
HWND				g_hWnd;				//  handle
HWND				g_hwndCB;

HWND g_hwnd_option1;					///option1 diag handle

TCHAR szString[64];						///common space for string temp
spToolHelp	ToolHelpBody;				///main body of Toolhelp

PBYTE pNew = NULL;						///pointer for memory allocate
spDLLSUMMARY DllSummary;				///info for Dlls
HANDLE g_hMonitorEvent = NULL;
HANDLE g_hMonitorThread = NULL;

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Option1(HWND, UINT, WPARAM, LPARAM);


///my function
void spLogFuncEnableCtl( spLog* pLog );
BOOL spTimerFuncEnableCtl( BOOL bChange );
BOOL spClearListItem( HWND hDlg, DWORD dwDlgItemID, DWORD dwCount );
BOOL spCheckSHMenuItem( HWND hCreateWND, UINT uiMenuID, UINT uiItemID, BOOL bCheck );
BOOL spClearListBox( HWND hDlg, DWORD dwDlgItemID );
BOOL spInit3rdListBox( HWND hDlg, DWORD dwDlgItemID );


BOOL spFillProcsComboBox( HWND hDlg, DWORD dwDlgItemID );
BOOL spFillProcs2List( HWND hDlg, DWORD dwDlgItemID );
BOOL spFillProcsAddr2List( HWND hDlg, DWORD dwDlgItemID );
BOOL spFillProc2List( HWND hDlg, DWORD dwDlgItemID, DWORD dwIdx );
BOOL spFillMods2List( HWND hDlg, DWORD dwDlgItemID );
BOOL spFillMod2List( HWND hDlg, DWORD dwDlgItemID, DWORD dwIdx );
BOOL spFillModsComboBox( HWND hDlg, DWORD dwDlgItemID );
BOOL spFillProcVMmap2List( HWND hDlg, DWORD dwDlgItemID );
BOOL spFillDllLow2List( HWND hDlg, DWORD dwDlgItemID );

BOOL spIsSHMenuItemChecked( HWND hCreateWND, UINT uiMenuID, UINT uiItemID );
BOOL spGetSHMenuItemInfo( HWND hCreateWND, UINT uiMenuID, UINT uiItemID, MENUITEMINFO *pMii );

DWORD MonitorThreadProc( LPVOID lpvoid );
DWORD GetTimerSetting();
void DoMemRx();
void DoDevHealth();
///function body

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
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CWIN32TEST));

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
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CWIN32TEST));
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

    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the device specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_CWIN32TEST, szWindowClass, MAX_LOADSTRING);

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

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


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
	int idButton = 0;

    static SHACTIVATEINFO s_sai;
	g_hWnd = hWnd;
	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            switch (wmId)
            {
				case ID_OPTION_OPTION1:
					///load the option1 diag
					DialogBox(g_hInst, (LPCTSTR)IDD_OPTION1BOX, hWnd, Option1);
					break;
				case ID_OPTION_OPTION2:
					///MessageBox(NULL, TEXT("Dump all info !!"), TEXT("Toolhelp"), 3);
					ToolHelpBody.LogInfoAllProcess();
					ToolHelpBody.LogInfoSlot( (0x1<<3)|(0x1<<4)|(0x1<<5) );
					ToolHelpBody.ClearIt();					
					break;
				case ID_LOG_ENABLE:
					///if it's checked? log
					///MessageBox(NULL, TEXT("Log enable"), TEXT("Log"), 3);
					spLogFuncEnableCtl( g_pLog );
					NKDbgPrintfW( TEXT("SPD:ID_LOG_ENABLE\r\n") );
					break;
				case ID_LOG_ROOT:
					if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_LOG_ROOT ) )
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_ROOT,  FALSE );
					}
					else
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_ROOT,  TRUE );
					}				
					break;
				case ID_LOG_IPAQ:
					if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_LOG_IPAQ ) )
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_IPAQ,  FALSE );
					}
					else
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_IPAQ,  TRUE );
					}
					break;
				case ID_LOG_MEMORX:
					if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_LOG_MEMORX ) )
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_MEMORX,  FALSE );
					}
					else
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_MEMORX,  TRUE );
					}
					break;
				case ID_LOG_DEVHEALTH:
					if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_LOG_DEVHEALTH ) )
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_DEVHEALTH,  FALSE );
					}
					else
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_DEVHEALTH,  TRUE );
					}
					break;
				case ID_LOG_TOOLHELP:
					if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_LOG_TOOLHELP ) )
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_TOOLHELP,  FALSE );
					}
					else
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_TOOLHELP,  TRUE );
					}
					break;

				case ID_TIMER_ENABLE:
					spTimerFuncEnableCtl( TRUE );
					if( spTimerFuncEnableCtl( FALSE ) )
					{
						///create thread
						if( NULL == g_hMonitorThread )
							g_hMonitorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitorThreadProc, NULL, 0, NULL);
					
						Sleep( 300 );
						if( NULL != g_hMonitorEvent ) {
							SetEvent( g_hMonitorEvent );
						}
					}
					break;
				case ID_TIMER_300SEC:
					if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_TIMER_300SEC ) )
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_300SEC,  FALSE );
					}
					else
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_300SEC,  TRUE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_900SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_1800SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_3600SEC,  FALSE );
					}
					break;
				case ID_TIMER_900SEC:
					if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_TIMER_900SEC ) )
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_900SEC,  FALSE );
					}
					else
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_300SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_900SEC,  TRUE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_1800SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_3600SEC,  FALSE );
					}
					break;
				case ID_TIMER_1800SEC:
					if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_TIMER_1800SEC ) )
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_1800SEC,  FALSE );
					}
					else
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_300SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_900SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_1800SEC,  TRUE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_3600SEC,  FALSE );
					}
					break;
				case ID_TIMER_3600SEC:
					if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_TIMER_3600SEC ) )
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_3600SEC,  FALSE );
					}
					else
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_300SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_900SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_1800SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_3600SEC,  TRUE );
					}
					break;
				case ID_TIMER_SETTING:
					///if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_TIMER_300SEC ) )
					///{
					///	spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_300SEC,  FALSE );
					///}
					///else
					{
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_300SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_900SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_1800SEC,  FALSE );
						spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_3600SEC,  FALSE );
					}
					break;

				case IDM_HELP_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
                    break;
                case IDM_OK:
					idButton = MessageBox( NULL, TEXT("Do you really want to Quit?"), TEXT("Quit or Not?"), (MB_YESNOCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) );
					if(idButton == IDYES)
					{
						idButton = MessageBox( NULL, TEXT("Are you sure?"), TEXT("Quit or Not?"), (MB_YESNOCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) );
						if(idButton == IDYES)
						{
							SendMessage (hWnd, WM_CLOSE, 0, 0);
						}	
					}
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_CREATE:

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
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            
            // TODO: Add any drawing code here...
            
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            CommandBar_Destroy(g_hWndMenuBar);
            PostQuitMessage(0);
            break;

        case WM_ACTIVATE:
            // Notify shell of our activate message
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
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
            {
                // Create a Done button and size it.  
                SHINITDLGINFO shidi;
                shidi.dwMask = SHIDIM_FLAGS;
                shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
                shidi.hDlg = hDlg;
                SHInitDialog(&shidi);
            }
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, message);
            return TRUE;

    }
    return (INT_PTR)FALSE;
}


// Message handler for option1 box.
INT_PTR CALLBACK Option1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	g_hwnd_option1 = hDlg;
    switch (message)
    {
        case WM_INITDIALOG:
            {
                // Create a Done button and size it.  
                SHINITDLGINFO shidi;
                shidi.dwMask = SHIDIM_FLAGS;
                shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
                shidi.hDlg = hDlg;
                SHInitDialog(&shidi);
            }
            return (INT_PTR)TRUE;
			
        case WM_COMMAND:
		
			if (LOWORD(wParam) == IDC_OPTION1_COMBO1)
			{
				if ((HIWORD(wParam) == CBN_SELCHANGE)) {						
					DWORD dwProcIdx = 0;
					//pick up what you want
					dwProcIdx = SendDlgItemMessage( hDlg, IDC_OPTION1_COMBO1, CB_GETCURSEL, 0, 0);
					
					if( dwProcIdx > 0 )
					{
						DWORD dwCurrModStoreProcID = 0;
						
						spClearListItem( hDlg, IDC_OPTION1_LIST1, 0 );
						
						spFillProc2List( hDlg, IDC_OPTION1_LIST1, dwProcIdx );
										
						spClearListBox( hDlg, IDC_OPTION1_COMBO2 );
						spClearListBox( hDlg, IDC_OPTION1_COMBO3 );
						
						///get the modules in this process
						dwCurrModStoreProcID = ToolHelpBody.GetProcessContent( dwProcIdx )->th32ProcessID;
						ToolHelpBody.ClearSnapModule();
						ToolHelpBody.SnapModule( dwCurrModStoreProcID );
						ToolHelpBody.ClearSnapHeap();
						ToolHelpBody.SnapHeap( dwCurrModStoreProcID );

						swprintf( szString, TEXT("  ModuleNum=%d"), ToolHelpBody.GetCurModuleCount() );
						SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );

						///list down the modules of this process
						spFillMods2List( hDlg, IDC_OPTION1_LIST1 );
						///list down the modules of this process
						spFillModsComboBox( hDlg, IDC_OPTION1_COMBO2 );

						///spInit3rdListBox( hDlg, IDC_OPTION1_COMBO3 );
						
						///list VM map
						spClearListItem( hDlg, IDC_OPTION1_LIST2, 0 );
						spFillProcVMmap2List( hDlg, IDC_OPTION1_LIST2 );					
						spFillDllLow2List( hDlg, IDC_OPTION1_LIST2 );
					}
					else
					{
						spClearListItem( hDlg, IDC_OPTION1_LIST1, 0 );
						spClearListItem( hDlg, IDC_OPTION1_LIST2, 0 );
						spClearListBox( hDlg, IDC_OPTION1_COMBO1 );
						spClearListBox( hDlg, IDC_OPTION1_COMBO2 );
						spClearListBox( hDlg, IDC_OPTION1_COMBO3 );

						spFillProcsComboBox( hDlg, IDC_OPTION1_COMBO1 );
						spFillProcs2List( hDlg, IDC_OPTION1_LIST1 );
						spFillProcsAddr2List( hDlg, IDC_OPTION1_LIST2 );
						spFillDllLow2List( hDlg, IDC_OPTION1_LIST2 );

					}
				}
				return TRUE;
            }
			else
			if (LOWORD(wParam) == IDC_OPTION1_COMBO2)
			{
				if ((HIWORD(wParam) == CBN_SELCHANGE)) {						
					DWORD dwModIdx = 0;
					dwModIdx = SendDlgItemMessage( hDlg, IDC_OPTION1_COMBO2, CB_GETCURSEL, 0, 0);
					spClearListItem( hDlg, IDC_OPTION1_LIST1, 0 );
					
					if( dwModIdx != CB_ERR )
					{
						spFillMod2List( hDlg, IDC_OPTION1_LIST1, dwModIdx+1 );
					}
					else
					{
						swprintf( szString, TEXT("Get CB list fail!=0x%08x"), dwModIdx );
						SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );
					}
				}
				return TRUE;
            }
			else
			if (LOWORD(wParam) == IDC_OPTION1_COMBO3)
			{
				if ((HIWORD(wParam) == CBN_SELCHANGE)) {						
					DWORD dwCB3Idx = 0;
					dwCB3Idx = SendDlgItemMessage( hDlg, IDC_OPTION1_COMBO3, CB_GETCURSEL, 0, 0);
/*					
					spClearListItem( hDlg, IDC_OPTION1_LIST2, 0 );
					
					if( dwCB3Idx != CB_ERR )
					{
						if( 0 == dwCB3Idx )
						{
							///VM table
							spDumpProcVMmap2List( hDlg, IDC_OPTION1_LIST2, &modEntryStore, &heapListStore );
						}
						else
						if( 1 == dwCB3Idx )
						{
							///Heap info list
							spDumpHeapListStore2List( hDlg, IDC_OPTION1_LIST2, &heapListStore, FALSE );
						}
						else
						if( 2 == dwCB3Idx )
						{
							///Heap info list
							spDumpHeapListStore2List( hDlg, IDC_OPTION1_LIST2, &heapListStore, TRUE );
						}
					}
					else
					{
						swprintf( szString, TEXT("Get CB list fail!=0x%08x"), dwCB3Idx );
						SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );
					}
*/
				}
				return TRUE;
            }
			else
			if (LOWORD(wParam) == IDOK)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
			else
            if (LOWORD(wParam) == IDC_OPTION1_BUTTON2)
            {	
				///cls button
				///clear list
				spClearListItem( hDlg, IDC_OPTION1_LIST1, 0 );
				spClearListItem( hDlg, IDC_OPTION1_LIST2, 0 );
				spClearListBox( hDlg, IDC_OPTION1_COMBO1 );
				spClearListBox( hDlg, IDC_OPTION1_COMBO2 );
				spClearListBox( hDlg, IDC_OPTION1_COMBO3 );
				
                return TRUE;
            }
			else
            if (LOWORD(wParam) == IDC_OPTION1_BUTTON1)
            {
				///shot button
				ToolHelpBody.ClearSnapProcess();
				ToolHelpBody.SnapProcess();
				ToolHelpBody.ClearSnapThread();
				ToolHelpBody.SnapThread();
				ToolHelpBody.ClearSnapStack();
				ToolHelpBody.SnapStack();
				ToolHelpBody.ClearSnapModule();
				ToolHelpBody.SnapModule();

				spFillProcsComboBox( hDlg, IDC_OPTION1_COMBO1 );
				spFillProcs2List( hDlg, IDC_OPTION1_LIST1 );
				spFillProcsAddr2List( hDlg, IDC_OPTION1_LIST2 );
				spFillDllLow2List( hDlg, IDC_OPTION1_LIST2 );

                return TRUE;
            }
            if (LOWORD(wParam) == IDC_OPTION1_BUTTON3)
            {	
				///new button
				if( pNew == NULL )
					pNew = (PBYTE)malloc( 1234567 );
                return TRUE;
            }
			else
            if (LOWORD(wParam) == IDC_OPTION1_BUTTON4)
            {	
				///free button
				if( pNew != NULL )
				{
					free( pNew );
					pNew = NULL;
				}
                return TRUE;
            }
			
            break;

        case WM_CLOSE:
            EndDialog(hDlg, message);
            return TRUE;

    }
    return (INT_PTR)FALSE;
}


void spLogFuncEnableCtl( spLog *pLog )
{
	static BOOL bLogEnabled = FALSE;///log enable or not

	if( bLogEnabled )
	{
		if( pLog != NULL )
		{
			if( pLog->LogDone() )
			{
				spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_ENABLE,  FALSE );
				bLogEnabled = FALSE;
				delete pLog;
				pLog = NULL;
			}
			else
			{	
				MessageBox(NULL, TEXT("LogDone fail"), TEXT("Error"), 3);
			}
		}
		else
		{
			spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_ENABLE,  FALSE );
			bLogEnabled = FALSE;
		}
	}
	else
	{
		if( pLog == NULL )
			pLog = new spLog();

		if( pLog != NULL )
		{
			if( pLog->LogEnable() )
			{
				spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_LOG_ENABLE,  TRUE );
				bLogEnabled = TRUE;
			}
			else
			{	
				MessageBox(NULL, TEXT("LogEnable fail"), TEXT("Error"), 3);
			}
		}
		else
		{
			MessageBox(NULL, TEXT("new spLog() fail"), TEXT("Error"), 3);
		}
	}

}

BOOL spTimerFuncEnableCtl( BOOL bChange )
{
	static BOOL bTimerEnabled = FALSE;

	///dwOption = 1 -> enable
	///dwOption = 2 -> disable
	if( !bChange )
		return bTimerEnabled;
	
	if( bTimerEnabled )
	{
		spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_ENABLE,  FALSE );
		bTimerEnabled = FALSE;
	}
	else
	{
		spCheckSHMenuItem( g_hWnd, IDM_HELP, ID_TIMER_ENABLE,  TRUE );
		bTimerEnabled = TRUE;
	}

	return bTimerEnabled;
}


BOOL spClearListItem( HWND hDlg, DWORD dwDlgItemID, DWORD dwCount )
{
	BOOL bRet = FALSE;
	DWORD dwListItemCnt = 0;
    
	///get the item count	
	dwListItemCnt = SendDlgItemMessage( hDlg, dwDlgItemID, LB_GETCOUNT, 0, 0 );
	while( dwListItemCnt > 0 ) {
		SendDlgItemMessage( hDlg, dwDlgItemID, LB_DELETESTRING, 0, 0 );	///delete item 0
		dwListItemCnt--;
	}

	return bRet;
}


BOOL spCheckSHMenuItem( HWND hCreateWND, UINT uiMenuID, UINT uiItemID, BOOL bCheck )
{
	BOOL bRet = FALSE;

	HWND hMenuMB = NULL;
	HMENU hMenu = NULL;
	
	hMenuMB = (HWND)SHFindMenuBar( hCreateWND );

	if( hMenuMB == NULL )
	{	///error handle
		MessageBox(NULL, TEXT("hMenuMB==NULL"), TEXT("Error"), 3);
		return bRet;
	}
	
	hMenu = (HMENU)SendMessage( hMenuMB, SHCMBM_GETSUBMENU, 0, uiMenuID );
	if( hMenu == NULL )
	{	///error handle
		MessageBox(NULL, TEXT("hMenu==NULL"), TEXT("Error"), 3);
		return bRet;
	}
	
	if (bCheck)
		CheckMenuItem( hMenu, uiItemID, MF_BYCOMMAND|MF_CHECKED );
	else
		CheckMenuItem( hMenu, uiItemID, MF_BYCOMMAND|MF_UNCHECKED );

	return bRet;
}

BOOL spIsSHMenuItemChecked( HWND hCreateWND, UINT uiMenuID, UINT uiItemID )
{
	BOOL bRet = FALSE;

	MENUITEMINFO menuIteminfo;
	
	if( spGetSHMenuItemInfo( hCreateWND, uiMenuID, uiItemID, &menuIteminfo ) )
	{
		bRet = (menuIteminfo.fState & MFS_CHECKED)?TRUE:FALSE;
	}

	return bRet;
}

BOOL spGetSHMenuItemInfo( HWND hCreateWND, UINT uiMenuID, UINT uiItemID, MENUITEMINFO *pMii )
{
	BOOL bRet = FALSE;

	HWND hMenuMB = NULL;
	HMENU hMenu = NULL;
	
	hMenuMB = (HWND)SHFindMenuBar( hCreateWND );

	if( hMenuMB == NULL )
	{	///error handle
		MessageBox(NULL, TEXT("hMenuMB==NULL"), TEXT("Error"), 3);
		return bRet;
	}
	
	hMenu = (HMENU)SendMessage( hMenuMB, SHCMBM_GETSUBMENU, 0, uiMenuID );
	if( hMenu == NULL )
	{	///error handle
		MessageBox(NULL, TEXT("hMenu==NULL"), TEXT("Error"), 3);
		return bRet;
	}
	
	if( hMenu )
	{
	    memset( pMii, 0, sizeof(MENUITEMINFO));
    	pMii->cbSize = sizeof(MENUITEMINFO);
    	pMii->fMask = (MIIM_CHECKMARKS|MIIM_DATA|MIIM_ID|MIIM_STATE|MIIM_SUBMENU|MIIM_TYPE);
		
		bRet = GetMenuItemInfo( hMenu, uiItemID, FALSE, pMii );
	}	
	
	return bRet;
}


BOOL spClearListBox( HWND hDlg, DWORD dwDlgItemID )
{
	BOOL bRet = TRUE;
	DWORD dwCount = 0;
	
	dwCount = SendDlgItemMessage( hDlg, dwDlgItemID, CB_GETCOUNT, 0, 0 );
	
	while( dwCount > 0 )
	{
		///SendDlgItemMessage(hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)ProcEntery.szExeFile );
		///swprintf( szString, TEXT("%s"), pList->procEntryA[dwCount].szExeFile );
		SendDlgItemMessage( hDlg, dwDlgItemID, CB_DELETESTRING, 0, 0 );
		///SSendDlgItemMessage( hDlg, dwDlgItemID, CB_ADDSTRING, 0, (LPARAM)pList->procEntryA[dwCount].szExeFile );
		dwCount--;
	}
	
	///SendDlgItemMessage( hDlg, dwDlgItemID, CB_SETCURSEL, 0, 0 );
	
	return bRet;
}


BOOL spInit3rdListBox( HWND hDlg, DWORD dwDlgItemID )
{
	BOOL bRet = TRUE;
	DWORD dwCount = 0;
	
	swprintf( szString, TEXT("VM table") );
	SendDlgItemMessage( hDlg, dwDlgItemID, CB_ADDSTRING, 0, (LPARAM)szString );

	swprintf( szString, TEXT("Heap list") );
	SendDlgItemMessage( hDlg, dwDlgItemID, CB_ADDSTRING, 0, (LPARAM)szString );

	swprintf( szString, TEXT("Heap detail") );
	SendDlgItemMessage( hDlg, dwDlgItemID, CB_ADDSTRING, 0, (LPARAM)szString );
	
	SendDlgItemMessage( hDlg, dwDlgItemID, CB_SETCURSEL, 0, 0 );
	
	return bRet;
}



BOOL spFillProcsComboBox( HWND hDlg, DWORD dwDlgItemID )
{
	BOOL bRet = FALSE;
	DWORD dwIndex = 1;
	DWORD dwProcCount = 0;

	dwProcCount = ToolHelpBody.GetCurProcessCount();
	if( dwProcCount < 1 )
		return bRet;
		
	swprintf( szString, TEXT("All Process") );
	SendDlgItemMessage( hDlg, dwDlgItemID, CB_ADDSTRING, 0, (LPARAM)szString );

	while( dwIndex <= dwProcCount )
	{
		swprintf( szString, TEXT("%s"), ToolHelpBody.GetProcessContent( dwIndex )->szExeFile );
		SendDlgItemMessage( hDlg, dwDlgItemID, CB_ADDSTRING, 0, (LPARAM)szString );
		dwIndex++;
	}
	
	SendDlgItemMessage( hDlg, dwDlgItemID, CB_SETCURSEL, 0, 0 );
	
	return bRet;
}


BOOL spFillProcsAddr2List( HWND hDlg, DWORD dwDlgItemID )
{
	BOOL bRet = TRUE;
	DWORD dwIndex = 1;
	DWORD dwProcCount = 0;
	PPROCESSENTRY32 pProc = NULL;

	dwProcCount = ToolHelpBody.GetCurProcessCount();
	if( dwProcCount < 1 )
		return bRet;

	while( dwIndex <= dwProcCount )
	{
		pProc = ToolHelpBody.GetProcessContent( dwIndex );
		if( pProc != NULL )
		{
			swprintf( szString, TEXT("%s(0x%08x)"), pProc->szExeFile, pProc->th32ProcessID );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );		
			swprintf( szString, TEXT("  MemBasAdr=0x%08x"), pProc->th32MemoryBase );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
		}
		dwIndex++;
	}
	
	return bRet;
}

BOOL spFillProcs2List( HWND hDlg, DWORD dwDlgItemID )
{
	BOOL bRet = TRUE;
	DWORD dwIndex = 1;
	DWORD dwProcCount = 0;
	PPROCESSENTRY32 pProc = NULL;

	dwProcCount = ToolHelpBody.GetCurProcessCount();
	if( dwProcCount < 1 )
		return bRet;

	swprintf( szString, TEXT("Total %d processes"), dwProcCount );
	SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );		

	while( dwIndex <= dwProcCount )
	{
		pProc = ToolHelpBody.GetProcessContent( dwIndex );
		if( pProc != NULL )
		{
			swprintf( szString, TEXT("%s"), pProc->szExeFile );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );		
			swprintf( szString, TEXT("  ProcID=0x%08x"), pProc->th32ProcessID );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
			swprintf( szString, TEXT("  DefHeapID=0x%08x"), pProc->th32DefaultHeapID );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
			swprintf( szString, TEXT("  ThreadNum=%d"), pProc->cntThreads  );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
			swprintf( szString, TEXT("  MemBasAdr=0x%08x"), pProc->th32MemoryBase );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
		}
		dwIndex++;
	}
	
	return bRet;
}


BOOL spFillProc2List( HWND hDlg, DWORD dwDlgItemID, DWORD dwIdx )
{
	BOOL bRet = TRUE;
	DWORD dwProcCount = 0;
	PPROCESSENTRY32 pProc = NULL;

	dwProcCount = ToolHelpBody.GetCurProcessCount();
	pProc = ToolHelpBody.GetProcessContent( dwIdx );
	if( (dwProcCount < 1) || (pProc == NULL) )
		return bRet;

	swprintf( szString, TEXT("selet %s(0x%08x)"), pProc->szExeFile, pProc->th32ProcessID );
	SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );
						
	///process detail
	swprintf( szString, TEXT("%s"), pProc->szExeFile );
	SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );		
	swprintf( szString, TEXT("  DefHeapID=0x%08x"), pProc->th32DefaultHeapID );
	SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
	swprintf( szString, TEXT("  ThreadNum=%d"), pProc->cntThreads  );
	SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
	swprintf( szString, TEXT("  MemBasAdr=0x%08x"), pProc->th32MemoryBase );
	SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
	
	return bRet;
}


BOOL spFillMods2List( HWND hDlg, DWORD dwDlgItemID )
{
	BOOL bRet = FALSE;
	DWORD dwIndex = 1;
	DWORD dwModCount = 0;
	PMODULEENTRY32 pMod = NULL;

	dwModCount = ToolHelpBody.GetCurModuleCount();
	if( (dwModCount < 1) )
		return bRet;

	swprintf( szString, TEXT("Total %d modules"), dwModCount );
	SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );		
	
	while( dwIndex <= dwModCount )
	{
		pMod = ToolHelpBody.GetModuleContent( dwIndex );
		if( NULL != pMod )
		{	
			swprintf( szString, TEXT("%s"), pMod->szModule );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );

			swprintf( szString, TEXT("  ModID=0x%08x"), pMod->th32ModuleID );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
			swprintf( szString, TEXT("  ProcID=0x%08x"), pMod->th32ProcessID );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
			swprintf( szString, TEXT("  ModGUsed=%d"), pMod->GlblcntUsage );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
			swprintf( szString, TEXT("  ModLUsed=%d"), pMod->ProccntUsage );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
			swprintf( szString, TEXT("  ModBasAdr=0x%08x"), pMod->modBaseAddr );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
			swprintf( szString, TEXT("  ModBasSiz=%d"), pMod->modBaseSize );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
			swprintf( szString, TEXT("  ModHand=0x%08x"), pMod->hModule );
			SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
		}
		dwIndex++;
	}
	
	return bRet;
}

BOOL spFillMod2List( HWND hDlg, DWORD dwDlgItemID, DWORD dwIdx )
{
	BOOL bRet = TRUE;
	DWORD dwModCount = 0;
	PMODULEENTRY32 pMod = NULL;

	dwModCount = ToolHelpBody.GetCurModuleCount();
	pMod = ToolHelpBody.GetModuleContent( dwIdx );
	if( (dwModCount < 1) || (pMod == NULL) )
		return bRet;

	swprintf( szString, TEXT("selet %s(0x%08x)"), pMod->szModule, pMod->th32ModuleID );
	SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );

	swprintf( szString, TEXT("  ProcID=0x%08x"), pMod->th32ProcessID );
	SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );
	swprintf( szString, TEXT("  ModGUsed=%d"), pMod->GlblcntUsage );
	SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );
	swprintf( szString, TEXT("  ModLUsed=%d"), pMod->ProccntUsage );
	SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );
	swprintf( szString, TEXT("  ModBasAdr=0x%08x"), pMod->modBaseAddr );
	SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );
	swprintf( szString, TEXT("  ModBasSiz=%d"), pMod->modBaseSize );
	SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );
	swprintf( szString, TEXT("  ModHand=0x%08x"), pMod->hModule );
	SendDlgItemMessage( hDlg, IDC_OPTION1_LIST1, LB_ADDSTRING, 0, (LPARAM)szString );
		
	return bRet;
}


BOOL spFillModsComboBox( HWND hDlg, DWORD dwDlgItemID )
{
	BOOL bRet = TRUE;
	DWORD dwIndex = 1;
	DWORD dwModCount = 0;
	PMODULEENTRY32 pMod = NULL;

	dwModCount = ToolHelpBody.GetCurModuleCount();
	if( (dwModCount < 1) )
		return bRet;
	
	while( dwIndex <= dwModCount )
	{
		pMod = ToolHelpBody.GetModuleContent( dwIndex );
		if( NULL != pMod )
		{
			swprintf( szString, TEXT("%s"), pMod->szModule );
			SendDlgItemMessage( hDlg, dwDlgItemID, CB_ADDSTRING, 0, (LPARAM)szString );
		}

		dwIndex++;
	}

	SendDlgItemMessage( hDlg, dwDlgItemID, CB_SETCURSEL, 0, 0 );
	
	return bRet;
}

BOOL spFillProcVMmap2List( HWND hDlg, DWORD dwDlgItemID )
{
	BOOL bRet = TRUE;
	int dwCount = 31;
	DWORD dwCutInx = 0;//0~15
	DWORD dwLineBaseAddr = 0;
/*
	~(0x00000~0x10000)
	 ~(0x10000~0x20000)
	   ~(0x20000~0x30000)
	     ~(0x30000~0x40000)
*/
	swprintf( szString, TEXT("VM map table: 64kB for each char") );
	SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
	swprintf( szString, TEXT("VM ADDrs:################") );
	SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );

	swprintf( szString, TEXT("02000000:                ") );
	SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );

    
	while( dwCount >= 0 )
	{
	    dwLineBaseAddr = (dwCount<<20);
		swprintf( szString, TEXT("%08X:%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"),
			dwLineBaseAddr,
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0x00000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0x10000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0x20000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0x30000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0x40000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0x50000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0x60000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0x70000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0x80000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0x90000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0xA0000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0xB0000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0xC0000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0xD0000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0xE0000, 0x10000 ),
			ToolHelpBody.CheckVMblockSign( dwLineBaseAddr+0xF0000, 0x10000 )
			);
		SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
		dwCount--;
	}

	
	return bRet;
}


BOOL spFillDllLow2List( HWND hDlg, DWORD dwDlgItemID )
{
	BOOL bRet = TRUE;
	DWORD dwModCount = 0;

	dwModCount = ToolHelpBody.GetCurModuleCount();
	if( (dwModCount < 1) )
		return bRet;

	swprintf( szString, TEXT("Dll Low=0x%08x"), ToolHelpBody.GetMuduleDllLow()  );
	SendDlgItemMessage( hDlg, dwDlgItemID, LB_ADDSTRING, 0, (LPARAM)szString );
	
	return bRet;
}

DWORD MonitorThreadProc( LPVOID lpvoid )
{
	DWORD dwRet = 0;
	DWORD dwWait = INFINITE;
	DWORD dwRsp = 0;
	
	if( NULL == g_hMonitorEvent )
		g_hMonitorEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	///dwWait = GetTimerSetting();
	
	while(1)
	{
		dwRet = WaitForSingleObject( g_hMonitorEvent, dwWait );

		dwWait = GetTimerSetting();
		
		if(	spTimerFuncEnableCtl( FALSE ) )
		{
			NKDbgPrintfW( TEXT("SPD:Log...\r\n") );
			
			if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_LOG_MEMORX ) )
			{
				DoMemRx();
			}
			
			if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_LOG_DEVHEALTH ) )
			{
				DoDevHealth();
			}
			
			if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_LOG_TOOLHELP ) )
			{
		#if 1
			///dump overall system memory status
			ToolHelpBody.LogInfoAllProcess();
			///dump system memory status base on slot
			ToolHelpBody.LogInfoSlot( (0x1<<4)|(0x1<<5)|(0x1<<6) );
			ToolHelpBody.ClearIt();
		#endif
			}
			NKDbgPrintfW( TEXT("SPD:Log...done %d\r\n"), dwWait );
		}
	}

	return dwRet;
}


DWORD GetTimerSetting()
{
	DWORD dwRet = 0;

	if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_TIMER_300SEC ) )
		dwRet = (5*60*1000);
	else
	if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_TIMER_900SEC ) )
		dwRet = (15*60*1000);
	else
	if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_TIMER_1800SEC ) )
		dwRet = (30*60*1000);
	else
	if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_TIMER_3600SEC ) )
		dwRet = (60*60*1000);
	else
		dwRet = (60*60*1000);

	NKDbgPrintfW( TEXT("SPD:Next Log time...%dms\r\n"), dwRet );
	return dwRet;
}

void DoMemRx()
{
	BOOL bRet = FALSE;
	static DWORD dwCount = 0;
	
	bRet = CreateProcess( MEMORX_BINARY_IPAQSTORE_LOCATION, NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
	
	dwCount++;
	if( spIsSHMenuItemChecked( g_hWnd, IDM_HELP, ID_LOG_ROOT ) )
		swprintf( szString, MEMORX_ROOTSTORE_LOG_FILES, dwCount );
	else
		swprintf( szString, MEMORX_IPAQSTORE_LOG_FILES, dwCount );
	
	if( bRet )
	{
		Sleep( 1000 );
		bRet = CopyFile(TEXT("\\memoRx.log"), szString, FALSE );
	}	
		
	if( bRet )
		bRet = DeleteFile(TEXT("\\memoRx.log"));

}

void DoDevHealth()
{
	BOOL bRet = FALSE;
	static DWORD dwCount = 0;
	
	bRet = CreateProcess( DEVHEALTH_BINARY_IPAQSTORE_LOCATION, NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
	
}
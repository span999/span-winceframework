// micAutoRunner.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "micAutoRunner.h"
#include <commctrl.h>

#define MAX_LOADSTRING 100

#define AUTORUNNER_EVENT_NAME					L"named autorunner event"
#define MIC_AUTORUNNER_REG_KEY					(TEXT("\\Software\\Mitac\\micAutoRunner"))

///CmdOption flag
#define 	CMDOPTION_SHOWAUTORUNNERDLG		0x10000000



// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle
HANDLE 				hAutoRunnerThread = NULL;
HANDLE 				hAutoRunnerEvent = NULL;
HWND				hWndAR;			// The AutoRuner dialog handle
BOOL 				bKeepCmdFile = FALSE;	

/*
	"CmdFilePath"="\\SDMMC Card\\"
	"CmdFileName"="micAR.cmd"
	"RspFileName"="micAR.rsp"
;;; Cmd list
	"CmdSign"="#"
	"CmdMark"=";"
	"CmdSource"="SRC:"
	"CmdTarget"="TARG:"
	"CmdRun"="RUN:"
	"CmdCopy"="COPY:"
	"CmdMove"="MOVE:"
	"CmdDelete"="DEL:"
	"CmdEnd"="END:"	
*/
TCHAR szCmdFilePath[32];
TCHAR szCmdFileName[32];
TCHAR szRspFileName[32];
DWORD dwCmdOption = 0;
TCHAR szCmdSign[4];
TCHAR szCmdMark[4];
TCHAR szCmdSource[16];
TCHAR szCmdTarget[16];
TCHAR szCmdLine[16];
TCHAR szCmdRun[16];
TCHAR szCmdCopy[16];
TCHAR szCmdMove[16];
TCHAR szCmdDelete[16];
TCHAR szCmdEnd[16];	
TCHAR szSourceFile[256];
TCHAR szTargetFile[256];
TCHAR szRunCmdLine[256];
TCHAR szCmdHide[16];
TCHAR szCmdShow[16];
TCHAR szCmdWait[16];
TCHAR szCmdKeepCmd[32];


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	AutoRunner		(HWND, UINT, WPARAM, LPARAM);
void 	WINAPI 		AutoRunnerThreadProc( HWND hDlg );
BOOL 				LoadRegSetting	( HWND hDlg );
void				ShowAutoRunerDlg();
void				TryCenterDlg	( HWND hThisWnd, BOOL bYes );

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

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MICAUTORUNNER);

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
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MICAUTORUNNER));
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
	LoadString(hInstance, IDC_MICAUTORUNNER, szWindowClass, MAX_LOADSTRING);
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
///	if (hwndCB)
///		CommandBar_Show(hwndCB, TRUE);

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
///			hwndCB = CommandBar_Create(hInst, hWnd, 1);			
///			CommandBar_InsertMenubar(hwndCB, hInst, IDM_MENU, 0);
///			CommandBar_AddAdornments(hwndCB, 0, 0);

			DialogBox(hInst, (LPCTSTR)IDD_AUTORUNBOX, hWnd, (DLGPROC)AutoRunner);
			
			break;
		case WM_PAINT:
///			RECT rt;
///			hdc = BeginPaint(hWnd, &ps);
///			GetClientRect(hWnd, &rt);
///			LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
///			DrawText(hdc, szHello, _tcslen(szHello), &rt, 
///				DT_SINGLELINE | DT_VCENTER | DT_CENTER);
///			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
///			CommandBar_Destroy(hwndCB);
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


// Mesage handler for the AutoRunner box.
LRESULT CALLBACK AutoRunner(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
///	RECT rt, rt1;
///	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
///	int NewPosX, NewPosY;
	TCHAR szString[64];         // Temperary string
	
	switch (message)
	{
		case WM_INITDIALOG:
			
			hWndAR = hDlg;
			
			TryCenterDlg( hWndAR, FALSE );
///			// trying to center the dialog
///			if (GetWindowRect(hDlg, &rt1)) {
///				GetClientRect(GetParent(hDlg), &rt);
///				DlgWidth	= rt1.right - rt1.left;
///				DlgHeight	= rt1.bottom - rt1.top ;
///				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
///				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
///				
///				// if the box is larger than the physical screen 
///				if (NewPosX < 0) NewPosX = 0;
///				if (NewPosY < 0) NewPosY = 0;
///				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
///					0, 0, SWP_NOZORDER | SWP_NOSIZE);
///			}
			
			///Create event
			if( NULL == hAutoRunnerEvent )
				hAutoRunnerEvent = CreateEvent(NULL, FALSE, FALSE, AUTORUNNER_EVENT_NAME);
			
			///Create Thread for cmd handle
			if( hAutoRunnerThread == NULL )
			{	
				hAutoRunnerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AutoRunnerThreadProc, (LPVOID)hDlg, 0, NULL );
				if( NULL == hAutoRunnerThread )
				{	
					wsprintf (szString, TEXT("CreateThread Fail !!! 0x%08x"), GetLastError() );
					SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				}	
				else
					CloseHandle(hAutoRunnerThread);
			}
			
			return TRUE;
			
		///case WM_DEIVCECHANGE:
		case 0x0219:
			wsprintf (szString, TEXT("WM_DEIVCECHANGE %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
			SetEvent( hAutoRunnerEvent );
			
			return TRUE;
			
		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK))
			{
				NKDbgPrintfW(L"IDOK\r\n" );
				TryCenterDlg( hWndAR, FALSE );
			}
			else
			if ((LOWORD(wParam) == IDCANCEL))
			{
				NKDbgPrintfW(L"IDCANCEL\r\n" );
				EndDialog(hDlg, LOWORD(wParam));
				///kill the windows after dialog end...
				PostQuitMessage(0);
				return TRUE;
			}
			break;
	}
    return FALSE;
}




void WINAPI AutoRunnerThreadProc( HWND hDlg )
{
	
	DWORD dwWaitRet = WAIT_FAILED;
	TCHAR szString[256];         // Temperary string
///	HKEY hKey;
///	BYTE pValueData[256];
///	DWORD dwValueType = 0;
///	DWORD dwValueLen = 0;

	Sleep(10);
	ShowAutoRunerDlg();

	/// get info from registry
	LoadRegSetting( hDlg );
			
	while(1)
	{
		dwWaitRet = WaitForSingleObject( hAutoRunnerEvent, INFINITE );
		
		if( dwWaitRet == WAIT_OBJECT_0 )
		{
		    FILE *fp = NULL;
			BYTE pPathFileData[256];
			PWSTR pTemp = (PWSTR)pPathFileData;
			UINT uiReTry = 5;
			
			wcscpy( (PWSTR)pPathFileData, (PWSTR)szCmdFilePath );
			pTemp = pTemp + (wcslen((PWSTR)pPathFileData));
			wcscpy( (PWSTR)pTemp, (PWSTR)szCmdFileName );
			
			wsprintf (szString, TEXT("Try reach cmd file %s"), pPathFileData );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			

#if 0
			Sleep(2000);	///??? wait for ready?
#else
			for( uiReTry = 4; uiReTry > 0; uiReTry-- ) 
			{
				Sleep(500);
				fp = _wfopen( (PWSTR)pPathFileData , _T("rt") );	///read,text mode
				///fp = fopen( "\\SDMMC Card\\micAR.cmd" , "rt" );	///read,text mode
				if( fp != NULL )
					break; 
			}	
#endif
			if( fp == NULL ) 
			{
				wsprintf (szString, TEXT("No such file %s"), pPathFileData );
				SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
			}
			else
			{
				#define MAX_LINE_LENGTH		256
				CHAR ucTempA[MAX_LINE_LENGTH];
				UINT uiIndex = 0;
				
				wsprintf (szString, TEXT("%s file found"), pPathFileData );
				SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///Parsing file
			    while(1)
			    {
			    	///clean buffer
			    	for( uiIndex = 0; uiIndex < MAX_LINE_LENGTH; uiIndex++ )
			    		ucTempA[MAX_LINE_LENGTH] = '\0';

			    	///get a line
			        if( fgets( ucTempA, MAX_LINE_LENGTH, fp) == NULL )	///NULL indicate error or EOF.
			        {
						wsprintf (szString, TEXT("EOF or Error !!!") );
						SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			        	break;
			        }
			        	
			        if( (';' == ucTempA[0]) )	///';' ignore char
			        {
						///wsprintf (szString, TEXT("Ignore sign!!!") );
						///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			        	continue;	
			        }
			        else
			        if( ('#' == ucTempA[0]) )	///'#' command
			        {
			        	UINT uiIdx = 1;
			        	TCHAR szTempStr[12];         // Temperary string
			        	TCHAR szTempStr1[128];         // Temperary string
			        	TCHAR *pTempStrPointer;
			        	TCHAR *pTempStrPointer1;
			        	
			        	bKeepCmdFile = FALSE;
			        	
						///wsprintf (szString, TEXT("Command sign!!!") );
						///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						
						///copy cmd to buffer
						pTempStrPointer = szTempStr;
						while( (':' != ucTempA[uiIdx]) )
						{
							///wsprintf (szString, TEXT("Cmd->%c") , ucTempA[uiIdx] );
							///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							
							*pTempStrPointer = (TCHAR)ucTempA[uiIdx];
							uiIdx++;
							pTempStrPointer++;
						}	
						*pTempStrPointer = (TCHAR)':';	///add ':'
						pTempStrPointer++;
						*pTempStrPointer = (TCHAR)'\0';	///add '\0'
						pTempStrPointer = szTempStr;


						///copy cmd parameter to buffer
						pTempStrPointer1 = szTempStr1;
						uiIdx++;
						while( ('\0' != ucTempA[uiIdx]) )
						{
							///wsprintf (szString, TEXT("Cmd->%c") , ucTempA[uiIdx] );
							///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							
							*pTempStrPointer1 = (TCHAR)ucTempA[uiIdx];
							uiIdx++;
							pTempStrPointer1++;
						}
						*pTempStrPointer1 = (TCHAR)'\0';	///add '\0'
						pTempStrPointer1--;
						*pTempStrPointer1 = (TCHAR)'\0';	///add '\0'
						///wsprintf (szString, TEXT("Cmd->%s") , szTempStr1 );
						///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

						
						///now, compare with cmd list
						if( 0 == wcscmp( szCmdSource, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							wcscpy( szSourceFile, szTempStr1 );
							wsprintf (szString, TEXT("Copy SRC->%s") , szSourceFile );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

						}
						else
						if( 0 == wcscmp( szCmdTarget, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							wcscpy( szTargetFile, szTempStr1 );
							wsprintf (szString, TEXT("Copy TARG->%s") , szTargetFile );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);							
						}
						else
						if( 0 == wcscmp( szCmdLine, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							wcscpy( szRunCmdLine, szTempStr1 );
							wsprintf (szString, TEXT("Copy CMDL->%s") , szRunCmdLine );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);							
						}
						else
						if( 0 == wcscmp( szCmdRun, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

							wsprintf (szString, TEXT("->%s"), szSourceFile );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

							///CreateProcess( szSourceFile, NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
							CreateProcess( szSourceFile, szRunCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
						}
						else
						if( 0 == wcscmp( szCmdCopy, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							wsprintf (szString, TEXT("%s->"), szSourceFile );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							wsprintf (szString, TEXT("%s"), szTargetFile );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

							CopyFile( szSourceFile, szTargetFile, TRUE);
						}
						else
						if( 0 == wcscmp( szCmdMove, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							
							wsprintf (szString, TEXT("%s->"), szSourceFile );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							wsprintf (szString, TEXT("%s"), szTargetFile );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

							MoveFile( szSourceFile, szTargetFile );
								
						}
						else
						if( 0 == wcscmp( szCmdDelete, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							
							DeleteFile( szSourceFile );
						}
						else
						if( 0 == wcscmp( szCmdEnd, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

							///move the screen to the last message		
							DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_GETCOUNT, 0, 0);							
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
							
							///if( !bKeepCmdFile )
							///	;
							
							break;
						}
						else
						if( 0 == wcscmp( szCmdShow, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							
							///set the option show autorunner dialog flag
							dwCmdOption = dwCmdOption | CMDOPTION_SHOWAUTORUNNERDLG;
							///Do it
							ShowAutoRunerDlg();
						}
						else
						if( 0 == wcscmp( szCmdHide, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							
							///set the option show autorunner dialog flag
							dwCmdOption = dwCmdOption & ~CMDOPTION_SHOWAUTORUNNERDLG;
							///Do it
							ShowAutoRunerDlg();
						}
						else
						if( 0 == wcscmp( szCmdWait, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							
							Sleep(1000);
						}
						else
						if( 0 == wcscmp( szCmdKeepCmd, pTempStrPointer ) )
						{
							wsprintf (szString, TEXT("Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
							
							bKeepCmdFile = TRUE;
						}
						else
						{
							wsprintf (szString, TEXT("?Cmd->%s"), pTempStrPointer );
							SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						}
						
						///move the screen to the last message		
						///DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_GETCOUNT, 0, 0);							
						///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

			        	continue;
			        }	

			    }

				
				fclose( fp );
			}

			
		}
		else
		if( dwWaitRet == WAIT_TIMEOUT )
		{
			wsprintf (szString, TEXT("wait Time out !!"));
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
		}
		else
		{
			wsprintf (szString, TEXT("wait unknow !!"));
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			

		}
			
	}///while()	
	
}


void ShowAutoRunerDlg()
{

	if( dwCmdOption & CMDOPTION_SHOWAUTORUNNERDLG )
	{
		TryCenterDlg( hWndAR, TRUE );
		SetForegroundWindow( hWndAR );
		ShowWindow( hWndAR, SW_SHOW );		
	}	
	else
	{
		TryCenterDlg( hWndAR, FALSE );			
		ShowWindow( hWndAR, SW_HIDE );
	}	

}

void TryCenterDlg( HWND hThisWnd, BOOL bYes )
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	
	// trying to center the dialog
	if (GetWindowRect(hThisWnd, &rt1)) {
		GetClientRect(GetParent(hThisWnd), &rt);
		DlgWidth	= rt1.right - rt1.left;
		DlgHeight	= rt1.bottom - rt1.top ;
		NewPosX		= (rt.right - rt.left - DlgWidth)/2;
		NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
		
		// if the box is larger than the physical screen 
		if (NewPosX < 0) NewPosX = 0;
		if (NewPosY < 0) NewPosY = 0;
		if( bYes )
			SetWindowPos(hThisWnd, 0, NewPosX, NewPosY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		else
			SetWindowPos(hThisWnd, 0, 500, 500, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
}

BOOL LoadRegSetting( HWND hDlg )
{
	BOOL bRet = FALSE;
	TCHAR szString[256];         // Temperary string
	HKEY hKey;
	BYTE pValueData[256];
	DWORD dwValueType = 0;
	DWORD dwValueLen = 0;
	DWORD dwValueData = 0;
	
	/// default at [HKEY_LOCAL_MACHINE\Software\Mitac\micAutoRunner]

	if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, MIC_AUTORUNNER_REG_KEY, 0, 0, &hKey ) )
	{		
		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdFilePath", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdFilePath=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdFilePath, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdFilePath=%s"), szCmdFilePath );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdFilePath, (PWSTR)L"\\SDMMC Card\\" );
	    	
			wsprintf (szString, TEXT("Set CmdFilePath=%s"), szCmdFilePath );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    
		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdFileName", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdFileName=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdFileName, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdFileName=%s"), szCmdFileName );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdFileName, (PWSTR)L"micAR.cmd" );
	    	
			wsprintf (szString, TEXT("Set CmdFileName=%s"), szCmdFileName );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    
		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"RspFileName", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load RspFileName=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szRspFileName, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load RspFileName=%s"), szRspFileName );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szRspFileName, (PWSTR)L"micAR.rsp" );
	    	
			wsprintf (szString, TEXT("Set RspFileName=%s"), szRspFileName );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }

		dwValueType = REG_DWORD;
		dwValueLen = sizeof( dwValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdOption", NULL, &dwValueType, (LPBYTE)&dwValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdOption=0x%08x"), dwValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	///wcscpy( (PWSTR)szRspFileName, (PWSTR)pValueData );
	    	dwCmdOption = dwValueData;
			
			wsprintf (szString, TEXT("Load CmdOption=0x%08x"), dwCmdOption );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	///wcscpy( (PWSTR)szRspFileName, (PWSTR)L"micAR.rsp" );
	    	dwCmdOption = 0;
	    	
			wsprintf (szString, TEXT("Set CmdOption=0x%08x"), dwCmdOption );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }


		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdSign", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdSign=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdSign, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdSign=%s"), szCmdSign );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdSign, (PWSTR)L"#" );
	    	
			wsprintf (szString, TEXT("Set CmdSign=%s"), szCmdSign );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdMark", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdMark=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdMark, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdMark=%s"), szCmdMark );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdMark, (PWSTR)L";" );
	    	
			wsprintf (szString, TEXT("Set CmdMark=%s"), szCmdMark );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdSource", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdSource=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdSource, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdSource=%s"), szCmdSource );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdSource, (PWSTR)L"SRC:" );
	    	
			wsprintf (szString, TEXT("Set CmdSource=%s"), szCmdMark );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdTarget", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdTarget=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdTarget, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdTarget=%s"), szCmdTarget );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdTarget, (PWSTR)L"TARG:" );
	    	
			wsprintf (szString, TEXT("Set CmdTarget=%s"), szCmdTarget );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdLine", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdLine=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdLine, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdLine=%s"), szCmdLine );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdLine, (PWSTR)L"CMDL:" );
	    	
			wsprintf (szString, TEXT("Set CmdLine=%s"), szCmdLine );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }


		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdRun", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdRun=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdRun, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdRun=%s"), szCmdRun );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdRun, (PWSTR)L"RUN:" );
	    	
			wsprintf (szString, TEXT("Set CmdRun=%s"), szCmdRun );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdCopy", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdCopy=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdCopy, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdCopy=%s"), szCmdCopy );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdCopy, (PWSTR)L"COPY:" );
	    	
			wsprintf (szString, TEXT("Set CmdCopy=%s"), szCmdCopy );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdMove", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdMove=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdMove, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdMove=%s"), szCmdMove );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdMove, (PWSTR)L"MOVE:" );
	    	
			wsprintf (szString, TEXT("Set CmdMove=%s"), szCmdMove );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdDelete", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdDelete=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdDelete, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdDelete=%s"), szCmdDelete );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdDelete, (PWSTR)L"DEL:" );
	    	
			wsprintf (szString, TEXT("Set CmdDelete=%s"), szCmdDelete );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }

		dwValueType = REG_SZ;
		dwValueLen = sizeof( pValueData );
	    if( ERROR_SUCCESS == RegQueryValueEx( hKey, L"CmdEnd", NULL, &dwValueType, pValueData, &dwValueLen ) )
	    {
			///wsprintf (szString, TEXT("Load CmdEnd=%s"), pValueData );
			///SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			
	    	wcscpy( (PWSTR)szCmdEnd, (PWSTR)pValueData );
			
			wsprintf (szString, TEXT("Load CmdEnd=%s"), szCmdEnd );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	    else
	    {	
	    	wcscpy( (PWSTR)szCmdEnd, (PWSTR)L"END:" );
	    	
			wsprintf (szString, TEXT("Set CmdEnd=%s"), szCmdEnd );
			SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	    }

    	wcscpy( (PWSTR)szCmdShow, (PWSTR)L"SHOW:" );
		wsprintf (szString, TEXT("Set CmdShow=%s"), szCmdShow );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdHide, (PWSTR)L"HIDE:" );
		wsprintf (szString, TEXT("Set CmdHide=%s"), szCmdHide );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdWait, (PWSTR)L"WAIT:" );
		wsprintf (szString, TEXT("Set CmdWait=%s"), szCmdWait );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdKeepCmd, (PWSTR)L"KEEPCMD:" );
		wsprintf (szString, TEXT("Set CmdKeepCmd=%s"), szCmdKeepCmd );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	}
	else
	{		
		///load default
    	wcscpy( (PWSTR)szCmdFilePath, (PWSTR)L"\\SDMMC Card\\" );	    	
		wsprintf (szString, TEXT("Set CmdFilePath=%s"), szCmdFilePath );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdFileName, (PWSTR)L"micAR.cmd" );
		wsprintf (szString, TEXT("Set CmdFileName=%s"), szCmdFileName );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szRspFileName, (PWSTR)L"micAR.rsp" );
		wsprintf (szString, TEXT("Set RspFileName=%s"), szRspFileName );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdSign, (PWSTR)L"#" );
		wsprintf (szString, TEXT("Set CmdSign=%s"), szCmdSign );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdMark, (PWSTR)L";" );
		wsprintf (szString, TEXT("Set CmdMark=%s"), szCmdMark );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdSource, (PWSTR)L"SRC:" );
		wsprintf (szString, TEXT("Set CmdSource=%s"), szCmdSource );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdTarget, (PWSTR)"TARG:" );
		wsprintf (szString, TEXT("Set CmdTarget=%s"), szCmdTarget );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdRun, (PWSTR)L"RUN:" );
		wsprintf (szString, TEXT("Set CmdRun=%s"), szCmdRun );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdCopy, (PWSTR)L"COPY:" );
		wsprintf (szString, TEXT("Set CmdCopy=%s"), szCmdCopy );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdMove, (PWSTR)L"MOVE:" );
		wsprintf (szString, TEXT("Set CmdMove=%s"), szCmdMove );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdDelete, (PWSTR)"DEL:" );
		wsprintf (szString, TEXT("Set CmdDelete=%s"), szCmdDelete );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdEnd, (PWSTR)"END:" );
		wsprintf (szString, TEXT("Set CmdEnd=%s"), szCmdEnd );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);


    	wcscpy( (PWSTR)szCmdShow, (PWSTR)L"SHOW:" );
		wsprintf (szString, TEXT("Set CmdShow=%s"), szCmdShow );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdHide, (PWSTR)L"HIDE:" );
		wsprintf (szString, TEXT("Set CmdHide=%s"), szCmdHide );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdWait, (PWSTR)L"WAIT:" );
		wsprintf (szString, TEXT("Set CmdWait=%s"), szCmdWait );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

    	wcscpy( (PWSTR)szCmdKeepCmd, (PWSTR)L"KEEPCMD:" );
		wsprintf (szString, TEXT("Set CmdKeepCmd=%s"), szCmdKeepCmd );
		SendDlgItemMessage(hDlg, IDC_AUTORUNNER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
		
	}	
	
	return bRet;
}
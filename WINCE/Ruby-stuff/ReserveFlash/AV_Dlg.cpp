// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
///#include <winuserm.h>
#include <streams.h>	///for DirectShow
#include <Objbase.h>	///for DirectShow, CoCreateInstance()
///#include <Uuids.h>	///for DirectShow, IID_xxxxxx, CLSID_xxxxx
#include "resource.h"



#define myHRESULTerrorCheck(x,y)   if( S_OK != x )	myErrorMessageBox(g_AVhwnd, y);

///prototype
DWORD AVDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK AV(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static HRESULT PlayMovie(LPTSTR lpszMovie);
static HRESULT StopMovie(void);
void WINAPI FillListBoxThread(HWND hwnd);
static void myErrorMessageBox(HWND hwnd, DWORD dwErrorCode);


#define WM_GRAPHNOTIFY  WM_APP + 99
static void HandleEvent();
static void CleanUp(void);

///
static HANDLE		   hListThread = NULL;
static HWND            g_AVhwnd;
static IGraphBuilder   *pGraph = NULL;
static IMediaControl   *pMediaControl = NULL;
static IMediaEventEx   *pEvent = NULL;


DWORD AVDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_AVBOX, hWnd, (DLGPROC)AV);
	
	return dwRet;
}

// Message handler for the AV box.
LRESULT CALLBACK AV(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
///	TCHAR szString[64];         // Temperary string

	HWND hwndPortCombo = NULL;      // Handle to the combobox control
	UINT index = 0;
	DWORD dwAddr = 0;
	DWORD dwVal = 0;
	DWORD dwRet = 0;
	DWORD dwSuccess = 0;
	static DWORD DiffA[64];
	static DWORD IDC_CHECK_ARRARY[16];
	static DWORD g_dwCODECValue = 0;
	///DWORD DiffB[64];

	switch (message)
	{
        case WM_GRAPHNOTIFY:
            HandleEvent();
            return TRUE;

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

			g_AVhwnd = hDlg;

			/// start com port with setting
			hListThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FillListBoxThread, (LPVOID)hDlg, 0, NULL );
			if ( !hListThread )
				myErrorMessageBox(hDlg, 119);
			else
				CloseHandle(hListThread);			

			
			return TRUE;
		
		case WM_COMMAND:
			if ((LOWORD(wParam) == IDC_AV_DUMP) )
			{
				HANDLE hDispFrame = NULL;
				hDispFrame = CreateEvent(NULL, FALSE, FALSE, L"named oem display frame test event");
				SetEvent(hDispFrame);
				CloseHandle(hDispFrame);
				myErrorMessageBox(hDlg, 129);
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_AV_PLAY) )
			{
				PlayMovie(TEXT("\\HDD4\\Mr_Big_ToBeWithYou.wmv"));
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_AV_STOP) )
			{
				StopMovie();
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_AV_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_AV_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_AV_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			else
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}							
			break;
	}
    return FALSE;
}

static void myErrorMessageBox(HWND hwnd, DWORD dwErrorCode)
{
	TCHAR szString[12];         // Temperary string
	
	wsprintf( szString, TEXT("%6d"), dwErrorCode);
	MessageBox(hwnd, szString, L"Error", MB_OK);
}	

void WINAPI FillListBoxThread(HWND hwnd)
{
	TCHAR szTstring[64];		// Temperary string
	DWORD dwLoop = 0;
	DWORD dwIndex = 0;
	
	while(1)
	{
		Sleep(100);
		wsprintf (szTstring, TEXT("->%08d"), dwLoop);
		SendDlgItemMessage(hwnd, IDC_AV_LIST1, LB_ADDSTRING, 0, (LPARAM)szTstring);
		///move the screen to the last message		
		dwIndex = SendDlgItemMessage(hwnd, IDC_AV_LIST1, LB_GETCOUNT, 0, 0);							
		SendDlgItemMessage(hwnd, IDC_AV_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);		
		dwLoop++;
	}	
}

static HRESULT StopMovie(void)
{
	HRESULT hr = S_FALSE;
	
	do
	{
		if( pGraph == NULL )
			break;
		
		CleanUp();
		hr = S_OK;
		
	}while(0);
	
	return hr;	
}


static HRESULT PlayMovie(LPTSTR lpszMovie)
{
	HRESULT hr;
///	long evCode; // something to hold a returned event code	

    // Initialize the COM library.
    hr = CoInitialize(NULL);
    myHRESULTerrorCheck(hr, 199)
	
    // Create the filter graph manager and render the file.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&pGraph);
    ///it means ...
    /// use the CLSID_FilterGraph class ID to create an Graph filter Object.
    /// use the IID_IGraphBuilder interface to communicate with the Graph filter object.
    /// the IID_IGraphBuilder physical access pointer will be in pGraph
    myHRESULTerrorCheck(hr, 100)
    if( REGDB_E_CLASSNOTREG == hr )
    	myErrorMessageBox(g_AVhwnd, 1001);
    if( CLASS_E_NOAGGREGATION == hr )
    	myErrorMessageBox(g_AVhwnd, 1002);
    	
    // Filenames on Windows CE start with a \\ instead of a drive letter.
    hr = pGraph->RenderFile(lpszMovie, NULL);
	myHRESULTerrorCheck(hr, 101)

    // Specify the owner window.
    IVideoWindow    *pVidWin = NULL;
    ///query if the object support interface IID_IVideoWindow
    ///the IID_IVideoWindow interface of this Object, physical access pointer will be in pVidWin
    hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
    myHRESULTerrorCheck(hr, 102)
    
    hr = pVidWin->put_Owner((OAHWND)g_AVhwnd);
    myHRESULTerrorCheck(hr, 103)
    
    hr = pVidWin->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS);
    myHRESULTerrorCheck(hr, 104)
    

    // Set the owner window to receive event notices.
    hr = pGraph->QueryInterface(IID_IMediaEventEx, (void **)&pEvent);
    myHRESULTerrorCheck(hr, 105)
    
    hr = pEvent->SetNotifyWindow((OAHWND)g_AVhwnd, WM_GRAPHNOTIFY, 0);
    myHRESULTerrorCheck(hr, 106)

    // Run the graph.
    hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
    myHRESULTerrorCheck(hr, 107)
    
    hr = pMediaControl->Run();
    myHRESULTerrorCheck(hr, 108)

	// block application until video rendering operations finish
///	hr = pEvent->WaitForCompletion(INFINITE, &evCode);   
///    myHRESULTerrorCheck(hr, 109)
    
	return hr;
}

static void CleanUp(void)
{
    IVideoWindow    *pVidWin = NULL;
    pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
    pVidWin->put_Visible(OAFALSE);
    pVidWin->put_Owner(NULL);

    // Stop the graph.
    pMediaControl->Stop();

    long evCode;
    pEvent->WaitForCompletion(INFINITE, &evCode);

    pMediaControl->Release();
    pEvent->Release();
    pGraph->Release();
    PostQuitMessage(0);
}

static void HandleEvent() 
{
    long evCode, param1, param2;
    HRESULT hr;
    while (hr = pEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr))
    { 
        hr = pEvent->FreeEventParams(evCode, param1, param2);
        if ((EC_COMPLETE == evCode) || (EC_USERABORT == evCode))
        { 
            CleanUp();
            break;
        } 
    } 
}


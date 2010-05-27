// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
///#include <winuserm.h>
///#include <commctrl.h>
#include "resource.h"
#include "OSBench_Dlg.h"


///#define SDMMC_PATH_OSBENCH_FILE1	_T("\\SDMMC Card\\osbench_dll.dll")
///#define SDMMC_PATH_OSBENCH_FILE2	_T("\\SDMMC Card\\osbench.exe")
#define SDMMC_PATH_OSBENCH_FILE1	_T("osbench_dll.dll")
#define SDMMC_PATH_OSBENCH_FILE2	_T("osbench.exe")

#define WINDOWS_PATH_OSBENCH_FILE1	_T("\\osbench_dll.dll")
#define WINDOWS_PATH_OSBENCH_FILE2	_T("\\osbench.exe")


#define SDMMC_PATH_ILTIMING_FILE1	_T("iltiming_dll.dll")
#define SDMMC_PATH_ILTIMING_FILE2	_T("iltiming.exe")

#define WINDOWS_PATH_ILTIMING_FILE1	_T("\\iltiming_dll.dll")
#define WINDOWS_PATH_ILTIMING_FILE2	_T("\\iltiming.exe")


static HWND ghDlg = NULL;
static TCHAR szString[64];         	// Temperary string
static BOOL bHDDtestRunning = FALSE;
static HANDLE ghWDTimer = NULL;
static HINSTANCE ghInstance = NULL;


///prototype
DWORD OSBenchDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK OSBench(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



DWORD OSBenchDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	ghInstance = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_OSBENCHBOX, hWnd, (DLGPROC)OSBench);
	
	return dwRet;
}



// Message handler for the OSBench box.
LRESULT CALLBACK OSBench(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	
	HANDLE hCreateThread = NULL;
	///TCHAR szString[64];         	// Temperary string
	HWND hwndBufferCombo = NULL;    // Handle to the combobox control
	UINT index = 0;

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

			ghDlg = hDlg;

			return TRUE;

		case WM_COMMAND:
		
			if ((LOWORD(wParam) == IDC_OSBENCH_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_OSBENCH_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_OSBENCH_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			else			
			if ((LOWORD(wParam) == IDC_OSBENCH_OSBENCH) )
			{
				if( 0 != CopyFile( SDMMC_PATH_OSBENCH_FILE1, WINDOWS_PATH_OSBENCH_FILE1, FALSE))
				{	
					MessageBox(hDlg, L" Dll file not ready !!", L"Error 00003", MB_OK);
					return FALSE;
				}	
				if( 0 != CopyFile( SDMMC_PATH_OSBENCH_FILE2, WINDOWS_PATH_OSBENCH_FILE2, FALSE))
				{	
					MessageBox(hDlg, L" EXE file not ready !!", L"Error 00003", MB_OK);
					return FALSE;
				}	

			    CreateProcess(TEXT("osbench.exe"), TEXT("-list"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
			    CreateProcess(TEXT("osbench.exe"), TEXT("-all -o osbench.log"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);			    
				return TRUE;					
			}				
			else
			if ((LOWORD(wParam) == IDC_OSBENCH_ILTIMING) )
			{
				if( 0 != CopyFile( SDMMC_PATH_ILTIMING_FILE1, WINDOWS_PATH_ILTIMING_FILE1, FALSE))
				{	
					MessageBox(hDlg, L" Dll file not ready !!", L"Error 00003", MB_OK);
					return FALSE;
				}	
				if( 0 != CopyFile( SDMMC_PATH_ILTIMING_FILE2, WINDOWS_PATH_ILTIMING_FILE2, FALSE))
				{	
					MessageBox(hDlg, L" EXE file not ready !!", L"Error 00003", MB_OK);
					return FALSE;
				}	

			    ///CreateProcess(TEXT("iltiming.exe"), TEXT("-list"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
			    ///CreateProcess(TEXT("iltiming.exe"), TEXT("-i1 -all -o iltiming.log"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
				///CreateProcess(TEXT("iltiming.exe"), TEXT("-i2 -all -o iltiming.log"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
				///CreateProcess(TEXT("iltiming.exe"), TEXT("-i3 -all -o iltiming.log"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
				CreateProcess(TEXT("iltiming.exe"), TEXT("-i4 -all -o iltiming.log"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
				///CreateProcess(TEXT("iltiming.exe"), TEXT("-i5 -all -o iltiming.log"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
				return TRUE;					
			}				
			else
			if ((LOWORD(wParam) == IDC_OSBENCH_DISPPERF) )
			{
///				if( 0 != CopyFile( SDMMC_PATH_ILTIMING_FILE1, WINDOWS_PATH_ILTIMING_FILE1, FALSE))
///				{	
///					MessageBox(hDlg, L" Dll file not ready !!", L"Error 00003", MB_OK);
///					return FALSE;
///				}	
///				if( 0 != CopyFile( SDMMC_PATH_ILTIMING_FILE2, WINDOWS_PATH_ILTIMING_FILE2, FALSE))
///				{	
///					MessageBox(hDlg, L" EXE file not ready !!", L"Error 00003", MB_OK);
///					return FALSE;
///				}	

				CreateProcess(TEXT("dispperf.exe"), TEXT("-c"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
				Sleep(5000);
				CreateProcess(TEXT("dispperf.exe"), TEXT("-d"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
				Sleep(5000);
				CreateProcess(TEXT("dispperf.exe"), TEXT("-w"), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
				Sleep(5000);
								
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
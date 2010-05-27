
#include "stdafx.h"
///#include <winuserm.h>
///#include "Thales_defines.h"
///#include <commctrl.h>
#include "resource.h"
///#include "CODEC_Lib.h"
#include "PerformanceCount_Dlg.h"

///prototype
DWORD PerformanceCountDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK PerformanceCount(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



DWORD PerformanceCountDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_PERFCOUNTBOX, hWnd, (DLGPROC)PerformanceCount);
	
	return dwRet;
}



// Message handler for the PerformanceCount box.
LRESULT CALLBACK PerformanceCount(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	TCHAR szString[64];         // Temperary string

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
			if ((LOWORD(wParam) == IDC_PERFCOUNT_CHECK) )
			{
				LARGE_INTEGER PerformanceCheck;
				DWORD dwFLowPart = 0;
				static DWORD dwLastLowPart = 0;
				DWORD dwCount = 1; 

				if( TRUE == QueryPerformanceCounter(&PerformanceCheck) )
				{
					NKDbgPrintfW(L"!!!QueryPerformanceCounter: HighPart=0x%08x LowPart=0x%08x\r\n", PerformanceCheck.HighPart, PerformanceCheck.LowPart );
					dwLastLowPart = PerformanceCheck.LowPart;
				}
				else
				{
					NKDbgPrintfW(L"!!!QueryPerformanceCounter: fail\r\n");
				}
				
				while(dwCount<6)
				{
					wsprintf (szString, TEXT("Check QueryPerformanceCounter in 3 second"));
					SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
					
					Sleep(3000);					
					if( TRUE == QueryPerformanceFrequency(&PerformanceCheck) )
					{
						NKDbgPrintfW(L"!!!QueryPerformanceFrequency: HighPart=0x%08x LowPart=0x%08x\r\n", PerformanceCheck.HighPart, PerformanceCheck.LowPart );
						wsprintf (szString, TEXT("QueryPerformanceFrequency %d"), dwCount);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						wsprintf (szString, TEXT("HighPart=0x%08x LowPart=0x%08x"), PerformanceCheck.HighPart, PerformanceCheck.LowPart);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						wsprintf (szString, TEXT("HighPart=%08d LowPart=%08d"), PerformanceCheck.HighPart, PerformanceCheck.LowPart);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						
						dwFLowPart = PerformanceCheck.LowPart;
					}
					else
					{
						NKDbgPrintfW(L"!!!QueryPerformanceFrequency: fail\r\n");
						wsprintf (szString, TEXT("QueryPerformanceFrequency %d fail!!"), dwCount);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);												
					}
					
					if( TRUE == QueryPerformanceCounter(&PerformanceCheck) )
					{
						NKDbgPrintfW(L"!!!QueryPerformanceCounter: HighPart=0x%08x LowPart=0x%08x\r\n", PerformanceCheck.HighPart, PerformanceCheck.LowPart );
						NKDbgPrintfW(L"!!!QueryPerformanceCounter: =%d sec \r\n", (PerformanceCheck.LowPart-dwLastLowPart)/dwFLowPart );
						wsprintf (szString, TEXT("QueryPerformanceCounter %d"), dwCount);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						wsprintf (szString, TEXT("HighPart=0x%08x LowPart=0x%08x"), PerformanceCheck.HighPart, PerformanceCheck.LowPart);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						wsprintf (szString, TEXT("HighPart=%08d LowPart=%08d"), PerformanceCheck.HighPart, PerformanceCheck.LowPart);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						wsprintf (szString, TEXT("=> %d seconds"), (PerformanceCheck.LowPart-dwLastLowPart)/dwFLowPart);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
						
						dwLastLowPart = PerformanceCheck.LowPart;
					}
					else
					{
						NKDbgPrintfW(L"!!!QueryPerformanceCounter: fail\r\n");
						wsprintf (szString, TEXT("QueryPerformanceCounter %d fail!!"), dwCount);
						SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);						
					}
					dwCount++;
					
					///move the screen to the last message		
					DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_GETCOUNT, 0, 0);							
					SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
					
				}
				return TRUE;					
			}
			if ((LOWORD(wParam) == IDC_PERFCOUNT_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_PERFCOUNT_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}							
			break;
	}
    return FALSE;
}


// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
///#include <winuserm.h>
#include <commctrl.h>
#include "resource.h"
#include "winioctl.h"
#include "Thales_defines.h"
#include "mio_ioctl.h"
#include "CUnit_Dlg.h"
///#include "Tlhelp32.h"

static HWND ghDlg = NULL;
static TCHAR szString[64];         	// Temperary string
static HINSTANCE ghInstance = NULL;
static BOOL g_bWinHide = FALSE;

///prototype
DWORD spCUnitDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK spCUnit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL InfoDriverCUint( HANDLE hThisDrv );
BOOL RunDriverCUint( HANDLE hThisDrv );

DWORD spCUnitDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	ghInstance = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_CUNITBOX, hWnd, (DLGPROC)spCUnit);
	
	return dwRet;
}


// Message handler for the CUnit box.
LRESULT CALLBACK spCUnit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;

	UINT index = 0;
	
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;

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
		
			if ((LOWORD(wParam) == IDC_CUNIT_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_CUNIT_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_CUNIT_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			else			
			if ((LOWORD(wParam) == IDC_CUNIT_RUN) )
			{
				HANDLE hMPM = INVALID_HANDLE_VALUE;		/// span0111
				hMPM = CreateFile( _T("MPM1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not share

				wsprintf(szString, TEXT("MPM CUnit start"));
				SendDlgItemMessage(ghDlg, IDC_CUNIT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
#if 1
				RunDriverCUint( hMPM );
#else
				///info system driver
				if( hMPM && !DeviceIoControl(hMPM, IOCTL_OEM_CUNIT_TEST, NULL, 0, NULL, 0, NULL, NULL) )
				{
	    			///SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("IOCTL_HAL_REBOOT Fail !!")); 
	    			NKDbgPrintfW( L"Dlg:: IOCTL_OEM_CUNIT_TEST Fail !!!\r\n" );
				}
#endif				
				CloseHandle(hMPM);
				return TRUE;
			}				
			else			
			if ((LOWORD(wParam) == IDC_CUNIT_RUN2) )
			{
				HANDLE hLGT = INVALID_HANDLE_VALUE;		/// span0111
				hLGT = CreateFile( _T("LGT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not share

				wsprintf(szString, TEXT("LGT CUnit start"));
				SendDlgItemMessage(ghDlg, IDC_CUNIT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
#if 1
				RunDriverCUint( hLGT );
#else
				///info system driver
				if( hLGT && !DeviceIoControl(hLGT, IOCTL_OEM_CUNIT_TEST, NULL, 0, NULL, 0, NULL, NULL) )
				{
	    			///SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("IOCTL_HAL_REBOOT Fail !!")); 
	    			NKDbgPrintfW( L"Dlg:: IOCTL_OEM_CUNIT_TEST Fail !!!\r\n" );
				}
#endif				
				CloseHandle(hLGT);
				return TRUE;
			}				
			else			
			if ((LOWORD(wParam) == IDC_CUNIT_RUN3) )
			{
				HANDLE hTVB = INVALID_HANDLE_VALUE;		/// span0111
				hTVB = CreateFile( _T("TVB1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not share

				wsprintf(szString, TEXT("TVB CUnit start"));
				SendDlgItemMessage(ghDlg, IDC_CUNIT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				RunDriverCUint( hTVB);

				CloseHandle(hTVB);
				return TRUE;
			}
			else
			if ((LOWORD(wParam) == IDC_CUNIT_RUN4) )
			{
				HANDLE hBAT = INVALID_HANDLE_VALUE;		/// span0111
				hBAT = CreateFile( _T("BAT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not share

				wsprintf(szString, TEXT("BAT CUnit start"));
				SendDlgItemMessage(ghDlg, IDC_CUNIT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				RunDriverCUint( hBAT);

				CloseHandle(hBAT);
				return TRUE;
			}				
			else			
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				///End the windows
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}							
			break;
	}

    return FALSE;
}


void AddToProgressBar(HWND hwnd, int nCnt)
{
///	TCHAR szString_[64];         	// Temperary string
  	//get the control window
  	HWND hwProgressBar = GetDlgItem(hwnd, IDC_CUNIT_PROGRESS1);
  	//set progress bar position
	SendMessage(hwProgressBar, PBM_SETPOS, (WPARAM)(nCnt), 0);
	NKDbgPrintfW( L"Dlg:: AddToProgressBar = %d\r\n", nCnt );
///	wsprintf(szString_, TEXT("%02d%%"), (100-nCnt));
///	SetDlgItemText(hwnd, IDC_CPU_STATIC2, szString_);
	///UpdateWindow(ghDlg);	
}

BOOL InfoDriverCUint( HANDLE hThisDrv, DWORD *pdwPercent)
{
	BOOL bRet = TRUE;
	///DWORD dwPercent = 0;
	
	if( NULL != hThisDrv )
	{
		///bRet = DeviceIoControl(hThisDrv, IOCTL_OEM_CUNIT_TEST, NULL, 0, (PBYTE)pdwPercent, sizeof(DWORD), NULL, NULL);
		bRet = DeviceIoControl(hThisDrv, IOCTL_MIO_UTF_TV, NULL, 0, (PBYTE)pdwPercent, sizeof(DWORD), NULL, NULL);
	}
	else
		bRet = FALSE;
	
	return bRet;
}

BOOL RunDriverCUint( HANDLE hThisDrv )
{
	BOOL bRet = TRUE;
	BOOL fRunTest = TRUE;
	DWORD dwPercent = 0;
	
	if( NULL == hThisDrv || INVALID_HANDLE_VALUE == hThisDrv )
	{
		wsprintf(szString, TEXT("Invalid driver handle!"));
		SendDlgItemMessage(ghDlg, IDC_CUNIT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);					
		return FALSE;
	}	
	
	while( fRunTest )
	{
		if( !InfoDriverCUint( hThisDrv, &dwPercent ) )
		{
			wsprintf(szString, TEXT("CUnit driver fail!"));
			SendDlgItemMessage(ghDlg, IDC_CUNIT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			break;
		}	
		///set progress bar
		AddToProgressBar( ghDlg, dwPercent );
		if( 100 == dwPercent )
			fRunTest = FALSE;
		Sleep(500);
	}
	
	wsprintf(szString, TEXT("CUnit test done"));
	SendDlgItemMessage(ghDlg, IDC_CUNIT_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
	
	return bRet;
}
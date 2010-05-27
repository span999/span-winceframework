
#include "stdafx.h"
///#include <winuserm.h>
///#include "Thales_defines.h"
///#include <commctrl.h>
#include "resource.h"
///#include "CODEC_Lib.h"
#include "BT_Dlg.h"

///prototype
DWORD BTDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK BT(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



DWORD BTDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_BTBOX, hWnd, (DLGPROC)BT);
	
	return dwRet;
}



// Message handler for the PerformanceCount box.
LRESULT CALLBACK BT(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	///TCHAR szString[64];         // Temperary string

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
			if ((LOWORD(wParam) == IDC_BT_CHECK) )
			{
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_BT_PCM_MUTE) )
			{
				waveOutMessage( 0, 500, (DWORD)21, 0 );
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_BT_PCM_UNMUTE) )
			{
				waveOutMessage( 0, 500, (DWORD)22, 0 );
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_BT_CTP_MUTE) )
			{
				waveOutMessage( 0, 500, (DWORD)11, 0 );
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_BT_CTP_UNMUTE) )
			{
				waveOutMessage( 0, 500, (DWORD)12, 0 );
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_BT_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_BT_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_BT_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

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


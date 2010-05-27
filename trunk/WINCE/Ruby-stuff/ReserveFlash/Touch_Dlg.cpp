
#include "stdafx.h"
///#include <winuserm.h>
///#include "Thales_defines.h"
///#include "winioctl.h"
///#include <commctrl.h>
#include "resource.h"
///#include "CODEC_Lib.h"
#include "Touch_Dlg.h"


///prototype
DWORD TouchDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK Touch(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



DWORD TouchDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_TOUCHBOX, hWnd, (DLGPROC)Touch);
	
	return dwRet;
}



// Mesage handler for the Touch box.
LRESULT CALLBACK Touch(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
			///IDC_RADIO_A1
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

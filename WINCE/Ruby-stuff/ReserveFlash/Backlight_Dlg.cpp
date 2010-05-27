
#include "stdafx.h"
///#include <winuserm.h>
#include "Thales_defines.h"
#include "winioctl.h"
#include <commctrl.h>
#include "resource.h"
///#include "CODEC_Lib.h"
#include "Touch_Dlg.h"


///prototype
DWORD BacklightDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK Backlight(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



DWORD BacklightDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_BACKLIGHTBOX, hWnd, (DLGPROC)Backlight);
	
	return dwRet;
}


void AddToProgressBar(HWND hwnd, int nCnt){
  //get the control window
  HWND hwProgressBar = GetDlgItem(hwnd, IDC_BACKLIGHT_PROGRESS);
  //set progress bar position
  if( 0 == nCnt%10 )
  	SendMessage(hwProgressBar, PBM_SETPOS, (WPARAM)(100-nCnt), 0);
}


void BACKLIGHT_SETTING( HWND hDlg , DWORD dwSelect)
{
	TCHAR szString[64];         // Temperary string
  	HANDLE hLGT = INVALID_HANDLE_VALUE;		/// span0111
  	BOOL bDoHWcontrol = FALSE;
  	static DWORD dwBrightness = 50;
  	///DWORD dwWait = 0;
	///DWORD dwLoop = 0;
	DWORD dwInBuf = 0;
	DWORD dwOutBuf = 0;
	DWORD dwOutLan = sizeof(DWORD);

	HANDLE hHandle = NULL;
	
	///disable the screen off timer
	hHandle = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE);		///span0417
	if( NULL == hHandle )
	{
		NKDbgPrintfW(TEXT("ERR:CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE) failed!!!\r\n"));
	}
	else
	if( 0 == SetEvent(hHandle) )
	{
		NKDbgPrintfW(L"ERR:SetEvent() EVENT_OEM_SCREEN_TIMER_DISABLE failed!!! error code =%d.\r\n", GetLastError());	
	}
	CloseHandle(hHandle);		
	///disable the screen off timer
	
	///get backlight device handle 
	hLGT = CreateFile( _T("LGT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare

	if( INVALID_HANDLE_VALUE != hLGT ) {
	}
	else
	{	
		NKDbgPrintfW( L"ERROR !!! CreateFile LGT1 fail.\r\n" );
		///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("ERROR !!! CreateFile LGT1 fail !!!"));
		SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)TEXT("ERROR1 !!!"));
		UpdateWindow(hDlg);
		return;
	}	
	///get backlight device handle 

	///parsing the command
	switch(dwSelect)
	{
		case IDC_BACKLIGHT_DOWN:
			dwBrightness = dwBrightness - 1;
	   		wsprintf (szString, TEXT("%d"), dwBrightness);
   			SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)szString);

			break;
		case IDC_BACKLIGHT_DOWN2:
			dwBrightness = dwBrightness - 5;			
	   		wsprintf (szString, TEXT("%d"), dwBrightness);
   			SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)szString);

			break;
		case IDC_BACKLIGHT_UP:			
			dwBrightness = dwBrightness + 1;			
	   		wsprintf (szString, TEXT("%d"), dwBrightness);
   			SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)szString);
			
			break;
		case IDC_BACKLIGHT_UP2:
			dwBrightness = dwBrightness + 5;			
	   		wsprintf (szString, TEXT("%d"), dwBrightness);
   			SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)szString);
			
			break;
		case IDC_BACKLIGHT_SET:
			bDoHWcontrol = TRUE;
			break;
		default:
			break;			
	}

	if( !bDoHWcontrol )
	{
		///check if the "change and go" checkbox has been checked
		if( BST_CHECKED == SendMessage(GetDlgItem(hDlg, IDC_CHECK_CHANGE_GO), BM_GETCHECK , NULL, NULL) )
			bDoHWcontrol = TRUE;
	}	
	
	///
	if( bDoHWcontrol )
	{
		dwInBuf = dwBrightness;
	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	Sleep(50);
   		DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
   		wsprintf (szString, TEXT("%d/%d"), dwInBuf, dwOutBuf);
   		SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT3), WM_SETTEXT, NULL, (LPARAM)szString);
   		UpdateWindow(hDlg);
	}


}


void BACKLIGHT_AUTO( HWND hDlg )
{
	//TCHAR szString[64];         // Temperary string
	HANDLE hLGT = INVALID_HANDLE_VALUE;		/// span0111
	BOOL fTESTING = TRUE;
	DWORD dwWait = 0;

	HANDLE hHandle = NULL;
	hHandle = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE);		///span0417
	if( NULL == hHandle )
	{
		NKDbgPrintfW(TEXT("ERR:CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE) failed!!!\r\n"));
	}
	else
	if( 0 == SetEvent(hHandle) )
	{
		NKDbgPrintfW(L"ERR:SetEvent() EVENT_OEM_SCREEN_TIMER_DISABLE failed!!! error code =%d.\r\n", GetLastError());	
	}
	CloseHandle(hHandle);		

	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT, TEXT("Auto Backlight Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT), WM_SETTEXT, NULL, (LPARAM)TEXT("Auto Backlight Testing !!!"));
	UpdateWindow(hDlg);

	hLGT = CreateFile( _T("LGT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare

	if( INVALID_HANDLE_VALUE != hLGT ) {
	}
	else
	{	
		NKDbgPrintfW( L"CreateFile LGT1 fail.\r\n" );
		SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT, TEXT("ERROR !!! CreateFile LGT1 fail !!!"));
		UpdateWindow(hDlg);
	}	
	
	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT, TEXT("Backlight glitter Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT), WM_SETTEXT, NULL, (LPARAM)TEXT("Backlight glitter Testing !!!"));
	UpdateWindow(hDlg);
	
	dwWait = 1000;
	fTESTING = TRUE;
	while(fTESTING)
	{	
		DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
		Sleep(dwWait);
		DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_OFF, NULL, 0, NULL, 0, NULL, NULL);
		Sleep(dwWait);
		dwWait = dwWait - 100;
		if( dwWait <= 0)
			fTESTING = FALSE;
		AddToProgressBar(hDlg, dwWait/10);	
	}	

	dwWait = 1000;
	fTESTING = TRUE;
	while(fTESTING)
	{	
		DeviceIoControl(hLGT, IOCTL_OEM_LCD_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
		DeviceIoControl(hLGT, IOCTL_OEM_KEYPAD_BACKLIGHT_OFF, NULL, 0, NULL, 0, NULL, NULL);
		Sleep(dwWait);
		DeviceIoControl(hLGT, IOCTL_OEM_LCD_BACKLIGHT_OFF, NULL, 0, NULL, 0, NULL, NULL);
		DeviceIoControl(hLGT, IOCTL_OEM_KEYPAD_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
		Sleep(dwWait);
		dwWait = dwWait - 100;
		if( dwWait <= 0)
			fTESTING = FALSE;
		AddToProgressBar(hDlg, dwWait/10);	
	}
 		    	
	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	CloseHandle(hLGT);
	
	SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT, TEXT("Backlight glitter done!"));
	

}


void BACKLIGHT_AUTO2( HWND hDlg )
{
	TCHAR szString[64];         // Temperary string
  	HANDLE hLGT = INVALID_HANDLE_VALUE;		/// span0111
  	BOOL fTESTING = TRUE;
  	DWORD dwWait = 0;
	DWORD dwLoop = 0;
	DWORD dwInBuf = 0;
	DWORD dwOutBuf = 0;
	DWORD dwOutLan = sizeof(DWORD);

	HANDLE hHandle = NULL;
	hHandle = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE);		///span0417
	if( NULL == hHandle )
	{
		NKDbgPrintfW(TEXT("ERR:CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SCREEN_TIMER_DISABLE) failed!!!\r\n"));
	}
	else
	if( 0 == SetEvent(hHandle) )
	{
		NKDbgPrintfW(L"ERR:SetEvent() EVENT_OEM_SCREEN_TIMER_DISABLE failed!!! error code =%d.\r\n", GetLastError());	
	}
	CloseHandle(hHandle);		

	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Auto brightness Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)TEXT("Auto brightness Testing !!!"));
	UpdateWindow(hDlg);

	hLGT = CreateFile( _T("LGT1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare

	if( INVALID_HANDLE_VALUE != hLGT ) {
	}
	else
	{	
		NKDbgPrintfW( L"ERROR !!! CreateFile LGT1 fail.\r\n" );
		///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("ERROR !!! CreateFile LGT1 fail !!!"));
		SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)TEXT("ERROR !!! CreateFile LGT1 fail !!!"));
		UpdateWindow(hDlg);
		return;
	}	

	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Brightness Sequent Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)TEXT("Brightness Sequent Testing !!!"));
	NKDbgPrintfW(L"\r\nBrightness Sequent Testing !!!\r\n");
	Sleep(500);	
	dwWait = 100;
	dwLoop = 200;
	fTESTING = TRUE;
	dwInBuf = 100;
	while(fTESTING)
	{	
	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	Sleep(50);
	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	wsprintf (szString, TEXT("Sequent Testing %d - %d"), dwInBuf, dwOutBuf);
	   	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, szString);
	   	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)szString);
	   	UpdateWindow(hDlg);
	   	Sleep(dwWait);
	   	if( dwLoop <= 100)
	   		dwInBuf++;
	   	else
	   		dwInBuf--;		   		
   		dwLoop--;
	   	if( dwLoop <= 0)
	   		fTESTING = FALSE;
	   	NKDbgPrintfW(L".");
	   	AddToProgressBar(hDlg, dwLoop/2);
	}

	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Brightness Random Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)TEXT("Brightness Random Testing !!!"));
	UpdateWindow(hDlg);
	NKDbgPrintfW(L"\r\nBrightness Random Testing !!!\r\n");
	dwWait = 100;
	dwLoop = 200;
	fTESTING = TRUE;
	while(fTESTING)
	{	
		do{
			dwInBuf = (Random() & 0x800000FF);
			Sleep(1);
		}
		while( dwInBuf<0 || dwInBuf>100);

	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	Sleep(50);
	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	wsprintf (szString, TEXT("Random Testing %d - %d"), dwInBuf, dwOutBuf);
	   	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, szString);
	   	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)szString);
	   	UpdateWindow(hDlg);
	   	Sleep(dwWait);
	   	dwLoop--;
	   	if( dwLoop <= 0)
	   		fTESTING = FALSE;
	   	NKDbgPrintfW(L".");	
	   	AddToProgressBar(hDlg, dwLoop/2);
	}

	
	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Brightness Error Testing !!!"));
	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)TEXT("Brightness Error Testing !!!"));
	UpdateWindow(hDlg);
	NKDbgPrintfW(L"\r\nBrightness Error Testing !!!\r\n");
	dwWait = 100;
	dwLoop = 200;
	fTESTING = TRUE;
	while(fTESTING)
	{	
		dwInBuf = (Random() & 0x800000FF);

	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	Sleep(50);
	   	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_GET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);
	   	wsprintf (szString, TEXT("Error Testing %d - %d"), dwInBuf, dwOutBuf);
	   	///SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, szString);
	   	SendMessage(GetDlgItem(hDlg, IDC_STATIC_BACKLIGHT2), WM_SETTEXT, NULL, (LPARAM)szString);
	   	UpdateWindow(hDlg);
	   	Sleep(dwWait);
	   	dwLoop--;
	   	if( dwLoop <= 0)
	   		fTESTING = FALSE;
	   	NKDbgPrintfW(L".");	
	   	AddToProgressBar(hDlg, dwLoop/2);
	}
 		    	
	dwInBuf = 100;
	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_BRIGHTNESS_SET, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, dwOutLan, NULL, NULL);	
	DeviceIoControl(hLGT, IOCTL_OEM_BACKLIGHT_ON, NULL, 0, NULL, 0, NULL, NULL);
	CloseHandle(hLGT);
	
	SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Brightness Test done!"));
	UpdateWindow(hDlg);
	NKDbgPrintfW(L"\r\nBrightness Testing Done!!!\r\n");

}


// Mesage handler for the Backlight box.
LRESULT CALLBACK Backlight(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	HANDLE hCreateThread = NULL;
	
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
			SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT, TEXT("Click it!"));
			SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT2, TEXT("Click it!"));
			SetDlgItemText(hDlg, IDC_STATIC_BACKLIGHT3, TEXT("50"));
			return TRUE;

		case WM_COMMAND:
			///IDC_RADIO_A1
			if ((LOWORD(wParam) == IDC_BACKLIGHT_AUTO) )
			{	
				BACKLIGHT_AUTO( hDlg );
				///hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)BACKLIGHT_AUTO, &hDlg, 0, NULL);
				///CloseHandle(hCreateThread);
				return TRUE;
			}
			if ((LOWORD(wParam) == IDC_BACKLIGHT_AUTO2) )
			{
				BACKLIGHT_AUTO2( hDlg );
				///hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)BACKLIGHT_AUTO2, &hDlg, 0, NULL);
				///CloseHandle(hCreateThread);
				return TRUE;					
			}
		
			if ((LOWORD(wParam) == IDC_BACKLIGHT_DOWN) )
			{
				BACKLIGHT_SETTING( hDlg , (DWORD)(LOWORD(wParam)) );
				return TRUE;					
			}
			if ((LOWORD(wParam) == IDC_BACKLIGHT_DOWN2) )
			{
				BACKLIGHT_SETTING( hDlg , (DWORD)(LOWORD(wParam)) );
				return TRUE;					
			}
			if ((LOWORD(wParam) == IDC_BACKLIGHT_UP) )
			{
				BACKLIGHT_SETTING( hDlg , (DWORD)(LOWORD(wParam)) );
				return TRUE;					
			}
			if ((LOWORD(wParam) == IDC_BACKLIGHT_UP2) )
			{
				BACKLIGHT_SETTING( hDlg , (DWORD)(LOWORD(wParam)) );
				return TRUE;					
			}
			if ((LOWORD(wParam) == IDC_BACKLIGHT_SET) )
			{
				BACKLIGHT_SETTING( hDlg , (DWORD)(LOWORD(wParam)) );
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

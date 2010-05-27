
#include "stdafx.h"
///#include <winuserm.h>
#include "Thales_defines.h"
#include <commctrl.h>
#include "winioctl.h"
#include "resource.h"
#include "Virtual_API.h"
#include "Vdbg_Dlg.h"


HANDLE				hVDBGThread = NULL;
HANDLE				ghVDBGPort = NULL;
BOOL				bVDBGPortOpened = FALSE;
BOOL				bVDBGForcePortClose = FALSE;
BOOL				bVDBGScreenPause = FALSE;
DWORD				dwVPortSelet = 0;
static PORT_IN_COMBO PortInCombo[MAX_PORT_LIST];


///prototype
DWORD VdbgDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK Vdbg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



DWORD VdbgDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_VDBGBOX, hWnd, (DLGPROC)Vdbg);
	
	return dwRet;
}


/*******************************************************************************
		AccessComPort(void)
********************************************************************************/
void WINAPI AccessVDBGPort(HWND hwnd)
{
	HANDLE hTestComm = INVALID_HANDLE_VALUE;
	DWORD dwSetting[9] = {CBR_1200,CBR_2400,CBR_4800,CBR_9600,CBR_19200,CBR_38400,CBR_57600,CBR_115200,921600};
	TCHAR szTstring[VDBG_MESSAGE_MAX_SIZE];		// Temperary string
	TCHAR szTstrtmp[VDBG_MESSAGE_MAX_SIZE];		// Temperary string
	CHAR  szCstring[VDBG_MESSAGE_MAX_SIZE];		// Temperary string
	DWORD dwLoop = 0;
	DWORD dwLoop1 = 0;
	

	NKDbgPrintfW( TEXT("\r\nSPD@AccessVDBGPort():start !! \r\n") );
	///MessageBox(hwnd, L"Com Port Already Open !!", L"Error", MB_OK);
	
	// Open VDBGPort
	if( NULL == ghVDBGPort )
	{	
		hTestComm = CreateFile ( VDBG_DRIVER_HANDLE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    	
		if ( INVALID_HANDLE_VALUE == hTestComm )
		{
			NKDbgPrintfW( TEXT("SPD@AccessVDBGPort():start CreateFile Fail !! \r\n") );
			MessageBox(hwnd, L"Com Port Open failed!!", L"Error", MB_OK);
			return;
		}
		ghVDBGPort = hTestComm;
	}
	else
		hTestComm = ghVDBGPort;	
	
	
	/// set flag port open
	bVDBGPortOpened = TRUE;

	DWORD dwVDBGPortID = 0x12345678;///dwVPortSelet
	DWORD dwVDBGEventHandle = NULL;
	DWORD dwByteRet = 0;
	
	
	dwVDBGPortID = PortInCombo[dwVPortSelet].PortID;
	//init VDBG port
   	DeviceIoControl(hTestComm, IOCTL_OEM_VDBG_PORT_OPEN, &dwVDBGPortID, sizeof(dwVDBGPortID), &dwVDBGEventHandle, sizeof(dwVDBGEventHandle), &dwByteRet, NULL);	


	while( hTestComm != INVALID_HANDLE_VALUE && dwVDBGEventHandle != NULL ) 
	{
		// Wait for an event to occur for the port.
		WaitForSingleObject( (HANDLE)dwVDBGEventHandle, INFINITE );
		dwLoop1 = 0;
		// Read the data from the VDBG port.
		DeviceIoControl(hTestComm, IOCTL_OEM_VDBG_PORT_GETDATA, NULL, 0, szTstrtmp, 128/*sizeof(String[128])*/, &dwByteRet, NULL);
		
		NKDbgPrintfW( L"IOCTL_OEM_VDBG_PORT_GETDATA:[%s] %d char\r\n", szCstring, dwByteRet );
		
		/// show to screen !!
		if( !bVDBGScreenPause )
		{
			///while( dwLoop1<VDBG_MESSAGE_MAX_SIZE )
			///{
			///	szTstrtmp[dwLoop1] = szCstring[dwLoop1];
			///	dwLoop1++;
			///}
			///wsprintf (szString, TEXT("%c"), tcByte);
			///szString[dwLoop++] = '\0';
			wsprintf (szTstring, TEXT("%d:%s"), dwLoop, szTstrtmp);
			
			///SendDlgItemMessage(hwnd, IDC_VDBG_LIST1, LB_ADDSTRING, 0, (LPARAM)(TEXT("ABC")) );
			SendDlgItemMessage(hwnd, IDC_VDBG_LIST1, LB_ADDSTRING, 0, (LPARAM)szTstring);
			///move the screen to the last message		
			DWORD dwIndex = SendDlgItemMessage(hwnd, IDC_VDBG_LIST1, LB_GETCOUNT, 0, 0);							
			SendDlgItemMessage(hwnd, IDC_VDBG_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
		}	


		Sleep(0);
		dwLoop++;

		if ( TRUE == bVDBGForcePortClose )	///force close ComPort
			break;

	}/// end of while()

	CloseHandle(hTestComm);
	bVDBGPortOpened = FALSE;

}


// Mesage handler for the VDBG box.
LRESULT CALLBACK Vdbg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	HWND hwndPortCombo = NULL;      // Handle to the combobox control
///	HWND hwndBaudCombo = NULL;      // Handle to the combobox control
	int index = 0;
///	DWORD dwBAUDList[9] = {1200,2400,4800,9600,19200,38400,57600,115200,921600};
	TCHAR szString[64];				// Temperary string
	TCHAR szString2[64];				// Temperary string
	static DWORD dwPortList[MAX_PORT_LIST];
	///static PORT_IN_COMBO PortInCombo[MAX_PORT_LIST];
	DWORD dwPortIDtemp = 0;
	DWORD dwByteRet;
	
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
			
			if( ghVDBGPort == NULL )
			{
				ghVDBGPort = CreateFile ( VDBG_DRIVER_HANDLE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		    	
				if ( INVALID_HANDLE_VALUE == ghVDBGPort )
				{
					NKDbgPrintfW( TEXT("SPD@Vdbg(): CreateFile Fail !! \r\n") );
				}
				else
				{
					///get the laa port list in VDBG
					DeviceIoControl(ghVDBGPort, IOCTL_OEM_VDBG_PORT_LIST, NULL, 0, &dwPortList, (MAX_PORT_LIST*(sizeof(DWORD))), &dwByteRet, NULL);	
				}					
			}	
			
			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_VDBG_PORT_COMBO);
			// Add items to the combo box.
			for (index = 0; index < MAX_PORT_LIST; index++)
			{
				if( dwPortList[index] != 0x0 )
				{	
					DWORD dwResult;
					///wsprintf (szString, TEXT("COM%d:"), index);
					dwPortIDtemp = dwPortList[index];
					NKDbgPrintfW( L"0x%08x ", dwPortIDtemp );
					DeviceIoControl(ghVDBGPort, IOCTL_OEM_VDBG_PORT_GETIDNAME, &dwPortIDtemp, sizeof(DWORD), &szString, 128, &dwByteRet, NULL);	
					NKDbgPrintfW( L"IOCTL_OEM_VDBG_PORT_GETIDNAME:[%s] %d char\r\n", szString, dwByteRet );
					///dwPortID[index] = dwPortIDtemp;
					wsprintf (szString2, TEXT("%d:%s"), index+1, szString);			
					SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString2);
					dwResult = SendMessage (hwndPortCombo, CB_GETCOUNT, 0, 0);
					PortInCombo[dwResult-1].PortContext = dwPortList[index];
					PortInCombo[dwResult-1].PortID = dwPortIDtemp;
				}	
			}

			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)1, 0);			

			CloseHandle(hwndPortCombo);
			
			return TRUE;

		case WM_COMMAND:
			///IDC_RADIO_A1
			///CBN_SELCHANGE 
			if ((HIWORD(wParam) == CBN_SELCHANGE))
			{
				NKDbgPrintfW( TEXT("SPD@VDBG():CBN_SELCHANGE idComboBox=0x%08x, hwndComboBox=0x%08x\r\n"), wParam, lParam );

				if ((LOWORD(wParam) == IDC_VDBG_PORT_COMBO))
				{
					NKDbgPrintfW( TEXT("SPD@VDBG():IDC_VDBG_PORT_COMBO=0x%08x\r\n"), IDC_VDBG_PORT_COMBO );				
				}
			}
								
			if ((LOWORD(wParam) == IDC_VDBG_START))
			{
				DWORD	dwResult;
				
				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_VDBG_PORT_COMBO, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwVPortSelet = 0;		///
				else
					dwVPortSelet = dwResult;
				
				NKDbgPrintfW( TEXT("SPD@AccessVDBGPort():dwVPortSelet=%d\r\n"), dwVPortSelet );
				
				/// start com port with setting

				hVDBGThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AccessVDBGPort, (LPVOID)hDlg, 0, NULL );
				if ( !hVDBGThread )
					;
				else
					CloseHandle(hVDBGThread);

				Sleep( 300 );				
				
				return TRUE;
			}

			if ((LOWORD(wParam) == IDC_VDBG_STOP))
			{
				if( hVDBGThread != NULL )
				{	
					bVDBGForcePortClose = TRUE;
					Sleep( 300 );
					CloseHandle(hVDBGThread);
					CloseHandle(ghVDBGPort);
					hVDBGThread = NULL;
				}	
				return TRUE;
			}

			if ((LOWORD(wParam) == IDC_VDBG_CLS))
			{
				///pause it ~
				if( !bVDBGScreenPause )
					bVDBGScreenPause = TRUE;
				
				SendDlgItemMessage(hDlg, IDC_VDBG_LIST1, LB_RESETCONTENT, 0, 0);
				///clear screen
				do
				{
					SendDlgItemMessage(hDlg, IDC_VDBG_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_VDBG_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );
				
				///resume it ~
				if( bVDBGScreenPause )
					bVDBGScreenPause = FALSE;
				
				return TRUE;
			}

			if ((LOWORD(wParam) == IDC_VDBG_PAUSE))
			{
				if( bVDBGScreenPause )
					bVDBGScreenPause = FALSE;
				else
					bVDBGScreenPause = TRUE;
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


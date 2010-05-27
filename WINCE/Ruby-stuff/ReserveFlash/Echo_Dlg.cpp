
#include "stdafx.h"
#include <winuserm.h>
#include "Thales_defines.h"
///#include <commctrl.h>
#include "winioctl.h"
#include "resource.h"
///#include "CODEC_Lib.h"
#include "Echo_Dlg.h"

///prototype
DWORD EchoDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK ECHO(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

WNDPROC				gfMsgProc;


DWORD EchoDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_ECHOBOX, hWnd, (DLGPROC)ECHO);
	
	return dwRet;
}


// ***************************************************************************
// Function Name: EditBoxSubProc
// 
// Purpose: Subclassed Message Handler for IDC_MESSAGE Edit Box
//
// Arguments: Standard Window Procedure Arguments
//
// Return Values:
//	The LRESULT for a normal message to the Edit Box, ANDed with ~DLGC_WANTMESSAGE
//	if the message is an up arrow and the cursor is on the first line.
//
// Side Effects:
//	Sends an up arrow command to the dialog box
//	
// Description:
//	Intercepts the message from the parent dialog box, asking if the edit control
//	wants to accept the message, and if the cursor is on the first line and the
//	up arrow has been pressed, it rejects the message.

LRESULT CALLBACK EditBoxSubProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	
	switch(uMsg)
	{
		// this is the message that is sent to the Edit Box asking if the Edit Box
		// wants a particular message.  In the case of an up arrow on the top line,
		// we actually want the Edit Box to deny the message and send it back to
		// the dialog box.  This way, focus will return to the previous control.
		case WM_GETDLGCODE:
			// Get the default response
			lResult = CallWindowProc(gfMsgProc, hwnd, uMsg, wParam, lParam);

			if(lParam)
			{
				LPMSG pmsg = (LPMSG)lParam;

				if((pmsg->message == WM_KEYDOWN || pmsg->message == WM_KEYUP))
				{
					switch (pmsg->wParam)
					{
						case VK_F19:
							NKDbgPrintfW( L"@VK_F19\r\n");
							break;
						case VK_F18:
							NKDbgPrintfW( L"@VK_F18\r\n");
							break;
						case VK_APP1:
							NKDbgPrintfW( L"@VK_APP1\r\n");
							break;
						case VK_APP2:
							NKDbgPrintfW( L"@VK_APP2\r\n");
							break;
						case VK_APP3:
							NKDbgPrintfW( L"@VK_APP3\r\n");
							break;
						case VK_APP4:
							NKDbgPrintfW( L"@VK_APP4\r\n");
							break;
						case VK_APP5:
							NKDbgPrintfW( L"@VK_APP5\r\n");
							break;
						case VK_APP6:
							NKDbgPrintfW( L"@VK_APP6\r\n");
							break;
						case VK_ESCAPE:
							NKDbgPrintfW( L"@VK_ESCAPE\r\n");
							break;
						case VK_VOLUME_MUTE:
							NKDbgPrintfW( L"@VK_VOLUME_MUTE\r\n");
							break;
						case VK_VOLUME_UP:
							NKDbgPrintfW( L"@VK_VOLUME_UP\r\n");
							break;
						case VK_VOLUME_DOWN:
							NKDbgPrintfW( L"@VK_VOLUME_DOWN\r\n");
							break;
						case VK_UP:
							NKDbgPrintfW( L"@VK_UP\r\n");
							break;
						case VK_DOWN:
							NKDbgPrintfW( L"@VK_DOWN\r\n");
							break;							
						case VK_LEFT:
							NKDbgPrintfW( L"@VK_LEFT\r\n");
							break;
						case VK_RIGHT:
							NKDbgPrintfW( L"@VK_RIGHT\r\n");							
							break;
						default:
							break;
					}
				}
			}

			break;

		default:
			// respond to the messages with the default action
			lResult = CallWindowProc(gfMsgProc, hwnd, uMsg, wParam, lParam);
			break;
	}
	
	return lResult;
}



// Message handler for the ECHO box.
LRESULT CALLBACK ECHO(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

			// make sure the dialog box was created
			///if(InitDialog(hDlg))
			{
				HWND hMsgDlg;

				// subclass the edit box
				VERIFY(hMsgDlg = GetDlgItem(hDlg, IDC_ECHO_LIST1));
				if(gfMsgProc = (WNDPROC)GetWindowLong(hMsgDlg, GWL_WNDPROC))
				{
					if(SetWindowLong(hMsgDlg, GWL_WNDPROC, (LONG)EditBoxSubProc))
					{
						// successful
						///break;
					}
				}
			}

			
			return TRUE;
		
		case WM_KEYDOWN:
			wsprintf (szString, TEXT("WM_KEYDOWN %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			switch (wParam)
			{
				case VK_F19:
					wsprintf (szString, TEXT("VK_F19 %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_VOLUME_UP:
					wsprintf (szString, TEXT("VK_VOLUME_UP %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_VOLUME_DOWN:
					wsprintf (szString, TEXT("VK_VOLUME_DOWN %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_UP:
					wsprintf (szString, TEXT("VK_UP %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_DOWN:
					wsprintf (szString, TEXT("VK_DOWN %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;

				default:
					break;
			}
			return TRUE;

		case WM_KEYUP:
			wsprintf (szString, TEXT("WM_KEYUP %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
			switch (wParam)
			{
				case VK_F19:
					wsprintf (szString, TEXT("VK_F19 %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_VOLUME_UP:
					wsprintf (szString, TEXT("VK_VOLUME_UP %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_VOLUME_DOWN:
					wsprintf (szString, TEXT("VK_VOLUME_DOWN %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_UP:
					wsprintf (szString, TEXT("VK_UP %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;
				case VK_DOWN:
					wsprintf (szString, TEXT("VK_DOWN %d"), GetTickCount());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					break;

				default:
					break;
			}
			return TRUE;
					
		///case WM_DEIVCECHANGE:
		case 0x0219:
			wsprintf (szString, TEXT("WM_DEIVCECHANGE %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_SD_REMOVED:
			wsprintf (szString, TEXT("WM_OEM_SD_REMOVED %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_SD_INSERT:
			wsprintf (szString, TEXT("WM_OEM_SD_INSERT %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;

		case WM_OEM_STARTSYNC:
			wsprintf (szString, TEXT("WM_OEM_STARTSYNC %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_OEM_BREAKSYNC:
			wsprintf (szString, TEXT("WM_OEM_BREAKSYNC %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_OEM_USB_CONNECT:
			wsprintf (szString, TEXT("WM_OEM_USB_CONNECT %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_OEM_USB_DISCONNECT:
			wsprintf (szString, TEXT("WM_OEM_USB_DISCONNECT %d"), GetTickCount());
			SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
			return TRUE;
		
		case WM_COMMAND:
			if ((LOWORD(wParam) == IDC_ECHO_IOCTL_PRESET1) )
			{
				HANDLE hEHC = INVALID_HANDLE_VALUE;		/// span0111
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_PRESET_ECHO_1, NULL, 0, NULL, 0, NULL, NULL) )
				{
					wsprintf (szString, TEXT("PRESET_ECHO_1 fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_PRESET_ECHO_1 fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("PRESET_ECHO_1 OK %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_PRESET_ECHO_1 OK. \r\n");
				}
				CloseHandle(hEHC);	
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_IOCTL_PRESET2) )
			{			
				HANDLE hEHC = INVALID_HANDLE_VALUE;		/// span0111
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_PRESET_ECHO_2, NULL, 0, NULL, 0, NULL, NULL) )
				{
					wsprintf (szString, TEXT("PRESET_ECHO_2 fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_PRESET_ECHO_2 fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("PRESET_ECHO_2 OK %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_PRESET_ECHO_2 OK. \r\n");
				}
				CloseHandle(hEHC);	
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_EVENT_PRESET1) )
			{
				HANDLE hECHO = NULL;
				hECHO = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_PRESET_ECHO_1);
				SetEvent(hECHO);
				CloseHandle(hECHO);
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_EVENT_PRESET2) )
			{			
				HANDLE hECHO = NULL;
				hECHO = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_PRESET_ECHO_2);
				SetEvent(hECHO);
				CloseHandle(hECHO);
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_IOCTL_BYPASS) )
			{			
				HANDLE hEHC = INVALID_HANDLE_VALUE;
				BOOL bSet = FALSE;
				BOOL bRet = FALSE;
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_BYPASS, NULL, 0, &bRet, sizeof(bRet), NULL, NULL)
				)
				{
					wsprintf (szString, TEXT("SET_ECHO_BYPASS fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS fail. \r\n");
				}
				else
				{
					bSet = !bRet;
				}	

				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_BYPASS, &bSet, sizeof(bSet), &bRet, sizeof(bRet), NULL, NULL) 
				)
				{
					wsprintf (szString, TEXT("SET_ECHO_BYPASS fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("SET_ECHO_BYPASS OK %d->%d"), bSet, bRet);
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS OK. \r\n");
				}
				CloseHandle(hEHC);
				bSet = !bSet;	
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_IOCTL_POWERDOWN) )
			{
				HANDLE hEHC = INVALID_HANDLE_VALUE;
				BOOL bSet = FALSE;
				BOOL bRet = FALSE;
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare

				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_POWERDOWN, NULL, 0, &bRet, sizeof(bRet), NULL, NULL)
				)
				{
					wsprintf (szString, TEXT("SET_ECHO_BYPASS fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS fail. \r\n");
				}
				else
				{
					bSet = !bRet;
				}	
				
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_POWERDOWN, &bSet, sizeof(bSet), &bRet, sizeof(bRet), NULL, NULL) )
				{
					wsprintf (szString, TEXT("SET_ECHO_POWERDOWN fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_POWERDOWN fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("SET_ECHO_POWERDOWN OK %d->%d"), bSet, bRet);
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_POWERDOWN OK. \r\n");
				}
				CloseHandle(hEHC);
				bSet = !bSet;	
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_EVENT_BYPASS) )
			{
				HANDLE hEHC = INVALID_HANDLE_VALUE;
				BOOL bRet = FALSE;
				
				///setevent
				HANDLE hECHO = NULL;
				hECHO = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SET_ECHO_BYPASS);
				SetEvent(hECHO);
				CloseHandle(hECHO);
				
				Sleep(500);				
				///get current setting
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_BYPASS, NULL, 0, &bRet, sizeof(bRet), NULL, NULL) )
				{
					wsprintf (szString, TEXT("SET_ECHO_BYPASS fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("SET_ECHO_BYPASS OK %d->%d"), bRet, bRet);
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS OK. \r\n");
				}
				CloseHandle(hEHC);
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_EVENT_POWERDOWN) )
			{
				HANDLE hEHC = INVALID_HANDLE_VALUE;
				BOOL bRet = FALSE;
				
				///setevent
				HANDLE hECHO = NULL;
				hECHO = CreateEvent(NULL, FALSE, FALSE, EVENT_OEM_SET_ECHO_POWERDOWN);
				SetEvent(hECHO);
				CloseHandle(hECHO);
				
				Sleep(500);
				///get current setting
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare
				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_POWERDOWN, NULL, 0, &bRet, sizeof(bRet), NULL, NULL) )
				{
					wsprintf (szString, TEXT("SET_ECHO_POWERDOWN fail %d"), GetLastError());
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_POWERDOWN fail. \r\n");
				}
				else
				{	
					wsprintf (szString, TEXT("SET_ECHO_POWERDOWN OK %d->%d"), bRet, bRet);
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_POWERDOWN OK. \r\n");
				}
				CloseHandle(hEHC);
				
				return TRUE;					
			}							
			if ((LOWORD(wParam) == IDC_ECHO_CHECK) )
			{			
				HANDLE hEHC = INVALID_HANDLE_VALUE;
				BOOL bRet = FALSE;
				BOOL bRet1 = FALSE;
				
				///get current setting
				hEHC = CreateFile( _T("EHC1:"), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);	///not ahare

				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_POWERDOWN, NULL, 0, &bRet, sizeof(bRet), NULL, NULL) )
				{
					wsprintf (szString, TEXT("Get PwrDn fail %d"), GetLastError());
					SetDlgItemText(hDlg, IDC_ECHO_EDIT1, szString);			
				}
				else
				{	
///					wsprintf (szString, TEXT("SET_ECHO_POWERDOWN OK %d->%d"), bRet, bRet);
///					SendDlgItemMessage(hDlg, IDC_ECHO_EDIT1, LB_ADDSTRING, 0, (LPARAM)szString);
///					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_POWERDOWN OK. \r\n");
				}

				if( 
					(hEHC == INVALID_HANDLE_VALUE) || 
					!DeviceIoControl(hEHC, IOCTL_OEM_SET_ECHO_BYPASS, NULL, 0, &bRet1, sizeof(bRet1), NULL, NULL) )
				{
					wsprintf (szString, TEXT("Get ByPas fail %d"), GetLastError());
					SetDlgItemText(hDlg, IDC_ECHO_EDIT1, szString);
					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS fail. \r\n");
				}
				else
				{	
///					wsprintf (szString, TEXT("SET_ECHO_BYPASS OK %d->%d"), bRet1, bRet1);
///					SendDlgItemMessage(hDlg, IDC_ECHO_EDIT1, LB_ADDSTRING, 0, (LPARAM)szString);
///					NKDbgPrintfW(L"IOCTL_OEM_SET_ECHO_BYPASS OK. \r\n");
				}

				wsprintf (szString, TEXT("PrD=%d ByP=%d"), bRet, bRet1);
				SetDlgItemText(hDlg, IDC_ECHO_EDIT1, szString);

				CloseHandle(hEHC);
								
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_ECHO_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_ECHO_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

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

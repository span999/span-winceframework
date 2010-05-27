
#include "stdafx.h"
///#include <winuserm.h>
#include "Thales_defines.h"
///#include <commctrl.h>
#include "winioctl.h"
#include "resource.h"
///#include "CODEC_Lib.h"
#include "HWVersion_Dlg.h"


///prototype
DWORD HWVersionDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK HWVersion(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
WCHAR* spCheckBattery(void);


DWORD HWVersionDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_HWVERSIONBOX, hWnd, (DLGPROC)HWVersion);
	
	return dwRet;
}


//
//
//
//
//
//
WCHAR* spCheckBattery(void)
{
	WCHAR *szBattery = L"??????????????\0\0";	///30
	DWORD dwStatus, dwType, dwSize;
	DWORD dwError;
	HKEY hKey;
	
	///Get the registry for the battery drvier.
	///SPMPMINFO( L"!!SPD@spCheckBattery():change the Battery driver~~\r\n" );	 
	dwError = RegOpenKeyEx( HKEY_LOCAL_MACHINE, TEXT("Drivers\\BuiltIn\\Battery"), 0, 0, &hKey);
	if((ERROR_SUCCESS == dwError) && hKey)
	{
       	dwSize = 30;
       	dwStatus = RegQueryValueEx(hKey, TEXT("Dll"), NULL, &dwType, (LPBYTE)szBattery, &dwSize);
       	if(dwStatus == ERROR_SUCCESS && dwType == REG_SZ) {
   	        NKDbgPrintfW( L"SPD@spCheckBattery() : %s.\r\n", szBattery);
       	}
       	else
       	{
       		///dwHWver2A = 0x00000036;
   	        NKDbgPrintfW( L"SPD@spCheckBattery() : Fail !!.\r\n");
       	}
		
       	// release resources
       	RegCloseKey(hKey);  					 
	}
	else
		NKDbgPrintfW( L"!!SPD@spCheckBattery:RegOpenKeyEx failed Error=%08X\r\n", GetLastError() );
	
	return szBattery; 
}


// Mesage handler for the HW Version box.
LRESULT CALLBACK HWVersion(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	int index = 0;
	TCHAR szString[64];         // Temperary string
	HWND hwndCombo = NULL;      // Handle to the combobox control
	DWORD hwVersion[4];
	DWORD dwRet = 0;
	int iItemNumber = 0;

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
			
			SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("Click Read button")); 
			SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, TEXT("Click Read button")); 
			
			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_HWVERSION_COMBO);
			// Add items to the combo box.
			for (index = 1; index < 10; index++)
			{
				wsprintf (szString, TEXT("HW Version [%d](0x%x)"), index, index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			for (index = 65; index < 91; index++)
			{
				wsprintf (szString, TEXT("HW Version [%c](0x%x)"), index, index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage(hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO1);
			// Add items to the combo box.
			wsprintf (szString, TEXT("0700-Ruby"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("0702-MRM860"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("0703-Pluto1"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("0704-Pluto2"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			
			
			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO2);
			// Add items to the combo box.
			wsprintf (szString, TEXT("00-Tulsa"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("20-San Dimas"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("30-Thailand"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("40-China"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("50-Taiwan"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			wsprintf (szString, TEXT("60-Nantes"));
			SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)3, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO3);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO4);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO5);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO6);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO7);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			

			///set COMBO list
			hwndCombo = GetDlgItem(hDlg, IDC_SERIALNUMBER_COMBO8);
			// Add items to the combo box.
			for (index = 0; index < 10; index++)
			{
				wsprintf (szString, TEXT("%d"), index);
				SendMessage (hwndCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}

			// Select the first item as default.
			SendMessage (hwndCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndCombo);			


			SetDlgItemText(hDlg, IDC_CURRDRIVER_EDIT, spCheckBattery() );

			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_HWVERSION_READ )
			{
				if( KernelIoControl( IOCTL_HAL_READ_HW_VERSION, NULL, 0, (LPVOID)hwVersion, 16, (LPDWORD)&dwRet ) )
				{
					wsprintf (szString, TEXT("HW Version [%c](0x%x)"), hwVersion[0], hwVersion[0]);
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, szString); 
				}
				else
				{
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("Get version Fail !!")); 
				}						
				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_HWVERSION_WRITE )
			{
				// Get the selected item.
				hwndCombo = GetDlgItem(hDlg, IDC_HWVERSION_COMBO);
				if( hwndCombo != NULL )
					iItemNumber = SendMessage (hwndCombo, CB_GETCURSEL, (WPARAM)0, 0);
				else
				{		
					wsprintf (szString, TEXT("Error !!"));
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, szString); 
					return FALSE;
				}	
				
				if( iItemNumber < 0 )
				{
					wsprintf (szString, TEXT("Error !!!"));
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, szString); 
					return FALSE;
				}
				else
				if( iItemNumber < 9 )
				{
					hwVersion[0] = iItemNumber+0x31;
				}			
				else
				if( iItemNumber < 36 )
				{
					hwVersion[0] = (iItemNumber-9)+0x41;
				}
				else			
				{
					wsprintf (szString, TEXT("Error !!!!"));
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, szString); 
					return FALSE;
				}
				
				if(	KernelIoControl( IOCTL_HAL_WRITE_HW_VERSION, (LPVOID)hwVersion, 16, NULL, 0, (LPDWORD)&dwRet ) )
				{
					wsprintf (szString, TEXT("Write [%c](0x%x)"), hwVersion[0], hwVersion[0]);
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, szString); 
				}
				else
				{
					SetDlgItemText(hDlg, IDC_HWVERSION_EDIT, TEXT("Write version Fail !!")); 
				}						
				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_SERIALNUMBER_READ )
			{
				BYTE SerialNumber[16];
				if( KernelIoControl( IOCTL_HAL_READ_SERIAL_NO, NULL, 0, (LPVOID)SerialNumber, 16, (LPDWORD)&dwRet ) )
				{
					if( 
						( (SerialNumber[0] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[1] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[2] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[3] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[4] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[5] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[6] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[7] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[8] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[9] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[10] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[11] & 0xF0 ) != 0x30 ) ||
						( (SerialNumber[12] & 0xF0 ) != 0x30 )
					)	
					{
						NKDbgPrintfW( L"SPD@HWVersion() 000.\r\n" );
						wsprintf (szString, TEXT("invalid Serial number"));
						SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, szString);  
					}		
					else
					{																																												
						wsprintf (szString, TEXT("%d%d%d%d%d%d%d%d%d%d%d%d%d"), 
							(SerialNumber[0]&0x0F), (SerialNumber[1]&0x0F),
							(SerialNumber[2]&0x0F), (SerialNumber[3]&0x0F),
							(SerialNumber[4]&0x0F), (SerialNumber[5]&0x0F),
							(SerialNumber[6]&0x0F), (SerialNumber[7]&0x0F),
							(SerialNumber[8]&0x0F), (SerialNumber[9]&0x0F),
							(SerialNumber[10]&0x0F), (SerialNumber[11]&0x0F),
							(SerialNumber[12]&0x0F)
							);
						SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, szString); 
					}	
				}
				else
				{
					SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, TEXT("Get serial number Fail !!")); 
				}						
				return TRUE;
			}
			else
			if ( LOWORD(wParam) == IDC_SERIALNUMBER_WRITE )
			{
				BYTE SerialNumber[16];
				DWORD dwResult = LB_ERR;
				int index;
				
				for( index = 0; index<16; index++ )	///clear it
					SerialNumber[index] = 0;
				
				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[0] = 0x30;
					SerialNumber[1] = 0x37;
					SerialNumber[2] = 0x30;
					SerialNumber[3] = 0x30;															
				}	
				else
				{	
					SerialNumber[0] = 0x30;
					SerialNumber[1] = 0x37;
					SerialNumber[2] = 0x30;
					if( dwResult == 0 )		
						SerialNumber[3] = 0x30;
					else
					if( dwResult == 1 )
						SerialNumber[3] = 0x32;
					else
					if( dwResult == 2 )
						SerialNumber[3] = 0x33;						
					else	
					if( dwResult == 3 )
						SerialNumber[3] = 0x34;						
					else
						SerialNumber[3] = 0x30;
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[4] = 0x34;
					SerialNumber[5] = 0x30;
				}	
				else
				{	
					if( dwResult == 0 )
						SerialNumber[4] = 0x30;
					else
					if( dwResult == 1 )
						SerialNumber[4] = 0x32;
					else
					if( dwResult == 2 )
						SerialNumber[4] = 0x33;
					else
					if( dwResult == 3 )
						SerialNumber[4] = 0x34;
					else
					if( dwResult == 4 )
						SerialNumber[4] = 0x35;
					else
					if( dwResult == 5 )
						SerialNumber[4] = 0x36;
					else						
						SerialNumber[4] = 0x34;
					SerialNumber[5] = 0x30;						
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO3, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[6] = 0x30;
				}	
				else
				{
					SerialNumber[6] = (0x30|(((BYTE)dwResult)&0x0F));
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO4, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[7] = 0x30;
				}	
				else
				{
					SerialNumber[7] = (0x30|(((BYTE)dwResult)&0x0F));
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO5, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[8] = 0x30;
				}	
				else
				{
					SerialNumber[8] = (0x30|(((BYTE)dwResult)&0x0F));
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO6, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[9] = 0x30;
				}	
				else
				{
					SerialNumber[9] = (0x30|(((BYTE)dwResult)&0x0F));
				}	
				
				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO7, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[10] = 0x30;
				}	
				else
				{
					SerialNumber[10] = (0x30|(((BYTE)dwResult)&0x0F));
				}	

				/// get port setting, // get the number of listbox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_SERIALNUMBER_COMBO8, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
				{	
					///get error, set as default
					SerialNumber[11] = 0x30;
				}	
				else
				{
					SerialNumber[11] = (0x30|(((BYTE)dwResult)&0x0F));
				}	

				SerialNumber[12] = 0x30 | (10-(((((SerialNumber[1]&0x0F)+(SerialNumber[3]&0x0F)+(SerialNumber[5]&0x0F)+\
									(SerialNumber[7]&0x0F)+(SerialNumber[9]&0x0F)+(SerialNumber[11]&0x0F))*3)+\
									((SerialNumber[0]&0x0F)+(SerialNumber[2]&0x0F)+(SerialNumber[4]&0x0F)+\
									(SerialNumber[6]&0x0F)+(SerialNumber[8]&0x0F)+(SerialNumber[10]&0x0F)))&0x0F));
				
				if(	KernelIoControl( IOCTL_HAL_WRITE_SERIAL_NO, (LPVOID)SerialNumber, 16, NULL, 0, (LPDWORD)&dwRet ) )
				{
					wsprintf (szString, TEXT("Write done!!!"));
					SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, szString); 
				}
				else
				{
					SetDlgItemText(hDlg, IDC_SERIALNUMBER_EDIT, TEXT("Write Fail !!")); 
				}						
				return TRUE;
			}

			break;
	}
    return FALSE;
}

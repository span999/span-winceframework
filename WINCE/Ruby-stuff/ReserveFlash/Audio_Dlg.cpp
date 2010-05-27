

#include "stdafx.h"
///#include <winuserm.h>
///#include "Thales_defines.h"
#include "resource.h"
#include "CODEC_Lib.h"
#include "Audio_Dlg.h"


#define SDMMC_PATH_AUDIO_REG_LOG_FILE		_T("\\SDMMC Card\\CodecReg.txt")

///prototype
DWORD AudioDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK AUDIO(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



DWORD AudioDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_AUDIOBOX, hWnd, (DLGPROC)AUDIO);
	
	return dwRet;
}

// Message handler for the Audio box.
LRESULT CALLBACK AUDIO(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	TCHAR szString[64];         // Temperary string


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

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_ADDR_COMBO1);
			// Add items to the combo box.
			for (index = 0; index < 8; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_ADDR_COMBO2);
			// Add items to the combo box.
			for (index = 0; index < 16; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_VALUE_COMBO1);
			// Add items to the combo box.
			for (index = 0; index < 16; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_VALUE_COMBO2);
			// Add items to the combo box.
			for (index = 0; index < 16; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_VALUE_COMBO3);
			// Add items to the combo box.
			for (index = 0; index < 16; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_AUDIO_VALUE_COMBO4);
			// Add items to the combo box.
			for (index = 0; index < 16; index++)
			{
				wsprintf(szString, TEXT("%X"), index);			
				SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			}
			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			IDC_CHECK_ARRARY[0] = IDC_AUDIO_CHECK0;
			IDC_CHECK_ARRARY[1] = IDC_AUDIO_CHECK1;
			IDC_CHECK_ARRARY[2] = IDC_AUDIO_CHECK2;
			IDC_CHECK_ARRARY[3] = IDC_AUDIO_CHECK3;
			IDC_CHECK_ARRARY[4] = IDC_AUDIO_CHECK4;
			IDC_CHECK_ARRARY[5] = IDC_AUDIO_CHECK5;
			IDC_CHECK_ARRARY[6] = IDC_AUDIO_CHECK6;
			IDC_CHECK_ARRARY[7] = IDC_AUDIO_CHECK7;
			IDC_CHECK_ARRARY[8] = IDC_AUDIO_CHECK8;
			IDC_CHECK_ARRARY[9] = IDC_AUDIO_CHECK9;
			IDC_CHECK_ARRARY[10] = IDC_AUDIO_CHECK10;
			IDC_CHECK_ARRARY[11] = IDC_AUDIO_CHECK11;
			IDC_CHECK_ARRARY[12] = IDC_AUDIO_CHECK12;
			IDC_CHECK_ARRARY[13] = IDC_AUDIO_CHECK13;
			IDC_CHECK_ARRARY[14] = IDC_AUDIO_CHECK14;
			IDC_CHECK_ARRARY[15] = IDC_AUDIO_CHECK15;

			return TRUE;
		
		case WM_COMMAND:
			if ((HIWORD(wParam) == CBN_SELCHANGE))
			{
				///if ((LOWORD(wParam) == IDC_VDBG_PORT_COMBO))
				///g_dwCODECValue
				DWORD	dwValue1000;
				DWORD	dwValue100;
				DWORD	dwValue10;
				DWORD	dwValue1;				
				DWORD 	dwResult;
				DWORD	dwLoop;
				
				/// get Value in combo list
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue1000 = 0;		///set default
				else
					dwValue1000 = dwResult;
				
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue100 = 0;		///set default
				else
					dwValue100 = dwResult;

				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO3, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue10 = 0;		///set default
				else
					dwValue10 = dwResult;

				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO4, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue1 = 0;		///set default
				else
					dwValue1 = dwResult;
				
				g_dwCODECValue = (dwValue1000*0x1000)+(dwValue100*0x100)+(dwValue10*0x10)+(dwValue1);
					
				///set check box with bit setting
				for( dwLoop = 0; dwLoop < 16; dwLoop++)
				{
					if( g_dwCODECValue & (0x0001<<dwLoop) )
						SendDlgItemMessage(hDlg, IDC_CHECK_ARRARY[dwLoop], BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0 );
					else
						SendDlgItemMessage(hDlg, IDC_CHECK_ARRARY[dwLoop], BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0 );
				}		
				
				return TRUE;	
			}
			
			if ((LOWORD(wParam) == IDC_AUDIO_DUMP) )
			{
				DWORD dwCount = 0;
				BOOL bWrite2file = FALSE;
				HANDLE	hW2file = INVALID_HANDLE_VALUE;
				BOOL bRet = FALSE;
				DWORD dwByte2write = 0;
				DWORD dwWritten = 0;
				
				///check if the "write to file" checkbox has been checked
				///if( BST_CHECKED == SendMessage(GetDlgItem(hDlg, IDC_AUDIO_W2FILE), BM_GETCHECK , NULL, NULL) )
				if(	BST_CHECKED == SendDlgItemMessage(hDlg, IDC_AUDIO_W2FILE, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
					bWrite2file = TRUE;
				else
					NKDbgPrintfW(L"No write to file!!!! \r\n" );	
					
				if( bWrite2file )
				{
					hW2file = CreateFile( SDMMC_PATH_AUDIO_REG_LOG_FILE, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					if( hW2file == INVALID_HANDLE_VALUE )
					{
						bWrite2file = FALSE;
						wsprintf (szString, TEXT("Open Log file fail !!") );
						SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);				
						NKDbgPrintfW(L"Open Log file fail !!!!!! \r\n" );
					}
					else
					{
						if (0xFFFFFFFF == SetFilePointer (hW2file, 0, NULL, FILE_END)) 
						{
							NKDbgPrintfW(L"SetFilePointer fail !!!\r\n" );
				        }						
						NKDbgPrintfW(L"---Audio Reg Dump start---\r\n" );
						wsprintf (szString, TEXT("---Audio Reg Dump start---\r\n") );
						dwByte2write = 2*wcslen(szString);
						///dwByte2write = _tcslen(szString);
						///dwByte2write = strlen(szString);
						bRet = WriteFile(hW2file, szString, dwByte2write, &dwWritten, NULL);
						
					}	
				}
						
				while( dwCount < WM_REGISTER_MAX_ADDRESS )
				{
					dwAddr = dwCount;
					spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
										
					wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s\r\n"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
					SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					dwByte2write = 2*wcslen(szString);
					///dwByte2write = _tcslen(szString);
					///dwByte2write = strlen(szString);
					bRet = WriteFile(hW2file, szString, dwByte2write, &dwWritten, NULL);
					
					if( dwAddr == 0x7E )
					{
						if( dwRet == 0x4C13 )
						{	
							wsprintf (szString, TEXT("WM_9713L"));
							SetDlgItemText(hDlg, IDC_AUDIO_EDIT1, szString);
						}	
						else
						if( dwRet == 0x4C12 )
						{	
							wsprintf (szString, TEXT("WM_9712L"));
							SetDlgItemText(hDlg, IDC_AUDIO_EDIT1, szString);
						}	
						else
						if( dwRet == 0x4C05 )
						{	
							wsprintf (szString, TEXT("WM_9705"));
							SetDlgItemText(hDlg, IDC_AUDIO_EDIT1, szString);
						}	
						else
						{
							wsprintf (szString, TEXT("WM_xxxx"));
							SetDlgItemText(hDlg, IDC_AUDIO_EDIT1, szString);					
						}	
					}
					if( (dwAddr == 0x7C) && ( dwRet != 0x574D ))
					{		
						wsprintf (szString, TEXT("UNKNOW CODEC"));
						SetDlgItemText(hDlg, IDC_AUDIO_EDIT1, szString);
					}						
					dwCount = dwCount+2; 
				}
				
				CloseHandle(hW2file);
				
				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}
			
			if ((LOWORD(wParam) == IDC_AUDIO_READ) )
			{
				DWORD	dwResult;
				DWORD	dwAddr10;
				DWORD	dwAddr1;
				DWORD	dwLoop;
				
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_ADDR_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwAddr10 = 0;		///error! set default
				else
					dwAddr10 = dwResult;

				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_ADDR_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwAddr1 = 0;		///error! set default
				else
					dwAddr1 = dwResult;

				/// access CODEC
				dwAddr = (dwAddr10*0x10)+dwAddr1;
				spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
										
				wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///set check box with bit setting
				for( dwLoop = 0; dwLoop < 16; dwLoop++)
				{
					if( dwRet & (0x0001<<dwLoop) )
						SendDlgItemMessage(hDlg, IDC_CHECK_ARRARY[dwLoop], BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0 );
					else
						SendDlgItemMessage(hDlg, IDC_CHECK_ARRARY[dwLoop], BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0 );	
				}		
					
				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
								
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_AUDIO_WRITE) )
			{
				DWORD	dwResult;
				DWORD	dwAddr10;
				DWORD	dwAddr1;
#if 0
				DWORD	dwValue1000;
				DWORD	dwValue100;
				DWORD	dwValue10;
				DWORD	dwValue1;
#endif
				DWORD	dwLoop;
				
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_ADDR_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwAddr10 = 0;		///error! set default
				else
					dwAddr10 = dwResult;
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_ADDR_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwAddr1 = 0;		///error! set default
				else
					dwAddr1 = dwResult;
#if 0
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue1000 = 0;		///error! set default
				else
					dwValue1000 = dwResult;
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue100 = 0;		///error! set default
				else
					dwValue100 = dwResult;
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO3, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue10 = 0;		///error! set default
				else
					dwValue10 = dwResult;
				/// get address setting, // get the number of Combobox entries
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_AUDIO_VALUE_COMBO4, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwValue1 = 0;		///error! set default
				else
					dwValue1 = dwResult;
					
				g_dwCODECValue = (dwValue1000*0x1000)+(dwValue100*0x100)+(dwValue10*0x10)+(dwValue1);	
#else
				///get check box with bit setting
				for( dwLoop = 0; dwLoop < 16; dwLoop++)
				{
					if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_CHECK_ARRARY[dwLoop], BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
						g_dwCODECValue |= (0x0001<<dwLoop);	///set bit
					else
						g_dwCODECValue &= ~(0x0001<<dwLoop);	///clear bit
				}		

#endif

				/// access CODEC
				dwAddr = (dwAddr10*0x10)+dwAddr1;
				dwVal = g_dwCODECValue;
				
				///write to CODEC
				spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("W:0x%02X-#:0x%04X %s"), dwAddr, dwVal, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				
				///read from CODEC
				spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}				
			if ((LOWORD(wParam) == IDC_AUDIO_DIFF_A) )
			{
				DWORD dwCount = 0;
				DWORD dwIndex = 0;
						
				while( dwCount < WM_REGISTER_MAX_ADDRESS )
				{
					dwAddr = dwCount;
					spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
										
					///wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
					///SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					
					DiffA[dwIndex] = dwRet;
					dwIndex++;
					dwCount = dwCount+2; 
				}
				
				///move the screen to the last message		
				dwIndex = SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}				

			if ((LOWORD(wParam) == IDC_AUDIO_DIFF_B) )
			{
				DWORD dwCount = 0;
				DWORD dwIndex = 0;
				BOOL bWrite2file = FALSE;
				HANDLE	hW2file = INVALID_HANDLE_VALUE;
				BOOL bRet = FALSE;
				DWORD dwByte2write = 0;
				DWORD dwWritten = 0;
				
				///check if the "write to file" checkbox has been checked
				///if( BST_CHECKED == SendMessage(GetDlgItem(hDlg, IDC_AUDIO_W2FILE), BM_GETCHECK , NULL, NULL) )
				if(	BST_CHECKED == SendDlgItemMessage(hDlg, IDC_AUDIO_W2FILE, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
					bWrite2file = TRUE;
				else
					NKDbgPrintfW(L"No write to file!!!! \r\n" );	
					
				if( bWrite2file )
				{
					hW2file = CreateFile( SDMMC_PATH_AUDIO_REG_LOG_FILE, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					if( hW2file == INVALID_HANDLE_VALUE )
					{
						bWrite2file = FALSE;
						wsprintf (szString, TEXT("Open Log file fail !!") );
						SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);				
						NKDbgPrintfW(L"Open Log file fail !!!!!! \r\n" );
					}
					else
					{
						if (0xFFFFFFFF == SetFilePointer (hW2file, 0, NULL, FILE_END)) 
						{
							NKDbgPrintfW(L"SetFilePointer fail !!!\r\n" );
				        }						
						NKDbgPrintfW(L"---Audio Reg Diff start---\r\n" );
						wsprintf (szString, TEXT("---Audio Reg Diff start---\r\n") );
						dwByte2write = 2*wcslen(szString);
						///dwByte2write = _tcslen(szString);
						///dwByte2write = strlen(szString);
						bRet = WriteFile(hW2file, szString, dwByte2write, &dwWritten, NULL);
						
					}	
				}
				
						
				while( dwCount < WM_REGISTER_MAX_ADDRESS )
				{
					dwAddr = dwCount;
					spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
										
					///wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
					///SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					
					if( DiffA[dwIndex] != dwRet )
					{
						wsprintf (szString, TEXT("R:0x%02X-#:0x%04X->0x%04X %s\r\n"), dwAddr, DiffA[dwIndex], dwRet, (dwSuccess==0)?L"Change":L"Fail" );
						SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

						dwByte2write = 2*wcslen(szString);
						///dwByte2write = _tcslen(szString);
						///dwByte2write = strlen(szString);
						bRet = WriteFile(hW2file, szString, dwByte2write, &dwWritten, NULL);		
					}	
					dwIndex++;
					dwCount = dwCount+2; 
				}
				
				CloseHandle(hW2file);
				
				///move the screen to the last message		
				dwIndex = SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}				
	
			if ((LOWORD(wParam) == IDC_AUDIO_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_AUDIO_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

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

// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
///#include <winuserm.h>
#include <streams.h>	///for DirectShow
#include <Objbase.h>	///for DirectShow, CoCreateInstance()
///#include <Uuids.h>	///for DirectShow, IID_xxxxxx, CLSID_xxxxx
#include "resource.h"
#include "CODEC_Lib.h"

///#define ORGTEST

#define myHRESULTerrorCheck(x,y)   if( S_OK != x )	myErrorMessageBox(g_EQhwnd, y);

///prototype
DWORD EQDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK EQ(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT PlayMovie(LPTSTR lpszMovie);
void myErrorMessageBox(HWND hwnd, DWORD dwErrorCode);



#ifndef ORGTEST
#define WM_GRAPHNOTIFY  WM_APP + 99
static void HandleEvent();
static void CleanUp(void);
#endif

///
HWND            g_EQhwnd;


DWORD EQDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_EQBOX, hWnd, (DLGPROC)EQ);
	
	return dwRet;
}

// Message handler for the EQ box.
LRESULT CALLBACK EQ(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
#ifndef ORGTEST		
        case WM_GRAPHNOTIFY:
            HandleEvent();
            break;
#endif
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

			g_EQhwnd = hDlg;
			
			/// get the Combo handle
			hwndPortCombo = GetDlgItem(hDlg, IDC_EQ_COMBO1);
			// Add items to the combo box.
			wsprintf(szString, TEXT("00-Default/Flat"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("01-Rock"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("02-POP"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("03-Jazz"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("04-Classical"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("05-Dance"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("06-Heavy"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("07-Disco"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("08-Soft"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("09-3D Stere/Live"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);
			// Add items to the combo box.
			wsprintf(szString, TEXT("10-Hall"));			
			SendMessage (hwndPortCombo, CB_ADDSTRING, 0, (LPARAM) szString);

			// Select the first item as default.
			SendMessage (hwndPortCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndPortCombo);			

			return TRUE;
		
		case WM_COMMAND:
			if ((HIWORD(wParam) == CBN_SELCHANGE))
			{
				DWORD dwModeSelect;
				DWORD dwResult;
				DWORD dwAddr, dwVal, dwRet, dwSuccess;
				DWORD dwVal40, dwVal20, dwVal1E;

				/// get Value in combo list
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_EQ_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwModeSelect = 0;		///set default
				else
					dwModeSelect = dwResult;
				
				///read from CODEC
				dwAddr = 0x40;
				spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwVal40 = dwRet;

				///read from CODEC
				dwAddr = 0x20;
				spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwVal20 = dwRet;

				///read from CODEC
				dwAddr = 0x1E;
				spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwVal1E = dwRet;
				
				switch(dwModeSelect)
				{
					case 0:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						///dwVal1E |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						break;
					case 1:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						///dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						dwVal20 &= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_10);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_2|WM_BIT_1);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_2|WM_BIT_0);	///3D level
						break;
					case 2:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_11|WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_2|WM_BIT_1);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						break;
					case 3:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						///dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_9|WM_BIT_8);		///Bass level
						///dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_2);	///Treble level
						
						///dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_2|WM_BIT_0);	///3D level
						
						break;
					case 4:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_1);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_1|WM_BIT_0);	///3D level

						break;
					case 5:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						///dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						dwVal20 &= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_1|WM_BIT_0);	///Treble level
						
						///dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_3|WM_BIT_2);	///3D level

						break;
					case 6:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_9|WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_1|WM_BIT_0);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						///dwVal1E |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level

						break;
					case 7:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						///dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						///dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						dwVal20 &= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_10);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_2);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						///dwVal1E |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level

						break;
					case 8:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						///dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						///dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						dwVal20 &= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_10);		///Bass level
						///dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_3|WM_BIT_1);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						///dwVal1E |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level

						break;
					case 9:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_3|WM_BIT_1|WM_BIT_0);	///3D level

						break;
					case 10:
						dwVal40 |= (WM_BIT_13);	///enable 3D
						dwVal20 &= ~WM_BIT_12;	///Bass cut-off 130Hz
						///dwVal20 |= WM_BIT_12;	///Bass cut-off 200Hz						
						dwVal20 &= ~WM_BIT_15;	///Bass type Linear
						///dwVal20 |&= WM_BIT_15;	///Bass Adative					
						dwVal20 &= ~(WM_BIT_11|WM_BIT_10|WM_BIT_9|WM_BIT_8);	///Bass level
						dwVal20 |= (WM_BIT_10);		///Bass level
						dwVal20 &= ~WM_BIT_4;	///Treble cut-off 8kHz
						///dwVal20 |= WM_BIT_4;	///Treble cut-off 4kHz				
						dwVal20 &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///Treble level
						dwVal20 |= (WM_BIT_1);	///Treble level
						
						dwVal1E &= ~WM_BIT_5;		///3D cut-off low 200Hz
						///dwVal1E |= WM_BIT_5;		///3D cut-off low 500Hz
						dwVal1E &= ~WM_BIT_4;		///3D cut-off high 2.2KHz
						///dwVal1E |= WM_BIT_4;		///3D cut-off high 1.5KHz
						dwVal1E &= ~(WM_BIT_3|WM_BIT_2|WM_BIT_1|WM_BIT_0);	///3D level
						dwVal1E |= (WM_BIT_3|WM_BIT_1|WM_BIT_0);	///3D level

						break;
					default:
						break;
				}///switch(dwModeSelect)

				///write to CODEC
				dwAddr = 0x40;
				dwVal = dwVal40;
				spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("W:0x%02X-#:0x%04X %s"), dwAddr, dwVal, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				
				///write to CODEC
				dwAddr = 0x20;
				dwVal = dwVal20;
				spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("W:0x%02X-#:0x%04X %s"), dwAddr, dwVal, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///write to CODEC
				dwAddr = 0x1E;
				dwVal = dwVal1E;
				spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
				wsprintf (szString, TEXT("W:0x%02X-#:0x%04X %s"), dwAddr, dwVal, (dwSuccess==0)?L"OK":L"Fail" );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				
				return TRUE;	
			}
			else
			if ((LOWORD(wParam) == IDC_EQ_DUMP) )
			{
				DWORD dwCount = 0;
						
				while( dwCount < WM_REGISTER_MAX_ADDRESS )
				{
					dwAddr = dwCount;
					spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
										
					wsprintf (szString, TEXT("R:0x%02X-#:0x%04X %s"), dwAddr, dwRet, (dwSuccess==0)?L"OK":L"Fail" );
					SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					
					if( dwAddr == 0x7E )
					{
						if( dwRet == WM9713L_CHIP_ID )
						{	
							wsprintf (szString, TEXT("WM_9713L"));
							SetDlgItemText(hDlg, IDC_EQ_EDIT1, szString);
						}	
						else
						if( dwRet == WM9712L_CHIP_ID )
						{	
							wsprintf (szString, TEXT("WM_9712L"));
							SetDlgItemText(hDlg, IDC_EQ_EDIT1, szString);
						}	
						else
						{
							wsprintf (szString, TEXT("WM_xxxx"));
							SetDlgItemText(hDlg, IDC_EQ_EDIT1, szString);					
						}	
					}
					if( (dwAddr == 0x7C) && ( dwRet != 0x574D ))
					{		
						wsprintf (szString, TEXT("UNKNOW CODEC"));
						SetDlgItemText(hDlg, IDC_EQ_EDIT1, szString);
					}						
					dwCount = dwCount+2; 
				}
				
				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_EQ_READ) )
			{				
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_EQ_WRITE) )
			{
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_EQ_VIDEO) )
			{
				///PlayMovie(TEXT("\\HDD4\\Mr_Big_ToBeWithYou.wmv"));
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_EQ_CHECK_DEVICE) )
			{
				DWORD dwNumDevs, dwLoop;
				WAVEOUTCAPS    woc;
				WAVEINCAPS     wic;

				dwNumDevs = waveOutGetNumDevs();
				wsprintf (szString, TEXT("waveOutGetNumDevs=%d"), dwNumDevs );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				if( dwNumDevs == 0 )
					return TRUE;

				for (dwLoop = 0; dwLoop < dwNumDevs; dwLoop++)
				{
				    /* Get info about the next device */
				    if (MMSYSERR_NOERROR == waveOutGetDevCaps(dwLoop, &woc, sizeof(WAVEOUTCAPS)))
				    {
				        /* Display its Device ID and name */
				        ///printf("Device ID #%u: %s\r\n", i, wic.szPname);
						wsprintf (szString, TEXT("Device ID #%u: %s"), dwLoop, woc.szPname );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						wsprintf (szString, TEXT("MID:0x%04x PID:0x%04x"), woc.wMid, woc.wPid );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						wsprintf (szString, TEXT("DrvVer:0x%08x Channels:%d"), woc.vDriverVersion , woc.wChannels );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						if( woc.dwFormats & WAVE_FORMAT_1M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_1M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_1S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_1S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_2M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_2M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_2S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_2S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_4M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_4M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_4S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwFormats & WAVE_FORMAT_4S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
							 

						if( woc.dwSupport & WAVECAPS_LRVOLUME )
						{
							wsprintf (szString, TEXT("WAVECAPS_LRVOLUME supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwSupport & WAVECAPS_PITCH )
						{
							wsprintf (szString, TEXT("WAVECAPS_PITCH supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwSupport & WAVECAPS_PLAYBACKRATE )
						{
							wsprintf (szString, TEXT("WAVECAPS_PLAYBACKRATE supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwSupport & WAVECAPS_VOLUME )
						{
							wsprintf (szString, TEXT("WAVECAPS_VOLUME supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( woc.dwSupport & WAVECAPS_SAMPLEACCURATE )
						{
							wsprintf (szString, TEXT("WAVECAPS_SAMPLEACCURATE supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	

				    }
				}

				dwNumDevs = waveInGetNumDevs();
				wsprintf (szString, TEXT("waveInGetNumDevs=%d"), dwNumDevs );
				SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				if( dwNumDevs == 0 )
					return TRUE;
				
				for (dwLoop = 0; dwLoop < dwNumDevs; dwLoop++)
				{
				    /* Get info about the next device */
				    if (MMSYSERR_NOERROR == waveInGetDevCaps(dwLoop, &wic, sizeof(WAVEINCAPS)))
				    {
				        /* Display its Device ID and name */
				        ///printf("Device ID #%u: %s\r\n", i, wic.szPname);
						wsprintf (szString, TEXT("Device ID #%u: %s"), dwLoop, wic.szPname );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						wsprintf (szString, TEXT("MID:0x%04x PID:0x%04x"), wic.wMid, wic.wPid );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						wsprintf (szString, TEXT("DrvVer:0x%08x Channels:%d"), wic.vDriverVersion , wic.wChannels );
						SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
						if( wic.dwFormats & WAVE_FORMAT_1M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_1M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_1S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_1S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_1S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_2M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_2M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_2S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_2S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_2S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_4M08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4M08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_4M16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4M16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_4S08 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4S08 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
						if( wic.dwFormats & WAVE_FORMAT_4S16 )
						{
							wsprintf (szString, TEXT("WAVE_FORMAT_4S16 supported") );
							SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);			
						}	
				    }
				}

				return TRUE;					
			}				
			else
			if ((LOWORD(wParam) == IDC_EQ_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_EQ_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

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

void myErrorMessageBox(HWND hwnd, DWORD dwErrorCode)
{
	TCHAR szString[12];         // Temperary string
	
	wsprintf( szString, TEXT("%6d"), dwErrorCode);
	MessageBox(hwnd, szString, L"Error", MB_OK);
}	

#ifdef ORGTEST

HRESULT PlayMovie(LPTSTR lpszMovie)
{
	HRESULT hr; 
	// we will use several DirectShow interfaces
	IMediaControl *pMC = NULL; 
	IGraphBuilder *pGB = NULL;
	IMediaEventEx *pME = NULL;
	
	long evCode; // something to hold a returned event code

    // Initialize the COM library.
    hr = CoInitialize(NULL);

	// instantiate a filter graph as in-proc server
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
	IID_IMediaControl, (void **)&pMC);
	
	// we'll use this interface to build the graph
	hr = pMC->QueryInterface(IID_IGraphBuilder, (void **)&pGB);
	
	// we'll want to wait for completion of the rendering, so we need a 
	// media event interface
	hr = pMC->QueryInterface(IID_IMediaEventEx, (void **)&pME);
	
	// now we're ready to build the filter graph based on the source file
	// data types
	hr = pGB->RenderFile(lpszMovie, NULL);
	
	// play  the source file
	hr = pMC->Run();
	
	// block application until video rendering operations finish
	hr = pME->WaitForCompletion(INFINITE, &evCode);
	
	// release interfaces
	
	return hr;
}

#else

static IGraphBuilder   *pGraph = NULL;
static IMediaControl   *pMediaControl = NULL;
static IMediaEventEx   *pEvent = NULL;

HRESULT PlayMovie(LPTSTR lpszMovie)
{
	HRESULT hr;
///	long evCode; // something to hold a returned event code	

    // Initialize the COM library.
    hr = CoInitialize(NULL);
    myHRESULTerrorCheck(hr, 199)
	
    // Create the filter graph manager and render the file.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&pGraph);
    myHRESULTerrorCheck(hr, 100)
    if( REGDB_E_CLASSNOTREG == hr )
    	myErrorMessageBox(g_EQhwnd, 1001);
    if( CLASS_E_NOAGGREGATION == hr )
    	myErrorMessageBox(g_EQhwnd, 1002);
    	
    // Filenames on Windows CE start with a \\ instead of a drive letter.
    hr = pGraph->RenderFile(lpszMovie, NULL);
	myHRESULTerrorCheck(hr, 101)

    // Specify the owner window.
    IVideoWindow    *pVidWin = NULL;
    hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
    myHRESULTerrorCheck(hr, 102)
    
    hr = pVidWin->put_Owner((OAHWND)g_EQhwnd);
    myHRESULTerrorCheck(hr, 103)
    
    hr = pVidWin->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS);
    myHRESULTerrorCheck(hr, 104)
    

    // Set the owner window to receive event notices.
    hr = pGraph->QueryInterface(IID_IMediaEventEx, (void **)&pEvent);
    myHRESULTerrorCheck(hr, 105)
    
    hr = pEvent->SetNotifyWindow((OAHWND)g_EQhwnd, WM_GRAPHNOTIFY, 0);
    myHRESULTerrorCheck(hr, 106)

    // Run the graph.
    hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
    myHRESULTerrorCheck(hr, 107)
    
    hr = pMediaControl->Run();
    myHRESULTerrorCheck(hr, 108)

	// block application until video rendering operations finish
///	hr = pEvent->WaitForCompletion(INFINITE, &evCode);   
///    myHRESULTerrorCheck(hr, 109)
    
	return hr;
}

static void CleanUp(void)
{
    IVideoWindow    *pVidWin = NULL;
    pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
    pVidWin->put_Visible(OAFALSE);
    pVidWin->put_Owner(NULL);

    // Stop the graph.
    pMediaControl->Stop();

    long evCode;
    pEvent->WaitForCompletion(INFINITE, &evCode);

    pMediaControl->Release();
    pEvent->Release();
    pGraph->Release();
    PostQuitMessage(0);
}

static void HandleEvent() 
{
    long evCode, param1, param2;
    HRESULT hr;
    while (hr = pEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr))
    { 
        hr = pEvent->FreeEventParams(evCode, param1, param2);
        if ((EC_COMPLETE == evCode) || (EC_USERABORT == evCode))
        { 
            CleanUp();
            break;
        } 
    } 
}

#endif

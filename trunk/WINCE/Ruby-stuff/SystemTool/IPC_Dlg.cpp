// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
///#include <winuserm.h>
#include <commctrl.h>
#include "resource.h"
#include "IPC_Dlg.h"
#include "spIPClibEx.h"


#define IPC_ID 0x12348765

static HWND ghDlg = NULL;
static TCHAR szString[64];         	// Temperary string
static HINSTANCE ghInstance = NULL;

SPIPC_INFO gIPCinfo;
BOOL gbHostThreadRun = FALSE;
BOOL gbClientThreadRun = FALSE;


///prototype
DWORD spIPCDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK spIPC(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
DWORD spIPCHostThread(void);
DWORD spIPCClientThread(void);
void GetRandomBYTE( PBYTE pByte );

///code
DWORD spIPCHostThread(void)
{
	DWORD dwRet = 0;
	BYTE pbData[4096];
	
	gIPCinfo.dwIndex = IPC_ID;
	gIPCinfo.dwBulkSize = 4096;
	if( !spIPClibInitHost( gIPCinfo ) ) {
		wsprintf( szString, TEXT("Host init fail !!!\r\n") );
		SendDlgItemMessage( ghDlg, IDC_IPC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

		return dwRet;
	}
	else
		gIPCinfo = spGetIPClibinfo( IPC_ID );
	
	gbHostThreadRun = TRUE;

	wsprintf( szString, TEXT("Host thread start !!!\r\n") );
	SendDlgItemMessage( ghDlg, IDC_IPC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

	while( 1 ) {
		
		while( !gbClientThreadRun )
			Sleep( 3000 );

		Sleep( 500 );
		///wsprintf( szString, TEXT("Host thread run !!!\r\n") );
		///SendDlgItemMessage( ghDlg, IDC_IPC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
		
		GetRandomBYTE( pbData );
		GetRandomBYTE( (pbData+1) );
		GetRandomBYTE( (pbData+2) );
		GetRandomBYTE( (pbData+3) );
		wsprintf( szString, TEXT("0x%02x 0x%02x 0x%02x 0x%02x"), pbData[0], pbData[1], pbData[2], pbData[3] );
		SendDlgItemMessage( ghDlg, IDC_IPC_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
		DWORD dwIndex = SendDlgItemMessage(ghDlg, IDC_IPC_LIST1, LB_GETCOUNT, 0, 0);							
		SendDlgItemMessage(ghDlg, IDC_IPC_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);	
		
		spIPClibHostTx( IPC_ID, pbData, 4096 );
		
		///WaitForSingleObject( gIPCinfo.hClientRxDone, INFINITE );
		
	}	
	
	gbHostThreadRun = FALSE; 
	
	return dwRet;
}

DWORD spIPCClientThread(void)
{
	DWORD dwRet = 0;
	BYTE pbData[4096];

	gIPCinfo.dwIndex = IPC_ID;
	if( !spIPClibInitClient( gIPCinfo ) ) {
		wsprintf( szString, TEXT("client init fail !!!\r\n") );
		SendDlgItemMessage( ghDlg, IDC_IPC_LIST2, LB_ADDSTRING, 0, (LPARAM)szString);

		return dwRet;
	}
	else
		gIPCinfo = spGetIPClibinfo( IPC_ID );
	
	gbClientThreadRun = TRUE;
	
	wsprintf( szString, TEXT("Client thread start !!!\r\n") );
	SendDlgItemMessage( ghDlg, IDC_IPC_LIST2, LB_ADDSTRING, 0, (LPARAM)szString);
	
	while( 1 ) {
		
		///Sleep( 1000 );
		///wsprintf( szString, TEXT("Client thread run !!!\r\n") );
		///SendDlgItemMessage( ghDlg, IDC_IPC_LIST2, LB_ADDSTRING, 0, (LPARAM)szString);
		
		WaitForSingleObject( gIPCinfo.hHostTxDone, INFINITE );
		///Sleep( 1000 );
		spIPClibClientRx( IPC_ID, pbData, 4096 );

		wsprintf( szString, TEXT("0x%02x 0x%02x 0x%02x 0x%02x"), pbData[0], pbData[1], pbData[2], pbData[3] );
		SendDlgItemMessage( ghDlg, IDC_IPC_LIST2, LB_ADDSTRING, 0, (LPARAM)szString);
		DWORD dwIndex = SendDlgItemMessage(ghDlg, IDC_IPC_LIST2, LB_GETCOUNT, 0, 0);							
		SendDlgItemMessage(ghDlg, IDC_IPC_LIST2, LB_SETTOPINDEX, (dwIndex-1), 0);
		///Sleep( 500 );
		SetEvent( gIPCinfo.hClientRxDone );
	}

	gbClientThreadRun = FALSE; 
	return dwRet;
}




DWORD spIPCDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	ghInstance = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_IPCBOX, hWnd, (DLGPROC)spIPC);
	
	return dwRet;
}


// Message handler for the CPU box.
LRESULT CALLBACK spIPC(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	
	HANDLE hCreateThread = NULL;
///	HWND hwndBufferCombo = NULL;    // Handle to the combobox control
	UINT index = 0;

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
			
			if( 0 )
			{
				MessageBox(NULL, NULL, TEXT("ERROR 00001"), MB_OK);
				return FALSE;
			}	
			
			///hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CPUtilization, NULL, 0, NULL);					
			///CloseHandle(hCreateThread);	

			return TRUE;

			
		case WM_COMMAND:
		
			if ((LOWORD(wParam) == IDC_IPC_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_IPC_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_IPC_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				do
				{
					SendDlgItemMessage(hDlg, IDC_IPC_LIST2, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_IPC_LIST2, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}
			else			
			if ((LOWORD(wParam) == IDC_IPC_INITHOST) )
			{
				if( !gbHostThreadRun ) {
					///gbHostThreadRun = TRUE;
					hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)spIPCHostThread, NULL, 0, NULL);					
					CloseHandle(hCreateThread);
				}
			}
			else
			if ((LOWORD(wParam) == IDC_IPC_CLIENTINIT) )
			{
				if( !gbClientThreadRun ) {
					///gbClientThreadRun = TRUE;
					hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)spIPCClientThread, NULL, 0, NULL);					
					CloseHandle(hCreateThread);
				}
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


void GetRandomBYTE( PBYTE pByte )
{
	static DWORD dwLoop = 0;
	
	if( NULL != pByte ) {
		srand( GetTickCount() + dwLoop );	///random seed
		*pByte = (BYTE)( rand()&0xFF );
	}
	
	dwLoop++;
}

///DWORD FillBufferRandom( SBuffer *pSbuffer )
///{
///	DWORD dwLen = 0;
///	DWORD dwLoop = 0;
///	BYTE bTemp = 0;
///	
///	///get length
///	GetRandomBYTE( &bTemp );
///	dwLen = bTemp;		///0 ~ 0xFF
///	if( 0 != dwLen%4 )
///		dwLen -= (dwLen%4);
///	if( 0 == dwLen )
///		dwLen = 0xFF;
///		
///	dwLoop = dwLen;
///
///	///data fill
///	while( dwLoop > 0 ) {
///		GetRandomBYTE( &bTemp );
///		///pSbuffer->Add2Buffer( &bTemp, sizeof(bTemp) );
///		pSbuffer->Add2Buffer( &bTemp, 1 );
///		dwLoop--;
///	}
///	
///	NKDbgPrintfW( L"TVB:: FillBufferRandom %dByte %dByte !!\r\n", dwLen, pSbuffer->GetBufferSize() );
///	return pSbuffer->GetBufferSize();
///}
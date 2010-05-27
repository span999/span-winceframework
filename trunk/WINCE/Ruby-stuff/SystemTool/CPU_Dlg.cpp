// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
///#include <winuserm.h>
#include <commctrl.h>
#include "resource.h"
#include "CPU_Dlg.h"

///#define		WM_TRAYICON		(WM_USER + 4096)

#define BufferSizeK 1024
#define HDD_PATH_TEST_SOURCE		_T("\\HDD2\\HDDTests.tmp")
#define HDD_PATH_TEST_DESTINATION	_T("\\HDD2\\HDDTestd.tmp")
#define FILE_BYTE_PATTERN			0x9A
///#define FILE_BYTE_SIZE				(16*BufferSizeK*BufferSizeK)
#define FILE_BYTE_SIZE				(32*BufferSizeK*BufferSizeK)
#define MEGA_BYTE_SIZE				(BufferSizeK*BufferSizeK)



///UINT BufferSizeArrary[10];
UINT BufferSizeArrary[13];
UINT FileSizeArrary[4];
static DWORD gdwBufSizeSelect = 0;
static DWORD gdwFileSizeSelect = 1;
static DWORD gdwAvgCUPusage = 0;
BOOL bTestInAction = FALSE;
BYTE *gpbBufferSource = NULL;
static HWND ghDlg = NULL;
static TCHAR szString[64];         	// Temperary string
static BOOL bHDDtestRunning = FALSE;
static HANDLE ghWDTimer = NULL;
static HINSTANCE ghInstance = NULL;
static BOOL g_bWinHide = FALSE;

typedef DWORD (*PFN_GetIdleTime)(void);
static PFN_GetIdleTime gpfGetIdleTime;



///prototype
DWORD CPUusageDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK CPUusage(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static BOOL LoadMyLib(void);
static void CPUtilization(void);
static void AddToProgressBar(HWND hwnd, int nCnt);
static void TrayIconSet(DWORD);
static DWORD QuerySytemMemoryStatus(LPMEMORYSTATUS lpMemStatus);
static void ShowToMemoryBox(HWND hwnd, LPMEMORYSTATUS lpMemStatus);


DWORD CPUusageDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	ghInstance = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_CPUBOX, hWnd, (DLGPROC)CPUusage);
	
	return dwRet;
}





BOOL LoadMyLib(void)
{
    HMODULE hCore;	
    BOOL bRet = FALSE;
    /// init system function call
    hCore = (HMODULE)LoadLibrary( L"coredll.dll" );
    if ( hCore ) {
        gpfGetIdleTime = (PFN_GetIdleTime)GetProcAddress( hCore, L"GetIdleTime" );
        if ( !gpfGetIdleTime )
        {
            FreeLibrary(hCore);
            bRet = FALSE;
		}
        else
            bRet = TRUE;
    }
    
    return bRet;	
}


void CPUtilization(void)
{
	DWORD dwMilliseconds = 2000;	///every second
	DWORD dwWaitRet;
	HANDLE hWDTimer;	
	DWORD dwStopTick = 0;
	DWORD dwIdleEd = 0;
	DWORD PercentIdle = 0;
	DWORD dwStartTick = 0;
	DWORD dwIdleSt = 0;
	DWORD totalPercentIdle = 0;
	DWORD totalCount = 0;
	MEMORYSTATUS MemStatus;

	hWDTimer = CreateEvent(NULL, FALSE, FALSE, L"named CPU Watch event");

	ghWDTimer = hWDTimer;
	
	dwStartTick = GetTickCount();

	///dwIdleSt = GetIdleTime();
	dwIdleSt = gpfGetIdleTime();
	///dwIdleSt = GetThreadTimes();

    while( 1 )
    {
        dwWaitRet = WaitForSingleObject( hWDTimer, dwMilliseconds );
               
        switch( dwWaitRet )
        {
        	case WAIT_TIMEOUT:
       			///timer is timeout

				dwStopTick = GetTickCount();
				///dwIdleEd = GetIdleTime();
				dwIdleEd = gpfGetIdleTime();
				PercentIdle = ( (100*(dwIdleEd - dwIdleSt) ) / (dwStopTick - dwStartTick));
				
				totalPercentIdle = totalPercentIdle + PercentIdle;
				totalCount++;
				                                
				
				///NKDbgPrintfW(L"-->Idle: dwIdleEd=0x%08x dwIdleSt=0x%08x dwStopTick=0x%08x dwStartTick=0x%08x\r\n", dwIdleEd, dwIdleSt, dwStopTick, dwStartTick );
				///NKDbgPrintfW(L"-->Idle: %d percent\r\n", PercentIdle );				
				///spShowPercentageBar(PercentIdle);
				AddToProgressBar(ghDlg, PercentIdle);
				
				QuerySytemMemoryStatus( &MemStatus );
				ShowToMemoryBox(ghDlg, &MemStatus );
				TrayIconSet((100-PercentIdle));
				dwStartTick = GetTickCount();
				///dwIdleSt = GetIdleTime();
				dwIdleSt = gpfGetIdleTime();
			
        		break;
        	case WAIT_OBJECT_0:
        		///reset counter
       			///To Do:
       			///calulate new average CPU usage
       			gdwAvgCUPusage = 100 - (totalPercentIdle/totalCount);
       			if( (gdwAvgCUPusage > 100) || (gdwAvgCUPusage < 0) )
       				gdwAvgCUPusage = 0;
       				
       			/// clear all
       			totalPercentIdle = 0;
       			totalCount = 0;
        		break;
        	default:
        		break;	
        }/// switch( dwWaitRet )
    }/// while( 1 )

}


void AddToProgressBar(HWND hwnd, int nCnt)
{
	TCHAR szString_[64];         	// Temperary string
  	//get the control window
  	HWND hwProgressBar = GetDlgItem(hwnd, IDC_CPU_PROGRESS1);
  	//set progress bar position
	SendMessage(hwProgressBar, PBM_SETPOS, (WPARAM)(100-nCnt), 0);
	wsprintf(szString_, TEXT("%02d%%"), (100-nCnt));
	SetDlgItemText(hwnd, IDC_CPU_STATIC2, szString_);
	///UpdateWindow(ghDlg);	
}


void TrayIconSet(DWORD dwIndex)
{
	static BOOL bTrayOn = FALSE;
	NOTIFYICONDATA cb;
	static HICON hPreIcon = NULL;
	static UINT uiPreID = 0;
	cb.cbSize = sizeof(cb);
	cb.hWnd = ghDlg;
	///cb.uFlags = NIF_ICON |NIF_MESSAGE|NIF_TIP;
	cb.uFlags = NIF_ICON|NIF_MESSAGE;
	cb.uCallbackMessage = WM_TRAYICON;
	cb.uID = 199;

	if( dwIndex >= 0 && dwIndex <= 100)
	{
		if( dwIndex == 0 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON00) );
			///cb.uID = IDI_ICON00;
		}
		else
		if( dwIndex <= 5 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON05) );
			///cb.uID = IDI_ICON05;
		}
		else
		if( dwIndex <= 6 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON06) );
			///cb.uID = IDI_ICON06;
		}
		else
		if( dwIndex <= 7 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON07) );
			///cb.uID = IDI_ICON07;
		}
		else
		if( dwIndex <= 8 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON08) );
			///cb.uID = IDI_ICON08;
		}
		else
		if( dwIndex <= 9 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON09) );
			///cb.uID = IDI_ICON09;
		}
		else
		if( dwIndex <= 10 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON10) );
			///cb.uID = IDI_ICON10;
		}
		else
		if( dwIndex <= 11 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON11) );
			///cb.uID = IDI_ICON11;
		}
		else
		if( dwIndex <= 12 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON12) );
			///cb.uID = IDI_ICON12;
		}
		else
		if( dwIndex <= 13 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON13) );
			///cb.uID = IDI_ICON13;
		}
		else
		if( dwIndex <= 14 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON14) );
			///cb.uID = IDI_ICON14;
		}
		else
		if( dwIndex <= 15 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON15) );
			///cb.uID = IDI_ICON15;
		}
		else
		if( dwIndex <= 20 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON20) );
			///cb.uID = IDI_ICON20;
		}
		else
		if( dwIndex <= 25 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON25) );
			///cb.uID = IDI_ICON25;
		}
		else
		if( dwIndex <= 30 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON30) );
			///cb.uID = IDI_ICON30;
		}
		else
		if( dwIndex <= 35 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON35) );
			///cb.uID = IDI_ICON35;
		}
		else
		if( dwIndex <= 40 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON40) );
			///cb.uID = IDI_ICON40;
		}
		else
		if( dwIndex <= 55 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON55) );
			///cb.uID = IDI_ICON55;
		}
		else
		if( dwIndex <= 60 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON60) );
			///cb.uID = IDI_ICON60;
		}
		else
		if( dwIndex <= 65 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON65) );
			///cb.uID = IDI_ICON65;
		}
		else
		if( dwIndex <= 70 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON70) );
			///cb.uID = IDI_ICON70;
		}
		else
		if( dwIndex <= 75 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON75) );
			///cb.uID = IDI_ICON75;
		}
		else
		if( dwIndex <= 80 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON80) );
			///cb.uID = IDI_ICON80;
		}
		else
		if( dwIndex <= 85 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON85) );
			///cb.uID = IDI_ICON85;
		}
		else
		if( dwIndex <= 90 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON90) );
			///cb.uID = IDI_ICON90;
		}
		else
		if( dwIndex <= 91 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON91) );
			///cb.uID = IDI_ICON91;
		}
		else
		if( dwIndex <= 92 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON92) );
			///cb.uID = IDI_ICON92;
		}
		else
		if( dwIndex <= 93 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON93) );
			///cb.uID = IDI_ICON93;
		}
		else
		if( dwIndex <= 94 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON94) );
			///cb.uID = IDI_ICON94;
		}
		else
		if( dwIndex <= 95 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON95) );
			///cb.uID = IDI_ICON95;
		}
		else
		if( dwIndex <= 96 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON96) );
			///cb.uID = IDI_ICON96;
		}
		else
		if( dwIndex <= 97 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON97) );
			///cb.uID = IDI_ICON97;
		}
		else
		if( dwIndex <= 98 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON98) );
			///cb.uID = IDI_ICON98;
		}
		else
		if( dwIndex <= 99 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON99) );
			///cb.uID = IDI_ICON99;
		}
		else
		if( dwIndex <= 100 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON100) );
			///cb.uID = IDI_ICON100;
		}

	}
	else
	{
		cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON_UNKNOW) );
		///cb.uID = IDI_ICON_UNKNOW;
	}		

	///if it's the same icon
	///if( cb.uID == uiPreID )
	if( cb.hIcon == hPreIcon )
	{
		return;
	}		

#if 0
	///kill the old one, if it exist
	if( NULL != hPreIcon ) {
		HICON hPreIcon_ = NULL;
		UINT uiPreID_ = 0;
		
		///store new
		hPreIcon_ = cb.hIcon;
		uiPreID_ = cb.uID;
		
		///kill old
		cb.hIcon = hPreIcon;
		cb.uID = uiPreID;
		Shell_NotifyIcon(NIM_DELETE,&cb);
		
		///add new
		cb.hIcon = hPreIcon_;
		cb.uID = uiPreID_;		
		Shell_NotifyIcon(NIM_ADD,&cb);
		
		bTrayOn = TRUE;
	}
#else
	if( !bTrayOn ) {
		Shell_NotifyIcon(NIM_ADD,&cb);
		bTrayOn = TRUE;
	}else{
		Shell_NotifyIcon(NIM_MODIFY,&cb);
		///Shell_NotifyIcon(NIM_ADD,&cb);
		bTrayOn = TRUE;		
	}
#endif

	///keep the 
	hPreIcon = cb.hIcon;
	uiPreID = cb.uID;

	if( dwIndex > 0xFF )
	{	
		Shell_NotifyIcon(NIM_DELETE,&cb);
		bTrayOn = FALSE;	
	}	
		///NIM_ADD, NIM_DELETE, NIM_MODIFY
		///m_stcStatus.SetWindowText(msg);	
		
}

// Message handler for the CPU box.
LRESULT CALLBACK CPUusage(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	
	HANDLE hCreateThread = NULL;
	///TCHAR szString[64];         	// Temperary string
	HWND hwndBufferCombo = NULL;    // Handle to the combobox control
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
			
			if( !LoadMyLib() )
			{
				MessageBox(NULL, NULL, TEXT("ERROR 00001"), MB_OK);
				return FALSE;
			}	
			
			hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CPUtilization, NULL, 0, NULL);					
			CloseHandle(hCreateThread);	

			return TRUE;

		case WM_TRAYICON:
			
			NKDbgPrintfW(L"SPD@WM_TRAYICON click !\r\n");
			if (TRUE == g_bWinHide)
			{
			    ShowWindow(hDlg, SW_SHOW);
			    SetForegroundWindow(hDlg);	// make us come to the front
			    g_bWinHide = FALSE;
			} 
			else 
			{
			    g_bWinHide = TRUE;
			    ShowWindow(hDlg, SW_HIDE);
			}
			
			return TRUE;
			
		case WM_COMMAND:
		
			if ((LOWORD(wParam) == IDC_CPU_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_CPU_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_CPU_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			else			
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_CPU_CHECK_EXIT2TAY, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
				{	///minimise the windows
					g_bWinHide = TRUE;
					ShowWindow(hDlg, SW_HIDE);
				}
				else
				{	///End the windows
					TrayIconSet( (0xFF+1) );
					EndDialog(hDlg, LOWORD(wParam));
				}	
				return TRUE;
			}							
			break;
	}

    return FALSE;
}


static DWORD QuerySytemMemoryStatus(LPMEMORYSTATUS lpMemStatus)
{
	DWORD dwRet = 0;
	
	lpMemStatus->dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus( lpMemStatus );
	
	return dwRet;
}


void ShowToMemoryBox(HWND hwnd, LPMEMORYSTATUS lpMemStatus)
{
	TCHAR szString_[64];         	// Temperary string
	HWND hwProgressBar;
	static DWORD dwMemLoad = 0;
	
	if( dwMemLoad == lpMemStatus->dwMemoryLoad )
		return;

	dwMemLoad = lpMemStatus->dwMemoryLoad;
	
  	//get the control window
  	hwProgressBar = GetDlgItem(hwnd, IDC_CPU_PROGRESS2);
  	//set progress bar position
	SendMessage(hwProgressBar, PBM_SETPOS, (WPARAM)(lpMemStatus->dwMemoryLoad), 0);

	wsprintf(szString_, TEXT("%02d%%"), lpMemStatus->dwMemoryLoad);
	SetDlgItemText(hwnd, IDC_CPU_STATIC3, szString_);
	
	///clear screen
	do
	{
		SendDlgItemMessage(hwnd, IDC_CPU_LIST1, LB_DELETESTRING, 0, 0);
	}
	while( 0 != SendDlgItemMessage(hwnd, IDC_CPU_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );	
	
	wsprintf(szString_, TEXT("Memory Load    : %02d%%"), lpMemStatus->dwMemoryLoad);
	SendDlgItemMessage(hwnd, IDC_CPU_LIST1, LB_ADDSTRING, 0, (LPARAM)szString_);

	wsprintf(szString_, TEXT("Total Physical : %08d KB"), lpMemStatus->dwTotalPhys/1024);
	SendDlgItemMessage(hwnd, IDC_CPU_LIST1, LB_ADDSTRING, 0, (LPARAM)szString_);

	wsprintf(szString_, TEXT("Available Phys : %08d KB"), lpMemStatus->dwAvailPhys/1024);
	SendDlgItemMessage(hwnd, IDC_CPU_LIST1, LB_ADDSTRING, 0, (LPARAM)szString_);

///	wsprintf(szString_, TEXT("Total page file: %08d"), lpMemStatus->dwTotalPageFile);
///	SendDlgItemMessage(hwnd, IDC_CPU_LIST1, LB_ADDSTRING, 0, (LPARAM)szString_);
///
///	wsprintf(szString_, TEXT("Available PF   : %08d"), lpMemStatus->dwAvailPageFile);
///	SendDlgItemMessage(hwnd, IDC_CPU_LIST1, LB_ADDSTRING, 0, (LPARAM)szString_);

	wsprintf(szString_, TEXT("Total Virtual  : %08d KB"), lpMemStatus->dwTotalVirtual/1024);
	SendDlgItemMessage(hwnd, IDC_CPU_LIST1, LB_ADDSTRING, 0, (LPARAM)szString_);

	wsprintf(szString_, TEXT("Available Virt : %08d KB"), lpMemStatus->dwAvailVirtual/1024);
	SendDlgItemMessage(hwnd, IDC_CPU_LIST1, LB_ADDSTRING, 0, (LPARAM)szString_);
	
	///UpdateWindow(ghDlg);	
}

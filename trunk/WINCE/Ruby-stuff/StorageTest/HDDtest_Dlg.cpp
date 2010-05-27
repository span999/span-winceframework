// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
///#include <winuserm.h>
#include <commctrl.h>
///#include "Thales_defines.h"
#include "resource.h"

#define		WM_TRAYICON		(WM_USER + 4096)

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

typedef DWORD (*PFN_GetIdleTime)(void);
static PFN_GetIdleTime gpfGetIdleTime;



///prototype
DWORD HDDtestDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK HDDtest(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static BYTE* BufferSource(BOOL bCreate, DWORD dwSize);
static BOOL IsSourceFileReady(void);
static BOOL IsDestinationFileReady(void);
static BOOL CreateSourceFile(void);
static BOOL DeleteSourceFile(void);
static BOOL DeleteDestinationFile(void);
static BOOL ButtonActive(HWND hDlg, BOOL bActive);
static DWORD CopyTest(void);
static DWORD ReadTest(void);
static DWORD WriteTest(void);
static BOOL LoadMyLib(void);
static void CPUtilization(void);
static void AddToProgressBar(HWND hwnd, int nCnt);
void TrayIconSet(DWORD);



DWORD HDDtestDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	ghInstance = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_HDDTESTBOX, hWnd, (DLGPROC)HDDtest);
	
	return dwRet;
}


BYTE GetRandomByte()
{
	BYTE byteReturn;

	srand( GetTickCount() );
	
	byteReturn = (BYTE)(0xFF & rand());
	
	return byteReturn;
}


BOOL RandomMemset(BYTE* pbBuf, DWORD dwSize)
{
	BOOL bRet = TRUE;
	DWORD dwSizeInByte = dwSize;
	
	if( dwSizeInByte < 1 )
		bRet = FALSE;
	else	
		bRet = TRUE;
		
	while( (bRet == TRUE) && (dwSizeInByte > 0) )
	{
		*pbBuf = GetRandomByte();
		pbBuf++;
		dwSizeInByte--;
	}

	return bRet;
}


BYTE* BufferSource(BOOL bCreate, DWORD dwSize)
{
	///gpbBufferSource
	BYTE* pbBuf = NULL;
	
	if( NULL == gpbBufferSource )
	{
		if( bCreate )
		{	///create buffer source
			pbBuf = (BYTE *)malloc(dwSize);
			if(pbBuf == NULL)
			{
				///NKDbgPrintfW(L"malloc fail !! insufficient memory available\r\n");
				pbBuf = NULL;
			}
			else
			{	/// fill with pattern
		#if 0		
				memset( pbBuf, FILE_BYTE_PATTERN, dwSize);
		#else
				RandomMemset( pbBuf, dwSize );
		#endif		
				gpbBufferSource = pbBuf;
			}				
		}
		else
		{
			pbBuf = NULL;
		}		
	}
	else
	{
		if( bCreate )
		{
			pbBuf = NULL;
		}
		else
		{	///delete buffer source
			free(gpbBufferSource);
			gpbBufferSource = NULL;
			pbBuf = NULL;
		}		
	}		
	
	return pbBuf;
} 


BOOL IsSourceFileReady(void)
{
	BOOL bRet = FALSE;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	
	hFile = CreateFile( HDD_PATH_TEST_SOURCE, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		bRet = FALSE ;
	else
		bRet = TRUE;	
	
	CloseHandle(hFile);
	return bRet;
}


BOOL IsDestinationFileReady(void)
{
	BOOL bRet = FALSE;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	
	hFile = CreateFile( HDD_PATH_TEST_DESTINATION, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		bRet = FALSE ;
	else
		bRet = TRUE;	
	
	CloseHandle(hFile);	
	return bRet;
}


BOOL CreateSourceFile(void)
{
	BOOL bRet = FALSE;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	
	if( IsSourceFileReady() )
	{	/// file already exist
		///bRet = TRUE;
		bRet = DeleteSourceFile();
	}
	else
	{	/// file not exist
		bRet = TRUE;
	}
	
	if( bRet )
	{	/// file not exist, create one
		BYTE *pbBufc = NULL;
		DWORD dwNumBytes = 0;
		DWORD dwRemainByte = (FileSizeArrary[gdwFileSizeSelect]*MEGA_BYTE_SIZE);
		hFile = CreateFile( HDD_PATH_TEST_SOURCE, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
		if( hFile != INVALID_HANDLE_VALUE )
		{
			///fill the file in the size
			///init buffer
			pbBufc = BufferSource( TRUE, (256*BufferSizeK));
			
			if( pbBufc != NULL )
			{	///fill file with buffer
				while(1)
				{
					if( dwRemainByte <= 0 )
						break;
						
					bRet = WriteFile(hFile, pbBufc, (256*BufferSizeK), &dwNumBytes, NULL);
					if( bRet )
						dwRemainByte = dwRemainByte - (256*BufferSizeK);	
				}	
			}	
			
			///de-init buffer
			BufferSource( FALSE, (256*BufferSizeK));
			CloseHandle(hFile);
		}
		
	}	
	
	return bRet;
}


BOOL DeleteSourceFile(void)
{
	BOOL bRet = FALSE;

	bRet = DeleteFile( HDD_PATH_TEST_SOURCE );
	if( !bRet )
		NKDbgPrintfW(L"Can not deletefile\r\n");
	
	return bRet;
}


BOOL DeleteDestinationFile(void)
{
	BOOL bRet = FALSE;

	bRet = DeleteFile( HDD_PATH_TEST_DESTINATION );
	if( !bRet )
		NKDbgPrintfW(L"Can not deletefile\r\n");
	
	return bRet;
}


BOOL ButtonActive(HWND hDlg, BOOL bActive)
{
	BOOL bRet = TRUE;
/*	
	SendDlgItemMessage(hDlg, IDC_HDDTEST_COPY, BM_SETSTYLE, (WPARAM)LOWORD(dwStyle), (LPARAM)(MAKELPARAM (TRUE, 0)));
*/	
	
	if( bActive )
	{	
		SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		UpdateWindow(hDlg);
	}	
	else
	{	
		SetCursor(LoadCursor(NULL,IDC_WAIT));
	}	
		
	return bRet; 
}


DWORD CopyTest(void)
{
	DWORD dwRet;
	BOOL bRet;
	DWORD time1 = 0;
	DWORD time2 = 0;
	DWORD time3 = 0;
	DWORD TransferRate = 0;
	
	if( !IsSourceFileReady() )
		CreateSourceFile();
		
	if( IsDestinationFileReady() )
		DeleteDestinationFile();
	
	time1 = GetTickCount();
	bRet = CopyFile( HDD_PATH_TEST_SOURCE, HDD_PATH_TEST_DESTINATION, FALSE );
	time2 = GetTickCount();
	
	if( bRet )
	{
		TransferRate = (FileSizeArrary[gdwFileSizeSelect]*MEGA_BYTE_SIZE)/(time2-time1);
		dwRet = (TransferRate*1000)/BufferSizeK;
	}
	else
	{
		dwRet = 0;
	}		
	
	return dwRet;
}


DWORD ReadTest(void)
{
	DWORD dwRet;
	BOOL bRet;
	DWORD time1 = 0;
	DWORD time2 = 0;
	DWORD time3 = 0;
	DWORD TransferRate = 0;
	DWORD dwRemainByte = (FileSizeArrary[gdwFileSizeSelect]*MEGA_BYTE_SIZE);
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwNumBytes;
	BYTE *pbBufr = NULL;
	
	if( !IsSourceFileReady() )
		CreateSourceFile();
		
	if( IsDestinationFileReady() )
		DeleteDestinationFile();

	hFile = CreateFile( HDD_PATH_TEST_SOURCE, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if( hFile != INVALID_HANDLE_VALUE )
	{
		pbBufr = BufferSource(TRUE, (BufferSizeArrary[gdwBufSizeSelect]*BufferSizeK));
		if( pbBufr != NULL )
		{	
			time1 = GetTickCount();
			while(1)
			{			
				if( dwRemainByte <= 0 )
					break;
						
				bRet = ReadFile(hFile, pbBufr, (BufferSizeArrary[gdwBufSizeSelect]*BufferSizeK), &dwNumBytes, NULL);
				if( bRet )
					dwRemainByte = dwRemainByte - (BufferSizeArrary[gdwBufSizeSelect]*BufferSizeK);
			}///while()		
			time2 = GetTickCount();
		}
		BufferSource(FALSE, (BufferSizeArrary[gdwBufSizeSelect]*BufferSizeK));
		CloseHandle(hFile);
	}
	
	if( bRet )
	{
		TransferRate = (FileSizeArrary[gdwFileSizeSelect]*MEGA_BYTE_SIZE)/(time2-time1);
		dwRet = (TransferRate*1000)/BufferSizeK;
	}
	else
	{
		dwRet = 0;
	}		
	
	return dwRet;
}


DWORD WriteTest(void)
{
	DWORD dwRet;
	BOOL bRet;
	DWORD time1 = 0;
	DWORD time2 = 0;
	DWORD time3 = 0;
	DWORD TransferRate = 0;
	DWORD dwRemainByte = (FileSizeArrary[gdwFileSizeSelect]*MEGA_BYTE_SIZE);
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwNumBytes;
	BYTE *pbBufw = NULL;
	
	if( !IsSourceFileReady() )
		CreateSourceFile();
		
	if( IsDestinationFileReady() )
		DeleteDestinationFile();

	hFile = CreateFile( HDD_PATH_TEST_DESTINATION, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if( hFile != INVALID_HANDLE_VALUE )
	{
		pbBufw = BufferSource(TRUE, (BufferSizeArrary[gdwBufSizeSelect]*BufferSizeK));
		if( pbBufw != NULL )
		{	
			time1 = GetTickCount();
			while(1)
			{			
				if( dwRemainByte <= 0 )
					break;
						
				bRet = WriteFile(hFile, pbBufw, (BufferSizeArrary[gdwBufSizeSelect]*BufferSizeK), &dwNumBytes, NULL);
				if( bRet )
					dwRemainByte = dwRemainByte - (BufferSizeArrary[gdwBufSizeSelect]*BufferSizeK);
			}///while()		
			time2 = GetTickCount();
		}
		BufferSource(FALSE, (BufferSizeArrary[gdwBufSizeSelect]*BufferSizeK));
		CloseHandle(hFile);
	}
	
	if( bRet )
	{
		TransferRate = (FileSizeArrary[gdwFileSizeSelect]*MEGA_BYTE_SIZE)/(time2-time1);
		dwRet = (TransferRate*1000)/BufferSizeK;
	}
	else
	{
		dwRet = 0;
	}		
	
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
  	HWND hwProgressBar = GetDlgItem(hwnd, IDC_HDDTEST_PROGRESS1);
  	//set progress bar position
	SendMessage(hwProgressBar, PBM_SETPOS, (WPARAM)(100-nCnt), 0);
	wsprintf(szString_, TEXT("%02d%%(%02d%%)"), (100-nCnt), gdwAvgCUPusage);
	SetDlgItemText(hwnd, IDC_HDDTEST_STATIC2, szString_);
	///UpdateWindow(ghDlg);	
}


void TrayIconSet(DWORD dwIndex)
{
	///CString title;
	///CString msg;
	///title.LoadString(IDS_TITLE);
	static BOOL bTrayOn = FALSE;
	NOTIFYICONDATA cb;
	static HICON hPreIcon = NULL;
	static UINT uiPreID = 0;
	cb.cbSize = sizeof(cb);
	cb.hWnd = ghDlg;
	///cb.uFlags = NIF_ICON |NIF_MESSAGE|NIF_TIP;
	cb.uFlags = NIF_ICON|NIF_MESSAGE;
	cb.uCallbackMessage = WM_TRAYICON;
	///swprintf(cb.szTip,L"%s",title);

	if( dwIndex >= 0 && dwIndex <= 100)
	{
		if( dwIndex == 0 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON00) );
			cb.uID = IDI_ICON00;
		}
		else
		if( dwIndex <= 5 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON05) );
			cb.uID = IDI_ICON05;
		}
		else
		if( dwIndex <= 6 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON06) );
			cb.uID = IDI_ICON06;
		}
		else
		if( dwIndex <= 7 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON07) );
			cb.uID = IDI_ICON07;
		}
		else
		if( dwIndex <= 8 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON08) );
			cb.uID = IDI_ICON08;
		}
		else
		if( dwIndex <= 9 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON09) );
			cb.uID = IDI_ICON09;
		}
		else
		if( dwIndex <= 10 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON10) );
			cb.uID = IDI_ICON10;
		}
		else
		if( dwIndex <= 11 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON11) );
			cb.uID = IDI_ICON11;
		}
		else
		if( dwIndex <= 12 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON12) );
			cb.uID = IDI_ICON12;
		}
		else
		if( dwIndex <= 13 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON13) );
			cb.uID = IDI_ICON13;
		}
		else
		if( dwIndex <= 14 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON14) );
			cb.uID = IDI_ICON14;
		}
		else
		if( dwIndex <= 15 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON15) );
			cb.uID = IDI_ICON15;
		}
		else
		if( dwIndex <= 20 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON20) );
			cb.uID = IDI_ICON20;
		}
		else
		if( dwIndex <= 25 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON25) );
			cb.uID = IDI_ICON25;
		}
		else
		if( dwIndex <= 30 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON30) );
			cb.uID = IDI_ICON30;
		}
		else
		if( dwIndex <= 35 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON35) );
			cb.uID = IDI_ICON35;
		}
		else
		if( dwIndex <= 40 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON40) );
			cb.uID = IDI_ICON40;
		}
		else
		if( dwIndex <= 55 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON55) );
			cb.uID = IDI_ICON55;
		}
		else
		if( dwIndex <= 60 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON60) );
			cb.uID = IDI_ICON60;
		}
		else
		if( dwIndex <= 65 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON65) );
			cb.uID = IDI_ICON65;
		}
		else
		if( dwIndex <= 70 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON70) );
			cb.uID = IDI_ICON70;
		}
		else
		if( dwIndex <= 75 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON75) );
			cb.uID = IDI_ICON75;
		}
		else
		if( dwIndex <= 80 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON80) );
			cb.uID = IDI_ICON80;
		}
		else
		if( dwIndex <= 85 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON85) );
			cb.uID = IDI_ICON85;
		}
		else
		if( dwIndex <= 90 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON90) );
			cb.uID = IDI_ICON90;
		}
		else
		if( dwIndex <= 91 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON91) );
			cb.uID = IDI_ICON91;
		}
		else
		if( dwIndex <= 92 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON92) );
			cb.uID = IDI_ICON92;
		}
		else
		if( dwIndex <= 93 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON93) );
			cb.uID = IDI_ICON93;
		}
		else
		if( dwIndex <= 94 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON94) );
			cb.uID = IDI_ICON94;
		}
		else
		if( dwIndex <= 95 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON95) );
			cb.uID = IDI_ICON95;
		}
		else
		if( dwIndex <= 96 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON96) );
			cb.uID = IDI_ICON96;
		}
		else
		if( dwIndex <= 97 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON97) );
			cb.uID = IDI_ICON97;
		}
		else
		if( dwIndex <= 98 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON98) );
			cb.uID = IDI_ICON98;
		}
		else
		if( dwIndex <= 99 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON99) );
			cb.uID = IDI_ICON99;
		}
		else
		if( dwIndex <= 100 )
		{	
			cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON100) );
			cb.uID = IDI_ICON100;
		}

	}
	else
	{
		cb.hIcon = LoadIcon( ghInstance, MAKEINTRESOURCE(IDI_ICON_UNKNOW) );
		cb.uID = IDI_ICON_UNKNOW;
	}		

	///if it's the same icon
	if( cb.uID == uiPreID )
	{
		return;
	}		

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
	}


///	if( !bTrayOn ) {
///		Shell_NotifyIcon(NIM_ADD,&cb);
		bTrayOn = TRUE;
///	}else{
///		Shell_NotifyIcon(NIM_MODIFY,&cb);
///		Shell_NotifyIcon(NIM_ADD,&cb);
///		bTrayOn = TRUE;		
///	}

	///keep the 
	hPreIcon = cb.hIcon;
	uiPreID = cb.uID;

		
	if( dwIndex > 0xFF )
		Shell_NotifyIcon(NIM_DELETE,&cb);
		///NIM_ADD, NIM_DELETE, NIM_MODIFY
		///m_stcStatus.SetWindowText(msg);	
}


void DoCopy()
{
	DWORD dwTxf1, dwTxf2, dwTxf3;
	
	/// show the selection to list box.
	///wsprintf(szString, TEXT("Copy function @ %dKB bulk"), BufferSizeArrary[gdwBufSizeSelect]);
	wsprintf(szString, TEXT("Copy function @ CopyFile()"));
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("FileSize @ %dMB"), FileSizeArrary[gdwFileSizeSelect]);
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);	
	///wsprintf(szString, TEXT("BufferSize @ %dKB bulk"), BufferSizeArrary[gdwBufSizeSelect]);
	///SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

	dwTxf1 = CopyTest();
	wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf1 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	dwTxf2 = CopyTest();
	wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf2 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	dwTxf3 = CopyTest();
	wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf3 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

	wsprintf(szString, TEXT("Average Rate=%dKB/s"), (dwTxf1+dwTxf2+dwTxf3)/3 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

	///move the screen to the last message		
	DWORD dwIndex = SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_GETCOUNT, 0, 0);							
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
	
	DeleteSourceFile();
	DeleteDestinationFile();

	if( ghWDTimer )
		SetEvent(ghWDTimer);
	bHDDtestRunning = FALSE;
	
}


void DoRead()
{
	DWORD dwTxf1, dwTxf2, dwTxf3;

	/// show the selection to list box.
	wsprintf(szString, TEXT("Read function @ ReadFile()"));
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("FileSize @ %dMB"), FileSizeArrary[gdwFileSizeSelect]);
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);	
	wsprintf(szString, TEXT("BufferSize @ %dKB bulk"), BufferSizeArrary[gdwBufSizeSelect]);
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

	dwTxf1 = ReadTest();
	wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf1 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	dwTxf2 = ReadTest();
	wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf2 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	dwTxf3 = ReadTest();
	wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf3 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

	wsprintf(szString, TEXT("Average Rate=%dKB/s"), (dwTxf1+dwTxf2+dwTxf3)/3 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

	///move the screen to the last message		
	DWORD dwIndex = SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_GETCOUNT, 0, 0);							
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
	
	DeleteSourceFile();
	DeleteDestinationFile();

	if( ghWDTimer )
		SetEvent(ghWDTimer);	
	bHDDtestRunning = FALSE;
	
}


void DoWrite()
{
	DWORD dwTxf1, dwTxf2, dwTxf3;
	
	/// show the selection to list box.
	wsprintf(szString, TEXT("Write function @ WriteFile()"));
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("FileSize @ %dMB"), FileSizeArrary[gdwFileSizeSelect]);
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("BufferSize @ %dKB bulk"), BufferSizeArrary[gdwBufSizeSelect]);
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	
	dwTxf1 = WriteTest();
	wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf1 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	dwTxf2 = WriteTest();
	wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf2 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	dwTxf3 = WriteTest();
	wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf3 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	
	wsprintf(szString, TEXT("Average Rate=%dKB/s"), (dwTxf1+dwTxf2+dwTxf3)/3 );
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	
	///move the screen to the last message		
	DWORD dwIndex = SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_GETCOUNT, 0, 0);							
	SendDlgItemMessage(ghDlg, IDC_HDDTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
	
	DeleteSourceFile();
	DeleteDestinationFile();

	if( ghWDTimer )
		SetEvent(ghWDTimer);
	bHDDtestRunning = FALSE;	
	
}


// Message handler for the HDDtest box.
LRESULT CALLBACK HDDtest(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

			FileSizeArrary[0] = 4;
			FileSizeArrary[1] = 16;
			FileSizeArrary[2] = 32;
			FileSizeArrary[3] = 64;

			/// get the Combo handle
			hwndBufferCombo = GetDlgItem(hDlg, IDC_HDDTEST_COMBO2);
			
			for( index = 0; index < 4; index++ )
			{
				// Add items to the combo box.
				wsprintf(szString, TEXT("%02d-%dMB"), index, FileSizeArrary[index]);			
				SendMessage (hwndBufferCombo, CB_ADDSTRING, 0, (LPARAM) szString);				
			}
			
			// Select the first item as default.
			SendMessage (hwndBufferCombo, CB_SETCURSEL, (WPARAM)1, 0);
			CloseHandle(hwndBufferCombo);					

			BufferSizeArrary[0] = 1;
			BufferSizeArrary[1] = 2;
			BufferSizeArrary[2] = 4;
			BufferSizeArrary[3] = 8;
			BufferSizeArrary[4] = 16;
			BufferSizeArrary[5] = 32;
			BufferSizeArrary[6] = 64;
			BufferSizeArrary[7] = 128;
			BufferSizeArrary[8] = 256;
			BufferSizeArrary[9] = 512;
			BufferSizeArrary[10] = 1024;
			BufferSizeArrary[11] = 2048;			
			BufferSizeArrary[12] = 4096;			

			/// get the Combo handle
			hwndBufferCombo = GetDlgItem(hDlg, IDC_HDDTEST_COMBO1);
			
			///for( index = 0; index < 10; index++ )
			for( index = 0; index < 13; index++ )
			{
				// Add items to the combo box.
				wsprintf(szString, TEXT("%02d-%dK byte"), index, BufferSizeArrary[index]);			
				SendMessage (hwndBufferCombo, CB_ADDSTRING, 0, (LPARAM) szString);				
			}
			
			// Select the first item as default.
			SendMessage (hwndBufferCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndBufferCombo);					
	
			return TRUE;

		case WM_TRAYICON:
			
			NKDbgPrintfW(L"SPD@WM_TRAYICON click !\r\n");
			
			return TRUE;
			
		case WM_COMMAND:
		
			if ((HIWORD(wParam) == CBN_SELCHANGE))
			{
				DWORD dwBufferSelect;
				DWORD dwFileSizeSelect;				
				DWORD dwResult;

				/// get Value in combo list
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_HDDTEST_COMBO2, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwFileSizeSelect = 1;		///set default
				else
					dwFileSizeSelect = dwResult;
				
				gdwFileSizeSelect = dwFileSizeSelect;
				
				/// show the selection to list box.
				wsprintf(szString, TEXT("FileSize in %dMB"), FileSizeArrary[dwFileSizeSelect]);
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);


				/// get Value in combo list
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_HDDTEST_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
				if( dwResult == LB_ERR )
					dwBufferSelect = 0;		///set default
				else
					dwBufferSelect = dwResult;
				
				gdwBufSizeSelect = dwBufferSelect;
				
				/// show the selection to list box.
				wsprintf(szString, TEXT("BufferSize in %dKB"), BufferSizeArrary[dwBufferSelect]);
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);



				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

				return TRUE;
			}
			else
			if ((LOWORD(wParam) == IDC_HDDTEST_COPY) )
			{
#if 0				
				DWORD dwTxf1, dwTxf2, dwTxf3;
				ButtonActive(hDlg, FALSE);
				/// show the selection to list box.
				///wsprintf(szString, TEXT("Copy function @ %dKB bulk"), BufferSizeArrary[gdwBufSizeSelect]);
				wsprintf(szString, TEXT("Copy function @ CopyFile()"));
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				///wsprintf(szString, TEXT("BufferSize @ %dKB bulk"), BufferSizeArrary[gdwBufSizeSelect]);
				///SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				dwTxf1 = CopyTest();
				wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf1 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwTxf2 = CopyTest();
				wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf2 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwTxf3 = CopyTest();
				wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf3 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				wsprintf(szString, TEXT("Average Rate=%dKB/s"), (dwTxf1+dwTxf2+dwTxf3)/3 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				DeleteSourceFile();
				DeleteDestinationFile();
				ButtonActive(hDlg, TRUE);
#else
				if(!bHDDtestRunning)
				{	
					bHDDtestRunning = TRUE;
					hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)DoCopy, NULL, 0, NULL);					
					CloseHandle(hCreateThread);
					if( ghWDTimer )
						SetEvent(ghWDTimer);
					///DoCopy();
				}
				else
				{
					wsprintf(szString, TEXT("Already Testing ...") );
					SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);					
				}		
#endif				
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_HDDTEST_READ) )
			{	
#if 0				
				DWORD dwTxf1, dwTxf2, dwTxf3;
				ButtonActive(hDlg, FALSE);
				/// show the selection to list box.
				wsprintf(szString, TEXT("Read function @ ReadFile()"));
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				wsprintf(szString, TEXT("BufferSize @ %dKB bulk"), BufferSizeArrary[gdwBufSizeSelect]);
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				dwTxf1 = ReadTest();
				wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf1 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwTxf2 = ReadTest();
				wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf2 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwTxf3 = ReadTest();
				wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf3 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				wsprintf(szString, TEXT("Average Rate=%dKB/s"), (dwTxf1+dwTxf2+dwTxf3)/3 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				DeleteSourceFile();
				DeleteDestinationFile();
				ButtonActive(hDlg, TRUE);
#else
				if(!bHDDtestRunning)
				{	
					bHDDtestRunning = TRUE;
					hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)DoRead, NULL, 0, NULL);					
					CloseHandle(hCreateThread);					
					if( ghWDTimer )
						SetEvent(ghWDTimer);					
					///DoRead();
				}
				else
				{
					wsprintf(szString, TEXT("Already Testing ...") );
					SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);					
				}		
#endif				
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_HDDTEST_WRITE) )
			{
#if 0				
				DWORD dwTxf1, dwTxf2, dwTxf3;
				ButtonActive(hDlg, FALSE);
				/// show the selection to list box.
				wsprintf(szString, TEXT("Write function @ WriteFile()"));
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				wsprintf(szString, TEXT("BufferSize @ %dKB bulk"), BufferSizeArrary[gdwBufSizeSelect]);
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				dwTxf1 = WriteTest();
				wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf1 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwTxf2 = WriteTest();
				wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf2 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				dwTxf3 = WriteTest();
				wsprintf(szString, TEXT("Transfer Rate=%dKB/s"), dwTxf3 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				wsprintf(szString, TEXT("Average Rate=%dKB/s"), (dwTxf1+dwTxf2+dwTxf3)/3 );
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				DeleteSourceFile();
				DeleteDestinationFile();
				ButtonActive(hDlg, TRUE);
#else
				if(!bHDDtestRunning)
				{	
					bHDDtestRunning = TRUE;
					hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)DoWrite, NULL, 0, NULL);
					CloseHandle(hCreateThread);	
					if( ghWDTimer )
						SetEvent(ghWDTimer);									
					///DoWrire();
				}
				else
				{
					wsprintf(szString, TEXT("Already Testing ...") );
					SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);					
				}		
#endif				
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_HDDTEST_CLEAR) )
			{
				ButtonActive(hDlg, FALSE);
				do
				{
					SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_HDDTEST_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				ButtonActive(hDlg, TRUE);
				return TRUE;					
			}				
			else			
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				TrayIconSet( (0xFF+1) );
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}							
			break;
	}

    return FALSE;
}
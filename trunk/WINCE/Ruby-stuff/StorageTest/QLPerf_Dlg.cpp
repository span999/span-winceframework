// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
///#include <windows.h>
///#include <winuserm.h>
///#include "Pkfuncs.h"
///#include "Mkfuncs.h"
#include <winbase.h>
#include <commctrl.h>
///#include "Thales_defines.h"
///#include <Pkfuncs.h>

#define CE_APP_BUILD
#define QL_HDD_PATH_TEST_SOURCE		_T("\\HDD2\\TEST.TXT")

void 	mainQLHDDPerf(HWND hDlg);
void	DoW(char, char*, long, long, long, long, int);
void	DoR(char, char*, long, long, long, long, int);

long	alSizes[] = {4096, 2048, 1024, 512, 256};
long	alNumbers[] = {25, 50, 100, 200, 400};
///long	alSizes[] = {4096, 2048, 1024, 512, 256, 128};
///long	alNumbers[] = {25, 50, 100, 200, 400, 800};


char	*pcBufUnaligned, *pcBufAligned, *pcBufRaw;
long	kWrites;
const long BUF_SIZE = 4096*1024;
const long BYTE_ALIGN = 32;

static HWND ghDlg = NULL;
TCHAR szString[64];         	// Temperary string
static BOOL bHDDtestRunning = FALSE;

///extern DWORD GetIdleTime (void);
typedef DWORD (*PFN_GetIdleTime)(void);
static PFN_GetIdleTime gpfGetIdleTime;

///prototype
DWORD QLtestDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK QLtest(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static BOOL LoadMyLib(void);
static void CPUtilization(void);
static void AddToProgressBar(HWND hwnd, int nCnt);


DWORD QLtestDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_QLTESTBOX, hWnd, (DLGPROC)QLtest);
	
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

	hWDTimer = CreateEvent(NULL, FALSE, FALSE, L"named CPU Watch event");

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
				///NKDbgPrintfW(L"-->Idle: dwIdleEd=0x%08x dwIdleSt=0x%08x dwStopTick=0x%08x dwStartTick=0x%08x\r\n", dwIdleEd, dwIdleSt, dwStopTick, dwStartTick );
				///NKDbgPrintfW(L"-->Idle: %d percent\r\n", PercentIdle );				
				///spShowPercentageBar(PercentIdle);
				AddToProgressBar(ghDlg, PercentIdle);
				dwStartTick = GetTickCount();
				///dwIdleSt = GetIdleTime();
				dwIdleSt = gpfGetIdleTime();
			
        		break;
        	case WAIT_OBJECT_0:
        		///set timer
       			///To Do:
        		break;
        	default:
        		break;	
        }/// switch( dwWaitRet )
    }/// while( 1 )

}


void mainQLHDDPerf(HWND hDlg)
{
	int	iPass;
#ifdef CE_APP_BUILD		
	DWORD dwIndex;
#endif	

	///ghDlg = hDlg;
	
	pcBufRaw = (char *)malloc(BUF_SIZE+BYTE_ALIGN+4);
	if ((unsigned)pcBufRaw & (BYTE_ALIGN-1)) {
		pcBufAligned = (char *)(((unsigned)(pcBufRaw+16)) & (~(BYTE_ALIGN-1)));
	} else {
		pcBufAligned = pcBufRaw;
	}
	pcBufUnaligned = pcBufAligned+1;
#ifdef CE_APP_BUILD	
	wsprintf(szString, TEXT("Raw=%x"), pcBufRaw );
	SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("Aligned=%x"), pcBufAligned );
	SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("Unaligned=%x"), pcBufUnaligned );
	SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	
	///move the screen to the last message		
	dwIndex = SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_GETCOUNT, 0, 0);							
	SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
	UpdateWindow(ghDlg);		
#else	
	printf("Raw=%x\nAligned=%x\nUnaligned=%x\n", pcBufRaw, pcBufAligned, pcBufUnaligned);
#endif	


	for (iPass = 0; iPass != sizeof(alSizes)/sizeof(alSizes[0]); iPass++) {
		DoW('A', pcBufAligned, alSizes[iPass], alSizes[iPass]*1024, alNumbers[iPass], 0, 0);
		DoR('A', pcBufAligned, alSizes[iPass], alSizes[iPass]*1024, alNumbers[iPass], 0, 0);
#ifdef CE_APP_BUILD		
		///move the screen to the last message		
		dwIndex = SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_GETCOUNT, 0, 0);							
		SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
		UpdateWindow(ghDlg);		
#endif				
	}
	for (iPass = 0; iPass != sizeof(alSizes)/sizeof(alSizes[0]); iPass++) {
		DoW('U', pcBufUnaligned, alSizes[iPass], alSizes[iPass]*1024, alNumbers[iPass], 0, 0);
		DoR('U', pcBufUnaligned, alSizes[iPass], alSizes[iPass]*1024, alNumbers[iPass], 0, 0);
#ifdef CE_APP_BUILD		
		///move the screen to the last message		
		dwIndex = SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_GETCOUNT, 0, 0);							
		SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);		
		UpdateWindow(ghDlg);				
#endif				
	}
	
#ifdef CE_APP_BUILD
	if( !DeleteFile( QL_HDD_PATH_TEST_SOURCE ) )
	{	
		wsprintf(szString, TEXT("Could not delete TEST.TXT") );
		SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	}
	else
	{
		wsprintf(szString, TEXT("TEST.TXT deleted") );
		SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);		
	}		
	SetDlgItemText(ghDlg, IDC_QLTEST_STATIC, TEXT("?????"));
	free(pcBufRaw);
	bHDDtestRunning = FALSE;
#else	
	exit(0);
#endif	
 }


void DoW(char cType, char *pcBuf, long lSize, long nBytesBuf, long kWrites, long nSleep, int fPrint) {
	HANDLE hFile;
	DWORD dwBytesWritten, dwPos;
	DWORD tBefore, tAfter, xferRate, dwByteCount;
	long nBytesFile = kWrites*nBytesBuf;
	int	iWrite;

#ifdef CE_APP_BUILD	
	if (fPrint)
	{	
		wsprintf(szString, TEXT("DoW(%ld, %ld, %ld)"), nBytesBuf/1024, kWrites, nSleep);
		SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	}
#else		
	if (fPrint) printf("DoW(%ld, %ld, %ld)\n", nBytesBuf/1024, kWrites, nSleep);
#endif		
	
	// Create file
#ifdef CE_APP_BUILD		
	hFile = CreateFile( QL_HDD_PATH_TEST_SOURCE,    // Open one.txt
#else
	hFile = CreateFile( L"\\Mounted Volume\\TEST.TXT",    // Open one.txt
#endif	
		GENERIC_WRITE | GENERIC_READ,      // Open for read&write
		0,                    // Do not share
		NULL,                 // No security
		CREATE_ALWAYS,        // Open or create
		FILE_ATTRIBUTE_NORMAL,// Normal file
		NULL );               // No template file
	if( hFile == INVALID_HANDLE_VALUE )
    {
#ifdef CE_APP_BUILD
		wsprintf(szString, TEXT("Could not open TEST.TXT"));
		SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
#else    	
		printf("Could not open TEST.TXT\n");
#endif		
		CloseHandle(hFile);                     // Close the first file
		return;
    }
	
	// Loop through writes...
	dwByteCount = 0;
	dwPos = SetFilePointer( hFile, 0, NULL, FILE_BEGIN ); 
#ifdef CE_APP_BUILD
	wsprintf(szString, TEXT("Write[%c]..."), cType);
	SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	fflush(stdout);
#else    		
	printf("Write[%c]...", cType); fflush(stdout);
#endif	
	Sleep(500);
	tBefore = GetTickCount();
	for( iWrite = 0; iWrite < kWrites; iWrite++ )
	{ 
		if (!WriteFile(hFile, pcBuf, nBytesBuf, &dwBytesWritten, NULL)) {
#ifdef CE_APP_BUILD
			wsprintf(szString, TEXT("File write failed"));
			SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
#else    	
				wprintf(L"File write failed!\n");
#endif				
			break;
		}
		dwByteCount += dwBytesWritten;
		if (!WriteFile(hFile, pcBuf, 2*128*1024, &dwBytesWritten, NULL)) {
#ifdef CE_APP_BUILD
			wsprintf(szString, TEXT("File write failed!"));
			SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
#else    	
				wprintf(L"File write failed!\n");
#endif				
			break;
		}
		dwByteCount += dwBytesWritten;
	}
	tAfter = GetTickCount();
	xferRate = dwByteCount / (tAfter - tBefore);	
#ifdef CE_APP_BUILD
	wsprintf(szString, TEXT("%2d.%02d[%4ld%c]"), xferRate/1000, (xferRate%1000)/10, lSize, cType);
	SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
#else    		
	printf("%2d.%02d[%4ld%c]\n", xferRate/1000, (xferRate%1000)/10, lSize, cType);
#endif	
	if (fPrint) printf("\n");
	
	// Close file.
	CloseHandle( hFile ); 
	return;
}


void DoR(char cType, char *pcBuf, long lSize, long nBytesBuf, long kReads, long nSleep, int fPrint) {
	HANDLE hFile;
	DWORD dwBytesRead, dwPos;
	DWORD tBefore, tAfter, xferRate, dwByteCount;;
	long nBytesFile = kWrites*nBytesBuf;
	int	iRead;

#ifdef CE_APP_BUILD	
	if (fPrint)
	{	
		wsprintf(szString, TEXT("DoR(%ld, %ld, %ld)"), nBytesBuf/1024, kReads, nSleep);
		SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	}
#else		
	if (fPrint) printf("DoR(%ld, %ld, %ld)\n", nBytesBuf/1024, kReads, nSleep);
#endif
	
	// Open file
#ifdef CE_APP_BUILD		
	hFile = CreateFile( QL_HDD_PATH_TEST_SOURCE,    // Open one.txt
#else	
	hFile = CreateFile( L"\\Mounted Volume\\TEST.TXT",    // Open test.txt
#endif	
			GENERIC_READ,			// Open for read
			0,						// Do not share
			NULL,					// No security
			OPEN_EXISTING,			// Open only if exists already
			FILE_ATTRIBUTE_NORMAL,	// Normal file
			NULL );
    if( hFile == INVALID_HANDLE_VALUE )
    {
#ifdef CE_APP_BUILD
		wsprintf(szString, TEXT("Could not open TEST.TXT"));
		SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
#else    	
      printf("Could not open TEST.TXT\n");
#endif      
      CloseHandle(hFile);                     // Close the first file
      return;
    }
	
	// Loop through writes...
	dwByteCount = 0;
	dwPos = SetFilePointer( hFile, 0, NULL, FILE_BEGIN ); 
#ifdef CE_APP_BUILD
	wsprintf(szString, TEXT("Read[%c]..."), cType);
	SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	fflush(stdout);
#else	
	printf("Read[%c]....", cType); fflush(stdout);
#endif	
	Sleep(500);
	tBefore = GetTickCount();
	for( iRead = 0; iRead < kReads; iRead++ )
	{ 
		if (!ReadFile(hFile, pcBuf, nBytesBuf, &dwBytesRead, NULL)) {
#ifdef CE_APP_BUILD
			wsprintf(szString, TEXT("File read failed"));
			SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
#else			
			wprintf(L"File read failed!\n");
#endif			
			break;
		}
		dwByteCount += dwBytesRead;
	}
	tAfter = GetTickCount();
	xferRate = dwByteCount / (tAfter - tBefore);
#ifdef CE_APP_BUILD
	wsprintf(szString, TEXT("%2d.%02d[%4ld%c]"), xferRate/1000, (xferRate%1000)/10, lSize, cType);
	SendDlgItemMessage(ghDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
#else		
	printf("%2d.%02d[%4ld%c]\n", xferRate/1000, (xferRate%1000)/10, lSize, cType);
#endif	
	if (fPrint) printf("\n");
	
	// Close file.
	CloseHandle( hFile );
	return;
}


void AddToProgressBar(HWND hwnd, int nCnt){
  	//get the control window
  	HWND hwProgressBar = GetDlgItem(hwnd, IDC_QLTEST_PROGRESS1);
  	//set progress bar position
	SendMessage(hwProgressBar, PBM_SETPOS, (WPARAM)(100-nCnt), 0);
	wsprintf(szString, TEXT("%02d%%"), (100-nCnt));
	SetDlgItemText(hwnd, IDC_QLTEST_STATIC2, szString);
	///UpdateWindow(ghDlg);
}


// Message handler for the QLtest box.
LRESULT CALLBACK QLtest(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	

///	TCHAR szString[64];         	// Temperary string
	HANDLE hCreateThread = NULL;
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
#if 0
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

			/// get the Combo handle
			hwndBufferCombo = GetDlgItem(hDlg, IDC_HDDTEST_COMBO1);
			
			for( index = 0; index < 10; index++ )
			{
				// Add items to the combo box.
				wsprintf(szString, TEXT("%02d-%dK byte"), index, BufferSizeArrary[index]);			
				SendMessage (hwndBufferCombo, CB_ADDSTRING, 0, (LPARAM) szString);				
			}
			
			// Select the first item as default.
			SendMessage (hwndBufferCombo, CB_SETCURSEL, (WPARAM)0, 0);
			CloseHandle(hwndBufferCombo);					
#endif
			return TRUE;
		
		case WM_COMMAND:
		
			if ((HIWORD(wParam) == CBN_SELCHANGE))
			{
#if 0
				DWORD dwBufferSelect;
				DWORD dwResult;

				/// get Value in combo list
				dwResult = (DWORD)SendDlgItemMessage(hDlg, IDC_QLTEST_COMBO1, CB_GETCURSEL, (WPARAM)0 , (LPARAM)0);
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
#else
				wsprintf(szString, TEXT("Combo changed"));
				SendDlgItemMessage(hDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
#endif
				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_QLTEST_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_QLTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);

				return TRUE;
			}
			else
			if ((LOWORD(wParam) == IDC_QLTEST_HDDPERF) )
			{
				if(!bHDDtestRunning)
				{
					bHDDtestRunning = TRUE;
					wsprintf(szString, TEXT("QL HDD Performance....") );
					SendDlgItemMessage(hDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					wsprintf(szString, TEXT("Do not press any button !!") );
					SendDlgItemMessage(hDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
					
					SetDlgItemText(hDlg, IDC_QLTEST_STATIC, TEXT("HDD perf !!!"));
#if 0
					SetCursor(LoadCursor(NULL,IDC_WAIT));	
					mainQLHDDPerf(hDlg);
					UpdateWindow(hDlg);
					SetCursor(LoadCursor(NULL,IDC_SIZEWE));					
#else					
					hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)mainQLHDDPerf, &hDlg, 0, NULL);					
					CloseHandle(hCreateThread);
#endif					
					
					///bHDDtestRunning = FALSE;
				}
				else
				{
					wsprintf(szString, TEXT("Already Testing ...") );
					SendDlgItemMessage(hDlg, IDC_QLTEST_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);					
				}	

				///move the screen to the last message		
				DWORD dwIndex = SendDlgItemMessage(hDlg, IDC_QLTEST_LIST1, LB_GETCOUNT, 0, 0);							
				SendDlgItemMessage(hDlg, IDC_QLTEST_LIST1, LB_SETTOPINDEX, (dwIndex-1), 0);
				
				return TRUE;					
			}
			else
			if ((LOWORD(wParam) == IDC_QLTEST_CLEAR) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_QLTEST_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_QLTEST_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

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
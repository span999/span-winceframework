// HDtest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "HDtest.h"
#include <commctrl.h>

#include <Storemgr.h>
///#include <Formatdisk.h>

#include <fatutil.h>
//#include <diskio.h>


extern "C" DWORD FormatVolume (HANDLE hVolume, PDISK_INFO pdi, PFORMAT_OPTIONS pfo, PFN_PROGRESS pfnProgress, PFN_MESSAGE pfnMessage);
extern "C" DWORD FormatVolumeEx(HANDLE hVolume, PFORMAT_PARAMS pfp);
extern "C" VOID FormatVolumeUI (HANDLE hVolume, HWND hDlg);

/*
typedef struct _FORMAT_OPTIONS {
    DWORD   dwClusSize;        // desired cluster size in bytes (must be power of 2)
    DWORD   dwRootEntries;     // desired number of root directory entries
    DWORD   dwFatVersion;      // FAT Version.  Either 12, 16, or 32    
    DWORD   dwNumFats;         // Number of FATs to create
    DWORD   dwFlags;
} FORMAT_OPTIONS;
typedef FORMAT_OPTIONS  *PFORMAT_OPTIONS;
*/

#define FATUTIL_FULL_FORMAT                 0x1     // perform a full format
#define FATUTIL_FORMAT_TFAT                 0x2    // format volume as TFAT
#define FATUTIL_DISABLE_MOUNT_CHK     0x4    // disable checking to see if volume is mounted before doing operation
#define FATUTIL_SCAN_VERIFY_FIX           0x1   // prompt user to perform a fix

// Flags for Part_FileSystem

#define PART_UNKNOWN            0
#define PART_DOS2_FAT           0x01    // legit DOS partition
#define PART_DOS3_FAT           0x04    // legit DOS partition
#define PART_EXTENDED           0x05    // legit DOS partition
#define PART_DOS4_FAT           0x06    // legit DOS partition
#define PART_DOS32              0x0B    // legit DOS partition (FAT32)
#define PART_DOS32X13           0x0C    // Same as 0x0B only "use LBA"
#define PART_DOSX13             0x0E    // Same as 0x06 only "use LBA"
#define PART_DOSX13X            0x0F    // Same as 0x05 only "use LBA"

#define PART_CE_HIDDEN          0x18

#define PART_BOOTSECTION        0x20
#define PART_BINFS              0x21
#define PART_XIP                0x22

#define PART_INVALID            0xFF    // invalid partition type


#define  ARRAYSIZE(a) (sizeof(a)/ sizeof(a[0]))


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);


// Global Variables:
//HINSTANCE           ghInst;         // The current instance
HWND                ghwndCB;        // The command bar handle
HWND                ghwndMain;      // The top-level window
//BOOL                gfUseStatic = TRUE;
//HANDLE              ghtLaunch;      // launcher thread
//HANDLE              ghevSignal;     // Notifies the launch thread of events
//enum { None, Launch, Interrupt, Exit } geSignal;  // signal types
//HANDLE              ghprDisk;
//HANDLE              ghprBacklight;
BOOL                gfPartionUseUIOption = FALSE;

///
int GetPartitionList(LPCWSTR pszStore, int cMaxPart, PPARTINFO ppi)
{
    int cIndex = 0;
    HANDLE hStore = OpenStore(pszStore);
    if(INVALID_HANDLE_VALUE != hStore)
    {
        ppi[cIndex].cbSize = sizeof(PARTINFO);
        HANDLE hFind = FindFirstPartition(hStore, &ppi[cIndex]);
        if(INVALID_HANDLE_VALUE != hFind)
        {
            do
            {
                cIndex++;
                ppi[cIndex].cbSize = sizeof(PARTINFO);
            }
            while(cIndex < cMaxPart && FindNextPartition(hFind, &ppi[cIndex]));

            FindClose(hFind);
        }

        // set cursor to top item
        CloseHandle(hStore);
    }

    // return the number of partitions
    return cIndex;
}


// this message is a callback from FormatVolume()
BOOL
FormatVolumeMessage(LPTSTR pszMessage, LPTSTR pszCaption, BOOL fYesNo)
{
    ///LogMessage(L"%s: %s", pszCaption, pszMessage);
	NKDbgPrintfW( TEXT("APP:: FormatVolumeMessage():%s: %s.\r\n"), pszCaption, pszMessage );
    return TRUE;
}


void MyFormatPartition(LPCWSTR pszStore, DWORD dwPartition)
{
    WCHAR szMessage[MAX_PATH];
    wsprintf(szMessage, L"This will erase all the data on '%s' partition %d.  Are you sure you want to continue?",
        pszStore, dwPartition);
    ///int idButton = MessageBox(ghwndMain, szMessage, L"Reformat Partition", MB_YESNO | MB_ICONWARNING);
    int idButton = MessageBox(NULL, szMessage, L"Reformat Partition", MB_YESNO | MB_ICONWARNING);
    if(idButton == IDYES) {
        PARTINFO pi[4];
        int nPartitions = GetPartitionList(pszStore, ARRAYSIZE(pi), pi);
        if(nPartitions != 2) {
            ///LogMessage(L"! Store '%s' has %d partitions, expected 2", pszStore, nPartitions);
        }
        if(nPartitions > (int) dwPartition) {
            // dismount the store
            HANDLE hStore = OpenStore(pszStore);
            if(hStore != INVALID_HANDLE_VALUE) {
                BOOL fOk = DismountStore(hStore);
                if(!fOk) {
                    ///LogMessage(L"Can't dismount store '%s'", pszStore);
                } else {
                    HANDLE hPart = OpenPartition(hStore, pi[dwPartition].szPartitionName);
                    if(hPart != INVALID_HANDLE_VALUE) {
                        // determine the partition type
                        BYTE bPartType = pi[dwPartition].bPartType;
                        if(bPartType == PART_UNKNOWN || bPartType == PART_INVALID) {
                            bPartType = PART_DOS32;
                        }

                        // do the format
                        ///LogMessage(L"Formatting partition %d ('%s') on '%s' ", dwPartition, pi[dwPartition].szPartitionName, pszStore);
						///"FormatPartitionEx()" This function formats the partition with the default file system of the loaded partition driver.
                        fOk = FormatPartitionEx(hPart, bPartType, FALSE);
                        if(fOk) {
#if 1
							DWORD dwStatus = ERROR_SUCCESS;
							if( gfPartionUseUIOption )
							{
								NKDbgPrintfW( TEXT("APP:: FormatVolume with UI.\r\n") );
								FormatVolumeUI(hPart, ghwndMain);
								///DWORD dwStatus = ERROR_SUCCESS;
							}
							else
							{
                            	FORMAT_OPTIONS fo;
                            	if(dwPartition == 0) {
                                	///fo.dwClusSize = 1;              // start with sec/cluster
                                	fo.dwClusSize = 4;              // start with sec/cluster
                            	} else {
                                	fo.dwClusSize = 32;             // start with sec/cluster
                            	}
                            	fo.dwClusSize *= 512;               // convert to size in bytes
                            	fo.dwFatVersion = 32;
                            	fo.dwNumFats = 2;
                            	fo.dwRootEntries = 0;               // required to be 0 by FAT32
                            	fo.dwFlags = FATUTIL_FORMAT_TFAT;   // add FATUTIL_FULL_FORMAT to clobber all data sectors
								///"FormatVolume()" This function formats a volume according to the options specified.
                            	///DWORD dwStatus = FormatVolume(hPart, NULL, &fo, NULL, FormatVolumeMessage);
                            	NKDbgPrintfW( TEXT("APP:: FormatVolume without UI.\r\n") );
                            	dwStatus = FormatVolume(hPart, NULL, &fo, NULL, FormatVolumeMessage);
							}
#else
							DWORD dwStatus = ERROR_SUCCESS;
#endif
                            if(dwStatus != ERROR_SUCCESS) {
                                ///LogMessage(L"FormatVolume() failed %d", dwStatus);
                                fOk = FALSE;
                            } else {
                                ///LogMessage(L"Format successful for partition %d", dwPartition);
                            }
                        } else {
                            ///LogMessage(L"FormatPartition() failed %d", GetLastError());
                        }
                        CloseHandle(hPart);
                    }

                    // remount partitions
                    for(int i = 0; i < nPartitions; i++) {
                        if(pi[i].dwAttributes & PARTITION_ATTRIBUTE_MOUNTED) {
                            fOk = FALSE;
                            hPart = OpenPartition(hStore, pi[i].szPartitionName);
                            if(hPart != INVALID_HANDLE_VALUE) {
                                fOk = MountPartition(hPart);
                                CloseHandle(hPart);
                            }
                            if(!fOk) {
                                ///LogMessage(L"! Can't re-mount partition '%s'", pi[i].szPartitionName);
                            }
                        }
                    }
                }
                CloseHandle(hStore);
            }
        }
    }
}

void DoFormatPartition_2()
{
    MyFormatPartition(L"dsk1:", 1);
}

void DoFormatPartition_1()
{
    MyFormatPartition(L"dsk1:", 0);
}

///

int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
	
///	LPTSTR lpTString;
///	lpTString = GetCommandLine();
///	NKDbgPrintfW( TEXT("APP:: lpCmdLine(0x%x) = [%s].\r\n"),lpCmdLine , lpCmdLine );
///	NKDbgPrintfW( TEXT("APP:: GetCommandLine(0x%x) = [%s].\r\n"),lpTString , lpTString );

	if(  0 != wcscmp(lpCmdLine, TEXT("") ) )
	{
	   gfPartionUseUIOption	= FALSE;
	   DoFormatPartition_1();		
	}
	else
	{	
		// Perform application initialization:
		if (!InitInstance (hInstance, nCmdShow)) 
		{
			return FALSE;
		}
    	
		hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_HDTEST);
    	
		// Main message loop:
		while (GetMessage(&msg, NULL, 0, 0)) 
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}	

	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    It is important to call this function so that the application 
//    will get 'well formed' small icons associated with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS	wc;

    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= (WNDPROC) WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HDTEST));
    wc.hCursor			= 0;
    wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName		= 0;
    wc.lpszClassName	= szWindowClass;

	return RegisterClass(&wc);
}

//
//  FUNCTION: InitInstance(HANDLE, int)
//
//  PURPOSE: Saves instance handle and creates main window
//
//  COMMENTS:
//
//    In this function, we save the instance handle in a global variable and
//    create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND	hWnd;
	TCHAR	szTitle[MAX_LOADSTRING];			// The title bar text
	TCHAR	szWindowClass[MAX_LOADSTRING];		// The window class name

	hInst = hInstance;		// Store instance handle in our global variable
	// Initialize global strings
	LoadString(hInstance, IDC_HDTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}
	ghwndMain = hWnd;	///
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	if (hwndCB)
		CommandBar_Show(hwndCB, TRUE);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HANDLE hHDControlEvent = NULL;	///
	HDC hdc;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	TCHAR szHello[MAX_LOADSTRING];

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_HELP_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_FILE_EXIT:
				   DestroyWindow(hWnd);
				   break;
				case IDM_OPTION_HDPOWERDOWN:
				   //HANDLE hHDControlEvent = NULL;
				   hHDControlEvent = CreateEvent(NULL,FALSE,FALSE,TEXT("Application Power Off HD Event"));
				   SetEvent(hHDControlEvent);
				   NKDbgPrintfW( TEXT("APP:: Application Power Off HD Event.\r\n") );
				   CloseHandle(hHDControlEvent);
				   break;
				case IDM_OPTION_HDPOWERUP:
				   //HANDLE hHDControlEvent = NULL;
				   hHDControlEvent = CreateEvent(NULL,FALSE,FALSE,TEXT("Application Power On HD Event"));
				   SetEvent(hHDControlEvent);
				   NKDbgPrintfW( TEXT("APP:: Application Power On HD Event.\r\n") );
				   CloseHandle(hHDControlEvent);
				   break;
				case IDM_OPTION_HDFORMATPARTITION_UI:
/*
				   if( gfPartionUseUIOption )
				   {	///un-check it
					   gfPartionUseUIOption = FALSE;
					   CheckMenuItem( (HMENU)ghwndCB, IDM_OPTION_HDFORMATPARTITION_UI, MF_UNCHECKED);
				   }
				   else
				   {	///check it
					   gfPartionUseUIOption	= TRUE;					   
					   CheckMenuItem( (HMENU)ghwndCB, IDM_OPTION_HDFORMATPARTITION_UI, MF_CHECKED);
				   }
*/
				   break;   
				case IDM_OPTION_HDFORMATPARTITION_1:				 
				   gfPartionUseUIOption	= FALSE;
				   DoFormatPartition_1();
				   break;   
				case IDM_OPTION_HDFORMATPARTITION_2:				 
				   gfPartionUseUIOption	= FALSE;
				   DoFormatPartition_2();				 
				   break;   
				case IDM_OPTION_HDFORMATPARTITION_1_UI:
				   gfPartionUseUIOption	= TRUE;	
				   DoFormatPartition_1();
				   break;   
				case IDM_OPTION_HDFORMATPARTITION_2_UI:
				   gfPartionUseUIOption	= TRUE;
				   DoFormatPartition_2();				 
				   break;   
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_CREATE:
			hwndCB = CommandBar_Create(hInst, hWnd, 1);			
			ghwndCB = hwndCB;
			CommandBar_InsertMenubar(hwndCB, hInst, IDM_MENU, 0);
			CommandBar_AddAdornments(hwndCB, 0, 0);
			break;
		case WM_PAINT:
			RECT rt;
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rt);
			LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
			DrawText(hdc, szHello, _tcslen(szHello), &rt, 
				DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			CommandBar_Destroy(hwndCB);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for the About box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;

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
			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

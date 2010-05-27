// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
///#include <winuserm.h>
#include <commctrl.h>
#include "resource.h"
#include "TOOLHELP_Dlg.h"
#include "Tlhelp32.h"

static HWND ghDlg = NULL;
static TCHAR szString[64];         	// Temperary string
static HINSTANCE ghInstance = NULL;
static BOOL g_bWinHide = FALSE;
static DWORD ProcessIDArrary[256];
static DWORD ProcessIDIndex = 0;

typedef HANDLE (*PFN_CreateToolhelp32Snapshot)(DWORD dwFlags, DWORD th32processID);
static PFN_CreateToolhelp32Snapshot gpfCreateToolhelp32Snapshot;

typedef BOOL (*PFN_CloseToolhelp32Snapshot)(HANDLE hSnapshot);
static PFN_CloseToolhelp32Snapshot gpfCloseToolhelp32Snapshot;

typedef BOOL (*PFN_Process32First)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
static PFN_Process32First gpfProcess32First;

typedef BOOL (*PFN_Process32Next)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
static PFN_Process32Next gpfProcess32Next;

typedef BOOL (*PFN_Module32First)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
static PFN_Module32First gpfModule32First;

typedef BOOL (*PFN_Module32Next)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
static PFN_Module32Next gpfModule32Next;

typedef BOOL (*PFN_Thread32First)(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
static PFN_Thread32First gpfThread32First;

typedef BOOL (*PFN_Thread32Next)(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
static PFN_Thread32Next gpfThread32Next;

///typedef DWORD (*PFN_GetIdleTime)(void);
///static PFN_GetIdleTime gpfGetIdleTime;

///prototype
DWORD spToolhelpDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK spToolhelp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static BOOL LoadMyLib(void);
static DWORD DumpAllProcess(HANDLE hSnapShot);
static DWORD DumpAllModule(HANDLE hSnapShot);
static DWORD DumpAllThread(HANDLE hSnapShot);
static void DumpProcessContent(PROCESSENTRY32 *pPe32);
static void DumpModuleContent(MODULEENTRY32 *pMe32);
static void DumpThreadContent(THREADENTRY32 *pTe32);


DWORD spToolhelpDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	ghInstance = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_TOOLHELPBOX, hWnd, (DLGPROC)spToolhelp);
	
	return dwRet;
}





BOOL LoadMyLib(void)
{
    HMODULE hCore;	
    BOOL bRet = FALSE;
    DWORD dwFail = 0;
    
    /// init system function call
    hCore = (HMODULE)LoadLibrary( L"toolhelp.dll" );
    
    if ( hCore ) {
        gpfCreateToolhelp32Snapshot = (PFN_CreateToolhelp32Snapshot)GetProcAddress( hCore, L"CreateToolhelp32Snapshot" );
        if ( !gpfCreateToolhelp32Snapshot )
            dwFail &= 1;

        gpfCloseToolhelp32Snapshot = (PFN_CloseToolhelp32Snapshot)GetProcAddress( hCore, L"CloseToolhelp32Snapshot" );
        if ( !gpfCloseToolhelp32Snapshot )
            dwFail &= 1;

        gpfProcess32First = (PFN_Process32First)GetProcAddress( hCore, L"Process32First" );
        if ( !gpfProcess32First )
            dwFail &= 1;

        gpfProcess32Next = (PFN_Process32Next)GetProcAddress( hCore, L"Process32Next" );
        if ( !gpfProcess32Next )
            dwFail &= 1;

        gpfModule32First = (PFN_Module32First)GetProcAddress( hCore, L"Module32First" );
        if ( !gpfModule32First )
            dwFail &= 1;

        gpfModule32Next = (PFN_Module32Next)GetProcAddress( hCore, L"Module32Next" );
        if ( !gpfModule32Next )
            dwFail &= 1;

        gpfThread32First = (PFN_Thread32First)GetProcAddress( hCore, L"Thread32First" );
        if ( !gpfThread32First )
            dwFail &= 1;

        gpfThread32Next = (PFN_Thread32Next)GetProcAddress( hCore, L"Thread32Next" );
        if ( !gpfThread32Next )
            dwFail &= 1;

    }
    else
    	dwFail &= 1;
    
    if( dwFail != 0 )
    {	
    	FreeLibrary(hCore);
    	bRet = FALSE;
    }
    else
    	bRet = TRUE;
    	
    return bRet;	
}





// Message handler for the TOOLHELP box.
LRESULT CALLBACK spToolhelp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	
	///HANDLE hCreateThread = NULL;
	///TCHAR szString[64];         	// Temperary string
	HWND hwndBufferCombo = NULL;    // Handle to the combobox control
	UINT index = 0;
	
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;

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

			for(index = 0; index < 256; index++)
				ProcessIDArrary[index] = 0;
			
			///hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CPUtilization, NULL, 0, NULL);					
			///CloseHandle(hCreateThread);	

			return TRUE;

		case WM_COMMAND:
		
			if ((LOWORD(wParam) == IDC_TOOLHELP_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_TOOLHELP_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_TOOLHELP_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			else			
			if ((LOWORD(wParam) == IDC_TOOLHELP_SNAPSHOT) )
			{
				DWORD dwProcessCount;
				///create snapshot
				hSnapShot = gpfCreateToolhelp32Snapshot(
													///TH32CS_SNAPALL
													TH32CS_SNAPPROCESS|TH32CS_SNAPTHREAD|TH32CS_GETALLMODS,
													0
													);
				if( hSnapShot == INVALID_HANDLE_VALUE )
					return FALSE;		
				
				dwProcessCount = DumpAllProcess(hSnapShot);
				
				wsprintf(szString, TEXT("%d Process"), dwProcessCount);
				SetDlgItemText(hDlg, IDC_TOOLHELP_STATIC1, szString);
				
				///close snapshut
				gpfCloseToolhelp32Snapshot(hSnapShot);
					
				return TRUE;
			}				
			else			
			if ((LOWORD(wParam) == IDC_TOOLHELP_SNAPSHOT2) )
			{
				DWORD dwSpecifiedProcessID;
				DWORD dwModuleCount;
				
				if( ProcessIDArrary[ProcessIDIndex] != 0 )
				{	
					dwSpecifiedProcessID = ProcessIDArrary[ProcessIDIndex];
					ProcessIDIndex++;
				}
				else
				{
					ProcessIDIndex = 0;
					dwSpecifiedProcessID = ProcessIDArrary[ProcessIDIndex];
					ProcessIDIndex++;				
				}		

				wsprintf(szString, TEXT("0x%08x"), dwSpecifiedProcessID);
				SetDlgItemText(hDlg, IDC_TOOLHELP_STATIC1, szString);
				
				///create snapshot
				hSnapShot = gpfCreateToolhelp32Snapshot(
													TH32CS_SNAPALL,
													///TH32CS_SNAPPROCESS|TH32CS_SNAPTHREAD|TH32CS_GETALLMODS,
													dwSpecifiedProcessID
													);
				if( hSnapShot == INVALID_HANDLE_VALUE )
					return FALSE;		
				
				dwModuleCount = DumpAllModule(hSnapShot);

				wsprintf(szString, TEXT("%d Module"), dwModuleCount);
				SetDlgItemText(hDlg, IDC_TOOLHELP_STATIC2, szString);
				
				///close snapshut
				gpfCloseToolhelp32Snapshot(hSnapShot);
					
				return TRUE;
			}				
			else			
			if ((LOWORD(wParam) == IDC_TOOLHELP_SNAPSHOT3) )
			{
				DWORD dwThreadCount;
				
				///create snapshot
				hSnapShot = gpfCreateToolhelp32Snapshot(
													///TH32CS_SNAPALL
													TH32CS_SNAPPROCESS|TH32CS_SNAPTHREAD|TH32CS_GETALLMODS,
													0
													);
				if( hSnapShot == INVALID_HANDLE_VALUE )
					return FALSE;		
				
				dwThreadCount = DumpAllThread(hSnapShot);

				wsprintf(szString, TEXT("%d Thread"), dwThreadCount);
				SetDlgItemText(hDlg, IDC_TOOLHELP_STATIC3, szString);

				
				///close snapshut
				gpfCloseToolhelp32Snapshot(hSnapShot);
					
				return TRUE;
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



static DWORD DumpAllProcess(HANDLE hSnapShot)
{
	PROCESSENTRY32 pe32;
	BOOL bRet;
	DWORD ProcessIDIdx = 0;
	DWORD ProcessCount = 0;
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	pe32.cntUsage = 1;
	pe32.th32ModuleID = 0;
	pe32.th32ParentProcessID = 0;
	bRet = gpfProcess32First(hSnapShot, &pe32);
	
	if( bRet )
	{
		DumpProcessContent(&pe32);
		///save to arrary
		ProcessIDArrary[ProcessIDIdx] = pe32.th32ProcessID;
		ProcessIDIdx++;
		ProcessCount++;
	}	
	else
		return ProcessCount;
		
	while( bRet )
	{	
		pe32.dwSize = sizeof(PROCESSENTRY32);
		pe32.cntUsage = 1;
		pe32.th32ModuleID = 0;
		pe32.th32ParentProcessID = 0;
		bRet = gpfProcess32Next(hSnapShot, &pe32);
		
		if( bRet )
		{	
			DumpProcessContent(&pe32);
			///save to arrary
			ProcessIDArrary[ProcessIDIdx] = pe32.th32ProcessID;
			ProcessIDIdx++;
			ProcessCount++;
		}	
	}	
	
	
	return ProcessCount;
}


static DWORD DumpAllModule(HANDLE hSnapShot)
{
	MODULEENTRY32 me32;
	BOOL bRet;
	DWORD ModuleCount = 0;
	
	me32.dwSize = sizeof(MODULEENTRY32);
	bRet = gpfModule32First(hSnapShot, &me32);
	
	if( bRet )
	{	
		DumpModuleContent(&me32);
		ModuleCount++;
	}	
	else
		return ModuleCount;
		
	while( bRet )
	{	
		me32.dwSize = sizeof(MODULEENTRY32);
		bRet = gpfModule32Next(hSnapShot, &me32);
		
		if( bRet )
		{	
			DumpModuleContent(&me32);	
			ModuleCount++;
		}	
	}	
	
	return ModuleCount;
}


static DWORD DumpAllThread(HANDLE hSnapShot)
{
	THREADENTRY32 te32;
	BOOL bRet;
	DWORD ThreadCount = 0;
	
	te32.dwSize = sizeof(THREADENTRY32);
	bRet = gpfThread32First(hSnapShot, &te32);
	
	if( bRet )
	{	
		DumpThreadContent(&te32);
		ThreadCount++;
	}
	else
		return ThreadCount;
		
	while( bRet )
	{	
		te32.dwSize = sizeof(THREADENTRY32);
		bRet = gpfThread32Next(hSnapShot, &te32);
		
		if( bRet )
		{	
			DumpThreadContent(&te32);	
			ThreadCount++;
		}	
	}	
	
	
	return ThreadCount;
}


static void DumpProcessContent(PROCESSENTRY32 *pPe32)
{
	wsprintf(szString, TEXT("ProcessID: 0x%08x%"), pPe32->th32ProcessID);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("Process Name: %s"), pPe32->szExeFile);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("MemoryBase: 0x%08x"), pPe32->th32MemoryBase);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("Thread #s: %03d"), pPe32->cntThreads);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("-----------------------------"));
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	
}


static void DumpModuleContent(MODULEENTRY32 *pMe32)
{
	wsprintf(szString, TEXT("ModuleID: 0x%08x%"), pMe32->th32ModuleID);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("Module Name: %s"), pMe32->szModule);	
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("ProcessID: 0x%08x%"), pMe32->th32ProcessID);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);	
	wsprintf(szString, TEXT("MemoryBase: 0x%08x"), pMe32->modBaseAddr);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("MemorySize: %08d KB"), pMe32->modBaseSize/1024);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("-----------------------------"));
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	
}


static void DumpThreadContent(THREADENTRY32 *pTe32)
{
	wsprintf(szString, TEXT("ThreadID: 0x%08x%"), pTe32->th32ThreadID);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("ProcessID: 0x%08x%"), pTe32->th32CurrentProcessID);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("Thread priority: %03d"), pTe32->tpBasePri);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("Thread priority change: %03d"), pTe32->tpDeltaPri);
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	wsprintf(szString, TEXT("-----------------------------"));
	SendDlgItemMessage(ghDlg, IDC_TOOLHELP_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
	
}

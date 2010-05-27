// GPIOview.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include <windowsx.h>
#include <aygshell.h>
#include "resource.h"
#include "gpio.h"
#include <CEDDK.h>

/*
///#include <Afxwin.h>			///span0517 , add ListBox
#include <atlbase.h>
#include "atlapp.h"
extern CAppModule _Module;
#include <atlwin.h>
#include "atlctrls.h"
*/


#define			sp_DBGMSG	NKDbgPrintfW
//#define			spBKL_DBGMSG	(void)



#define CACHED_TO_UNCACHED_OFFSET       0x20000000
#define PERIF_BASE_PHYSICAL         0x40000000
#define PERIF_BASE_C_VIRTUAL        0x86200000  
#define PERIF_BASE_U_VIRTUAL        (PERIF_BASE_C_VIRTUAL+CACHED_TO_UNCACHED_OFFSET)
#define GPIO_OFFSET                   0x00E00000      // GPIO

#define GPIO_BASE_PHYSICAL              (PERIF_BASE_PHYSICAL + GPIO_OFFSET)
#define GPIO_BASE_C_VIRTUAL             (PERIF_BASE_C_VIRTUAL + GPIO_OFFSET)
#define GPIO_BASE_U_VIRTUAL             (PERIF_BASE_U_VIRTUAL + GPIO_OFFSET)

#define ALLOC_SIZE sizeof(GPIO_REGS)

extern "C" BOOL VirtualCopy(LPVOID lpvDest, LPVOID lpvSrc, DWORD cbSize, DWORD fdwProtect);
extern "C" LPVOID AllocPhysMem(DWORD cbSize, DWORD fdwProtect, DWORD dwAlignmentMask, DWORD dwFlags, PULONG pPhysicalAddress);
extern "C" BOOL VirtualFree(LPVOID lpAddress, DWORD dwSize, DWORD dwFreeType ); 

/*===========================================================================
 * Function Prototypes 
*/
BOOL CALLBACK DialogProc(const HWND hDlg, const UINT uiMessage, 
    const WPARAM wParam, const LPARAM lParam);
static LPCTSTR StringFromResources(UINT uStringID);
int WINAPI DialogMain(HWND hwnd);
static LRESULT OnCommandDo(HWND hwnd);



HINSTANCE g_hInst = NULL;  // Local copy of hInstance

#define ARRAYSIZE(a)   (sizeof(a)/sizeof(*a))

const TCHAR* g_szAppWndClass = TEXT("HelloApp");

TCHAR g_szMessage[30];

static volatile GPIO_REGS *pGPIOReg = NULL;            // pointer to GPIO registers

/**************************************************************************************

   OnCreate

 **************************************************************************************/
LRESULT OnCreate(
    HWND hwnd,
    CREATESTRUCT* lParam
    )
{

    // create the menu bar
    SHMENUBARINFO mbi;
    ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
    mbi.cbSize = sizeof(SHMENUBARINFO);
    mbi.hwndParent = hwnd;
    mbi.nToolBarId = IDR_HELLO_MENUBAR;
    mbi.hInstRes = g_hInst;

    if(!SHCreateMenuBar(&mbi))
    {
        // Couldn't create the menu bar.  Fail creation of the window.
        return(-1);
    }


    // Get our message text.
    if(0 == LoadString(g_hInst, IDS_HELLO_MESSAGE, g_szMessage, ARRAYSIZE(g_szMessage)))
    {
        // Couldn't load the string.  Fail creation of the window.
        return(-1);
    }

    // Do other window creation related things here.
	
	/// span0517, add ListBox
	/*
	LPDRAWITEMSTRUCT lpdrawitemstruct; 
	CListBox mylistbox1;	/// create it
	mylistbox1.AddString(_T("Disabled"));
	mylistbox1.AddString(_T("Once"));
	mylistbox1.AddString(_T("Every day"));
	mylistbox1.SetCurSel(-1);					/// no selection

	lpdrawitemstruct->CtlType = ODT_LISTBOX;
	lpdrawitemstruct->itemState = ODS_DEFAULT;
	lpdrawitemstruct->hwndItem = hwnd;
	mylistbox1.DrawItem(lpdrawitemstruct);
	*/
	/// span0517, add ListBox

    return(0); // continue creation of the window
}


/**************************************************************************************

   OnCreate

 **************************************************************************************/
LRESULT OnCommandDo(
    HWND hwnd
    )
{
	int iRet = -1;
	sp_DBGMSG( L"##### OnCommandDo(). #####\r\n" );
	iRet = DialogMain(hwnd);

    return iRet; // continue creation of the window
}



/**************************************************************************************

   WndProc

 **************************************************************************************/
LRESULT CALLBACK WndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wp,
    LPARAM lp
    )
{
    LRESULT lResult = TRUE;

    switch(msg)
    {
        case WM_CREATE:
			sp_DBGMSG( L"##### WM_CREATE. #####\r\n" );
            lResult = OnCreate(hwnd, (CREATESTRUCT*)lp);
            break;

        case WM_COMMAND:
            switch (wp)
            {
                case IDOK:
					sp_DBGMSG( L"##### WM_COMMAND IDOK. #####\r\n" );
                    DestroyWindow(hwnd);
                    break;
                case ID_HELLO_DO:
                    ///DestroyWindow(hwnd);
					///DialogMain();
					sp_DBGMSG( L"##### WM_COMMAND IDDO. #####\r\n" );
					lResult = OnCommandDo(hwnd);
                    break;
                default:
                    goto DoDefault;
            }
            break;

        case WM_PAINT:
            {
                HDC hdc;
                PAINTSTRUCT ps;
                RECT rect;

                hdc = BeginPaint(hwnd, &ps);
                GetClientRect(hwnd, &rect);

				sp_DBGMSG( L"##### WM_PAINT. #####\r\n" );
                DrawText(hdc, g_szMessage, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                EndPaint (hwnd, &ps);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        DoDefault:
        default:
            lResult = DefWindowProc(hwnd, msg, wp, lp);
            break;
    }

    return(lResult);
}

/****************************************************************************

   ActivatePreviousInstance

  ****************************************************************************/
HRESULT ActivatePreviousInstance(
    const TCHAR* pszClass,
    const TCHAR* pszTitle,
    BOOL* pfActivated
    )
{
    HRESULT hr = S_OK;
    int cTries;
    HANDLE hMutex = NULL;

    *pfActivated = FALSE;
    cTries = 5;
    while(cTries > 0)
    {
        hMutex = CreateMutex(NULL, FALSE, pszClass); // NOTE: We don't want to own the object.
        if(NULL == hMutex)
        {
            // Something bad happened, fail.
            hr = E_FAIL;
            goto Exit;
        }

        if(GetLastError() == ERROR_ALREADY_EXISTS)
        {
            HWND hwnd;

            CloseHandle(hMutex);
            hMutex = NULL;

            // There is already an instance of this app
            // running.  Try to bring it to the foreground.

            hwnd = FindWindow(pszClass, pszTitle);
            if(NULL == hwnd)
            {
                // It's possible that the other window is in the process of being created...
                Sleep(500);
                hwnd = FindWindow(pszClass, pszTitle);
            }

            if(NULL != hwnd) 
            {
                // Set the previous instance as the foreground window

                // The "| 0x01" in the code below activates
                // the correct owned window of the
                // previous instance's main window.
                SetForegroundWindow((HWND) (((ULONG) hwnd) | 0x01));

                // We are done.
                *pfActivated = TRUE;
                break;
            }

            // It's possible that the instance we found isn't coming up,
            // but rather is going down.  Try again.
            cTries--;
        }
        else
        {
            // We were the first one to create the mutex
            // so that makes us the main instance.  'leak'
            // the mutex in this function so it gets cleaned
            // up by the OS when this instance exits.
            break;
        }
    }

    if(cTries <= 0)
    {
        // Someone else owns the mutex but we cannot find
        // their main window to activate.
        hr = E_FAIL;
        goto Exit;
    }

Exit:
    return(hr);
}


/*****************************************************************************

  WinMain

  ***************************************************************************/

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow
    )
{
    MSG msg;
    HWND hwnd = NULL;
    BOOL fActivated;
    WNDCLASS wc;
    HWND hwndMain;
    TCHAR szAppTitle[20];

    g_hInst = hInstance;

    if(0 == LoadString(g_hInst, IDS_HELLO_TITLE, szAppTitle, ARRAYSIZE(szAppTitle)))
    {
        return(0);
    }

    if(FAILED(ActivatePreviousInstance(g_szAppWndClass, szAppTitle, &fActivated)) ||
            fActivated)
    {
        return(0);
    }

    // Register our main window's class.
    ZeroMemory(&wc, sizeof(wc));
    wc.style = CS_HREDRAW | CS_VREDRAW ;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = NULL;
    wc.hInstance = g_hInst;
    wc.hCursor = NULL;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szAppWndClass;
    if(!RegisterClass(&wc))
    {
        return(0);
    }

    // Create the main window.    
    hwndMain = CreateWindow(g_szAppWndClass, szAppTitle,
            WS_CLIPCHILDREN, // Setting this to 0 gives a default style we don't want.  Use a benign style bit instead.
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, g_hInst, NULL );
    if(!hwndMain)
    {
        return(0);
    }




    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);

    // Pump messages until a PostQuitMessage.
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

// end GPIOview.cpp



BOOL CALLBACK DialogProc
(
    const HWND		hDlg, 
    const UINT		uiMessage, 
    const WPARAM	wParam, 
    const LPARAM	/*lParam*/
)
{
    BOOL    bProcessedMsg = TRUE;
	int		iLoop;
///    DWORD   dwDisposition;
///    DWORD   dwDWORD1;
///    int     iResult;
///    int     iLength;
///    LPTSTR  lpszString1;
///    HWND    hwndStr;

	sp_DBGMSG( L"##### DialogProc():uiMessage:%x,wParam:%x#####\r\n",uiMessage,wParam );

    switch(uiMessage)
    {
        case WM_INITDIALOG:
/*
            if (!InitWindow(hDlg, IDR_REGDEMO_MAINMENU)) 
            {
                EndDialog(hDlg, -1);
            }
*/
			SHINITDLGINFO shidi;
			ZeroMemory(&shidi, sizeof(shidi));
			shidi.dwMask	= SHIDIM_FLAGS;
			shidi.dwFlags	= SHIDIF_SIZEDLGFULLSCREEN;
			shidi.hDlg		= hDlg;

			SHInitDialog(&shidi);

			// create the menu bar
			SHMENUBARINFO mbi;
			ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
			mbi.cbSize = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hDlg;
			mbi.nToolBarId = IDR_DIALOG_MENUBAR_1;
			mbi.hInstRes = g_hInst;
			if(!SHCreateMenuBar(&mbi))
			{
				// Couldn't create the menu bar.  Fail creation of the window.
				return(-1);
			}


///			pGPIOReg = (volatile GPIO_REGS *)VirtualAllocCopy(1, "PMU::GPIO", (PVOID)(GPIO_BASE_U_VIRTUAL));
			if ( NULL == pGPIOReg )
			{
///				pGPIOReg = (volatile GPIO_REGS *)MmMapIoSpace( (PHYSICAL_ADDRESS)(GPIO_BASE_U_VIRTUAL), 120, FALSE );

				pGPIOReg = (volatile GPIO_REGS *)VirtualAlloc(0, ALLOC_SIZE, MEM_RESERVE, PAGE_NOACCESS);
				if ( FALSE == VirtualCopy( (LPVOID)pGPIOReg, (LPVOID)GPIO_BASE_U_VIRTUAL, (DWORD)ALLOC_SIZE, PAGE_READWRITE | PAGE_NOCACHE) )
				{
					sp_DBGMSG( L"VirtualCopy failed.\r\n" );
					VirtualFree( (LPVOID)pGPIOReg, 0, MEM_RELEASE);
					pGPIOReg = NULL;
				}

///				DWORD dwPhyAddress = GPIO_BASE_U_VIRTUAL;
///				pGPIOReg = (volatile GPIO_REGS *)AllocPhysMem(1000, PAGE_READWRITE | PAGE_NOCACHE, 0, 0, &dwPhyAddress);
			}




            SetWindowText(hDlg, TEXT("Test Dialog"));
            SendDlgItemMessage(hDlg, IDC_EDIT1, EM_LIMITTEXT, 3, 0);
            SendDlgItemMessage(hDlg, IDC_EDIT2, EM_LIMITTEXT, 3, 0);

            ///SetDlgItemInt(hDlg, IDC_EDIT1, (UINT) 88, FALSE);
            ///SetDlgItemText(hDlg, IDC_EDIT2, TEXT("Fuck") );
            SetDlgItemText(hDlg, IDC_EDIT1, TEXT("In/Output") );
            SetDlgItemText(hDlg, IDC_EDIT2, TEXT("High/Low") );

			

			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_00") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_01") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_02") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_03") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_04") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_05") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_06") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_07") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_08") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_09") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_11") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_12") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_13") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_14") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_15") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_16") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_17") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_18") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_19") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("GPIO_20") );

			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("first") );
			SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_ADDSTRING, 0, (LPARAM)TEXT("second") );


/*
            // try to open the registry key
            if (RegCreateKeyEx(REGISTRY_ROOT, REGISTRY_KEY, (DWORD) 0, TEXT(""), 
                    (DWORD) 0, (REGSAM) 0, (LPSECURITY_ATTRIBUTES) NULL, 
                    &g_hkResult, &dwDisposition) != ERROR_SUCCESS) 
            {
                MessageBox(NULL, StringFromResources(IDS_REGCREATEKEYEX_FAILED), 
                    StringFromResources(IDS_ERROR_TITLE), MB_OK);
                EndDialog(hDlg, -1);
            }

            // get the keys, if they exist
            if (REG_OPENED_EXISTING_KEY == dwDisposition) 
            {
                // try to set the DWORD
                if (!GetDWORD1(hDlg)) 
                {
                    MessageBox(NULL, StringFromResources(IDS_GETDWORD1_FAILED), 
                        StringFromResources(IDS_ERROR_TITLE), MB_OK);
                    RegCloseKey(g_hkResult);
                    EndDialog(hDlg, -1);
                }

                // try to set the String
                if (!GetString1(hDlg)) 
                {
                    MessageBox(NULL, StringFromResources(IDS_GETSTRING1_FAILED), 
                        StringFromResources(IDS_ERROR_TITLE), MB_OK);
                    RegCloseKey(g_hkResult);
                    EndDialog(hDlg, -1);
                }
            }
*/
			break;

        case 0x134:

			if ( NULL == pGPIOReg )
			{
				pGPIOReg = (volatile GPIO_REGS *)VirtualAlloc(0, ALLOC_SIZE, MEM_RESERVE, PAGE_NOACCESS);
				if ( FALSE == VirtualCopy( (LPVOID)pGPIOReg, (LPVOID)(GPIO_BASE_U_VIRTUAL/256), (DWORD)ALLOC_SIZE, PAGE_READWRITE | PAGE_NOCACHE | PAGE_PHYSICAL) )
				{
					sp_DBGMSG( L"VirtualCopy failed.\r\n" );
					VirtualFree( (LPVOID)pGPIOReg, 0, MEM_RELEASE);
					pGPIOReg = NULL;
				}
			}


			sp_DBGMSG( L"##### 0x134. #####\r\n" );
			/// ListBox just changed...
			LONG lTemp;
			
			/// get total item count
			lTemp = SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_GETCOUNT, 0, 0 );
			sp_DBGMSG( L"##### 0x134. ##### LB_GETCOUNT:%d.\r\n", lTemp );
			/// get current selection, -1 for none, 0 for #0, start with count 0
			lTemp = SendDlgItemMessage(hDlg, IDL_LISTBOX1, LB_GETCURSEL, 0, 0 );
			sp_DBGMSG( L"##### 0x134. ##### LB_GETCURSEL:%d.\r\n", lTemp );

			if ( NULL != pGPIOReg )
			{
				sp_DBGMSG( L"pGPIOReg:%.8x.\r\n", pGPIOReg );
				if ( lTemp >= 0 && lTemp < 32 )
				{
					if ( 0 == (pGPIOReg->GPDR_x & (0x01 << lTemp)) )
						SetDlgItemText(hDlg, IDC_EDIT1, TEXT("Input pin") );
					else
						SetDlgItemText(hDlg, IDC_EDIT1, TEXT("Output pin") );
				}
				else if ( lTemp < 64 )
				{
					///pGPIOReg->GPDR_y
				}
				else if ( lTemp < 89 )
				{
					///pGPIOReg->GPDR_z
				}

				///SetDlgItemText(hDlg, IDC_EDIT1, TEXT("In/Output") );
				///SetDlgItemText(hDlg, IDC_EDIT2, TEXT("High/Low") );
			}

			break;

        case WM_COMMAND:
			sp_DBGMSG( L"##### WM_COMMAND. #####\r\n" );
            switch (wParam) 
            {
				case ID_DIALOG1_OK:
					sp_DBGMSG( L"##### WM_COMMAND ID_DIALOG1_OK. #####\r\n" );
					EndDialog(hDlg,0);
                    ///DestroyWindow(hDlg);
                    break;
				case IDCANCEL:
					sp_DBGMSG( L"##### WM_COMMAND IDCANCEL. #####\r\n" );
					EndDialog(hDlg,0);
                    break;

				case 0x59c43:
					sp_DBGMSG( L"##### WM_COMMAND 0x59c43. #####\r\n" );
					EndDialog(hDlg,0);
                    break;

/*
                case IDM_REGDEMO_UPDATE:
                    // Update the DWORD1 value
                    dwDWORD1 = GetDlgItemInt(hDlg, IDC_REGDEMO_DWORD1, NULL, FALSE);
                    iResult = RegSetValueEx(g_hkResult, REGISTRY_VALUE_DWORD1, (DWORD) 0,
                        REG_DWORD, (PBYTE) &dwDWORD1, sizeof(DWORD));

                    // Update the String1 value
                    hwndStr = GetDlgItem(hDlg, IDC_REGDEMO_STRING1);
					VERIFY(hwndStr != NULL);
                    iLength = GetWindowTextLength(hwndStr) + 1;
					lpszString1 = (LPTSTR) malloc(iLength * sizeof(TCHAR));
                    if (lpszString1 == NULL) 
					{
                        MessageBox(NULL, StringFromResources(IDS_MALLOC_FAILED), 
                            StringFromResources(IDS_ERROR_TITLE), MB_OK);
                        RegCloseKey(g_hkResult);
                        EndDialog(hDlg, 0);
                    }

                    GetDlgItemText(hDlg, IDC_REGDEMO_STRING1, lpszString1, iLength);
                    iResult = RegSetValueEx(g_hkResult, REGISTRY_VALUE_STRING1, (DWORD) 0,
						REG_SZ, (PBYTE) lpszString1, iLength * sizeof(TCHAR));
                    break;

                case IDM_REGDEMO_REMOVE:
                    iResult = RegDeleteValue(g_hkResult, REGISTRY_VALUE_DWORD1);
                    if (!((iResult == ERROR_SUCCESS) || 
                        (iResult == ERROR_INVALID_PARAMETER))) 
					{
                        MessageBox(NULL, StringFromResources(IDS_REGDELETEVALUE_FAILED), 
                                   StringFromResources(IDS_ERROR_TITLE), MB_OK);
                        RegCloseKey(g_hkResult);
                        EndDialog(hDlg, 0);
                    }
                    
                    iResult = RegDeleteValue(g_hkResult, REGISTRY_VALUE_STRING1);
                    if (!((iResult == ERROR_SUCCESS) || 
                        (iResult == ERROR_INVALID_PARAMETER))) 
					{
                        MessageBox(NULL, StringFromResources(IDS_REGDELETEVALUE_FAILED), 
                                   StringFromResources(IDS_ERROR_TITLE), MB_OK);
                        RegCloseKey(g_hkResult);
                        EndDialog(hDlg, 0);
                    }

                    // set the keys back to the default values
                    VERIFY(SetDlgItemInt(hDlg, IDC_REGDEMO_DWORD1, 0, 0));
                    VERIFY(SetDlgItemText(hDlg, IDC_REGDEMO_STRING1, TEXT("")));
                    break;
            
                case IDM_REGDEMO_QUIT:
                    RegCloseKey(g_hkResult);
                    EndDialog(hDlg, 0);
                    break;
*/
                default:
                    bProcessedMsg = FALSE;
                    break;
            }
        
        default:
            bProcessedMsg = FALSE;
            break;
    }

    return bProcessedMsg;
}


int WINAPI DialogMain(HWND hwnd)
{

	sp_DBGMSG( L"##### DialogMain(). #####\r\n" );

	int iRet = 0;
	int iResult = 0;

    // Use a globally named mutex to detect another instance of RegDemo
    const HANDLE hMutex = CreateMutex(0, 0, TEXT("_REGDEMO_EXE_MUTEX_"));

    // check the result code
    if (0 != hMutex) 
    {
        // No other instances running? Okay to proceed...
        if (ERROR_ALREADY_EXISTS != GetLastError()) 
        {
			sp_DBGMSG( L"##### DialogMain()::No other instances running. #####\r\n" );
            InitCommonControls();

            iResult = DialogBox(g_hInst, MAKEINTRESOURCE(IDD_PROPPAGE_LARGE), hwnd, (DLGPROC)DialogProc);

            if (iResult < 0) 
            {
				iRet = -1;
				sp_DBGMSG( L"##### DialogMain()::DialogBox fail. #####\r\n" );
                MessageBox(NULL, TEXT("DialogMain()\r\nDialogBox()\r\nFail !!"), TEXT("Error ..."), MB_OK | MB_ICONERROR);
/*
                MessageBox(NULL, StringFromResources(IDS_DIALOG_ERROR),
                    StringFromResources(IDS_ERROR_TITLE), MB_OK | MB_ICONERROR);
*/
            }

        }
        else 
        {
			sp_DBGMSG( L"##### DialogMain()::Already an instance running. #####\r\n" );
/*
            // Already an instance running - attempt to switch to it and then exit
            LPCTSTR pszTitle = StringFromResources(IDS_REGDEMO_TITLE);
            const HWND hwndExistingInstance = FindWindow(TEXT("Dialog"), pszTitle);
            if (hwndExistingInstance != NULL) 
            {
                SetForegroundWindow((HWND)(((ULONG)hwndExistingInstance) | 0x1));
            }
*/
			iRet = 0;
        }
        VERIFY(CloseHandle(hMutex));
    }

/*
    // create the menu bar
    SHMENUBARINFO mbi;
    ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
    mbi.cbSize = sizeof(SHMENUBARINFO);
    mbi.hwndParent = hwnd;
    mbi.nToolBarId = IDR_HELLO_MENUBAR;
    mbi.hInstRes = g_hInst;
    if(!SHCreateMenuBar(&mbi))
    {
        // Couldn't create the menu bar.  Fail creation of the window.
        return(-1);
    }
*/
/*
	// Specify that the dialog box should stretch full screen
	SHINITDLGINFO shidi;
	ZeroMemory(&shidi, sizeof(shidi));
	shidi.dwMask = SHIDIM_FLAGS;
	shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	shidi.hDlg = hDlg;
	if(!SHInitDialog(&shidi))
	{
		return(-1);
	}
*/

	return iRet;
}


static LPCTSTR StringFromResources(UINT uStringID)
{
    // NOTE: Passing NULL for the 3rd parameter of LoadString causes it to
    // return a pointer to the string in the resource file. It requires that
    // the resource file is compiled with the "/n" switch (see SDK docs).
    return (LPCTSTR) LoadString(g_hInst, uStringID, NULL, 0);
}
// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
///#include <winuserm.h>
#include <commctrl.h>
#include "resource.h"
#include <msgqueue.h>		///span0420, add messageQ
#include <pm.h>		///
#include "SYSPOWER_Dlg.h"
///#include "Tlhelp32.h"

static HWND ghDlg = NULL;
static TCHAR szString[64];         	// Temperary string
static HINSTANCE ghInstance = NULL;
static BOOL g_bWinHide = FALSE;
static HANDLE hPowerReq = NULL;

#define QUEUE_ENTRIES   3															///span0420, add messageQ
#define MAX_NAMELEN     200															///span0420, add messageQ
#define QUEUE_SIZE      (QUEUE_ENTRIES * (sizeof(POWER_BROADCAST) + MAX_NAMELEN))	///span0420, add messageQ

///prototype
DWORD spSysPowerDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK spSysPower(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
int spMonSystemPowerState(void);				///span0420, add messageQ


DWORD spSysPowerDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	ghInstance = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_SYSPOWERBOX, hWnd, (DLGPROC)spSysPower);
	
	return dwRet;
}


// Message handler for the SYSPOWER box.
LRESULT CALLBACK spSysPower(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	
	HANDLE hCreateThread = NULL;
	///TCHAR szString[64];         	// Temperary string
	///HWND hwndBufferCombo = NULL;    // Handle to the combobox control
	UINT index = 0;
	
	///HANDLE hSnapShot = INVALID_HANDLE_VALUE;

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

			///hCreateThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CPUtilization, NULL, 0, NULL);					
			///CloseHandle(hCreateThread);
		
			///span0420, add messageQ
    		hCreateThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)spMonSystemPowerState, NULL, 0, NULL );
    		if( NULL == hCreateThread )
    			NKDbgPrintfW( L"Dlg:: Create spMonSystemPowerState Fail !!!\r\n" );
		    else
		    {		
    			CloseHandle( hCreateThread );
				wsprintf( szString, TEXT("System power state monitor start") );
				SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
    		}	
    					
			return TRUE;

		case WM_COMMAND:
		
			if ((LOWORD(wParam) == IDC_SYSPOWER_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_SYSPOWER_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_SYSPOWER_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			else			
			if ((LOWORD(wParam) == IDC_SYSPOWER_SET) )
			{
				if( hPowerReq == NULL )
				{	
					hPowerReq = SetPowerRequirement( _T("COM2:"), D0, (POWER_NAME|POWER_NAME), NULL, 0 );
					if( 0 != hPowerReq )
						wsprintf( szString, TEXT("SetPowerRequirement ok") );
					else
						wsprintf( szString, TEXT("SetPowerRequirement fail!") );
				}
				else
					wsprintf( szString, TEXT("SetPowerRequirement aleady done!") );

				SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				return TRUE;
			}				
			else			
			if ((LOWORD(wParam) == IDC_SYSPOWER_RELEASE) )
			{
				if( hPowerReq != NULL )
				{	
					if( ERROR_SUCCESS == ReleasePowerRequirement( hPowerReq ) )
					{	
						wsprintf( szString, TEXT("ReleasePowerRequirement ok") );
						hPowerReq = NULL;
					}	
					else
						wsprintf( szString, TEXT("ReleasePowerRequirement fail!") );
				}	
				else
					wsprintf( szString, TEXT("ReleasePowerRequirement aleady done!") );
					
				SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				return TRUE;
			}				
			else			
			if ((LOWORD(wParam) == IDC_SYSPOWER_NONE) )
			{
					
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

#define	SPMPMDBG	NKDbgPrintfW
///#define	SPMPMDBG	(void)
#define	SPMPMINFO	NKDbgPrintfW
///#define	SPMPMINFO	(void)
#define	SPMPMFATAL	NKDbgPrintfW

///span0420, add messageQ
int spMonSystemPowerState(void)
{
///	volatile DRIVER_GLOBALS *pDriverGlobals = NULL;
    HANDLE hMsgQ = NULL;
    HANDLE hNotifications = NULL;
    DWORD dwErr, dwFlags = 0, dwCount = 0;
    DWORD dwPri = CeGetThreadPriority(GetCurrentThread());
    
    MSGQUEUEOPTIONS msgOptions = {0};   

    UCHAR buf[QUEUE_SIZE];
    ///int iBytesInQueue;
    DWORD dwBytesInQueue;
    
    SYSTEMTIME spSystemTime;///span0614	
	
	///span0614, set init local time/date
	spSystemTime.wYear = 2005;
	spSystemTime.wMonth = 6;
	spSystemTime.wDay = 15;
	spSystemTime.wHour = 0;
	spSystemTime.wMinute = 0;
	spSystemTime.wSecond = 0;
	SetLocalTime( &spSystemTime );
	///span0614, set init local time/date
	
    // create a message queue for Power Manager notifications
    //
    msgOptions.dwSize = sizeof(MSGQUEUEOPTIONS);
    msgOptions.dwFlags = 0;
    msgOptions.dwMaxMessages = QUEUE_ENTRIES;
    msgOptions.cbMaxMessage = sizeof(POWER_BROADCAST) + MAX_NAMELEN;
    msgOptions.bReadAccess = TRUE;

    hMsgQ = CreateMsgQueue(NULL, &msgOptions);
    if (!hMsgQ)
    {
        dwErr = GetLastError();
        SPMPMFATAL( TEXT("PMMON!CreateMessageQueue ERROR:%d\r\n"), dwErr );

		wsprintf( szString, TEXT("CreateMessageQueue ERROR:%d\r\n"), dwErr );
		SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
        
        goto _Exit;
    }

    // request Power notifications
    //
    hNotifications = RequestPowerNotifications(hMsgQ,
                                               POWER_NOTIFY_ALL); // Flags
    if (!hNotifications)
    {
        dwErr = GetLastError();
        SPMPMFATAL( TEXT("PMMON!RequestPowerNotifications ERROR:%d\r\n"), dwErr );
		wsprintf( szString, TEXT("RequestPowerNotifications ERROR:%d\r\n"), dwErr );
		SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

        goto _Exit;
    }

#if 0
    dwPri = GetPriority();
    if ( !CeSetThreadPriority(GetCurrentThread(), dwPri)) {
        dwErr = GetLastError();
        SPMPMDBG( TEXT("PMAPI!CeSetThreadPriority ERROR:%d\r\n"), dwErr );
		wsprintf( szString, TEXT("CeSetThreadPriority ERROR:%d\r\n"), dwErr );
		SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

        goto _Exit;
    }
#else
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);    
#endif

    while (1) 
    {
        ///iBytesInQueue = 0;
        dwBytesInQueue = 0;
        memset(&buf, 0, QUEUE_SIZE);
		
		///NKDbgPrintfW( L"SPD@Thread:monsystempower: GetCurrentThreadId=0x%08x, CeGetThreadPriority=%d.\r\n", GetCurrentThreadId() ,CeGetThreadPriority(GetCurrentThread()) );	
        // Block on our message queue.
        // This thread runs when the power manager writes a notification into the queue.
        if ( !ReadMsgQueue(hMsgQ,
                           &buf,
                           QUEUE_SIZE,
///                           &iBytesInQueue,
							&dwBytesInQueue,
                           INFINITE,    // Timeout
                           &dwFlags))
        {
            dwErr = GetLastError();
            SPMPMFATAL( TEXT("PMMON!ReadMsgQueue: ERROR:%d\r\n"), dwErr );
			wsprintf( szString, TEXT("ReadMsgQueue ERROR:%d\r\n"), dwErr );
			SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

//            ASSERT(0);
        }
        ///else if(iBytesInQueue >= sizeof(POWER_BROADCAST)) 
        else if(dwBytesInQueue >= sizeof(POWER_BROADCAST)) 
        {
            //
            // process power notifications
            //
            PPOWER_BROADCAST pB = (PPOWER_BROADCAST)&buf;
            
            SPMPMDBG( TEXT("PMMON!*** Power Notification @ Tick:%u, Count:%d, Pri:%d***\r\n"), GetTickCount(), dwCount++, dwPri );
            switch (pB->Message) 
            {
            case PBT_TRANSITION:
                SPMPMDBG( TEXT("PMMON! PBT_TRANSITION to system power state: '%s'\r\n"), pB->SystemPowerState );
				wsprintf( szString, TEXT("PBT_TRANSITION to '%s' @%d\r\n"), pB->SystemPowerState, GetTickCount() );
				SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
                
                switch (POWER_STATE(pB->Flags)) 
                {
                case POWER_STATE_ON:
                    SPMPMDBG( TEXT("PMMON! POWER_STATE_ON\r\n") );
					wsprintf( szString, TEXT("POWER_STATE_ON") );
                    break;
                case POWER_STATE_OFF:
                    SPMPMDBG( TEXT("PMMON! POWER_STATE_OFF\r\n") );
					wsprintf( szString, TEXT("POWER_STATE_OFF") );
                    break;
                case POWER_STATE_CRITICAL:
                    SPMPMDBG( TEXT("PMMON! POWER_STATE_CRITICAL\r\n") );
					wsprintf( szString, TEXT("POWER_STATE_CRITICAL") );
                    break;
                case POWER_STATE_BOOT:
                    SPMPMDBG( TEXT("PMMON! POWER_STATE_BOOT\r\n") );
					wsprintf( szString, TEXT("POWER_STATE_BOOT") );
                    break;
                case POWER_STATE_IDLE:
                    SPMPMDBG( TEXT("PMMON! POWER_STATE_IDLE\r\n") );
					wsprintf( szString, TEXT("POWER_STATE_IDLE") );
                    break;
                case POWER_STATE_SUSPEND:
                    SPMPMDBG( TEXT("PMMON! POWER_STATE_SUSPEND\r\n") );
					wsprintf( szString, TEXT("POWER_STATE_SUSPEND") );
                    break;
                case POWER_STATE_RESET:
                    SPMPMDBG( TEXT("PMMON! POWER_STATE_RESET\r\n") );
					wsprintf( szString, TEXT("POWER_STATE_RESET") );
                    break;
                case 0:
                    break;
					wsprintf( szString, TEXT("0") );                    
                default:
                    SPMPMDBG( TEXT("PMMON! Unknown Power State Flags:0x%x\r\n"),pB->Flags );
					wsprintf( szString, TEXT("Unknown Power State Flags:0x%x"),pB->Flags );
                    break;
                }
				SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
                break;
                
			case PBT_RESUME:
				SPMPMDBG( TEXT("PMMON! PBT_RESUME\r\n") );
///				spPSLOGs("PBT_RESUME");
				wsprintf( szString, TEXT("PBT_RESUME @%d"), GetTickCount() );
				SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				break;
                    
			case PBT_POWERSTATUSCHANGE:
				SPMPMDBG( TEXT("PMMON! PBT_POWERSTATUSCHANGE\r\n") );
///				spPSLOGs("PBT_POWERSTATUSCHANGE");
				wsprintf( szString, TEXT("PBT_POWERSTATUSCHANGE @%d"), GetTickCount() );
				SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				break;
                    
			case PBT_POWERINFOCHANGE:
				wsprintf( szString, TEXT("PBT_POWERINFOCHANGE @%d"), GetTickCount() );
				SendDlgItemMessage( ghDlg, IDC_SYSPOWER_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				{
				///static BYTE bBattflg;	///span0620, add for TN Dennis
				static BYTE bACstatus;	///span0621, add for TN Dennis
				///static BYTE bBattTemp;	///span0621, add for TN Dennis
				///static BYTE bBattPerc;	///span0621, add for TN Dennis
				PPOWER_BROADCAST_POWER_INFO ppbpi = (PPOWER_BROADCAST_POWER_INFO) pB->SystemPowerState;

				SPMPMDBG( TEXT("PMMON! PBT_POWERINFOCHANGE\r\n") );
///				spPSLOGs("PBT_POWERINFOCHANGE");				
				SPMPMDBG( TEXT("PMMON! AC line status %u, battery flag %u, backup flag %u, %d levels\r\n"),
                            ppbpi->bACLineStatus, ppbpi->bBatteryFlag, ppbpi->bBackupBatteryFlag, ppbpi->dwNumLevels );
				SPMPMDBG( TEXT("PMMON!  battery life %d, backup life %d\r\n"),
                            ppbpi->bBatteryLifePercent, ppbpi->bBackupBatteryLifePercent );
				SPMPMDBG( TEXT("PMMON! lifetime 0x%08x, full lifetime 0x%08x\r\n"),
                            ppbpi->dwBatteryLifeTime, ppbpi->dwBatteryFullLifeTime );
				SPMPMDBG( TEXT("PMMON!  backup lifetime 0x%08x, backup full lifetime 0x%08x\r\n"),
                            ppbpi->dwBackupBatteryLifeTime, ppbpi->dwBackupBatteryFullLifeTime );

				}
				break;
                    
			default:
				SPMPMDBG( TEXT("PMMON! Unknown Message:%d\r\n"), pB->Message );
//                    ASSERT(0);
                    break;
            }

            SPMPMDBG( TEXT("PMMON! Message: 0x%x\r\n"), pB->Message );
            SPMPMDBG( TEXT("PMMON! Flags: 0x%x\r\n"), pB->Flags );
            SPMPMDBG( TEXT("PMMON! dwLen: %d\r\n"), pB->Length );
            
            SPMPMDBG( TEXT("PMMON!***********************\r\n") );
        }
        else
        {
            ///SPMPMDBG( TEXT("PMMON! Received short message: %d bytes\r\n"), iBytesInQueue );
            SPMPMFATAL( TEXT("PMMON! Received short message: %d bytes\r\n"), dwBytesInQueue );
        }
        
    }/// end of while

_Exit:
    if (hMsgQ)
        CloseMsgQueue(hMsgQ);

    if (hNotifications && !StopPowerNotifications(hNotifications)) {
        SPMPMDBG( TEXT("PMMON!StopNotifications ERROR:%d\r\n"), GetLastError() );
//        ASSERT(0);
    }

    ExitThread(ERROR_SUCCESS);  
    return ERROR_SUCCESS;
}


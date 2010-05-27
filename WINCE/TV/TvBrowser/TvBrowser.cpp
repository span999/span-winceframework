// TvBrowser.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TvBrowser.h"
#include "TvOverlayFun.h"
#include <commctrl.h>
#include <winuserm.h>	///for VK_APPx key define


///#define	OVERLAY_MOSQUITO

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE           hInst;          // The current instance
HWND    g_hWnd;
HWND    g_hDlg;

WNDPROC	gfMsgProc;

DWORD dwProjectCode = 0;
DWORD dwAVparameter = 0;
DWORD dwAVparaSwitch = 0;
BOOL bTVModulePowerOn = FALSE;
DWORD dwModuleFWversion = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass (HINSTANCE, LPTSTR);
BOOL                InitInstance    (HINSTANCE, int);
LRESULT CALLBACK    WndProc         (HWND, UINT, WPARAM, LPARAM);

PBYTE pMsgBuf, pMsgSignalBuf, pMsgCommonBuf;
DWORD nMsgBufPtr;


#define STR_SVC_LABLE       L"SrvLable: "


void InitChListBox(HWND hDlg);
DWORD GetListValue(HWND hDlg);




int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPTSTR    lpCmdLine,
                    int       nCmdShow)
{
    MSG msg;
    HACCEL hAccelTable;

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow)) 
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TVBROWSER);


    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
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
    WNDCLASS    wc;

    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC) WndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TVBROWSER));
    wc.hCursor          = 0;
    wc.hbrBackground    = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName     = 0;
    wc.lpszClassName    = szWindowClass;

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
    HWND    hWnd;
    TCHAR   szTitle[MAX_LOADSTRING];            // The title bar text
    TCHAR   szWindowClass[MAX_LOADSTRING];      // The window class name

    hInst = hInstance;      // Store instance handle in our global variable
    // Initialize global strings
    LoadString(hInstance, IDC_TVBROWSER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance, szWindowClass);

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {   
        return FALSE;
    }

	g_hWnd = hWnd;
	
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void TvBrowserInit()
{
    pMsgBuf = (PBYTE)LocalAlloc(LPTR, 256);
    nMsgBufPtr = 0;
    
    pMsgSignalBuf =  (PBYTE)LocalAlloc(LPTR, 256);
    pMsgCommonBuf =  (PBYTE)LocalAlloc(LPTR, 512);

    memcpy(pMsgBuf, STR_SVC_LABLE, sizeof(STR_SVC_LABLE));
    nMsgBufPtr += sizeof(STR_SVC_LABLE) - 2;
}

void TvBrowserDeinit()
{
    LocalFree(pMsgBuf);
    LocalFree(pMsgSignalBuf);
    LocalFree(pMsgCommonBuf);
}


// ***************************************************************************
// Function Name: EditBoxSubProc
// 
// Purpose: Subclassed Message Handler for IDC_MESSAGE Edit Box
//
// Arguments: Standard Window Procedure Arguments
//
// Return Values:
//	The LRESULT for a normal message to the Edit Box, ANDed with ~DLGC_WANTMESSAGE
//	if the message is an up arrow and the cursor is on the first line.
//
// Side Effects:
//	Sends an up arrow command to the dialog box
//	
// Description:
//	Intercepts the message from the parent dialog box, asking if the edit control
//	wants to accept the message, and if the cursor is on the first line and the
//	up arrow has been pressed, it rejects the message.

LRESULT CALLBACK EditBoxSubProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	
	switch(uMsg)
	{
		// this is the message that is sent to the Edit Box asking if the Edit Box
		// wants a particular message.  In the case of an up arrow on the top line,
		// we actually want the Edit Box to deny the message and send it back to
		// the dialog box.  This way, focus will return to the previous control.
		case WM_GETDLGCODE:
			// Get the default response
			lResult = CallWindowProc(gfMsgProc, hwnd, uMsg, wParam, lParam);

			if(lParam)
			{
				LPMSG pmsg = (LPMSG)lParam;

				if((pmsg->message == WM_KEYUP))
				{
					switch (pmsg->wParam)
					{
		        		case VK_UP:
				    		swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_UP KEYUP"));
		        			break;
		        		case VK_DOWN:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_DOWN KEYUP"));
		        			break;
		        		case VK_LEFT:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_LEFT KEYUP"));
		        			break;
		        		case VK_RIGHT:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_RIGHT KEYUP"));
		        			break;
		        		case VK_RETURN:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_RETURN KEYUP"));
		        			break;
		        		case VK_VOLUME_UP:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_UP KEYUP"));
		        			TvVolumlUp();
		        			break;
		        		case VK_VOLUME_DOWN:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_DOWN KEYUP"));
		        			TvVolumlDown();
		        			break;
		        		case VK_VOLUME_MUTE:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_MUTE KEYUP"));
		        			break;
		        		case VK_F7:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_F7 KEYUP"));
		        			break;
		        		case VK_F8:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_F8 KEYUP"));
		        			break;
		        		case VK_APP1:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP1 KEYUP"));
		        			break;
		        		case VK_APP2:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP2 KEYUP"));
		        			break;
		        		case VK_APP3:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP3 KEYUP"));
		        			break;
		        		case VK_APP4:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP4 KEYUP"));
		        			break;
		        		default:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("unknow VK KEYUP!!"));
							break;
					}
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);
				}
				
				if((pmsg->message == WM_KEYDOWN))
				{
					switch (pmsg->wParam)
					{
		        		case VK_UP:
				    		swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_UP KEYDOWN"));
		        			break;
		        		case VK_DOWN:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_DOWN KEYDOWN"));
		        			break;
		        		case VK_LEFT:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_LEFT KEYDOWN"));
		        			break;
		        		case VK_RIGHT:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_RIGHT KEYDOWN"));
		        			break;
		        		case VK_RETURN:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_RETURN KEYDOWN"));
		        			break;
		        		case VK_VOLUME_UP:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_UP KEYDOWN"));
		        			break;
		        		case VK_VOLUME_DOWN:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_DOWN KEYDOWN"));
		        			break;
		        		case VK_VOLUME_MUTE:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_MUTE KEYDOWN"));
		        			break;
		        		case VK_F7:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_F7 KEYDOWN"));
		        			break;
		        		case VK_F8:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_F8 KEYDOWN"));
		        			break;
		        		case VK_APP1:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP1 KEYDOWN"));
		        			break;
		        		case VK_APP2:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP2 KEYDOWN"));
		        			break;
		        		case VK_APP3:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP3 KEYDOWN"));
		        			break;
		        		case VK_APP4:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP4 KEYDOWN"));
		        			break;
		        		default:
		        			swprintf( (PWORD)pMsgCommonBuf, TEXT("unknow VK KEYDOWN!!"));
							break;
					}
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);	
				}
			}

			break;

		default:
			// respond to the messages with the default action
			lResult = CallWindowProc(gfMsgProc, hwnd, uMsg, wParam, lParam);
			break;
	}
	
	return lResult;
}


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    int wmId, wmEvent;
    PAINTSTRUCT ps;
//  TCHAR szHello[MAX_LOADSTRING];

    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            break;
        case WM_CREATE:
            TvBrowserFunInit();
            TvBrowserInit();
            DialogBox(hInst, (LPCTSTR)IDD_TVMain, hWnd, (DLGPROC)MainPro);  
            break;
        case WM_PAINT:
            RECT rt;
            hdc = BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &rt);
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

LRESULT CALLBACK MainPro(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rt, rt1;
    int DlgWidth, DlgHeight;    // dialog width and height in pixel units
    int NewPosX, NewPosY, NewSizeX, NewSizeY;
    HDC hdc;
    PAINTSTRUCT ps;
    DWORD EnsembleIndex, dwStrength, dwSNR, dwBER;
    static BOOL bOnRecord = FALSE, bOnPlay = FALSE;
    static BOOL bOnStillShot = FALSE, bOnPause = FALSE;
    static BOOL bOSD1on = FALSE, bOSD2on = FALSE, bOSD3on = FALSE;
    static BOOL bVideoOn = FALSE;
    BOOL bDMB;
    WORD TextBuf1[256];

    switch (message) 
    {

        case WM_INITDIALOG:
            // trying to center the About dialog            
            if (GetWindowRect(hDlg, &rt1)) {
                GetClientRect(GetParent(hDlg), &rt);
                DlgWidth    = rt1.right - rt1.left;
                DlgHeight   = rt1.bottom - rt1.top ;
                NewPosX     = (rt.right - rt.left - DlgWidth)/2;
                NewPosY     = (rt.bottom - rt.top - DlgHeight)/2;
                
                // if the About box is larger than the physical screen 
                if (NewPosX < 0) NewPosX = 0;
                if (NewPosY < 0) NewPosY = 0;
                SetWindowPos(hDlg, HWND_TOPMOST , 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            }
            
            InitChListBox(hDlg);
            
            g_hDlg = hDlg;

			{
				HWND hMsgDlg;

				// subclass the edit box
				///VERIFY(hMsgDlg = GetDlgItem(hDlg, IDC_MSG_LIST));
				VERIFY(hMsgDlg = GetDlgItem(hDlg, IDC_MSG_LIST));
				if(gfMsgProc = (WNDPROC)GetWindowLong(hMsgDlg, GWL_WNDPROC))
				{
					if(SetWindowLong(hMsgDlg, GWL_WNDPROC, (LONG)EditBoxSubProc))
					{
						// successful
						///break;
					}
				}
			}
			
			#ifdef OVERLAY_MOSQUITO 
			DDInitApp( hInst, g_hDlg );
			#endif	///#ifdef OVERLAY_MOSQUITO
            
            return TRUE;

        case WM_COMMAND:
            // Parse the menu selections:
            switch (LOWORD(wParam))
            {
            	case IDC_CHLIST:
					if ((HIWORD(wParam) == CBN_SELCHANGE)) {
						if( bVideoOn ) {	///video is on, show the channel user select
							
							if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
								DWORD dwCh = 0;
								dwCh = SendDlgItemMessage( g_hDlg, IDC_CHLIST, CB_GETCURSEL, 0, 0);
								///SetChannelISDBT( (dwCh+13) );
								SetChannelISDBT2( (dwCh+13), 0 );
							}
						} else {
				    		wsprintf( (PWORD)pMsgCommonBuf, TEXT("Press Vdo On to Show!!"), &pMsgCommonBuf[256]);
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);
						}
					}
            		break;
            	
                case IDC_SCAN:
                    EnableWindow(GetDlgItem(hDlg, IDC_SCAN), FALSE);
                    swprintf(TextBuf1, L"Scanning");
                    SendMessage(GetDlgItem(hDlg, IDC_SCAN), WM_SETTEXT, 0, LPARAM(TextBuf1));

                    EnsembleIndex = SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_GETCURSEL, 0, 0);
                    if ( TvScanService(EnsembleIndex) )
                        swprintf((PWORD)&pMsgBuf[nMsgBufPtr], L"CH LOCKED");
                    else
                        swprintf((PWORD)&pMsgBuf[nMsgBufPtr], L"NO SERVICE");
                    SendMessage(GetDlgItem(hDlg, IDC_channel), WM_SETTEXT, 0, LPARAM(pMsgBuf));

                    swprintf(TextBuf1, L"Scan");
                    SendMessage(GetDlgItem(hDlg, IDC_SCAN), WM_SETTEXT, 0, LPARAM(TextBuf1));
                    EnableWindow(GetDlgItem(hDlg, IDC_SCAN), TRUE);
                    break;
                case ID_Chup:
                    if ( TvChannelUp(bDMB) ) {
                        if ( bDMB )
                            swprintf(TextBuf1, L"Service Type: DMB");
                        else
                            swprintf(TextBuf1, L"Service Type: DAB");
                        SendMessage(GetDlgItem(hDlg, IDC_channel), WM_SETTEXT, 0, LPARAM(TextBuf1));
                        
                        TvGetSignal(dwStrength, dwSNR, dwBER);
                        swprintf((PWORD)pMsgSignalBuf, L"Strength:%02x,SNR:%08x,BER:%08x", dwStrength, dwSNR, dwBER);
                        SendMessage(GetDlgItem(hDlg, IDC_STRENGTH), WM_SETTEXT, 0, LPARAM(pMsgSignalBuf));
                    }
                   break;
                case ID_Chdown:
                    if ( TvChannelDown(bDMB) ) {
                        if ( bDMB )
                            swprintf(TextBuf1, L"Service Type: DMB");
                        else
                            swprintf(TextBuf1, L"Service Type: DAB");
                        SendMessage(GetDlgItem(hDlg, IDC_channel), WM_SETTEXT, 0, LPARAM(TextBuf1));
                        
                        TvGetSignal(dwStrength, dwSNR, dwBER);
                        swprintf((PWORD)pMsgSignalBuf, L"Strength:%02x,SNR:%08x,BER:%08x", dwStrength, dwSNR, dwBER);
                        SendMessage(GetDlgItem(hDlg, IDC_STRENGTH), WM_SETTEXT, 0, LPARAM(pMsgSignalBuf));
                    }
                   break;
                case ID_VolUp:
                    TvVolumlUp();
                    break;
                case ID_Voldown:
                   	TvVolumlDown();
                    break;
                case ID_VideoOn:
                	if( !bVideoOn ) {
                    	NewSizeX = GetListValue(GetDlgItem(hDlg, IDC_VIN_SIZE_X));
                    	NewSizeY = GetListValue(GetDlgItem(hDlg, IDC_VIN_SIZE_Y));
                    	TvSetVideoInSize(NewSizeX, NewSizeY);
                    	NewSizeX = GetListValue(GetDlgItem(hDlg, IDC_VOUT_SIZE_X));
                    	NewSizeY = GetListValue(GetDlgItem(hDlg, IDC_VOUT_SIZE_Y));
                    	TvSetVideoOutSize(NewSizeX, NewSizeY);
                    	if ( NewSizeX > 460)
                    	    NewPosX = 0;
                    	else
                    	    NewPosX = 16;
                    	if ( NewSizeY > 136)
                    	    NewPosY = 0;
                    	else
                    	    NewPosY = 200;
                    	///TvSetVideoPos(NewPosX, NewPosY);
                    	if( 0x67333130 == dwProjectCode ) {	///PT310
                    		TvSetVideoPos( 15, 10 );	///new position for new camera drviver
                    	} else {
                    		TvSetVideoPos( 15, 105 );	///new position for new camera drviver
                    	}
                    	
                    	TvVedioOn();
                    	bVideoOn = TRUE;
                    	SendMessage(GetDlgItem(hDlg, ID_VideoOn), WM_SETTEXT, 0, (LPARAM)L"Vdo off");
                    	TvGetChLabel(pMsgBuf);
                    	swprintf(TextBuf1, L"SrvLable: %s", pMsgBuf);
                    	SendMessage(GetDlgItem(hDlg, IDC_channel), WM_SETTEXT, 0, LPARAM(TextBuf1));
                    	
                    	TvGetSignal(dwStrength, dwSNR, dwBER);
                    	swprintf((PWORD)pMsgSignalBuf, L"Strength:%02x,SNR:%08x,BER:%08x", dwStrength, dwSNR, dwBER);
                    	SendMessage(GetDlgItem(hDlg, IDC_STRENGTH), WM_SETTEXT, 0, LPARAM(pMsgSignalBuf));
                    	waveOutMessage(0, 0x1AE, 0, 0);
                    	
                    	#ifndef OVERLAY_MOSQUITO
                    	#ifndef OVERLAY_DRAWTEXT
                    	///DDOverlayBitmap( hInst, g_hDlg, 32, 20, 220, 10, 0, IDB_BATT3, TRUE );
                    	///DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 3"), IDB_BATT3, 32, 20, 210, 0, TRUE );
                    	DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 3"), IDB_BATT3, 30, 15, 210, 0, IDB_SIG3, 53, 15, 150, 0, TRUE );
                    	#endif	///#ifndef OVERLAY_DRAWTEXT
                    	#endif	///#ifndef OVERLAY_MOSQUITO 
                    } else {
	                    TvVedioOff();
    	                bVideoOn = FALSE;
                    	#ifndef OVERLAY_MOSQUITO
                    	#ifndef OVERLAY_DRAWTEXT
                    	///DDOverlayBitmap( NULL, NULL, 0, 0, 0, 0, 0, 0, FALSE );
                    	///DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 0"), IDB_BATT0, 32, 20, 210, 0, FALSE );
                    	DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 0"), IDB_BATT0, 30, 15, 210, 0, IDB_SIG0, 53, 15, 150, 0, FALSE );
                    	#endif	///#ifndef OVERLAY_DRAWTEXT
                    	#endif	///#ifndef OVERLAY_MOSQUITO 

    	                SendMessage(GetDlgItem(hDlg, ID_VideoOn), WM_SETTEXT, 0, (LPARAM)L"Vdo On");
                    }
                    break;
                case ID_CHANNELSET:
                	if( 0x67333130 == dwProjectCode ) {	///PT310
						DWORD dwCh = 0;
						DWORD dwSerV = 0;
						dwCh = SendDlgItemMessage( g_hDlg, IDC_CHLIST, CB_GETCURSEL, 0, 0);
						dwSerV = SendDlgItemMessage( g_hDlg, IDC_SERVLIST, CB_GETCURSEL, 0, 0);
		
						///SetChannelISDBT( (dwCh+13) );
						SetChannelISDBT2( (dwCh+13), dwSerV );
					}
                    break;
				case IDC_POWERON_TEST:
                	if( 0x67333130 == dwProjectCode ) {	///PT310
                		;
                	} else {
		    		wsprintf( (PWORD)pMsgCommonBuf, TEXT("!!test 900 loops w/ power"), &pMsgCommonBuf[256]);
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);			

					TvPowerOnTest( 900, TRUE );
					}
					break;
				case IDC_POWERON_TEST2:
                	if( 0x67333130 == dwProjectCode ) {	///PT310
                		;
                	} else {
		    		wsprintf( (PWORD)pMsgCommonBuf, TEXT("!!test 900 loops w/o power"), &pMsgCommonBuf[256]);
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);			

					TvPowerOnTest( 900, FALSE );
					}
					break;
				case IDC_POWERON_TEST3:
                	if( 0x67333130 == dwProjectCode ) {	///PT310
                		;
                	} else {

						DWORD dwLoop;

						wsprintf( (PWORD)pMsgCommonBuf, TEXT("!!test 30px30 loops w/ power"), &pMsgCommonBuf[256]);
						SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);			

						
						for( dwLoop = 30; dwLoop > 0; dwLoop-- ) {
							///power off
							///TvNeptunePowerOnOff( FALSE, TRUE );
						    bTVModulePowerOn = TvModulePowerOnOff( FALSE, TRUE );

							///power on
							///TvNeptunePowerOnOff( TRUE, TRUE );
							bTVModulePowerOn = TvModulePowerOnOff( TRUE, TRUE );
							
							TvPowerOnTest( 30, FALSE );
						}
						
	                	if( bTVModulePowerOn ) {
    	            		SendMessage(GetDlgItem(hDlg, IDC_MODULEPOWER), WM_SETTEXT, 0, (LPARAM)L"Pwr Off");
        	        	} else {
            	    		SendMessage(GetDlgItem(hDlg, IDC_MODULEPOWER), WM_SETTEXT, 0, (LPARAM)L"Pwr On");
                		}

					}
					break;
                case IDC_VIDEOPOSCENTER:
                	TvVideoPosition(1);
                	break;
                case IDC_VIDEOPOSRIGHT:
                	TvVideoPosition(2);
                	break;
                case IDC_VIDEOPOSDOWN:
                	TvVideoPosition(3);
                	break;

                case IDC_GETFWID:
                	if( 0x67333130 == dwProjectCode ) {	///PT310
                		BYTE bVer1 = 0;
                		BYTE bVer2 = 0;
                		BYTE bVer3 = 0;
                		BYTE bVer4 = 0;
                		dwModuleFWversion = DTVmoduleFWversion();
                		
                		if( dwModuleFWversion != 0 ){
                			bVer1 = (BYTE)(dwModuleFWversion>>0 &0x000000FF);
                			bVer2 = (BYTE)(dwModuleFWversion>>8 &0x000000FF);
                			bVer3 = (BYTE)(dwModuleFWversion>>16 &0x000000FF);
                			bVer4 = (BYTE)(dwModuleFWversion>>24 &0x000000FF);

	                    	swprintf((PWORD)pMsgCommonBuf, L"F/W:%c%c%c%c", bVer1, bVer2, bVer3, bVer4 );
    	                	SendMessage(GetDlgItem(hDlg, IDC_FWINFO), WM_SETTEXT, 0, LPARAM(pMsgCommonBuf));

			    			wsprintf( (PWORD)pMsgCommonBuf, TEXT("F/W:%c%c%c%c.0x%08x"), bVer1, bVer2, bVer3, bVer4, dwModuleFWversion );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);

                		} else {
	                    	swprintf((PWORD)pMsgCommonBuf, L"F/W:0x%08x", dwModuleFWversion );
    	                	SendMessage(GetDlgItem(hDlg, IDC_FWINFO), WM_SETTEXT, 0, LPARAM(pMsgCommonBuf));

			    			wsprintf( (PWORD)pMsgCommonBuf, TEXT("F/W:0x%08x"), dwModuleFWversion );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);
						}
                	} else {

                    ZeroMemory(pMsgCommonBuf, 512);
                    TvGetVersionId(&pMsgCommonBuf[256]);
                    TvGetChecksum(&pMsgCommonBuf[384]);
                    swprintf((PWORD)pMsgCommonBuf, L"F/W:%s,CKS=%s", &pMsgCommonBuf[256], &pMsgCommonBuf[384]);
                    SendMessage(GetDlgItem(hDlg, IDC_FWINFO), WM_SETTEXT, 0, LPARAM(pMsgCommonBuf));
                    
		    		wsprintf( (PWORD)pMsgCommonBuf, TEXT("F/W:%s"), &pMsgCommonBuf[256]);
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);			
		    		wsprintf( (PWORD)pMsgCommonBuf, TEXT("CKS=%s"), &pMsgCommonBuf[384]);
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);			
                    
                    TvSetModuleSysInfo( 0x05060708 );	///set the default module setting
					}
                    break;

                case IDC_MODULEPOWER:
                	bTVModulePowerOn = TvModulePowerOnOff( !bTVModulePowerOn, FALSE );
                	if( bTVModulePowerOn ) {
                		SendMessage(GetDlgItem(hDlg, IDC_MODULEPOWER), WM_SETTEXT, 0, (LPARAM)L"Pwr Off");
                	} else {
                		SendMessage(GetDlgItem(hDlg, IDC_MODULEPOWER), WM_SETTEXT, 0, (LPARAM)L"Pwr On");
                	}
                    break;

                ///case IDC_NEPPWROFF:
                ///    TvNeptunePowerOnOff(FALSE, FALSE);
                ///    break;
                case IDC_RF_REGDUMP:
                	TvRFregisterDump( 0xFF, FALSE );
                	break;
                case IDC_CLS_LIST:
                	{
						do
						{
							SendDlgItemMessage(hDlg, IDC_MSG_LIST, LB_DELETESTRING, 0, 0);
						}
						while( 0 != SendDlgItemMessage(hDlg, IDC_MSG_LIST, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

                	}
                	break;
				case IDC_OSD1:
					bOSD1on = !bOSD1on;
					TvOSD( 0, bOSD1on );
					break;

				case IDC_OSD2:
					bOSD2on = !bOSD2on;
					TvOSD( 1, bOSD2on );
					break;

				case IDC_OSD3:
					bOSD3on = !bOSD3on;
					TvOSD( 2, bOSD3on );
					break;
				case IDC_OSD4:
					TvOSD( 5, 0 );
					break;
				case IDC_BATTERY:
					TvSetBattery();
					break;	 
				case IDC_CONTRAST:
					TvContrast();
					break;					
				case IDC_SATURATION:
					TvSaturation();
					break;					
				case IDC_BRIGHTNESS:
					TvBrightness();
					break;
				case IDC_DATASERVICE:
					TvDataService(0);
					break;
				case IDC_CHECKMODULE:
					TvCheckModule();
					break;
				case IDC_AV_PARAM:
					if( dwProjectCode == 0x67333130 ) {	///PT310
						if( 0 == dwAVparameter ) {
							SendMessage(GetDlgItem(hDlg, IDC_AV_PARAM), WM_SETTEXT, 0, (LPARAM)L"Aes");///1
							dwAVparameter++;
							wsprintf( (PWORD)pMsgCommonBuf, TEXT("Audio ES"));
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);			
						} else
						if( 1 == dwAVparameter ) {
							SendMessage(GetDlgItem(hDlg, IDC_AV_PARAM), WM_SETTEXT, 0, (LPARAM)L"Amd");///2
							dwAVparameter++;
							wsprintf( (PWORD)pMsgCommonBuf, TEXT("Audio Mode"));
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);			
						} else
						if( 2 == dwAVparameter ) {
							SendMessage(GetDlgItem(hDlg, IDC_AV_PARAM), WM_SETTEXT, 0, (LPARAM)L"Ves");///3
							dwAVparameter++;
							wsprintf( (PWORD)pMsgCommonBuf, TEXT("Video ES"));
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);			
						} else
						if( 3 == dwAVparameter ) {
							SendMessage(GetDlgItem(hDlg, IDC_AV_PARAM), WM_SETTEXT, 0, (LPARAM)L"CC");///4
							dwAVparameter++;
							wsprintf( (PWORD)pMsgCommonBuf, TEXT("ClosedCaption"));
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);			
						} else {
							SendMessage(GetDlgItem(hDlg, IDC_AV_PARAM), WM_SETTEXT, 0, (LPARAM)L"AVs");///others then 1,2,3
							dwAVparameter = 0;
						}
						dwAVparaSwitch = 0;///reset it
					}
					break;
				case IDC_AV_CHANGE:
					TVISDBchangeAVparameter( &dwAVparameter, &dwAVparaSwitch );
					break;
				case IDC_OSD3SET:
					TvOSD3setting( FALSE );
					break;
				case IDC_OSD3GET:
					TvOSD3setting( TRUE );
					break;
				case IDC_LOOPBACK:
					TvCmdLoopBack();
					break;
				case IDC_ZOOMTEST:
					TvCmdZoomTest( bOnPlay );
					break;
				case IDC_SCREEN:
					
	#if 0	///for mindy test
					waveOutMessage(0, 0x1AE, 0, 0);
					while(1) {
						Sleep(1000);
						TvChangeScreen( bOnPlay );
					}
	#else
					TvChangeScreen( bOnPlay );
					waveOutMessage(0, 0x1AE, 0, 0);
	#endif
    	            bVideoOn = TRUE;
    	            SendMessage(GetDlgItem(hDlg, ID_VideoOn), WM_SETTEXT, 0, (LPARAM)L"Vdo Off");

					break;

                case IDC_STILLSHOT:
#if 1               
					if( dwProjectCode == 0x67333130 ) {	///PT310
						TvStillShot(7);                	
					} else {
						TvStillShot(5);                	
					}
#else                        
                	bOnStillShot = !bOnStillShot;
                    if (bOnStillShot) {
                        SendMessage(GetDlgItem(hDlg, IDC_STILLSHOT), WM_SETTEXT, 0, (LPARAM)L"Stop");
                        EnableWindow(GetDlgItem(hDlg, IDC_PLAY), FALSE);
                        EnableWindow(GetDlgItem(hDlg, IDC_RECORD), FALSE);                        
                        TvStillShot(5);
                    } else {
                        SendMessage(GetDlgItem(hDlg, IDC_STILLSHOT), WM_SETTEXT, 0, (LPARAM)L"StillShot");
                        EnableWindow(GetDlgItem(hDlg, IDC_PLAY), TRUE);
                        EnableWindow(GetDlgItem(hDlg, IDC_RECORD), TRUE);                        
                        TvStillShot(6);
                    }
#endif
					break;

                case IDC_RECORD:
                    bOnRecord = !bOnRecord;
                    if (bOnRecord) {
                        SendMessage(GetDlgItem(hDlg, IDC_RECORD), WM_SETTEXT, 0, (LPARAM)L"Stop");
                        EnableWindow(GetDlgItem(hDlg, IDC_PLAY), FALSE);
                        EnableWindow(GetDlgItem(hDlg, IDC_STILLSHOT), FALSE);                        
                        ///TvRecord(0);
                        TvRecord(5);
                    } else {
                        SendMessage(GetDlgItem(hDlg, IDC_RECORD), WM_SETTEXT, 0, (LPARAM)L"Record");
                        EnableWindow(GetDlgItem(hDlg, IDC_PLAY), TRUE);
                        EnableWindow(GetDlgItem(hDlg, IDC_STILLSHOT), TRUE);                        
                        ///TvRecord(2);
                        TvRecord(6);
                    }
                    break;

                case IDC_PAUSE:
                	bOnPause = !bOnPause;
                    if (bOnPause) {
                        SendMessage(GetDlgItem(hDlg, IDC_PAUSE), WM_SETTEXT, 0, (LPARAM)L"Resume");
                        EnableWindow(GetDlgItem(hDlg, IDC_PAUSE), TRUE);
					} else {
                        SendMessage(GetDlgItem(hDlg, IDC_PAUSE), WM_SETTEXT, 0, (LPARAM)L"Pause");
                        EnableWindow(GetDlgItem(hDlg, IDC_PAUSE), TRUE);
					}                	
					TvPlayback(7);
                    break;

                case IDC_PLAY:
                    bOnPlay = !bOnPlay;
                    if (bOnPlay) {
                        SendMessage(GetDlgItem(hDlg, IDC_PLAY), WM_SETTEXT, 0, (LPARAM)L"Stop");
                        EnableWindow(GetDlgItem(hDlg, IDC_RECORD), FALSE);
                        EnableWindow(GetDlgItem(hDlg, IDC_STILLSHOT), FALSE);
                        ///TvPlayback(0);
                        TvPlayback(5);
                    } else {
                        SendMessage(GetDlgItem(hDlg, IDC_PLAY), WM_SETTEXT, 0, (LPARAM)L"Play");
                        EnableWindow(GetDlgItem(hDlg, IDC_RECORD), TRUE);
                        EnableWindow(GetDlgItem(hDlg, IDC_STILLSHOT), TRUE);
                        ///TvPlayback(2);
                        TvPlayback(6);
                    }
                    break;

                default:
                   return DefWindowProc(hDlg, message, wParam, lParam);
            }
            break;

        case WM_PAINT:
            {
                RECT rt;
                static TCHAR szValue[MAX_PATH];
                
                hdc = BeginPaint(hDlg, &ps);
                GetClientRect(hDlg, &rt);
                
                SetRect (&rt, rt.left,rt.top, rt.right,rt.bottom) ;
                FillRect (hdc, &rt, CreateSolidBrush(GetSysColor (COLOR_BTNFACE))) ;
    
                EndPaint(hDlg, &ps);
            }

            SendMessage(GetDlgItem(hDlg, IDC_channel), WM_SETTEXT, 0, LPARAM(pMsgBuf));
            TvGetSignal(dwStrength, dwSNR, dwBER);
            swprintf((PWORD)pMsgSignalBuf, L"Strength:%02x,SNR:%08x,BER:%08x", dwStrength, dwSNR, dwBER);
            SendMessage(GetDlgItem(hDlg, IDC_STRENGTH), WM_SETTEXT, 0, LPARAM(pMsgSignalBuf));

            break;

        case WM_KEYUP:
        	switch (wParam) {
        		case VK_UP:
		    		swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_UP KEYUP"));
        			break;
        		case VK_DOWN:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_DOWN KEYUP"));
        			break;
        		case VK_LEFT:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_LEFT KEYUP"));
        			break;
        		case VK_RIGHT:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_RIGHT KEYUP"));
        			break;
        		case VK_RETURN:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_RETURN KEYUP"));
        			break;
        		case VK_VOLUME_UP:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_UP KEYUP"));
        			TvVolumlUp();
        			break;
        		case VK_VOLUME_DOWN:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_DOWN KEYUP"));
        			TvVolumlDown();
        			break;
        		case VK_VOLUME_MUTE:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_MUTE KEYUP"));
        			break;
        		case VK_F7:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_F7 KEYUP"));
        			break;
        		case VK_F8:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_F8 KEYUP"));
        			break;
        		case VK_APP1:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP1 KEYUP"));
        			break;
        		case VK_APP2:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP2 KEYUP"));
        			break;
        		case VK_APP3:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP3 KEYUP"));
        			break;
        		case VK_APP4:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP4 KEYUP"));
        			break;
        		default:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("unknow VK KEYUP!!"));
					break;
        	}
        	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);
            break;
        case WM_KEYDOWN:
        	switch (wParam) {
        		case VK_UP:
		    		swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_UP KEYDOWN"));
        			break;
        		case VK_DOWN:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_DOWN KEYDOWN"));
        			break;
        		case VK_LEFT:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_LEFT KEYDOWN"));
        			break;
        		case VK_RIGHT:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_RIGHT KEYDOWN"));
        			break;
        		case VK_RETURN:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_RETURN KEYDOWN"));
        			break;
        		case VK_VOLUME_UP:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_UP KEYDOWN"));
        			break;
        		case VK_VOLUME_DOWN:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_DOWN KEYDOWN"));
        			break;
        		case VK_VOLUME_MUTE:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_VOLUME_MUTE KEYDOWN"));
        			break;
        		case VK_F7:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_F7 KEYDOWN"));
        			break;
        		case VK_F8:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_F8 KEYDOWN"));
        			break;
        		case VK_APP1:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP1 KEYDOWN"));
        			break;
        		case VK_APP2:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP2 KEYDOWN"));
        			break;
        		case VK_APP3:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP3 KEYDOWN"));
        			break;
        		case VK_APP4:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("VK_APP4 KEYDOWN"));
        			break;
        		default:
        			swprintf( (PWORD)pMsgCommonBuf, TEXT("unknow VK KEYDOWN!!"));
					break;
        	}
        	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)pMsgCommonBuf);
            break;

        case WM_TIMER:
            // Update and flip surfaces
            if( 333 == wParam ) {
            	#ifdef OVERLAY_MOSQUITO 
                DDMoveOverlay();
                #endif	//#ifdef OVERLAY_MOSQUITO 
            }
            break;

        case WM_DESTROY:
            TvBrowserFunDeinit();
            TvBrowserDeinit();
            #ifdef OVERLAY_MOSQUITO 
            DDdeInitApp();
            #endif	///#ifdef OVERLAY_MOSQUITO 
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hDlg, message, wParam, lParam);
   }
   return 0;
}

void InitChListBox(HWND hDlg)
{
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"07A (175.280)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"07B (177.008)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"07C (178.736)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"08A (181.280)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"08B (183.008)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"08C (184.736)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"09A (187.280)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"09B (189.008)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"09C (190.736)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"10A (193.280)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"10B (195.008)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"10C (196.736)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"11A (199.280)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"11B (201.008)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"11C (202.736)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"12A (205.280)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"12B (207.008)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"12C (208.736)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"13A (211.280)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"13B (213.008)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_ADDSTRING, 0,  (LPARAM)L"13C (214.736)");
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_SETCURSEL, 0, 0);

    SendMessage(GetDlgItem(hDlg, IDC_VIN_SIZE_X), CB_INSERTSTRING, -1,  (LPARAM)L"492");
    SendMessage(GetDlgItem(hDlg, IDC_VIN_SIZE_X), CB_INSERTSTRING, -1,  (LPARAM)L"640");
    SendMessage(GetDlgItem(hDlg, IDC_VIN_SIZE_X), CB_INSERTSTRING, -1,  (LPARAM)L"480");
    SendMessage(GetDlgItem(hDlg, IDC_VIN_SIZE_X), CB_SETCURSEL, 1, 0);

    SendMessage(GetDlgItem(hDlg, IDC_VIN_SIZE_Y), CB_INSERTSTRING, -1,  (LPARAM)L"240");
    SendMessage(GetDlgItem(hDlg, IDC_VIN_SIZE_Y), CB_INSERTSTRING, -1,  (LPARAM)L"480");
    SendMessage(GetDlgItem(hDlg, IDC_VIN_SIZE_Y), CB_INSERTSTRING, -1,  (LPARAM)L"288");
    SendMessage(GetDlgItem(hDlg, IDC_VIN_SIZE_Y), CB_SETCURSEL, 1, 0);

    SendMessage(GetDlgItem(hDlg, IDC_VOUT_SIZE_X), CB_INSERTSTRING, -1,  (LPARAM)L"492");
    SendMessage(GetDlgItem(hDlg, IDC_VOUT_SIZE_X), CB_INSERTSTRING, -1,  (LPARAM)L"480");
    SendMessage(GetDlgItem(hDlg, IDC_VOUT_SIZE_X), CB_INSERTSTRING, -1,  (LPARAM)L"240");
    SendMessage(GetDlgItem(hDlg, IDC_VOUT_SIZE_X), CB_SETCURSEL, 2, 0);

    SendMessage(GetDlgItem(hDlg, IDC_VOUT_SIZE_Y), CB_INSERTSTRING, -1,  (LPARAM)L"240");
    SendMessage(GetDlgItem(hDlg, IDC_VOUT_SIZE_Y), CB_INSERTSTRING, -1,  (LPARAM)L"272");
    SendMessage(GetDlgItem(hDlg, IDC_VOUT_SIZE_Y), CB_INSERTSTRING, -1,  (LPARAM)L"136");
    SendMessage(GetDlgItem(hDlg, IDC_VOUT_SIZE_Y), CB_SETCURSEL, 2, 0);
}

DWORD GetListValue(HWND hDlg)
{
    DWORD dwCurSel;
    PBYTE pTextBuf;
    
    pTextBuf = (PBYTE)LocalAlloc(LPTR, 256);
    ZeroMemory(pTextBuf, 256);
    
    dwCurSel = SendMessage(hDlg, CB_GETCURSEL, 0, 0);
    SendMessage(hDlg, CB_GETLBTEXT, (WORD)dwCurSel, (LPARAM)pTextBuf);
    
    LocalFree(pTextBuf);
    
    return wcstoul((USHORT *)pTextBuf, NULL, 10);
}
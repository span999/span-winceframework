//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
///#include <windows.h>
///#include <drvlib.h>
#include "spOS.h"
#include "spPlatform.h"


#include "..\..\Inc\spLibErrCodeDef.h"
#include "..\..\Inc\spLibDDrawClass.h"
#include "..\..\Inc\spLibSysInfoIdle.h"
#include "SPDebugDef.h"

#include "resource.h"
///
// Debugging defines and functions for KITL output
#define MSG_INFO 0
#define MSG_TRACE 0
#define MSG_WARNING 0
#define MSG_ERROR 1


#define NAMED_SYSTEM_IDLE_CHECK_EXIT_EVENT		TEXT("named system idle check exit event")


//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                TEXT("DDExample1")
#define TITLE               TEXT("my DDraw")

//-----------------------------------------------------------------------------
// Default settings
//-----------------------------------------------------------------------------
#define TIMER_ID            1
///#define TIMER_RATE          2000
#define TIMER_RATE          1000


//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
static TCHAR                szMsg[] = TEXT("Page Flipping Test: Press F12 to exit");
static TCHAR                szFrontMsg[] = TEXT("Front buffer (F12 to quit)");
static TCHAR                szBackMsg[] = TEXT("Back buffer (F12 to quit)");

///phototype



///
static TCHAR szString_[64];         	// Temperary string

static spLibDDraw	*pmyDD = NULL;

BOOL                        g_bActive = FALSE;   // Is application active?
int                         g_RotationAngles = 0; // Supported rotation angles.
int                         g_CurrentAngle = 0;   // Current rotation angle.
// Our instance handle.
HINSTANCE g_hInstance;


static void DrawPixel( void );
static void DrawCPULoading( DWORD dwValue );


/// *****************************************
/// routine
/// *****************************************

inline void DebugOutput(int out_en, char* fmt, ...)
{
	if (out_en)
	{
		WCHAR out[1024];
		WCHAR wfmt[256];
		va_list body;
		va_start(body, fmt);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fmt, -1, wfmt, 256);
		vswprintf(out, wfmt, body);
		va_end(body);
		RETAILMSG(1, (out));
	}
}


//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: Finished with all objects we use; release them
//-----------------------------------------------------------------------------
static void
ReleaseAllObjects(void)
{

	spLibSysInfoIdle_Deinit( 0 );
	
	delete pmyDD;
/*
    if (g_pDDSBack != NULL)
    {
        g_pDDSBack->Release();
        g_pDDSBack = NULL;
    }
    if (g_pDDSPrimary != NULL)
    {
        g_pDDSPrimary->Release();
        g_pDDSPrimary = NULL;
    }
    if (g_pDD != NULL)
    {
        g_pDD->Release();
        g_pDD = NULL;
    }
*/	
}



//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
#define PREFIX      TEXT("DDEX1: ")
#define PREFIX_LEN  7

HRESULT
InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...)
{
    TCHAR                       szBuff[128] = PREFIX;
    va_list                     vl;

    va_start(vl, szError);

    StringCchVPrintf(szBuff + PREFIX_LEN, (128-PREFIX_LEN), szError, vl);

    ReleaseAllObjects();
	
    ///OutputDebugString(szBuff);
	MessageBox( NULL, szBuff, TEXT("MainRoutine"), MB_OK );
	
    DestroyWindow(hWnd);
    va_end(vl);
    return hRet;
}

#if 0
void spMessageBoxOut( LPCTSTR szError,... )
{
    TCHAR                       szBuff[128] = PREFIX;
    va_list                     vl;

    va_start(vl, szError);

    StringCchVPrintf(szBuff + PREFIX_LEN, (128-PREFIX_LEN), szError, vl);

	MessageBox( NULL, szBuff, TEXT("Debug Msg !!"), MB_OK|MB_SETFOREGROUND );
	
    va_end(vl);
}
#else
void spMessageBoxOut( DWORD dwFlag, LPCTSTR szError,... )
{
    TCHAR                       szBuff[128];
    va_list                     vl;

	if( dwFlag&DBGMSG_LEVEL )
	{
		va_start(vl, szError);

		StringCchVPrintf(szBuff + PREFIX_LEN, (128-PREFIX_LEN), szError, vl);

		MessageBox( NULL, szBuff, TEXT("Debug Msg !!"), MB_OK|MB_SETFOREGROUND );
	
		va_end(vl);
	}	
}
#endif


#undef PREFIX_LEN
#undef PREFIX



//-----------------------------------------------------------------------------
// Name: UpdateFrame()
// Desc: Displays the proper text for the page
//-----------------------------------------------------------------------------
static void
UpdateFrame(HWND hWnd)
{
    static BYTE phase = 0;
    HDC hdc;
    RECT rc;
    SIZE size;
    int nMsg;
    DDBLTFX ddbltfx;

    // Use the blter to do a color fill to clear the back buffer
    memset(&ddbltfx, 0, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;
///    g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAITNOTBUSY, &ddbltfx);

///    if (g_pDDSBack->GetDC(&hdc) == DD_OK)
	if( 1 )
    {
        SetBkColor(hdc, RGB(0, 0, 255));
        SetTextColor(hdc, RGB(255, 255, 0));
        GetClientRect(hWnd, &rc);
        if (phase)
        {
	        nMsg = lstrlen(szMsg);
            GetTextExtentPoint(hdc, szMsg, nMsg, &size);
            ExtTextOut(hdc, 
		       (rc.right - size.cx) / 2, 
		       (rc.bottom - size.cy) / 2,
		       0,                        // fuOptions
		       NULL,                     // lprc
               szMsg, 
		       nMsg,
		       NULL);                    // lpDx

            nMsg = lstrlen(szFrontMsg);
            GetTextExtentPoint(hdc, szFrontMsg, nMsg, &size);
            ExtTextOut(hdc, 
		       (rc.right - size.cx) / 2, // Center horz. for tv reasons
		       0, 
		       0,                        // fuOptions
		       NULL,                     // lprc
		       szFrontMsg, 
		       nMsg,
		       NULL);                    // lpDx
            phase = 0;
        }
        else
        {
            nMsg = lstrlen(szBackMsg);
            GetTextExtentPoint(hdc, szBackMsg, nMsg, &size);
            ExtTextOut(hdc, 
		       (rc.right - size.cx) / 2, // Center horz. for tv reasons 
		       0,  
		       0,                        // fuOptions
		       NULL,                     // lprc
		       szBackMsg, 
		       nMsg,
		       NULL);                    // lpDx
            phase = 1;
        }
///        g_pDDSBack->ReleaseDC(hdc);
    }
}




//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------
long FAR PASCAL
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HRESULT                     hRet;
	static DWORD 	dwCount = 0;
	
	
    switch (message)
    {
#ifdef UNDER_CE
        case WM_ACTIVATE:
#else
        case WM_ACTIVATEAPP:
#endif
            // Pause if minimized or not the top window
            g_bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
            return 0L;

        case WM_DESTROY:
            // Clean up and close the app
            ReleaseAllObjects();
            PostQuitMessage(0);
            return 0L;

        case WM_KEYDOWN:
		case WM_KEYUP:
            // Handle any non-accelerated key commands
            switch (wParam)
            {
                case VK_ESCAPE:
                case VK_F12:
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0L;
            }
			
			///any key!!
			// Clean up and close the app
			ReleaseAllObjects();
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			PostQuitMessage(0);
			return 0L;	
            break;

        case WM_SETCURSOR:
            // Turn off the cursor since this is a full-screen app
            SetCursor(NULL);
            return TRUE;

        case WM_TIMER:
            // Update and flip surfaces
			///spMessageBoxOut( TEXT("WM_TIMER Hit !!") );
            ///if (g_bActive && TIMER_ID == wParam)
			if( TIMER_ID == wParam )
            {
				if( dwCount < 20 )
				{
					///DrawPixel();
					DrawCPULoading( 100 - spLibSysInfoIdle_Get() );
				}	
				else
				{
					// Clean up and close the app
					ReleaseAllObjects();
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					PostQuitMessage(0);
					return 0L;					
				}
				
				dwCount++;
	#if 0			
	
                ///UpdateFrame(hWnd);
                while (TRUE)
                {
///                    hRet = g_pDDSPrimary->Flip(NULL, 0);
                    if (hRet == DD_OK)
                        break;
                    if (hRet == DDERR_SURFACELOST)
                    {
///                        hRet = g_pDDSPrimary->Restore();
                        if (hRet != DD_OK)
                            break;
                    }
                    if (hRet != DDERR_WASSTILLDRAWING)
                        break;
                }
	#endif			
            }
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


//-----------------------------------------------------------------------------
// Name: EnumFunction()
// Desc: Enumeration callback for surfaces in flipping chain. We expect this
//          function to be called once with the surface interface pointer of
//          our back buffer (we only ask for a single back buffer.)
//-----------------------------------------------------------------------------
static HRESULT PASCAL
EnumFunction(LPDIRECTDRAWSURFACE pSurface,
             LPDDSURFACEDESC lpSurfaceDesc,
             LPVOID  lpContext)
{
    static BOOL bCalled = FALSE;

    if (!bCalled) {

        *((LPDIRECTDRAWSURFACE *)lpContext) = pSurface;
        bCalled = TRUE;
        return DDENUMRET_OK;
    }
    else {

		spMessageBoxOut( dWARN, TEXT("DDEX1: Enumerated more than surface?") );
        pSurface->Release();
        return DDENUMRET_CANCEL;
    }
}


#define CPULOADDRAW_X_START		50
#define CPULOADDRAW_Y_START		0

#define CPULOADDRAW_X_SCALE		5
#define CPULOADDRAW_Y_SCALE		3

#define CPULOADDRAW_WIDTH		100
#define CPULOADDRAW_HIGHT		4

#define CPULOADDRAW_X_END		(CPULOADDRAW_X_START+(CPULOADDRAW_WIDTH*CPULOADDRAW_X_SCALE))
#define CPULOADDRAW_Y_END		(CPULOADDRAW_Y_START+(CPULOADDRAW_HIGHT*CPULOADDRAW_Y_SCALE))


static void DrawCPULoading( DWORD dwValue )
{
	DWORD dwLoop = 0;
	static DWORD dwTimes = 0;
	RECT rcDest;
	DWORD dwNum = 0;

	spMessageBoxOut( dINIT, TEXT("DrawCPULoading+++") );	

	if( NULL != pmyDD )
	{
	
		///draw backgound
		SetRect( &rcDest, CPULOADDRAW_X_START, CPULOADDRAW_Y_START, CPULOADDRAW_X_END, CPULOADDRAW_Y_END );
		pmyDD->spLibBltDDraw( &rcDest, 0, 0xFF, 0x0 );		///green as Backgoung

		///draw percentage
		if( dwValue > 0 )
		{
		SetRect( &rcDest, CPULOADDRAW_X_START, CPULOADDRAW_Y_START, (CPULOADDRAW_X_START+(dwValue*CPULOADDRAW_X_SCALE)), CPULOADDRAW_Y_END );
		pmyDD->spLibBltDDraw( &rcDest, 0xFF, 0x0, 0x0 );		///red as Backgoung
		}

		for( dwLoop = 1; dwLoop <= 20; dwLoop++ )
		{
			///draw bloack dot every 10 
			pmyDD->spLibPixelDraw( (CPULOADDRAW_X_START+(dwLoop*5*CPULOADDRAW_X_SCALE)), CPULOADDRAW_Y_START, 0x0, 0x0, 0x0 );
			if( 0 == dwLoop % 2 ) 
				pmyDD->spLibPixelDraw( (CPULOADDRAW_X_START+(dwLoop*5*CPULOADDRAW_X_SCALE))+1, CPULOADDRAW_Y_START, 0x0, 0x0, 0x0 );
			
			if( CPULOADDRAW_Y_SCALE > 0 )
			{
				pmyDD->spLibPixelDraw( (CPULOADDRAW_X_START+(dwLoop*5*CPULOADDRAW_X_SCALE)), CPULOADDRAW_Y_START+1, 0x0, 0x0, 0x0 );
				if( 0 == dwLoop % 2 ) 
					pmyDD->spLibPixelDraw( (CPULOADDRAW_X_START+(dwLoop*5*CPULOADDRAW_X_SCALE))+1, CPULOADDRAW_Y_START+1, 0x0, 0x0, 0x0 );
			}	
			if( CPULOADDRAW_Y_SCALE > 1 )
			{
				pmyDD->spLibPixelDraw( (CPULOADDRAW_X_START+(dwLoop*5*CPULOADDRAW_X_SCALE)), CPULOADDRAW_Y_START+2, 0x0, 0x0, 0x0 );
				if( 0 == dwLoop % 2 ) 
					pmyDD->spLibPixelDraw( (CPULOADDRAW_X_START+(dwLoop*5*CPULOADDRAW_X_SCALE))+1, CPULOADDRAW_Y_START+2, 0x0, 0x0, 0x0 );
			}
		}
		
		
		pmyDD->spLibTextDraw( CPULOADDRAW_X_START, CPULOADDRAW_Y_END+CPULOADDRAW_HIGHT, TEXT("CPU utilization => %4d%% !!!"), dwValue );

#if 0		
		for( ; dwLoop < 1000; dwLoop++ )
		{	
			dwNum = (dwLoop%100) + 2;
			
			//clean it, fill with color
			SetRect( &rcDest, 0, 0, 800, 400 );
			pmyDD->spLibBltDDraw( &rcDest, 0, 0, 0xFF );
			
			///pmyDD->spLibDrawPixel( 2+(dwLoop%100), 2+(dwLoop%100) );
			pmyDD->spLibPixelDraw( dwNum+dwTimes, dwNum );
			SetRect( &rcDest, dwNum+dwTimes+10, dwNum+10, dwNum+dwTimes+10+5, dwNum+10+5 );
			pmyDD->spLibBltDDraw( &rcDest, 0, 0xFF, 0 );
			
			pmyDD->spLibTextDraw( TEXT("TEST string out to DD surface !!!") );
			
		}
		dwTimes++;
#endif	
	}

	spMessageBoxOut( dINIT, TEXT("DrawCPULoading---") );	
}

static void DrawPixel( void )
{
	DWORD dwLoop = 0;
	static DWORD dwTimes = 0;
	RECT rcDest;
	DWORD dwNum = 0;

	spMessageBoxOut( dINIT, TEXT("Start draw pixel+++") );	

	if( NULL != pmyDD )
	{
		for( ; dwLoop < 1000; dwLoop++ )
		{	
			dwNum = (dwLoop%100) + 2;
			
			//clean it, fill with color
			SetRect( &rcDest, 0, 0, 800, 400 );
			pmyDD->spLibBltDDraw( &rcDest, 0, 0, 0xFF );
			
			///pmyDD->spLibDrawPixel( 2+(dwLoop%100), 2+(dwLoop%100) );
			pmyDD->spLibPixelDraw( dwNum+dwTimes, dwNum );
			SetRect( &rcDest, dwNum+dwTimes+10, dwNum+10, dwNum+dwTimes+10+5, dwNum+10+5 );
			pmyDD->spLibBltDDraw( &rcDest, 0, 0xFF, 0 );
			
			pmyDD->spLibTextDraw( TEXT("TEST string out to DD surface !!!") );
			
			
			
	#if 0
			if( 0 == dwLoop%10 )
				pmyDD->spLibFlipDDraw();
			else	
			if( 0 == (dwLoop+5)%10 )
				pmyDD->spLibRestoreDDraw();
			else
				;///Sleep( 10 );
	#endif			
		}
		dwTimes++;
	}
	
	spMessageBoxOut( dINIT, TEXT("Start draw pixel---") );	
}


static HRESULT InitDDraw( HWND hWnd )
{
    HRESULT	hRet;
	
	spMessageBoxOut( dINIT, TEXT("Create spLibDDRaw object !") );
	pmyDD = new spLibDDraw( hWnd );
	///pmyDD = new spLibDDraw();
	
	if( pmyDD == NULL )
	{
		spMessageBoxOut( dFAIL,  TEXT("Create spLibDDRaw object fail !") );	
	}
	else
	{
		DWORD dwRet = 0;
		
		spMessageBoxOut( dINFO, TEXT("Start Init DDraw ") );	
		///init primary with 1 back
		///pmyDD->spLibInitDDrawBack( 0 );
		///pmyDD->spLibInitDDrawBack( 0,  EnumFunction );
		dwRet = pmyDD->spLibInitDDraw();
		
		
		if( dwRet == 0 )
		{

		}
		else
		{
			wsprintf(szString_, TEXT("Init DDraw error %02d%%"), dwRet);
			MessageBox( NULL, szString_, TEXT("MainRoutine"), MB_OK );
		}
	}		
	
	spMessageBoxOut( dINIT, TEXT("spLibDDRaw done !!") );
	
	return hRet;
}



BOOL InitCheckRotation( void )
{
	BOOL bRet = TRUE;

    DEVMODE                     DevMode;

    // Check for rotation support by getting the rotation angles supported.
 
    memset(&DevMode, 0, sizeof(DevMode));
    DevMode.dmSize = sizeof(DevMode);
    DevMode.dmFields = DM_DISPLAYQUERYORIENTATION;

    if (DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettingsEx(NULL, &DevMode, NULL, CDS_TEST, NULL)) {

        g_RotationAngles = DevMode.dmDisplayOrientation;
    }
    else {

        OutputDebugString(L"MOSQUITO: Device does not support any rotation modes. Rotation disabled.");
        g_RotationAngles = -1;
    }

    // Get the current rotation angle.

    memset(&DevMode, 0, sizeof (DevMode));
    DevMode.dmSize = sizeof (DevMode);
    DevMode.dmFields = DM_DISPLAYORIENTATION;

    if (DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettingsEx(NULL, &DevMode, NULL, CDS_TEST, NULL)) {

        g_CurrentAngle = DevMode.dmDisplayOrientation;
    }
    else {

        OutputDebugString(L"MOSQUITO: Unable to read current rotation. Rotation disabled.");
        g_CurrentAngle = -1;
    }

	
	return bRet;
}



//-----------------------------------------------------------------------------
// Name: InitApp()
// Desc: Do work required for every instance of the application:
//          Create the window, initialize data
//-----------------------------------------------------------------------------
static HRESULT
InitApp(HINSTANCE hInstance, int nCmdShow)
{
    HWND                        hWnd;
    WNDCLASS                    wc;
    DDSURFACEDESC               ddsd;
    HRESULT                     hRet;

	InitCheckRotation();
	
    // Set up and register window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NAME;
    RegisterClass(&wc);

    // Create a window
    hWnd = CreateWindowEx(WS_EX_TOPMOST,
                          NAME,
                          TITLE,
                          WS_POPUP,
                          0,
                          0,
#if 0						  
                          GetSystemMetrics(SM_CXSCREEN),
                          GetSystemMetrics(SM_CYSCREEN),
#else
						  100,
						  50,
#endif
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    if (!hWnd)
        return FALSE;
		
    // We never show the window, only set focus to it.		
    ///ShowWindow(hWnd, nCmdShow);
    ///UpdateWindow(hWnd);
    SetFocus(hWnd);

	
	hRet = InitDDraw( hWnd );
	
	///init
	spLibSysInfoIdle_Init( 0 );
	
    // Create a timer to flip the pages
    if (TIMER_ID != SetTimer(hWnd, TIMER_ID, TIMER_RATE, NULL))
		spMessageBoxOut( dFAIL,  TEXT("SetTimer FAILED !") );
        ///return InitFail(hWnd, hRet, TEXT("SetTimer FAILED"));

		
    return DD_OK;
}


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//-----------------------------------------------------------------------------
int WINAPI WinMain (
              HINSTANCE hInstance,    // Handle to the current instance
              HINSTANCE hPrevInstance,// Handle to the previous instance
              LPTSTR lpCmdLine,       // Pointer to the command line
              int nCmdShow)           // Show state of the window
{

    MSG                         msg;

	spMessageBoxOut( dINIT, TEXT("InitApp+++ !") );
    if (InitApp(hInstance, nCmdShow) != DD_OK)
        return FALSE;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;


/*	
	///MessageBox( NULL, TEXT("Create spLibDDRaw object !"), TEXT("MainRoutine"), MB_OK );
	spMessageBoxOut( TEXT("Create spLibDDRaw object !") );
	pmyDD = new spLibDDraw();
	
	if( pmyDD == NULL )
	{
		MessageBox( NULL, TEXT("Create spLibDDRaw object fail !"), TEXT("MainRoutine"), MB_OK );	
	}
	else
	{
		DWORD dwLoop = 1000;
		DWORD dwRet = 0;
		
		MessageBox( NULL, TEXT("Start Init DDraw "), TEXT("MainRoutine"), MB_OK );	
		dwRet = pmyDD->spLibInitDDraw();
		
		if( dwRet == 0 )
		{
			MessageBox( NULL, TEXT("Start draw pixel"), TEXT("MainRoutine"), MB_OK );	
			for( ;dwLoop > 0; dwLoop-- )
				pmyDD->spLibDrawPixel( 2, 2 );
		}
		else
		{
			wsprintf(szString_, TEXT("Init DDraw error %02d%%"), dwRet);
			MessageBox( NULL, szString_, TEXT("MainRoutine"), MB_OK );
		}
	}		
	
	MessageBox( NULL, TEXT("exit !!"), TEXT("MainRoutine"), MB_OK );
	
	delete pmyDD;
	return 0;
*/	
	
}




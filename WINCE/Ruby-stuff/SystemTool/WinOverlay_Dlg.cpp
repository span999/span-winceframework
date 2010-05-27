// ReserveFlash.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
///#include <winuserm.h>
#include <commctrl.h>
#include "CEDDK.h"
#include "resource.h"
#include <msgqueue.h>		///span0420, add messageQ
#include <pm.h>		///
#include "WinOverlay_Dlg.h"
#include "WinOverlay.h"

static HWND ghDlg = NULL;
static TCHAR szString[64];         	// Temperary string
static HINSTANCE ghInstance = NULL;
static BOOL g_bWinHide = FALSE;
static HANDLE hPowerReq = NULL;

#define QUEUE_ENTRIES   3															///span0420, add messageQ
#define MAX_NAMELEN     200															///span0420, add messageQ
#define QUEUE_SIZE      (QUEUE_ENTRIES * (sizeof(POWER_BROADCAST) + MAX_NAMELEN))	///span0420, add messageQ


#define COLOR_BLUE	0x001F
#define COLOR_GREEN	0x07E0
#define COLOR_RED	0xF800
#define COLOR_WHILE	0xFFFF


///prototype
DWORD spWinOverlayDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK spWinOverlay(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void WinOverlayRunTest( void );
void WinOverlayAlphaTest( void );
void WinOverlayKeyTest( void );



DWORD spWinOverlayDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	ghInstance = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_WINOVERLAYBOX, hWnd, (DLGPROC)spWinOverlay);
	
	return dwRet;
}


// Message handler for the WINOVERLAY box.
LRESULT CALLBACK spWinOverlay(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

		
			return TRUE;

		case WM_COMMAND:
		
			if ((LOWORD(wParam) == IDC_WINOVERLAY_CLS) )
			{
				do
				{
					SendDlgItemMessage(hDlg, IDC_WINOVERLAY_LIST1, LB_DELETESTRING, 0, 0);
				}
				while( 0 != SendDlgItemMessage(hDlg, IDC_WINOVERLAY_LIST1, LB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

				return TRUE;					
			}				
			else			
			if ((LOWORD(wParam) == IDC_WINOVERLAY_ALPHA) )
			{
				wsprintf( szString, TEXT("Alpha test !") );					
				SendDlgItemMessage( ghDlg, IDC_WINOVERLAY_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);

				WinOverlayAlphaTest();
				return TRUE;
			}				
			else			
			if ((LOWORD(wParam) == IDC_WINOVERLAY_KEY) )
			{
				wsprintf( szString, TEXT("Color Key test !") );					
				SendDlgItemMessage( ghDlg, IDC_WINOVERLAY_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				
				WinOverlayKeyTest();
				return TRUE;
			}				
			else			
			if ((LOWORD(wParam) == IDC_WINOVERLAY_RUN) )
			{
				wsprintf( szString, TEXT("Run test !") );					
				SendDlgItemMessage( ghDlg, IDC_WINOVERLAY_LIST1, LB_ADDSTRING, 0, (LPARAM)szString);
				
				WinOverlayRunTest();
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

void WinOverlayRunTest( void )
{
	DWORD dwLoop = 0;
	BYTE iLoop = 0;
    CWinOverlay OSDdbgPort( 240, 140, 20, 20 );
    DWORD dwWait = 2000;
    
    while( dwLoop < 1 ) {
		
		OSDdbgPort.SetALPHAvalue0( 0xF, 0xF, 0xF);
		while( dwLoop < 50 ){
			OSDdbgPort.DbgPrintColor( COLOR_RED, COLOR_WHILE );
			OSDdbgPort.DbgPrintString( L"R" );
			Sleep(100);

			OSDdbgPort.DbgPrintColor( COLOR_GREEN, COLOR_WHILE );
			OSDdbgPort.DbgPrintString( L"G" );
			Sleep(100);

			OSDdbgPort.DbgPrintColor( COLOR_BLUE, COLOR_WHILE );
			OSDdbgPort.DbgPrintString( L"B" );
			Sleep(100);
			dwLoop++;
		}
		dwLoop = 0;
		
		///OSDdbgPort.DbgPrintString( L"1111111111222222222233333333334444444444" );
		///OSDdbgPort.DbgPrintString( L"55555555556666666666777777777788888888889999999999" );
		///Sleep(3000);
		
    	OSDdbgPort.WinRePosition( 20, 20 );
    	OSDdbgPort.DbgPrintString( L"change RED alpha value...." );
    	for( iLoop = 0; iLoop <= 0xF; iLoop++ ) {
    	    OSDdbgPort.SetALPHAvalue0( iLoop, 0x7, 0x7);
    	    Sleep(100);
    	}
    	OSDdbgPort.DbgPrintString( L"change GREEN alpha value...." );
    	for( iLoop = 0; iLoop <= 0xF; iLoop++ ) {
    	    OSDdbgPort.SetALPHAvalue0( 0x7, iLoop, 0x7);
    	    Sleep(100);
    	}
    	OSDdbgPort.DbgPrintString( L"change BLUE alpha value...." );    
    	for( iLoop = 0; iLoop <= 0xF; iLoop++ ) {
    	    OSDdbgPort.SetALPHAvalue0( 0x7, 0x7, iLoop);
    	    Sleep(100);
    	}
    	OSDdbgPort.DbgPrintString( L"change RED/GREEN/BLUE alpha value...." );
    	for( iLoop = 0; iLoop <= 0xF; iLoop++ ) {
    	    OSDdbgPort.SetALPHAvalue0( iLoop, iLoop, iLoop);
    	    Sleep(100);
    	}    
    	
    	OSDdbgPort.DbgPrintString( L"set alpha value 777...." );
    	OSDdbgPort.SetALPHAvalue0( 0x7, 0x7, 0x7);

    	Sleep(dwWait);

    	OSDdbgPort.WinRePosition( 60, 100 );
    	OSDdbgPort.DbgPrintString( L"Change position ...." );
    	Sleep(dwWait);

    	OSDdbgPort.WinRePosition( 20, 20 );
    	OSDdbgPort.DbgPrintString( L"Change position ...." );
    	Sleep(dwWait);

    	OSDdbgPort.WinRePosition( 60, 100 );
    	OSDdbgPort.DbgPrintString( L"Change position ...." );
    	Sleep(dwWait);

    	OSDdbgPort.WinRePosition( 20, 20 );
    	OSDdbgPort.DbgPrintString( L"Change position ...." );
    	Sleep(dwWait);
    	
    	///OSDdbgPort.WinRePosition( 20, 20 );
    	OSDdbgPort.DbgPrintColor( COLOR_WHILE, COLOR_RED );
    	OSDdbgPort.DbgPrintString( L"change background color RED...." );
    	Sleep(dwWait);
    	
    	OSDdbgPort.DbgPrintColor( COLOR_WHILE, COLOR_GREEN );
    	OSDdbgPort.DbgPrintString( L"change background color GREEN...." );
    	Sleep(dwWait);

    	OSDdbgPort.DbgPrintColor( COLOR_WHILE, COLOR_BLUE );
    	OSDdbgPort.DbgPrintString( L"change background color BLUE...." );
    	Sleep(dwWait);

    	OSDdbgPort.DbgPrintColor( COLOR_RED, COLOR_WHILE );
    	OSDdbgPort.DbgPrintString( L"change foreground color RED...." );
    	Sleep(dwWait);
    	
    	OSDdbgPort.DbgPrintColor( COLOR_GREEN, COLOR_WHILE );
    	OSDdbgPort.DbgPrintString( L"change foreground color GREEN...." );
    	Sleep(dwWait);

    	OSDdbgPort.DbgPrintColor( COLOR_BLUE, COLOR_WHILE );
    	OSDdbgPort.DbgPrintString( L"change foreground color BLUE...." );
    	Sleep(dwWait);


    	OSDdbgPort.WinRePosition( 40, 40 );
    	OSDdbgPort.DbgPrintString( L"Change position ...." );
    	Sleep(dwWait);

		OSDdbgPort.DbgPrintColor( COLOR_RED, COLOR_GREEN );
    	OSDdbgPort.SetColorKey1( TRUE, TRUE, TRUE, 0x00030307, 0xFFFFFFFF );
    	OSDdbgPort.DbgPrintString( L"Key FFFFFFFF front RED back GREEN ...." );
    	Sleep(dwWait);
    	///OSDdbgPort.WinRePosition( 60, 100 );
    	OSDdbgPort.DbgPrintColor( COLOR_GREEN, COLOR_BLUE );
    	OSDdbgPort.SetColorKey1( TRUE, TRUE, TRUE, 0x00030307, 0xFFFFFFFF );
    	OSDdbgPort.DbgPrintString( L"Key FFFFFFFF front GREEN back BLUE" );
    	Sleep(dwWait);
 
 
        Sleep(dwWait);
        OSDdbgPort.DbgPrintString( L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
        Sleep(dwWait);
        OSDdbgPort.DbgPrintString( L"0123456789abcdefghijklmnopqrstuvwkyz" );
        Sleep(dwWait);
        OSDdbgPort.DbgPrintString( L"!@#$%^&*()_+{}[]'\"|;:?/.,><" );


    	OSDdbgPort.DbgPrintString( L"set alpha value FFF...." );
    	OSDdbgPort.SetALPHAvalue0( 0xF, 0xF, 0xF);


        Sleep(dwWait);
        OSDdbgPort.DbgPrintString( L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
        Sleep(dwWait);
        OSDdbgPort.DbgPrintString( L"0123456789abcdefghijklmnopqrstuvwkyz" );
        Sleep(dwWait);
        OSDdbgPort.DbgPrintString( L"!@#$%^&*()_+{}[]'\"|;:?/.,><" );


		OSDdbgPort.DbgPrintColor( COLOR_RED, COLOR_GREEN );
    	OSDdbgPort.SetColorKey1( TRUE, FALSE, FALSE, 0x00030307, 0xFFFFFFFF );
    	OSDdbgPort.DbgPrintString( L"No Key front RED back GREEN ...." );
    	Sleep(dwWait);

        dwLoop++;
    }    

}

void WinOverlayKeyTest( void )
{
	DWORD dwLoop = 0;
	BYTE iLoop = 0;
	DWORD dwWait = 4000;
    CWinOverlay OSDdbgPort( 240, 140, 20, 20 );
    BOOL bAlphaEnable = TRUE;
    
    while( dwLoop < 1 ) {
		
		OSDdbgPort.SetALPHAvalue0( 0xF, 0xF, 0xF);

		while( dwLoop < 40 ){
			OSDdbgPort.DbgPrintColor( COLOR_RED, COLOR_WHILE );
			OSDdbgPort.DbgPrintString( L"R" );
			Sleep(10);

			OSDdbgPort.DbgPrintColor( COLOR_GREEN, COLOR_WHILE );
			OSDdbgPort.DbgPrintString( L"G" );
			Sleep(10);

			OSDdbgPort.DbgPrintColor( COLOR_BLUE, COLOR_WHILE );
			OSDdbgPort.DbgPrintString( L"B" );
			Sleep(10);
			dwLoop++;
		}
		dwLoop = 0;
		
		OSDdbgPort.SetALPHAvalue0( 0x7, 0x7, 0x7 );
    	OSDdbgPort.SetALPHAvalue1( 0x3, 0x3, 0x3 );
    	
		Sleep(dwWait);

    	OSDdbgPort.SetColorKey1( FALSE, TRUE, FALSE, 0x00000000, 0x00070307 );
    	OSDdbgPort.DbgPrintString( L"Key 00000000." );
    	Sleep(dwWait);
    	Sleep(dwWait);
		
    	OSDdbgPort.SetColorKey1( FALSE, TRUE, FALSE, 0xFFFFFFFF, 0x00070307 );
    	OSDdbgPort.DbgPrintString( L"Key FFFFFFFF." );
    	Sleep(dwWait);
    	Sleep(dwWait);
    	
    	OSDdbgPort.SetColorKey1( FALSE, TRUE, FALSE, 0x000000F8, 0x00070307 );
    	OSDdbgPort.DbgPrintString( L"Key BLUE." );
    	Sleep(dwWait);
    	Sleep(dwWait);
    	
    	OSDdbgPort.SetColorKey1( FALSE, TRUE, FALSE, 0x0000FC00, 0x00070307 );
    	OSDdbgPort.DbgPrintString( L"Key GREEN." );
    	Sleep(dwWait);
    	Sleep(dwWait);

    	OSDdbgPort.SetColorKey1( FALSE, TRUE, FALSE, 0x00F80000, 0x00070307 );
    	OSDdbgPort.DbgPrintString( L"Key RED." );
    	Sleep(dwWait);
    	Sleep(dwWait);

		OSDdbgPort.DbgPrintString( L"Alpha Enable." );

    	OSDdbgPort.SetColorKey1( TRUE, TRUE, FALSE, 0x00000000, 0x00070307 );
    	OSDdbgPort.DbgPrintString( L"Key 00000000." );
    	Sleep(dwWait);
    	Sleep(dwWait);
		
    	OSDdbgPort.SetColorKey1( TRUE, TRUE, FALSE, 0xFFFFFFFF, 0x00070307 );
    	OSDdbgPort.DbgPrintString( L"Key FFFFFFFF." );
    	Sleep(dwWait);
    	Sleep(dwWait);
    	
    	OSDdbgPort.SetColorKey1( TRUE, TRUE, FALSE, 0x000000F8, 0x00070307 );
    	OSDdbgPort.DbgPrintString( L"Key BLUE." );
    	Sleep(dwWait);
    	Sleep(dwWait);
    	
    	OSDdbgPort.SetColorKey1( TRUE, TRUE, FALSE, 0x0000FC00, 0x00070307 );
    	OSDdbgPort.DbgPrintString( L"Key GREEN." );
    	Sleep(dwWait);
    	Sleep(dwWait);

    	OSDdbgPort.SetColorKey1( TRUE, TRUE, FALSE, 0x00F80000, 0x00070307 );
    	OSDdbgPort.DbgPrintString( L"Key RED." );
    	Sleep(dwWait);
    	Sleep(dwWait);
    	
        dwLoop++;
    }    

}


void WinOverlayAlphaTest( void )
{
	DWORD dwLoop = 0;
	BYTE iLoop = 0;
    CWinOverlay OSDdbgPort( 240, 140, 20, 20 );
    DWORD dwWait = 4000;
    
    while( dwLoop < 1 ) {
		
		OSDdbgPort.SetALPHAvalue0( 0xF, 0xF, 0xF);
		while( dwLoop < 40 ){
			OSDdbgPort.DbgPrintColor( COLOR_RED, COLOR_WHILE );
			OSDdbgPort.DbgPrintString( L"R" );
			Sleep(10);

			OSDdbgPort.DbgPrintColor( COLOR_GREEN, COLOR_WHILE );
			OSDdbgPort.DbgPrintString( L"G" );
			Sleep(10);

			OSDdbgPort.DbgPrintColor( COLOR_BLUE, COLOR_WHILE );
			OSDdbgPort.DbgPrintString( L"B" );
			Sleep(10);
			dwLoop++;
		}
		dwLoop = 0;
		
		Sleep(dwWait);
		
    	OSDdbgPort.WinRePosition( 20, 20 );
    	OSDdbgPort.DbgPrintString( L"changing RED alpha value...." );
    	for( iLoop = 0; iLoop <= 0xF; iLoop++ ) {
    	    OSDdbgPort.SetALPHAvalue0( iLoop, 0xF, 0xF);
    	    Sleep(300);
    	}
    	OSDdbgPort.DbgPrintString( L"change GREEN alpha value...." );
    	for( iLoop = 0; iLoop <= 0xF; iLoop++ ) {
    	    OSDdbgPort.SetALPHAvalue0( 0xF, iLoop, 0xF);
    	    Sleep(300);
    	}
    	OSDdbgPort.DbgPrintString( L"changing BLUE alpha value...." );    
    	for( iLoop = 0; iLoop <= 0xF; iLoop++ ) {
    	    OSDdbgPort.SetALPHAvalue0( 0xF, 0xF, iLoop);
    	    Sleep(300);
    	}
    	OSDdbgPort.DbgPrintString( L"changing RED/GREEN/BLUE alpha value...." );
    	for( iLoop = 0; iLoop <= 0xF; iLoop++ ) {
    	    OSDdbgPort.SetALPHAvalue0( iLoop, iLoop, iLoop);
    	    Sleep(300);
    	}    
    	
    	OSDdbgPort.DbgPrintString( L"set alpha value 777...." );
    	OSDdbgPort.SetALPHAvalue0( 0x7, 0x7, 0x7);

    	Sleep(dwWait);

        dwLoop++;
    }    

}
// ComPort.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ComPort.h"
#include <commctrl.h>

#define MAX_LOADSTRING 100


#define spBAUD_921600		0x00000100
#define spBAUD_115200		0x00000080
#define spBAUD_057600		0x00000040
#define spBAUD_038400		0x00000020
#define spBAUD_019200		0x00000010
#define spBAUD_009600		0x00000008
#define spBAUD_004800		0x00000004
#define spBAUD_002400		0x00000002
#define spBAUD_001200		0x00000001

#define spCOM_1				0x00010000
#define spCOM_2				0x00020000
#define spCOM_3				0x00040000
#define spCOM_4				0x00080000
#define spCOM_5				0x00100000
#define spCOM_6				0x00200000
#define spCOM_7				0x00400000
#define spCOM_8				0x00800000

#define spCOM1_NAME			TEXT("COM1:")
#define spCOM2_NAME			TEXT("COM2:")
#define spCOM3_NAME			TEXT("COM3:")
#define spCOM4_NAME			TEXT("COM4:")
#define spCOM5_NAME			TEXT("COM5:")
#define spCOM6_NAME			TEXT("COM6:")
#define spCOM7_NAME			TEXT("COM7:")
#define spCOM8_NAME			TEXT("COM8:")

// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle

DWORD				dwBaudSelet = spBAUD_004800; 
DWORD				dwComSelet = spCOM_2;
BOOL				bPortOpened = FALSE;
BOOL				bForcePortClose = FALSE;
BOOL				bScreenPause = FALSE;
TCHAR				tcByte;

HANDLE				hDebugThread = NULL;
HANDLE 				hComPort = INVALID_HANDLE_VALUE;

#define	X_OFFSET			8
#define	Y_OFFSET			14
LONG				x_offset = 0;
LONG				y_offset = 0;
LONG				x_total = 0;
LONG				y_total = 0;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Filelist1		(HWND, UINT, WPARAM, LPARAM);


/*******************************************************************************
		
********************************************************************************/
void ShowToScreen(HWND hwnd)
{
#if 0
	HDC				hdc;
	PAINTSTRUCT		ps;
	RECT			rect;

	hdc = BeginPaint(hwnd, &ps);
	GetClientRect(hwnd, &rect);

	///DrawText(hdc, g_szMessage, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	DrawText(hdc, &tcByte, -1, &rect, DT_LEFT | DT_TOP);
	EndPaint (hwnd, &ps);
#else


	HDC				hdc;
	RECT			rect;
    HBRUSH			hbrush;              // current brush

	hdc = GetDC(hwnd); 
	GetClientRect(hwnd, &rect);
	x_total = rect.right - rect.left;
	y_total = rect.bottom - rect.top;

///	NKDbgPrintfW( TEXT("SPD@AccessComPort():rect.left=%d.\r\n"),rect.left );
///	NKDbgPrintfW( TEXT("SPD@AccessComPort():rect.right=%d.\r\n"),rect.right );
///	NKDbgPrintfW( TEXT("SPD@AccessComPort():rect.top=%d.\r\n"),rect.top );
///	NKDbgPrintfW( TEXT("SPD@AccessComPort():rect.bottom=%d.\r\n"),rect.bottom );
///	DrawText(hdc, TEXT(">"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);	
/*
	if ( 0 == x_offset)
	{
///		rect.left = rect.left + x_offset; 
		rect.top = rect.top + y_offset; 
		DrawText(hdc, TEXT(">"), -1, &rect, DT_LEFT | DT_TOP);	
///		y_offset = y_offset + Y_OFFSET;
		x_offset = x_offset + X_OFFSET;
	}
*/
	rect.left = rect.left + x_offset; 
	rect.top = rect.top + y_offset;
	DrawText(hdc, TEXT(" "), -1, &rect, DT_LEFT | DT_TOP);		
	DrawText(hdc, &tcByte, 1, &rect, DT_LEFT | DT_TOP);
///	y_offset = y_offset + Y_OFFSET;
	x_offset = x_offset + X_OFFSET;

	if (x_offset > x_total - X_OFFSET)
	{
		y_offset = y_offset + Y_OFFSET;
		x_offset = 0;
	}

	if (y_offset > y_total - Y_OFFSET)
	{
		y_offset = 0;
		x_offset = 0;
		rect.left = 0;
		rect.top = 0;
		// Create a monochrome bitmap. 
 
		///hbm = CreateBitmap(8, 8, 1, 1, (LPBYTE)bBrushBits); 
 
		// Select the custom brush into the DC. 
 
		//hbrush = CreatePatternBrush(hbm); 
		hbrush = CreateSolidBrush(WHITE_BRUSH);
		///SelectObject(hdc, (HBRUSH)GetStockObject(GRAY_BRUSH));
		///SelectObject(hdc, (HBRUSH)CreateSolidBrush(BLACK_BRUSH));
		SelectObject(hdc, hbrush);
		
		///FillRect(hdc, &rect, hbrush);
		FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	}
	
	//	MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL); 
//	LineTo(hdc, LOWORD(lParam), HIWORD(lParam)); 
	ReleaseDC(hwnd, hdc); 

#endif
}

/*******************************************************************************
		AccessComPort(void)
********************************************************************************/
void WINAPI AccessComPort(HWND hwnd)
{
	BYTE Byte;
	DWORD dwBytesTransferred;
	HANDLE hTestComm = INVALID_HANDLE_VALUE;
	DWORD dwCommModemStatus;
	DCB dcb;
	COMMTIMEOUTS CommTimeouts;
	

	NKDbgPrintfW( TEXT("SPD@AccessComPort():start !! \r\n") );
	///MessageBox(hwnd, L"Com Port Already Open !!", L"Error", MB_OK);
	
	// Open CommPort
	if( spCOM_1 == dwComSelet )
	{
		hTestComm = CreateFile ( TEXT("COM1:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	if( spCOM_2 == dwComSelet )
	{
		hTestComm = CreateFile ( TEXT("COM2:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	if( spCOM_3 == dwComSelet )
	{
		hTestComm = CreateFile ( TEXT("COM3:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	if( spCOM_4 == dwComSelet )
	{
		hTestComm = CreateFile ( TEXT("COM4:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	if( spCOM_5 == dwComSelet )
	{
		hTestComm = CreateFile ( TEXT("COM5:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	if( spCOM_6 == dwComSelet )
	{
		hTestComm = CreateFile ( TEXT("COM6:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	if( spCOM_7 == dwComSelet )
	{
		hTestComm = CreateFile ( TEXT("COM7:"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}



	if ( INVALID_HANDLE_VALUE == hTestComm )
	{
		NKDbgPrintfW( TEXT("SPD@AccessComPort():start CreateFile Fail !! \r\n") );
		MessageBox(hwnd, L"Com Port Open failed!!", L"Error", MB_OK);
		return;
	}
	
	
	hComPort = hTestComm;
	/// set flag port open
	bPortOpened = TRUE;

	memset(&dcb,0,sizeof(dcb));

	dcb.DCBlength = sizeof (DCB);	

	if (spBAUD_001200 == dwBaudSelet)
		dcb.BaudRate = CBR_1200;
	else
	if (spBAUD_002400 == dwBaudSelet)
		dcb.BaudRate = CBR_2400;
	else
	if (spBAUD_004800 == dwBaudSelet)
		dcb.BaudRate = CBR_4800;
	else
	if (spBAUD_009600 == dwBaudSelet)
		dcb.BaudRate = CBR_9600;
	else
	if (spBAUD_038400 == dwBaudSelet)
		dcb.BaudRate = CBR_38400;
	else
	if (spBAUD_057600 == dwBaudSelet)
		dcb.BaudRate = CBR_57600;
	else
	if (spBAUD_115200 == dwBaudSelet)
		dcb.BaudRate = CBR_115200;
	else
	if (spBAUD_921600 == dwBaudSelet)
		///dcb.BaudRate = CBR_921600;
		;
	else
		dcb.BaudRate = CBR_4800;

    dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fTXContinueOnXoff = TRUE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fAbortOnError = FALSE;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

	SetCommState(hTestComm, &dcb);

	GetCommTimeouts (hTestComm, &CommTimeouts);

	// Change the COMMTIMEOUTS structure settings.
	CommTimeouts.ReadIntervalTimeout = MAXDWORD;  
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;  
	CommTimeouts.ReadTotalTimeoutConstant = 0;    
	CommTimeouts.WriteTotalTimeoutMultiplier = 10;  
	CommTimeouts.WriteTotalTimeoutConstant = 1000;    
	
	// Set the time-out parameters for all read and write operations
	// on the port. 
	if (!SetCommTimeouts (hTestComm, &CommTimeouts))
	{
	  // Could not set the time-out parameters.
	  NKDbgPrintfW( TEXT("Unable to set the time-out parameters.\r\n") );
	  MessageBox(hwnd, L"Unable to set the time-out parameters!!", L"Error", MB_OK);
	}


	// Specify a set of events to be monitored for the port.
	SetCommMask (hTestComm, EV_RXCHAR | EV_RLSD | EV_RXFLAG |EV_ERR);

	while (hTestComm != INVALID_HANDLE_VALUE) 
	{
		// Wait for an event to occur for the port.
		WaitCommEvent (hTestComm, &dwCommModemStatus, 0);
		
///		NKDbgPrintfW( TEXT("\r\nComEvent: ") );
		NKDbgPrintfW( TEXT("\r\n>>>") );
		
		// Re-specify the set of events to be monitored for the port.
		SetCommMask (hTestComm, EV_RXCHAR | EV_RLSD | EV_RXFLAG | EV_ERR);

		if (dwCommModemStatus & EV_RXCHAR) 
		{
			// Loop for waiting for the data.
			do 
			{
				// Read the data from the serial port.
				ReadFile (hTestComm, &Byte, 1, &dwBytesTransferred, NULL);

				// Display the data read.
				if (dwBytesTransferred == 1)
				{
					NKDbgPrintfW( TEXT("%c"), Byte);				
///					NKDbgPrintfW( TEXT("%c,"), Byte);
					tcByte = 0x00ff&(TCHAR)Byte;

					/// show to screen !!
					if( !bScreenPause )
						ShowToScreen(hwnd);
				}

			} while (dwBytesTransferred == 1);		/// read until no data in it !!
		}
		if (dwCommModemStatus & EV_RLSD) 
		{
			NKDbgPrintfW( TEXT("SPD@AccessComPort(): EV_RLSD event !! \r\n") );
		}
		if (dwCommModemStatus & EV_RXFLAG) 
		{
			NKDbgPrintfW( TEXT("SPD@AccessComPort(): EV_RLSD event !! \r\n") );
			ReadFile (hTestComm, &Byte, 1, &dwBytesTransferred, 0);
				if (dwBytesTransferred == 1)
					NKDbgPrintfW( TEXT("\r\n:%c.\r\n"), Byte);

		}
		if (dwCommModemStatus & EV_ERR) 
		{
			NKDbgPrintfW( TEXT("SPD@AccessComPort(): EV_ERR event !! \r\n") );
			
		}

		if ( TRUE == bForcePortClose )	///force close ComPort
			break;

	}/// end of while()

	CloseHandle(hTestComm);
	bPortOpened = FALSE;

}


int WINAPI WinMain(	HINSTANCE hInstance,
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

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_COMPORT);

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
	WNDCLASS	wc;

    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= (WNDPROC) WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COMPORT));
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
	LoadString(hInstance, IDC_COMPORT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}

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
	HDC hdc;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	TCHAR szHello[MAX_LOADSTRING];

	HWND hWndMB = NULL;
	DWORD dwBytesTransferred = 0;

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
				case IDM_FILE_LIST:
				   DialogBox(hInst, (LPCTSTR)IDD_FILELISTBOX1, hWnd, (DLGPROC)Filelist1);
				   break;
				case IDM_FILE_EXIT:
				   DestroyWindow(hWnd);
				   break;
				case IDM_PORT_COM1:
				   dwComSelet = spCOM_1;
				   break;
				case IDM_PORT_COM2:
				   dwComSelet = spCOM_2;
				   break;
				case IDM_PORT_COM3:
				   dwComSelet = spCOM_3;
				   break;
				case IDM_PORT_COM4:
				   dwComSelet = spCOM_4;
				   break;
				case IDM_PORT_COM5:
				   dwComSelet = spCOM_5;
				   break;
				case IDM_PORT_COM6:
				   dwComSelet = spCOM_6;
				   break;
				case IDM_PORT_COM7:
				   dwComSelet = spCOM_7;
				   break;


				case IDM_BAUDRATE_1200:
				   dwBaudSelet = spBAUD_001200;
				   break;
				case IDM_BAUDRATE_2400:
				   dwBaudSelet = spBAUD_002400;
				   break;
				case IDM_BAUDRATE_4800:
				   dwBaudSelet = spBAUD_004800;
				   break;
				case IDM_BAUDRATE_9600:
				   dwBaudSelet = spBAUD_009600;
				   break;
				case IDM_BAUDRATE_19200:
				   dwBaudSelet = spBAUD_019200;
				   break;
				case IDM_BAUDRATE_38400:
				   dwBaudSelet = spBAUD_038400;
				   break;
				case IDM_BAUDRATE_57600:
				   dwBaudSelet = spBAUD_057600;
				   break;
				case IDM_BAUDRATE_115200:
				   dwBaudSelet = spBAUD_115200;
				   break;
				case IDM_BAUDRATE_921600:
				   dwBaudSelet = spBAUD_921600;
				   break;

				case IDM_OPERATION_START:
				    if ( TRUE == bPortOpened)
					{
						MessageBox(hWnd, L"Com Port Already Open !!", L"Error", MB_OK);
						break;
					}

					/// span
					hDebugThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AccessComPort, (LPVOID)hWnd, 0, NULL );
					if ( !hDebugThread )
						;///_EXCPLOG("Create spDoCommandThread Fail!!\r\n");
					else
						CloseHandle(hDebugThread);
					/// span

					Sleep( 300 );

				   break;
				case IDM_OPERATION_STOP:
					bForcePortClose = TRUE;
					Sleep( 300 );
				   break;
				case IDM_OPERATION_CLEAR:

				   break;
				case IDM_OPERATION_PAUSE:
					if( bScreenPause )
						bScreenPause = FALSE;
					else
						bScreenPause = TRUE;
				   break;
				case IDM_OPERATION_TMC_VERSION:
					///DWORD dwBytesTransferred = 0;
					BOOL bWriteRet;
					BYTE bCommand[5];
					bCommand[0] = (char)0xFF;
					bCommand[1] = (char)0x56;	///"V"
					bCommand[2] = (char)0x78;	///"x"
					bCommand[3] = (char)0x78;	///"x"
					bCommand[4] = (char)0x56;	///"V"
					 
					bWriteRet = WriteFile(hComPort, (LPCVOID)bCommand, 5, &dwBytesTransferred, NULL);
					if( 0 == bWriteRet )
						MessageBox(hWnd, L"Send Version command fail!!", L"Error", MB_OK);
					else
						MessageBox(hWnd, L"Send Version command ok!!", L"Note", MB_OK);
						
				   break;
				default:

				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			///handle the Menu bar
			if( hwndCB )
			{				
				HMENU	hMenuBar = CommandBar_GetMenu( hwndCB, 0 );
				CheckMenuItem( hMenuBar, IDM_PORT_COM1, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_PORT_COM2, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_PORT_COM3, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_PORT_COM4, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_PORT_COM5, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_PORT_COM6, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_PORT_COM7, MF_BYCOMMAND | MF_UNCHECKED);

				CheckMenuItem( hMenuBar, IDM_BAUDRATE_1200, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_BAUDRATE_2400, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_BAUDRATE_4800, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_BAUDRATE_9600, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_BAUDRATE_19200, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_BAUDRATE_38400, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_BAUDRATE_57600, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_BAUDRATE_115200, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem( hMenuBar, IDM_BAUDRATE_921600, MF_BYCOMMAND | MF_UNCHECKED);

				if (spBAUD_001200 == dwBaudSelet)
					CheckMenuItem( hMenuBar, IDM_BAUDRATE_1200, MF_BYCOMMAND | MF_CHECKED);
				else
				if (spBAUD_002400 == dwBaudSelet)
					CheckMenuItem( hMenuBar, IDM_BAUDRATE_2400, MF_BYCOMMAND | MF_CHECKED);
				else
				if (spBAUD_004800 == dwBaudSelet)
					CheckMenuItem( hMenuBar, IDM_BAUDRATE_4800, MF_BYCOMMAND | MF_CHECKED);
				else
				if (spBAUD_009600 == dwBaudSelet)
					CheckMenuItem( hMenuBar, IDM_BAUDRATE_9600, MF_BYCOMMAND | MF_CHECKED);
				else
				if (spBAUD_038400 == dwBaudSelet)
					CheckMenuItem( hMenuBar, IDM_BAUDRATE_38400, MF_BYCOMMAND | MF_CHECKED);
				else
				if (spBAUD_057600 == dwBaudSelet)
					CheckMenuItem( hMenuBar, IDM_BAUDRATE_57600, MF_BYCOMMAND | MF_CHECKED);
				else
				if (spBAUD_115200 == dwBaudSelet)
					CheckMenuItem( hMenuBar, IDM_BAUDRATE_115200, MF_BYCOMMAND | MF_CHECKED);
				else
				if (spBAUD_921600 == dwBaudSelet)
					CheckMenuItem( hMenuBar, IDM_BAUDRATE_921600, MF_BYCOMMAND | MF_CHECKED);


				if( spCOM_1 == dwComSelet )
					CheckMenuItem( hMenuBar, IDM_PORT_COM1, MF_BYCOMMAND | MF_CHECKED);
				else
				if( spCOM_2 == dwComSelet )
					CheckMenuItem( hMenuBar, IDM_PORT_COM2, MF_BYCOMMAND | MF_CHECKED);
				else
				if( spCOM_3 == dwComSelet )
					CheckMenuItem( hMenuBar, IDM_PORT_COM3, MF_BYCOMMAND | MF_CHECKED);
				else
				if( spCOM_4 == dwComSelet )
					CheckMenuItem( hMenuBar, IDM_PORT_COM4, MF_BYCOMMAND | MF_CHECKED);
				else
				if( spCOM_5 == dwComSelet )
					CheckMenuItem( hMenuBar, IDM_PORT_COM5, MF_BYCOMMAND | MF_CHECKED);
				else
				if( spCOM_6 == dwComSelet )
					CheckMenuItem( hMenuBar, IDM_PORT_COM6, MF_BYCOMMAND | MF_CHECKED);
				else
				if( spCOM_7 == dwComSelet )
					CheckMenuItem( hMenuBar, IDM_PORT_COM7, MF_BYCOMMAND | MF_CHECKED);


			}
			
			break;
		case WM_CREATE:
			hwndCB = CommandBar_Create(hInst, hWnd, 1);			
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


// Mesage handler for the About box.
LRESULT CALLBACK Filelist1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	HWND hListTree;

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
			if ( LOWORD(wParam) == IDFLASH )
			{
				hListTree = GetDlgItem(hDlg, IDC_FILETREE1);
				AddItemToTree(hListTree, TEXT("\\"), NULL, TRUE);
				return TRUE;
			}
			
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

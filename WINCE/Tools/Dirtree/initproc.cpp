// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1997  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:     initproc.cpp
//
//  PURPOSE:    Contains application initialization routines.
//
//  PLATFORMS:  Windows CE
//
//  FUNCTIONS:
//      InitApplication()   - Register window classes
//      InitInstance()      - Create and display main windows
//
//  COMMENTS:
//
//

#include <windows.h>
#include <commctrl.h>
#include "globals.h"
#include "resource.h"

//----------------------------------------------------------------------------
// Application global variables

HINSTANCE g_hInstance = NULL;


//
//  FUNCTION:   InitApplication(HINSTANCE)
//
//  PURPOSE:    Registers the application's window class(es).
//
//  PARAMETERS:
//      hInstance   - handle that uniquely identifies this application instance
//
//  RETURN VALUE:
//      (BOOL) Returns TRUE if the window classes are registered successfully
//             Returns FALSE otherwise
//
//  COMMENTS:
//

#define DEFBKGDCOLOR (COLOR_WINDOW + 1)
BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASS  wc;
	
	// The window class is not registered yet, so we have to fill a
	// window class structure with parameters that describe the
	// window class.
	wc.style         = 0;
	wc.lpfnWndProc   = MainWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hCursor       = NULL;
	wc.lpszMenuName  = NULL;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));;
	wc.hbrBackground = (HBRUSH)DEFBKGDCOLOR;
	wc.lpszClassName = TEXT("DirTreeWin");
	
	// Attempt to register the class first with the WNDCLASSEX structure
	// and if that doesn't work try just the WNDCLASS for NT and Win32s.
	if (!RegisterClass(&wc))
	{
		ErrorHandler();
		return FALSE;
	}
	return (TRUE);
}


//
//  FUNCTION:   InitInstance(HINSTANCE, int)
//
//  PURPOSE:    Creates and displays the application's initial window(s).
//
//  PARAMETERS:
//      hInstance   - handle which uniquely identifies this instance
//      nCmdShow    - determines the state which the window should be initially
//                    shown in
//
//  RETURN VALUE:
//      (BOOL) Returns TRUE if the window(s) are created successfully.
//             Returns FALSE otherwise.
//
//  COMMENTS:
//

BOOL InitInstance(HINSTANCE  hInstance, int nCmdShow)
{
	HWND hwnd;                       // temporary window handle
	BOOL bStatus = FALSE;            // holds return status for function
	SIPINFO si = {0};
	//Set default window creation sizd info
	int iDelta, x = CW_USEDEFAULT, y = CW_USEDEFAULT, cx, cy;	
	
	// Save the instance handle in global variable, which will be used in
	// many subsequent calls from this application to Windows
	g_hInstance = hInstance;
	
	
 	////////// Setting default main window size
	// This technique allows for you to create the main
	// window to allow for the postion of a menubar and/or
	// the SIP button at the bottom of the screen
	

	
	si.cbSize = sizeof(si);
	SHSipInfo(SPI_GETSIPINFO, 0, &si, 0);
		
	//Consider the menu at the bottom, please.
	iDelta = (si.fdwFlags & SIPF_ON) ? 0 : MENU_HEIGHT;
	cx = si.rcVisibleDesktop.right - si.rcVisibleDesktop.left;
	cy = si.rcVisibleDesktop.bottom - si.rcVisibleDesktop.top - iDelta;
		
	
	hwnd = CreateWindow(TEXT("DirTreeWin"), TEXT("DirTree"), WS_VISIBLE ,
		x, y, cx, cy, NULL, NULL, hInstance, NULL);
	
	// If the window was successfully created, make the window visible,
	// update its client area, and return "success".  If the window
	// was not created, return "failure"
	if (hwnd)
	{
		ShowWindow(hwnd, nCmdShow); // Set to visible & paint non-client area
		UpdateWindow(hwnd);         // Tell window to paint client area
		
		bStatus = TRUE;             // Indicate success, default is failure
	}
	else
	{
		ErrorHandler();
	}
	
	return bStatus;                  // Return status code
}





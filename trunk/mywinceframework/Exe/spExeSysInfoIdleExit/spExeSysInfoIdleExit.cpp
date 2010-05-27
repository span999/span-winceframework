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
#include <windows.h>
#include <drvlib.h>

#include "..\..\Inc\spLibErrCodeDef.h"
#include "SPDebugDef.h"


///
// Debugging defines and functions for KITL output
#define MSG_INFO 0
#define MSG_TRACE 0
#define MSG_WARNING 0
#define MSG_ERROR 1


#define NAMED_SYSTEM_IDLE_CHECK_EXIT_EVENT		TEXT("named system idle check exit event")




///phototype



///
static TCHAR szString_[64];         	// Temperary string


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



int WINAPI WinMain (
              HINSTANCE hInstance,    // Handle to the current instance
              HINSTANCE hPrevInstance,// Handle to the previous instance
              LPTSTR lpCmdLine,       // Pointer to the command line
              int nCmdShow)           // Show state of the window
{

	HANDLE hExitEvent = NULL;
	
	hExitEvent = CreateEvent( NULL, FALSE, FALSE, NAMED_SYSTEM_IDLE_CHECK_EXIT_EVENT );
	
	MessageBox( NULL, TEXT("Send Exit Event ???"), TEXT("MainRoutine"), MB_OK );
	SetEvent( hExitEvent );
	
	CloseHandle( hExitEvent );

	return 0;
}




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


static DWORD MainRoutine( DWORD dwPararm );




/// *****************************************
/// routine
/// *****************************************




int WINAPI WinMain (
              HINSTANCE hInstance,    // Handle to the current instance
              HINSTANCE hPrevInstance,// Handle to the previous instance
              LPTSTR lpCmdLine,       // Pointer to the command line
              int nCmdShow)           // Show state of the window
{


	MainRoutine( 0 );


	return 0;
}





static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;
	DWORD dwZero = 5;
	DWORD *pdwVaule = NULL;
	
	MessageBox( NULL, TEXT("go illegal Memory Access !!!"), TEXT("MainRoutine"), MB_ICONQUESTION|MB_OKCANCEL );
	
	for( dwZero = 5; dwZero < 100; dwZero-- )
	{
		dwRet = 456789 / dwZero;
	}	

	///damage !!!
	*pdwVaule = 0x11111111;
	
	return dwRet;
}



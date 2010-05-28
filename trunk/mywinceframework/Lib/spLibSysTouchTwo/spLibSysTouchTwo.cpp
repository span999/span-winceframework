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

#define		NAMED_TOUCHTWO_SIZE_UP						TEXT("named touchtwo size up event")
#define		NAMED_TOUCHTWO_SIZE_DOWN					TEXT("named touchtwo size down event")
#define		NAMED_TOUCHTWO_ROTATE_CLOCKWISE				TEXT("named touchtwo rotate clockwise event")
#define		NAMED_TOUCHTWO_ROTATE_ANTICLOCKWISE			TEXT("named touchtwo rotate anti clockwise event")


#define 	TWOFINGER_MIN_TIME_INTERVAL			13
#define 	TWOFINGER_MIN_DISTANCE				25



typedef DWORD (*PFN_GetIdleTime)(void);

///phototype
static BOOL spLibInitContent( DWORD dwParam );
static int spLibTwoPointDistance( int x1, int y1, int x2, int y2 );
static BOOL spLibTouch2Process( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

static PFN_GetIdleTime gpfGetIdleTime;



/// *****************************************
/// interface functions
/// *****************************************

DWORD spLibSysTouchTwo_Init( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibSysTouchTwo_Init: %d \r\n", 1) );
	

		
	SPDMSG( dINIT, (L"$$$spLibSysTouchTwo_Init: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibSysTouchTwo_Deinit( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibSysTouchTwo_Deinit: %d \r\n", 1) );
	

	
	SPDMSG( dINIT, (L"$$$spLibSysTouchTwo_Deinit: %d \r\n", 0) );
	return dwRet;
}


BOOL spLibSysTouchTwo_Process( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	DWORD bRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibSysTouchTwo_Process: %d \r\n", 1) );
	
	bRet = spLibTouch2Process( hWnd, message, wParam, lParam );
	
	SPDMSG( dINFO, (L"$$$spLibSysTouchTwo_Process: %d \r\n", 0) );
	
	return bRet;
}




/// *****************************************
/// internal functions
/// *****************************************
static BOOL spLibFirstLoad( void )
{
	BOOL bRet = TRUE;
	
	///if( (0 == pThisContent->dwStartTick) || (0 == pThisContent->dwStartTick) )
	if( 1 )
		bRet = TRUE;
	else
		bRet = FALSE;
		
	return bRet;
}


static BOOL spLibInitContent( DWORD dwParam )
{
	BOOL bRet = TRUE;

	
	return bRet;
}



static BOOL spLibLoadMyModule(void)
{
    HMODULE hCore;	
    BOOL bRet = FALSE;
	
    /// init system function call
    hCore = (HMODULE)LoadLibrary( L"coredll.dll" );
    if ( hCore ) {
        gpfGetIdleTime = (PFN_GetIdleTime)GetProcAddress( hCore, L"GetIdleTime" );
        if ( !gpfGetIdleTime )
        {
            FreeLibrary(hCore);
            bRet = FALSE;
		}
        else
            bRet = TRUE;
    }
    
    return bRet;	
}



static int spLibTwoPointDistance( int x1, int y1, int x2, int y2 )
{
	return (int)sqrt( pow(abs(x1-x2),2) + pow(abs(y1-y2),2) );
}



static BOOL spLibTouch2Process( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	BOOL bRet = FALSE;

	static int myxPrv, myyPrv;
	static int myxMovO, myyMovO;
	static int myxMovR, myyMovR;
	static int myDistOR, myDistPrv;
	static DWORD dwTickCount = 0;
	static BOOL bTwoFinger = FALSE;
	int myxPos, myyPos, myDist;
///	RECT rtTemp;
	static HANDLE		hEventSizeUp = NULL;
	static HANDLE		hEventSizeDown = NULL;
	static HANDLE		hEventRotateClockwise = NULL;
	static HANDLE		hEventRotateAntiClockwise = NULL;

	
	myxPos = LOWORD(lParam);
	myyPos = HIWORD(lParam);
	

	if( NULL == hEventSizeDown )
	{
		hEventSizeUp = CreateEvent( NULL, FALSE, FALSE, NAMED_TOUCHTWO_SIZE_UP );
		hEventSizeDown = CreateEvent( NULL, FALSE, FALSE, NAMED_TOUCHTWO_SIZE_DOWN );
		hEventRotateClockwise = CreateEvent( NULL, FALSE, FALSE, NAMED_TOUCHTWO_ROTATE_CLOCKWISE );
		hEventRotateAntiClockwise = CreateEvent( NULL, FALSE, FALSE, NAMED_TOUCHTWO_ROTATE_ANTICLOCKWISE );
	}
	
	
	if( 
		WM_LBUTTONDOWN == message ||
		WM_LBUTTONUP == message
	)
	{
		///clear flag
		bTwoFinger = FALSE;
		///DrawTEXTtoPanel( hWnd, TEXT("up-down-%d,%d,%d,%d"), myxPos, myyPos, myxPrv, myyPrv );
		bRet = FALSE;
		
		///DrawRECTtoPanel( hWnd, &rtTemp, 0 );
	}
	else	///WM_MOUSEMOVE == message
	{	
		///DrawTEXTtoPanel( hWnd, TEXT("mov-%d,%d,%d,%d,%d"), myxPos, myyPos, myxPrv, myyPrv, spLibTwoPointDistance(myxPos,myyPos,myxPrv,myyPrv) );
		
		if( !bTwoFinger )	///not two fingers yet
		{
			myDist = spLibTwoPointDistance(myxPos,myyPos,myxPrv,myyPrv);
			if( 
				((GetTickCount() - dwTickCount) < TWOFINGER_MIN_TIME_INTERVAL) 
				&&
				( myDist > TWOFINGER_MIN_DISTANCE )
///				( abs(myxPrv-myxPos) > 25 || abs(myyPrv-myyPos) > 25 )
///				( spLibTwoPointDistance(myxPos,myyPos,myxPrv,myyPrv) > TWOFINGER_MIN_DISTANCE )
			)
			{
				///we have two fingers now!!
				bTwoFinger = TRUE;
				myxMovR = myxPos;
				myyMovR = myyPos;
				myxMovO = myxPrv;
				myyMovO = myyPrv;
				myDistOR = spLibTwoPointDistance(myxMovR,myyMovR,myxMovO,myyMovO);
				myDistPrv = myDistOR;
				
				///DrawTEXTtoPanel( hWnd, TEXT("!!Two fingers!!%d"), myDistOR );
				
				
				bRet = TRUE;
			}
			else
			{
				bTwoFinger = FALSE; 
				bRet = FALSE;
				///DrawTEXTtoPanel( hWnd, TEXT("mov-%d,%d,%d,%d,%d"), myxPos, myyPos, myxPrv, myyPrv, myDist );
			}
		}
		else	///two figners on!
		{
			///check two fingers distance
			myDist = spLibTwoPointDistance(myxPos,myyPos,myxMovO,myyMovO);
			
			///DrawTEXTtoPanel( hWnd, TEXT("Two move") );
			
			///second finger release
			if( myDist < (TWOFINGER_MIN_DISTANCE/2) )
			{
				bTwoFinger = FALSE;
				bRet = FALSE;
				///DrawTEXTtoPanel( hWnd, TEXT("second fingers release") );
			}	
			else	///two fingers operation
			{
				///check zoom operation
				if( (myDist - myDistPrv) > 0 )
				{
					///bigger
					///DrawTEXTtoPanel( hWnd, TEXT("Bigger") );
					///DrawRECTtoPanel( hWnd, &rtTemp, 1 );
					///CtrlBacklight( 1 );
					SetEventData( hEventSizeUp, (myDist - myDistPrv) );
					SetEvent( hEventSizeUp );
				}
				else
				{
					///smaller
					///DrawTEXTtoPanel( hWnd, TEXT("Smaller") );
					///DrawRECTtoPanel( hWnd, &rtTemp, 2 );
					///CtrlBacklight( 2 );
					SetEventData( hEventSizeDown, (myDistPrv - myDist) );
					SetEvent( hEventSizeDown );
				}
				
				///check rotation 
				if( myxMovO > myxPos )
				{
					if( myyPos > myyMovO )
					{
						///anti-clockwise
						///DrawTEXTtoPanel( hWnd, TEXT("anti-clockwise") );
						SetEvent( hEventRotateAntiClockwise );
					}
					else
					{
						///clockwise
						///DrawTEXTtoPanel( hWnd, TEXT("clockwise") );
						SetEvent( hEventRotateClockwise );
					}
				}
				else
				{
					if( myyPos > myyMovO )
					{
						///clockwise
						///DrawTEXTtoPanel( hWnd, TEXT("clockwise") );
						SetEvent( hEventRotateClockwise );
					}
					else
					{
						///anti-clockwise
						///DrawTEXTtoPanel( hWnd, TEXT("anti-clockwise") );
						SetEvent( hEventRotateAntiClockwise );
					}
				}
				
				///update stored value
				myDistPrv = myDist;
				
				bRet = TRUE;
			}
		}

	}

	///update variable
	myxPrv = myxPos;
	myyPrv = myyPos;

	///update tickcount
	dwTickCount = GetTickCount();
	
	return bRet;

}



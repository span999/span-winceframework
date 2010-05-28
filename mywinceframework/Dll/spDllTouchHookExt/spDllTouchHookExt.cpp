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
#include "spOS.h"
#include "spPlatform.h"
#include "spCommon.h"
#include "SPDebugDef.h"
#include "spDllTouchHookExt.h"
#include "spLibDDrawClass.h"
#include "spLibNumPadParser.h"


#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only
#define		SPPREFIX			TEXT("THEDll:")



static BOOL Dll_Init( void );

///static DWORD 	dwPreTickCount = 0;
///static spLibDDraw	*pmyDD = NULL;






void TouchHookExt_Down( TOUCHHOOKEXT_ACT_TYPE Type, TOUCH_PANEL_SAMPLE_FLAGS Flags, INT X, INT Y )
{
	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchHookExt_Down +++"), SPPREFIX );
	
	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchHookExt_Down +++"), SPPREFIX );
	return;
}

void TouchHookExt_Move( TOUCHHOOKEXT_ACT_TYPE Type, TOUCH_PANEL_SAMPLE_FLAGS Flags, INT X, INT Y )
{
	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchHookExt_Move +++"), SPPREFIX );
	
	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchHookExt_Move +++"), SPPREFIX );
	return;
}

void TouchHookExt_Up( TOUCHHOOKEXT_ACT_TYPE Type, TOUCH_PANEL_SAMPLE_FLAGS Flags, INT X, INT Y )
{
	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchHookExt_Up +++"), SPPREFIX );
	
	if( Dll_Init() )
		if( spLibParseSequenceCondition( spLibParseAreaCondition( X, Y ) ) )
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchHookExt_Up Hit!!!"), SPPREFIX );
			///pmyDD->spLibTextDraw( 200, 200, TEXT("HIT!!!, X =>%4d, Y=>%4d!!!"), X, Y );
	
	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchHookExt_Up +++"), SPPREFIX );
	return;
}

void TouchHookExt_Point( TOUCHHOOKEXT_ACT_TYPE Type, TOUCH_PANEL_SAMPLE_FLAGS Flags, INT X, INT Y )
{
	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchHookExt_Point +++"), SPPREFIX );
	
	///spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchHookExt_Point +++"), SPPREFIX );
	return;
}


BOOL WINAPI  DllEntry( HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved )
{
      switch (dwReason)
      {
	  	case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls((HMODULE) hinstDll);
			SPDMSG(dINIT, (_T("SPD@%s() : Process Attach\r\n"), _T(__FUNCTION__)));
			break;

		case DLL_PROCESS_DETACH:
			SPDMSG(dINIT, (_T("SPD@%s() : Process Detach\r\n"), _T(__FUNCTION__)));
			spLibParseAreaDeInit();
			break;
      	}

    return TRUE;
}


static BOOL Dll_Init( void )
{
	BOOL bRet = FALSE;
	static BOOL bInited = FALSE;
	
	if( !bInited && IsAPIReady( SH_WMGR ) )
	{
		spLibParseAreaInit();	
		bInited = TRUE;
	}	
	
	bRet = bInited;
	
	return bRet;
}



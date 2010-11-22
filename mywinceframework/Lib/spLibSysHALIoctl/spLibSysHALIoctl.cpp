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
#include "SPDebugDef.h"
///#include "mio_ioctl.h"
///#include "odm_ioctl.h"
#include "spLibDbgMsgBuf.h"


typedef DWORD (*PFN_GetIdleTime)(void);

///phototype
static BOOL spLibInitContent( DWORD dwParam );
static BOOL spLibLoadMyModule( void );
static BOOL spLibFirstLoad( void );
static DWORD spLibHalReboot( void );
static DWORD spLibHalBLGet( void );
static DWORD spLibHalBLSet( DWORD dwValue );
static DWORD spLibHalGetDbgMsgBuf( SPDBG_MSG_INFO *pDbgMsgInfo );


static PFN_GetIdleTime gpfGetIdleTime;



/// *****************************************
/// interface functions
/// *****************************************

DWORD spLibSysHALIoctl_Init( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibSysHALIoctl_Init: %d \r\n", 1) );
	

		
	SPDMSG( dINIT, (L"$$$spLibSysHALIoctl_Init: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibSysHALIoctl_Deinit( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibSysHALIoctl_Deinit: %d \r\n", 1) );
	

	
	SPDMSG( dINIT, (L"$$$spLibSysHALIoctl_Deinit: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibSysHALIoctl_Reboot( void )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibSysHALIoctl_Reboot: %d \r\n", 1) );
	
	dwRet = spLibHalReboot();
	
	SPDMSG( dINFO, (L"$$$spLibSysHALIoctl_Reboot: %d \r\n", 0) );
	
	return dwRet;
}


DWORD spLibSysHALIoctl_BacklightGet( void )
{
	return spLibHalBLGet();
}


DWORD spLibSysHALIoctl_BacklightSet( DWORD dwValue )
{
	return spLibHalBLSet( dwValue );
}

DWORD spLibSysHALIoctl_BacklightUp( void )
{
	return spLibHalBLSet( spLibHalBLGet()+1 );
}

DWORD spLibSysHALIoctl_BacklightDown( void )
{
	return spLibHalBLSet( spLibHalBLGet()-1 );
}

///#ifdef IOCTL_HAL_GET_DBG_MSG_BUF
DWORD spLibSysHALIoctl_GetDbgMsgBuf( PVOID pBuf )
{
	DWORD dwRet = 0;
	SPDBG_MSG_INFO spTmpBuf;
	
	if( pBuf )
	{
		memset( &spTmpBuf, 0, sizeof(SPDBG_MSG_INFO) );
		dwRet = spLibHalGetDbgMsgBuf( &spTmpBuf );
		if( (-1) != dwRet )
		{
			memset( pBuf, 0, sizeof(SPDBG_MSG_INFO) );
			memcpy( pBuf, &spTmpBuf, dwRet );
		}
		else
		{
			dwRet = 0;
		}
		
	}
	else
	{	///query buffer size
		dwRet = spLibHalGetDbgMsgBuf( NULL );
	}
	
	return dwRet;
}
///#endif


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




static DWORD spLibHalReboot( void )
{
	DWORD dwRet = 0;
	BOOL bRet = 0;
	
	bRet = KernelIoControl( IOCTL_HAL_REBOOT, NULL, 0, NULL, 0, NULL ); 
	
	if( !bRet )
		dwRet = (-1);
		
	return dwRet;
}



static DWORD spLibHalBLGet( void )
{
	DWORD dwRet;
	DWORD dwOutBuf;
	
	if( !KernelIoControl( IOCTL_MIO_BKL_GET_BRIGHTNESS, NULL, 0, (DWORD *)&dwOutBuf, sizeof(DWORD), NULL ) )
		if( !KernelIoControl( IOCTL_ODM_BKL_GET_BRIGHTNESS, NULL, 0, (DWORD *)&dwOutBuf, sizeof(DWORD), NULL ) )
		{
			SPDMSG( dFAIL, (L"$$$spLibHalBLGet: fail !!! %d \r\n", 0) );
			return 0;
		}	
	
	dwRet = dwOutBuf;
	
	return dwRet;
}


static DWORD spLibHalBLSet( DWORD dwValue )
{
	DWORD dwRet;
	DWORD dwInBuf;
	
	dwInBuf = dwValue;
	
	if( !KernelIoControl(IOCTL_MIO_BKL_SET_BRIGHTNESS,(DWORD *)&dwInBuf, sizeof(DWORD), NULL, 0, NULL ) )
		if( !KernelIoControl(IOCTL_ODM_BKL_SET_BRIGHTNESS,(DWORD *)&dwInBuf, sizeof(DWORD), NULL, 0, NULL ) )
		{
			SPDMSG( dFAIL, (L"$$$spLibHalBLSet: fail !!! %d \r\n", 0) );
			return 0;
		}	
	
	dwRet = spLibHalBLGet();
	
	return dwRet;
}


static DWORD spLibHalGetDbgMsgBuf( SPDBG_MSG_INFO *pDbgMsgInfo )
{
	DWORD dwRet = 0;
	
	if( pDbgMsgInfo )
	{
#ifdef IOCTL_HAL_GET_DBG_MSG_BUF
		if( !KernelIoControl( IOCTL_HAL_GET_DBG_MSG_BUF,  NULL, 0, pDbgMsgInfo,  sizeof(SPDBG_MSG_INFO), NULL ) )
		{
			SPDMSG( dFAIL, (L"$$$spLibHalGetDbgMsgBuf: fail !!! %d \r\n", 0) );
			dwRet = (-1);
		}
		else
		{
			dwRet = sizeof(SPDBG_MSG_INFO);
		}
#else
		dwRet = sizeof(SPDBG_MSG_INFO);
#endif		
	}
	else
	{	///query for buffer size
		dwRet = sizeof(SPDBG_MSG_INFO);
	}
	
	return dwRet;
}



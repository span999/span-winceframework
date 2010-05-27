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
#include <ddraw.h>

#include "..\..\Inc\spLibErrCodeDef.h"
#include "..\..\Inc\spLibDDrawClass.h"
#include "SPDebugDef.h"





BOOL spLibDDraw::Init( void )
{
	BOOL bRet = FALSE;
	
	///bRet = PriInit();
	
	InitializeCriticalSection( &m_csDDaccess );
	
	m_pDD = NULL;
	m_pDDSPrimary = NULL;
	m_hWnd = NULL;
	m_bSurfaceEnumFunctionCalled = FALSE;

	///m_dwLogStatus |= SPLOG_STATUS_FLAG_INITED;
	return bRet;
}

spLibDDraw::spLibDDraw( HWND hWnd )
{
	BOOL bRet = FALSE;
	
	bRet = Init();
	
	m_hWnd = hWnd;
	
}


BOOL spLibDDraw::DeInit( void )
{
	BOOL bRet = FALSE;

	m_pDDSPrimary->Release();
	m_pDDSPrimary = NULL;
	m_pDD->Release();
	m_pDD = NULL;
	m_hWnd = NULL;	
	DeleteCriticalSection( &m_csDDaccess );

	return bRet;
}


DWORD spLibDDraw::spLibInitDDraw( void )
{
	DWORD dwRet = 0;
	
	dwRet = InitDDraw();
	if( 0 == dwRet )
		dwRet = InitDDPrimarySurface();
		
	return dwRet;
}


DWORD spLibDDraw::spLibInitDDrawBack( DWORD dwBackSurfNum )
{
	DWORD dwRet = 0;
	
	dwRet = InitDDraw();
	if( 0 == dwRet )
		dwRet = InitDDPrimarySurfaceBack( dwBackSurfNum );
		
	return dwRet;
}

DWORD spLibDDraw::spLibInitDDrawBack( DWORD dwBackSurfNum, PVOID pfn )
{
	DWORD dwRet = 0;
	
	dwRet = InitDDraw();
	if( 0 == dwRet )
		dwRet = InitDDPrimarySurfaceBack( dwBackSurfNum, pfn );
		
	return dwRet;
}


void spLibDDraw::spMessageBoxOut( LPCTSTR szError,... )
{
    TCHAR                       szBuff[128];
    va_list                     vl;

    va_start(vl, szError);

    StringCchVPrintf(szBuff, (128), szError, vl);

	MessageBox( NULL, szBuff, TEXT("spLibDDraw Msg:"), MB_OK|MB_SETFOREGROUND );
	
    va_end(vl);
}




DWORD spLibDDraw::InitDDraw( void )
{
	DWORD dwRet = (-1);
	HRESULT hr;
	
	spMessageBoxOut( TEXT("InitDDraw+++") );
	
	// Create the main DirectDraw object
	hr = DirectDrawCreate( NULL, &m_pDD, NULL );
	if( DD_OK != hr )
		spMessageBoxOut( TEXT("DirectDrawCreate fail") );
	else
	{
		if( NULL == m_hWnd )
		{
			// Get normal mode.
			hr = m_pDD->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL );
		}
		else
		{
			// Get Fullscreen mode.
			hr = m_pDD->SetCooperativeLevel( m_hWnd, DDSCL_FULLSCREEN );
		}
		
		if( DD_OK != hr )
		{
			spMessageBoxOut( TEXT("SetCooperativeLevel fail") );	
		}
		else
			dwRet = 0;
	}

	spMessageBoxOut( TEXT("InitDDraw---") );
	
	return dwRet;
}

BOOL spLibDDraw::GetDDCaps( void )
{
	BOOL bRet = FALSE;
	
	spMessageBoxOut( TEXT("GetDDCaps+++") );
	
	if( NULL != m_pDD )
	{
		HRESULT	hRet;
		
		memset( &m_ddcaps, 0, sizeof(m_ddcaps) );
		m_ddcaps.dwSize = sizeof( m_ddcaps );
		hRet = m_pDD->GetCaps( &m_ddcaps, NULL );
		
		if (hRet != DD_OK)
			spMessageBoxOut( TEXT("GetDDCaps:GetCaps fail !!!") );
		else
			bRet = TRUE;
	}
	else
		spMessageBoxOut( TEXT("GetDDCaps: m_pDD fail !!!") );
	
	spMessageBoxOut( TEXT("GetDDCaps---") );
	
	return bRet;
}


BOOL spLibDDraw::IsOverlaySupport( void )
{
	BOOL bRet = FALSE;
	
	spMessageBoxOut( TEXT("IsOverlaySupport+++") );
	
	if( GetDDCaps() )
	{
		if (m_ddcaps.dwOverlayCaps == 0) 
		{
			spMessageBoxOut( TEXT("IsOverlaySupport:Overlays are not supported in hardware!") );
			bRet = FALSE;
		}
		else
		{
			spMessageBoxOut( TEXT("IsOverlaySupport:Overlays are supported in hardware!") );
			bRet = TRUE;
		}
	}
	else
		spMessageBoxOut( TEXT("IsOverlaySupport: GetDDCaps fail !!!") );
	
	
	spMessageBoxOut( TEXT("IsOverlaySupport---") );
	
	return bRet;
}


DWORD spLibDDraw::InitDDSurface( void )
{
	DWORD dwRet = 0;
	HRESULT hr;
	
	spMessageBoxOut( TEXT("InitDDSurface+++") );
	

#if 1
	if( 0 == dwRet )
	{
		// Get a primary surface interface pointer (only needed for init.)
		
		memset(&m_ddsd, 0, sizeof(m_ddsd));
		
		m_ddsd.dwSize = sizeof(m_ddsd);
		m_ddsd.dwFlags = DDSD_CAPS;	///indicate which member is valid
		m_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		hr = m_pDD->CreateSurface( &m_ddsd, &m_pDDSPrimary, NULL );
		if( DD_OK != hr )
		{
			dwRet = 3;
			spMessageBoxOut( TEXT("CreateSurface fail") );	
		}

	}
	
	if( 0 == dwRet )
	{
		// See if we can support overlays.
		
	}
#else
	if( 0 == dwRet )
	{

		// Create the primary surface with 1 back buffer
		memset(&m_ddsd, 0, sizeof(m_ddsd));
		m_ddsd.dwSize = sizeof(m_ddsd);
		m_ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		m_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP;
		m_ddsd.dwBackBufferCount = 1;
		hr = m_pDD->CreateSurface(&m_ddsd, &m_pDDSPrimary, NULL);
		if( DD_OK != hr )
		{
			dwRet = 3;
			spMessageBoxOut( TEXT("CreateSurface fail") );
			if (hr == DDERR_NOFLIPHW)
				spMessageBoxOut( TEXT("******** Display driver doesn't support flipping surfaces. ********"));
		}
	}
#endif
	spMessageBoxOut( TEXT("InitDDSurface---") );
	
	return dwRet;
}

DWORD spLibDDraw::InitDDPrimarySurface( void )
{
	DWORD dwRet = (-1);
	HRESULT hr;
	
	spMessageBoxOut( TEXT("InitDDPrimarySurface+++") );
	
	if( NULL != m_pDD )
	{
		// Get a primary surface interface pointer (only needed for init.)
		memset(&m_ddsd, 0, sizeof(m_ddsd));
		
		m_ddsd.dwSize = sizeof(m_ddsd);
		m_ddsd.dwFlags = DDSD_CAPS;			///indicate which member is valid
		m_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		hr = m_pDD->CreateSurface( &m_ddsd, &m_pDDSPrimary, NULL );
		if( DD_OK != hr )
			spMessageBoxOut( TEXT("InitDDPrimarySurface:CreateSurface fail") );	
		else
			dwRet = 0;
	}
	else
		spMessageBoxOut( TEXT("InitDDPrimarySurface: m_pDD fail") );	
	
	spMessageBoxOut( TEXT("InitDDPrimarySurface---") );
	
	return dwRet;
}

DWORD spLibDDraw::InitDDPrimarySurfaceBack( DWORD dwBackBufNum )
{
	DWORD dwRet = (-1);
	HRESULT hr;
	
	spMessageBoxOut( TEXT("InitDDPrimarySurfaceBack+++") );
	
	if( 0 == dwBackBufNum )
		dwBackBufNum = 1;		///set as default
	
	if( NULL != m_pDD )
	{
		// Get a primary surface interface pointer (only needed for init.)
		memset(&m_ddsd, 0, sizeof(m_ddsd));
		
		m_ddsd.dwSize = sizeof(m_ddsd);
		m_ddsd.dwFlags = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;			///indicate which member is valid
		m_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP;
	    m_ddsd.dwBackBufferCount = dwBackBufNum;
		hr = m_pDD->CreateSurface( &m_ddsd, &m_pDDSPrimary, NULL );
		if( DD_OK != hr )
			spMessageBoxOut( TEXT("InitDDPrimarySurfaceBack: Not support flipping surfaces!!") );
		else
			dwRet = 0;
	}
	else
		spMessageBoxOut( TEXT("InitDDPrimarySurfaceBack: m_pDD fail") );
		
	if( 0 == dwRet )
	{
		// Get a pointer to the back buffer
		hr = m_pDDSPrimary->EnumAttachedSurfaces( &m_pDDSBack, NULL );	///big problem here????????
		if( hr != DD_OK )
		{
			dwRet = (-1);
			spMessageBoxOut( TEXT("InitDDPrimarySurfaceBack: EnumAttachedSurfaces fail!!") );	
		}
		
	}
	
	spMessageBoxOut( TEXT("InitDDPrimarySurfaceBack---") );
	
	return dwRet;
}

DWORD spLibDDraw::InitDDPrimarySurfaceBack( DWORD dwBackBufNum, PVOID pfn )
{
	DWORD dwRet = (-1);
	HRESULT hr;
	
	spMessageBoxOut( TEXT("InitDDPrimarySurfaceBack+++") );
	
	if( 0 == dwBackBufNum )
		dwBackBufNum = 1;		///set as default
	
	if( NULL != m_pDD )
	{
		// Get a primary surface interface pointer (only needed for init.)
		memset(&m_ddsd, 0, sizeof(m_ddsd));
		
		m_ddsd.dwSize = sizeof(m_ddsd);
		m_ddsd.dwFlags = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;			///indicate which member is valid
		m_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP;
	    m_ddsd.dwBackBufferCount = dwBackBufNum;
		hr = m_pDD->CreateSurface( &m_ddsd, &m_pDDSPrimary, NULL );
		if( DD_OK != hr )
			spMessageBoxOut( TEXT("InitDDPrimarySurfaceBack: Not support flipping surfaces!!") );
		else
			dwRet = 0;
	}
	else
		spMessageBoxOut( TEXT("InitDDPrimarySurfaceBack: m_pDD fail") );
		
	if( 0 == dwRet )
	{
		// Get a pointer to the back buffer
		hr = m_pDDSPrimary->EnumAttachedSurfaces( &m_pDDSBack, (LPDDENUMSURFACESCALLBACK)pfn );
		if( hr != DD_OK )
		{
			dwRet = (-1);
			spMessageBoxOut( TEXT("InitDDPrimarySurfaceBack: EnumAttachedSurfaces fail!!") );	
		}
		
	}
	
	spMessageBoxOut( TEXT("InitDDPrimarySurfaceBack---") );
	
	return dwRet;
}


HRESULT spLibDDraw::SurfaceEnumFunctionStub( 
		LPDIRECTDRAWSURFACE pSurface,
        LPDDSURFACEDESC lpSurfaceDesc,
        LPVOID  lpContext)
{
	return 0;
}

HRESULT spLibDDraw::SurfaceEnumFunction( 
		LPDIRECTDRAWSURFACE pSurface,
        LPDDSURFACEDESC lpSurfaceDesc,
        LPVOID  lpContext)
{

	spMessageBoxOut( TEXT("SurfaceEnumFunction+++") );
	
    if (!m_bSurfaceEnumFunctionCalled) {

        *((LPDIRECTDRAWSURFACE *)lpContext) = pSurface;
        m_bSurfaceEnumFunctionCalled = TRUE;
        return DDENUMRET_OK;
    }
    else
	{
        spMessageBoxOut( TEXT("SurfaceEnumFunction: Enumerated more than surface?") );
        pSurface->Release();
        return DDENUMRET_CANCEL;
    }
	
	spMessageBoxOut( TEXT("SurfaceEnumFunction---") );
}

DWORD spLibDDraw::PrimaryFlip( void )
{
	DWORD dwRet = (-1);
	
	if( NULL != m_pDDSPrimary )
	{
		m_pDDSPrimary->Flip(NULL, 0);
		dwRet = 0;
	}
	
	return dwRet;
}


DWORD spLibDDraw::PrimaryRestore( void )
{
	DWORD dwRet = (-1);

	if( NULL != m_pDDSPrimary )
	{
		m_pDDSPrimary->Restore();
		dwRet = 0;
	}

	
	return dwRet;
}


BOOL spLibDDraw::DrawPixel( DWORD dwX, DWORD dwY )
{
	BOOL bRet = TRUE;
	
	HRESULT hr;
	RECT *LockRect = NULL;

	hr = m_pDDSPrimary->Lock( LockRect, &m_ddsd, DDLOCK_WAITNOTBUSY, NULL );
	// Set pixel 57, 97 to Red (assuming RGB565 pixel fmt)
	///int x = 20;
	///int y = 20;
	///BYTE * pPixelOffset = (BYTE*)m_ddsd.lpSurface + x * m_ddsd.lXPitch + y * m_ddsd.lPitch;
	BYTE * pPixelOffset = (BYTE*)m_ddsd.lpSurface + dwX * m_ddsd.lXPitch + dwY * m_ddsd.lPitch;
	*(WORD*)pPixelOffset = 0xf800;
	hr = m_pDDSPrimary->Unlock( LockRect );

	return bRet;
}
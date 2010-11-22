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
#include <ddraw.h>

#include "..\..\Inc\spLibErrCodeDef.h"
#include "..\..\Inc\spLibDDrawClass.h"
#include "SPDebugDef.h"






spLibDDraw::spLibDDraw( HWND hWnd )
{
	BOOL bRet = FALSE;
	
	bRet = Init();
	
	m_hWnd = hWnd;
	
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


BOOL spLibDDraw::spLibTextDraw( LPCTSTR szError,... )
{
	BOOL bRet = TRUE;
	
    va_list                     vl;

    va_start(vl, szError);

	StringCchVPrintf(m_szTempString, (256), szError, vl);
	
	///MessageBox( NULL, m_szTempString, TEXT("spLibDDraw Msg:"), MB_OK|MB_SETFOREGROUND );
	///bRet = TextDraw( m_pDDSPrimary, m_szTempString );
	bRet = TextDraw( 50, 50, m_pDDSPrimary, m_szTempString );
	
    va_end(vl);
	
	return bRet;
}

BOOL spLibDDraw::spLibTextDraw( DWORD dwX, DWORD dwY, LPCTSTR szError,... )
{
	BOOL bRet = TRUE;
	
    va_list                     vl;

    va_start(vl, szError);

	StringCchVPrintf(m_szTempString, (256), szError, vl);
	
	///MessageBox( NULL, m_szTempString, TEXT("spLibDDraw Msg:"), MB_OK|MB_SETFOREGROUND );
	bRet = TextDraw( dwX, dwY, m_pDDSPrimary, m_szTempString );
	
    va_end(vl);
	
	return bRet;
}


void spLibDDraw::spMessageBoxOut( LPCTSTR szError,... )
{
    ///TCHAR                       szBuff[128];
    va_list                     vl;

    va_start(vl, szError);

    ///StringCchVPrintf(szBuff, (128), szError, vl);
	StringCchVPrintf(m_szTempString, (256), szError, vl);
	
	MessageBox( NULL, m_szTempString, TEXT("spLibDDraw Msg:"), MB_OK|MB_SETFOREGROUND );
	
    va_end(vl);
}

void spLibDDraw::spMessageBoxOut( DWORD dwFlag, LPCTSTR szError,... )
{
    ///TCHAR                       szBuff[128];
    va_list                     vl;

	if( dwFlag&DBGMSG_LEVEL )
	{
		va_start(vl, szError);

		///StringCchVPrintf(szBuff, (128), szError, vl);
		StringCchVPrintf(m_szTempString, (256), szError, vl);
	
		MessageBox( NULL, m_szTempString, TEXT("spLibDDraw Msg:"), MB_OK|MB_SETFOREGROUND );
	
		va_end(vl);
	}	
}



BOOL spLibDDraw::Init( void )
{
	BOOL bRet = FALSE;
	
	///bRet = PriInit();
	
	InitializeCriticalSection( &m_csDDaccess );
	
	m_pDD = NULL;
	m_pDDSPrimary = NULL;
	m_hWnd = NULL;
	m_bSurfaceEnumFunctionCalled = FALSE;
	m_iBpp = 0;

	///m_dwLogStatus |= SPLOG_STATUS_FLAG_INITED;
	return bRet;
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



DWORD spLibDDraw::InitDDraw( void )
{
	DWORD dwRet = (-1);
	HRESULT hr;
	
	spMessageBoxOut( dINIT, TEXT("InitDDraw+++") );
	
	// Create the main DirectDraw object
	hr = DirectDrawCreate( NULL, &m_pDD, NULL );
	
	if( DD_OK != hr )
		spMessageBoxOut( dFAIL, TEXT("DirectDrawCreate fail") );
	else
	{
		DWORD dwFlagset = 0;
	
		if( NULL == m_hWnd )
		{
			// Get normal mode.
			dwFlagset = DDSCL_NORMAL;
		}
		else
		{
			// Get Fullscreen mode.
			///dwFlagset = DDSCL_FULLSCREEN|DDSCL_EXCLUSIVE;
			///dwFlagset = DDSCL_FULLSCREEN;	///CE not support DDSCL_EXCLUSIVE flag
			dwFlagset = DDSCL_NORMAL;
		}
		
		hr = m_pDD->SetCooperativeLevel( m_hWnd, dwFlagset );
		
		if( DD_OK != hr )
		{
			spMessageBoxOut( dFAIL, TEXT("SetCooperativeLevel fail") );	
		}
		else
			dwRet = 0;
	}

	spMessageBoxOut( dINIT, TEXT("InitDDraw---") );
	
	return dwRet;
}

BOOL spLibDDraw::GetDDCaps( void )
{
	BOOL bRet = FALSE;
	
	spMessageBoxOut( dINIT, TEXT("GetDDCaps+++") );
	
	if( NULL != m_pDD )
	{
		HRESULT	hRet;
		
		memset( &m_ddcaps, 0, sizeof(m_ddcaps) );
		m_ddcaps.dwSize = sizeof( m_ddcaps );
		hRet = m_pDD->GetCaps( &m_ddcaps, NULL );
		
		if (hRet != DD_OK)
			spMessageBoxOut( dFAIL, TEXT("GetDDCaps:GetCaps fail !!!") );
		else
			bRet = TRUE;
	}
	else
		spMessageBoxOut( dFAIL, TEXT("GetDDCaps: m_pDD fail !!!") );
	
	spMessageBoxOut( dINIT, TEXT("GetDDCaps---") );
	
	return bRet;
}


DWORD spLibDDraw::GetBitDepth( IDirectDrawSurface *pThisSurf )
{
	DWORD dwRet = 0;
	HRESULT hr;
	
	spMessageBoxOut( dINIT, TEXT("GetBitDepth+++") );
	
    if( NULL != pThisSurf )
    {
        ///hr = pThisSurf->Lock( NULL, &m_ddsd, DDLOCK_WAIT, NULL );
		hr = pThisSurf->Lock( NULL, &m_ddsd, DDLOCK_WAITNOTBUSY, NULL );	///CE not support DDLOCK_WAIT flag
		
		if( hr != DD_OK )
			spMessageBoxOut( dFAIL, TEXT("GetBitDepth:Lock fail !!") );
		else
		{
            // Store bit depth of surface
            dwRet = m_ddsd.ddpfPixelFormat.dwRGBBitCount;

			// Unlock surface
            hr = pThisSurf->Unlock( NULL );
			if( hr != DD_OK )
				spMessageBoxOut( dFAIL, TEXT("GetBitDepth:Unlock fail !!") );
		}
	}
	
	spMessageBoxOut( dINIT, TEXT("GetBitDepth---") );
	
	return dwRet;
}


BOOL spLibDDraw::IsOverlaySupport( void )
{
	BOOL bRet = FALSE;
	
	spMessageBoxOut( dINIT, TEXT("IsOverlaySupport+++") );
	
	if( GetDDCaps() )
	{
		if (m_ddcaps.dwOverlayCaps == 0) 
		{
			spMessageBoxOut( dWARN, TEXT("IsOverlaySupport:Overlays are not supported in hardware!") );
			bRet = FALSE;
		}
		else
		{
			spMessageBoxOut( dINFO, TEXT("IsOverlaySupport:Overlays are supported in hardware!") );
			bRet = TRUE;
		}
	}
	else
		spMessageBoxOut( dFAIL, TEXT("IsOverlaySupport: GetDDCaps fail !!!") );
	
	
	spMessageBoxOut( dINIT, TEXT("IsOverlaySupport---") );
	
	return bRet;
}


DWORD spLibDDraw::InitDDSurface( void )
{
	DWORD dwRet = 0;
	HRESULT hr;
	
	spMessageBoxOut( dINIT, TEXT("InitDDSurface+++") );
	

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
			spMessageBoxOut( dFAIL, TEXT("CreateSurface fail") );	
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
			spMessageBoxOut( dFAIL, TEXT("CreateSurface fail") );
			if (hr == DDERR_NOFLIPHW)
				spMessageBoxOut( dWARN, TEXT("******** Display driver doesn't support flipping surfaces. ********"));
		}
	}
#endif
	spMessageBoxOut( dINIT, TEXT("InitDDSurface---") );
	
	return dwRet;
}

DWORD spLibDDraw::InitDDPrimarySurface( void )
{
	DWORD dwRet = (-1);
	HRESULT hr;
	
	spMessageBoxOut( dINIT, TEXT("InitDDPrimarySurface+++") );
	
	if( NULL != m_pDD )
	{
		// Get a primary surface interface pointer (only needed for init.)
		memset(&m_ddsd, 0, sizeof(m_ddsd));
		
		m_ddsd.dwSize = sizeof(m_ddsd);
		m_ddsd.dwFlags = DDSD_CAPS;			///indicate which member is valid
		m_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		hr = m_pDD->CreateSurface( &m_ddsd, &m_pDDSPrimary, NULL );
		if( DD_OK != hr )
			spMessageBoxOut( dFAIL, TEXT("InitDDPrimarySurface:CreateSurface fail") );	
		else
		{
			dwRet = 0;
			m_iBpp = GetBitDepth( m_pDDSPrimary );
		}
	}
	else
		spMessageBoxOut( dFAIL, TEXT("InitDDPrimarySurface: m_pDD fail") );	
	
	spMessageBoxOut( dINIT, TEXT("InitDDPrimarySurface---") );
	
	return dwRet;
}

DWORD spLibDDraw::InitDDPrimarySurfaceBack( DWORD dwBackBufNum )
{
	DWORD dwRet = (-1);
	HRESULT hr;
	
	spMessageBoxOut( dINIT, TEXT("InitDDPrimarySurfaceBack+++") );
	
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
			spMessageBoxOut( dFAIL, TEXT("InitDDPrimarySurfaceBack: Not support flipping surfaces!!") );
		else
			dwRet = 0;
	}
	else
		spMessageBoxOut( dFAIL, TEXT("InitDDPrimarySurfaceBack: m_pDD fail") );
		
	if( 0 == dwRet )
	{
		// Get a pointer to the back buffer
		hr = m_pDDSPrimary->EnumAttachedSurfaces( &m_pDDSBack, NULL );	///big problem here????????
		if( hr != DD_OK )
		{
			dwRet = (-1);
			spMessageBoxOut( dFAIL, TEXT("InitDDPrimarySurfaceBack: EnumAttachedSurfaces fail!!") );	
		}
		
	}
	
	spMessageBoxOut( dINIT, TEXT("InitDDPrimarySurfaceBack---") );
	
	return dwRet;
}

DWORD spLibDDraw::InitDDPrimarySurfaceBack( DWORD dwBackBufNum, PVOID pfn )
{
	DWORD dwRet = (-1);
	HRESULT hr;
	
	spMessageBoxOut( dINIT, TEXT("InitDDPrimarySurfaceBack+++") );
	
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
			spMessageBoxOut( dWARN, TEXT("InitDDPrimarySurfaceBack: Not support flipping surfaces!!") );
		else
			dwRet = 0;
	}
	else
		spMessageBoxOut( dFAIL, TEXT("InitDDPrimarySurfaceBack: m_pDD fail") );
		
	if( 0 == dwRet )
	{
		// Get a pointer to the back buffer
		hr = m_pDDSPrimary->EnumAttachedSurfaces( &m_pDDSBack, (LPDDENUMSURFACESCALLBACK)pfn );
		if( hr != DD_OK )
		{
			dwRet = (-1);
			spMessageBoxOut( dFAIL, TEXT("InitDDPrimarySurfaceBack: EnumAttachedSurfaces fail!!") );	
		}
		
	}
	
	spMessageBoxOut( dINIT, TEXT("InitDDPrimarySurfaceBack---") );
	
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

	spMessageBoxOut( dINIT, TEXT("SurfaceEnumFunction+++") );
	
    if (!m_bSurfaceEnumFunctionCalled) {

        *((LPDIRECTDRAWSURFACE *)lpContext) = pSurface;
        m_bSurfaceEnumFunctionCalled = TRUE;
        return DDENUMRET_OK;
    }
    else
	{
        spMessageBoxOut( dWARN, TEXT("SurfaceEnumFunction: Enumerated more than surface?") );
        pSurface->Release();
        return DDENUMRET_CANCEL;
    }
	
	spMessageBoxOut( dINIT, TEXT("SurfaceEnumFunction---") );
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


DWORD spLibDDraw::PrimaryBlt( RECT *prt, DWORD dwRGB )
{
	DWORD dwRet = 0;
    HRESULT hr;
	DDBLTFX ddbfx;
	RECT rcDest;

    // Safety net
    if ( NULL != m_pDDSPrimary )
    {
		// Initialize the DDBLTFX structure with the pixel color
        ddbfx.dwSize = sizeof( ddbfx );
        ddbfx.dwFillColor = dwRGB;

        // Prepare the destination rectangle as a 1x1 (1 pixel) rectangle
        ///SetRect( &rcDest, x, y, x+1, y+1 );
		
		if( NULL == prt )
		{
			SetRect( &rcDest, 0, 0, 0+1, 0+1 );
			prt = &rcDest;
		}
		
        // Blit 1x1 rectangle using solid color op
        ///hr = m_pDDSPrimary->Blt( prt, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbfx );
		hr = m_pDDSPrimary->Blt( prt, NULL, NULL, DDBLT_COLORFILL|DDBLT_WAITNOTBUSY, &ddbfx );	///CE not support DDBLT_WAIT flag
		
		if( hr != DD_OK )
			spMessageBoxOut( dFAIL, TEXT("PrimaryBlt: Blt fail !!") );
	}
	else
		spMessageBoxOut( dINIT, TEXT("PrimaryBlt: m_pDDSPrimary fail !!") );

	
	return dwRet;
}

BOOL spLibDDraw::PixelDraw( DWORD dwX, DWORD dwY )
{
	return PixelDraw( dwX, dwY, 0xFF, 0x0, 0x0 );
}

BOOL spLibDDraw::PixelDraw( DWORD dwX, DWORD dwY, DWORD dwR, DWORD dwG, DWORD dwB )
{
	BOOL bRet = TRUE;
	
	HRESULT hr;
	RECT *LockRect = NULL;

	if( NULL != m_pDDSPrimary )
	{
		hr = m_pDDSPrimary->Lock( LockRect, &m_ddsd, DDLOCK_WAITNOTBUSY, NULL );

		BYTE * pPixelOffset = (BYTE*)m_ddsd.lpSurface + dwX * m_ddsd.lXPitch + dwY * m_ddsd.lPitch;
		*(WORD*)pPixelOffset = (WORD)CreateRGB( dwR, dwG, dwB );
		hr = m_pDDSPrimary->Unlock( LockRect );
	}
	else
		;///spMessageBoxOut( dFAIL, TEXT("PixelDraw: m_pDDSPrimary fail !!") );

	return bRet;
}

BOOL spLibDDraw::TextDraw( IDirectDrawSurface *pThisSurf, TCHAR *szString )
{
	BOOL bRet = FALSE;

    HDC hdc;
    RECT rc;
    SIZE size;
    int nMsg;
///    DDBLTFX ddbltfx;

	if( NULL != pThisSurf )
	{
		if( pThisSurf->GetDC( &hdc ) == DD_OK )
		{
			SetBkColor(hdc, RGB(0, 0, 255));
			SetTextColor(hdc, RGB(255, 255, 0));
			GetClientRect( m_hWnd, &rc );

	        nMsg = lstrlen( szString );
            GetTextExtentPoint(hdc, szString, nMsg, &size);
            ExtTextOut(hdc, 
		       50, 
		       50,
		       0,                        // fuOptions
		       NULL,                     // lprc
               szString, 
		       nMsg,
		       NULL);                    // lpDx
			
			pThisSurf->ReleaseDC(hdc);
			bRet = TRUE;
        }
        else
			spMessageBoxOut( dFAIL, TEXT("TextDraw: GetDC fail !!") );
    }
	else
		spMessageBoxOut( dFAIL, TEXT("TextDraw: pThisSurf fail !!") );
	
	
	return bRet;
}


BOOL spLibDDraw::TextDraw( DWORD dwX, DWORD dwY, IDirectDrawSurface *pThisSurf, TCHAR *szString )
{
	BOOL bRet = FALSE;

    HDC hdc;
    RECT rc;
    SIZE size;
    int nMsg;
///    DDBLTFX ddbltfx;

	if( NULL != pThisSurf )
	{
		if( pThisSurf->GetDC( &hdc ) == DD_OK )
		{
			SetBkColor(hdc, RGB(0, 0, 255));
			SetTextColor(hdc, RGB(255, 255, 0));
			GetClientRect( m_hWnd, &rc );

	        nMsg = lstrlen( szString );
            GetTextExtentPoint(hdc, szString, nMsg, &size);
            ExtTextOut(hdc, 
		       dwX, 
		       dwY,
		       0,                        // fuOptions
		       NULL,                     // lprc
               szString, 
		       nMsg,
		       NULL);                    // lpDx
			
			pThisSurf->ReleaseDC(hdc);
			bRet = TRUE;
        }
        else
			spMessageBoxOut( dFAIL, TEXT("TextDraw: GetDC fail !!") );
    }
	else
		spMessageBoxOut( dFAIL, TEXT("TextDraw: pThisSurf fail !!") );
	
	
	return bRet;
}


DWORD spLibDDraw::CreateRGB( DWORD dwR, DWORD dwG, DWORD dwB )
{
	DWORD dwRet = 0x0000f800;
	
    switch( m_iBpp )
    {
        case 8:
                // Here you should do a palette lookup to find the closes match.
                // I'm not going to bother with that. Many modern games no
                // longer support 256-color modes, and neither should you :)
                dwRet = 0;
			break;
        case 16:
                // Break down r,g,b into 5-6-5 format.
                dwRet = ((dwR/8)<<11) | ((dwG/4)<<5) | (dwB/8);
			break;
        case 24:
        case 32:
                dwRet = (dwR<<16) | (dwG<<8) | (dwB);
			break;
		default:
			break;
    }
	
	return dwRet;
}
// TvBrowserFun.cpp: implementation of the TvBrowserFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
///#include <streams.h>	///for DirectShow
///#include <Objbase.h>	///for DirectShow, CoCreateInstance()
///#include <Unknwn.h>
#include <ddraw.h>
#include "resource.h"



///DEFINE_GUID( IID_IDirectDraw4,                  0x9c59509a,0x39bd,0x11d1,0x8c,0x4a,0x00,0xc0,0x4f,0xd9,0x30,0xc5 );



//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                TEXT("MosquitoWndClass")
#define TITLE               TEXT("Mosquito")

#define DDTEXT              TEXT("Overlay-Text")

///#define BUG_WIDTH           320
///#define BUG_HEIGHT          200
#define BUG_WIDTH           64
#define BUG_HEIGHT          40


#ifdef UNDER_CE
#define RAND_INT(x) (Random() % x)
#else
#define RAND_INT(x) (rand()*x/RAND_MAX)
#endif
#define RANDOM_VELOCITY() (int)(((RAND_INT(5)+3)*2))

//-----------------------------------------------------------------------------
// Default settings
//-----------------------------------------------------------------------------
///#define TIMER_ID            1
#define TIMER_ID            333
#define TIMER_RATE          200
///#define TIMER_RATE          500

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
LPDIRECTDRAW4               g_pDD = NULL;        // DirectDraw object
LPDIRECTDRAWSURFACE4        g_pDDSPrimary = NULL; // Primary Surface.
LPDIRECTDRAWSURFACE4        g_pDDSOverlay = NULL; // The overlay primary.
BOOL                        g_bActive = FALSE;   // Is application active?

// Overlay position and velocity data.

int g_nOverlayXPos, g_nOverlayYPos;
int g_nOverlayXVel, g_nOverlayYVel;
int g_nOverlayWidth, g_nOverlayHeight;
DWORD g_dwOverlayXPositionAlignment;

// Our instance handle.

HINSTANCE g_hInstance;

///for DDOverlayBitmap()
HINSTANCE gsp_hInst;
HWND gsp_hWnd;
LPDIRECTDRAW4               gsp_pDD = NULL;        // DirectDraw object
LPDIRECTDRAWSURFACE4        gsp_pDDSPrimary = NULL; // Primary Surface.
LPDIRECTDRAWSURFACE4        gsp_pDDSOverlay = NULL; // The overlay primary.
DWORD gsp_dwBitmapWidth;
DWORD gsp_dwBitmapHeight;
RECT gsp_rs;
RECT gsp_rd;
DWORD gsp_dwSrcColorkey;
int gsp_nOverlayXPos, gsp_nOverlayYPos;
int gsp_nOverlayXVel, gsp_nOverlayYVel;
int gsp_nOverlayWidth, gsp_nOverlayHeight;
DWORD gsp_dwOverlayXPositionAlignment;
DWORD gsp_dwX;
DWORD gsp_dwY;
DWORD gsp_dwOSDWidth;
DWORD gsp_dwOSDHeight;

DWORD gsp_dwBitMapID1;
DWORD gsp_dwBM1Width;
DWORD gsp_dwBM1Height;
DWORD gsp_dwBM1X;
DWORD gsp_dwBM1Y;

DWORD gsp_dwBitMapID2;
DWORD gsp_dwBM2Width;
DWORD gsp_dwBM2Height;
DWORD gsp_dwBM2X;
DWORD gsp_dwBM2Y;




//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
static TCHAR                szImg1[] = TEXT("IDB_BUGIMAGE1");
static TCHAR                szImg2[] = TEXT("IDB_BUGIMAGE2");
static TCHAR                szImg3[] = TEXT("IDB_BUGIMAGE3");

// These are the pixel formats this app supports.  Most display adapters
// with overlay support will recognize one or more of these formats.
// We start with YUV format, then work down to RGB. (All 16 bpp.)

#if 1	///erine code 
static DDPIXELFORMAT ddpfOverlayFormats[] = {
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07e0, 0x001F, 0},         // 16-bit RGB 5:6:5
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0x7C00, 0x03e0, 0x001F, 0},        // 16-bit RGB 5:5:5
    {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','U','Y','V'),0,0,0,0,0},  // YUYV
    {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('U','Y','V','Y'),0,0,0,0,0},  // UYVY  
};
#else	///orginal code 
static DDPIXELFORMAT ddpfOverlayFormats[] = {
    {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','U','Y','V'),0,0,0,0,0},  // YUYV
    {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('U','Y','V','Y'),0,0,0,0,0},  // UYVY
    {sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0x7C00, 0x03e0, 0x001F, 0},        // 16-bit RGB 5:5:5
    {sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07e0, 0x001F, 0}         // 16-bit RGB 5:6:5
};
#endif

#define PF_TABLE_SIZE 4

static RECT rs;
static RECT rd;

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
void ReleaseAllObjects(void);
HRESULT InitFail(HWND, HRESULT, LPCTSTR, ...);
HRESULT RestoreAllSurfaces();
void DDMoveOverlay();
///long FAR PASCAL WindowProc(HWND, UINT, WPARAM, LPARAM);
BOOL CopyBitmapToYUVSurface(LPDIRECTDRAWSURFACE4, HBITMAP);
BOOL LoadImageOntoSurface(LPDIRECTDRAWSURFACE4, LPCTSTR);
HRESULT WINAPI EnumSurfacesCallback(LPDIRECTDRAWSURFACE4, LPDDSURFACEDESC2, LPVOID);
HRESULT LoadBugImages();
HRESULT DDInitApp(HINSTANCE hInstance, int nCmdShow);




//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: Finished with all objects we use; release them
//-----------------------------------------------------------------------------
static void
ReleaseAllObjects(void)
{
    if (g_pDDSOverlay != NULL)
    {
        // Use UpdateOverlay() with the DDOVER_HIDE flag to remove an overlay 
	    // from the display.
	    g_pDDSOverlay->UpdateOverlay(NULL, g_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
        g_pDDSOverlay->Release();
        g_pDDSOverlay = NULL;
    }

    if (g_pDDSPrimary != NULL)
    {
        g_pDDSPrimary->Release();
        g_pDDSPrimary = NULL;
    }

    if (g_pDD != NULL)
    {
        g_pDD->Release();
        g_pDD = NULL;
    }
}


//-----------------------------------------------------------------------------
// Name: RestoreAllSurfaces
// Desc: Called in case we lose our surface's vram.
//-----------------------------------------------------------------------------
static HRESULT 
RestoreAllSurfaces()
{
    HRESULT hRet;

    // Try Restoring the primary surface.

    hRet = g_pDDSPrimary->Restore();
    if (hRet != DD_OK)
	    return hRet;

    // Try Restoring the overlay surface.

    hRet = g_pDDSOverlay->Restore();
    if (hRet != DD_OK)
	    return hRet;

    // Reload the images.
    hRet = LoadBugImages();
    if (hRet != DD_OK)
        return hRet;

    // Show the overlay.

    hRet = g_pDDSOverlay->UpdateOverlay(&rs, g_pDDSPrimary, &rd, DDOVER_SHOW, NULL);
    return hRet;
}




//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
#define PREFIX      TEXT("MOSQUITO: ")
#define PREFIX_LEN  10

static HRESULT
InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...)
{
    TCHAR                       szBuff[128];
    va_list                     vl;

    va_start(vl, szError);
    wsprintf(szBuff, PREFIX);
    wvsprintf(szBuff + PREFIX_LEN, szError, vl);
    wsprintf(szBuff + wcslen(szBuff), TEXT("\r\n"));
    ReleaseAllObjects();
///    OutputDebugString(szBuff);
	NKDbgPrintfW( szBuff );
///    DestroyWindow(hWnd);
    va_end(vl);
    return hRet;
}

#undef PREFIX_LEN
#undef PREFIX




//-----------------------------------------------------------------------------
// Name: MoveOverlay()
// Desc: Called on the timer, this function moves the overlay around the
//       screen, periodically calling flip to animate the mosquito.
//-----------------------------------------------------------------------------
void 
DDMoveOverlay()
{
    HRESULT         hRet;
    DWORD           dwXAligned;

    // Add the current velocity vectors to the position.

    g_nOverlayXPos += g_nOverlayXVel;
    g_nOverlayYPos += g_nOverlayYVel;

    // Check to see if this new position puts the overlay off the edge of the screen.
    // SetOverlayPosition() won't like that.

    // Have we gone off the left edge?

    if (g_nOverlayXPos < 0) {
	    g_nOverlayXPos = 0;
	    g_nOverlayXVel = RANDOM_VELOCITY();
    }

    // Have we gone off the right edge?

    if ((g_nOverlayXPos+g_nOverlayWidth) >  GetSystemMetrics(SM_CXSCREEN)){
	    g_nOverlayXPos = GetSystemMetrics(SM_CXSCREEN) - g_nOverlayWidth;
	    g_nOverlayXVel = -RANDOM_VELOCITY();
    }

    // Have we gone off the top edge?

    if (g_nOverlayYPos < 0) {
	    g_nOverlayYPos = 0;
	    g_nOverlayYVel = RANDOM_VELOCITY();
    }

    // Have we gone off the bottom edge?

    if ( (g_nOverlayYPos+g_nOverlayHeight) >  GetSystemMetrics(SM_CYSCREEN)) {
	    g_nOverlayYPos = GetSystemMetrics(SM_CYSCREEN) - g_nOverlayHeight;
	    g_nOverlayYVel = -RANDOM_VELOCITY();
    }

    // We need to check for any alignment restrictions on the X position.

    if (g_dwOverlayXPositionAlignment)
	    dwXAligned = g_nOverlayXPos - g_nOverlayXPos % g_dwOverlayXPositionAlignment;
    else
	    dwXAligned = g_nOverlayXPos;

    // Set the overlay to it's new position.

    hRet = g_pDDSOverlay->SetOverlayPosition(dwXAligned, g_nOverlayYPos);
    if (hRet == DDERR_SURFACELOST)
    {
	    if (FAILED(RestoreAllSurfaces())) 
	        return;
    }

    // Flip.

    while (TRUE)
    {
        hRet = g_pDDSOverlay->Flip(NULL, 0);
        if (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
            hRet = RestoreAllSurfaces();
            if (hRet != DD_OK)
                break;
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            break;
    }
}


//-----------------------------------------------------------------------------
//  Function: CopyBitmapToYUVSurface
//  Description: 
//      Copies an RGB GDI bitmap to a YUV surface. Both bitmap and surface
//      must be a multiple of 2 pixels in width for the supported YUV formats.  
//      The following formats are supported:
//              YUYV
//              UYVY
//      
//      The "YUYV" YUV pixel format looks like this:
//          As a series of BYTES:    [Y0][U][Y1][V] (reverse it for a DWORD)
// 
//      The "UYVY" YUV pixel format looks like this:
//          As a series of BYTES:    [U][Y0][V][Y1] (reverse it for a DWORD)
// 
//      As you can see, both formats pack two pixels into a single DWORD. The 
//      pixels share U and V components and have separate Y components.
//      
//  Returns: TRUE if successful, otherwise FALSE.
//-----------------------------------------------------------------------------
static BOOL 
CopyBitmapToYUVSurface(LPDIRECTDRAWSURFACE4 lpDDSurf, HBITMAP hbm)
{
    HDC                 hdcImage;
    HRESULT             ddrval;
    DDSURFACEDESC2      ddsd;
    DWORD               x, y, dwWidth, dwHeight;
    LONG                lPitch;
    LPBYTE              pSurf;
    DWORD               dwBytesInRow;
    COLORREF            color;
    BYTE                R,G,B, Y0,Y1,U,V;
    BOOL                bRet = FALSE;

    if (hbm == NULL || lpDDSurf == NULL)
	return FALSE;

	NKDbgPrintfW( L"TVB::CopyBitmapToYUVSurface +++\r\n" );

    //
    //  select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    SelectObject(hdcImage, hbm);

    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    // Lock down the surface so we can modify it's contents.
    ddrval=lpDDSurf->Lock( NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);
    if (FAILED(ddrval))
	    goto CleanUp;

    dwWidth=ddsd.dwWidth;
    dwHeight=ddsd.dwHeight;
    lPitch=ddsd.lPitch;
    pSurf=(LPBYTE)ddsd.lpSurface;
	dwBytesInRow=ddsd.dwWidth*2;

    // Go through the image 2 pixels at a time and convert to YUV
    for(y=0; y<dwHeight; y++)
    {
	for(x=0; x<dwWidth; x+=2)
		{
	    // The equations for color conversion used here, probably aren't 
	    // exact, but they seem to do an OK job.
			color=GetPixel(hdcImage, x,y);
			R=GetRValue(color);
			G=GetGValue(color);
			B=GetBValue(color);
		    Y0= (BYTE)(0.29*R + 0.59*G + 0.14*B);
			U= (BYTE)(128.0 - 0.14*R - 0.29*G + 0.43*B);

			color=GetPixel(hdcImage, x+1,y);
			R=GetRValue(color);
			G=GetGValue(color);
			B=GetBValue(color);
			Y1= (BYTE)(0.29*R + 0.57*G + 0.14*B);
			V= (BYTE)(128.0 + 0.36*R - 0.29*G - 0.07*B);

			switch (ddsd.ddpfPixelFormat.dwFourCC)
			{
				case MAKEFOURCC('Y','U','Y','V'): 
					*(pSurf++) = Y0;
					*(pSurf++) = U;
					*(pSurf++) = Y1;
					*(pSurf++) = V;
					break;
				case MAKEFOURCC('U','Y','V','Y'): 
					*(pSurf++) = U;
					*(pSurf++) = Y0;
					*(pSurf++) = V;
					*(pSurf++) = Y1;
					break;
			}                       
		}
		pSurf+=(lPitch-dwBytesInRow);
    }

    lpDDSurf->Unlock(NULL);     

CleanUp:
    if(hdcImage)
	DeleteDC(hdcImage);

	return TRUE;
}


static BOOL 
CopyTextDCToYUVSurface(LPDIRECTDRAWSURFACE4 lpDDSurf, HBITMAP hbm, LPCTSTR lpstrText )
{
    HDC                 hdcImage;
    HRESULT             ddrval;
    DDSURFACEDESC2      ddsd;
    DWORD               x, y, dwWidth, dwHeight;
    LONG                lPitch;
    LPBYTE              pSurf;
    DWORD               dwBytesInRow;
    COLORREF            color;
    BYTE                R,G,B, Y0,Y1,U,V;
    BOOL                bRet = FALSE;
   	RECT 				rtWindow;


    if (hbm == NULL || lpDDSurf == NULL)
		return FALSE;

	NKDbgPrintfW( L"TVB::CopyTextDCToYUVSurface +++\r\n" );

    //
    //  select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    SelectObject(hdcImage, hbm);

	GetClientRect(gsp_hWnd, &rtWindow);
	rtWindow.left = gsp_dwX;
	rtWindow.top = gsp_dwY;
	rtWindow.right = gsp_dwX+gsp_dwBitmapWidth;
	rtWindow.bottom = gsp_dwY+gsp_dwBitmapHeight;
	
	
	///SetBkMode( hdcImage, TRANSPARENT );
	SetTextColor( hdcImage, RGB(255,0,0) );
	SetBkColor( hdcImage, RGB(0,0,255) );
	FillRect(hdcImage, &rtWindow,  CreateSolidBrush(GetSysColor (COLOR_BTNFACE)));
	DrawText( hdcImage, DDTEXT, _tcslen(DDTEXT), &rtWindow, DT_SINGLELINE|DT_TOP );
	///DrawText( hdcImage, lpstrText, _tcslen(lpstrText), &rtWindow, DT_SINGLELINE|DT_TOP );



    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    // Lock down the surface so we can modify it's contents.
    ddrval=lpDDSurf->Lock( NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);
    if (FAILED(ddrval))
	    goto CleanUp;

    dwWidth=ddsd.dwWidth;
    dwHeight=ddsd.dwHeight;
    lPitch=ddsd.lPitch;
    pSurf=(LPBYTE)ddsd.lpSurface;
	dwBytesInRow=ddsd.dwWidth*2;

    // Go through the image 2 pixels at a time and convert to YUV
    for(y=0; y<dwHeight; y++)
    {
	for(x=0; x<dwWidth; x+=2)
		{
	    // The equations for color conversion used here, probably aren't 
	    // exact, but they seem to do an OK job.
			color=GetPixel(hdcImage, x,y);
			R=GetRValue(color);
			G=GetGValue(color);
			B=GetBValue(color);
		    Y0= (BYTE)(0.29*R + 0.59*G + 0.14*B);
			U= (BYTE)(128.0 - 0.14*R - 0.29*G + 0.43*B);

			color=GetPixel(hdcImage, x+1,y);
			R=GetRValue(color);
			G=GetGValue(color);
			B=GetBValue(color);
			Y1= (BYTE)(0.29*R + 0.57*G + 0.14*B);
			V= (BYTE)(128.0 + 0.36*R - 0.29*G - 0.07*B);

			switch (ddsd.ddpfPixelFormat.dwFourCC)
			{
				case MAKEFOURCC('Y','U','Y','V'): 
					*(pSurf++) = Y0;
					*(pSurf++) = U;
					*(pSurf++) = Y1;
					*(pSurf++) = V;
					break;
				case MAKEFOURCC('U','Y','V','Y'): 
					*(pSurf++) = U;
					*(pSurf++) = Y0;
					*(pSurf++) = V;
					*(pSurf++) = Y1;
					break;
			}                       
		}
		pSurf+=(lPitch-dwBytesInRow);
    }

    lpDDSurf->Unlock(NULL);     

CleanUp:
    if(hdcImage)
	DeleteDC(hdcImage);

	return TRUE;
}




//-----------------------------------------------------------------------------
//  Function: LoadImageOnToSurface
//  Description:
//      Loads a resource based bitmap image onto a DirectDraw surface.  Can
//      covert the bitmap to all RGB formats, plus a couple YUV formats.
//-----------------------------------------------------------------------------
static BOOL 
LoadImageOntoSurface(LPDIRECTDRAWSURFACE4 lpdds, LPCTSTR lpstrResID)
{
    HBITMAP hbm = NULL;
    HDC     hdcImage = NULL;
    HDC     hdcSurf = NULL;
    BOOL bRetVal = FALSE;
    HRESULT ddrval;
    DDSURFACEDESC2 ddsd;

    if (!lpdds) return FALSE;

    //
    // get surface size and format.
    //
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    ddrval = lpdds->GetSurfaceDesc(&ddsd);
    if (FAILED(ddrval)) {
    	NKDbgPrintfW( L"TVB::GetSurfaceDesc fail\r\n" );
	    goto Exit;
	}

	// Load the bitmap resource.  We'll use LoadImage() since it'll scale the 
    // image to fit our surface, and maintain the color information in the
    // bitmap.
    hbm = (HBITMAP)LoadImage(g_hInstance, lpstrResID, IMAGE_BITMAP, 0, 0, 0);
	if (hbm == NULL) {
		NKDbgPrintfW( L"TVB::LoadImage fail g_hInstance=0x%08x\r\n", g_hInstance );
	    goto Exit;
	}


    // If our surface is a FOURCC YUV format, we need to do a little work to convert
    // our RGB resource bitmap into the appropriate YUV format.
    if (ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC)
    ///if (0)
    {
		if (!CopyBitmapToYUVSurface(lpdds, hbm)) {
			NKDbgPrintfW( L"TVB::CopyBitmapToYUVSurface fail\r\n" );
	    	goto Exit;        
	    }
    }
    else  //Looks like we're just using a standard RGB surface format, let GDI do the work.
    {
	    // Create a DC and associate the bitmap with it.
	    hdcImage = CreateCompatibleDC(NULL);
	    SelectObject(hdcImage, hbm);
   
	    ddrval = lpdds->GetDC(&hdcSurf);
	    if (FAILED(ddrval)) {
	    	NKDbgPrintfW( L"TVB::GetDC fail\r\n" );
	        goto Exit;
	    }
    
	    if (BitBlt(hdcSurf, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, 0, 0, SRCCOPY) == FALSE) {
	    	NKDbgPrintfW( L"TVB::BitBlt fail\r\n" );
	        goto Exit;
	    }
    }

    bRetVal = TRUE;
    
Exit:
    if (hdcSurf)
	    lpdds->ReleaseDC(hdcSurf);
    if (hdcImage)
	    DeleteDC(hdcImage);
    if (hbm)
	    DeleteObject(hbm);

    return bRetVal;
}


static BOOL 
spLoadImageOntoSurface(LPDIRECTDRAWSURFACE4 lpdds, LPCTSTR lpstrResID)
{
    HBITMAP hbm = NULL;
    HDC     hdcImage = NULL;
    HDC     hdcSurf = NULL;
    BOOL bRetVal = FALSE;
    HRESULT ddrval;
    DDSURFACEDESC2 ddsd;

    if (!lpdds) return FALSE;

    //
    // get surface size and format.
    //
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    ddrval = lpdds->GetSurfaceDesc(&ddsd);
    if (FAILED(ddrval)) {
    	NKDbgPrintfW( L"TVB::GetSurfaceDesc fail\r\n" );
	    goto Exit;
	}

	// Load the bitmap resource.  We'll use LoadImage() since it'll scale the 
    // image to fit our surface, and maintain the color information in the
    // bitmap.
    hbm = (HBITMAP)LoadImage(gsp_hInst, lpstrResID, IMAGE_BITMAP, 0, 0, 0);
	if (hbm == NULL) {
		NKDbgPrintfW( L"TVB::LoadImage fail g_hInstance=0x%08x\r\n", g_hInstance );
	    goto Exit;
	}


    // If our surface is a FOURCC YUV format, we need to do a little work to convert
    // our RGB resource bitmap into the appropriate YUV format.
    if (ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC)
    ///if (0)
    {
		if (!CopyBitmapToYUVSurface(lpdds, hbm)) {
			NKDbgPrintfW( L"TVB::CopyBitmapToYUVSurface fail\r\n" );
	    	goto Exit;        
	    }
    }
    else  //Looks like we're just using a standard RGB surface format, let GDI do the work.
    {
	    // Create a DC and associate the bitmap with it.
	    hdcImage = CreateCompatibleDC(NULL);
	    SelectObject(hdcImage, hbm);
   
	    ddrval = lpdds->GetDC(&hdcSurf);
	    if (FAILED(ddrval)) {
	    	NKDbgPrintfW( L"TVB::GetDC fail\r\n" );
	        goto Exit;
	    }
    
	    if (BitBlt(hdcSurf, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, 0, 0, SRCCOPY) == FALSE) {
	    	NKDbgPrintfW( L"TVB::BitBlt fail\r\n" );
	        goto Exit;
	    }
    }

    bRetVal = TRUE;
    
Exit:
    if (hdcSurf)
	    lpdds->ReleaseDC(hdcSurf);
    if (hdcImage)
	    DeleteDC(hdcImage);
    if (hbm)
	    DeleteObject(hbm);

    return bRetVal;
}


static BOOL 
spTextDCOntoSurface(LPDIRECTDRAWSURFACE4 lpdds, LPCTSTR lpstrText)
{
    HBITMAP hbm = NULL;
///   	RECT 	rtWindow;
    HBITMAP hbm_old = NULL;
    HDC     hdcImage = NULL;
    HDC     hdcSurf = NULL;
    BOOL bRetVal = FALSE;
    HRESULT ddrval;
    DDSURFACEDESC2 ddsd;

    if (!lpdds) return FALSE;

    //
    // get surface size and format.
    //
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    ddrval = lpdds->GetSurfaceDesc(&ddsd);
    if (FAILED(ddrval)) {
    	NKDbgPrintfW( L"TVB::GetSurfaceDesc fail\r\n" );
	    goto Exit;
	}

	// Load the bitmap resource.  We'll use LoadImage() since it'll scale the 
    // image to fit our surface, and maintain the color information in the
    // bitmap.
///    hbm = (HBITMAP)LoadImage(gsp_hInst, lpstrResID, IMAGE_BITMAP, 0, 0, 0);
///	if (hbm == NULL) {
///		NKDbgPrintfW( L"TVB::LoadImage fail g_hInstance=0x%08x\r\n", g_hInstance );
///	    goto Exit;
///	}

	hdcImage = CreateCompatibleDC(NULL);
	
	hbm = CreateBitmap( ddsd.dwWidth, ddsd.dwHeight, 1, 32, NULL);
	
	hbm_old = (HBITMAP)SelectObject(hdcImage, hbm);


	///SetBkMode( hdcImage, TRANSPARENT );
	SetTextColor( hdcImage, RGB(255,0,0) );
	SetBkColor( hdcImage, RGB(0,0,255) );
#if 0
	GetClientRect(gsp_hWnd, &rtWindow);
	rtWindow.left = gsp_dwX;
	rtWindow.top = gsp_dwY;
	rtWindow.right = gsp_dwX+gsp_dwBitmapWidth;
	rtWindow.bottom = gsp_dwY+gsp_dwBitmapHeight;
	FillRect(hdcImage, &rtWindow,  CreateSolidBrush(GetSysColor (COLOR_BTNFACE)));
#endif
	///DrawText( hdcImage, DDTEXT, _tcslen(DDTEXT), &rtWindow, DT_SINGLELINE|DT_TOP );
	ExtTextOut( hdcImage, 0, 0, 0, 0, lpstrText, wcslen(lpstrText), 0 );
    if (ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC)
		hbm = (HBITMAP)SelectObject( hdcImage, hbm_old );



    // If our surface is a FOURCC YUV format, we need to do a little work to convert
    // our RGB resource bitmap into the appropriate YUV format.
    if (ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC)
    {
		///if (!CopyTextDCToYUVSurface(lpdds, hbm, lpstrText)) {
		///	NKDbgPrintfW( L"TVB::CopyTextDCToYUVSurface fail\r\n" );
	    ///	goto Exit;        
	    ///}
		if (!CopyBitmapToYUVSurface(lpdds, hbm)) {
			NKDbgPrintfW( L"TVB::CopyBitmapToYUVSurface fail\r\n" );
	    	goto Exit;        
	    }
    }
    else  //Looks like we're just using a standard RGB surface format, let GDI do the work.
    {
	    // Create a DC and associate the bitmap with it.
	    ///hdcImage = CreateCompatibleDC(NULL);
	    ///SelectObject(hdcImage, hbm);
   
	    ddrval = lpdds->GetDC(&hdcSurf);
	    if (FAILED(ddrval)) {
	    	NKDbgPrintfW( L"TVB::GetDC fail\r\n" );
	        goto Exit;
	    }
    
	    if (BitBlt(hdcSurf, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, 0, 0, SRCCOPY) == FALSE) {
	    	NKDbgPrintfW( L"TVB::BitBlt fail\r\n" );
	        goto Exit;
	    }
    }

    bRetVal = TRUE;
    
Exit:
    if (hdcSurf)
	    lpdds->ReleaseDC(hdcSurf);
    if (hdcImage)
	    DeleteDC(hdcImage);
    if (hbm)
	    DeleteObject(hbm);

    return bRetVal;
}


static BOOL 
spLoadOSDDCOntoSurface(LPDIRECTDRAWSURFACE4 lpdds, LPCTSTR lpstrText, LPCTSTR lpstrResID1, LPCTSTR lpstrResID2)
{
    HBITMAP hbm = NULL;
    HBITMAP hbm1 = NULL;
    HBITMAP hbm2 = NULL;
    HDC     hdcIcon1 = NULL;
    HDC     hdcIcon2 = NULL;
///   	RECT 	rtWindow;
    HBITMAP hbm_old = NULL;
    HDC     hdcImage = NULL;
    HDC     hdcSurf = NULL;
    BOOL bRetVal = FALSE;
    HRESULT ddrval;
    DDSURFACEDESC2 ddsd;

    if (!lpdds) return FALSE;

    //
    // get surface size and format.
    //
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    ddrval = lpdds->GetSurfaceDesc(&ddsd);
    if (FAILED(ddrval)) {
    	NKDbgPrintfW( L"TVB::GetSurfaceDesc fail\r\n" );
	    goto Exit;
	}

	// Load the bitmap resource.  We'll use LoadImage() since it'll scale the 
    // image to fit our surface, and maintain the color information in the
    // bitmap.
    hbm1 = (HBITMAP)LoadImage(gsp_hInst, lpstrResID1, IMAGE_BITMAP, 0, 0, 0);
	if (hbm1 == NULL) {
		NKDbgPrintfW( L"TVB::OSD LoadImage1 fail g_hInstance=0x%08x\r\n", gsp_hInst );
	    ///goto Exit;
	} else {
	
		hdcIcon1 = CreateCompatibleDC(NULL);
		SelectObject(hdcIcon1, hbm1);

	}

    hbm2 = (HBITMAP)LoadImage(gsp_hInst, lpstrResID2, IMAGE_BITMAP, 0, 0, 0);
	if (hbm2 == NULL) {
		NKDbgPrintfW( L"TVB::OSD LoadImage2 fail g_hInstance=0x%08x\r\n", gsp_hInst );
	    ///goto Exit;
	} else {
	
		hdcIcon2 = CreateCompatibleDC(NULL);
		SelectObject(hdcIcon2, hbm2);

	}



	hdcImage = CreateCompatibleDC(NULL);
	
	hbm = CreateBitmap( ddsd.dwWidth, ddsd.dwHeight, 1, 32, NULL);
	
	hbm_old = (HBITMAP)SelectObject(hdcImage, hbm);


	///SetBkMode( hdcImage, TRANSPARENT );
	SetTextColor( hdcImage, RGB(255,255,0) );
	SetBkColor( hdcImage, RGB(0,0,0) );
#if 0
	GetClientRect(gsp_hWnd, &rtWindow);
	rtWindow.left = gsp_dwX;
	rtWindow.top = gsp_dwY;
	rtWindow.right = gsp_dwX+gsp_dwBitmapWidth;
	rtWindow.bottom = gsp_dwY+gsp_dwBitmapHeight;
	FillRect(hdcImage, &rtWindow,  CreateSolidBrush(GetSysColor (COLOR_BTNFACE)));
#endif
	///DrawText( hdcImage, DDTEXT, _tcslen(DDTEXT), &rtWindow, DT_SINGLELINE|DT_TOP );
	ExtTextOut( hdcImage, 0, 0, 0, 0, lpstrText, wcslen(lpstrText), 0 );
	
	if( NULL != hbm1 ) {
		if (BitBlt(hdcImage, gsp_dwBM1X, gsp_dwBM1Y, ddsd.dwWidth, ddsd.dwHeight, hdcIcon1, 0, 0, SRCCOPY) == FALSE) {
	   		NKDbgPrintfW( L"TVB::BitBlt 1 fail\r\n" );
	    	///goto Exit;
		}
	}

	if( NULL != hbm2 ) {
		if (BitBlt(hdcImage, gsp_dwBM2X, gsp_dwBM2Y, ddsd.dwWidth, ddsd.dwHeight, hdcIcon2, 0, 0, SRCCOPY) == FALSE) {
	   		NKDbgPrintfW( L"TVB::BitBlt 2 fail\r\n" );
	    	///goto Exit;
		}
	}
	
	
    if (ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC)
		hbm = (HBITMAP)SelectObject( hdcImage, hbm_old );






    // If our surface is a FOURCC YUV format, we need to do a little work to convert
    // our RGB resource bitmap into the appropriate YUV format.
    if (ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC)
    {
		///if (!CopyTextDCToYUVSurface(lpdds, hbm, lpstrText)) {
		///	NKDbgPrintfW( L"TVB::CopyTextDCToYUVSurface fail\r\n" );
	    ///	goto Exit;        
	    ///}
		if (!CopyBitmapToYUVSurface(lpdds, hbm)) {
			NKDbgPrintfW( L"TVB::CopyBitmapToYUVSurface fail\r\n" );
	    	goto Exit;        
	    }
    }
    else  //Looks like we're just using a standard RGB surface format, let GDI do the work.
    {
	    // Create a DC and associate the bitmap with it.
	    ///hdcImage = CreateCompatibleDC(NULL);
	    ///SelectObject(hdcImage, hbm);
   
	    ddrval = lpdds->GetDC(&hdcSurf);
	    if (FAILED(ddrval)) {
	    	NKDbgPrintfW( L"TVB::GetDC fail\r\n" );
	        goto Exit;
	    }
    
	    if (BitBlt(hdcSurf, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, 0, 0, SRCCOPY) == FALSE) {
	    	NKDbgPrintfW( L"TVB::BitBlt fail\r\n" );
	        goto Exit;
	    }
    }

    bRetVal = TRUE;
    
Exit:
    if (hdcSurf)
	    lpdds->ReleaseDC(hdcSurf);
    if (hdcImage)
	    DeleteDC(hdcImage);
    if (hbm)
	    DeleteObject(hbm);

    if (hdcIcon1)
	    DeleteDC(hdcIcon1);
    if (hbm1)
	    DeleteObject(hbm1);

    if (hdcIcon2)
	    DeleteDC(hdcIcon2);
    if (hbm2)
	    DeleteObject(hbm2);


    return bRetVal;
}



//-----------------------------------------------------------------------------
// Name: EnumSurfacesCallback()
// Desc: Used by LoadBugImages to aid it loading all three bug images.
//-----------------------------------------------------------------------------
static HRESULT WINAPI 
EnumSurfacesCallback(LPDIRECTDRAWSURFACE4 lpDDSurface,  
                     LPDDSURFACEDESC2 lpDDSurfaceDesc,  
                     LPVOID lpContext)
{
    int * CallCount = (int *)lpContext;
    HRESULT hr = (HRESULT)DDENUMRET_OK;
    LPTSTR ResName;

    // Check to see if we've gone full circle through all surfaces and 
    // are now looking at the first one again.

    if (lpDDSurface == g_pDDSOverlay)
	    goto exit;

    // Load the Bug Image appropriate...

    if (*CallCount == 0) {
        ///ResName = szImg2;
        ResName = MAKEINTRESOURCE(IDB_BUGIMAGE2);
    }
    else if (*CallCount == 1) {
        ///ResName = szImg3;
        ResName = MAKEINTRESOURCE(IDB_BUGIMAGE3);
    }
    else {
        // Eh?
        hr = (HRESULT)DDENUMRET_CANCEL;
        goto exit;
    }

    if (!LoadImageOntoSurface(lpDDSurface, ResName)) {
        hr = (HRESULT)DDENUMRET_CANCEL;
        goto exit;
    }

    // Bump the count.

    (*CallCount)++;

    // Recurse

    lpDDSurface->EnumAttachedSurfaces(lpContext, EnumSurfacesCallback);

exit:
    lpDDSurface->Release();
    return hr;
}





//-----------------------------------------------------------------------------
// Name: LoadBugImages()
// Desc: Load the bug resource images into our various flipping surfaces.
//-----------------------------------------------------------------------------
static HRESULT
LoadBugImages()
{
    HRESULT hRet;
    int CallCount = 0;

    // Put the first bug image onto the first buffer of our complex surface.
    ///if (!LoadImageOntoSurface(g_pDDSOverlay, szImg1)) {
    if (!LoadImageOntoSurface(g_pDDSOverlay, MAKEINTRESOURCE(IDB_BUGIMAGE1))) {
    	NKDbgPrintfW( L"TVB::LoadImageOntoSurface fail\r\n" );
	    return (E_FAIL);
	}    

    // Use the enumeration attachment function to load the other images.
    hRet = g_pDDSOverlay->EnumAttachedSurfaces((LPVOID)&CallCount,EnumSurfacesCallback);
    if( DD_OK != hRet )
    	NKDbgPrintfW( L"TVB::EnumAttachedSurfaces fail\r\n" );
    	 
    return (hRet);
}




//-----------------------------------------------------------------------------
// Name: DDInitApp()
// Desc: Do work required for every instance of the application:
//          Create the window, initialize data
//-----------------------------------------------------------------------------
HRESULT
DDInitApp( HINSTANCE hInstance, HWND hWnd )
{
///    HWND                        hWnd;
///    WNDCLASS                    wc;
    DDSURFACEDESC2              ddsd;
    DDCAPS                      ddcaps;
    HRESULT                     hRet;
    LPDIRECTDRAW                pDD;
    DWORD                       dwUpdateFlags = 0;
    DDOVERLAYFX                 ovfx;

///    // Set up and register window class.
///
///    wc.style = CS_HREDRAW | CS_VREDRAW;
///    wc.lpfnWndProc = WindowProc;
///    wc.cbClsExtra = 0;
///    wc.cbWndExtra = 0;
///    wc.hInstance = hInstance;
///    ///wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
///    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
///    wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
///    wc.lpszMenuName = NULL;
///    wc.lpszClassName = NAME;
///    RegisterClass(&wc);
///
///    // Create a window.
///
///    hWnd = CreateWindowEx(WS_EX_TOPMOST,
///                          NAME,
///                          TITLE,
///                          WS_POPUP,
///                          0,
///                          0,
///                          GetSystemMetrics(SM_CXSCREEN),
///                          GetSystemMetrics(SM_CYSCREEN),
///                          NULL,
///                          NULL,
///                          hInstance,
///                          NULL);
///    if (!hWnd)
///        return FALSE;
///    // We never show the window, only set focus to it.
///    SetFocus(hWnd);

	NKDbgPrintfW( L"TVB::DDInitApp +++\r\n" );

	g_hInstance = hInstance;

    // Create the main DirectDraw object

    hRet = DirectDrawCreate(NULL, &pDD, NULL);
    if (hRet != DD_OK) {
        return InitFail(hWnd, hRet, TEXT("DirectDrawCreate FAILED"));
    }

	///NKDbgPrintfW( L"TVB::DDInitApp 111\r\n" );
    // Fetch DirectDraw4 interface.

    hRet = pDD->QueryInterface(IID_IDirectDraw4, (LPVOID *) & g_pDD);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, TEXT("QueryInterface FAILED"));
    pDD->Release();
    pDD = NULL;

	///NKDbgPrintfW( L"TVB::DDInitApp 222\r\n" );
    // Get normal mode.

    hRet = g_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, TEXT("SetCooperativeLevel FAILED"));

	///NKDbgPrintfW( L"TVB::DDInitApp 333\r\n" );
	
    // Get a primary surface interface pointer (only needed for init.)

    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, TEXT("CreateSurface FAILED"));

	///NKDbgPrintfW( L"TVB::DDInitApp 444\r\n" );
    // See if we can support overlays.

    memset(&ddcaps, 0, sizeof(ddcaps));
    ddcaps.dwSize = sizeof(ddcaps);
    hRet = g_pDD->GetCaps(&ddcaps,NULL);
    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, TEXT("GetCaps FAILED"));

    if (!(ddcaps.dwCaps & DDCAPS_OVERLAY)) {
    	NKDbgPrintfW( L"TVB::Overlays are not supported in hardware!!!\r\n" );
        return InitFail(hWnd, hRet, TEXT("Overlays are not supported in hardware!"));
    } else {
    	NKDbgPrintfW( L"TVB::Overlays supported in hardware!!!\r\n" );
    }

	///NKDbgPrintfW( L"TVB::DDInitApp 555\r\n" );
	
    // Get alignment info to compute our overlay surface size.

    rs.left = 0;
    rs.top = 0;
    rs.right = BUG_WIDTH;
    rs.bottom = BUG_HEIGHT;
    if (ddcaps.dwCaps & DDCAPS_ALIGNSIZESRC && ddcaps.dwAlignSizeSrc)
	    rs.right += rs.right % ddcaps.dwAlignSizeSrc;
    
    // Create the overlay flipping surface. We will attempt the pixel formats
    // in our table one at a time until we find one that jives.

    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_FLIP | DDSCAPS_COMPLEX | 
                          DDSCAPS_VIDEOMEMORY;
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_BACKBUFFERCOUNT | 
                   DDSD_PIXELFORMAT;
	ddsd.dwWidth = rs.right;
	ddsd.dwHeight = rs.bottom;
    ddsd.dwBackBufferCount = 2;

    int i = 0;
    do {
	    ddsd.ddpfPixelFormat = ddpfOverlayFormats[i];
	    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSOverlay, NULL);
    } while (hRet != DD_OK && (++i < PF_TABLE_SIZE));

    if (hRet != DD_OK)
        return InitFail(hWnd, hRet, TEXT("Unable to create overlay surface!"));

    // Load the images.

    if (LoadBugImages() != DD_OK)
        return InitFail(hWnd, hRet, TEXT("Unable to load images to overlay surface!"));

    // Finish setting up the overlay.

    int StretchFactor1000 = ddcaps.dwMinOverlayStretch > 1000 ? ddcaps.dwMinOverlayStretch : 1000;

    rd.left=0; 
    rd.top=0;
    // Adding 999 takes care of integer truncation problems.
    rd.right  = (rs.right * StretchFactor1000 + 999) / 1000;
    rd.bottom = rs.bottom * StretchFactor1000 / 1000;
    if (ddcaps.dwCaps & DDCAPS_ALIGNSIZEDEST && ddcaps.dwAlignSizeDest)
	    rd.right = (int)((rd.right + ddcaps.dwAlignSizeDest - 1)/ ddcaps.dwAlignSizeDest) *
                    ddcaps.dwAlignSizeDest;

    // Set the flags we'll send to UpdateOverlay
    dwUpdateFlags = DDOVER_SHOW;

    // Does the overlay hardware support source color keying?
    // If so, we can hide the black background around the image.
    // This probably won't work with YUV formats
    memset(&ovfx, 0, sizeof(ovfx));
    ovfx.dwSize = sizeof(ovfx);
    if (ddcaps.dwCKeyCaps & DDCKEYCAPS_SRCOVERLAY)
    {
        dwUpdateFlags |= DDOVER_KEYSRCOVERRIDE | DDOVER_DDFX;

        // Create an overlay FX structure so we can specify a source color key.
        // This information is ignored if the DDOVER_SRCKEYOVERRIDE flag 
        // isn't set.
        ovfx.dckSrcColorkey.dwColorSpaceLowValue=0; // black as the color key
        ovfx.dckSrcColorkey.dwColorSpaceHighValue=0;
        NKDbgPrintfW( L"TVB::the overlay hardware support source color keying\r\n" );
    } else
    	NKDbgPrintfW( L"TVB::the overlay hardware no support source color keying\r\n" );

    // Update the overlay parameters.

    hRet = g_pDDSOverlay->UpdateOverlay(&rs, g_pDDSPrimary, &rd, dwUpdateFlags, &ovfx);
    if (hRet != DD_OK)
        InitFail(gsp_hWnd, hRet, TEXT("Unable to show overlay surface!"));

    // Set a bunch of position and velocity module vars.

    g_nOverlayXPos = 0;
    g_nOverlayYPos = 0;
    g_nOverlayXVel = RANDOM_VELOCITY();
    g_nOverlayYVel = RANDOM_VELOCITY();
    g_nOverlayWidth = rd.right - rd.left;
    g_nOverlayHeight = rd.bottom - rd.top;
    
    // Set the "destination position alignment" global so we won't have to
    // keep calling GetCaps() everytime we move the overlay surface.

    if (ddcaps.dwCaps & DDCAPS_ALIGNBOUNDARYDEST)
	    g_dwOverlayXPositionAlignment = ddcaps.dwAlignBoundaryDest;
    else 
	    g_dwOverlayXPositionAlignment = 0;

    // Create a timer to flip the pages.

    if (TIMER_ID != SetTimer(hWnd, TIMER_ID, TIMER_RATE, NULL))
        return InitFail(hWnd, hRet, TEXT("SetTimer FAILED"));

    return DD_OK;
}


void DDdeInitApp()
{

	NKDbgPrintfW( L"TVB::DDdeInitApp +++\r\n" );
	ReleaseAllObjects();
}


BOOL DDOverlayBitmap( 
	HINSTANCE hInstance, 
	HWND hWnd,
	DWORD dwWidth,
	DWORD dwHeight,
	DWORD dwX,
	DWORD dwY,
	DWORD dwSrcColorkey,
	DWORD dwBitMapID,
	BOOL bSetup
)
{
	BOOL bRet = FALSE;
    HRESULT hRet;
    LPDIRECTDRAW pDD;
    DDSURFACEDESC2 ddsd;
    DDCAPS ddcaps;
    DWORD dwUpdateFlags = 0;
    DDOVERLAYFX ovfx;
    DWORD dwXAligned;
	

	NKDbgPrintfW( L"TVB::DDOverlayBitmap +++\r\n" );


	if( !bSetup ) {
	    if (gsp_pDDSOverlay != NULL)
	    {
	        // Use UpdateOverlay() with the DDOVER_HIDE flag to remove an overlay 
		    // from the display.
		    gsp_pDDSOverlay->UpdateOverlay(NULL, gsp_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
	        gsp_pDDSOverlay->Release();
	        gsp_pDDSOverlay = NULL;
	    }
	
	    if (gsp_pDDSPrimary != NULL)
	    {
	        gsp_pDDSPrimary->Release();
	        gsp_pDDSPrimary = NULL;
	    }
	
	    if (gsp_pDD != NULL)
	    {
	        gsp_pDD->Release();
	        gsp_pDD = NULL;
	    }
	    ///goto DDO_EXIT;
	    
	} else {

		gsp_hInst = hInstance;			///store it here 
		gsp_hWnd = hWnd;				///store it here
		gsp_dwBitmapWidth = dwWidth;	///store it here
		gsp_dwBitmapHeight = dwHeight;	///store it here
		gsp_dwSrcColorkey = dwSrcColorkey;	///store it here
		gsp_dwBitMapID1 = dwBitMapID;	///store it here
		gsp_dwX = dwX;					///store it here
		gsp_dwY = dwY;					///store it here
	
	    // Create the main DirectDraw object
	    hRet = DirectDrawCreate(NULL, &pDD, NULL);
	    if (hRet != DD_OK) {
	        InitFail( gsp_hWnd, hRet, TEXT("DirectDrawCreate FAILED") );
	    }
	
	    // Fetch DirectDraw4 interface.
	    hRet = pDD->QueryInterface(IID_IDirectDraw4, (LPVOID *)&gsp_pDD);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("QueryInterface FAILED"));
	    }
	    pDD->Release();
	    pDD = NULL;
	
	    // Get normal mode.
	    hRet = gsp_pDD->SetCooperativeLevel(gsp_hWnd, DDSCL_NORMAL);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("SetCooperativeLevel FAILED"));
	    }
		
	    // Get a primary surface interface pointer (only needed for init.)
	    memset(&ddsd, 0, sizeof(ddsd));
	    ddsd.dwSize = sizeof(ddsd);
	    ddsd.dwFlags = DDSD_CAPS;
	    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	    hRet = gsp_pDD->CreateSurface(&ddsd, &gsp_pDDSPrimary, NULL);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("CreateSurface FAILED"));
	    }
	
	    // See if we can support overlays.
	    memset(&ddcaps, 0, sizeof(ddcaps));
	    ddcaps.dwSize = sizeof(ddcaps);
	    hRet = gsp_pDD->GetCaps(&ddcaps,NULL);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("GetCaps FAILED"));
	    }
	
	    if (!(ddcaps.dwCaps & DDCAPS_OVERLAY)) {
	    	NKDbgPrintfW( L"TVB::Overlays are not supported in hardware!!!\r\n" );
	       	InitFail(gsp_hWnd, hRet, TEXT("Overlays are not supported in hardware!"));
	    } else {
	    	NKDbgPrintfW( L"TVB::Overlays supported in hardware!!!\r\n" );
	    }
	
		
	    // Get alignment info to compute our overlay surface size.
	    gsp_rs.left = 0;
	    gsp_rs.top = 0;
	    gsp_rs.right = gsp_dwBitmapWidth;
	    gsp_rs.bottom = gsp_dwBitmapHeight;
	    if (ddcaps.dwCaps & DDCAPS_ALIGNSIZESRC && ddcaps.dwAlignSizeSrc)
		    gsp_rs.right += gsp_rs.right % ddcaps.dwAlignSizeSrc;
	    
	    // Create the overlay flipping surface. We will attempt the pixel formats
	    // in our table one at a time until we find one that jives.
	    memset(&ddsd, 0, sizeof(ddsd));
	    ddsd.dwSize = sizeof(ddsd);
	    ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_FLIP | DDSCAPS_COMPLEX | 
	                          DDSCAPS_VIDEOMEMORY;
	    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_BACKBUFFERCOUNT | 
	                   DDSD_PIXELFORMAT;
		ddsd.dwWidth = gsp_rs.right;
		ddsd.dwHeight = gsp_rs.bottom;
	    ddsd.dwBackBufferCount = 2;
	
	    int i = 0;
	    do {
		    ddsd.ddpfPixelFormat = ddpfOverlayFormats[i];
		    hRet = gsp_pDD->CreateSurface(&ddsd, &gsp_pDDSOverlay, NULL);
	    } while (hRet != DD_OK && (++i < PF_TABLE_SIZE));
	
	    if (hRet != DD_OK)
	        InitFail(gsp_hWnd, hRet, TEXT("Unable to create overlay surface!"));
	
	
	    // Load the images.
	    ///if (LoadBugImages() != DD_OK)
	    ///    InitFail(gsp_hWnd, hRet, TEXT("Unable to load images to overlay surface!"));
	    if (!spLoadImageOntoSurface(gsp_pDDSOverlay, MAKEINTRESOURCE(gsp_dwBitMapID1))) {
	    	NKDbgPrintfW( L"TVB::LoadImageOntoSurface fail\r\n" );
		}
	
	    // Finish setting up the overlay.
	    int StretchFactor1000 = ddcaps.dwMinOverlayStretch > 1000 ? ddcaps.dwMinOverlayStretch : 1000;
	
	    gsp_rd.left=0; 
	    gsp_rd.top=0;
	    // Adding 999 takes care of integer truncation problems.
	    gsp_rd.right  = (gsp_rs.right * StretchFactor1000 + 999) / 1000;
	    gsp_rd.bottom = gsp_rs.bottom * StretchFactor1000 / 1000;
	    if (ddcaps.dwCaps & DDCAPS_ALIGNSIZEDEST && ddcaps.dwAlignSizeDest)
		    gsp_rd.right = (int)((gsp_rd.right + ddcaps.dwAlignSizeDest - 1)/ ddcaps.dwAlignSizeDest) *
	                    ddcaps.dwAlignSizeDest;
	
	    // Set the flags we'll send to UpdateOverlay
	    dwUpdateFlags = DDOVER_SHOW;
	
	    // Does the overlay hardware support source color keying?
	    // If so, we can hide the black background around the image.
	    // This probably won't work with YUV formats
	    memset(&ovfx, 0, sizeof(ovfx));
	    ovfx.dwSize = sizeof(ovfx);
	    if (ddcaps.dwCKeyCaps & DDCKEYCAPS_SRCOVERLAY)
	    {
	        dwUpdateFlags |= DDOVER_KEYSRCOVERRIDE | DDOVER_DDFX;
	
	        // Create an overlay FX structure so we can specify a source color key.
	        // This information is ignored if the DDOVER_SRCKEYOVERRIDE flag 
	        // isn't set.
	        ovfx.dckSrcColorkey.dwColorSpaceLowValue=0; // black as the color key
	        ovfx.dckSrcColorkey.dwColorSpaceHighValue=0;
	        NKDbgPrintfW( L"TVB::the overlay hardware support source color keying\r\n" );
	    } else
	    	NKDbgPrintfW( L"TVB::the overlay hardware no support source color keying\r\n" );
	
	
	    // Update the overlay parameters.
	    hRet = gsp_pDDSOverlay->UpdateOverlay(&gsp_rs, gsp_pDDSPrimary, &gsp_rd, dwUpdateFlags, &ovfx);
	    if (hRet != DD_OK)
	        InitFail(gsp_hWnd, hRet, TEXT("Unable to show overlay surface!"));
	
	    // Set a bunch of position and velocity module vars.
	
	    gsp_nOverlayXPos = gsp_dwX;
	    gsp_nOverlayYPos = gsp_dwY;
	    gsp_nOverlayXVel = 0;
	    gsp_nOverlayYVel = 0;
	    gsp_nOverlayWidth = gsp_rd.right - gsp_rd.left;
	    gsp_nOverlayHeight = gsp_rd.bottom - gsp_rd.top;
	    
	    // Set the "destination position alignment" global so we won't have to
	    // keep calling GetCaps() everytime we move the overlay surface.
	
	    if (ddcaps.dwCaps & DDCAPS_ALIGNBOUNDARYDEST)
		    gsp_dwOverlayXPositionAlignment = ddcaps.dwAlignBoundaryDest;
	    else 
		    gsp_dwOverlayXPositionAlignment = 0;


    	// We need to check for any alignment restrictions on the X position.
    	
    	if (gsp_dwOverlayXPositionAlignment)
		    dwXAligned = gsp_nOverlayXPos - gsp_nOverlayXPos % gsp_dwOverlayXPositionAlignment;
    	else
		    dwXAligned = gsp_nOverlayXPos;
    	
    	// Set the overlay to it's new position.
    	
    	hRet = gsp_pDDSOverlay->SetOverlayPosition(dwXAligned, gsp_nOverlayYPos);
    	if (hRet == DDERR_SURFACELOST)
    	{
		    if (FAILED(RestoreAllSurfaces())) 
		        bRet = FALSE;
    	}

	}
///DDO_EXIT:

	NKDbgPrintfW( L"TVB::DDOverlayBitmap %s ---\r\n", bSetup?L"Init":L"Deinit" );
	return bRet;
}


BOOL DDOverlayText( 
	HINSTANCE hInstance, 
	HWND hWnd,
	DWORD dwWidth,
	DWORD dwHeight,
	DWORD dwX,
	DWORD dwY,
	DWORD dwSrcColorkey,
	DWORD dwBitMapID,
	BOOL bSetup
)
{
	BOOL bRet = FALSE;
    HRESULT hRet;
    LPDIRECTDRAW pDD;
    DDSURFACEDESC2 ddsd;
    DDCAPS ddcaps;
    DWORD dwUpdateFlags = 0;
    DDOVERLAYFX ovfx;
    DWORD dwXAligned;
	

	NKDbgPrintfW( L"TVB::DDOverlayText +++\r\n" );


	if( !bSetup ) {
	    if (gsp_pDDSOverlay != NULL)
	    {
	        // Use UpdateOverlay() with the DDOVER_HIDE flag to remove an overlay 
		    // from the display.
		    gsp_pDDSOverlay->UpdateOverlay(NULL, gsp_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
	        gsp_pDDSOverlay->Release();
	        gsp_pDDSOverlay = NULL;
	    }
	
	    if (gsp_pDDSPrimary != NULL)
	    {
	        gsp_pDDSPrimary->Release();
	        gsp_pDDSPrimary = NULL;
	    }
	
	    if (gsp_pDD != NULL)
	    {
	        gsp_pDD->Release();
	        gsp_pDD = NULL;
	    }
	    ///goto DDO_EXIT;
	    
	} else {

		gsp_hInst = hInstance;			///store it here 
		gsp_hWnd = hWnd;				///store it here
		gsp_dwBitmapWidth = dwWidth;	///store it here
		gsp_dwBitmapHeight = dwHeight;	///store it here
		gsp_dwSrcColorkey = dwSrcColorkey;	///store it here
		gsp_dwBitMapID1 = dwBitMapID;	///store it here
		gsp_dwX = dwX;					///store it here
		gsp_dwY = dwY;					///store it here
	
	    // Create the main DirectDraw object
	    hRet = DirectDrawCreate(NULL, &pDD, NULL);
	    if (hRet != DD_OK) {
	        InitFail( gsp_hWnd, hRet, TEXT("DirectDrawCreate FAILED") );
	    }
	
	    // Fetch DirectDraw4 interface.
	    hRet = pDD->QueryInterface(IID_IDirectDraw4, (LPVOID *)&gsp_pDD);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("QueryInterface FAILED"));
	    }
	    pDD->Release();
	    pDD = NULL;
	
	    // Get normal mode.
	    hRet = gsp_pDD->SetCooperativeLevel(gsp_hWnd, DDSCL_NORMAL);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("SetCooperativeLevel FAILED"));
	    }
		
	    // Get a primary surface interface pointer (only needed for init.)
	    memset(&ddsd, 0, sizeof(ddsd));
	    ddsd.dwSize = sizeof(ddsd);
	    ddsd.dwFlags = DDSD_CAPS;
	    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	    hRet = gsp_pDD->CreateSurface(&ddsd, &gsp_pDDSPrimary, NULL);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("CreateSurface FAILED"));
	    }
	
	    // See if we can support overlays.
	    memset(&ddcaps, 0, sizeof(ddcaps));
	    ddcaps.dwSize = sizeof(ddcaps);
	    hRet = gsp_pDD->GetCaps(&ddcaps,NULL);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("GetCaps FAILED"));
	    }
	
	    if (!(ddcaps.dwCaps & DDCAPS_OVERLAY)) {
	    	NKDbgPrintfW( L"TVB::Overlays are not supported in hardware!!!\r\n" );
	       	InitFail(gsp_hWnd, hRet, TEXT("Overlays are not supported in hardware!"));
	    } else {
	    	NKDbgPrintfW( L"TVB::Overlays supported in hardware!!!\r\n" );
	    }
	
		
	    // Get alignment info to compute our overlay surface size.
	    gsp_rs.left = 0;
	    gsp_rs.top = 0;
	    gsp_rs.right = gsp_dwBitmapWidth;
	    gsp_rs.bottom = gsp_dwBitmapHeight;
	    if (ddcaps.dwCaps & DDCAPS_ALIGNSIZESRC && ddcaps.dwAlignSizeSrc)
		    gsp_rs.right += gsp_rs.right % ddcaps.dwAlignSizeSrc;
	    
	    // Create the overlay flipping surface. We will attempt the pixel formats
	    // in our table one at a time until we find one that jives.
	    memset(&ddsd, 0, sizeof(ddsd));
	    ddsd.dwSize = sizeof(ddsd);
	    ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_FLIP | DDSCAPS_COMPLEX | 
	                          DDSCAPS_VIDEOMEMORY;
	    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_BACKBUFFERCOUNT | 
	                   DDSD_PIXELFORMAT;
		ddsd.dwWidth = gsp_rs.right;
		ddsd.dwHeight = gsp_rs.bottom;
	    ddsd.dwBackBufferCount = 2;
	
	    int i = 0;
	    do {
		    ddsd.ddpfPixelFormat = ddpfOverlayFormats[i];
		    hRet = gsp_pDD->CreateSurface(&ddsd, &gsp_pDDSOverlay, NULL);
	    } while (hRet != DD_OK && (++i < PF_TABLE_SIZE));
	
	    if (hRet != DD_OK)
	        InitFail(gsp_hWnd, hRet, TEXT("Unable to create overlay surface!"));
	
	
	    // Load the images.
	    ///if (LoadBugImages() != DD_OK)
	    ///    InitFail(gsp_hWnd, hRet, TEXT("Unable to load images to overlay surface!"));
	    ///if (!spLoadImageOntoSurface(gsp_pDDSOverlay, MAKEINTRESOURCE(gsp_dwBitMapID))) {
	    ///	NKDbgPrintfW( L"TVB::LoadImageOntoSurface fail\r\n" );
	    if (!spTextDCOntoSurface(gsp_pDDSOverlay, DDTEXT)) {
	    	NKDbgPrintfW( L"TVB::spTextDCOntoSurface fail\r\n" );
		}
	
	    // Finish setting up the overlay.
	    int StretchFactor1000 = ddcaps.dwMinOverlayStretch > 1000 ? ddcaps.dwMinOverlayStretch : 1000;
	
	    gsp_rd.left=0; 
	    gsp_rd.top=0;
	    // Adding 999 takes care of integer truncation problems.
	    gsp_rd.right  = (gsp_rs.right * StretchFactor1000 + 999) / 1000;
	    gsp_rd.bottom = gsp_rs.bottom * StretchFactor1000 / 1000;
	    if (ddcaps.dwCaps & DDCAPS_ALIGNSIZEDEST && ddcaps.dwAlignSizeDest)
		    gsp_rd.right = (int)((gsp_rd.right + ddcaps.dwAlignSizeDest - 1)/ ddcaps.dwAlignSizeDest) *
	                    ddcaps.dwAlignSizeDest;
	
	    // Set the flags we'll send to UpdateOverlay
	    dwUpdateFlags = DDOVER_SHOW;
	
	    // Does the overlay hardware support source color keying?
	    // If so, we can hide the black background around the image.
	    // This probably won't work with YUV formats
	    memset(&ovfx, 0, sizeof(ovfx));
	    ovfx.dwSize = sizeof(ovfx);
	    if (ddcaps.dwCKeyCaps & DDCKEYCAPS_SRCOVERLAY)
	    {
	        dwUpdateFlags |= DDOVER_KEYSRCOVERRIDE | DDOVER_DDFX;
	
	        // Create an overlay FX structure so we can specify a source color key.
	        // This information is ignored if the DDOVER_SRCKEYOVERRIDE flag 
	        // isn't set.
	        ovfx.dckSrcColorkey.dwColorSpaceLowValue=0; // black as the color key
	        ovfx.dckSrcColorkey.dwColorSpaceHighValue=0;
	        NKDbgPrintfW( L"TVB::the overlay hardware support source color keying\r\n" );
	    } else
	    	NKDbgPrintfW( L"TVB::the overlay hardware no support source color keying\r\n" );
	
	
	    // Update the overlay parameters.
	    hRet = gsp_pDDSOverlay->UpdateOverlay(&gsp_rs, gsp_pDDSPrimary, &gsp_rd, dwUpdateFlags, &ovfx);
	    if (hRet != DD_OK)
	        InitFail(gsp_hWnd, hRet, TEXT("Unable to show overlay surface!"));
	
	    // Set a bunch of position and velocity module vars.
	
	    gsp_nOverlayXPos = gsp_dwX;
	    gsp_nOverlayYPos = gsp_dwY;
	    gsp_nOverlayXVel = 0;
	    gsp_nOverlayYVel = 0;
	    gsp_nOverlayWidth = gsp_rd.right - gsp_rd.left;
	    gsp_nOverlayHeight = gsp_rd.bottom - gsp_rd.top;
	    
	    // Set the "destination position alignment" global so we won't have to
	    // keep calling GetCaps() everytime we move the overlay surface.
	
	    if (ddcaps.dwCaps & DDCAPS_ALIGNBOUNDARYDEST)
		    gsp_dwOverlayXPositionAlignment = ddcaps.dwAlignBoundaryDest;
	    else 
		    gsp_dwOverlayXPositionAlignment = 0;


    	// We need to check for any alignment restrictions on the X position.
    	
    	if (gsp_dwOverlayXPositionAlignment)
		    dwXAligned = gsp_nOverlayXPos - gsp_nOverlayXPos % gsp_dwOverlayXPositionAlignment;
    	else
		    dwXAligned = gsp_nOverlayXPos;
    	
    	// Set the overlay to it's new position.
    	
    	hRet = gsp_pDDSOverlay->SetOverlayPosition(dwXAligned, gsp_nOverlayYPos);
    	if (hRet == DDERR_SURFACELOST)
    	{
		    if (FAILED(RestoreAllSurfaces())) 
		        bRet = FALSE;
    	}

	}
///DDO_EXIT:

	NKDbgPrintfW( L"TVB::DDOverlayText %s ---\r\n", bSetup?L"Init":L"Deinit" );
	return bRet;
}


BOOL DDOverlayOSD( 
	HINSTANCE hInstance, 
	HWND hWnd,
	DWORD dwWidth,
	DWORD dwHeight,
	DWORD dwX,
	DWORD dwY,
	DWORD dwSrcColorkey,
	LPCTSTR lpstrText,
	DWORD dwBitMapID1,
	DWORD dwBM1Width,
	DWORD dwBM1Height,
	DWORD dwBM1X,
	DWORD dwBM1Y,
	DWORD dwBitMapID2,
	DWORD dwBM2Width,
	DWORD dwBM2Height,
	DWORD dwBM2X,
	DWORD dwBM2Y,
	BOOL bSetup
)
{
	BOOL bRet = FALSE;
    HRESULT hRet;
    LPDIRECTDRAW pDD;
    DDSURFACEDESC2 ddsd;
    DDCAPS ddcaps;
    DWORD dwUpdateFlags = 0;
    DDOVERLAYFX ovfx;
    DWORD dwXAligned;
	

	NKDbgPrintfW( L"TVB::DDOverlayOSD +++\r\n" );


	if( !bSetup ) {
	    if (gsp_pDDSOverlay != NULL)
	    {
	        // Use UpdateOverlay() with the DDOVER_HIDE flag to remove an overlay 
		    // from the display.
		    gsp_pDDSOverlay->UpdateOverlay(NULL, gsp_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
	        gsp_pDDSOverlay->Release();
	        gsp_pDDSOverlay = NULL;
	    }
	
	    if (gsp_pDDSPrimary != NULL)
	    {
	        gsp_pDDSPrimary->Release();
	        gsp_pDDSPrimary = NULL;
	    }
	
	    if (gsp_pDD != NULL)
	    {
	        gsp_pDD->Release();
	        gsp_pDD = NULL;
	    }
	    ///goto DDO_EXIT;
	    
	} else {

		gsp_hInst = hInstance;			///store it here 
		gsp_hWnd = hWnd;				///store it here
		gsp_dwOSDWidth = dwWidth;	///store it here
		gsp_dwOSDHeight = dwHeight;	///store it here
		gsp_dwX = dwX;					///store it here
		gsp_dwY = dwY;					///store it here
		gsp_dwSrcColorkey = dwSrcColorkey;	///store it here
		gsp_dwBitMapID1 = dwBitMapID1;	///store it here
		gsp_dwBM1Width = dwBM1Width;
		gsp_dwBM1Height = dwBM1Height;
		gsp_dwBM1X = dwBM1X;
		gsp_dwBM1Y = dwBM1Y;
		gsp_dwBitMapID2 = dwBitMapID2;	///store it here
		gsp_dwBM2Width = dwBM2Width;
		gsp_dwBM2Height = dwBM2Height;
		gsp_dwBM2X = dwBM2X;
		gsp_dwBM2Y = dwBM2Y;



	    // Create the main DirectDraw object
	    hRet = DirectDrawCreate(NULL, &pDD, NULL);
	    if (hRet != DD_OK) {
	        InitFail( gsp_hWnd, hRet, TEXT("DirectDrawCreate FAILED") );
	    }
	
	    // Fetch DirectDraw4 interface.
	    hRet = pDD->QueryInterface(IID_IDirectDraw4, (LPVOID *)&gsp_pDD);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("QueryInterface FAILED"));
	    }
	    pDD->Release();
	    pDD = NULL;
	
	    // Get normal mode.
	    hRet = gsp_pDD->SetCooperativeLevel(gsp_hWnd, DDSCL_NORMAL);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("SetCooperativeLevel FAILED"));
	    }
		
	    // Get a primary surface interface pointer (only needed for init.)
	    memset(&ddsd, 0, sizeof(ddsd));
	    ddsd.dwSize = sizeof(ddsd);
	    ddsd.dwFlags = DDSD_CAPS;
	    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	    hRet = gsp_pDD->CreateSurface(&ddsd, &gsp_pDDSPrimary, NULL);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("CreateSurface FAILED"));
	    }
	
	    // See if we can support overlays.
	    memset(&ddcaps, 0, sizeof(ddcaps));
	    ddcaps.dwSize = sizeof(ddcaps);
	    hRet = gsp_pDD->GetCaps(&ddcaps,NULL);
	    if (hRet != DD_OK) {
	        InitFail(gsp_hWnd, hRet, TEXT("GetCaps FAILED"));
	    }
	
	    if (!(ddcaps.dwCaps & DDCAPS_OVERLAY)) {
	    	NKDbgPrintfW( L"TVB::Overlays are not supported in hardware!!!\r\n" );
	       	InitFail(gsp_hWnd, hRet, TEXT("Overlays are not supported in hardware!"));
	    } else {
	    	NKDbgPrintfW( L"TVB::Overlays supported in hardware!!!\r\n" );
	    }
	
		
	    // Get alignment info to compute our overlay surface size.
	    gsp_rs.left = 0;
	    gsp_rs.top = 0;
	    gsp_rs.right = gsp_dwOSDWidth;
	    gsp_rs.bottom = gsp_dwOSDHeight;
	    if (ddcaps.dwCaps & DDCAPS_ALIGNSIZESRC && ddcaps.dwAlignSizeSrc)
		    gsp_rs.right += gsp_rs.right % ddcaps.dwAlignSizeSrc;
	    
	    // Create the overlay flipping surface. We will attempt the pixel formats
	    // in our table one at a time until we find one that jives.
	    memset(&ddsd, 0, sizeof(ddsd));
	    ddsd.dwSize = sizeof(ddsd);
	    ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_FLIP | DDSCAPS_COMPLEX | 
	                          DDSCAPS_VIDEOMEMORY;
	    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_BACKBUFFERCOUNT | 
	                   DDSD_PIXELFORMAT;
		ddsd.dwWidth = gsp_rs.right;
		ddsd.dwHeight = gsp_rs.bottom;
	    ddsd.dwBackBufferCount = 2;
	
	    int i = 0;
	    do {
		    ddsd.ddpfPixelFormat = ddpfOverlayFormats[i];
		    hRet = gsp_pDD->CreateSurface(&ddsd, &gsp_pDDSOverlay, NULL);
	    } while (hRet != DD_OK && (++i < PF_TABLE_SIZE));
	
	    if (hRet != DD_OK)
	        InitFail(gsp_hWnd, hRet, TEXT("Unable to create overlay surface!"));
	
	
	    // Load the images.
	    ///if (LoadBugImages() != DD_OK)
	    ///    InitFail(gsp_hWnd, hRet, TEXT("Unable to load images to overlay surface!"));
	    ///if (!spLoadImageOntoSurface(gsp_pDDSOverlay, MAKEINTRESOURCE(gsp_dwBitMapID))) {
	    ///	NKDbgPrintfW( L"TVB::LoadImageOntoSurface fail\r\n" );
	    ///if (!spTextDCOntoSurface(gsp_pDDSOverlay, DDTEXT)) {
	    ///	NKDbgPrintfW( L"TVB::spTextDCOntoSurface fail\r\n" );
	    ///if (!spLoadOSDDCOntoSurface(gsp_pDDSOverlay, lpstrText, MAKEINTRESOURCE(gsp_dwBitMapID))) {
	    ///	NKDbgPrintfW( L"TVB::spLoadOSDDCOntoSurface fail\r\n" );
	    if (!spLoadOSDDCOntoSurface(gsp_pDDSOverlay, lpstrText, MAKEINTRESOURCE(gsp_dwBitMapID1), MAKEINTRESOURCE(gsp_dwBitMapID2))) {
	    	NKDbgPrintfW( L"TVB::spLoadOSDDCOntoSurface fail\r\n" );
		}
	
	    // Finish setting up the overlay.
	    int StretchFactor1000 = ddcaps.dwMinOverlayStretch > 1000 ? ddcaps.dwMinOverlayStretch : 1000;
	
	    gsp_rd.left=0; 
	    gsp_rd.top=0;
	    // Adding 999 takes care of integer truncation problems.
	    gsp_rd.right  = (gsp_rs.right * StretchFactor1000 + 999) / 1000;
	    gsp_rd.bottom = gsp_rs.bottom * StretchFactor1000 / 1000;
	    if (ddcaps.dwCaps & DDCAPS_ALIGNSIZEDEST && ddcaps.dwAlignSizeDest)
		    gsp_rd.right = (int)((gsp_rd.right + ddcaps.dwAlignSizeDest - 1)/ ddcaps.dwAlignSizeDest) *
	                    ddcaps.dwAlignSizeDest;
	
	    // Set the flags we'll send to UpdateOverlay
	    dwUpdateFlags = DDOVER_SHOW;
	
	    // Does the overlay hardware support source color keying?
	    // If so, we can hide the black background around the image.
	    // This probably won't work with YUV formats
	    memset(&ovfx, 0, sizeof(ovfx));
	    ovfx.dwSize = sizeof(ovfx);
	    if (ddcaps.dwCKeyCaps & DDCKEYCAPS_SRCOVERLAY)
	    {
	        dwUpdateFlags |= DDOVER_KEYSRCOVERRIDE | DDOVER_DDFX;
	
	        // Create an overlay FX structure so we can specify a source color key.
	        // This information is ignored if the DDOVER_SRCKEYOVERRIDE flag 
	        // isn't set.
	        ovfx.dckSrcColorkey.dwColorSpaceLowValue=0; // black as the color key
	        ovfx.dckSrcColorkey.dwColorSpaceHighValue=0;
	        NKDbgPrintfW( L"TVB::the overlay hardware support source color keying\r\n" );
	    } else
	    	NKDbgPrintfW( L"TVB::the overlay hardware no support source color keying\r\n" );
	
	
	    // Update the overlay parameters.
	    hRet = gsp_pDDSOverlay->UpdateOverlay(&gsp_rs, gsp_pDDSPrimary, &gsp_rd, dwUpdateFlags, &ovfx);
	    if (hRet != DD_OK)
	        InitFail(gsp_hWnd, hRet, TEXT("Unable to show overlay surface!"));
	
	    // Set a bunch of position and velocity module vars.
	
	    gsp_nOverlayXPos = gsp_dwX;
	    gsp_nOverlayYPos = gsp_dwY;
	    gsp_nOverlayXVel = 0;
	    gsp_nOverlayYVel = 0;
	    gsp_nOverlayWidth = gsp_rd.right - gsp_rd.left;
	    gsp_nOverlayHeight = gsp_rd.bottom - gsp_rd.top;
	    
	    // Set the "destination position alignment" global so we won't have to
	    // keep calling GetCaps() everytime we move the overlay surface.
	
	    if (ddcaps.dwCaps & DDCAPS_ALIGNBOUNDARYDEST)
		    gsp_dwOverlayXPositionAlignment = ddcaps.dwAlignBoundaryDest;
	    else 
		    gsp_dwOverlayXPositionAlignment = 0;


    	// We need to check for any alignment restrictions on the X position.
    	
    	if (gsp_dwOverlayXPositionAlignment)
		    dwXAligned = gsp_nOverlayXPos - gsp_nOverlayXPos % gsp_dwOverlayXPositionAlignment;
    	else
		    dwXAligned = gsp_nOverlayXPos;
    	
    	// Set the overlay to it's new position.
    	
    	hRet = gsp_pDDSOverlay->SetOverlayPosition(dwXAligned, gsp_nOverlayYPos);
    	if (hRet == DDERR_SURFACELOST)
    	{
		    if (FAILED(RestoreAllSurfaces())) 
		        bRet = FALSE;
    	}

	}
///DDO_EXIT:

	NKDbgPrintfW( L"TVB::DDOverlayOSD %s ---\r\n", bSetup?L"Init":L"Deinit" );
	return bRet;
}
/*

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF

  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO

  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A

  PARTICULAR PURPOSE.

  

    This is "Sample Code" and is distributable subject to the terms of the end user license agreement.

*/

// ****************************************************************************
// FILE: dvorak_implementation.cpp
// ABTRACT: Dvorak IInputMethod implementation
// ****************************************************************************

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include <sipapi.h>
#include <keybd.h>
#include "resource.h"
#include "dllmain.h"
#include "dvorak_implementation.h"
#include "resrc1.h"

//
// The KEYENTRY structure describes a single key on the keyboard.
//
typedef struct {
    BYTE    bVk;
    UINT    wcUnshift;
    UINT    wcShift;
    UINT    nCtrl;
    DWORD   fdwFlags;
    int     nLeft;
} KEYENTRY;


//


#define NUM_ROWS            5
#define MAX_KEYS_IN_ROW     14
#define BITMAP_HEIGHT       80
#define BITMAP_WIDTH        240
#define SENTINEL            VK_END
#define NO_CTL              0xFFFFFFFF


//
// Globals.
//
static HBITMAP		g_hbmKeybd;
static HBITMAP		g_hbmOld;
static HDC			g_hdcKeybd;
static int			g_nDown;
static HWND			g_hwndMain;
static IIMCallback*	g_pIMCallback;
static HIMAGELIST	g_hImagelistWide;
static HIMAGELIST	g_hImagelistNarrow;

static TCHAR g_ptszClassName[] = TEXT("42429670-ae04-11d0-a4f8-00aa00a749b9 - FullDvorakIMWndClass");


// To change keys but preserve a 2-screen (regular and CAPS) implementation, 
// change ONLY the below KEYENTRY array and keybd.bmp file

//
// Dvorak Keyboard layout.  Here we define the VK code, shift, unshifted, and control
// characters, and whether or not the key generates a character or just a VK
// code (and thus should be sent via SendVirtualKey).
//


KEYENTRY g_keys[NUM_ROWS][MAX_KEYS_IN_ROW + 1] = {

//
//    virtual       Un-
//    key           shift   shift   control                 left
//    code          char    char    char    flags           coordinate
//

// first row
{
    { VK_ESCAPE,    0,      0,      NO_CTL, F_VK,           0   },
    { '1',          '1',    '!',    NO_CTL, 0,              17  },
    { '2',          '2',    '@',    0,      0,              34  },
    { '3',          '3',    '#',    NO_CTL, 0,              51  },
    { '4',          '4',    '$',    NO_CTL, 0,              68  },
    { '5',          '5',    '%',    NO_CTL, 0,              85  },
    { '6',          '6',    '^',    30,     0,              102 },
    { '7',          '7',    '&',    NO_CTL, 0,              119 },
    { '8',          '8',    '*',    NO_CTL, 0,              136 },
    { '9',          '9',    '(',    NO_CTL, 0,              153 },
    { '0',          '0',    ')',    NO_CTL, 0,              170 },
    { VK_LBRACKET,  '[',    '{',    27,	    0,              187 },
    { VK_RBRACKET,  ']',    '}',    29,		0,              204 },
    { VK_BACKSLASH, '\\',   '|',    28,     0,              221 },
    { SENTINEL,     0,      0,      NO_CTL, 0,              239 }
},


// second row
{
    { VK_TAB,       0,      0,      NO_CTL, F_VK,           0   },
    { VK_APOSTROPHE,'\'',   '\"',   NO_CTL, 0,              25  },
    { VK_COMMA,     ',',    '<',    NO_CTL, 0,              42  },
    { VK_PERIOD,    '.',    '>',    NO_CTL, 0,              59  },
    { 'P',          'p',    'P',    16,     0,              76  },
    { 'Y',          'y',    'Y',    25,     0,              93  },
    { 'F',          'f',    'F',    6,      0,              110 },
    { 'G',          'g',    'G',    7,      0,              127 },
    { 'C',          'c',    'C',    3,      0,              144 },
    { 'R',          'r',    'R',    18,     0,              161 },
    { 'L',          'l',    'L',    12,     0,              178 },
    { VK_SLASH,     '/',    '?',    NO_CTL, 0,              195 },
    { VK_EQUAL,     '=',    '+',    NO_CTL, 0,              212 },
    { VK_RETURN,    '\xD',  '\xD',  10,     0,              229 },
    { SENTINEL,     0,      0,      NO_CTL, 0,              239 }
},

// third row
{
    { VK_BACK,      '\x8',  '\x8',  127,    0,              0   },
    { 'A',          'a',    'A',    1,      0,              31  },
    { 'O',          'o',    'O',    15,     0,              48  },
    { 'E',          'e',    'E',    5,      0,              65  },
    { 'U',          'u',    'U',    21,     0,              82  },
    { 'I',          'i',    'I',    9,      0,              99  },
    { 'D',          'd',    'D',    4,      0,              116 },
    { 'H',          'h',    'H',    8,      0,              133 },
    { 'T',          't',    'T',    20,     0,              150 },
    { 'N',          'n',    'N',    14,     0,              167 },
    { 'S',          's',    'S',    19,     0,              184 },
    { VK_HYPHEN,    '-',    '_',    31,		0,              201 },
    { VK_RETURN,    '\xD',  '\xD',  10,     0,              218 },
    { SENTINEL,     0,      0,      NO_CTL, 0,              239 }
},

// fourth row
{
    { VK_SHIFT,     0,      0,      NO_CTL, F_VK | F_STK | F_REDRAW, 0 },
    { VK_SEMICOLON, ';',    ':',    NO_CTL, 0,              39  },
    { 'Q',          'q',    'Q',    17,     0,              56  },
    { 'J',          'j',    'J',    10,     0,              73  },
    { 'K',          'k',    'K',    11,     0,              90  },
    { 'X',          'x',    'X',    24,     0,              107 },
    { 'B',          'b',    'B',    2,		0,              124 },
    { 'M',          'm',    'M',    13,     0,              141 },
    { 'W',          'w',    'W',    23,		0,              158 },
    { 'V',          'v',    'V',    22,		0,              175 },
    { 'Z',          'z',    'Z',    26,		0,              192 },
    { VK_CAPITAL,   0,      0,      NO_CTL, F_VK | F_STK | F_REDRAW, 209 },
    { SENTINEL,     0,      0,      NO_CTL, 0,              239 }
},

// fifth row
{
    { VK_CONTROL,   0,      0,      NO_CTL, F_VK | F_STK,   0   },
    { VK_LWIN,      0,      0,      NO_CTL, F_VK | F_STK,   22  },
    { VK_LMENU,     0,      0,      NO_CTL, F_VK | F_STK,   44  },
    { VK_SPACE,     ' ',    ' ',    32,     0,              65  },
    { VK_DELETE,    0,      0,      NO_CTL, F_VK,           130 },
    { VK_UP,        0,      0,      NO_CTL, F_VK,           150 },
    { VK_DOWN,      0,      0,      NO_CTL, F_VK,           167 },
    { VK_LEFT,      0,      0,      NO_CTL, F_VK,           184 },
    { VK_RIGHT,     0,      0,      NO_CTL, F_VK,           201 },
    { SENTINEL,     0,      0,      NO_CTL, 0,              239 }
} };


//
// Y coordinates of rows in the keyboard.
//
static int g_nRowCoord[NUM_ROWS + 1] = { 0, 16, 32, 48, 64, 79 };


//
// Prototypes.
//
static BOOL WINAPI IM_PressKey( BOOL fPress, KEYENTRY *pKey, RECT *prcKey );


//
// Macros
//
#define SHIFTED() ((g_keys[3][0].fdwFlags & F_DOWN) ^ (g_keys[3][11].fdwFlags & F_DOWN))
#define CONTROLLED() (g_keys[4][0].fdwFlags & F_DOWN)
#define ALTED() (g_keys[4][2].fdwFlags & F_DOWN)
#define WINED() (g_keys[4][1].fdwFlags & F_DOWN)


//
// Paint an area of the keyboard.
//
static void WINAPI IM_DrawArea( HDC hdc, RECT *prcUpdate )
{
    KEYENTRY*	pKey;
    RECT		rc;
	RECT		rcDummy;
    int			nRow		= 0;
	int			nDrawn		= 0;


    //
    // Draw the normal bitmap
    //

    BitBlt(
		hdc,
        prcUpdate->left,
        prcUpdate->top,
        prcUpdate->right - prcUpdate->left,
        prcUpdate->bottom - prcUpdate->top,
        g_hdcKeybd,
        prcUpdate->left,
        prcUpdate->top + (SHIFTED() ? BITMAP_HEIGHT : 0),
        SRCCOPY );


    //
    // Now draw all keys that are down.
    //

    if( nDrawn < g_nDown ) 
	{
        for( nRow = 0; nRow < NUM_ROWS; nRow++ ) 
		{
            pKey = g_keys[nRow];
            while( nDrawn < g_nDown && pKey->bVk != SENTINEL ) 
			{
                if( pKey->fdwFlags & F_DOWN ) 
				{
                    nDrawn++;
                    rc.left   = pKey->nLeft;
                    rc.right  = (pKey + 1)->nLeft;
                    rc.top    = g_nRowCoord[nRow];
                    rc.bottom = g_nRowCoord[nRow + 1];
                    InflateRect( &rc, -1, -1 );
                    if( IntersectRect( &rcDummy, prcUpdate, &rc ) ) 
					{
                        BitBlt(
                            hdc,
                            rc.left,
                            rc.top,
                            rc.right - rc.left + 1,
                            rc.bottom - rc.top + 1,
                            NULL,
                            0,
                            0,
                            DSTINVERT );
                    }
                }
                pKey++;
            }
        }
    }

    return;
}


//
// Refresh the entire keyboard bitmap.
//
static void WINAPI IM_SwitchBitmap( void )
{
    HDC		hdc;
    RECT	rc;

    GetClientRect( g_hwndMain, &rc );
    hdc = GetDC( g_hwndMain );
    IM_DrawArea( hdc, &rc );
    ReleaseDC( g_hwndMain, hdc );

    return;
}


//
// Determine what key contains the point specified in rcKey.left,rcKey.top.
// Fill in the full coordinates for the key and return the keyentry.
//
// Assumptions: we will not get a down event outside of our window, and every
// point in the window maps to a key.
//
__inline static KEYENTRY* WINAPI IM_GetKeyFromCoord( RECT *prcKey )
{
    int nRow = 0;
	int nKey = 0;

    //
    // Row
    //

    while( prcKey->top > g_nRowCoord[nRow + 1] ) 
	{
        nRow++;
    }


    //
    // Key
    //

    while( prcKey->left > g_keys[nRow][nKey + 1].nLeft ) 
	{
        nKey++;
    }


    //
    // Set up rect and return the key.
    //

    prcKey->left   = g_keys[nRow][nKey].nLeft;
    prcKey->right  = g_keys[nRow][nKey + 1].nLeft;
    prcKey->top    = g_nRowCoord[nRow];
    prcKey->bottom = g_nRowCoord[nRow + 1];
        
    return g_keys[nRow] + nKey;
}


//
// Press or unpress a key.
// fPress is TRUE to press the key, FALSE to unpress it.
//
static BOOL WINAPI IM_PressKey( BOOL fPress, KEYENTRY *pKey, RECT *prcKey )
{
    HDC				hdc;
    RECT			rc;
    int				nRow;
    UINT*			pnChar;
    HRESULT			hRes;
    DWORD			dwVkFlags;
	DWORD			dwRop;
    KEY_STATE_FLAGS nShiftState;


    rc = *prcKey;


    //
    // Set up values specific to press/unpress.
    //

    if( fPress ) 
	{
        pKey->fdwFlags |= F_DOWN;
        dwVkFlags = KEYEVENTF_SILENT;
        nShiftState = KeyStateDownFlag;
        dwRop = DSTINVERT;
        InflateRect( &rc, -1, -1 );
    }
	else
	{
        pKey->fdwFlags &= ~F_DOWN;
        dwVkFlags = KEYEVENTF_KEYUP | KEYEVENTF_SILENT;
        nShiftState = KeyStatePrevDownFlag | KeyShiftNoCharacterFlag;
        dwRop = SRCCOPY;
    }


    //
    // Adjust down count (an optimization for redraw and searching).
    //

    g_nDown += fPress ? 1 : -1;


    //
    // Draw key in new state.
    //

    if( pKey->fdwFlags & F_REDRAW ) 
	{
        IM_SwitchBitmap();
    }
	else 
	{
        hdc = GetDC( g_hwndMain );
        BitBlt(
            hdc,
            rc.left,
            rc.top,
            rc.right - rc.left + 1,
            rc.bottom - rc.top + 1,
            g_hdcKeybd,
            rc.left,
            rc.top + (SHIFTED() ? BITMAP_HEIGHT : 0),
            dwRop );
        ReleaseDC( g_hwndMain, hdc );
    }


    //
    // Send the appropriate key message.
    //

    if( pKey->fdwFlags & F_VK || WINED() ) 
	{

        hRes = g_pIMCallback->SendVirtualKey( pKey->bVk, dwVkFlags );

    }
	else 
	{
        if( ALTED() ) 
		{
            nShiftState |= KeyShiftAnyAltFlag;
        }
        if( CONTROLLED() ) 
		{
            nShiftState |= KeyShiftAnyCtrlFlag;
            if( NO_CTL == *(pnChar = (UINT*)&pKey->nCtrl) ) 
			{
                nShiftState |= KeyShiftNoCharacterFlag;
            } 
        } 
		else if( SHIFTED() ) 
		{
            nShiftState |= KeyShiftAnyShiftFlag;
            pnChar = &pKey->wcShift;
        }
		else
		{
            pnChar = &pKey->wcUnshift;
        }

        hRes = g_pIMCallback->SendCharEvents(
                                pKey->bVk,
                                nShiftState,
                                1,
                                &nShiftState,
                                pnChar );
    }


    //
    // If the key unpressed is not sticky, unpress all currently stuck keys
    // except for caps lock.
    //

    if( !fPress && g_nDown && !(pKey->fdwFlags & F_STK) ) 
	{
        for( nRow = 0; nRow < NUM_ROWS; nRow++ ) 
		{
            pKey = g_keys[nRow];
            while( g_nDown && pKey->bVk != SENTINEL ) 
			{
                if( pKey->fdwFlags & F_STK &&
                    pKey->fdwFlags & F_DOWN &&
                    VK_CAPITAL != pKey->bVk )
                {
                    rc.left   = pKey->nLeft;
                    rc.right  = (pKey + 1)->nLeft;
                    rc.top    = g_nRowCoord[nRow];
                    rc.bottom = g_nRowCoord[nRow + 1];
                    IM_PressKey( FALSE, pKey, &rc );
                }
                pKey++;
            }
        }
    }


    return SUCCEEDED(hRes);
}


//
// Handle mouse events.
//
__inline static LRESULT WINAPI IM_OnMouseEvent( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    static RECT			rcKey;
    static KEYENTRY*	pKey	= NULL;


    switch( msg ) 
	{

    //
    // On a keydown, either press the button, or if it is sticky, toggle it.
    //
    case WM_LBUTTONDOWN:
        rcKey.left = LOWORD(lParam);
        rcKey.top = HIWORD(lParam);
        if( pKey = IM_GetKeyFromCoord( &rcKey ) ) 
		{
            if( pKey->fdwFlags & F_STK && pKey->fdwFlags & F_DOWN ) 
			{
                if( VK_CAPITAL == pKey->bVk ) 
				{
                    g_pIMCallback->SendVirtualKey(pKey->bVk, KEYEVENTF_SILENT);
                }
                IM_PressKey( FALSE, pKey, &rcKey );         
            } 
			else 
			{
                IM_PressKey( TRUE, pKey, &rcKey );
                if( VK_CAPITAL == pKey->bVk ) 
				{
                    g_pIMCallback->SendVirtualKey(
                                        pKey->bVk,
                                        KEYEVENTF_SILENT | KEYEVENTF_KEYUP );
                }
            }
            SetCapture( hwnd );
        }
        break;  

    //
    // on a keyup, unpress a non-sticky button.
    //
    case WM_LBUTTONUP:
        if( pKey && !(pKey->fdwFlags & F_STK) ) 
		{
            IM_PressKey( FALSE, pKey, &rcKey );
        }
        pKey = NULL;
        ReleaseCapture();
        break;

    } // switch( msg )

    return 0;
}


//
// Main window procedure.
//
LRESULT WINAPI ImWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;

    switch( msg )
    {

    case WM_PAINT:
		BeginPaint( hwnd, &ps );
        IM_DrawArea( ps.hdc, &ps.rcPaint );
        EndPaint( hwnd, &ps );
        return 0;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDBLCLK:
        return IM_OnMouseEvent( hwnd, msg, wParam, lParam );
            
    } // switch( message )

    return DefWindowProc( hwnd, msg, wParam, lParam );
}



//
// At startup, set our keystate for all of our keys to up.
// We rely on the SIP manager to lift all sticky/modifier keys, and to turn
// caps lock off, before selecting a new IM.
//
__inline static void WINAPI IM_SetDownKeys( void )
{
    KEYENTRY*	pKey;
    int			nRow;
    

    g_nDown = 0;

    for( nRow = 0; nRow < NUM_ROWS; nRow++ ) 
	{
        pKey = g_keys[nRow];
        while( pKey->bVk != SENTINEL ) 
		{
            pKey->fdwFlags &= ~F_DOWN;
            pKey++;
        }
    }

    return;
}



//
// IInputMethod implementation.
//


//
// Ctor, Dtor.
//

CInputMethod::CInputMethod( IUnknown *pUnkOuter, HINSTANCE hInstance )
{
    m_cRef = 0;
    g_dwObjectCount++;

    if( !pUnkOuter ) 
	{
        m_pUnkOuter = this;
    }
	else 
	{
        m_pUnkOuter = pUnkOuter;
    }

    return;
}

CInputMethod::~CInputMethod()
{
    g_dwObjectCount--;
    return;
}


//
// IInputMethod methods.
//

STDMETHODIMP CInputMethod::Select( HWND hwndSip )
{
    WNDCLASS wc;


    ZeroMemory( &wc, sizeof(wc) );
    wc.lpfnWndProc = ImWndProc;
    wc.hInstance = g_hInstDll;
    wc.hbrBackground = NULL;
    wc.lpszClassName = g_ptszClassName;

    if( !RegisterClass( &wc ) ) 
	{
        return E_FAIL;
    }

    IM_SetDownKeys();

    g_hbmKeybd = LoadBitmap( g_hInstDll, MAKEINTRESOURCE(IDB_KEYBD1) );

    g_hdcKeybd = CreateCompatibleDC( NULL );
    g_hbmOld   = (HBITMAP)SelectObject( g_hdcKeybd, g_hbmKeybd );


    g_hwndMain = CreateWindow(
                        g_ptszClassName,
                        TEXT(""),
                        WS_CHILD,
                        0,
                        0,
                        10,
                        10,
                        hwndSip,
                        (HMENU)NULL,
                        g_hInstDll,
                        NULL );

    ShowWindow( g_hwndMain, SW_SHOWNOACTIVATE );

    return NOERROR;
}


STDMETHODIMP CInputMethod::Deselect( void )
{
    ImageList_Destroy( g_hImagelistWide );
    ImageList_Destroy( g_hImagelistNarrow );
    g_hImagelistWide = g_hImagelistNarrow = NULL;
    SelectObject( g_hdcKeybd, g_hbmOld );
    DeleteObject( g_hbmKeybd );
    DeleteDC( g_hdcKeybd );
    DestroyWindow( g_hwndMain );
    UnregisterClass( g_ptszClassName, g_hInstDll );
    return NOERROR;
}
 

STDMETHODIMP CInputMethod::Showing( void )
{
    return NOERROR;
}


STDMETHODIMP CInputMethod::Hiding( void )
{
    return NOERROR;
}


STDMETHODIMP CInputMethod::GetInfo( IMINFO *pimi )
{
    HBITMAP hbm;


    if( !g_hImagelistWide ) 
	{

        g_hImagelistWide = ImageList_Create(
                            32,
                            16,
                            ILC_COLOR | ILC_MASK,
                            1,
                            1 );
        g_hImagelistNarrow = ImageList_Create(
                            16,
                            16,
                            ILC_COLOR | ILC_MASK,
                            1,
                            1 );

        if( hbm = LoadBitmap( g_hInstDll, MAKEINTRESOURCE(IDB_WIDE1) ) ) 
		{
            ImageList_AddMasked( g_hImagelistWide, hbm, RGB(192,192,192) );
            DeleteObject( hbm );
        }

        if( hbm = LoadBitmap( g_hInstDll, MAKEINTRESOURCE(IDB_NARROW1) ) ) 
		{
            ImageList_AddMasked( g_hImagelistNarrow, hbm, RGB(192,192,192) );
            DeleteObject( hbm );
        }
    }

    pimi->fdwFlags = SIPF_DOCKED;
    pimi->hImageNarrow = (HANDLE)g_hImagelistNarrow;
    pimi->hImageWide = (HANDLE)g_hImagelistWide;
    pimi->iNarrow = pimi->iWide = 0;
    pimi->rcSipRect.left = pimi->rcSipRect.top = 0;
    pimi->rcSipRect.right = BITMAP_WIDTH;
    pimi->rcSipRect.bottom = BITMAP_HEIGHT;

    return NOERROR;
}


STDMETHODIMP CInputMethod::ReceiveSipInfo( SIPINFO *psi )
{
    MoveWindow(
        g_hwndMain,
        0,
        0,
        psi->rcSipRect.right - psi->rcSipRect.left,
        psi->rcSipRect.bottom - psi->rcSipRect.top,
        FALSE );

    return NOERROR;
}


STDMETHODIMP CInputMethod::RegisterCallback( IIMCallback *pIMCallback )
{
    g_pIMCallback = pIMCallback;
    return NOERROR;
}


STDMETHODIMP CInputMethod::GetImData( DWORD dwSize, void *pvImData )
{
    return E_NOTIMPL;
}


STDMETHODIMP CInputMethod::SetImData( DWORD dwSize, void *pvImData )
{
    return E_NOTIMPL;
}


STDMETHODIMP CInputMethod::UserOptionsDlg( HWND hwndParent )
{
    MessageBox(hwndParent, _T("Options menu for Dvorak InputMethod"), 0, MB_SETFOREGROUND);
    return S_OK;
}


//
// IUnknown methods.
//

STDMETHODIMP CInputMethod::QueryInterface( REFIID riid, LPVOID FAR* ppobj )
{
    if( IID_IUnknown == riid || IID_IInputMethod == riid ) 
	{
        *ppobj = this;
        AddRef();
        return NOERROR;
    } 
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CInputMethod::AddRef( void )
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CInputMethod::Release( void )
{
    if( --m_cRef ) 
	{
        return m_cRef;
    }
    delete this;
    return 0;
}

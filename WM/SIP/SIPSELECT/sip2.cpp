//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++

	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
	ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
	THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
	PARTICULAR PURPOSE.


	Module Name:  

	sip2.cpp

--*/

#include <windows.h>
#include <commctrl.h>
#include <windef.h>

#include <sipapi.h>
#include "sipdefs.h"

#include "resource.h"

extern HINSTANCE ghInst;
extern BOOL SipShowHide(BOOL show);
extern BOOL g_bSipIMShow;

//
// globals
//

IMENUMINFO **g_ppimei = NULL;
int g_nIms = 0, g_nCurIm;

//
// Im list defines and variables.
//


void SIP_FreeImEnums( void );

//
// Sort the IMs alphabetically.
//
__inline static
BOOL WINAPI
SIP_SortImArray( IMENUMINFO **ppimei, int nIms )
{
    //
    // If there aren't any IMs, we're done.
    //
    if( !nIms ) {
        return TRUE;
    }

    //
    // Now sort it.
    //

#define COMPARE_LESS    1
#define SWAP(a,b) { IMENUMINFO *t = a; a = b; b = t; }

    int i;

    for( i = 0; i < nIms/2; i++ ) {
        SWAP( g_ppimei[i], g_ppimei[nIms-i-1] );
    }

#if 0
    int i, j, low;

    for( i = 0; i < nIms - 1; i++ ) {
        low = i;
        for( j = i + 1; j < nIms; j++ ) {
            if( COMPARE_LESS == CompareString(
                                    LOCALE_USER_DEFAULT,
                                    NORM_IGNORECASE,
                                    ppimei[j]->szName,
                                    -1,
                                    ppimei[low]->szName,
                                    -1 ) ) {
                low = j;
            }
        }
        SWAP( g_ppimei[low], g_ppimei[i] );
    }
#endif

    return TRUE;
}


//
// Called by the OS for each installed IM, in response to SipEnumIm.
//
int ImEnumProc( IMENUMINFO *pimei )
{
    if( g_nCurIm >= g_nIms ) {
        return FALSE;
    }

    if( !(g_ppimei[g_nCurIm] = (IMENUMINFO *)LocalAlloc(LMEM_FIXED,
                                                sizeof(IMENUMINFO) )) ) {
        return FALSE;
    }

    *g_ppimei[g_nCurIm++] = *pimei;

    return TRUE;
}


//
// Free IMENUMINFO array
//
void SIP_FreeImEnums( void )
{
    int i;

    for( i = 0; i < g_nCurIm; i++ ) 
    {
        LocalFree( g_ppimei[i] );
    }
    LocalFree( g_ppimei );
    g_ppimei = NULL;

    return;
}


//
// Show a popup menu allowing selection of the current SIP.
//
BOOL WINAPI
SIP_IMListPopup( HWND hwnd, UINT uFlags, int nX, int nY, RECT *prc )
{
    int i, nCheck = -1;
    CLSID clsidImCur;
    HMENU hmenu;
	BOOL ret = FALSE;
    DWORD dwFlags;
    
    //
    // If g_ppimei is not NULL, free the allocated memory.
    //
    if ( g_ppimei )
    {
        SIP_FreeImEnums();
    }

    //
    // Get number of IMs
    //
    g_nIms = 0;
    g_nCurIm = 0;
    g_nIms = SipEnumIM( NULL );


    //
    // Allocate pointer array
    //
    g_ppimei = (IMENUMINFO **)LocalAlloc(LMEM_FIXED,  
                                        g_nIms * sizeof(IMENUMINFO *));  
    if( !g_ppimei ) {
        return FALSE;
    }


    //
    // Enumerate
    //
    g_nCurIm = 0;
    SipEnumIM( ImEnumProc );
    g_nIms = g_nCurIm;


    //
    // Sort it 
    //
    SIP_SortImArray( g_ppimei, g_nIms );
    

    //
    // Find out which IM is currently selected
    //
	SipGetCurrentIM(&clsidImCur);

    //
    // Popup
    //
    if( hmenu = CreatePopupMenu() ) {

        //
        // Build up the menu.
        //
        for( i = 0; i < g_nIms; i++ ) {

            if ( clsidImCur == g_ppimei[i]->clsid ) {
                nCheck = i;
            }

            dwFlags = MF_STRING;

            AppendMenu(
                hmenu,
                dwFlags,
                i + 1,
                g_ppimei[i]->szName );
        }

        //
        // Add close menu.
        //
        AppendMenu(
            hmenu,
            MFT_SEPARATOR,
            0,
            NULL );

		TCHAR	closetxt[64];
        LoadString(ghInst, IDS_CLOSE_SIP, closetxt, sizeof(closetxt)/sizeof(closetxt[0]));
		dwFlags = MF_STRING;
		if(!g_bSipIMShow){
			dwFlags |= MF_GRAYED;
		}
        AppendMenu(
            hmenu,
            dwFlags,
            i + 1,
            closetxt );

        if( -1 != nCheck ) {
			CheckMenuItem(hmenu, nCheck, MF_CHECKED | MF_BYPOSITION);
        }


        //
        // Display popup menu.
        //
        i = TrackPopupMenu(
                    hmenu,
                    uFlags | TPM_RETURNCMD | TPM_NONOTIFY,
                    nX,
                    nY,
                    0,
                    hwnd,
                    prc );
        DestroyMenu( hmenu );


        //
        // Check for valid selection and select IM if so.
        //
		if(i > 0 && i <= g_nIms){
			ret = TRUE;
            if( clsidImCur != g_ppimei[i-1]->clsid ) {
		        SipSetCurrentIM(&g_ppimei[i-1]->clsid);
            }
	    } else if(i == g_nIms + 1){
			SipShowHide(FALSE);
		}
	}

	SIP_FreeImEnums();

    return(ret);
}

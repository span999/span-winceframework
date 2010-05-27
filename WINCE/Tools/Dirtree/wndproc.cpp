// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1997  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:     wndproc.cpp
//
//  PURPOSE:    Contains window procedures for the main application windows.
//
//  PLATFORMS:  Windows CE
//
//  FUNCTIONS:
//      MainWndProc()       - Message handler for the main application window
//      Main_OnCommand()    - Processes WM_COMMAND messages
//      Main_OnDestroy()    - Handles the WM_DESTROY message
//      AboutDlgProc()      - Message handler for the application's about
//                            dialog.
//
//  COMMENTS:
//
//

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <aygshell.h>
#include "resource.h"
#include "globals.h"

//----------------------------------------------------------------------------
// Local prototypes

void Main_OnCommand(HWND, int, HWND, UINT);
void Main_OnDestroy(HWND);
BOOL Main_OnCreate(HWND, LPCREATESTRUCT);
LRESULT CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT Main_OnNotify(HWND, int, LPNMHDR);
void Main_OnSize(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam );


//----------------------------------------------------------------------------
// Global variables

HWND g_hwndTreeView = 0;
HWND hwndCB;



//
//  FUNCTION:   MainWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:    Processes messages for the main application window.
//
//  PARAMETERS:
//      hwnd        - handle the the window receiving the message
//      uMsg        - identifier of the message
//      wParam      - additional info associated with the message
//      lParam      - additional info associated with the message
//
//  RETURN VALUE:
//      (LRESULT) Returns 0 by default if the message is handled by this
//                procedure.
//
//  COMMENTS:
//

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam,
														 LPARAM lParam)
{
	switch(uMsg)
	{
	//Handle the SIP being put up
	// This is done in the message loop as there are no message crackers for WM_SETTINGCHANGE
	// However you can use WM_WININICHANGE crackers if necessary
	case WM_SETTINGCHANGE:

		SHACTIVATEINFO sai;
		memset(&sai, 0, sizeof(SHACTIVATEINFO));

		//This will force a HWND resize depending on the SIP condition	
		SHHandleWMSettingChange(hwnd, -1, 0, &sai);

		break;
		HANDLE_MSG(hwnd, WM_COMMAND, 		Main_OnCommand);
		HANDLE_MSG(hwnd, WM_DESTROY, 		Main_OnDestroy);
		HANDLE_MSG(hwnd, WM_CREATE,  		Main_OnCreate );
		HANDLE_MSG(hwnd, WM_NOTIFY,  		Main_OnNotify );
		HANDLE_MSG(hwnd, WM_SIZE,  		Main_OnSize );

	default:                // Pass message on for default proccessing
		return DefWindowProc( hwnd, uMsg, wParam, lParam );
	}
	
	// If we performed non-default processing on the message, return FALSE
	return FALSE;
}


//
//  FUNCTION:   Main_OnSize(HWND)
//
void Main_OnSize(HWND hwnd, UINT uMsg, WPARAM wParam,
														 LPARAM lParam)
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	MoveWindow(g_hwndTreeView, rc.left, rc.top, rc.right, rc.bottom, TRUE);
}

//
//  FUNCTION:   Main_OnCommand(HWND, int, HWND, UINT)
//
//  PURPOSE:    Handles the WM_COMMAND messages for the Win32Generic window
//              class
//
//  PARAMETERS:
//      hwnd        - handle of the window receiving the message
//      id          - identifier of the command
//      hwndCtl     - handle of the control sending the message)
//      codeNotify  - specifies the notification code if the message is from
//                    a control
//
//  RETURN VALUE:
//      none
//
//  COMMENTS:
//      codeNotify is 1 if from an accelerator, 0 if from a menu.
//      If the message is not from a control hwndCtl is NULL.
//

void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{

	case ID_HELP_ABOUT:
		// Display the "About" dialog
		DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_ABOUT), hwnd, (DLGPROC)AboutDlgProc);
		break;
	}
}


//
//  FUNCTION:   Main_OnDestroy(HWND)
//
//  PURPOSE:    Handle any clean up and post the quit message to exit the
//              message loop.
//
//  PARAMETERS:
//      hwnd    - handle of the window receiving this message
//
//  RETURN VALUE:
//      none
//

void Main_OnDestroy(HWND hwnd)
{
	// Indicate that the message loop should exit since the main window
	// is being destroyed.
	
	PostQuitMessage(0);
}


//
//  FUNCTION:   Main_OnCreate(HWND, LPCREATESTRUCT)
//
//  PURPOSE:    Create the treeview control and initialize the data
//
//  PARAMETERS:
//      hwnd    		- handle of the window receiving this message
//		lpCreateStruct	- points to a CREATESTRUCT containing information
//						  about the window creation
//
//  RETURN VALUE:
//      returns TRUE if the window should be created, FALSE otherwise
//
//  COMMENTS:
//

BOOL Main_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	HWND hwndTV;
	RECT rcClient;
	SHMENUBARINFO mbi;
	// Start by initializing the common control libraries
	InitCommonControls();
	// Get the client area rect to put the treeview in
	GetClientRect(hwnd, &rcClient);
	
	//Create a MenuBar
	memset(&mbi, 0, sizeof(SHMENUBARINFO));
	mbi.cbSize     = sizeof(SHMENUBARINFO);
	mbi.hwndParent = hwnd;
	mbi.nToolBarId = IDM_MAIN_MENU;
	mbi.hInstRes   = g_hInstance;
	mbi.nBmpId     = 0;
	mbi.cBmpImages = 0;	
	
	SHCreateMenuBar(&mbi);
	if (!SHCreateMenuBar(&mbi))
		MessageBox(hwnd, L"SHCreateMenuBar Failed", L"Error", MB_OK);
	hwndCB = mbi.hwndMB;

	
	// We need to create the treeview control
	hwndTV = CreateWindowEx(0, WC_TREEVIEW, TEXT("Tree View Control"),
		WS_VISIBLE  | TVS_HASLINES |                   //| WS_CHILD
		TVS_LINESATROOT | TVS_HASBUTTONS, rcClient.top, rcClient.left,
		rcClient.right, rcClient.bottom, hwnd,
		(HMENU) IDC_TREEVIEW, g_hInstance, NULL);
	
	
	// Make sure the treeview was actually created
	if (!hwndTV)
	{
		ErrorHandler();
		return FALSE;
	}
	
	
	// Initialize the image list, and add items to the control.
	// InitTreeViewImageLists() and InitTreeViewItems() are defined in treeview.c.
	if (!InitTreeViewImageLists(hwndTV) || !InitTreeViewItems(hwndTV))
	{
		DestroyWindow(hwndTV);
		return FALSE;
	}
	
	// Everything is set up correctly, now set the global handle to the treeview
	// and finish window creation.
	g_hwndTreeView = hwndTV;
	
	return TRUE;
}


//
//  FUNCTION:   AboutDlgProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:    Processes messages for the About dialog box
//
//  PARAMETERS:
//      hwnd    - window handle of the dialog box
//      uMsg    - identifier of the message being handled
//      wParam  - additional info associated with the message
//      lParam  - additional info associated with the message
//
//  RETURN VALUE:
//      Except in response to the WM_INITDIALOG message, the dialog box
//      procedure should return nonzero if it processes the message, and zero
//      if it does not.
//
//  COMMENTS:
//

LRESULT CALLBACK AboutDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam,
															LPARAM lParam)
{
	SHINITDLGINFO shidi;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		//On Pocket PC devices you normally create all Dialog's as fullscreen dialog's
		// with an OK button in the upper corner. 
		// Create a Done button and size it.
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = hwnd;
		//initialzes the dialog based on the dwFlags parameter
		SHInitDialog(&shidi);
		// Should return nonzero to set focus to the first control in the
		// dialog, or zero if this routine sets the focus manually.
		return (TRUE);
		
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwnd, 0);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		return (TRUE);
	}
	return (FALSE);
}


//
//  FUNCTION:   Main_OnNotify(HWND, int, LPNMHDR)
//
//  PURPOSE:    Handles any notifications the treeview control spits out.
//
//  PARAMETERS:
//      hwnd    - handle of the window receiving the notification
//      idCtl   - identifies the control sending the notification
//      pnmh    - points to a NMHDR struct with more inforamation regarding the
//                notification
//
//  RETURN VALUE:
//      (LRESULT) Dependant on the specific notification.  See Below.
//
//  COMMENTS:
//

LRESULT Main_OnNotify(HWND hwnd, int idCtl, LPNMHDR pnmh)
{
	HTREEITEM hti, htiNext;
	TCHAR	  szDir[MAX_PATH];
	
	switch (pnmh->code)
	{	
		// A directory node has been collapsed.  Now we can remove the child items
		// from the node.
	case TVN_ITEMEXPANDED:
		{
			LPNM_TREEVIEW pnmtv = (LPNM_TREEVIEW) pnmh;
			
			if (pnmtv->action == TVE_COLLAPSE)
			{
				OutputDebugString(TEXT("Item collapsed\r\n"));
				
				// Now actually remove the child items within this directory
				hti = TreeView_GetChild(pnmh->hwndFrom, pnmtv->itemNew.hItem);
				while (hti)
				{
					htiNext = TreeView_GetNextSibling(pnmh->hwndFrom, hti);
					TreeView_DeleteItem(pnmh->hwndFrom, hti);
					hti = htiNext;
				}
			}
			
			return (FALSE);
		}
		
		// A node is expanding or collapsing.  We need to update the folder
		// images to reflect either a closed or open folder depending on it's
		// new state.
	case TVN_ITEMEXPANDING:
		{
			// cast the NMHDR into a treeview notify structure
			LPNM_TREEVIEW pnmtv = (LPNM_TREEVIEW) pnmh;
			
			if (pnmtv->action == TVE_COLLAPSE)
			{
				OutputDebugString(TEXT("Item collapsing\r\n"));
				
				// Retrieve the image from the current item
				pnmtv->itemNew.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
				TreeView_GetItem(pnmh->hwndFrom, &(pnmtv->itemNew));
				
				// Set the item's image to the closed folder
				pnmtv->itemNew.iImage = IMAGE_CLOSED;
				pnmtv->itemNew.iSelectedImage = IMAGE_CLOSED;
				
				TreeView_SetItem(pnmh->hwndFrom, &(pnmtv->itemNew));
			}
			else
			{
				OutputDebugString(TEXT("Item expanding\r\n"));
				
				// Retrieve the image from the current item
				pnmtv->itemNew.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
				TreeView_GetItem(pnmh->hwndFrom, &(pnmtv->itemNew));
				
				// Set the item's image to the closed folder
				pnmtv->itemNew.iImage = IMAGE_OPEN;
				pnmtv->itemNew.iSelectedImage = IMAGE_OPEN;
				TreeView_SetItem(pnmh->hwndFrom, &(pnmtv->itemNew));
				
				// We need to fill in the subdirectory just expanded
				BuildDirectory(pnmtv->hdr.hwndFrom, pnmtv->itemNew, szDir);
				
				GetDirectoryContents(pnmtv->hdr.hwndFrom,
					szDir,
					(HTREEITEM)pnmtv->itemNew.hItem);
			}
			
			return (FALSE);	// return value is ignored
		}

		//Run the application or show a commandprompt if option is set
	case NM_DBLCLK:
		{
			DWORD 			dwPos;
			TV_HITTESTINFO 	tvhti;
			POINT 			point;
			HWND			hwndTV = GetDlgItem(hwnd, IDC_TREEVIEW);
			TV_ITEM         tvi;
			TCHAR szbufW[MAX_PATH];
			TCHAR sztempW[MAX_PATH];
			szbufW[0] = '\0';
			sztempW[0] = '\0';
			
			// Find out where the cursor was
			dwPos = GetMessagePos();
			point.x = LOWORD(dwPos);
			point.y = HIWORD(dwPos);
			ScreenToClient(hwndTV, &point);
			tvhti.pt = point;
			tvi.hItem = TreeView_HitTest(hwndTV, &tvhti);
			tvi.pszText = szbufW;
			tvi.cchTextMax = MAX_PATH;
			tvi.mask = TVIF_IMAGE | TVIF_HANDLE | TVIF_TEXT;
			TreeView_GetItem(hwndTV, &tvi);
			
			if (tvi.iImage == IMAGE_EXE){
				HTREEITEM htv;
				htv = tvi.hItem;
				_tcsrev(szbufW);
				if (_tcsncicmp(szbufW, _T("exe."),4) != 0) return (0L);
				_tcscat(szbufW, _T("\\"));
				
				while (htv = TreeView_GetParent(hwndTV, htv)){
					//Construct path 
					tvi.hItem = htv;
					tvi.pszText = sztempW;
					tvi.cchTextMax = MAX_PATH;
					tvi.mask =  TVIF_TEXT;
					TreeView_GetItem(hwndTV, &tvi);
					_tcsrev(sztempW);
					if (_tcscmp(sztempW,  _T("\\")) != 0) _tcscat(szbufW, sztempW);
					if (szbufW[lstrlen(szbufW)-1] != '\\') _tcscat(szbufW, _T("\\"));
				};
				_tcsrev(szbufW);
				CreateProcess(szbufW, NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
			}
			return (0L);    // Return value is ignored
		}
}
return (0L);
}

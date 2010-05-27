// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1997  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:     globals.h
//
//  PURPOSE:    contains variables and prototypes global to the application
//
//  PLATFORMS:  Windows CE
//
//  COMMENTS:
//
//

#ifndef __GLOBALS_H__
#define __GLOBALS_H__
/// include stdafx.h ///
/**********************************************************************

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

This is "Sample Code" and is distributable subject to the terms of the end user license agreement.

**********************************************************************/
/////////////////////////// Pocket PC only samples ///////////////////////
#if _WIN32_WCE < 212//non-Pocket PC devices
#error 'This sample works on Pocket PC devices only'
#endif
#include <aygshell.h>

//Menu Bar Height
#define MENU_HEIGHT 26

////////////////////////// SHGetMenu Macro's
#if _WIN32_WCE > 211 //Pocket PC devices

//#define SHCMBM_SETSUBMENU   (WM_USER + 400)
//#define SHCMBM_GETSUBMENU   (WM_USER + 401) // lParam == ID
//#define SHCMBM_GETMENU      (WM_USER + 402) // get the owning hmenu (as specified in the load resource)

#define SHGetMenu(hWndMB)  (HMENU)SendMessage((hWndMB), SHCMBM_GETMENU, (WPARAM)0, (LPARAM)0);
#define SHGetSubMenu(hWndMB,ID_MENU) (HMENU)SendMessage((hWndMB), SHCMBM_GETSUBMENU, (WPARAM)0, (LPARAM)ID_MENU);
#define SHSetSubMenu(hWndMB,ID_MENU) (HMENU)SendMessage((hWndMB), SHCMBM_SETSUBMENU, (WPARAM)0, (LPARAM)ID_MENU);

//Alternate definition
#define SHMenuBar_GetMenu(hWndMB,ID_MENU) (HMENU)SendMessage((hWndMB), SHCMBM_GETSUBMENU, (WPARAM)0, (LPARAM)ID_MENU);

#else //non-Pocket PC devices

#define SHGetMenu(hWndCB) (HMENU)CommandBar_GetMenu(hWndCB, 0)
#define SHGetSubMenu(hWndCB,ID_MENU) (HMENU)GetSubMenu((HMENU)CommandBar_GetMenu(hWndCB, 0), ID_MENU)
#define SHSetSubMenu(hWndMB,ID_MENU)

#endif
/// end include stdafx.h ///

//----------------------------------------------------------------------------
// Constants and Enumerations
typedef enum tagVERSION {WINNT, WIN32S, WIN95} VERSION;

typedef enum tagIMAGES { IMAGE_OPEN, IMAGE_CLOSED,
						IMAGE_EXE, IMAGE_TREE, IMAGE_DOCUMENT} IMAGES;

//----------------------------------------------------------------------------
// Global Variables

extern HINSTANCE g_hInstance;                                   // initproc.cpp
extern VERSION g_version;                                         // main.cpp
extern HWND hwndCB;
extern HIMAGELIST himl;			// handle of the image list

//----------------------------------------------------------------------------
// Prototypes

void ErrorHandlerEx(WORD, LPTSTR);                               // main.cpp
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);       // wndproc.cpp
BOOL InitApplication(HINSTANCE);                                // initproc.cpp
BOOL InitInstance(HINSTANCE, int);                              // initproc.cpp
BOOL InitTreeViewImageLists(HWND);
BOOL InitTreeViewItems(HWND);
HTREEITEM AddItemToTree(HWND, LPTSTR, HTREEITEM, BOOL);
BOOL GetDirectoryContents(HWND hwndTV, LPTSTR pszDirectory,
                          HTREEITEM htiParent);
BOOL BuildDirectory(HWND, TV_ITEM, LPTSTR);
BOOL GetDrives(HWND);


//----------------------------------------------------------------------------
// Macros

#define IDC_TREEVIEW	101
#define CX_BITMAP		 16
#define CY_BITMAP		 15
#define NUM_BITMAPS		  3

#define TVIS_GCNOCHECK   1        // One-based index to state image list
#define TVIS_GCCHECK     2
#define TVIS_GCSOMECHECK 3

#define ErrorHandler() ErrorHandlerEx(__LINE__, TEXT(__FILE__))






#endif

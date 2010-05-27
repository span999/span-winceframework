// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1997  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:		treeview.cpp
//
//  PURPOSE:    Contains functions that maintain the treeview control
//
//  PLATFORMS: 	Windows CE
//
//  FUNCTIONS:
//		InitTreeViewImageLists()
//		InitTreeViewItems()
//
//  COMMENTS:
//
//

#include <windows.h>
#include <memory.h>
#include <commctrl.h>
#include "resource.h"
#include "globals.h"

//Globals
HIMAGELIST himl;			// handle of the image list

HTREEITEM AddItemToTree(HWND, LPTSTR, HTREEITEM, BOOL);


//
//  FUNCTION:   InitTreeViewImageLists(HWND)
//
//  PURPOSE:    Here we load the bitmaps and create image lists for the item
//              images and the state images.
//
//  PARAMETERS:
//      hwndTV  - Handle of the treeview that these image lists are being added
//                to.
//
//  RETURN VALUE:
//      (BOOL) Returns TRUE if the image lists are added successfully, FALSE
//             otherwise.
//
//  COMMENTS:
//

BOOL InitTreeViewImageLists(HWND hwndTV)
{
	HBITMAP	   hbmp;			// handle of the bitmaps to add

	// Create the image list for the item pictures
	if ((himl = ImageList_Create(CX_BITMAP, CY_BITMAP, ILC_MASK, NUM_BITMAPS, 0))
			  == NULL)
		return FALSE;

	// Add the bitmaps to the list
	hbmp = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_IMAGES));

	if (-1 == ImageList_AddMasked(himl, hbmp, RGB(0, 255, 0)))
	{
		ErrorHandler();
		return FALSE;
	}

    // Clean up the GDI objects
	DeleteObject(hbmp);

	// Fail if not all the images were added
	if (ImageList_GetImageCount(himl) < NUM_BITMAPS)
		return FALSE;

	// Associate the image list with the treeview control
	TreeView_SetImageList(hwndTV, himl, TVSIL_NORMAL);
	
	return TRUE;
}


//
//  FUNCTION:   InitTreeViewItems(HWND)
//
//  PURPOSE:    Here is where items are initially added to the tree.
//
//  PARAMETERS:
//      hwndTV  - Handle of the treeview to add images to.
//
//  RETURN VALUE:
//      (BOOL) Returns TRUE if the items were added successfully, FALSE
//             otherwise.
//
//  COMMENTS:
//      For now all we do is add the current drives attached to the system.
//
//

BOOL InitTreeViewItems(HWND hwndTV)
{
	// GetDrives calls GetLogicalDriveStrings() which is not supported
	// Win32s.  So, if this is not NT/95 just add the C drive.
	if (WIN32S != g_version )
	{
		GetDrives(hwndTV);
	}
	else
	{
		AddItemToTree(hwndTV, TEXT("\\"), NULL, TRUE);
	}
	
	return TRUE;	
}


//
//  FUNCTION:   AddItemToTree(HWND, LPSTR, int, int)
//
//  PURPOSE:    Here is where the item is actually inserted into the tree.  The
//              position on the tree is also determined.
//
//  PARAMETERS:
//      hwndTV     - handle of the treeview to add the item to
//      lpszItem   - string to add to the tree
//		htiParent  - handle of the tree item that will be this item's parent
//		fDirectory - TRUE if this item is a directory
//
//  RETURN VALUE:
//      (HTREEITEM) Returns the handle of the item once it's been added to the
//                  tree. Otherwise it returns NULL.
//
//  COMMENTS:
//

HTREEITEM AddItemToTree(HWND hwndTV, LPTSTR lpszItem, HTREEITEM htiParent,
						BOOL fDirectory)
{
	TV_ITEM          tvi;
	TV_INSERTSTRUCT  tvins;
	HTREEITEM        hti;

	// Filter out the "." and ".." directories.
	if (!lstrcmpi(lpszItem, TEXT(".")) || !lstrcmpi(lpszItem, TEXT("..")))
		return (HTREEITEM)TRUE;

    // Start by initializing the structures
	memset(&tvi, 0, sizeof(TV_ITEM));
	memset(&tvins,0, sizeof(TV_INSERTSTRUCT));
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	// If it's a directory, add a child count so the expand button shows
	if (fDirectory)
	{
		tvi.mask |= TVIF_CHILDREN;
		tvi.cChildren = 1;
	}

	// Set the text of the item
	tvi.pszText = lpszItem;
	tvi.cchTextMax = lstrlen(lpszItem);

	// Give the item the appropriate image
	if (fDirectory){
		tvi.iSelectedImage = tvi.iImage = IMAGE_CLOSED;
	} else {
		_tcsrev(lpszItem);
		if (_tcsncicmp(lpszItem, _T("exe."),4))
			tvi.iSelectedImage = tvi.iImage =  IMAGE_DOCUMENT;
		else
			tvi.iSelectedImage = tvi.iImage =  IMAGE_EXE;
		_tcsrev(lpszItem);
	}

	tvins.item = tvi;
	tvins.hInsertAfter = TVI_SORT;

	// Set the parent item based on the specified level
	if (!htiParent)
		tvins.hParent = TVI_ROOT;
	else
		tvins.hParent = htiParent;

	// Add the item to the tree view control
	hti = (HTREEITEM) SendMessage(hwndTV, TVM_INSERTITEM, 0,
								  (LPARAM)(LPTV_INSERTSTRUCT) &tvins);

    // Return the handle to the item
	return hti;
}


//
//  FUNCTION:   BuildDirectory(HWND, TV_ITEM, LPTSTR)
//
//  PURPOSE:    Takes an item in the treeview and builds the path to the item
//
//  PARAMETERS:
//      hwndTV  - handle of the treeview control
//      tvi     - item to build the path for
//      lpszDir - string to place the path into
//
//  RETURN VALUE:
//      Returns TRUE if the path is built successfully, FALSE otherwise.
//
//  COMMENTS:
//

BOOL BuildDirectory(HWND hwndTV, TV_ITEM tvi, LPTSTR lpszDir)
{
    HTREEITEM hti;
	LPTSTR sz0, sz1;

	// Allocate some memory for the temp strings
	sz0 = (LPTSTR) LocalAlloc(LMEM_FIXED|LMEM_ZEROINIT,sizeof(TCHAR) * MAX_PATH);
	sz1 = (LPTSTR) LocalAlloc(LMEM_FIXED|LMEM_ZEROINIT,sizeof(TCHAR) * MAX_PATH);
    // Get the text for the first item
    tvi.mask |= TVIF_TEXT;
    tvi.pszText = sz0;
    tvi.cchTextMax =  MAX_PATH;

    if (!TreeView_GetItem(hwndTV, &tvi))
        return (FALSE);

    // Create the initial string
    wsprintf(sz1, TEXT("%s"), tvi.pszText);
	lstrcpy(lpszDir, sz1);

	hti = tvi.hItem;

    // Now add the parent directories if any
    while (hti = TreeView_GetParent(hwndTV, hti))
    {
        tvi.mask = TVIF_TEXT;
        tvi.hItem = hti;
        if (!TreeView_GetItem(hwndTV, &tvi))
            return (FALSE);

		lstrcpy(sz1, lpszDir);
		if (wcscmp(tvi.pszText,TEXT("\\")) == 0) //we are at the root.
			wsprintf(lpszDir, TEXT("%s%s"), tvi.pszText, sz1);
		else
			wsprintf(lpszDir, TEXT("%s\\%s"), tvi.pszText, sz1);
    }

	// Add the wildcard needed for FindFirstFile()
	lstrcpy(sz1, lpszDir);
	if (wcscmp(sz1,TEXT("\\")) == 0) //we are at the root.
		wsprintf(lpszDir, TEXT("%s*.*"), sz1);
	else
		wsprintf(lpszDir, TEXT("%s\\*.*"), sz1);

	// Free the strings now that we're done
	LocalFree(sz0);
	LocalFree(sz1);

    return (TRUE);
}

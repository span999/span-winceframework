// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1997  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:		treeview.cpp
//
//  PURPOSE:    Contains functions that perform the directory traversal
//
//  PLATFORMS: 	Windows CE
//
//  FUNCTIONS:
//
//  COMMENTS:
//
//

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "globals.h"

//
//  FUNCTION:   GetDirectoryContents(HWND, LPTSTR, HTREEITEM)
//
//  PURPOSE:    Enumerates the contents of the specified directory and adds
//              them to the TreeView control
//
//  PARAMETERS:
//      hwndTV          - TreeView to add the contents to
//      pszDirectory    - Path of the directory to list the contents for
//      htiParent       - TreeView item to add the contents as children of
//
//  RETURN VALUE:
//      (BOOL) Returns TRUE if the items are added sucessfully,
//             FALSE otherwise.
//
//  COMMENTS:
//

BOOL GetDirectoryContents(HWND hwndTV, LPTSTR pszDirectory,
                          HTREEITEM htiParent)
{
    WIN32_FIND_DATA findData;
    HANDLE fileHandle;
    BOOL fInserted = FALSE;
    DWORD       dwError;

	__try
	{
		// Get the first file in the directory
		fileHandle = FindFirstFile(pszDirectory, &findData);
		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			if (!AddItemToTree(hwndTV, findData.cFileName, htiParent,findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				__leave;

			// Loop on all remaining entries in the directory
			while (FindNextFile(fileHandle, &findData))
			{
				if (!AddItemToTree(hwndTV, findData.cFileName, htiParent,
					findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				__leave;
			}
		} else {
			if ( ERROR_NO_MORE_FILES != (dwError=GetLastError()))
			{
				OutputDebugString(TEXT("FindFirstFile returned invalid handle"));
				ErrorHandler();
			} else {
				TV_ITEM         tvi;

				tvi.hItem = htiParent;
				tvi.mask = TVIF_CHILDREN;
				tvi.cChildren = 0;

				if( !TreeView_SetItem(hwndTV, &tvi) )
					OutputDebugString(TEXT("ERROR: setting item for no children.\r\n"));
			}
		}

		// All done, everything worked.
		fInserted = TRUE;
	}
	__finally
	{
		if (fileHandle != INVALID_HANDLE_VALUE )
		FindClose(fileHandle);
	}

    return (fInserted);
}


//
//  FUNCTION:   GetDrives(HWND)
//
//  PURPOSE:    This function will retrieve a list of all the drives connected
//				the system and adds them to the tree.
//
//  PARAMETERS:
//      hwndTV  - Handle of the treeview that these image lists are being added
//                to.
//
//  RETURN VALUE:
//      (BOOL) Returns TRUE if the drives were added successfully,
// 			   FALSE otherwise
//
//  COMMENTS:
//

BOOL GetDrives(HWND hwndTV)
{
	LPTSTR lpszDrives, lpsz;
	DWORD  cch = 12; //128 * sizeof(TCHAR);

	// Allocate some memory for the drive string.  If there isn't enough space
	// then the call to GetLogicalDriveStrings() will tell us how much we need
	// and we can realloc as necessary
	lpszDrives = (LPTSTR) LocalAlloc(LMEM_FIXED|LMEM_ZEROINIT, cch * sizeof(TCHAR));
	if (!lpszDrives)
	{
		ErrorHandler();
		return (FALSE);
	}
	_tcscpy(lpszDrives, TEXT("\\\\"));  // specify the root
										// remember there is no concept of a drive

	// Now we have the string, let's save the pointer to free later and
	// add the drives to the tree
	lpsz = lpszDrives;
	while (*lpsz)
	{
		// Remove the backslash, it will be easier to parse later in
		lpsz[lstrlen(lpsz) - 1] = TEXT('\0');
		AddItemToTree(hwndTV, lpsz, NULL, TRUE);

		// Advance the pointer to the next drive		
		lpsz += (lstrlen(lpsz) + 2);//sizeof(TCHAR));// + sizeof(TCHAR));
	}

	// All done, now clean up after ourselves
	LocalFree(lpszDrives);

	return (TRUE);
}


			

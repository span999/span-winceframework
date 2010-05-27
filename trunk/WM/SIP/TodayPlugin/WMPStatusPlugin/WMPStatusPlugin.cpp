//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft 
// license agreement under which you licensed this sample source code and is 
// provided AS-IS.  If you did not accept the terms of the license agreement, you 
// are not authorized to use this sample source code.  For the terms of the 
// license, please see the license agreement between you and Microsoft.
//
//
// ****************************************************************************
// FILE: WMPStatusPlugin.cpp
// ABTRACT: Main implementation file for the today plug-in 
// ****************************************************************************
//

#include "stdafx.h"

#define STATUS_STRING_LEN   500   // buffer size
#define NOTIFY_CNT 4              // number of notifications


//global variables    
HICON               g_hIcon;
UINT                g_plugInHeight;
HINSTANCE           g_hInst;
HWND                g_hWnd;
BOOL                g_WMPStatusChanged = FALSE; // WMP data has changed flag
HREGNOTIFY          g_hNotify[NOTIFY_CNT];      // Handles to notifications

// forward function declarations
static INT InitializeClasses();
HRESULT RegisterNotifications();
HRESULT UnregisterNotifications();

/*************************************************************************/
/* Entry point for the dll                                               */
/*************************************************************************/
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:            
        
        // The DLL is being loaded for the first time by a given process.
        // Perform per-process initialization here.  If the initialization
        // is successful, return TRUE; if unsuccessful, return FALSE.
        g_hInst = (HINSTANCE)hModule;
        
        //load the icon
        g_hIcon = (HICON)LoadImage(g_hInst, MAKEINTRESOURCE(IDI_WMPSTATUSICON), IMAGE_ICON, DRA::SCALEX(16), DRA::SCALEY(16) ,LR_DEFAULTCOLOR);
        
        //initilize the application class, and set the global window handle
        UnregisterClass((LPCTSTR)LoadString(g_hInst, IDS_WMPSTATUS_APPNAME,0,0), g_hInst);
        InitializeClasses();
        g_hWnd = 0;
        
        break;
        
    case DLL_PROCESS_DETACH:
        // The DLL is being unloaded by a given process.  Do any
        // per-process clean up here, such as undoing what was done in
        // DLL_PROCESS_ATTACH.    The return value is ignored.
        DestroyIcon(g_hIcon);
        UnregisterNotifications();
        UnregisterClass((LPCTSTR)LoadString(g_hInst, IDS_WMPSTATUS_APPNAME,0,0), g_hInst);
        g_hInst = NULL;
        break;           
  }
    
  return TRUE;
}


/*************************************************************************/
/* Handle any messages that may be needed for the plugin                 */
/* Handled messages:                                                     */
/*        WM_TODAYCUSTOM_QUERYREFRESHCACHE                               */
/*        WM_LBUTTONUP                                                   */
/*        WM_PAINT                                                       */
/*                                                                       */
/*        State and Notification Broker Messages:                        */
/*        WM_CHANGE_TRACKARTIST:                                         */
/*        WM_CHANGE_ALBUMTITLE:                                          */
/*        WM_CHANGE_TRACKNUMBER:                                         */
/*        WM_CHANGE_TRACKTITLE:                                          */
/*                                                                       */
/*************************************************************************/
LRESULT CALLBACK WndProc (HWND hwnd, UINT uimessage, WPARAM wParam, LPARAM lParam) 
{
  switch (uimessage)
  {          
    //check to see if a refresh is required
    case WM_TODAYCUSTOM_QUERYREFRESHCACHE:
    {
      TODAYLISTITEM *ptliItem;
      
      BOOL    bReturn = FALSE;
      
      // get the pointer to the item from the Today screen
      ptliItem = (TODAYLISTITEM*)wParam;

      // make sure we have a TODAYLISTITEM and that the shell is ready to go
      if ((NULL == ptliItem) || (WaitForSingleObject(SHELL_API_READY_EVENT, 0) == WAIT_TIMEOUT))
      {
          return FALSE;
      }
      
      // check to see if our WMP data flag has been set, then reset it
      bReturn = g_WMPStatusChanged;
      g_WMPStatusChanged = FALSE;

      // if this is the first time this is called, we should set our height
      if (0 == ptliItem->cyp)
      {
        ptliItem->cyp = DRA::SCALEY(40);
        bReturn = TRUE;
      }
        
      return bReturn;
    }

    case WM_CREATE:         
      break;
        
    case WM_LBUTTONUP: 
      // toggle between play/pause if the WMP window is found
      HWND hwndComm;  

      // find the handle to the plug-in window
      // find this value each time in case WMP has been restarted by the user and the handle has changed
      hwndComm = FindWindow(SZ_MEDIAPLAYERPLUGIN_COMMWINDOW_CLASSNAME, SZ_MEDIAPLAYERPLUGIN_COMMWINDOW_WINDOWNAME);

      if (NULL != hwndComm)
      {
        // send a message to the WMP plug-in to toggle the play/pause state
        PostMessage(hwndComm, WM_WMPTOGGLE, 0, 0);
      }

      break;          
        
    case WM_PAINT: 
      PAINTSTRUCT     ps;
      RECT            rcWindBounds; 
      RECT            rcMyBounds;
      HDC             hDC;
      HFONT           hFontOld;
      TCHAR           szTextBuffer[STATUS_STRING_LEN];
      TCHAR           szRegBuffer[STATUS_STRING_LEN];
      COLORREF        crText;
      size_t          nLen;
      HRESULT hr;

      GetWindowRect( hwnd, &rcWindBounds); 
      hDC = BeginPaint(hwnd, &ps);
      
      // create a custom rectangle relative to the client area
      rcMyBounds.left = 0;
      rcMyBounds.top = DRA::SCALEY(2);
      rcMyBounds.right = rcWindBounds.right - rcWindBounds.left;
      rcMyBounds.bottom = rcWindBounds.bottom - rcWindBounds.top;          
      
      // draw the icon on the screen
      SetBkMode(hDC, TRANSPARENT);
      DrawIcon(hDC, DRA::SCALEY(2), 0, g_hIcon);
      
      BOOL bIsFarEast;
      LOGFONT lf;
      HFONT hSysFont;
      HFONT hFont;

      //determine if this is a Far East platform
      switch (PRIMARYLANGID(LANGIDFROMLCID(GetSystemDefaultLCID())))
      {
        case LANG_CHINESE:
        case LANG_KOREAN:
        case LANG_JAPANESE:
          bIsFarEast = TRUE;
          break;
            
        default:
          bIsFarEast = FALSE;
          break;
      }

      hSysFont = (HFONT) GetStockObject(SYSTEM_FONT);
      GetObject(hSysFont, sizeof(LOGFONT), &lf);

      // If it is far east, use a normal font at 9 points,
      //  otherwise use a bold font as 8 points
      if (bIsFarEast)
      {
        lf.lfWeight = FW_NORMAL;
        // Calculate the font size, making sure to round the result to the nearest integer
        lf.lfHeight = (long) -((9.0 * (double)GetDeviceCaps(hDC, LOGPIXELSY) / 72.0)+.5);
      }
      else
      {
        lf.lfWeight = FW_BOLD;
        // Calculate the font size, making sure to round the result to the nearest integer
        lf.lfHeight = (long) -((8.0 * (double)GetDeviceCaps(hDC, LOGPIXELSY) / 72.0)+.5);
      }

      // create the font
      hFont = CreateFontIndirect(&lf);
  
      // Select the system font into the device context
      hFontOld = (HFONT) SelectObject(hDC, hFont);

      // determine the theme's current text color
      //  this color will change when the user picks a new theme,
      //  so get it each time the display needs to be painted
      crText = SendMessage(GetParent(hwnd), TODAYM_GETCOLOR, (WPARAM) TODAYCOLOR_TEXT, NULL);

      // set that color
      SetTextColor(hDC, crText);

      rcMyBounds.left = rcMyBounds.left + DRA::SCALEX(28);

      // clear our strings
      StringCchCopy(szRegBuffer, STATUS_STRING_LEN, L"");
      StringCchCopy(szTextBuffer, STATUS_STRING_LEN, L"");

      // Read the current data about the currently playing track
      hr = RegistryGetString(SN_MEDIAPLAYERTRACKARTIST_ROOT, SN_MEDIAPLAYERTRACKARTIST_PATH, SN_MEDIAPLAYERTRACKARTIST_VALUE, szTextBuffer, STATUS_STRING_LEN);
      hr = RegistryGetString(SN_MEDIAPLAYERALBUMTITLE_ROOT, SN_MEDIAPLAYERALBUMTITLE_PATH, SN_MEDIAPLAYERALBUMTITLE_VALUE, szRegBuffer, STATUS_STRING_LEN);

      // form the first line of text and draw it
      hr = StringCchLength(szTextBuffer, STATUS_STRING_LEN, &nLen);

      if (nLen > 0)
      {
        StringCchCat(szTextBuffer, STATUS_STRING_LEN, L" : ");
      }

      StringCchCat(szTextBuffer, STATUS_STRING_LEN, szRegBuffer);
      DrawText(hDC, szTextBuffer, -1, &rcMyBounds, DT_LEFT);
          
      // start the 2nd line of text by clearing out our strings
      StringCchCopy(szRegBuffer, STATUS_STRING_LEN, L"");
      StringCchCopy(szTextBuffer, STATUS_STRING_LEN, L"");

      // Read the current data about the currently playing track
      hr = RegistryGetString(SN_MEDIAPLAYERTRACKNUMBER_ROOT, SN_MEDIAPLAYERTRACKNUMBER_PATH, SN_MEDIAPLAYERTRACKNUMBER_VALUE, szTextBuffer, STATUS_STRING_LEN);
      hr = RegistryGetString(SN_MEDIAPLAYERTRACKTITLE_ROOT, SN_MEDIAPLAYERTRACKTITLE_PATH, SN_MEDIAPLAYERTRACKTITLE_VALUE, szRegBuffer, STATUS_STRING_LEN);

      // form the 2nd line of text and draw it
      hr = StringCchLength(szTextBuffer, STATUS_STRING_LEN, &nLen);

      if (nLen > 0)
      {
        StringCchCat(szTextBuffer, STATUS_STRING_LEN, L" : ");
      }

      hr = StringCchLength(szTextBuffer, STATUS_STRING_LEN, &nLen);

      if (nLen > 0)
      {
        StringCchCat(szTextBuffer, STATUS_STRING_LEN, szRegBuffer);
      }
      else
      {
        StringCchCat(szTextBuffer, STATUS_STRING_LEN, L"Windows Media Player Stopped");
      }

      //draw the program item text
      rcMyBounds.top += DRA::SCALEX(20);
      DrawText(hDC, szTextBuffer, -1, &rcMyBounds, DT_LEFT);
      
      // Select the previous font back into the device context
      SelectObject(hDC, hFontOld);
      DeleteObject(hFont);
      EndPaint(hwnd, &ps);
      return 0;
        
    case WM_DESTROY :          
      return 0;
        
    // this fills in the background with the today screen image
    case WM_ERASEBKGND:
      TODAYDRAWWATERMARKINFO dwi;
      dwi.hdc = (HDC)wParam;
      GetClientRect(hwnd, &dwi.rc);
        
      dwi.hwnd = hwnd;
      SendMessage(GetParent(hwnd), TODAYM_DRAWWATERMARK, 0,(LPARAM)&dwi);
      return TRUE;

    // handle messages from the State and Notification Broker when WMP info has changed
    case WM_CHANGE_TRACKARTIST:     
    case WM_CHANGE_ALBUMTITLE:      
    case WM_CHANGE_TRACKNUMBER:     
    case WM_CHANGE_TRACKTITLE:
      g_WMPStatusChanged = TRUE;
      break;
  }

  return DefWindowProc (hwnd, uimessage, wParam, lParam) ;
}



/*************************************************************************/
/* Create and register our window class for the today item                 */
/*************************************************************************/
INT InitializeClasses()
{
  WNDCLASS         wc; 
  memset(&wc, 0, sizeof(wc));
  
  wc.style         = 0;                   
  wc.lpfnWndProc   = (WNDPROC) WndProc;
  wc.hInstance     = g_hInst;
  wc.hIcon         = 0;
  wc.hCursor       = 0;
  wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  wc.lpszClassName = (LPCTSTR)LoadString(g_hInst, IDS_WMPSTATUS_APPNAME,0,0);
  
  //register our window
  if(!RegisterClass(&wc))
  { 
      return 0 ;
  }

  return 1;
}


/*************************************************************************/
/* Initialize the DLL by creating a new window                           */
/*************************************************************************/
HWND InitializeCustomItem(TODAYLISTITEM *ptli, HWND hwndParent) 
{
  long lNotifyIndx;
  LPCTSTR appName = (LPCTSTR)LoadString(g_hInst,IDS_WMPSTATUS_APPNAME,0,0);
  
  //create a new window
  g_hWnd = CreateWindow(appName,appName,WS_VISIBLE | WS_CHILD,
      CW_USEDEFAULT,CW_USEDEFAULT,0,0,hwndParent, NULL, g_hInst, NULL) ;
  
  //display the window
  ShowWindow (g_hWnd, SW_SHOWNORMAL);
  UpdateWindow (g_hWnd) ;  

  // clear out our notification handles
  for (lNotifyIndx=0; lNotifyIndx < NOTIFY_CNT; lNotifyIndx++)
  {
    g_hNotify[lNotifyIndx] = NULL;
  }

  // register our State and Notification Broker notifications
  RegisterNotifications();

  return g_hWnd;
}



/*************************************************************************/
/* Register our State and Notification Broker notifications for          */
/* changes in the Windows Media Player Mobile                            */
/*************************************************************************/
HRESULT RegisterNotifications()
{
  HRESULT hr;

  // Make sure we aren't already registered.
  UnregisterNotifications();

  // Register track artist change notification.
  hr = RegistryNotifyWindow(
                SN_MEDIAPLAYERTRACKARTIST_ROOT,
                SN_MEDIAPLAYERTRACKARTIST_PATH,
                SN_MEDIAPLAYERTRACKARTIST_VALUE,
                g_hWnd, 
                WM_CHANGE_TRACKARTIST,
                0,
                NULL,
                &g_hNotify[0]
                );

  if (SUCCEEDED(hr))
  {
    // Register album title change notification.
    hr = RegistryNotifyWindow(
                  SN_MEDIAPLAYERALBUMTITLE_ROOT,
                  SN_MEDIAPLAYERALBUMTITLE_PATH,
                  SN_MEDIAPLAYERALBUMTITLE_VALUE,
                  g_hWnd, 
                  WM_CHANGE_ALBUMTITLE,
                  0,
                  NULL,
                  &g_hNotify[1]
                  );
  }

  if (SUCCEEDED(hr))
  {
    // Register track number change notification.
    hr = RegistryNotifyWindow(
                  SN_MEDIAPLAYERTRACKNUMBER_ROOT,
                  SN_MEDIAPLAYERTRACKNUMBER_PATH,
                  SN_MEDIAPLAYERTRACKNUMBER_VALUE,
                  g_hWnd, 
                  WM_CHANGE_TRACKNUMBER,
                  0,
                  NULL,
                  &g_hNotify[2]
                  );
  }

  if (SUCCEEDED(hr))
  {
    // Register track title change notification.
    hr = RegistryNotifyWindow(
                  SN_MEDIAPLAYERTRACKTITLE_ROOT,
                  SN_MEDIAPLAYERTRACKTITLE_PATH,
                  SN_MEDIAPLAYERTRACKTITLE_VALUE,
                  g_hWnd, 
                  WM_CHANGE_TRACKTITLE,
                  0,
                  NULL,
                  &g_hNotify[3]
                  );
  }

  return hr;
}



/*************************************************************************/
/* Un-register our notifications                                         */
/*************************************************************************/
HRESULT UnregisterNotifications()
{
  long lNotifyIndx;

  // For each notification handle:
  for (lNotifyIndx=0; lNotifyIndx < NOTIFY_CNT; lNotifyIndx++)
  {
    if (g_hNotify[lNotifyIndx] != NULL)
    {
      // Close off the notification.
      RegistryCloseNotification(g_hNotify[lNotifyIndx]);
    }

    g_hNotify[lNotifyIndx] = 0;
  }

  return S_OK;
}


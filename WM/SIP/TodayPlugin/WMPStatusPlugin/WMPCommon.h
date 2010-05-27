//
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
//
//
// WMPCommon.h - Header file common to both plug-ins.  
//


// message sent from the today plug-in to the WMP background UI plug-in
#define WM_WMPTOGGLE              (WM_APP + 1)    // toggle between play and pause based on current status

// our custom windows messages for our notifications; not used in the WMP plug-in 
// but we don't want the custom window message values to clash in any way
#define WM_CHANGE_TRACKARTIST     (WM_APP + 2)
#define WM_CHANGE_ALBUMTITLE      (WM_APP + 3)
#define WM_CHANGE_TRACKNUMBER     (WM_APP + 4)
#define WM_CHANGE_TRACKTITLE      (WM_APP + 5)



// the WMP background UI plug-in window information
#define SZ_MEDIAPLAYERPLUGIN_COMMWINDOW_CLASSNAME   _T("WMPlayerPlugin_CW_ClassName")
#define SZ_MEDIAPLAYERPLUGIN_COMMWINDOW_WINDOWNAME  _T("WMPlayerPlugin_CW_WindowName")

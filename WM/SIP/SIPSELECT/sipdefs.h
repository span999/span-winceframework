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

	sipdefs.h
	
--*/


#ifndef _SIPDEFS_H_
#define _SIPDEFS_H_


#include <sip.h>
#include "sipproto.h"


#ifdef __cplusplus
extern "C" {
#endif



#define SIPCLASSNAME    TEXT("SipWndClass")


//
// Internal message sent to the SIP thread to select a new IM (from the SIP
// select menu).  We need to do a post to avoid a deadlock in taskbar.cpp when
// we try to update the SIP button icon.
//
#define WM_SIPSELECT        (WM_APP + 0x69)


//
// Info for a loaded IM.
//
typedef struct {
    IMINFO          imi;
    IInputMethod    *pIm;
    CLSID           clsid;
} FULLIMINFO;



#ifdef __cplusplus
}
#endif

#endif // _SIPDEFS_H_

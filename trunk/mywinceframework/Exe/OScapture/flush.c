//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
//------------------------------------------------------------------------------
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//  
//------------------------------------------------------------------------------
//
//  Module Name:  
//  
//      flush.c
//  
//  Abstract:  
//
//      Implements the OS activity capture and flushing routines.
//      
//------------------------------------------------------------------------------

#include <windows.h>
#include <celog.h>
#include "transport.h"
#include "flushzones.h"
#include "spLibIniUtil.h"


#ifdef DEBUG
DBGPARAM dpCurSettings = { TEXT("OSCapture"), {
        TEXT("Verbose"), TEXT(""), TEXT(""), TEXT(""),
        TEXT(""), TEXT(""), TEXT(""), TEXT(""),
        TEXT(""), TEXT(""), TEXT(""), TEXT(""),
        TEXT(""), TEXT(""), TEXT(""), TEXT("")},
    0x00000000
};
#endif

#define lengthof(x) ( (sizeof((x))) / (sizeof(*(x))) )
#define INI_NAME_ON_FLASH   L"\\My Flash Disk\\celog.ini"
#define INI_NAME_ON_SD      L"\\Storage Card\\celog.ini"


//------------------------------------------------------------------------------
// GLOBAL VARIABLES

// Program name
const WCHAR g_szProgName[] = TEXT("OSCapture");

// Program parameters default to these settings but may be overridden in registry
param_t g_Params = {
    2*1000,                           // dwCeLogFlushTimeout, Flush every 2 seconds
    15,                               // dwCeLogSavedFlushes, Hold 15*2sec=30sec of data in the buffer
    248,                              // nCeLogThreadPrio, lowest realtime prio
    TEXT("\\OSCapture.clg"),          // szCeLogFileName
    CELOGFLUSH_TRANSPORT_LOCALFILE,   // nCeLogTransport
    CELOGFLUSH_FILEFLAG_ALWAYS_CLOSE, // dwFileFlags, IGNORED BY THIS APP
    CELZONE_PROCESS | CELZONE_THREAD | CELZONE_RESCHEDULE | CELZONE_PRIORITYINV | CELZONE_CRITSECT | CELZONE_SYNCH | CELZONE_LOADER | CELZONE_INTERRUPT, // dwZoneCE
    TRUE,                             // fSetZones
    FALSE,                            // fUseUI
    0,                                // dwBufSize
    0,                                // dwFileSize
    FALSE,                            // fCapture
};

typedef struct {
    // Keep a set of pointers to the starts of the last several flushes in the ring 
    // buffer, so we can throw data away on flush boundaries
    LPBYTE *ppFlushRing;        // Kept as a ring buffer itself
    DWORD   dwFirstFlush;       // Index of first (oldest) flush in the flush ring
    DWORD   dwNextFlush;        // Index of next flush the flush ring
} FlushRing;

// The RAM buffering is controlled by a trigger event.  In the normal case when
// the event is not set, all data older than g_dwMaxEventAge is discarded so
// that the buffer contains only the most recent data.  When the event is set,
// we take all the data in the buffer and flush it to a file.  Then we reset the 
// event so that we can go back to buffering data.


//------------------------------------------------------------------------------
// Increment mod the number of flushes being saved so that it's always a valid
// index into the flush ring
//------------------------------------------------------------------------------
static _inline DWORD NextIndex(DWORD dwIndex)
{
    return ((dwIndex + 1) % g_Params.dwCeLogSavedFlushes);
}

#if 0
BOOL ReadIni(LPCWSTR pwszSection, LPCWSTR pwszKey, LPWSTR pwszValue, size_t cchValue, LPCWSTR pwszIniFile)
{
   // GetPrivateProfileString is unavailable on CE

   BOOL bSuccess = FALSE;
   FILE * fin;
   WCHAR wszLine[256];
   LPWSTR pwsz = NULL;

   if (!pwszIniFile ||
      !pwszSection ||
      !pwszKey ||
      !pwszValue ||
      !cchValue)
   {
      return bSuccess;
   }

   fin = _wfopen(pwszIniFile, L"r");
   if (fin)
   {
      while (fgetws(wszLine, lengthof(wszLine), fin) && !bSuccess )
      {
         if (!wcsstr(wszLine, pwszSection)) // Section
            continue;

         while (fgetws(wszLine, lengthof(wszLine), fin)) // Key & Value
         {
            pwsz = wcschr(wszLine, L'=');
            if (!pwsz)
               continue;

            *pwsz = L'\0'; // Erase the '='

            if (0 != _wcsicmp(wszLine, pwszKey)) // Key
               continue;

            pwsz++; // Point to the value

            bSuccess = SUCCEEDED(StringCchCopyEx( pwszValue, cchValue, pwsz, &pwsz, NULL, 0 ));

            if (bSuccess) {
               if (*--pwsz == L'\n')
                  *pwsz = L'\0';
               break;
            }
         }
      }

      fclose(fin);
   }

   return bSuccess;
}
#endif


void ReadParamsIni(LPWSTR lpszFileName)
{
    DWORD  dwValSize = 0;
    WCHAR wsz[MAX_PATH];

    NKDbgPrintfW(L"CeLog read ini from %s\r\n", lpszFileName);

    if ( ReadIni(L"[OSCapture]", L"BufferSize", wsz, lengthof(wsz), lpszFileName) )
        g_Params.dwBufSize = _wtoi(wsz);
        //NKDbgPrintfW(L"Pat test 3 %d\r\n", v1);

    if ( ReadIni(L"[OSCapture]", L"FileName", wsz, lengthof(wsz), lpszFileName) ) {
        dwValSize = MAX_PATH * sizeof(WCHAR);
        dwValSize /= sizeof(WCHAR);
        if (dwValSize >= MAX_PATH) dwValSize = MAX_PATH-1;
        wcsncpy(g_Params.szCeLogFileName, wsz, dwValSize);
        g_Params.szCeLogFileName[dwValSize] = 0;  // Make sure it's null-terminated
        //NKDbgPrintfW(L"CeLogFlush FN=%s\r\n", g_Params.szCeLogFileName);
    }

    if ( ReadIni(L"[OSCapture]", L"FileSize", wsz, lengthof(wsz), lpszFileName) )
        g_Params.dwFileSize = _wtoi(wsz);
        NKDbgPrintfW(L"CeLog Flush file size=%d\r\n", g_Params.dwFileSize);

    if ( ReadIni(L"[OSCapture]", L"FlushTimeout", wsz, lengthof(wsz), lpszFileName) )
        g_Params.dwCeLogFlushTimeout = _wtoi(wsz);

    if ( ReadIni(L"[OSCapture]", L"SavedFlushes", wsz, lengthof(wsz), lpszFileName) )
        g_Params.dwCeLogSavedFlushes = _wtoi(wsz);

    if ( ReadIni(L"[OSCapture]", L"ThreadPriority", wsz, lengthof(wsz), lpszFileName) )
        g_Params.nCeLogThreadPrio = _wtoi(wsz);

    if ( ReadIni(L"[OSCapture]", L"Transport", wsz, lengthof(wsz), lpszFileName) )
        g_Params.nCeLogTransport = _wtoi(wsz);

    if ( ReadIni(L"[OSCapture]", L"UseUI", wsz, lengthof(wsz), lpszFileName) )
        g_Params.fUseUI = _wtoi(wsz)==1 ? TRUE : FALSE;

    if ( ReadIni(L"[OSCapture]", L"ZoneCE", wsz, lengthof(wsz), lpszFileName) )
        g_Params.dwZoneCE = _wtoi(wsz);

    if ( ReadIni(L"[OSCapture]", L"Enabled", wsz, lengthof(wsz), lpszFileName) )
        g_Params.fEnabled = _wtoi(wsz)==1 ? TRUE : FALSE;

    NKDbgPrintfW(L"CeLog read ini end\r\n");
}

//------------------------------------------------------------------------------
// Advance the read pointer on flush boundaries
//------------------------------------------------------------------------------
static void AdvanceFlushRing(
    PRINGBUFFER pRingBuf,
    FlushRing*  pFlushes,
    BOOL        fTooFull    // TRUE=buffer is too full to hold required number of flushes
    )
{
    // Update our copy of the write pointer -- only time we read from the header
    pRingBuf->pWrite = (pRingBuf->pBuffer + pRingBuf->pHeader->dwWriteOffset);
    
    // If first==next, all ring slots are free (no saved flushes)
    // If first==next+1, no free ring slots to save in (no room for another flush)
    if ((fTooFull && (pFlushes->dwFirstFlush != pFlushes->dwNextFlush))     // "TooFull + all free" case can happen after a capture
        || (pFlushes->dwFirstFlush == NextIndex(pFlushes->dwNextFlush))) {
        // Need to dump an old flush

        // Add the current pointer to the ring
        if (!fTooFull) {
            pFlushes->ppFlushRing[pFlushes->dwNextFlush] = pRingBuf->pWrite;
            pFlushes->dwNextFlush = pFlushes->dwFirstFlush;  // shortcut instead of calling NextIndex
        }

        if (pFlushes->ppFlushRing[pFlushes->dwFirstFlush]) {
            // Throw away one data flush in the ring buffer
            DEBUGMSG(ZONE_VERBOSE, (TEXT("%s: discarding data from 0x%08x to 0x%08x\r\n"),
                                    g_szProgName,
                                    pFlushes->ppFlushRing[pFlushes->dwFirstFlush],
                                    pFlushes->ppFlushRing[NextIndex(pFlushes->dwFirstFlush)]));
            pRingBuf->pRead = pFlushes->ppFlushRing[pFlushes->dwFirstFlush];
            pRingBuf->pHeader->dwReadOffset = (pRingBuf->pRead - pRingBuf->pBuffer); // Update map header

            // Remove the first pointer in the ring
            pFlushes->ppFlushRing[pFlushes->dwFirstFlush] = NULL;
            pFlushes->dwFirstFlush = NextIndex(pFlushes->dwFirstFlush);
        
        } else {
            // Safety case to prevent setting pRead=NULL: should not happen
            RETAILMSG(1, (TEXT("%s: Recovering from drastic error, losing all buffered data\r\n"),
                          g_szProgName));
            DEBUGCHK(0);
            
            // Throw away all buffered data and reset the flush ring
            pRingBuf->pRead = pRingBuf->pWrite;
            pRingBuf->pHeader->dwReadOffset = (pRingBuf->pRead - pRingBuf->pBuffer); // Update map header
            pFlushes->dwFirstFlush = 0;
            pFlushes->dwNextFlush  = 0;
            memset(pFlushes->ppFlushRing, 0, (g_Params.dwCeLogSavedFlushes+1) * sizeof(LPBYTE));
        }
    
    } else {
        // No need to dump an old flush, there's room to add a new one

        // Add the current pointer to the ring
        pFlushes->ppFlushRing[pFlushes->dwNextFlush] = pRingBuf->pWrite;
        pFlushes->dwNextFlush = NextIndex(pFlushes->dwNextFlush);
    }
}


// Used with rghWaitEvents below
typedef enum {
    FILL_EVENT = 0,
    QUIT_EVENT = 1,
    TRIGGER_EVENT = 2,

    NUM_WAIT_EVENTS
} EventID;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static DWORD FlushForever()
{
    HANDLE     hLib = NULL;
    HANDLE     rghWaitEvents[NUM_WAIT_EVENTS];
    RINGBUFFER RingBuf;
    DWORD      dwWaitResult;
    CeLogTransport transport;
    BOOL       fResync = FALSE;       // Flag indicating whether a resync is called for
    BOOL       fCheckIni = TRUE;
    FlushRing  flushes;

    transport.hInstance = INVALID_HANDLE_VALUE;
    flushes.ppFlushRing = NULL;

    rghWaitEvents[FILL_EVENT] = NULL;
    rghWaitEvents[QUIT_EVENT] = NULL;
    rghWaitEvents[TRIGGER_EVENT] = NULL;

    // Get exclusive access to the memory-mapped file, load celog.dll if necessary
    hLib = NULL;  // Receives handle to celog.dll if loaded
    if (!OpenMap(&RingBuf, &hLib)) {
        goto error;
    }

    // Call into CeLog to set the zones
    CeLogSetZones((DWORD)-1, g_Params.dwZoneCE, (DWORD)-1);
    
    // CeLog should create the fill event
    rghWaitEvents[FILL_EVENT] = OpenEvent(EVENT_ALL_ACCESS, FALSE,
                                          CELOG_FILLEVENT_NAME);
    if (rghWaitEvents[FILL_EVENT] == NULL) {
        OutputMsg(TEXT("%s: Unable to open CeLog fill event\r\n"), g_szProgName);
        goto error;
    }

    // Create or open the quit event
    rghWaitEvents[QUIT_EVENT] = CreateEvent(NULL, TRUE, FALSE,
                                               CELOGFLUSH_QUITEVENT_NAME);
    if (rghWaitEvents[QUIT_EVENT] == NULL) {
        OutputMsg(TEXT("%s: Unable to open CeLog quit event\r\n"), g_szProgName);
        goto error;
    }
    
    // Create or open the trigger event
    rghWaitEvents[TRIGGER_EVENT] = CreateEvent(NULL, TRUE, FALSE,
                                               OSCAPTURE_TRIGGEREVENT_NAME);
    if (rghWaitEvents[TRIGGER_EVENT] == NULL) {
        OutputMsg(TEXT("%s: Unable to create trigger event\r\n"), g_szProgName);
        goto error;
    }
    
    // Set up the flush ring to track flushes as pointers into the ring buffer
    if ((g_Params.dwCeLogSavedFlushes+1 < g_Params.dwCeLogSavedFlushes) ||
        (ULONG_MAX / sizeof(LPBYTE) <= (g_Params.dwCeLogSavedFlushes+1))) {
        OutputMsg(TEXT("Too many saved flushes, buffer overflow would occur\r\r"));
        goto error;
    }
    flushes.ppFlushRing = LocalAlloc(LPTR, (g_Params.dwCeLogSavedFlushes+1) * sizeof(LPBYTE));
    if (!flushes.ppFlushRing) {
        goto error;
    }
    flushes.dwFirstFlush = 0;
    flushes.dwNextFlush  = 0;

    // Initialize the transport
    GetTransport(&transport);
    if (transport.Init
        && !transport.Init(g_Params.szCeLogFileName)) {
        OutputMsg(TEXT("%s: Transport initialization failed\r\n"), g_szProgName);
        goto error;
    }

    // show again    
    PrintParams();

    // Keep flushing until the quit event is signalled
    do {
        
        // Wait until the buffer gets relatively full or until the timeout
        // expires, and then flush it.
        
        dwWaitResult = WaitForMultipleObjects(NUM_WAIT_EVENTS, rghWaitEvents, FALSE, g_Params.dwCeLogFlushTimeout);
        
        if (GetFileAttributes(INI_NAME_ON_SD) == (DWORD)-1) {
            fCheckIni = TRUE;
        } else {
            if ( fCheckIni ) {
                DWORD dwKeep = g_Params.dwCeLogSavedFlushes;

                NKDbgPrintfW(L"CeLog Ini file detected on SD card, update params\r\n");
                fCheckIni = FALSE;
                ReadParamsIni(INI_NAME_ON_SD);
                g_Params.dwCeLogSavedFlushes = dwKeep;

                if (g_Params.fUseUI)
                    InitUI();
                else
                    CleanupUI();
                CeLogSetZones((DWORD)-1, g_Params.dwZoneCE, (DWORD)-1);
                if (!CeSetThreadPriority(GetCurrentThread(), g_Params.nCeLogThreadPrio))
                    NKDbgPrintfW(L"%s: Warning, unable to set thread priority (%u)\r\n", g_szProgName, GetLastError());

            }
        }
       
        
        // Flush if the trigger was set, else discard
        if  ( (dwWaitResult == (WAIT_OBJECT_0 + TRIGGER_EVENT))
              || ((dwWaitResult == (WAIT_OBJECT_0 + FILL_EVENT)) && g_Params.fEnabled)
              || ((dwWaitResult == WAIT_TIMEOUT) && g_Params.fEnabled)
            )
            {
            // Set off a resync to make it easier to understand the log
            RETAILMSG(1, (TEXT("%s: Triggered, flushing data\r\n"), g_szProgName));
            DEBUGMSG(ZONE_VERBOSE, (TEXT("%s: Causing ReSync\r\n"), g_szProgName));
            CeLogReSync();
            
            if (OpenTransport(&transport, g_Params.szCeLogFileName, &fResync)) {
                FlushMap(&RingBuf, &transport, FALSE, NULL);
                CloseTransport(&transport, FALSE, TRUE);

                // Done flushing, everything in the ring is discarded
                flushes.dwFirstFlush = 0;
                flushes.dwNextFlush  = 0;
                memset(flushes.ppFlushRing, 0, (g_Params.dwCeLogSavedFlushes+1) * sizeof(LPBYTE));

                OutputMsg(TEXT("Recent system activity has been captured to %s."), g_Params.szCeLogFileName);
            }
        
            // Reset the trigger event
            ResetEvent(rghWaitEvents[TRIGGER_EVENT]);

        } else {
            AdvanceFlushRing(&RingBuf, &flushes,
                             (dwWaitResult == (WAIT_OBJECT_0 + FILL_EVENT)));
        }

        // Allow the logger to set the fill event
        RingBuf.pHeader->fSetEvent = TRUE;
        
    } while ((dwWaitResult == WAIT_TIMEOUT)
             || (dwWaitResult == (WAIT_OBJECT_0 + FILL_EVENT))
             || (dwWaitResult == (WAIT_OBJECT_0 + TRIGGER_EVENT)));

    OutputMsg(TEXT("%s: Exiting\r\n"), g_szProgName);

error:
    
    // Force transport closure
    CloseTransport(&transport, FALSE, TRUE);

    if (flushes.ppFlushRing) {
        LocalFree(flushes.ppFlushRing);
    }

    if (rghWaitEvents[TRIGGER_EVENT]) {
        CloseHandle(rghWaitEvents[TRIGGER_EVENT]);
    }

    if (rghWaitEvents[QUIT_EVENT]) {
        CloseHandle(rghWaitEvents[QUIT_EVENT]);
    }
    
    if (rghWaitEvents[FILL_EVENT]) {
        CloseHandle(rghWaitEvents[FILL_EVENT]);
    }

    CloseMap(&RingBuf);
    if (hLib) {
        FreeLibrary(hLib);
    }
    
    return 0;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static VOID TriggerOtherInstance()
{
    HANDLE hInstanceEvent;
    HANDLE hTriggerEvent;

    // A named event tells us if a flush application is already open.  There's
    // no guarantee that it's not be celogflush or the shell flusher, but if
    // the event doesn't exist we know for sure OSCapture is not open.
    hInstanceEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, CELOGFLUSH_INSTANCEEVENT_NAME);
    CloseHandle(hInstanceEvent);
    if (hInstanceEvent == NULL) {
        // Cannot capture if there's not another instance running
        OutputMsg(TEXT("%s: Unable to capture activity, no other instances of OSCapture.exe are running\r\n"),
                  g_szProgName);
        return;
    }
        
    // Another instance of OSCapture is running (expected)
    RETAILMSG(1, (TEXT("%s: Trigger other instance to capture recent activity.\r\n"),
                  g_szProgName));
    hTriggerEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, OSCAPTURE_TRIGGEREVENT_NAME);
    if (hTriggerEvent != NULL) {
        SetEvent(hTriggerEvent);
        CloseHandle(hTriggerEvent);
    } else {
        OutputMsg(TEXT("%s: Unable to capture activity, trigger event not found\r\n"),
                  g_szProgName);
    }
}


// From coredll; not in any headers for some reason
extern BOOL IsAPIReady(DWORD hAPI);


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int wmain(int argc, WCHAR *argv[])
{   
    DWORD  dwStartIndex = 0;  // Index in the HKLM\Init sequence

    // Read program parameters
    ReadParams();
    if (GetFileAttributes(INI_NAME_ON_SD) == (DWORD)-1)
        ReadParamsIni(INI_NAME_ON_FLASH);
    else
        ReadParamsIni(INI_NAME_ON_SD);
    if (!ParseCommandLine(argc, argv, &dwStartIndex)) {
        return 1;
    }
    
    InitUI();  // continue even if this fails

    // -c param means we must signal the other instance to capture the most
    // recent data, and then exit.
    if (g_Params.fCapture) {
        TriggerOtherInstance();
        CleanupUI();
        return 0;
    }

    DEBUGMSG(1, (TEXT("%s application starting.\r\n"), g_szProgName));

    // Enable debug zones
    DEBUGREGISTER(NULL);
    
    // Setting the buffer size in the registry is the only way we can try to honor it
    if (g_Params.dwBufSize && IsAPIReady(SH_FILESYS_APIS)) {
        HKEY hKey;
        LONG lResult;

        lResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("System\\CeLog"), 0, NULL, 0, 0, NULL, &hKey, NULL);
        if (lResult == ERROR_SUCCESS) {
            RegSetValueEx(hKey, TEXT("BufferSize"), 0, REG_DWORD, (LPBYTE)&g_Params.dwBufSize, sizeof(DWORD));
            RegSetValueEx(hKey, TEXT("ZoneCE"), 0, REG_DWORD, (LPBYTE)&g_Params.dwZoneCE, sizeof(DWORD));
            RegCloseKey(hKey);
        }
    }

    // Signal if this application was started in the HKLM\init sequence
    if (dwStartIndex) {
        SignalStarted(dwStartIndex);
    }
    
    PrintParams();

    // Become the flush thread
    if (!CeSetThreadPriority(GetCurrentThread(), g_Params.nCeLogThreadPrio)) {
        RETAILMSG(1, (TEXT("%s: Warning, unable to set thread priority (%u)\r\n"), g_szProgName, GetLastError()));
    }
    FlushForever();

    // Reach this point only if quit event was set or if other instance exists
    CleanupUI();
    
    
    RETAILMSG(1, (TEXT("%s application exit.\r\n"), g_szProgName));

    return 0;
}


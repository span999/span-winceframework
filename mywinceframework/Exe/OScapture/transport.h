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

// Private copy of information in the memory-mapped file header
typedef struct _RINGBUFFER {
    PMAPHEADER pHeader;
    LPBYTE pBuffer;     // Start of buffer
    LPBYTE pRead;       // Private copy of read pointer, not strictly necessary but
                        // nice if we need to avoid updating the map header for a while
    LPBYTE pWrite;      // Copy of write pointer, in case it's moving
    LPBYTE pWrap;       // End of buffer
    DWORD  dwLostBytes; // Last count of lost bytes
    
    HANDLE hMap;        // Handle to memory-mapped file
    HANDLE hInstanceEvent; // Handle to event that indicates ownership of the memory-mapped file
} RINGBUFFER, *PRINGBUFFER;


// CeLog transport information

typedef struct CeLogTransport {
    BOOL   (*Init)(LPWSTR lpszFileName);
    HANDLE (*Open)(LPWSTR lpszFileName, BOOL* pfWriteMarker);
    BOOL   (*Send)(HANDLE hInstance, BYTE* pData, DWORD dwNumBytes);
    BOOL   (*Close)(HANDLE hInstance);
    HANDLE hInstance;
} CeLogTransport;

void GetTransport(CeLogTransport* pTransport);
BOOL OpenTransport(CeLogTransport* pTransport, LPWSTR lpszOutputFile, BOOL* pfResync);
VOID CloseTransport(CeLogTransport* pTransport, BOOL fBusy, BOOL fForce);


// This event is used to keep from running more than one flush app at the same time
#define CELOGFLUSH_INSTANCEEVENT_NAME   TEXT("CeLog Flush Instance")

// This event is used to make celogflush and oscapture exit on request
#define CELOGFLUSH_QUITEVENT_NAME       TEXT("SYSTEM/CeLogFlush Quit")

// This event is used to trigger a running instance of OSCapture.exe to flush
#define OSCAPTURE_TRIGGEREVENT_NAME     TEXT("SYSTEM/OSCaptureTrigger")


#define CELOGFLUSH_TRANSPORT_NONE       0
#define CELOGFLUSH_TRANSPORT_CESH       1
#define CELOGFLUSH_TRANSPORT_RAM        2
#define CELOGFLUSH_TRANSPORT_LOCALFILE  3

// These flags are not compatible with each other
#define CELOGFLUSH_FILEFLAG_NEVER_CLOSE  0x1
#define CELOGFLUSH_FILEFLAG_ALWAYS_CLOSE 0x2

extern const WCHAR g_szProgName[];

// Program parameters
typedef struct param_t {
    DWORD dwCeLogFlushTimeout;       // Timeout between flushes
    DWORD dwCeLogSavedFlushes;       // # of flushes to hold in OSCapture buffer
    int   nCeLogThreadPrio;          // Priority of flush thread
    WCHAR szCeLogFileName[MAX_PATH]; // Filename to flush to, if applicable
    int   nCeLogTransport;           // One of the CELOGFLUSH_TRANSPORT flags
    DWORD dwFileFlags;               // Bitwise OR of the CELOGFLUSH_FILEFLAGs
    DWORD dwZoneCE;                  // Zones to use if fSetZones=TRUE
    BOOL  fSetZones;                 // TRUE=Call CeLogSetZones with dwZoneCE, FALSE=leave zones as-is
    BOOL  fUseUI;                    // TRUE=Popup UI, FALSE=debug messages only
    DWORD dwBufSize;                 // Size of RAM buffer to use
    DWORD dwFileSize;                // Maximum file size
    // Command-line only
    BOOL  fCapture;                  // -c param: Trigger other instance of OSCapture to capture data
    BOOL  fEnabled;
} param_t;

extern param_t g_Params;


// UI
extern BOOL InitUI();
extern BOOL CleanupUI();
extern void OutputMsg(const WCHAR* format, ...);

// Program parameters
extern BOOL ReadParams();
extern BOOL ParseCommandLine(int argc, WCHAR *argv[], DWORD *pdwStartIndex);
extern VOID PrintParams();

// Memory-mapped file helpers
BOOL OpenMap(PRINGBUFFER pRingBuf, HANDLE* phLib);
VOID CloseMap(PRINGBUFFER pRingBuf);
void FlushMap(PRINGBUFFER pRingBuf, CeLogTransport* pTransport, BOOL fWriteLossCounter, BOOL* pfBusy);


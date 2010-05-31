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
//      transport.c
//  
//  Abstract:  
//
//      Abstraction of data transport for celog data flushing routines.
//      
//------------------------------------------------------------------------------

#include <windows.h>
#include <celog.h>
#include "transport.h"
#include "flushzones.h"


//------------------------------------------------------------------------------
// PROGRAM PARAMETERS
//------------------------------------------------------------------------------
//
// These registry values can be set to override the program defaults.
//   [HKEY_LOCAL_MACHINE\System\CeLog]
//      "FlushTimeout"=dword:<timeout>
//      "SavedFlushes"=dword:<count>
//      "ThreadPriority"=dword:<priority>
//      "Transport"="<transport>"
//      "FileName"="<filename>"
//      "FileFlags"=dword:<flags>
//      "ZoneCE"=dword:<zones>
//      "UseUI"=dword:<0 or 1>
//
// These command-line parameters can be used to override the registry values and
// program defaults.  (All Numbers are decimal unless otherwise noted.)
//      -time <timeout>      FlushTimeout,    eg. -time 10000
//      -save <count>        SavedFlushes,    eg. -save 15
//      -pri <priority>      ThreadPriority,  eg. -pri 248
//      -trans <transport>   Transport,       eg. -trans LocalFile
//      -n <filename>        FileName,        eg. -n \Release\celog.clg
//      -flags <hex flags>   FileFlags,       eg. -flags 0x00000001
//      -z <hex zones>       ZoneCE,          eg. -z 0x000043E3
//      -ui <0 or 1>         UseUI,           eg. -ui 1
//      -buf <hex size>      BufferSize,      eg. -buf 0x00100000  (ignored if CeLog is already loaded)
//      -filesize <hex size> FileSize,        eg. -filesize 0x00100000
//      -c                   CaptureActivity, eg. -c
//
// FlushTimeout:   Hex value for timeout (in milliseconds) between successive
//                 flushes.  Flushes will occur more frequently if data is
//                 collected very quickly.
//                 - CeLogFlush.exe default: 0x00002710 = 10000 = 10 seconds
//                 - OSCapture.exe default:  0x000007D0 =  2000 =  2 seconds
//
// SavedFlushes:   Hex value for number of flushes to save.  This lets the
//                 user to choose how much time to save in the buffer, eg.
//                 (FlushTimeout) * (SavedFlushes) = # of milliseconds of
//                 activity saved.  This is not precise because data will
//                 be thrown out faster if the buffer overruns.
//                 - CeLogFlush.exe default: 0x00000000 (IGNORED)
//                 - OSCapture.exe default:  0x0000000F = 15
//
// ThreadPriority: Hex value for priority of the flushing thread (0-255).
//                 - Default 248 = lowest realtime prio
//
// Transport:      Which transport to use, choices are:
//      "RAM"           Buffer data in RAM, keeping newest
//      "LocalFile"     Store data in log file on the local file system
//      "CESH"          Store data in log file on the remote host
//                 - Default "LocalFile"
//
// FileName:       Which log file name to store the data in, if transport is
//                 localfile or cesh.
//                 - CeLogFlush.exe default: "\Release\celog.clg"
//                 - OSCapture.exe default:  "\OSCapture.clg"
//
// FileFlags:      Hex flags for file management:  (cannot be used together)
//      0x00000000      Auto-close the log file if no data has been lost
//                      recently (default)
//      0x00000001      Never close the log file (best performance but
//                      keeps file handle open)
//      0x00000002      Always close the log file
//                 - CeLogFlush.exe default: 0x00000000
//                 - OSCapture.exe default:  0x00000002 (IGNORED)
//
// ZoneCE:         Hex flags for CeLog zone settings.  If this value is
//                 present, the zones will be passed to CeLog via a call
//                 to CeLogSetZones.
//                 - CeLogFlush.exe default:  If the value is not present, 
//                   CeLogSetZones will not be called so CeLog will keep its
//                   previous zone settings.
//                 - OSCapture.exe default:  0x000043E3 (scheduler-related zones)
//
// UseUI:          Boolean value, TRUE=use popup messages, FALSE=debug messages only
//                 - Default FALSE
//
// BufferSize:     Hex size of the RAM buffer to use.
//                 - Default 128KB
//
// FileSize:       Hex size to limit the log file to.  When the file reaches
//                 that size, it will be renamed to <FileName>.old.  So the log
//                 files will take approximately 2x the specified FileSize.
//                 Forces FileFlags=2 because the rename happens only when
//                 the file is opened at the start of a flush.  Set to 0 to not
//                 impose a file size limit.
//                 - Default 0, no limit
//


//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

#define COPY_INCREMENT 128*1024  // How much to copy at a time (bytes)

// Used to put up UI if possible
HANDLE  g_hCoreDLL;
int (*g_pfnMessageBox) (HWND, LPCTSTR, LPCTSTR, UINT);


//------------------------------------------------------------------------------
// TRANSPORT-INDEPENDENT HELPERS
//------------------------------------------------------------------------------

// Open the transport if not already open
BOOL OpenTransport(
    CeLogTransport* pTransport,
    LPWSTR lpszOutputFile,
    BOOL*  pfResync
    )
{
    if ((pTransport->hInstance == INVALID_HANDLE_VALUE) && pTransport->Open) {
        pTransport->hInstance = pTransport->Open(lpszOutputFile, pfResync);
        if (pTransport->hInstance == INVALID_HANDLE_VALUE) {
            DEBUGMSG(1, (TEXT("%s: Warning, unable to open %s!\r\n"),
                         g_szProgName, lpszOutputFile));
            return FALSE;
        }
    }

    return TRUE;
}

// Close the transport or keep it open, based on file flags and "busyness" flag
VOID CloseTransport(
    CeLogTransport* pTransport,
    BOOL fBusy,                     // TRUE=too busy to close unless forced
    BOOL fForce                     // TRUE=force close
    )
{
    if (pTransport->Close && (pTransport->hInstance != INVALID_HANDLE_VALUE)) {
        if (fForce
            || (g_Params.dwFileFlags & CELOGFLUSH_FILEFLAG_ALWAYS_CLOSE)
            || (!(g_Params.dwFileFlags & CELOGFLUSH_FILEFLAG_NEVER_CLOSE) && !fBusy)) {
            pTransport->Close(pTransport->hInstance);
            pTransport->hInstance = INVALID_HANDLE_VALUE;
        }
    }
}


//------------------------------------------------------------------------------
// MEMORY-MAPPED FILE ACCESS FUNCTIONS
//------------------------------------------------------------------------------

// Close the memory-mapped file
VOID CloseMap(
    PRINGBUFFER pRingBuf
    )
{
    if (pRingBuf->pHeader) {
        UnmapViewOfFile(pRingBuf->pHeader);
        pRingBuf->pHeader = NULL;
    }
    
    if (pRingBuf->hMap) {
        CloseHandle(pRingBuf->hMap);
        pRingBuf->hMap = NULL;
    }

    if (pRingBuf->hInstanceEvent) {
        CloseHandle(pRingBuf->hInstanceEvent);
        pRingBuf->hInstanceEvent = NULL;
    }
}


// Open up the memory-mapped file and initialize the local RingBuffer
BOOL OpenMap(
    PRINGBUFFER pRingBuf,
    HANDLE*     phLib       // If non-NULL, celog.dll may be loaded and returned
    )
{
    pRingBuf->pHeader = NULL;
    pRingBuf->hMap = NULL;
    pRingBuf->hInstanceEvent = NULL;

    //
    // A named event keeps multiple apps from accessing the memory-mapped file
    // at the same time.
    //

    pRingBuf->hInstanceEvent = CreateEvent(NULL, FALSE, FALSE, CELOGFLUSH_INSTANCEEVENT_NAME);
    if (!pRingBuf->hInstanceEvent) {
        OutputMsg(TEXT("%s: Error, unable to create named event (%u)\r\n"),
                  g_szProgName, GetLastError());
        return FALSE;
    
    } else if (GetLastError() == ERROR_ALREADY_EXISTS) {
        // Some other flusher already owns the memory-mapped file
        OutputMsg(TEXT("%s: Another CeLog flushing tool is already running, aborting to avoid conflict.\r\n"),
                  g_szProgName);
        CloseMap(pRingBuf);
        return FALSE;
    }

    
    //
    // Open the memory-mapped file
    //

    pRingBuf->hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
                                       PAGE_READWRITE, 0, 0,
                                       CELOG_DATAMAP_NAME);
    if (!pRingBuf->hMap) {
        if (phLib) {
            // Attempt to load CeLog DLL
            *phLib = LoadKernelLibrary(TEXT("CeLog.dll"));
            if (!(*phLib)) {
                OutputMsg(TEXT("%s: Failed to load CeLog DLL\r\n"), g_szProgName);
                CloseMap(pRingBuf);
                return FALSE;
            }

            // Now try again
            pRingBuf->hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
                                               PAGE_READWRITE, 0, 0,
                                               CELOG_DATAMAP_NAME);
            if (!pRingBuf->hMap) {
                OutputMsg(TEXT("%s: CeLog map not found\r\n"), g_szProgName);
                CloseMap(pRingBuf);
                return FALSE;
            }
        } else {
            CloseMap(pRingBuf);
            return FALSE;
        }
    }

    pRingBuf->pHeader = (PMAPHEADER)MapViewOfFile(pRingBuf->hMap,
                                                  FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!pRingBuf->pHeader) {
        OutputMsg(TEXT("%s: Unable to open CeLog map\r\n"), g_szProgName);
        CloseMap(pRingBuf);
        return FALSE;
    }
    
    
    //
    // Set up the local copy of the global ring structure
    //
    
    // Make sure the header is V2, since the read pointer from the old version
    // won't work.
    if (pRingBuf->pHeader->pRead || pRingBuf->pHeader->pWrite
        || (pRingBuf->pHeader->dwVersion != 2)) {
        OutputMsg(TEXT("%s: Unknown CeLog map format\r\n"), g_szProgName);
        CloseMap(pRingBuf);
        return FALSE;
    }
    
    pRingBuf->pBuffer     = (LPBYTE)pRingBuf->pHeader + pRingBuf->pHeader->dwBufferStart;
    pRingBuf->pRead       = pRingBuf->pBuffer + pRingBuf->pHeader->dwReadOffset;
    pRingBuf->pWrap       = pRingBuf->pBuffer + pRingBuf->pHeader->dwBufSize;
    pRingBuf->dwLostBytes = 0;

    DEBUGMSG(ZONE_VERBOSE, (TEXT("pHeader = 0x%08x\r\n"), pRingBuf->pHeader));
    DEBUGMSG(ZONE_VERBOSE, (TEXT("pBuffer = 0x%08x\r\n"), pRingBuf->pBuffer));
    DEBUGMSG(ZONE_VERBOSE, (TEXT("pWrap   = 0x%08x\r\n"), pRingBuf->pWrap));

    return TRUE;
}


// Flush all of the data in the memory-mapped file to the given transport.
void FlushMap(
    PRINGBUFFER pRingBuf,
    CeLogTransport* pTransport,
    BOOL  fWriteLossCounter,        // TRUE=write data loss counter; FALSE=don't write
    BOOL* pfBusy                    // If non-NULL, tells caller whether the log's too busy to close
    )
{
    DWORD  dwSrcLen1, dwSrcLen2;
    DWORD  dwWrite = 0;

    // Update our copy of the write pointer -- only time we read from the header
    pRingBuf->pWrite = (pRingBuf->pBuffer + pRingBuf->pHeader->dwWriteOffset);
    
    // Determine where the data is in the map
    if (pRingBuf->pWrite >= pRingBuf->pRead) {
        //   pBuffer                 pWrap
        //   v                       v
        //  +-----------------------+ 
        //  |      |====1===|       | 
        //  +-----------------------+ 
        //          ^        ^
        //          pRead    pWrite 
        dwSrcLen1 = (DWORD) pRingBuf->pWrite - (DWORD) pRingBuf->pRead;
        dwSrcLen2 = 0;

    } else {
        //   pBuffer                 pWrap
        //   v                       v
        //  +-----------------------+ 
        //  |===2==|        |===1===| 
        //  +-----------------------+ 
        //          ^        ^
        //          pWrite   pRead  
        dwSrcLen1 = (DWORD) pRingBuf->pWrap - (DWORD) pRingBuf->pRead;
        dwSrcLen2 = (DWORD) pRingBuf->pWrite - (DWORD) pRingBuf->pBuffer;
    }

        
    // Write out in smaller chunks to free up space immediately, instead of 
    // writing the whole buffer at once.

    if (dwSrcLen1) {
        // Write out in chunks
        do {
            if (dwSrcLen1 < COPY_INCREMENT) {
               dwWrite = dwSrcLen1;
            } else {
               dwWrite = COPY_INCREMENT;
            }

            if (pTransport->Send) {
                pTransport->Send(pTransport->hInstance, (BYTE*)pRingBuf->pRead, dwWrite);
            }
            
            pRingBuf->pRead += dwWrite;
            pRingBuf->pHeader->dwReadOffset = (pRingBuf->pRead - pRingBuf->pBuffer); // Update map header
            dwSrcLen1 -= dwWrite;
        
        } while (dwSrcLen1);
    }
    
    if (dwSrcLen2) {
        // Wrapped.
        pRingBuf->pRead = pRingBuf->pBuffer;
        pRingBuf->pHeader->dwReadOffset = (pRingBuf->pRead - pRingBuf->pBuffer); // Update map header

        // Write out in chunks
        do {
            if (dwSrcLen2 < COPY_INCREMENT) {
               dwWrite = dwSrcLen2;
            } else {
               dwWrite = COPY_INCREMENT;
            }

            if (pTransport->Send) {
                pTransport->Send(pTransport->hInstance, (BYTE*)pRingBuf->pRead, dwWrite);
            }

            pRingBuf->pRead += dwWrite;
            pRingBuf->pHeader->dwReadOffset = (pRingBuf->pRead - pRingBuf->pBuffer); // Update map header
            dwSrcLen2 -= dwWrite;
        
        } while (dwSrcLen2);
    }

    if (fWriteLossCounter) {
        DWORD dwLostCopy, dwData[2];

        // Write out buffer overrun data, whether we lost anything or not.
        dwLostCopy = pRingBuf->pHeader->dwLostBytes; // Make a copy to prevent race conditions
        dwData[0] = CELID_DATA_LOSS << 16 | sizeof(CEL_DATA_LOSS);
        dwData[1] = dwLostCopy - pRingBuf->dwLostBytes;
        pRingBuf->dwLostBytes = dwLostCopy;
    
        if (pTransport->Send) {
            pTransport->Send(pTransport->hInstance, (BYTE*)dwData, 2*sizeof(DWORD));
        }
    
        // Set the busy flag only if data loss has not been a problem.  Constantly
        // opening and closing the file can cause the flush to be slow enough to
        // cause data loss if data is arriving at a high rate.
        if (pfBusy && (dwData[1] != 0)) {  // if no data lost this flush
            *pfBusy = TRUE;
        }
    }
}


//------------------------------------------------------------------------------
// CESH TRANSPORT FUNCTIONS
//------------------------------------------------------------------------------

static BOOL InitCESH(LPWSTR lpszFileName)
{
    int hRemoteFile;

    // Blow away the log file, if it exists
    hRemoteFile = U_ropen(lpszFileName, 0x80002);

    if (hRemoteFile == HFILE_ERROR) {
        RETAILMSG(1, (TEXT("%s: Failed opening %s\r\n"), g_szProgName, lpszFileName));
        return FALSE;
    }
    
    U_rclose(hRemoteFile);
    
    return TRUE;
}

// Open file and return handle
static HANDLE OpenCESH(LPWSTR lpszFileName, BOOL* pfWriteMarker)
{
    int hRemoteFile;

    // Try to open an existing file first
    hRemoteFile = U_ropen(lpszFileName, 0x10002);
    if (hRemoteFile == HFILE_ERROR) {
        // The file does not exist or cannot be opened -- try to create it
        hRemoteFile = U_ropen(lpszFileName, 0x90002);
        if (hRemoteFile == HFILE_ERROR) {
            RETAILMSG(1, (TEXT("%s: Failed opening %s\r\n"), g_szProgName, lpszFileName));
            return INVALID_HANDLE_VALUE;
        }

        // Signal that we should write a marker
        *pfWriteMarker = TRUE;
    }

    U_rlseek(hRemoteFile, 0, FILE_END);

    return (HANDLE)hRemoteFile;
}

// Close handle
static BOOL CloseCESH(HANDLE hRemoteFile)
{
    U_rclose((int)hRemoteFile);
    return TRUE;
}

// Log to an open file over CESH
static BOOL SendCESH(HANDLE hRemoteFile, BYTE* pData, DWORD dwNumBytes)
{
    U_rwrite((int)hRemoteFile, (char*)pData, dwNumBytes);
    return TRUE;
}



//------------------------------------------------------------------------------
// FileLog TRANSPORT FUNCTIONS
//------------------------------------------------------------------------------

// create the local log file if it does not already exist
static BOOL InitFileLog(LPWSTR lpszFileName)
{
    HANDLE hLocalFile;

    //
    // Blow away the log file, if it exists.  But instead of just deleting it,
    // we first overwrite it to check that we can write to this filename.
    //

    hLocalFile = CreateFile(lpszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL, NULL);
    if (hLocalFile == INVALID_HANDLE_VALUE) {
        OutputMsg(TEXT("%s: Unable to open file %s\r\n"), g_szProgName, lpszFileName);
        return FALSE;
    }
    CloseHandle(hLocalFile);
        
    // Now delete it so that a resync is triggered on the first open
    DeleteFile(lpszFileName);
    
    OutputMsg(TEXT("%s: Using file %s\r\n"), g_szProgName, lpszFileName);
    
    return TRUE;
}

// Open file and return handle to the local log file on the machine
static HANDLE OpenFileLog(LPWSTR lpszFileName, BOOL* pfWriteMarker)
{
    HANDLE hLocalFile;
    BOOL   fWriteMarker = FALSE;
    DWORD  cbFile = 0;

    // Don't use LastError to know if the file previously existed, in case the
    // file system does not set that flag properly.
    if (GetFileAttributes(lpszFileName) == (DWORD)-1) {
        fWriteMarker = TRUE;  // Don't set *pfWriteMarker until after CreateFile succeeds
    }

    hLocalFile = CreateFile(lpszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
    if (hLocalFile == INVALID_HANDLE_VALUE) {
        RETAILMSG(1, (TEXT("%s: Failed opening %s\r\n"), g_szProgName, lpszFileName));
        return hLocalFile;
    }
    cbFile = GetFileSize (hLocalFile, NULL);
    
    // If the existing file is already larger than the specified file size, rename it
    // to a swap-file and start with a fresh file.
    if (g_Params.dwFileSize && (cbFile > g_Params.dwFileSize)) {
        WCHAR szSwapFile[MAX_PATH];

        if (SUCCEEDED (StringCchCopy (szSwapFile, MAX_PATH, lpszFileName))
            && SUCCEEDED (StringCchCat (szSwapFile, MAX_PATH, TEXT(".old")))) {

            CloseHandle (hLocalFile);
            hLocalFile = INVALID_HANDLE_VALUE;

            DeleteFile (szSwapFile);
            MoveFile (lpszFileName, szSwapFile);

            hLocalFile = CreateFile (lpszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
                                     FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
            if (hLocalFile == INVALID_HANDLE_VALUE) {
                RETAILMSG(1, (TEXT("%s: Failed opening %s\r\n"), g_szProgName, lpszFileName));
                return hLocalFile;
            }
            fWriteMarker = TRUE;
        }
    }

    *pfWriteMarker = fWriteMarker;
    SetFilePointer(hLocalFile, 0, 0, FILE_END);

    return hLocalFile;
}

// Close the handle to close the file
static BOOL CloseFileLog(HANDLE hLocalFile)
{
    CloseHandle(hLocalFile);
    return TRUE;
}

// Log to an open file 
static BOOL SendFileLog(HANDLE hLocalFile, BYTE* pData, DWORD dwNumBytes)
{
    DWORD nBytesWritten = 0;
    
    if (!WriteFile(hLocalFile, (char*)pData, dwNumBytes, &nBytesWritten, NULL)
        || (nBytesWritten != dwNumBytes)) {
        return FALSE;
    }
    
    return TRUE;
}


//------------------------------------------------------------------------------
// TRANSPORT CHOOSER
//------------------------------------------------------------------------------

void GetTransport(CeLogTransport* pTransport)
{
    if (g_Params.nCeLogTransport == CELOGFLUSH_TRANSPORT_RAM) {
        // Using the RAM buffer means simply advancing the read pointer without
        // copying the data anywhere.  That way the most recent data is always
        // in the buffer.
        DEBUGMSG(1, (TEXT("%s: Keeping data in RAM mapping\r\n"), g_szProgName));

        memset(pTransport, 0, sizeof(CeLogTransport));
    
    } else if (g_Params.nCeLogTransport == CELOGFLUSH_TRANSPORT_LOCALFILE) {
        // The "transport" writes data to a local file on the device
        DEBUGMSG(1, (TEXT("%s: Writing data to local file\r\n"), g_szProgName));
        
        pTransport->Init  = InitFileLog;
        pTransport->Open  = OpenFileLog;
        pTransport->Send  = SendFileLog;
        pTransport->Close = CloseFileLog;
    
    } else {
        // Default to CESH
        DEBUGMSG(1, (TEXT("%s: Writing data to desktop file via CESH\r\n"), g_szProgName));
        
        pTransport->Init  = InitCESH;
        pTransport->Open  = OpenCESH;
        pTransport->Send  = SendCESH;
        pTransport->Close = CloseCESH;
    }
}



//------------------------------------------------------------------------------
// UI FUNCTIONS
//------------------------------------------------------------------------------
BOOL InitUI()
{
    // Nothing to do if no UI
    if (!g_Params.fUseUI || (g_pfnMessageBox != NULL)) {
        return TRUE;
    }

    // Use MessageBox for output, if available
    g_hCoreDLL = LoadLibrary(TEXT("coredll.dll"));
    if (g_hCoreDLL) {
        g_pfnMessageBox = (LPVOID)GetProcAddress(g_hCoreDLL, TEXT("MessageBoxW"));
    }

    return (g_hCoreDLL && g_pfnMessageBox);
}


// Send to debug output, and use UI if enabled.
void OutputMsg(
    __format_string const WCHAR* szFormat,
    ...
    )
{
    va_list arglist;
    WCHAR   szTemp[MAX_PATH];
    HRESULT hr;

    // Compose a single string using the input args
    va_start(arglist, szFormat);
    hr = StringCchVPrintfW(szTemp, MAX_PATH, szFormat, arglist);
    if (SUCCEEDED(hr)) {
        RETAILMSG(1, (szTemp));
        if (g_pfnMessageBox) {
            g_pfnMessageBox(NULL, szTemp, g_szProgName, MB_OK | MB_ICONINFORMATION);
        }
    }
}


BOOL CleanupUI()
{
    if (g_hCoreDLL) {
        FreeLibrary(g_hCoreDLL);
        g_pfnMessageBox = NULL;
    }

    return TRUE;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static VOID PrintHelp()
{
    // BUGBUG how to print to console or dialog box?
    NKDbgPrintfW(TEXT("USAGE:  %s [options]\r\n"), g_szProgName);
    NKDbgPrintfW(TEXT("options:  (All Numbers are decimal unless otherwise noted.)\r\n"));
    NKDbgPrintfW(TEXT("    -time <timeout>      FlushTimeout,    eg. -time 10000\r\n"));
    // BUGBUG it would be nice to show this only for oscapture.exe...
    NKDbgPrintfW(TEXT("    -save <count>        SavedFlushes,    eg. -save 15\r\n"));
    NKDbgPrintfW(TEXT("    -pri <priority>      ThreadPriority,  eg. -pri 248\r\n"));
    NKDbgPrintfW(TEXT("    -trans <transport>   Transport,       eg. -trans LocalFile\r\n"));
    NKDbgPrintfW(TEXT("    -n <filename>        FileName,        eg. -n \\Release\\celog.clg\r\n"));
    NKDbgPrintfW(TEXT("    -flags <hex flags>   FileFlags,       eg. -flags 0x00000001\r\n"));
    NKDbgPrintfW(TEXT("    -z <hex zones>       ZoneCE,          eg. -z 0x000043E3\r\n"));
    NKDbgPrintfW(TEXT("    -ui <0 or 1>         UseUI,           eg. -ui 1\r\n"));
    NKDbgPrintfW(TEXT("    -buf <hex size>      BufferSize,      eg. -buf 0x00100000  (ignored if CeLog is already loaded)\r\n"));
    NKDbgPrintfW(TEXT("    -filesize <hex size> FileSize,        eg. -filesize 0x00100000  (forces FileFlags=2)\r\n"));
    // BUGBUG it would be nice to show this only for oscapture.exe...
    NKDbgPrintfW(TEXT("    -c                   CaptureActivity, eg. -c\r\n"));
}


//------------------------------------------------------------------------------
// Read program parameters from the registry and set the corresponding globals.
//------------------------------------------------------------------------------
BOOL ReadParams()
{
    HKEY  hKey;
    DWORD dwType, dwValSize;
    DWORD dwTemp;
    WCHAR szBuffer[MAX_PATH];

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("System\\CeLog"),
                     0, 0, &hKey) != ERROR_SUCCESS) {
        DEBUGMSG(1, (TEXT("%s: Unable to open HKLM\\System\\CeLog, using default settings\r\n"), g_szProgName));
        return FALSE;
    }
        
    // FlushTimeout
    dwValSize = sizeof(DWORD);
    if ((RegQueryValueEx(hKey, TEXT("FlushTimeout"), NULL, &dwType,
                         (LPBYTE)&dwTemp, &dwValSize) == ERROR_SUCCESS)
        && (dwValSize == sizeof(DWORD)) && (dwType == REG_DWORD)) {
        
        g_Params.dwCeLogFlushTimeout = dwTemp;
    }
    
    // SavedFlushes
    dwValSize = sizeof(DWORD);
    if ((RegQueryValueEx(hKey, TEXT("SavedFlushes"), NULL, &dwType,
                         (LPBYTE)&dwTemp, &dwValSize) == ERROR_SUCCESS)
        && (dwValSize == sizeof(DWORD)) && (dwType == REG_DWORD)) {
        
        g_Params.dwCeLogSavedFlushes = dwTemp;
    }
    
    // ThreadPriority
    dwValSize = sizeof(DWORD);
    if ((RegQueryValueEx(hKey, TEXT("ThreadPriority"), NULL, &dwType,
                         (LPBYTE)&dwTemp, &dwValSize) == ERROR_SUCCESS)
        && (dwValSize == sizeof(DWORD)) && (dwType == REG_DWORD)
        && (dwTemp < 256)) {
        
        g_Params.nCeLogThreadPrio = (int)dwTemp;
    }
    
    // Transport
    dwValSize = MAX_PATH * sizeof(WCHAR);
    if ((RegQueryValueEx(hKey, TEXT("Transport"), NULL, &dwType,
                         (LPBYTE)szBuffer, &dwValSize) == ERROR_SUCCESS)
        && (dwType == REG_SZ)) {
        
        if (!wcsicmp(szBuffer, TEXT("RAM"))) {
            g_Params.nCeLogTransport = CELOGFLUSH_TRANSPORT_RAM;
        } else if (!wcsicmp(szBuffer, TEXT("LocalFile"))) {
            g_Params.nCeLogTransport = CELOGFLUSH_TRANSPORT_LOCALFILE;
        } else {
            // Default to CESH
            g_Params.nCeLogTransport = CELOGFLUSH_TRANSPORT_CESH;
        }
    }
    
    // FileName
    dwValSize = MAX_PATH * sizeof(WCHAR);
    if ((RegQueryValueEx(hKey, TEXT("FileName"), NULL, &dwType,
                         (LPBYTE)szBuffer, &dwValSize) == ERROR_SUCCESS)
        && (dwType == REG_SZ)) {
        
        dwValSize /= sizeof(WCHAR);
        if (dwValSize >= MAX_PATH) {
            dwValSize = MAX_PATH-1;
        }

        wcsncpy(g_Params.szCeLogFileName, szBuffer, dwValSize);
        g_Params.szCeLogFileName[dwValSize] = 0;  // Make sure it's null-terminated
    }
    
    // FileFlags
    dwValSize = sizeof(DWORD);
    if ((RegQueryValueEx(hKey, TEXT("FileFlags"), NULL, &dwType,
                         (LPBYTE)&dwTemp, &dwValSize) == ERROR_SUCCESS)
        && (dwValSize == sizeof(DWORD)) && (dwType == REG_DWORD)) {
        
        g_Params.dwFileFlags = dwTemp;
    }
    
    // ZoneCE
    dwValSize = sizeof(DWORD);
    if ((RegQueryValueEx(hKey, TEXT("ZoneCE"), NULL, &dwType,
                         (LPBYTE)&dwTemp, &dwValSize) == ERROR_SUCCESS)
        && (dwValSize == sizeof(DWORD)) && (dwType == REG_DWORD)) {
        
        g_Params.fSetZones = TRUE;
        g_Params.dwZoneCE  = dwTemp;
    }
    
    // UseUI
    dwValSize = sizeof(DWORD);
    if ((RegQueryValueEx(hKey, TEXT("UseUI"), NULL, &dwType,
                         (LPBYTE)&dwTemp, &dwValSize) == ERROR_SUCCESS)
        && (dwValSize == sizeof(DWORD)) && (dwType == REG_DWORD)) {
        
        g_Params.fUseUI = (dwTemp ? TRUE : FALSE);
    }
    
    // No need to read BufferSize from the registry, CeLog will get it from there
    
    // FileSize
    dwValSize = sizeof(DWORD);
    if ((RegQueryValueEx(hKey, TEXT("FileSize"), NULL, &dwType,
                         (LPBYTE)&dwTemp, &dwValSize) == ERROR_SUCCESS)
        && (dwValSize == sizeof(DWORD)) && (dwType == REG_DWORD)) {
        
        g_Params.dwFileSize = dwTemp;
        if (g_Params.dwFileSize) {
            g_Params.dwFileFlags &= ~CELOGFLUSH_FILEFLAG_NEVER_CLOSE;
            g_Params.dwFileFlags |= CELOGFLUSH_FILEFLAG_ALWAYS_CLOSE;
        }
    }
    
    RegCloseKey(hKey);

    return TRUE;
}


//------------------------------------------------------------------------------
// Get program parameters from the command line and set the corresponding globals.
//------------------------------------------------------------------------------
BOOL ParseCommandLine (
    int    argc,
    WCHAR *argv[],
    DWORD *pdwStartIndex    // Receives index from HKLM\init if started in boot sequence
    )
{
    DWORD dwTemp;
    int   argnum;

    for (argnum = 1; argnum < argc; argnum++) {

        if ((argv[argnum][0] == L'-')
            || (argv[argnum][0] == L'/')) {
            
            // Accepted character -- move past it to parse the rest of this param
            argv[argnum]++;

            if (!wcsicmp(L"h", argv[argnum]) || !wcsicmp(L"?", argv[argnum])) {
                // -h or -? to get help
                PrintHelp();
                return FALSE;
            
            } else if (!wcsicmp(L"time", argv[argnum])) {
                // FlushTimeout, -time <timeout>
                argnum++;
                if (argnum < argc) {
                    dwTemp = wcstoul(argv[argnum], NULL, 10);
                    if (dwTemp != 0) {
                        g_Params.dwCeLogFlushTimeout = dwTemp;
                    }
                } else {
                    PrintHelp();
                    return FALSE;
                }

            } else if (!wcsicmp(L"save", argv[argnum])) {
                // SavedFlushes, -save <count>
                argnum++;
                if (argnum < argc) {
                    dwTemp = wcstoul(argv[argnum], NULL, 10);
                    if (dwTemp != 0) {
                        g_Params.dwCeLogSavedFlushes = dwTemp;
                    }
                } else {
                    PrintHelp();
                    return FALSE;
                }

            } else if (!wcsicmp(L"pri", argv[argnum])) {
                // ThreadPriority, -pri <priority>
                argnum++;
                if (argnum < argc) {
                    dwTemp = wcstoul(argv[argnum], NULL, 10);
                    if ((dwTemp != 0) && (dwTemp < 256)) {
                        g_Params.nCeLogThreadPrio = (int)dwTemp;
                    }
                } else {
                    PrintHelp();
                    return FALSE;
                }

            } else if (!wcsicmp(L"trans", argv[argnum])) {
                // Transport, -trans <transport>
                argnum++;
                if (argnum < argc) {
                    if (!wcsicmp(TEXT("RAM"), argv[argnum])) {
                        g_Params.nCeLogTransport = CELOGFLUSH_TRANSPORT_RAM;
                    } else if (!wcsicmp(TEXT("LocalFile"), argv[argnum])) {
                        g_Params.nCeLogTransport = CELOGFLUSH_TRANSPORT_LOCALFILE;
                    } else if (!wcsicmp(TEXT("CESH"), argv[argnum])) {
                        g_Params.nCeLogTransport = CELOGFLUSH_TRANSPORT_CESH;
                    } else {
                        PrintHelp();
                        return FALSE;
                    }
                } else {
                    PrintHelp();
                    return FALSE;
                }

            } else if (!wcsicmp(L"n", argv[argnum])) {
                // FileName, -n <filename>
                argnum++;
                if (argnum < argc) {
                    WORD wLen = wcslen(argv[argnum]);
                    if (wLen < MAX_PATH) {
                        wcsncpy(g_Params.szCeLogFileName, argv[argnum], wLen);
                        g_Params.szCeLogFileName[wLen] = 0;
                    }
                } else {
                    PrintHelp();
                    return FALSE;
                }

            } else if (!wcsicmp(L"flags", argv[argnum])) {
                // FileFlags, -flags <hex flags>
                argnum++;
                if (argnum < argc) {
                    dwTemp = wcstoul(argv[argnum], NULL, 16);  // hex
                    g_Params.dwFileFlags = dwTemp;
                } else {
                    PrintHelp();
                    return FALSE;
                }

            } else if (!wcsicmp(L"z", argv[argnum])) {
                // ZoneCE, -z <hex zones>
                argnum++;
                if (argnum < argc) {
                    dwTemp = wcstoul(argv[argnum], NULL, 16);  // hex
                    // BUGBUG can be used to change zones but can't override registry
                    // setting and revert to CeLog defaults
                    g_Params.fSetZones = TRUE;
                    g_Params.dwZoneCE  = dwTemp;
                } else {
                    PrintHelp();
                    return FALSE;
                }

            } else if (!wcsicmp(L"ui", argv[argnum])) {
                // UseUI, -ui <0 or 1>
                argnum++;
                if (argnum < argc) {
                    dwTemp = wcstoul(argv[argnum], NULL, 16);  // hex
                    g_Params.fUseUI = dwTemp ? TRUE : FALSE;
                } else {
                    PrintHelp();
                    return FALSE;
                }

            } else if (!wcsicmp(L"buf", argv[argnum])) {
                // BufferSize, -buf <hex size>
                argnum++;
                if (argnum < argc) {
                    dwTemp = wcstoul(argv[argnum], NULL, 16);  // hex
                    g_Params.dwBufSize = dwTemp;
                } else {
                    PrintHelp();
                    return FALSE;
                }

            } else if (!wcsicmp(L"filesize", argv[argnum])) {
                // FileSize, -filesize <hex size>
                argnum++;
                if (argnum < argc) {
                    dwTemp = wcstoul(argv[argnum], NULL, 16);  // hex
                    g_Params.dwFileSize = dwTemp;
                    if (g_Params.dwFileSize) {
                        g_Params.dwFileFlags &= ~CELOGFLUSH_FILEFLAG_NEVER_CLOSE;
                        g_Params.dwFileFlags |= CELOGFLUSH_FILEFLAG_ALWAYS_CLOSE;
                    }
                } else {
                    PrintHelp();
                    return FALSE;
                }

            } else if (!wcsicmp(L"c", argv[argnum])) {
                // Capture, -c
                g_Params.fCapture = TRUE;
            
            } else {
                PrintHelp();
                return FALSE;
            }
        
        } else {
            // Command-line switch not beginning with "-" or "/"
            
            // Unrecognized initial string, this may be the start index for HKLM\Init
            if ((2 == argc) && (1 == argnum)
                // Try to convert to a number
                && (0 != (dwTemp = wcstoul(argv[argnum], NULL, 10)))) {
    
                *pdwStartIndex = dwTemp;
                return TRUE;
            } else {
                PrintHelp();
                return FALSE;
            }
        }
    
    }

    return TRUE;
}


//------------------------------------------------------------------------------
// Print final program parameters after parsing the command line and reading
// values from the registry.
//------------------------------------------------------------------------------
VOID PrintParams()
{
    // Print out the final program settings
    RETAILMSG(1, (TEXT("%s: FlushTimeout   = %ums\r\n"), g_szProgName, g_Params.dwCeLogFlushTimeout));
    RETAILMSG(1, (TEXT("%s: SavedFlushes   = %u\r\n"), g_szProgName, g_Params.dwCeLogSavedFlushes));
    RETAILMSG(1, (TEXT("%s: ThreadPriority = %u\r\n"), g_szProgName, g_Params.nCeLogThreadPrio));
    RETAILMSG(1, (TEXT("%s: Transport      = %s\r\n"), g_szProgName,
                  (g_Params.nCeLogTransport == CELOGFLUSH_TRANSPORT_CESH) ? TEXT("CESH") :
                  (g_Params.nCeLogTransport == CELOGFLUSH_TRANSPORT_LOCALFILE) ? TEXT("LocalFile") :
                  (g_Params.nCeLogTransport == CELOGFLUSH_TRANSPORT_RAM) ? TEXT("RAM") :
                  TEXT("Unknown")));
    RETAILMSG(1, (TEXT("%s: FileName       = %s\r\n"), g_szProgName, g_Params.szCeLogFileName));
    RETAILMSG(1, (TEXT("%s: FileFlags      = 0x%08X\r\n"), g_szProgName, g_Params.dwFileFlags));
    if (g_Params.fSetZones) {
        RETAILMSG(1, (TEXT("%s: ZoneCE         = 0x%08X\r\n"), g_szProgName, g_Params.dwZoneCE));
    }
    if (g_Params.dwBufSize) {
        RETAILMSG(1, (TEXT("%s: BufferSize     = 0x%08X\r\n"), g_szProgName, g_Params.dwBufSize));
    }
    if (g_Params.dwFileSize) {
        RETAILMSG(1, (TEXT("%s: FileSize       = 0x%08X\r\n"), g_szProgName, g_Params.dwFileSize));
    }
    RETAILMSG(1, (TEXT("%s: UseUI          = %u\r\n"), g_szProgName, g_Params.fUseUI));
}


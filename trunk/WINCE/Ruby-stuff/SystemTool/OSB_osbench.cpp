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
//------------------------------------------------------------------------------
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//  
//------------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include <stdarg.h>
#include <pkfuncs.h>
#include <nkintr.h>
#include <ceddk.h>
#include <OSB_osbench.h>

#ifdef DEBUG
DBGPARAM dpCurSettings = {
    TEXT("OSBENCH"), {
    TEXT("Errors"),TEXT("Warnings"),TEXT("Functions"),TEXT("Initialization"),
    TEXT("Threads"),TEXT("Commands"),TEXT("Undefined"),TEXT("Undefined"),
    TEXT("Undefined"),TEXT("Undefined"),TEXT("Undefined"),TEXT("Undefined"),
    TEXT("Undefined"),TEXT("Undefined"),TEXT("Undefined"),TEXT("Undefined") },
    0x1
};

#define ZONE_ERROR      DEBUGZONE(0)
#define ZONE_WARNING    DEBUGZONE(1)
#define ZONE_FUNCTION   DEBUGZONE(2)
#define ZONE_INIT       DEBUGZONE(3)
#define ZONE_THREADS    DEBUGZONE(4)
#define ZONE_COMMANDS   DEBUGZONE(5)

#endif  // DEBUG


// User interface variables (values can be changed by command line options).
int uiCounterFreq;                  // count register frequency 
int uiThreadPriority = THREAD_PRIORITY_TIME_CRITICAL;   // for interrupt service thread
int uiRunIdleThread = 1;            // run idle thread 
int uiNumInterrupts = 10;           // how many interrupts to use
int uiTimingInterval = 5;           // how many interrupts to use
BOOL g_fSecondaryProc = FALSE;
BOOL g_fShowAll = FALSE;                // show all data
BOOL g_fList = FALSE;                // show all data
BOOL fCacheSync = TRUE;
HANDLE g_hFile = INVALID_HANDLE_VALUE;  // file for printout
HANDLE hEvent1, hEvent2;
DWORD g_dwCountOverhead;            // Overhead in 100ns units
HANDLE hProcEventNotify = NULL;
HANDLE hProcEventAck = NULL;

LARGE_INTEGER g_liPerf1, g_liPerf2;

HANDLE g_hMappedBuffer;
PSHAREDMEM g_pBuffer;

#define ITERATIONS_DEFAULT  100
int g_nIterations = ITERATIONS_DEFAULT;
volatile DWORD* g_pdwMarker;

LARGE_INTEGER g_liPerfFreq;
DWORD g_dwCountOverhead;

static HANDLE hSynchEvent1, hSynchEvent2;    

const PFNTEST TestTable[] = {
    (PFNTEST) TestCriticalSections,
    (PFNTEST) TestEvents,
    (PFNTEST) TestSemaphores,
    (PFNTEST) TestMutexes,
    (PFNTEST) TestYield,
    (PFNTEST) TestPSL,
    (PFNTEST) TestInterlock,
};

#define NUM_TESTS   (sizeof(TestTable) / sizeof(PFNVOID))

BOOL TestEnabled[NUM_TESTS];

TCHAR g_szPlatform[32];

//------------------------------------------------------------------------------
//  Print a message to a file.
//------------------------------------------------------------------------------
int 
printW(
    LPWSTR lpszFmt, 
    ...
    )
{
    va_list ap;
    int count = 0;
    wchar_t buf[512];
    DWORD num;
    int i;

    if (g_hFile != INVALID_HANDLE_VALUE) {
        va_start(ap, lpszFmt);
        count = wvsprintf(buf, lpszFmt, ap);
        va_end(ap);
        
        for (i=0; i<count; i++) 
            ((unsigned char*)buf)[i] = (unsigned char) buf[i];
        WriteFile(g_hFile, buf, count, &num, 0);
    }   
    return count;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void 
printHelp(void)
{
    RETAILMSG(1, (TEXT("Usage: osbench [ options ]\r\n")));
    RETAILMSG(1, (TEXT("Options:\r\n")));
    RETAILMSG(1, (TEXT("  -all     Run all tests (default: run only those specified by -t option)\r\n")));
    RETAILMSG(1, (TEXT("  -t num   ID of test to run (need separate -t for each test)\r\n")));
    RETAILMSG(1, (TEXT("  -n num   Number of samples per test (default = %d)\r\n"), ITERATIONS_DEFAULT));
    RETAILMSG(1, (TEXT("  -m addr  Virtual address to write marker values to (default = <none>)\r\n")));
    RETAILMSG(1, (TEXT("  -list    List test ID's with descriptions\r\n")));
    RETAILMSG(1, (TEXT("  -v       Verbose : show all measurements\r\n")));
    RETAILMSG(1, (TEXT("  -o file  Output to CSV file (default: output only to debug)\r\n")));
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void
OB_PrintResults(
    DWORD  dwTestId,
    DWORD  dwTestSubId,
    BOOL   fInterprocess,
    DWORD  dwIterationsPerSample,
    DWORD  dwBaseResult,
    LPTSTR pszTitle
    )
{
    int i;
    DWORD dwMaxTime = 0;
    DWORD dwMinTime = (DWORD) -1;
    DWORD dwTime;
    DWORD dwAvgTime = 0;
    LARGE_INTEGER liDiff;
    OSVERSIONINFO sOsVersionInfo;

    RETAILMSG(1, (TEXT("===================================================================\r\n")));
    RETAILMSG(1, (TEXT("| %2d.%02d  |  IP = %3s  |  CS = %3s  |  %6d IPS\r\n"), 
                    dwTestId, dwTestSubId, 
                    fInterprocess ? TEXT("YES") : TEXT("NO"),
                    fCacheSync ? TEXT("YES") : TEXT("NO"),
                    dwIterationsPerSample));
    RETAILMSG(1, (TEXT("-------------------------------------------------------------------\r\n")));
    RETAILMSG(1, (TEXT("%s\r\n"), pszTitle));
    RETAILMSG(1, (TEXT("-------------------------------------------------------------------\r\n")));
    if (dwBaseResult) {
        RETAILMSG(1, (TEXT("|  Subtracting out base result of %d ticks\r\n"), dwBaseResult));
    }
    
    //
    // Start from '1' to skip the first measurement.
    // 
    for (i = 1; i < g_nIterations; i++) {
        dwTime = (DWORD) (g_pBuffer->liPerf2[i].QuadPart - g_pBuffer->liPerf1[i].QuadPart);
         
        //
        // Subtract off the base amount for this test type, if any (in raw ticks).
        //
        if (dwTime > dwBaseResult)
            dwTime -= dwBaseResult;
        else
            dwTime = 0;
        
        liDiff.QuadPart = dwTime;
        liDiff.QuadPart *= 1000000000;
        liDiff.QuadPart /= g_liPerfFreq.QuadPart;
        
        if (dwBaseResult == 0) {
            //
            // Subtract off the QueryPerformanceCounter overhead (in 1ns units).
            //
            if (liDiff.LowPart > g_dwCountOverhead)
                dwTime = liDiff.LowPart - g_dwCountOverhead;
            else
                dwTime = 0;
        } else {
            //
            // The normalization amount was provided explicitly for this test.
            //
            dwTime = liDiff.LowPart;
        }
        
        
        //
        // Get individual results, if many iterations were taken per sample.
        //
        if (dwIterationsPerSample > 1) {
            dwTime /= dwIterationsPerSample;
        }
        

        if (g_fShowAll) {
            if (g_hFile != INVALID_HANDLE_VALUE) {
               printW(TEXT("Time [%4d]= %10u.%02u us\r\n"), i, dwTime/1000, dwTime%1000);
            } else {
               RETAILMSG(1, (TEXT("Time [%4d]= %10u.%02u us\r\n"), i, dwTime/1000, dwTime%1000));
            }
        }
        
        dwAvgTime += dwTime;

        if (dwTime > dwMaxTime) dwMaxTime = dwTime;
        if (dwTime < dwMinTime) dwMinTime = dwTime;
    }
    
    dwAvgTime /= (g_nIterations - 1);

    RETAILMSG(1, (TEXT("|  Max Time = %10u.%03u us\r\n"), dwMaxTime/1000, dwMaxTime%1000));
    RETAILMSG(1, (TEXT("|  Min Time = %10u.%03u us\r\n"), dwMinTime/1000, dwMinTime%1000));
    RETAILMSG(1, (TEXT("|  Avg Time = %10u.%03u us\r\n"), dwAvgTime/1000, dwAvgTime%1000));
    RETAILMSG(1, (TEXT("===================================================================\r\n")));
        
    sOsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx( &sOsVersionInfo );
    printW(TEXT("%d,%s,%s,%s,%s,%d,%d,%d,%d,%d,%u.%03u,%u.%03u,%u.%03u,\"%s\"\r\n"),
           sOsVersionInfo.dwBuildNumber,
           g_szPlatform, TEXT(TGTCPU), TEXT(TGTPROJ), TEXT(WINCEDEBUG),
           dwTestId,
           dwTestSubId,
           fInterprocess,
           fCacheSync,
           dwIterationsPerSample,
           dwMinTime/1000, dwMinTime%1000, 
           dwMaxTime/1000, dwMaxTime%1000,
           dwAvgTime/1000, dwAvgTime%1000,
           pszTitle);
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
HANDLE
OB_SpinThread(
    PVOID pfnThread,
    DWORD dwParam,
    DWORD dwPriority
    )
{
    HANDLE hThread;
    DWORD dwThreadId;
            
    if (!(hThread = CreateThread(0, 0, pfnThread, (LPVOID) dwParam, CREATE_SUSPENDED, &dwThreadId)))
        return NULL;
    
    if (FALSE == CeSetThreadPriority(hThread, dwPriority)) {
        CloseHandle(hThread);
        return NULL;
    }
    
    //
    // Don't preempt our test threads. They are all designed to yield at the 
    // appropriate time.
    //
    if (FALSE == CeSetThreadQuantum(hThread, 0)) {
        CloseHandle(hThread);
        return NULL;
    }

    ResumeThread(hThread);
    return (hThread);
}




//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
BOOL
OB_SpinThreadAndBlock(
    PVOID pfnThread,
    DWORD dwParam,
    DWORD dwPriority
    )
{
    HANDLE hThread;
    
    hThread = OB_SpinThread(pfnThread, dwParam, dwPriority);
    if (hThread == NULL)
        return FALSE;

    //
    // Wait for thread to complete
    //
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    
    return TRUE;
}




//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void
OB_SynchThread1()
{
    SetEvent(hSynchEvent1);
    WaitForSingleObject(hSynchEvent2,INFINITE);
    if (fCacheSync) {
        CacheSync(CACHE_SYNC_DISCARD | CACHE_SYNC_INSTRUCTIONS);
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void
OB_SynchThread2()
{
    WaitForSingleObject(hSynchEvent1,INFINITE);
    SetEvent(hSynchEvent2);
    if (fCacheSync) {
        CacheSync(CACHE_SYNC_DISCARD | CACHE_SYNC_INSTRUCTIONS);
    }
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
BOOL
OB_SendCommand(
    DWORD dwTest,
    DWORD dwType,
    DWORD dwParam
    )
{
    //
    // Write the Command to the shared memory area.
    //
    g_pBuffer->dwTest   = dwTest;
    g_pBuffer->dwType   = dwType;
    g_pBuffer->dwParam  = dwParam;
    g_pBuffer->dwCacheSync = (DWORD) fCacheSync;

    DEBUGMSG(ZONE_CMD, (TEXT("SendCommand %d\r\n"), g_pBuffer->dwTest));
    SetEvent(hProcEventNotify);
    //
    // Yield and wait for the acknowledge
    //
    Sleep(0);
    WaitForSingleObject(hProcEventAck, INFINITE);
    
    return TRUE;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
BOOL
OB_DoCommand()
{
    BOOL fRet = FALSE;

    //
    // Wait for the server to be signalled
    //
    WaitForSingleObject(hProcEventNotify, INFINITE);
    
    if (g_pBuffer->dwTest == TESTID_QUIT) {
        DEBUGMSG(ZONE_CMD, (TEXT("TESTID_QUIT received\r\n")));
        goto EXIT;
    }

    DEBUGMSG(ZONE_CMD, (TEXT("DoCommand %d\r\n"), g_pBuffer->dwTest));
    
    fCacheSync = (BOOL) g_pBuffer->dwCacheSync;
    TestTable[g_pBuffer->dwTest](g_pBuffer->dwTest, g_pBuffer->dwType, g_pBuffer->dwParam);
    
    fRet = TRUE;
EXIT:
    SetEvent(hProcEventAck);
    return fRet;
}

#define OVERHEAD_ITERATIONS 1000
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void
OB_ComputeOverhead()
{
    LARGE_INTEGER liStartTime, liCurrentTime;
    int i;

    QueryPerformanceCounter(&liStartTime);
    for (i=0; i < OVERHEAD_ITERATIONS; i++) {
        QueryPerformanceCounter(&liCurrentTime);
    }
    QueryPerformanceCounter(&liCurrentTime);
    
    liCurrentTime.QuadPart = liCurrentTime.QuadPart - liStartTime.QuadPart;
    liCurrentTime.QuadPart /= OVERHEAD_ITERATIONS;
    //
    // Convert to 100ns units.
    //
    liCurrentTime.QuadPart *= 1000000000;
    liCurrentTime.QuadPart /= g_liPerfFreq.QuadPart;
    g_dwCountOverhead = (DWORD) liCurrentTime.QuadPart;
    
}




//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int WINAPI
OSBENCH_Main(
///    LPWSTR pszCmdLine
		HWND hDlg
    )
{
    int i;
    DWORD dwTickNS;

    //
    // Clear the TestEnable array.
    //
    for (i=0; i < NUM_TESTS; i++) {
        TestEnabled[i] = FALSE;
    }
    
    // Parse command line parameters (updates the ui variables).
    ///if (!parseCmdLine(pszCmdLine))  
    ///    return 0;
    
    ///RETAILMSG(1, (TEXT("OsBench %s\r\n"), pszCmdLine));
    
    if (g_fList) {
        for (i=0; i < NUM_TESTS; i++) {
            TestTable[i](i, TYPE_PRINT_HELP, 0);
        }
        return 0;
    }
    

    //
    // All instances need to setup a few global variables.
    //
    hProcEventNotify = CreateEvent(NULL, FALSE, FALSE, TEXT("OSBENCH_PROC_EVENT_NOTIFY"));
    hProcEventAck    = CreateEvent(NULL, FALSE, FALSE, TEXT("OSBENCH_PROC_EVENT_ACK"));
    
    hSynchEvent1 = CreateEvent(NULL, FALSE, FALSE, TEXT("OSBENCH_SYNCH_EVENT_1"));
    hSynchEvent2 = CreateEvent(NULL, FALSE, FALSE, TEXT("OSBENCH_SYNCH_EVENT_2"));
    
    
    if (hProcEventNotify == NULL) {
        ///RETAILMSG(1, (TEXT("Named event not created!\r\n")));
        ///RETAILMSG(1, (TEXT("Exitting OSBENCH process.\r\n")));
        MessageBox(hDlg, L"Named event not created!\r\nExitting OSBENCH process.\r\n", L"Error", MB_OK);
        return 0;
    }

    //
    // Create a shared memory-mapped object
    //
    g_hMappedBuffer = CreateFileMapping((HANDLE) -1, NULL, 
                                     PAGE_READWRITE | SEC_COMMIT, 0,
                                     sizeof(SHAREDMEM) + (2* MAX_ITERATIONS * sizeof(LARGE_INTEGER)), 
                                     TEXT ("OSBENCH_SHAREDMEM"));
    if (g_hMappedBuffer) {
        // Map in the object
        g_pBuffer = (PSHAREDMEM) MapViewOfFile(g_hMappedBuffer, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    }

    if (g_pBuffer == NULL) {
        RETAILMSG(1, (TEXT("Error allocating shared buffer\r\n")));
        return 0;
    }

    g_pBuffer->liPerf1 = (LARGE_INTEGER*) (g_pBuffer + 1);
    g_pBuffer->liPerf2 = (LARGE_INTEGER*) (g_pBuffer->liPerf1 + MAX_ITERATIONS);

    if (g_fSecondaryProc) {
        //
        // This is the secondary process. Simply wait to be terminated.
        //
        ///DEBUGMSG(ZONE_INIT, (TEXT("Starting Secondary OSBENCH process.\r\n")));
        
        g_nIterations = g_pBuffer->dwNumIterations;
        //
        // Acknowledge that we're up and running.
        //
        SetEvent(hProcEventAck);

        //
        // Server command processing loop
        //
        while (OB_DoCommand()) {
            //
            // Only break when OB_DoCommand returns FALSE indicating a TESTID_QUIT.
            //
        }
        
        ///DEBUGMSG(ZONE_INIT, (TEXT("Exitting Secondary OSBENCH process.\r\n")));
        
        if (hProcEventNotify != NULL) {
            CloseHandle(hProcEventNotify);
        }
        if (hProcEventAck != NULL) {
            CloseHandle(hProcEventAck);
        }
        return 0;
    }

    g_pBuffer->dwNumIterations = g_nIterations;

    //
    // Start a second process
    //
    CreateProcess(TEXT("osbench.exe"), TEXT("-z"), NULL, NULL, FALSE,
                  0, NULL, NULL, NULL, NULL);
    
    //
    // Wait for the secondary process to acknowledge startup!
    //
    if (WAIT_OBJECT_0 != WaitForSingleObject(hProcEventAck, 10000)) {
        RETAILMSG(1, (TEXT("Wait for acknowledge failed or timed out.\r\n")));
    }
    
    QueryPerformanceFrequency(&g_liPerfFreq);
    dwTickNS = (DWORD) ((__int64) 1000000000 / g_liPerfFreq.QuadPart);

    //
    // Get platform string
    //
    if (!SystemParametersInfo(SPI_GETPLATFORMTYPE, sizeof(g_szPlatform), g_szPlatform, FALSE))
        wcscpy(g_szPlatform, TEXT("Unknown"));

    OB_ComputeOverhead();


    RETAILMSG(1, (TEXT("\r\n")));
    RETAILMSG(1, (TEXT("\r\n")));
    RETAILMSG(1, (TEXT("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\r\n")));
    RETAILMSG(1, (TEXT("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\r\n")));
    RETAILMSG(1, (TEXT("===================================================================\r\n")));
    RETAILMSG(1, (TEXT("                INTERPRETING THE OSBENCH RESULTS                 \r\n")));
    RETAILMSG(1, (TEXT("-------------------------------------------------------------------\r\n")));
    RETAILMSG(1, (TEXT("  OSBench measures key operating system timings as closely as    \r\n")));
    RETAILMSG(1, (TEXT("  possible. All tests use the QueryPerformanceCounter (QPC) to get\r\n")));
    RETAILMSG(1, (TEXT("  time stamps. The frequency of the counter and the overhead of\r\n")));
    RETAILMSG(1, (TEXT("  calling this function must be taken into account when\r\n")));
    RETAILMSG(1, (TEXT("  analyzing the results.\r\n")));
    RETAILMSG(1, (TEXT("\r\n")));
    RETAILMSG(1, (TEXT("  Some operations take a very small time to complete. The overhead\r\n")));
    RETAILMSG(1, (TEXT("  of calling QPC becomes significant. In these case, the operation\r\n")));
    RETAILMSG(1, (TEXT("  is looped for a fixed number of iterations per sample (IPS). \r\n")));
    RETAILMSG(1, (TEXT("  The IPS is listed with each test. IPS of 1 means QPC is called\r\n")));
    RETAILMSG(1, (TEXT("  around only one instance of the operation. This will show\r\n")));
    RETAILMSG(1, (TEXT("  variation in the run times of the operation. For larger IPS,\r\n")));
    RETAILMSG(1, (TEXT("  the individual run times are averaged out, but the average\r\n")));
    RETAILMSG(1, (TEXT("  time is far more accurate. A noteable side effect of this\r\n")));
    RETAILMSG(1, (TEXT("  looping is that the cache cannot be flushed between each\r\n")));
    RETAILMSG(1, (TEXT("  iteration of the operation. For tests of IPS = 1, the test is\r\n")));
    RETAILMSG(1, (TEXT("  run with both a cache flush on each sample and without a cache\r\n")));
    RETAILMSG(1, (TEXT("  flush on each sample.\r\n")));
    RETAILMSG(1, (TEXT("\r\n")));
    RETAILMSG(1, (TEXT("  The cache sync   status is noted for each test (CS = YES/NO)\r\n")));
    RETAILMSG(1, (TEXT("  The interprocess status is noted for each test (IP = YES/NO)\r\n")));
    RETAILMSG(1, (TEXT("\r\n")));
    RETAILMSG(1, (TEXT("-------------------------------------------------------------------\r\n")));
    RETAILMSG(1, (TEXT("  SYSTEM BUILD INFO\r\n")));
    RETAILMSG(1, (TEXT("\r\n")));
    RETAILMSG(1, (TEXT("  PLATFORM   = %s\r\n"), g_szPlatform));
    RETAILMSG(1, (TEXT("  PROCESSOR  = %s\r\n"), TEXT(TGTCPU)));
    RETAILMSG(1, (TEXT("  CONFIG     = %s\r\n"), TEXT(TGTPROJ)));
    RETAILMSG(1, (TEXT("  RET/DEBUG  = %s\r\n"), TEXT(WINCEDEBUG)));
    RETAILMSG(1, (TEXT("  BUILD DATE = %s @ (%s)\r\n"), TEXT(__DATE__), TEXT(__TIME__)));
    RETAILMSG(1, (TEXT("-------------------------------------------------------------------\r\n")));
    RETAILMSG(1, (TEXT("  PERFORMANCE COUNTER INFO\r\n")));
    RETAILMSG(1, (TEXT("\r\n")));
    RETAILMSG(1, (TEXT("  Counter Frequency     = %u Hz\r\n"), g_liPerfFreq.LowPart));
    RETAILMSG(1, (TEXT("  Counter tick interval = %u ns\r\n"), dwTickNS));
    RETAILMSG(1, (TEXT("  QPC() call overhead   = %u.%03u usecs\r\n"), g_dwCountOverhead/1000, g_dwCountOverhead%1000));
    RETAILMSG(1, (TEXT("-------------------------------------------------------------------\r\n")));
    RETAILMSG(1, (TEXT("  RUNTIME OPTIONS\r\n")));
    RETAILMSG(1, (TEXT("\r\n")));
    RETAILMSG(1, (TEXT("  NUmber of samples per test = %d\r\n"), g_nIterations));
    RETAILMSG(1, (TEXT("===================================================================\r\n")));
    RETAILMSG(1, (TEXT("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\r\n")));
    RETAILMSG(1, (TEXT("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\r\n")));
    RETAILMSG(1, (TEXT("\r\n")));
    RETAILMSG(1, (TEXT("\r\n")));

    //
    // For CSV File output, first row is column descriptions (last is cmdline)
    //
    printW(TEXT("OS Version,Platform,CPU,Project,Debug?,Test ID,Sub ID,Interprocess?,CacheSync?,Iterations Per Sample,Min,Max,Avg,OsBench %s\r\n"), pszCmdLine);

    //
    // This is the main process. Start each of the tests.
    //
    for (i=0; i < NUM_TESTS; i++) {
        if (TestEnabled[i]) {
            fCacheSync = FALSE;
            TestTable[i](i, TYPE_PRIMARY, 0);
            fCacheSync = TRUE;
            TestTable[i](i, TYPE_PRIMARY, 0);
        }
    }

    OB_SendCommand(TESTID_QUIT, 0, 0);

    if (g_hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(g_hFile);
        g_hFile = INVALID_HANDLE_VALUE;
    }
    
    if (hProcEventNotify != NULL) {
        CloseHandle(hProcEventNotify);
    }
    if (hProcEventAck != NULL) {
        CloseHandle(hProcEventAck);
    }
    
    DEBUGMSG(ZONE_FUNCTION, (TEXT("-OsBench\r\n")));
    
    return 1;   
}


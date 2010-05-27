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
#include <osbench.h>

#define CRITSECT_ITERATIONS 1000
CRITICAL_SECTION g_TestCS;
static DWORD g_dwBase;
static HANDLE hEvent1, hEvent2;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DWORD 
OB_CriticalSectionThread1(
    LPVOID parm
    )
{
    int i;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_CriticalSectionThread1\r\n")));
    
    for (i=0; i< g_nIterations; i++) {
        EnterCriticalSection(&g_TestCS);
        OB_SynchThread1();
        QPC1(i);
        LeaveCriticalSection(&g_TestCS);
        POST1(i);
    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_CriticalSectionThread1\r\n")));
    return 0;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DWORD 
OB_CriticalSectionThread2(
    LPVOID parm
    )
{
    int i;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_CriticalSectionThread2\r\n")));
    
    for (i=0; i< g_nIterations; i++) {
        OB_SynchThread2();
        EnterCriticalSection(&g_TestCS);
        QPC2(i);
        LeaveCriticalSection(&g_TestCS);
    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_CriticalSectionThread2\r\n")));
    return 0;
}



//------------------------------------------------------------------------------
// HIGH PRIORITY
//------------------------------------------------------------------------------
DWORD 
OB_CriticalSectionThread3(
    LPVOID parm
    )
{
    int i;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_CriticalSectionThread3\r\n")));
    
    for (i=0; i< g_nIterations; i++) {
        OB_SynchThread1();
        //
        // Thread4 will run before us here (because of thread quantum)
        //
        SetEvent(hEvent1);
        //
        // Take time stamp. When we hit the EnterCS, we will block and the lower
        // priority thread will wake up and get the ending time stamp.
        //
        QPC1(i);
        EnterCriticalSection(&g_TestCS);
        LeaveCriticalSection(&g_TestCS);
        POST1(i);
        //
        // Sleep so that the lower priority thread can wrap around and block.
        //
        Sleep(5);
    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_CriticalSectionThread3\r\n")));
    return 0;
}



//------------------------------------------------------------------------------
// HIGH PRIORITY (blocker)
//------------------------------------------------------------------------------
DWORD 
OB_CriticalSectionThread4(
    LPVOID parm
    )
{
    int i;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_CriticalSectionThread4\r\n")));
    
    for (i=0; i< g_nIterations; i++) {
        OB_SynchThread2();
        //
        // We run first out of the SyncThread function. Take the critical section
        //
        EnterCriticalSection(&g_TestCS);
        //
        // Now block until the low priority thread releases us.
        //
        WaitForSingleObject(hEvent2,INFINITE);
        //
        // Release the critical section
        //
        LeaveCriticalSection(&g_TestCS);
        //
        // Sleep so that the lower priority thread can wrap around and block.
        //
        Sleep(50);
    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_CriticalSectionThread4\r\n")));
    return 0;
}



//------------------------------------------------------------------------------
// LOW PRIORITY
//------------------------------------------------------------------------------
DWORD 
OB_CriticalSectionThread5(
    LPVOID parm
    )
{
    int i;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_CriticalSectionThread5\r\n")));
    
    for (i=0; i< g_nIterations; i++) {
        //
        // Now block until the low priority thread releases us.
        //
        WaitForSingleObject(hEvent1,INFINITE);
        
        QPC2(i);
        //
        // Wake up the blocked higher priority thread.
        //
        SetEvent(hEvent2);
    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_CriticalSectionThread5\r\n")));
    return 0;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DWORD 
OB_CriticalSectionThread_Enter(
    LPVOID parm
    )
{
    int i, j;
    LARGE_INTEGER liRef1, liRef2;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_CriticalSectionThread_Enter\r\n")));
    
    //----------------------------------------------------------
    // Reference
    //
    QueryPerformanceCounter(&liRef1);
    for (j = 0; j < CRITSECT_ITERATIONS; j++) {
        //
        // Just measuring the loop overhead and the overhead of
        // calling the QueryPerformanceCounter() function.
        //
        SUBMARKER1();
        SUBMARKER2();
    }
    QueryPerformanceCounter(&liRef2);
    g_dwBase = (DWORD) (liRef2.QuadPart - liRef1.QuadPart);
    
    for (i=0; i< g_nIterations; i++) {
        QPC1(i);
        for (j = 0; j < CRITSECT_ITERATIONS; j++) {
            SUBMARKER1();
            EnterCriticalSection(&g_TestCS);
            SUBMARKER2();
        }
        QPC2(i);
        POST1(i);
        
        //
        // Release the critical section
        //
        for (j = 0; j < CRITSECT_ITERATIONS; j++) {
            LeaveCriticalSection(&g_TestCS);
        }
    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_CriticalSectionThread_Enter\r\n")));
    return 0;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DWORD 
OB_CriticalSectionThread_Leave(
    LPVOID parm
    )
{
    int i, j;
    LARGE_INTEGER liRef1, liRef2;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_CriticalSectionThread_Leave\r\n")));
    
    //----------------------------------------------------------
    // Reference
    //
    QueryPerformanceCounter(&liRef1);
    for (j = 0; j < CRITSECT_ITERATIONS; j++) {
        //
        // Just measuring the loop overhead and the overhead of
        // calling the QueryPerformanceCounter() function.
        //
        SUBMARKER1();
        SUBMARKER2();
    }
    QueryPerformanceCounter(&liRef2);
    g_dwBase = (DWORD) (liRef2.QuadPart - liRef1.QuadPart);
    
    for (i=0; i< g_nIterations; i++) {
        for (j = 0; j < CRITSECT_ITERATIONS; j++) {
            EnterCriticalSection(&g_TestCS);
        }
        
        //
        // Release the critical section
        //
        QPC1(i);
        for (j = 0; j < CRITSECT_ITERATIONS; j++) {
            SUBMARKER1();
            LeaveCriticalSection(&g_TestCS);
            SUBMARKER2();
        }
        QPC2(i);
        POST1(i);
    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_CriticalSectionThread_Leave\r\n")));
    return 0;
}



static const LPTSTR szTest0 = \
    TEXT("EnterCriticalSection traditional (blocking) with priority inversion :\r\n") \
    TEXT("Time from a lower priority thread calling LeaveCS to a higher priority\r\n") \
    TEXT("thread unblocking from the EnterCS call");

static const LPTSTR szTest1 = \
    TEXT("EnterCriticalSection traditional (blocking) without priority inversion :\r\n") \
    TEXT("Time from a higher priority thread calling EnterCS (blocked) to a lower \r\n") \
    TEXT("priority runnable thread getting run");

static const LPTSTR szTest2 = \
    TEXT("EnterCriticalSection fastpath (uncontested)");

static const LPTSTR szTest3 = \
    TEXT("LeaveCriticalSection fastpath (uncontested)");

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void
TestCriticalSections(
    DWORD dwTestId,
    DWORD dwType,
    DWORD dwParam
    )
{
    HANDLE hThread3, hThread2, hThread1;

    DEBUGMSG(ZONE_FUNCTION, (TEXT("TestCriticalSections (%d, %d)\r\n"), dwType, dwParam));
    
    hEvent1 = CreateEvent(NULL, FALSE, FALSE, TEXT("OSBENCH_TEST_EVENT_1"));
    hEvent2 = CreateEvent(NULL, FALSE, FALSE, TEXT("OSBENCH_TEST_EVENT_2"));
    
    switch (dwType) {
        
        case TYPE_PRIMARY : {

            InitializeCriticalSection(&g_TestCS);
        
            //
            // Run the test intra-process
            //
            hThread2 = OB_SpinThread((PVOID) OB_CriticalSectionThread2, 0, RT_PRIO_0);
            hThread1 = OB_SpinThread((PVOID) OB_CriticalSectionThread1, 0, RT_PRIO_1);
            
            //
            // Wait for both threads to complete
            //
            WaitForSingleObject(hThread1, INFINITE);
            CloseHandle(hThread1);
            WaitForSingleObject(hThread2, INFINITE);
            CloseHandle(hThread2);
            
            OB_PrintResults(dwTestId, 0, OB_INTRAPROCESS, 1, 0, szTest0);
            
            //
            // Run the test intra-process
            //
            hThread3 = OB_SpinThread((PVOID) OB_CriticalSectionThread5, 0, RT_PRIO_1);
            hThread2 = OB_SpinThread((PVOID) OB_CriticalSectionThread4, 0, RT_PRIO_0);
            hThread1 = OB_SpinThread((PVOID) OB_CriticalSectionThread3, 0, RT_PRIO_0);
            
            //
            // Wait for both threads to complete
            //
            WaitForSingleObject(hThread1, INFINITE);
            CloseHandle(hThread1);
            WaitForSingleObject(hThread2, INFINITE);
            CloseHandle(hThread2);
            WaitForSingleObject(hThread3, INFINITE);
            CloseHandle(hThread3);
            
            OB_PrintResults(dwTestId, 1, OB_INTRAPROCESS, 1, 0, szTest1);
            
            //
            // Uncontested results are looped (because they are so fast), so 
            // we can't do the cache flush for these measurements.
            //
            if (!fCacheSync) {
                //--------------------------------------------------------------
                // Get the uncontested (fastpath) results for EnterCriticalSection
                //
                OB_SpinThreadAndBlock((PVOID) OB_CriticalSectionThread_Enter, 0, RT_PRIO_0);
                
                OB_PrintResults(dwTestId, 2, OB_INTRAPROCESS, CRITSECT_ITERATIONS, 
                                g_dwBase, szTest2);
                
                //--------------------------------------------------------------
                // Get the uncontested (fastpath) results for LeaveCriticalSection
                //
                OB_SpinThreadAndBlock((PVOID) OB_CriticalSectionThread_Leave, 0, RT_PRIO_0);
                
                OB_PrintResults(dwTestId, 3, OB_INTRAPROCESS, CRITSECT_ITERATIONS, 
                                g_dwBase, szTest3);
            }
            DeleteCriticalSection(&g_TestCS);
            break;
        }
        
        case TYPE_PRINT_HELP : {
           RETAILMSG(1, (TEXT("TestId %2d : CriticalSections\r\n"), dwTestId));
           break;
        }
        
        default : {
           RETAILMSG(1, (TEXT("TestId %2d : Unknown command type %d\r\n"), dwTestId, dwType));
           break;
        }
    }

    CloseHandle(hEvent1);
    CloseHandle(hEvent2);
}




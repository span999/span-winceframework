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

HANDLE hEvent1;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DWORD 
OB_YieldThread1(
    LPVOID parm
    )
{
    int i;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_YieldThread1\r\n")));
    
    for (i = 0; i < g_nIterations; i++) {
        
        if (fCacheSync) {
            CacheSync(CACHE_SYNC_DISCARD | CACHE_SYNC_INSTRUCTIONS);
        }
        
        SetEvent(hEvent1);
        //
        // Signalled the other thread, but we're still running. Switch to
        // the other thread now!
        //
        Sleep(0);
        QPC2(i);
        //
        // Yield back to T2 so it can block on the event again.
        //
        Sleep(0);
    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_YieldThread1\r\n")));
    return 0;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DWORD 
OB_YieldThread2(
    LPVOID parm
    )
{
    int i;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_YieldThread2\r\n")));
    
    //
    // Thread 2 must start first.
    //
    for (i=0; i< g_nIterations; i++) {
        
        if (fCacheSync) {
            CacheSync(CACHE_SYNC_DISCARD | CACHE_SYNC_INSTRUCTIONS);
        }
        
        WaitForSingleObject(hEvent1,INFINITE);
        QPC1(i);
        Sleep(0);
        POST1(i);
    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_YieldThread2\r\n")));
    return 0;
}


static const LPTSTR szTest0 = \
    TEXT("Yield to thread timing intra-process :\r\n") \
    TEXT("Time from a thread calling Sleep(0) to a thread in the same process\r\n") \
    TEXT("of the same priority which previously called Sleep(0)");

static const LPTSTR szTest1 = \
    TEXT("Yield to thread timing inter-process :\r\n") \
    TEXT("Time from a thread calling Sleep(0) to a thread in a different process\r\n") \
    TEXT("of the same priority which previously called Sleep(0)");


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void
TestYield(
    DWORD dwTestId,
    DWORD dwType,
    DWORD dwParam
    )
{
    HANDLE hThread1, hThread2;

    DEBUGMSG(ZONE_FUNCTION, (TEXT("TestYields (%d, %d)\r\n"), dwType, dwParam));

    hEvent1 = CreateEvent(NULL, FALSE, FALSE, TEXT("OSBENCH_TEST_EVENT_1"));
    
    switch (dwType) {
        
        case TYPE_PRIMARY : {
            //
            // Run the test once intra-process
            //
            hThread2 = OB_SpinThread((PVOID) OB_YieldThread2, 0, RT_PRIO_0);
            hThread1 = OB_SpinThread((PVOID) OB_YieldThread1, 0, RT_PRIO_0);
            
            //
            // Wait for both threads to complete
            //
            WaitForSingleObject(hThread1, INFINITE);
            CloseHandle(hThread1);
            WaitForSingleObject(hThread2, INFINITE);
            CloseHandle(hThread2);

            OB_PrintResults(dwTestId, 0, OB_INTRAPROCESS, 1, 0, szTest0);
            
            //
            // Run the test again inter-process
            //
            hThread2 = OB_SpinThread((PVOID) OB_YieldThread2, 0, RT_PRIO_0);
            OB_SendCommand (dwTestId, TYPE_SECONDARY, 0);
            
            //
            // Wait for second thread to complete
            //
            WaitForSingleObject(hThread2, INFINITE);
            CloseHandle(hThread2);
            
            OB_PrintResults(dwTestId, 0, OB_INTERPROCESS, 1, 0, szTest1);
            break;
        }

        case TYPE_SECONDARY : {
            //
            // This is the second process
            //
            hThread1 = OB_SpinThread((PVOID) OB_YieldThread1, 0, RT_PRIO_0);
            WaitForSingleObject(hThread1, INFINITE);
            CloseHandle(hThread1);
            break;
        }
        
        case TYPE_PRINT_HELP : {
           RETAILMSG(1, (TEXT("TestId %2d : Voluntary yield\r\n"), dwTestId));
           break;
        }
        
        default : {
           RETAILMSG(1, (TEXT("TestId %2d : Unknown command type %d\r\n"), dwTestId, dwType));
           break;
        }
    }

    CloseHandle(hEvent1);
}




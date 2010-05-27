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

static HANDLE hMutex;


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DWORD 
OB_MutexThread1(
    LPVOID parm
    )
{
    int i;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_MutexThread1\r\n")));
    
    for (i=0; i< g_nIterations; i++) {
        WaitForSingleObject(hMutex,INFINITE);
        OB_SynchThread1();
        QPC1(i);
        ReleaseMutex(hMutex);
        POST1(i);
    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_MutexThread1\r\n")));
    return 0;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DWORD 
OB_MutexThread2(
    LPVOID parm
    )
{
    int i;
    
    DEBUGMSG(ZONE_THREAD, (TEXT("+OB_MutexThread2\r\n")));
    
    for (i=0; i< g_nIterations; i++) {
        OB_SynchThread2();
        WaitForSingleObject(hMutex,INFINITE);
        QPC2(i);
        ReleaseMutex(hMutex);

    }
    
    DEBUGMSG(ZONE_THREAD, (TEXT("-OB_MutexThread2\r\n")));
    return 0;
}


static const LPTSTR szTest0 = \
    TEXT("Mutex intra-process :\r\n") \
    TEXT("Time from a lower priority thread calling ReleaseMutex to a higher priority\r\n") \
    TEXT("thread in the same process unblocking from a WaitForSingleObject call");

static const LPTSTR szTest1 = \
    TEXT("Mutex inter-process :\r\n") \
    TEXT("Time from a lower priority thread calling ReleaseMutex to a higher priority\r\n") \
    TEXT("thread in a different process unblocking from a WaitForSingleObject call");


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void
TestMutexes(
    DWORD dwTestId,
    DWORD dwType,
    DWORD dwParam
    )
{
    HANDLE hThread1, hThread2;

    DEBUGMSG(ZONE_FUNCTION, (TEXT("TestMutexes (%d, %d)\r\n"), dwType, dwParam));

    hMutex = CreateMutex(NULL, FALSE, TEXT("OSBENCH_TEST_MUTEX_1"));

    switch (dwType) {
        
        case TYPE_PRIMARY : {
            //
            // Run the test once intra-process
            //
            hThread2 = OB_SpinThread((PVOID) OB_MutexThread2, 0, RT_PRIO_0);
            hThread1 = OB_SpinThread((PVOID) OB_MutexThread1, 0, RT_PRIO_1);
            
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
            hThread2 = OB_SpinThread((PVOID) OB_MutexThread2, 0, RT_PRIO_0);
            OB_SendCommand (dwTestId, TYPE_SECONDARY, 0);
            
            //
            // Wait for thread to complete
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
            hThread1 = OB_SpinThread((PVOID) OB_MutexThread1, 0, RT_PRIO_1);
            
            //
            // Wait for thread to complete
            //
            WaitForSingleObject(hThread1, INFINITE);
            CloseHandle(hThread1);
            break;
        }
        
        case TYPE_PRINT_HELP : {
           RETAILMSG(1, (TEXT("TestId %2d : Mutex\r\n"), dwTestId));
           break;
        }
        
        default : {
           RETAILMSG(1, (TEXT("TestId %2d : Unknown command type %d\r\n"), dwTestId, dwType));
           break;
        }
    }

    CloseHandle(hMutex);
}




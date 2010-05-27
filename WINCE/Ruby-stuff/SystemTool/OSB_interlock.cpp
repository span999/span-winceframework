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

#define INTERLOCK_ITERATIONS  1000


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void
TestInterlock(
    DWORD dwTestId,
    DWORD dwType,
    DWORD dwParam
    )
{
    int i, j;
    long lNum;
    LARGE_INTEGER liRef1, liRef2;
    DWORD dwBase;

    DEBUGMSG(ZONE_FUNCTION, (TEXT("TestInterlock (%d, %d)\r\n"), dwType, dwParam));

    switch (dwType) {
        
        case TYPE_PRIMARY : {
            
            if (fCacheSync) {
                //
                // Results are too small to get accurate values while flushing
                // the cache for each iteration.
                //
                return;
            }
            
            CeSetThreadPriority(GetCurrentThread(), RT_PRIO_0);
            
            //----------------------------------------------------------
            // Reference
            //
            QueryPerformanceCounter(&liRef1);
            for (j = 0; j < INTERLOCK_ITERATIONS; j++) {
                //
                // Just measuring the loop overhead and the overhead of
                // calling the QueryPerformanceCounter() function.
                //
                SUBMARKER1();
                SUBMARKER2();
            }
            QueryPerformanceCounter(&liRef2);
            dwBase = (DWORD) (liRef2.QuadPart - liRef1.QuadPart);
        
            //----------------------------------------------------------
            // InterlockedIncrement
            //
            for (i=0; i< g_nIterations; i++) {
                QPC1(i);
                for (j = 0; j < INTERLOCK_ITERATIONS; j++) {
                    SUBMARKER1();
                    InterlockedIncrement(&lNum);
                    SUBMARKER2();
                }
                QPC2(i);
                POST1(i);
            }
            
            OB_PrintResults(dwTestId, 0, 0, INTERLOCK_ITERATIONS, 
                            dwBase, TEXT("InterlockedIncrement"));
            
            //----------------------------------------------------------
            // InterlockedDecrement
            //
            for (i=0; i< g_nIterations; i++) {
                QPC1(i);
                for (j = 0; j < INTERLOCK_ITERATIONS; j++) {
                    SUBMARKER1();
                    InterlockedDecrement(&lNum);
                    SUBMARKER2();
                }
                QPC2(i);
                POST1(i);
            }
            
            OB_PrintResults(dwTestId, 1, 0, INTERLOCK_ITERATIONS,
                            dwBase, TEXT("InterlockedDecrement"));
            
            //----------------------------------------------------------
            // InterlockedExchange
            //
            for (i=0; i< g_nIterations; i++) {
                QPC1(i);
                for (j = 0; j < INTERLOCK_ITERATIONS; j++) {
                    SUBMARKER1();
                    InterlockedExchange(&lNum, 14);
                    SUBMARKER2();
                }
                QPC2(i);
                POST1(i);
            }
            
            OB_PrintResults(dwTestId, 2, 0, INTERLOCK_ITERATIONS,
                            dwBase, TEXT("InterlockedExchange"));
            
            //----------------------------------------------------------
            // InterlockedTestExchange
            //
            for (i=0; i< g_nIterations; i++) {
                QPC1(i);
                for (j = 0; j < INTERLOCK_ITERATIONS; j++) {
                    SUBMARKER1();
                    InterlockedTestExchange(&lNum, 14, 14);
                    SUBMARKER2();
                }
                QPC2(i);
                POST1(i);
            }
            OB_PrintResults(dwTestId, 3, 0, INTERLOCK_ITERATIONS,
                            dwBase, TEXT("InterlockedTestExchange"));
            
            //
            // Return to normal priority.
            //
            SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
            break;
        }
        
        case TYPE_PRINT_HELP : {
           RETAILMSG(1, (TEXT("TestId %2d : Interlocked API's (decrement, increment, testexchange, exchange)\r\n"), dwTestId));
           break;
        }
        
        default : {
           RETAILMSG(1, (TEXT("TestId %2d : Unknown command type %d\r\n"), dwTestId, dwType));
           break;
        }
    }
}




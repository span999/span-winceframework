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
#ifndef _OSBENCH_H_
#define _OSBENCH_H_

typedef void (*PFNTEST)(DWORD dwTestId, DWORD dwType, DWORD dwParam);

#define TESTID_QUIT         (DWORD) -1L

#define TYPE_PRIMARY        0
#define TYPE_SECONDARY      1
#define TYPE_PRINT_HELP     2

#define RT_PRIO_0           0
#define RT_PRIO_1           1
#define RT_PRIO_2           2

#define OB_INTERPROCESS     1
#define OB_INTRAPROCESS     0

#define MAX_ITERATIONS 10000

// #define NO_HARDWARE_VERIFY

#ifndef NO_HARDWARE_VERIFY
#define MARKER1()       {if (g_pdwMarker) { *g_pdwMarker = 0xCBCB0001; }}
#define MARKER2()       {if (g_pdwMarker) { *g_pdwMarker = 0xCBCB0002; }}
#define SUBMARKER1()    {if (g_pdwMarker) { *g_pdwMarker = 0xCBCB0011; }}
#define SUBMARKER2()    {if (g_pdwMarker) { *g_pdwMarker = 0xCBCB0012; }}
#else
#define MARKER1()
#define MARKER2()
#define SUBMARKER1()
#define SUBMARKER2()
#endif


#define QPC1(n)     {QueryPerformanceCounter(&g_liPerf1); MARKER1();}
#define POST1(n)    {g_pBuffer->liPerf1[n].QuadPart = g_liPerf1.QuadPart;}

#define QPC2(n)     {MARKER2(); QueryPerformanceCounter(&g_liPerf2); g_pBuffer->liPerf2[n].QuadPart = g_liPerf2.QuadPart;}

typedef struct tSHAREDMEM {
    DWORD dwTest;
    DWORD dwType;
    DWORD dwParam;
    DWORD dwCacheSync;
    DWORD dwNumIterations;
    DWORD dwPadding;
    LARGE_INTEGER* liPerf1;
    LARGE_INTEGER* liPerf2;
} SHAREDMEM, *PSHAREDMEM;

extern volatile DWORD* g_pdwMarker;
extern PSHAREDMEM g_pBuffer;
extern int  g_nIterations;
extern BOOL fCacheSync;
extern LARGE_INTEGER g_liPerf1, g_liPerf2;

extern BOOL CreateOsBenchAPISet();

extern void TestCriticalSections(DWORD, DWORD, DWORD);
extern void TestEvents(DWORD, DWORD, DWORD);
extern void TestSemaphores(DWORD, DWORD, DWORD);
extern void TestMutexes(DWORD, DWORD, DWORD);
extern void TestYield(DWORD, DWORD, DWORD);
extern void TestPSL(DWORD, DWORD, DWORD);
extern void TestInterlock(DWORD, DWORD, DWORD);

extern void
OB_PrintResults(
    DWORD  dwTestId,
    DWORD  dwTestSubId,
    BOOL   fInterprocess,
    DWORD  dwIterationsPerSample,
    DWORD  dwBaseResult,
    LPTSTR pszTitle
    );

extern HANDLE OB_SpinThread(        PVOID pfnThread,DWORD dwParam, DWORD dwPriority);
extern BOOL   OB_SpinThreadAndBlock(PVOID pfnThread,DWORD dwParam, DWORD dwPriority);

extern BOOL OB_SendCommand(DWORD dwCommand,DWORD dwParam1,DWORD dwParam2);

void OB_SynchThread1();
void OB_SynchThread2();


int printW(LPWSTR lpszFmt, ...);


#ifdef DEBUG
#define ZONE_ERROR      DEBUGZONE(0)
#define ZONE_WARNING    DEBUGZONE(1)
#define ZONE_FUNCTION   DEBUGZONE(2)
#define ZONE_INIT       DEBUGZONE(3)
#define ZONE_THREAD     DEBUGZONE(4)
#define ZONE_CMD        DEBUGZONE(5)
#endif  // DEBUG


// Want to define RETAILMSG even for ship build
#undef RETAILMSG
#define RETAILMSG(cond,printf_exp)      ((cond)?(NKDbgPrintfW printf_exp),1:0)


#endif

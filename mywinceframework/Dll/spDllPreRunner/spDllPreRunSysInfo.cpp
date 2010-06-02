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
#include "spOS.h"
#include "spPlatform.h"
#include "spCommon.h"
#include "SPDebugDef.h"
#include "spLibDDrawClass.h"
#include "spLibSysInfoIdle.h"
#include "spLibSysInfoMemory.h"


#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
#define		LIBMSGFLAG_NK		(dNOUT|0x0FFFFFFF)		///NK msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only

#define		SPPREFIX			TEXT("ARPre:")


#define CPULOADDRAW_X_START		50
#define CPULOADDRAW_Y_START		0

#define CPULOADDRAW_WIDTH		100
#define CPULOADDRAW_HIGHT		4

///#define CPULOADDRAW_X_SCALE		5
#define CPULOADDRAW_X_SCALE		4
#define CPULOADDRAW_Y_SCALE		3

#define CPULOADDRAW_X_END		(CPULOADDRAW_X_START+(CPULOADDRAW_WIDTH*CPULOADDRAW_X_SCALE))
#define CPULOADDRAW_Y_END		(CPULOADDRAW_Y_START+(CPULOADDRAW_HIGHT*CPULOADDRAW_Y_SCALE))


#define CPULOADDRAW_R_BACKGROUND	0x0
#define CPULOADDRAW_G_BACKGROUND	0xFF
#define CPULOADDRAW_B_BACKGROUND	0x0

#define CPULOADDRAW_R_FOREGROUND	0xFF
#define CPULOADDRAW_G_FOREGROUND	0x0
#define CPULOADDRAW_B_FOREGROUND	0x0

#define CPULOADDRAW_R_SCALEDOT		0x0
#define CPULOADDRAW_G_SCALEDOT		0x0
#define CPULOADDRAW_B_SCALEDOT		0x0





typedef struct _CPUUtilizationDrawArea
{
    DWORD	dwXbase;
    DWORD	dwYbase;
    DWORD	dwStartX;
    DWORD	dwStartY;
    DWORD	dwXscale;
    DWORD	dwYscale;
    DWORD	dwEndX;
    DWORD	dwEndY;
} CPUUtilizationDrawArea, *PCPUUtilizationDrawArea;



static void DrawCPUMemLoading( DWORD dwValue );
void InitSysInfoDrawArea( void );

static CPUUtilizationDrawArea	DrawArea;
static PCPUUtilizationDrawArea	pDrawArea = &DrawArea;
static HANDLE	hStartEvent = NULL;
static HANDLE	hExitEvent = NULL;
static HANDLE	hResetEvent = NULL;
static BOOL 	bExitRoutine = FALSE;
static spLibDDraw	*pmyDD = NULL;


/// *****************************************
/// routine
/// *****************************************

void DrawCPUMem( BOOL bDraw )
{
	if( bDraw )
		DrawCPUMemLoading( 100 - spLibSysInfoIdle_Get() );
	else
		spLibSysInfoIdle_Get();
}

void InitSysInfoDrawArea( spLibDDraw *pDD )
{
	pDrawArea = &DrawArea;

	
	if( NULL != pDD )
		pmyDD = pDD;
	
	memset( pDrawArea, 0, sizeof(DrawArea) );
	
	pDrawArea->dwXbase = CPULOADDRAW_WIDTH;
	pDrawArea->dwYbase = CPULOADDRAW_HIGHT;
	pDrawArea->dwStartX = CPULOADDRAW_X_START;
	pDrawArea->dwStartY = CPULOADDRAW_Y_START;
	pDrawArea->dwXscale = CPULOADDRAW_X_SCALE;
	pDrawArea->dwYscale = CPULOADDRAW_Y_SCALE;
	pDrawArea->dwEndX = CPULOADDRAW_X_END;
	pDrawArea->dwEndY = CPULOADDRAW_Y_END;

	if( 500 < GetSystemMetrics( SM_CXSCREEN ) )
		pDrawArea->dwXscale = 5;
				
	pDrawArea->dwEndX = (pDrawArea->dwStartX+(pDrawArea->dwXbase*pDrawArea->dwXscale));
	pDrawArea->dwEndY = (pDrawArea->dwStartY+(pDrawArea->dwYbase*pDrawArea->dwYscale));;

	spLibSysInfoIdle_Init( 0 );
	spLibSysInfoMemory_Init( 0 );	

}

void DeInitSysInfoDrawArea( void )
{
	pDrawArea = &DrawArea;

	pmyDD = NULL;
	
	memset( pDrawArea, 0, sizeof(DrawArea) );
	
	spLibSysInfoIdle_Deinit( 0 );
	spLibSysInfoMemory_Deinit( 0 );	

}


static void DrawCPUMemLoading( DWORD dwValue )
{
	DWORD dwLoop = 0;
	static DWORD dwTimes = 0;
	RECT rcDest;
	DWORD dwNum = 0;

	///spMessageBoxOut( dINIT, TEXT("DrawCPULoading+++") );	
	SPDMSG( dINIT, (TEXT("DrawCPUMemLoading+++")) );	
	
	if( NULL != pmyDD && NULL != pDrawArea )
	{
		LibSysInfoMemoryContent mmi;
		DWORD dwRet;
		
		memset( &mmi, 0, sizeof(LibSysInfoMemoryContent) );
		dwRet = spLibSysInfoMemory_Get( &mmi );
		
		///draw backgound
		SetRect( &rcDest, pDrawArea->dwStartX, pDrawArea->dwStartY, pDrawArea->dwEndX, pDrawArea->dwEndY );
		pmyDD->spLibBltDDraw( &rcDest, CPULOADDRAW_R_BACKGROUND, CPULOADDRAW_G_BACKGROUND, CPULOADDRAW_B_BACKGROUND );		///green as Backgound

		///draw percentage
		if( dwValue > 0 )	///only draw when we have!!
		{
			SetRect( &rcDest, pDrawArea->dwStartX, pDrawArea->dwStartY, (pDrawArea->dwStartX+(dwValue*pDrawArea->dwXscale)), pDrawArea->dwEndY );
			pmyDD->spLibBltDDraw( &rcDest, CPULOADDRAW_R_FOREGROUND, CPULOADDRAW_G_FOREGROUND, CPULOADDRAW_B_FOREGROUND );		///red as Backgound
		}

		for( dwLoop = 1; dwLoop < 20; dwLoop++ )
		{
			if( 0 == dwLoop % 2 ) 
				SetRect( &rcDest, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale), pDrawArea->dwStartY, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale)+2, pDrawArea->dwStartY+(1*pDrawArea->dwYscale) );
			else
				SetRect( &rcDest, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale), pDrawArea->dwStartY, pDrawArea->dwStartX+(dwLoop*5*pDrawArea->dwXscale)+1, pDrawArea->dwStartY+(1*pDrawArea->dwYscale) );
	
			pmyDD->spLibBltDDraw( &rcDest, CPULOADDRAW_R_SCALEDOT, CPULOADDRAW_G_SCALEDOT, CPULOADDRAW_B_SCALEDOT );		///black as dot
		}
		
	#if 0	///two lines, put memory info in second line
		pmyDD->spLibTextDraw( pDrawArea->dwStartX, pDrawArea->dwEndY+pDrawArea->dwYbase, TEXT("CPU utilization => %4d%%, Avg=> %4d%%!!!"), dwValue, spLibSysInfoIdle_GetAvg() );
		if( (-1) == dwRet )
			pmyDD->spLibTextDraw( pDrawArea->dwStartX, pDrawArea->dwEndY+(pDrawArea->dwYbase*5), TEXT("Memory get fail !!!") );		
		else	
			pmyDD->spLibTextDraw( pDrawArea->dwStartX, pDrawArea->dwEndY+(pDrawArea->dwYbase*5), TEXT("Memory %d%%=> P[%d/%d]k, V[%d/%d]k, F[%d/%d]k"), mmi.dwMemoryLoad, mmi.dwAvailPhys/1024, mmi.dwTotalPhys/1024, mmi.dwAvailVirtual/1024, mmi.dwTotalVirtual/1024, mmi.dwAvailPageFile/1024, mmi.dwTotalPageFile/1024 );
	#else	///one line for both CPU & memory
		pmyDD->spLibTextDraw( pDrawArea->dwStartX, pDrawArea->dwEndY+pDrawArea->dwYbase, TEXT("CPU =>%4d%%, Avg=>%4d%%!! Phy Memory =>%4d%% [%d/%d/%d]k!!"), dwValue, spLibSysInfoIdle_GetAvg(), mmi.dwMemoryLoad, mmi.dwAvailPhys/1024, mmi.dwTotalPhys/1024, mmi.iDiffPhys/1024 );
	#endif
	}

	///spMessageBoxOut( dINIT, TEXT("DrawCPULoading---") );	
	SPDMSG( dINIT, (TEXT("DrawCPUMemLoading---")) );	
}

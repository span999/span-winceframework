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
#include "spLibDataXferEvent.h"



#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only
#define		SPPREFIX			TEXT("DTEExe:")



static DWORD MainRoutine( DWORD dwPararm );
static void spExeReleaseAll( void );



static spLibDDraw	*pmyDD = NULL;


/// *****************************************
/// routine
/// *****************************************

int WINAPI WinMain (
              HINSTANCE hInstance,    // Handle to the current instance
              HINSTANCE hPrevInstance,// Handle to the previous instance
              LPTSTR lpCmdLine,       // Pointer to the command line
              int nCmdShow)           // Show state of the window
{
	MainRoutine( 0 );
	return 0;
}


BOOL DataXferEventExeCallback( PVOID pData, DWORD dwByteSize )
{
	BOOL bRet = TRUE;
	DWORD dwLoop = 0;
	
	pmyDD->spLibTextDraw( 100, 200, TEXT("pData=>0x%08x, %4d!!"), pData, dwByteSize );
	
	for( dwLoop = 0; dwLoop < dwByteSize; dwLoop++ )
		pmyDD->spLibTextDraw( 100+(dwLoop*20), 220, TEXT("%02x"), *((BYTE *)pData+dwLoop) );
	
	
	return bRet;
}

static DWORD MainRoutine( DWORD dwPararm )
{
	DWORD dwRet = 0;

	DWORD dwCount = 0;	
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s go dataXferEvent.exe!!!"), SPPREFIX );
	
	///init ddraw
	pmyDD = new spLibDDraw();
	pmyDD->spLibInitDDraw();
	
	spLibDataXferEvent_Init( 0 );
	spLibDataXferEvent_SetReceiverCallback( DataXferEventExeCallback );
	
	if( spLibDataXferEvent_IsHost() )
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s I am Host!!!"), SPPREFIX );
		Sleep( 60000 );
	}	
	else
	{
		DWORD dwData = 0x12345678;
		BYTE bData16[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };
		BYTE bData13[] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31, 0x32 };
		
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s I am Client!!!"), SPPREFIX );
		spLibDataXferEvent_SendData( &dwData, sizeof(dwData) );
		spLibDataXferEvent_SendData( bData13, 13 );
		spLibDataXferEvent_SendData( bData16, 16 );
	}
	
	spLibDataXferEvent_Deinit( 0 );
	spExeReleaseAll();
	
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s exit dataXferEvent.exe!!!"), SPPREFIX );
	return dwRet;
}



static void spExeReleaseAll( void )
{
	if( NULL != pmyDD )
	{
		delete pmyDD;
		pmyDD = NULL;
	}
}
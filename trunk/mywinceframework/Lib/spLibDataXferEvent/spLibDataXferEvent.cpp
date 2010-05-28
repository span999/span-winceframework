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
#include "spLibDataXferEvent.h"
#include "spLibDataXferEventHeader.h"




#define		SPPREFIX			TEXT("DTELib:")
#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only



///phototype
static BOOL spLibInitContent( LibDataTransferEventContent* pThis );
static BOOL spLibDeInitContent( LibDataTransferEventContent* pThis );
static BOOL spLibInitEvent( LibDataTransferEventContent* pThis );
static BOOL spLibInitCreateThread( LibDataTransferEventContent* pThis );
static DWORD WINAPI DataTransferEventThread( LPVOID  pContext );
static void spGetEventDataIn( HANDLE hEvent, LibDataTransferEventContent* pThis );
static BOOL spLibValidChksum( LibDataTransferEventContent* pThis );
static DWORD spLibSendData( PVOID pData, DWORD dwByteSize, LibDataTransferEventContent* pThis );
static void spSendDataOut( HANDLE hEvent, DWORD dwData );


static LibDataTransferEventContent ThisContent;
static LibDataTransferEventContent *pThisContent = &ThisContent;

/*
BOOL WriteToDatabase (HANDLE hMutex)
{
  DWORD dwWaitResult;

  dwWaitResult = WaitForSingleObject (hMutex,   // Handle of mutex object
                                      5000L);   // Five-second time-out
  switch (dwWaitResult)
  {
    case WAIT_OBJECT_0:
      __try
      {
        // Your code to write to the database goes here.
      }
      __finally
      {
        // Your code to clean up the database operations goes here.

        if (! ReleaseMutex (hMutex))
        {
          // Your code to deal with the error goes here.
        }
      }
      break;

    // Cannot get mutex object ownership due to time-out
    case WAIT_TIMEOUT:
      return FALSE;

    // Got ownership of an abandoned mutex object
    case WAIT_ABANDONED:
      return FALSE;
  }

  return TRUE;
} // End of WriteToDatabase example code

*/


/// *****************************************
/// interface functions
/// *****************************************

DWORD spLibDataTransferEvent_Init( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibDataTransferEvent_Init: %d \r\n", 1) );
	
	if( (1 != pThisContent->dwInitStep) && (2 != pThisContent->dwInitStep) && (3 != pThisContent->dwInitStep) )
	{
		if( spLibInitContent( pThisContent ) && spLibInitEvent( pThisContent ) )
		{
			///create thread
			if( spLibInitCreateThread( pThisContent ) )
				dwRet = 0;
			else
				dwRet = (-1);

			dwRet = 0;
		}
		else
			dwRet = (-1);
	}

	if( 0 != dwRet )
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibDataTransferEvent_Init fail !!!"), SPPREFIX );
		
	SPDMSG( dINIT, (L"$$$spLibDataTransferEvent_Init: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibDataTransferEvent_Deinit( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibDataTransferEvent_Deinit: %d \r\n", 1) );
	
	spLibDeInitContent( pThisContent );
	
	SPDMSG( dINIT, (L"$$$spLibDataTransferEvent_Deinit: %d \r\n", 0) );
	return dwRet;
}

BOOL spLibDataTransferEvent_IsHost( void )
{
	BOOL bRet = FALSE;
	SPDMSG( dINFO, (L"$$$spLibDataTransferEvent_IsHost: %d \r\n", 1) );
	
	SPDMSG( dINFO, (L"$$$spLibDataTransferEvent_IsHost: %d \r\n", 0) );
	return bRet;
}


BOOL spLibDataTransferEvent_SetReceiverCallback( PFN_DATATRANSFEREVENT_RECEIVER_CALLBACK pfnDataTransferReceiverCAllback )
{
	BOOL bRet = FALSE;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibDataTransferEvent_SetReceiverCallback: %d \r\n", 1) );
	
	if( pfnDataTransferReceiverCAllback )
	{
		pThisContent->pfnDataTransferReceiverCallback = pfnDataTransferReceiverCAllback;
		bRet = TRUE;
	}
	
	SPDMSG( dINFO, (L"$$$spLibDataTransferEvent_SetReceiverCallback: %d \r\n", 0) );
	return bRet;
}


DWORD spLibDataTransferEvent_SendData( PVOID pData, DWORD dwByteSize )
{
	DWORD dwRet = 0;
	
	dwRet = spLibSendData( pData, dwByteSize, pThisContent );
	
	return dwRet;
}


/// *****************************************
/// internal functions
/// *****************************************


static BOOL spLibInitContent( LibDataTransferEventContent* pThis )
{
	BOOL bRet = TRUE;

	pThis->hDataTransEvent1 = NULL;
	pThis->hDataTransEvent2 = NULL;
///	pThis->hTouchEventUp = NULL;
	pThis->hDataTransEventControl = NULL;
	pThis->hDataTransMutex = NULL;
	pThis->pfnDataTransferReceiverCallback = NULL;
	pThis->hMonitorThread = NULL;
	pThis->eState = INIT_STATE;
	pThis->pState = NONE_DATA_STATE;
	pThis->dwPacketSize = 0;
	pThis->dwPacketChksum = 0;
	pThis->dwPacketChksumCount = 0;
	pThis->pRevData = NULL;
	pThis->pRevDataCurr = NULL;
	pThis->bIsHost = FALSE;
	
///	pteData->teType = TOUCH_NONE_EVENT;
///	pteData->tiX = 0;
///	pteData->tiY = 0;

	pThis->dwInitStep = 1;
	
	return bRet;
}



static BOOL spLibDeInitContent( LibDataTransferEventContent* pThis )
{
	BOOL bRet = TRUE;

	///SetEvent( pThis->hTouchEventControl );
	///Sleep( 500 );
	
	CloseHandle( pThis->hDataTransEvent1 );
	CloseHandle( pThis->hDataTransEvent2 );
///	CloseHandle( pThis->hTouchEventUp );
	CloseHandle( pThis->hDataTransEventControl );	
	CloseHandle( pThis->hDataTransMutex );
	
	
	pThis->hDataTransEvent1 = NULL;
	pThis->hDataTransEvent2 = NULL;
///	pThis->hTouchEventUp = NULL;
	pThis->hDataTransEventControl = NULL;
	pThis->hDataTransMutex = NULL;
	pThis->pfnDataTransferReceiverCallback = NULL;
	pThis->hMonitorThread = NULL;
	pThis->eState = INIT_STATE;
	pThis->pState = NONE_DATA_STATE;
	pThis->dwPacketSize = 0;
	pThis->dwPacketChksum = 0;
	pThis->dwPacketChksumCount = 0;
	pThis->pRevData = NULL;
	free( pThis->pRevDataCurr );
	pThis->pRevDataCurr = NULL;

	
	pThis->dwInitStep = 0;
	
	return bRet;
}


static BOOL spLibInitEvent( LibDataTransferEventContent* pThis )
{
	BOOL bRet = TRUE;

	if( NULL != pThis )
	{	
		pThis->hDataTransEvent1 = CreateEvent( NULL, FALSE, FALSE, SPLIB_DATATRANSFER_EVENT_1_NAME );
		
		if( NULL != pThis->hDataTransEvent1 )
		{
			if( ERROR_ALREADY_EXISTS == GetLastError() )
			{	/// Host already there, we are client
				pThis->bIsHost = FALSE;
			}
			else
			{	/// we are Host~
				pThis->bIsHost = TRUE;
			}
			pThis->hDataTransEvent2 = CreateEvent( NULL, FALSE, FALSE, SPLIB_DATATRANSFER_EVENT_2_NAME );
///			pThis->hTouchEventUp = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_UP_EVENT_NAME );
			pThis->hDataTransEventControl = CreateEvent( NULL, FALSE, FALSE, NULL );


			pThis->hDataTransMutex = CreateMutex( NULL, FALSE, SPLIB_DATATRANSFER_MUTEX_NAME );

			pThis->dwInitStep = 2;
		}
		else
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitEvent fail1 !!!"), SPPREFIX );
			bRet = FALSE;
		}
		
	}
	else
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitEvent fail !!!"), SPPREFIX );
		bRet = FALSE;
	}
	
	return bRet;
}


static BOOL spLibInitCreateThread( LibDataTransferEventContent* pThis )
{
	BOOL bRet = TRUE;
	DWORD dwThreadId = 0;
	DWORD dwThreadPararm = 0;
	
	pThis->hMonitorThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)DataTransferEventThread, (LPVOID)&dwThreadPararm, 0, (LPDWORD)&dwThreadId );
	
	if( NULL == pThis->hMonitorThread )
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitCreateThread fail !!!"), SPPREFIX );
		pThis->dwInitStep = 0;
	}
	else	
		pThis->dwInitStep = 3;
	
	return bRet;
}


static DWORD WINAPI DataTransferEventThread( LPVOID pContext )
{
#define		WAITEVENT_NUM		2

	DWORD dwRet = 0;
	DWORD dwReturns = 0;
	BOOL bExitMonitor = FALSE;
	HANDLE hWaitEvents[WAITEVENT_NUM];
	DWORD dwWaitMS = INFINITE;
	
	if( pThisContent->bIsHost )
		hWaitEvents[0] = pThisContent->hDataTransEvent2;
	else
		hWaitEvents[0] = pThisContent->hDataTransEvent1;

	hWaitEvents[1] = pThisContent->hDataTransEventControl;
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s DataTransferEventThread start !!!"), SPPREFIX );
	
	while( !bExitMonitor )
	{
		dwReturns = WaitForMultipleObjects( WAITEVENT_NUM, hWaitEvents, FALSE, dwWaitMS );
		
		switch( dwReturns )
		{
			case WAIT_OBJECT_0 + 0:
				/// got data in event
				spGetEventDataIn( hWaitEvents[0], pThisContent );
				break;
			case WAIT_OBJECT_0 + 1:
				/// got control event
				bExitMonitor = TRUE;
				break;
			case WAIT_TIMEOUT:
				break;
			default:
				break;
				
		}	///switch
		
	}	///while
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s DataTransferEventThread exit !!!"), SPPREFIX );
	
	return dwRet;
}


static void spGetEventDataIn( HANDLE hEvent, LibDataTransferEventContent* pThis )
{
	DWORD dwData = 0;
	
	dwData = GetEventData( hEvent );
	
	///default init with NONE_DATA_STATE, wait for coming packet head sign...
	if( NONE_DATA_STATE == pThis->pState )
	{
		if( dwData == SPLIB_DATATRANSFER_PACK_SIGN )	///got packet head sign
			pThis->pState = PACK_SIGN_STATE;
		else
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s spGetEventDataIn error pack sign !!!"), SPPREFIX );
	}
	else
	if( PACK_SIGN_STATE == pThis->pState )	/// wait for size data coming
	{
		pThis->pState = PACK_SIZE_STATE;
		pThis->dwPacketSize = dwData;
	}
	else
	if( PACK_SIZE_STATE == pThis->pState )	///wait for packet start sign
	{
		if( dwData == SPLIB_DATATRANSFER_PACKSTART_SIGN )	///got packet start sign
			pThis->pState = PACK_START_STATE;
		else
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s spGetEventDataIn error pack start !!!"), SPPREFIX );
	}
	else
	if( PACK_START_STATE == pThis->pState )	/// wait for data coming
	{
		if( dwData != SPLIB_DATATRANSFER_PACKEND_SIGN )	///first data in
		{
			if( NULL != pThis->pRevData )
				free( pThis->pRevData );
			
			pThis->pRevData = NULL;
			pThis->pDataCurr = NULL;
			pThis->dwPacketChksumCount = 0;
			
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s spGetEventDataIn -data-0x%08x !!!"), SPPREFIX, dwData );
			
			if( NULL == pThis->pRevData )
			{	///create containter
				///pThis->pRevData = malloc( pThis->dwPacketSize );
				pThis->pRevData = malloc( pThis->dwPacketSize + ( sizeof(DWORD) - (pThis->dwPacketSize % sizeof(DWORD))) );
				pThis->pDataCurr = (DWORD *)pThis->pRevData;
			}
			
			if( NULL != pThis->pDataCurr )
			{
				*(pThis->pDataCurr) = dwData;
				(pThis->pDataCurr)++;
				dwPacketChksumCount = dwPacketChksumCount + *(pThis->pDataCurr);
			}
			else
				spLibDbgMsg( LIBMSGFLAG, TEXT("%s spGetEventDataIn allocate memory fail !!"), SPPREFIX );

			pThis->pState = PACK_DATA_STATE;
		}
		else
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s spGetEventDataIn -no data- !!!"), SPPREFIX );
		}
	}
	else
	if( PACK_DATA_STATE == pThis->pState ) ///wait for cont. data in
	{
		if( dwData == SPLIB_DATATRANSFER_PACKEND_SIGN )
			pThis->pState = PACK_END_STATE;
		else
		{
			if( NULL != pThis->pDataCurr )
			{
				*(pThis->pDataCurr) = dwData;
				(pThis->pDataCurr)++;
				dwPacketChksumCount = dwPacketChksumCount + *(pThis->pDataCurr);
			}
		}
	}
	else
	if( PACK_END_STATE == pThis->pState )
	{
		pThis->pState = NONE_DATA_STATE;
		pThis->dwPacketChksum = dwData;
		
		if( spLibValidChksum( pThis ) )
			pThis->pfnDataTransferReceiverCallback( pThis->pRevData, pThis->dwPacketSize );
	}


	
}


static BOOL spLibValidChksum( LibDataTransferEventContent* pThis )
{
	BOOL bRet = FALSE;
	
	if( pThis )
	{
		if( pThis->dwPacketChksum == pThis->dwPacketChksumCount )
			bRet = TRUE;
		else
			bRet = FALSE;
	}
	
	return bRet;
}

static DWORD spLibSendData( PVOID pData, DWORD dwByteSize, LibDataTransferEventContent* pThis )
{
	DWORD dwRet = 0;
	DWORD dwWaitResult = 0;

	spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibSendData 0x%08x %d"), SPPREFIX, pData, dwByteSize);
	
	if( pThis && pData && 0 != dwByteSize )
	{
		PDWORD pdwData = NULL;
		DWORD dwDWSize = dwByteSize;
		
		///try to get ownership of mutex
        dwWaitResult = WaitForSingleObject( 
            pThis->hDataTransMutex,    // handle to mutex
            INFINITE);  // no time-out interval
 
        switch (dwWaitResult) 
        {
            // The thread got ownership of the mutex
            case WAIT_OBJECT_0: 
                __try { 
                    // TODO: Write to the database
					
					spSendDataOut( NULL, SPLIB_DATATRANSFER_PACK_SIGN );
					spSendDataOut( NULL, dwByteSize );
					spSendDataOut( NULL, SPLIB_DATATRANSFER_PACKSTART_SIGN );
					
					pdwData = (DWORD *)pData;
					do
					{
						spSendDataOut( NULL, *pdwData );
						dwDWSize = dwDWSize - 4;
						pdwData++;
					}while( dwDWSize > 0 );
					
					spSendDataOut( NULL, dwByteSize );
					spSendDataOut( NULL, SPLIB_DATATRANSFER_PACKEND_SIGN );
                } 

                __finally { 
                    // Release ownership of the mutex object
                    if (! ReleaseMutex(pThis->hDataTransMutex)) 
                    { 
                        // Handle error.
						spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibSendData ReleaseMutex fail !!!"), SPPREFIX );
                    } 
                } 
                break; 

            // The thread got ownership of an abandoned mutex
            // The database is in an indeterminate state
            case WAIT_ABANDONED:
				spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibSendData WAIT_ABANDONED !!!"), SPPREFIX );
                dwRet = 0;
        }
	}
	else
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibSendData invalid paramter !!!"), SPPREFIX );
	
	return dwRet;
}


static void spSendDataOut( HANDLE hEvent, DWORD dwData )
{
	HANDLE hSend = NULL;

	if( NULL == hEvent )
	{
		if( pThisContent->bIsHost )
			hSend = pThisContent->hDataTransEvent1;
		else
			hSend = pThisContent->hDataTransEvent2;
	}
	else
		hSend = hEvent;
		
	SetEventData( hSend, dwData );
	SetEvent( hSend );
}
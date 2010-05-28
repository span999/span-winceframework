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


///
typedef struct _LibTouchHookContent
{
	HANDLE 							hTouchEventDown;
	HANDLE 							hTouchEventMove;
	HANDLE 							hTouchEventUp;
	HANDLE 							hTouchEventControl;
	PFN_TOUCH_HOOK_LIB_CALLBACK		pfnTouchHookLibCallback;
	HANDLE							hMonitorThread;
	DWORD							dwInitStep;
} LibTouchHookContent;


#define		SPPREFIX			TEXT("THLib:")
#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only



///phototype
static BOOL spLibInitContent( LibTouchHookContent* pThis );
///static BOOL spLibLoadMyModule( void );
///static BOOL spLibFirstLoad( void );
static BOOL spLibDeInitContent( LibTouchHookContent* pThis );
static BOOL spLibInitEvent( LibTouchHookContent* pThis );
static BOOL spLibInitCreateThread( LibTouchHookContent* pThis );
static DWORD WINAPI TouchEventThread( LPVOID  pContext );
static void spGetEventIn( HANDLE hEvent, TOUCH_EVENT_DATA *pteD );


static LibTouchHookContent ThisContent;
static LibTouchHookContent *pThisContent = &ThisContent;
static TOUCH_EVENT_DATA teData;
static TOUCH_EVENT_DATA *pteData = &teData;



/// *****************************************
/// interface functions
/// *****************************************

DWORD spLibTouchHook_Init( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibTouchHook_Init: %d \r\n", 1) );
	
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
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibTouchHook_Init fail !!!"), SPPREFIX );
		
	SPDMSG( dINIT, (L"$$$spLibTouchHook_Init: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibTouchHook_Deinit( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibTouchHook_Deinit: %d \r\n", 1) );
	
	spLibDeInitContent( pThisContent );
	
	SPDMSG( dINIT, (L"$$$spLibTouchHook_Deinit: %d \r\n", 0) );
	return dwRet;
}


BOOL spLibTouchHook_SetCallback( PFN_TOUCH_HOOK_LIB_CALLBACK pfnTouchHookLibCB )
{
	BOOL bRet = FALSE;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibTouchHook_SetCallback: %d \r\n", 1) );
	
	if( pfnTouchHookLibCB )
	{
		pThisContent->pfnTouchHookLibCallback = pfnTouchHookLibCB;
		bRet = TRUE;
	}
	
	SPDMSG( dINFO, (L"$$$spLibTouchHook_SetCallback: %d \r\n", 0) );
	return bRet;
}





/// *****************************************
/// internal functions
/// *****************************************
/*
static BOOL spLibFirstLoad( void )
{
	BOOL bRet = TRUE;
	
	if( (NULL == pThisContent->hCheckEvent) || (NULL == pThisContent->hResponseEvent) )
		bRet = TRUE;
	else
		bRet = FALSE;
		
	return bRet;
}
*/


static BOOL spLibInitContent( LibTouchHookContent* pThis )
{
	BOOL bRet = TRUE;

	pThis->hTouchEventDown = NULL;
	pThis->hTouchEventMove = NULL;
	pThis->hTouchEventUp = NULL;
	pThis->hTouchEventControl = NULL;
	pThis->pfnTouchHookLibCallback = NULL;
	pThis->hMonitorThread = NULL;
	
	pteData->teType = TOUCH_NONE_EVENT;
	pteData->tiX = 0;
	pteData->tiY = 0;

	pThis->dwInitStep = 1;
	
	return bRet;
}



static BOOL spLibDeInitContent( LibTouchHookContent* pThis )
{
	BOOL bRet = TRUE;

	SetEvent( pThis->hTouchEventControl );
	Sleep( 500 );
	
	CloseHandle( pThis->hTouchEventDown );
	CloseHandle( pThis->hTouchEventMove );
	CloseHandle( pThis->hTouchEventUp );
	CloseHandle( pThis->hTouchEventControl );	
	
	pThis->hTouchEventDown = NULL;
	pThis->hTouchEventMove = NULL;
	pThis->hTouchEventUp = NULL;
	pThis->hTouchEventControl = NULL;
	pThis->pfnTouchHookLibCallback = NULL;
	
	pThis->dwInitStep = 0;
	
	return bRet;
}


static BOOL spLibInitEvent( LibTouchHookContent* pThis )
{
	BOOL bRet = TRUE;

	if( NULL != pThis )
	{	
		pThis->hTouchEventDown = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_DOWN_EVENT_NAME );
		pThis->hTouchEventMove = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_MOVE_EVENT_NAME );
		pThis->hTouchEventUp = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_UP_EVENT_NAME );
		pThis->hTouchEventControl = CreateEvent( NULL, FALSE, FALSE, SPLIB_TOUCHHOOK_CONTROL_EVENT_NAME );
		pThis->dwInitStep = 2;
	}
	else
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitEvent fail !!!"), SPPREFIX );
		bRet = FALSE;
	}
	
	return bRet;
}


static BOOL spLibInitCreateThread( LibTouchHookContent* pThis )
{
	BOOL bRet = TRUE;
	DWORD dwThreadId = 0;
	DWORD dwThreadPararm = 0;
	
	pThis->hMonitorThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)TouchEventThread, (LPVOID)&dwThreadPararm, 0, (LPDWORD)&dwThreadId );
	
	if( NULL == pThis->hMonitorThread )
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitCreateThread fail !!!"), SPPREFIX );
		pThis->dwInitStep = 0;
	}
	else	
		pThis->dwInitStep = 3;
	
	return bRet;
}


static DWORD WINAPI TouchEventThread( LPVOID pContext )
{
#define		WAITEVENT_NUM		4

	DWORD dwRet = 0;
	DWORD dwReturns = 0;
	BOOL bExitMonitor = FALSE;
	HANDLE hWaitEvents[WAITEVENT_NUM];
	DWORD dwWaitMS = INFINITE;
	
	hWaitEvents[0] = pThisContent->hTouchEventDown;
	hWaitEvents[1] = pThisContent->hTouchEventMove;
	hWaitEvents[2] = pThisContent->hTouchEventUp;
	hWaitEvents[3] = pThisContent->hTouchEventControl;
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchEventThread start !!!"), SPPREFIX );
	
	while( !bExitMonitor )
	{
		dwReturns = WaitForMultipleObjects( WAITEVENT_NUM, hWaitEvents, FALSE, dwWaitMS );
		
		switch( dwReturns )
		{
			case WAIT_OBJECT_0 + 0:
				/// got touch DOWN event
				pteData->teType = TOUCH_DOWN_EVENT;
				spGetEventIn( hWaitEvents[0], pteData );
				break;
			case WAIT_OBJECT_0 + 1:
				/// got touch MOVE event
				pteData->teType = TOUCH_MOVE_EVENT;
				spGetEventIn( hWaitEvents[1], pteData );
				break;
			case WAIT_OBJECT_0 + 2:
				/// got touch UP event
				pteData->teType = TOUCH_UP_EVENT;
				spGetEventIn( hWaitEvents[2], pteData );
				break;
			case WAIT_OBJECT_0 + 3:
				/// got control event
				pteData->teType = TOUCH_NONE_EVENT;
				spGetEventIn( hWaitEvents[3], pteData );
				bExitMonitor = TRUE;
				break;
			case WAIT_TIMEOUT:
				break;
			default:
				break;
				
		}	///switch
		
	}	///while
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s TouchEventThread exit !!!"), SPPREFIX );
	
	return dwRet;
}


static void spGetEventIn( HANDLE hEvent, TOUCH_EVENT_DATA *pteD )
{
	DWORD dwData = 0;
	TOUCH_EVENT_DATA Data;
	
	dwData = GetEventData( hEvent );
	
	Data.teType = pteD->teType;
	Data.tiX = pteD->tiX = (INT)(dwData & 0x0000FFFF);
	Data.tiY = pteD->tiY = (INT)((dwData>>16) & 0x0000FFFF);
	
	if( pThisContent->pfnTouchHookLibCallback )
		pThisContent->pfnTouchHookLibCallback( Data );
		
}


/*
static BOOL spLibLoadMyModule(void)
{
    HMODULE hCore;	
    BOOL bRet = FALSE;
	
    /// init system function call
    hCore = (HMODULE)LoadLibrary( L"coredll.dll" );
    if ( hCore ) {
        gpfGetIdleTime = (PFN_GetIdleTime)GetProcAddress( hCore, L"GetIdleTime" );
        if ( !gpfGetIdleTime )
        {
            FreeLibrary(hCore);
            bRet = FALSE;
		}
        else
            bRet = TRUE;
    }
    
    return bRet;	
}
*/

void Encrypt( void )
{
#include <windows.h>
#include <stdio.h>
#include <wincrypt.h>

#define BLOCK_SIZE            1000
#define BUFFER_SIZE           1008

BOOL EncryptFile (LPTSTR, LPTSTR, LPTSTR);

/***********************************************************************

  WinMain

***********************************************************************/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    LPTSTR lpCmdLine, int nCmdShow)            
{
  LPTSTR lpszSource  = TEXT("test2.txt");
  LPTSTR lpszDestination = TEXT("test.xxx");
  LPTSTR lpszPassword  = TEXT("password");
  
  if (!EncryptFile (lpszSource, lpszDestination, lpszPassword)) 
  {
    wprintf (TEXT("Error encrypting file!\n"));
    return 1;
  }

  return 0;
}

/***********************************************************************

  EncryptFile

***********************************************************************/
BOOL EncryptFile (LPTSTR lpszSource, LPTSTR lpszDestination, 
                  LPTSTR lpszPassword)
{
  FILE *hSrcFile = NULL, 
       *hDestFile = NULL;

  HCRYPTPROV hProv = 0;
  HCRYPTHASH hHash = 0;
  HCRYPTKEY hKey = 0, 
            hXchgKey = 0;

  PBYTE pbBuffer = NULL, 
        pbKeyBlob = NULL;

  BOOL bEOF = 0, 
       bReturn = FALSE;

  DWORD dwCount, 
        dwKeyBlobLen;

  // Open the source file.
  if ((hSrcFile = _wfopen (lpszSource, TEXT("rb"))) == NULL) 
  {
    wprintf (TEXT("Error opening Plaintext file!\n"));
    goto exit;
  }

  // Open the destination file.

  if ((hDestFile = _wfopen (lpszDestination, TEXT("wb"))) == NULL) 
  {
    wprintf (TEXT("Error opening Ciphertext file!\n"));
    goto exit;
  }
 
  // Get the handle to the default provider.
  if (!CryptAcquireContext (&hProv, NULL, NULL, PROV_RSA_FULL, 0)) 
  {
      DWORD ret = GetLastError();
      if (ret !=  NTE_BAD_KEYSET)
      {
           wprintf (TEXT("Error 0x%x during CryptAcquireContext!\n"),ret);
           goto exit;
      }
      else
      {
          if (!CryptAcquireContext (&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) 
          {
              wprintf (TEXT("Error 0x%x during CryptAcquireContext!\n"), 
                  GetLastError());
              goto exit;
          }
      }
  }
 
  if (lpszPassword == NULL) 
  {
    // Encrypt the file with a random session key.
    // Create a random session key.
    if (!CryptGenKey (hProv, CALG_RC2, CRYPT_EXPORTABLE, &hKey)) 
    {
      wprintf (TEXT("Error %x during CryptGenKey!\n"), 
               GetLastError ());
      goto exit;
    }
 
    // Get the handle to the key exchange public key.
    if (!CryptGetUserKey (hProv, AT_KEYEXCHANGE, &hXchgKey)) 
    {
        DWORD ret = GetLastError();
        if (ret != NTE_NO_KEY)
        {
            wprintf (TEXT("Error 0x%x during CryptGetUserKey!\n"), ret);
            goto exit;
        }
        // Create the key.
        if (!CryptGenKey(hProv,AT_KEYEXCHANGE,NULL,&hXchgKey))
        {
            ret = GetLastError();
            wprintf (TEXT("Error 0x%x during CryptGenKey For KeyXchange!\n"), ret);
            goto exit;
        }
    }
 
    // Determine the size of the key BLOB and allocate memory.
    if (!CryptExportKey (hKey, hXchgKey, SIMPLEBLOB, 0, NULL, 
                         &dwKeyBlobLen)) 
    {
      wprintf (TEXT("Error %x computing blob length!\n"), 
               GetLastError ());
      goto exit;
    }
 
    if ((pbKeyBlob = (PBYTE)malloc(dwKeyBlobLen))==NULL) 
    {
      wprintf (TEXT("Out of memory!\n"));
      goto exit;
    }

    // Export the session key into a simple key BLOB.
    if (!CryptExportKey (hKey, hXchgKey, SIMPLEBLOB, 0, pbKeyBlob, 
                         &dwKeyBlobLen)) 
    {
      wprintf (TEXT("Error %x during CryptExportKey!\n"), 
               GetLastError ());
      goto exit;
    }
 
    // Write the size of key BLOB to the destination file.
    fwrite (&dwKeyBlobLen, sizeof (DWORD), 1, hDestFile);
 
    if (ferror (hDestFile)) 
    {
      wprintf (TEXT("Error writing header!\n"));
      goto exit;
    }

    // Write the key BLOB to the destination file.
    fwrite (pbKeyBlob, 1, dwKeyBlobLen, hDestFile);
 
    if (ferror (hDestFile)) 
    {
      wprintf (TEXT("Error writing header!\n"));
      goto exit;
    }
  } 
  else 
  {
    // Encrypt the file with a session key derived from a password.
    // Create a hash object.
    if (!CryptCreateHash (hProv, CALG_MD5, 0, 0, &hHash)) 
    {
      wprintf (TEXT("Error %x during CryptCreateHash!\n"), 
               GetLastError ());
      goto exit;
    }

    // Hash in the password data.
    if (!CryptHashData (hHash, (PBYTE)lpszPassword, 
                        wcslen (lpszPassword), 0)) 
    {
      wprintf (TEXT("Error %x during CryptHashData!\n"), 
               GetLastError ());
      goto exit;
    }

    // Derive a session key from the hash object.
    if (!CryptDeriveKey (hProv, CALG_RC2, hHash, 0, &hKey)) 
    {
      wprintf (TEXT("Error %x during CryptDeriveKey!\n"), 
               GetLastError ());
      goto exit;
    }
  }

  // Allocate memory.
  // XXX: Changed: if ((pbBuffer=(PBYTE)malloc(dwKeyBlobLen))==NULL) 
  // C_ASSERT(BUFFER_SIZE > BLOCK_SIZE):
  if ((pbBuffer=(PBYTE)malloc(BUFFER_SIZE))==NULL) 
  {
    wprintf (TEXT("Out of memory!\n"));
    goto exit;
  }
 
  // Encrypt the source file and write to the destination file.
  do 
  {
    // Read up to BLOCK_SIZE bytes from the source file.
    dwCount = fread (pbBuffer, 1, BLOCK_SIZE, hSrcFile);

    if (ferror (hSrcFile)) 
    {
      wprintf (TEXT("Error reading Plaintext!\n"));
      goto exit;
    }
    bEOF = feof (hSrcFile);

    // Encrypt the data.
    if (!CryptEncrypt (hKey, 0, bEOF, 0, pbBuffer, &dwCount, 
                       BUFFER_SIZE)) 
    {
      wprintf (TEXT("bytes required:%d\n"), dwCount);
      wprintf (TEXT("Error %x during CryptEncrypt!\n"), 
               GetLastError ());
      goto exit;
    }

    // Write the data to the destination file.
    fwrite (pbBuffer, 1, dwCount, hDestFile);
    if (ferror (hDestFile)) 
    {
      wprintf (TEXT("Error writing Ciphertext!\n"));
      goto exit;
    }

  } while (!bEOF);
 
  bReturn = TRUE;
  wprintf (TEXT("OK\n"));

exit:
   // Close the files.
  if (hSrcFile) fclose (hSrcFile);
  if (hDestFile) fclose (hDestFile);

  // Free memory.
  if (pbKeyBlob) free (pbKeyBlob);
  if (pbBuffer) free (pbBuffer);

  // Destroy the session key.
  if (hKey) CryptDestroyKey (hKey);

  // Release the key exchange key handle.
  if (hXchgKey) CryptDestroyKey (hXchgKey);

  // Destroy the hash object.
  if (hHash) CryptDestroyHash (hHash);

  // Release the provider handle.
  if (hProv) CryptReleaseContext (hProv, 0);
  return bReturn;
}
}

void Decrypt( void )
{
#include <windows.h>
#include <stdio.h>
#include <wincrypt.h>
   
#define BLOCK_SIZE            1000
#define BUFFER_SIZE           1008

BOOL DecryptFile (LPTSTR, LPTSTR, LPTSTR);

/***********************************************************************

  WinMain

***********************************************************************/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    LPTSTR lpCmdLine, int nCmdShow)                   
{
  LPTSTR lpszSource  = TEXT("test.xxx");
  LPTSTR lpszDestination = TEXT("test2.txt");
  LPTSTR lpszPassword  = TEXT("password");
 
  if (!DecryptFile (lpszSource, lpszDestination, lpszPassword)) 
  {
    wprintf (TEXT("Error encrypting file!\n"));
    return 1;
  }

  return 0;
}

/***********************************************************************

  DecryptFile

***********************************************************************/
BOOL DecryptFile (LPTSTR lpszSource, LPTSTR lpszDestination, 
                  LPTSTR lpszPassword)
{
  FILE *hSrcFile = NULL, 
       *hDestFile = NULL;

  HCRYPTPROV hProv = 0;
  HCRYPTHASH hHash = 0;
  HCRYPTKEY hKey = 0;

  PBYTE pbBuffer = NULL, 
        pbKeyBlob = NULL;

  BOOL bEOF = 0, 
       bReturn = FALSE;

  DWORD dwCount, 
        dwKeyBlobLen;

  // Open the source file.
  if ((hSrcFile = _wfopen (lpszSource, TEXT("rb"))) == NULL) 
  {
    wprintf (TEXT("Error opening Ciphertext file!\n"));
    goto exit;
  }

  // Open the destination file.
  if ((hDestFile = _wfopen (lpszDestination, TEXT("wb"))) == NULL) 
  {
    wprintf (TEXT("Error opening Plaintext file!\n"));
    goto exit;
  }

  // Get the handle to the default provider.
  if (!CryptAcquireContext (&hProv, NULL, NULL, PROV_RSA_FULL, 0)) 
  {
      DWORD ret = GetLastError();
      if (ret !=  NTE_BAD_KEYSET)
      {
              wprintf (TEXT("Error 0x%x during CryptAcquireContext!\n"), 
                  ret);
              goto exit;
      }
      else
      {
          if (!CryptAcquireContext (&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) 
          {
              wprintf (TEXT("Error 0x%x during CryptAcquireContext!\n"), 
                  GetLastError());
              goto exit;
          }
      }
  }
 

  if (lpszPassword == NULL) 
  {
    // Decrypt the file with the saved session key.
    // Read key BLOB length from the source file and allocate memory.
    fread (&dwKeyBlobLen, sizeof (DWORD), 1, hSrcFile);
    if (ferror (hSrcFile) || feof (hSrcFile)) 
    {
      wprintf (TEXT("Error reading file header!\n"));
      goto exit;
    }

    if ((pbKeyBlob = (PBYTE)malloc (dwKeyBlobLen)) == NULL) 
    {
      wprintf (TEXT("Out of memory or improperly formatted source ")
               TEXT("file!\n"));
      goto exit;
    }

    // Read the key BLOB from source file.
    fread (pbKeyBlob, 1, dwKeyBlobLen, hSrcFile);

    if (ferror (hSrcFile) || feof (hSrcFile)) 
    {
      wprintf (TEXT("Error reading file header!\n"));
      goto exit;
    }

    // Import the key BLOB into the CSP.
    if (!CryptImportKey (hProv, pbKeyBlob, dwKeyBlobLen, 0, 0, &hKey)) 
    {
      wprintf (TEXT("Error %x during CryptImportKey!\n"), 
               GetLastError ());
      goto exit;
    }
  } 
  else
  {
    // Decrypt the file with a session key derived from a password.
    // Create a hash object.
    if (!CryptCreateHash (hProv, CALG_MD5, 0, 0, &hHash)) 
    {
      wprintf (TEXT("Error %x during CryptCreateHash!\n"), 
               GetLastError ());
      goto exit;
    }
 
    // Hash in the password data.
    if (!CryptHashData (hHash, (PBYTE)lpszPassword, 
                        wcslen (lpszPassword), 0)) 
    {
      wprintf (TEXT("Error %x during CryptHashData!\n"), 
               GetLastError ());
      goto exit;
    }

    // Derive a session key from the hash object.
    if (!CryptDeriveKey (hProv, CALG_RC2, hHash, 0, &hKey)) 
    {
      wprintf (TEXT("Error %x during CryptDeriveKey!\n"), 
               GetLastError ());
      goto exit;
    }
  }
 
  // Allocate memory.
  if ((pbBuffer = (PBYTE)malloc (BUFFER_SIZE)) == NULL) 
  {
    wprintf (TEXT("Out of memory!\n"));
    goto exit;
  }

  // Decrypt the source file and write to the destination file.
  do 
  {
    // Read up to BLOCK_SIZE bytes from the source file.
    dwCount = fread (pbBuffer, 1, BLOCK_SIZE, hSrcFile);

    if (ferror (hSrcFile)) 
    {
      wprintf (TEXT("Error reading Ciphertext!\n"));
      goto exit;
    }

    bEOF = feof (hSrcFile);

    // Decrypt the data.
    if (!CryptDecrypt (hKey, 0, bEOF, 0, pbBuffer, &dwCount)) 
    {
      wprintf (TEXT("Error %x during CryptDecrypt!\n"), 
               GetLastError ());
      goto exit;
    }

    // Write the data to the destination file.
    fwrite (pbBuffer, 1, dwCount, hDestFile);
    if (ferror (hDestFile)) 
    {
      wprintf (TEXT("Error writing Plaintext!\n"));
      goto exit;
    }
  } while (!bEOF);

  bReturn = TRUE;
  wprintf (TEXT("OK\n"));

exit:
  // Close the source files.
  if (hSrcFile) fclose (hSrcFile);
  if (hDestFile) fclose (hDestFile);

  // Free memory.
  if (pbKeyBlob) free (pbKeyBlob);
  if (pbBuffer) free (pbBuffer);

  // Destroy the session key.
  if (hKey) CryptDestroyKey (hKey);

  // Destroy the hash object.
  if (hHash) CryptDestroyHash (hHash);

  // Release the provider handle.
  if (hProv) CryptReleaseContext (hProv, 0);

  return bReturn;
}
}
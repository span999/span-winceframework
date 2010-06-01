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
#include "spLibRegNotify.h"


#define MIC_OALDBGMSG_REG_KEY					(TEXT("\\Software\\Mitac\\micOALdebugMsg"))


///
typedef struct _LibRegNotifyContent
{
	HKEY							hKey;
	HANDLE 							hNotifyHandle;
	HANDLE 							hNotifyControl;
	HANDLE							hMonitorThread;
	PFN_REG_NOTIFY_LIB_CALLBACK		pfnRegNotifyLibCB;
	char 							KeyPathName[256];
} LibRegNotifyContent;


#define		SPPREFIX			TEXT("RNLib:")
#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
#define		LIBMSGFLAG_NK			(dNOUT|0x0FFFFFFF)		///nk msg only
#define		LIBMSGFLAG_NULL			(0x0|0x0)			///null msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only



///phototype
static BOOL spLibInitContent( LibRegNotifyContent* pThis, LPCTSTR KeyStr );
static BOOL spLibSetupContent( LibRegNotifyContent* pThis );
///static BOOL spLibLoadMyModule( void );
///static BOOL spLibFirstLoad( void );
static BOOL spLibDeInitContent( LibRegNotifyContent* pThis );
static BOOL spLibInitEvent( LibRegNotifyContent* pThis );
static BOOL spLibInitCreateThread( LibRegNotifyContent* pThis );
static DWORD WINAPI MonitorEventThread( LPVOID  pContext );
static DWORD spLibGetRegDWORD( LibRegNotifyContent* pThis );


static LibRegNotifyContent ThisContent;
static LibRegNotifyContent *pThisContent = &ThisContent;



/// *****************************************
/// interface functions
/// *****************************************

DWORD spLibRegNotify_Init( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibRegNotify_Init: %d \r\n", 1) );
	
	spLibInitContent( pThisContent, (LPCTSTR)dwParam );	///clear it
	
	///spLibSetupContent( pThisContent, (LPCTSTR)dwParam );	///setup
	
	
	if( 0 != dwRet )
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibRegNotify_Init fail !!!"), SPPREFIX );
		
	SPDMSG( dINIT, (L"$$$spLibRegNotify_Init: %d \r\n", 0) );
	return dwRet;
}


DWORD spLibRegNotify_Deinit( DWORD dwParam )
{
	DWORD dwRet = 0;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINIT, (L"$$$spLibRegNotify_Deinit: %d \r\n", 1) );
	
	spLibDeInitContent( pThisContent );
	
	SPDMSG( dINIT, (L"$$$spLibRegNotify_Deinit: %d \r\n", 0) );
	return dwRet;
}


HANDLE spLibRegNotify_GetNotifyHandle( void )
{
	HANDLE hHandle = NULL;
	///DWSPERRCODE	dwRet = 0;
	
	SPDMSG( dINFO, (L"$$$spLibRegNotify_GetNotifyHandle: %d \r\n", 1) );
	
	if( !pThisContent->hNotifyHandle )
		spLibSetupContent( pThisContent );
	
	
	hHandle = pThisContent->hNotifyHandle;
	
	SPDMSG( dINFO, (L"$$$spLibRegNotify_GetNotifyHandle: %d \r\n", 0) );
	return hHandle;
}


BOOL spLibRegNotify_SetCallback( PFN_REG_NOTIFY_LIB_CALLBACK pfnRegNotifyLibCB )
{
	BOOL bRet = TRUE;
	
	if( pfnRegNotifyLibCB )
	{
		pThisContent->pfnRegNotifyLibCB = pfnRegNotifyLibCB;
		spLibSetupContent( pThisContent );
	}	
	else
		bRet = FALSE;
	
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


static BOOL spLibInitContent( LibRegNotifyContent* pThis, LPCTSTR KeyStr )
{
	BOOL bRet = TRUE;

	pThis->hNotifyHandle = NULL;
	pThis->hNotifyControl = NULL;
	pThis->hMonitorThread = NULL;
	pThis->pfnRegNotifyLibCB = NULL;
	memset( pThis->KeyPathName, 0, 256 );

	if( NULL == KeyStr )
		///strcpy( pThis->KeyPathName, (char *)MIC_OALDBGMSG_REG_KEY );
		wcscpy( (TCHAR *)pThis->KeyPathName, MIC_OALDBGMSG_REG_KEY );
	else
		strcpy( pThis->KeyPathName, (char *)KeyStr );
		///wcscpy( pThis->KeyPathName, KeyStr );

	spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitContent %s %s !!!"), SPPREFIX, KeyStr, pThis->KeyPathName );	
	return bRet;
}

static BOOL spLibSetupContent( LibRegNotifyContent* pThis )
{
	BOOL bRet = TRUE;

	if( pThis )
	{
		
		bRet = spLibInitEvent( pThis );
		
		///create thread only with callback function set
		if( bRet && pThis->pfnRegNotifyLibCB )
			bRet = spLibInitCreateThread( pThis );
	}
	
	return bRet;
}


static BOOL spLibDeInitContent( LibRegNotifyContent* pThis )
{
	BOOL bRet = TRUE;

	SetEvent( pThis->hNotifyControl );
	Sleep( 500 );
	
	///CloseHandle( pThis->hNotifyHandle );
	CeFindCloseRegChange( pThis->hNotifyHandle );
	CloseHandle( pThis->hNotifyControl );
	CloseHandle( pThis->hMonitorThread );
	RegCloseKey( pThis->hKey );
	
	pThis->hNotifyHandle = NULL;
	pThis->hNotifyControl = NULL;
	pThis->hMonitorThread = NULL;

	return bRet;
}


/// default at [HKEY_LOCAL_MACHINE\Software\Mitac\micOALdebugMsg]
static BOOL spLibInitEvent( LibRegNotifyContent* pThis )
{
	BOOL bRet = TRUE;
	///HKEY hKey = 0;

	if( NULL != pThis )
	{	
	
		if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, (LPCWSTR)pThis->KeyPathName , 0, 0, &pThis->hKey ) )
		{
			pThis->hNotifyHandle = CeFindFirstRegChange( pThis->hKey, TRUE, REG_NOTIFY_CHANGE_LAST_SET );
			if( INVALID_HANDLE_VALUE == pThis->hNotifyHandle )
			{
				spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitEvent CeFindFirstRegChange fail %d !!!"), SPPREFIX, GetLastError() );
				bRet = FALSE;
			}
		}
		else
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitEvent RegOpenKeyEx fail %d !!!"), SPPREFIX, GetLastError() );
			bRet = FALSE;
		}
		
		if( bRet )
		{
			///pThis->hNotifyHandle = CreateEvent( NULL, FALSE, FALSE, NULL );
			pThis->hNotifyControl = CreateEvent( NULL, FALSE, FALSE, NULL );
		}
		
		if( !pThis->hNotifyControl || !pThis->hNotifyHandle )
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitEvent fail !!!"), SPPREFIX );
			bRet = FALSE;
		}	
	}
	else
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitEvent NULL fail !!!"), SPPREFIX );
		bRet = FALSE;
	}
	
	return bRet;
}


static BOOL spLibInitCreateThread( LibRegNotifyContent* pThis )
{
	BOOL bRet = TRUE;
	DWORD dwThreadId = 0;
	DWORD dwThreadPararm = (DWORD)pThis;
	
	pThis->hMonitorThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)MonitorEventThread, (LPVOID)&dwThreadPararm, 0, (LPDWORD)&dwThreadId );
	
	if( NULL == pThis->hMonitorThread )
	{
		spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitCreateThread fail !!!"), SPPREFIX );
		bRet = FALSE;
	}
	else	
		;
	
	return bRet;
}


static DWORD WINAPI MonitorEventThread( LPVOID pContext )
{
#define		WAITEVENT_NUM		2

	DWORD dwRet = 0;
	DWORD dwReturns = 0;
	BOOL bExitMonitor = FALSE;
	HANDLE hWaitEvents[WAITEVENT_NUM];
	DWORD dwWaitMS = INFINITE;
	LibRegNotifyContent* pThis = (LibRegNotifyContent *)(*(DWORD *)pContext);
	
	do
	{
		if( NULL == pThis )
			break;
		
		hWaitEvents[0] = pThis->hNotifyHandle;
		hWaitEvents[1] = pThis->hNotifyControl;
	
		spLibDbgMsg( LIBMSGFLAG_NK, TEXT("%s MonitorEventThread start !!!"), SPPREFIX );
	
		while( !bExitMonitor )
		{
			dwReturns = WaitForMultipleObjects( WAITEVENT_NUM, hWaitEvents, FALSE, dwWaitMS );
		
			switch( dwReturns )
			{
				case WAIT_OBJECT_0 + 0:
					/// got notify
					///no message here, or you will get a endless message loop...
					///spLibDbgMsg( LIBMSGFLAG_NULL, TEXT("%s MonitorEventThread 1 !!!"), SPPREFIX );
					if( pThis->pfnRegNotifyLibCB )
					{
						///pThis->pfnRegNotifyLibCB( 0 );
						pThis->pfnRegNotifyLibCB( spLibGetRegDWORD( pThis ) );
					}	
					CeFindNextRegChange( hWaitEvents[0] );
					break;
				case WAIT_OBJECT_0 + 1:
					/// got control event
					spLibDbgMsg( LIBMSGFLAG_NK, TEXT("%s MonitorEventThread 2 !!!"), SPPREFIX );
					bExitMonitor = TRUE;
					
					break;
				case WAIT_TIMEOUT:
					break;
				default:
					break;
				
			}	///switch
		
		}	///while
	}while(0);
	
	
	spLibDbgMsg( LIBMSGFLAG, TEXT("%s MonitorEventThread exit !!!"), SPPREFIX );
	
	return dwRet;
}


static DWORD spLibGetRegDWORD( LibRegNotifyContent* pThis )
{
	BOOL bRet = FALSE;
	DWORD dwTol = 0;
	DWORD dwRet = 0;
	DWORD dwType = REG_DWORD;
	DWORD dwLen = sizeof(DWORD);
	HKEY hKey = 0;
	
	if( pThis )
	{
		if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, (LPCWSTR)pThis->KeyPathName , 0, 0, &hKey ) )
		{
			if( ERROR_SUCCESS == RegQueryValueEx( hKey, TEXT("Total"), 0, &dwType, (LPBYTE)&dwTol, &dwLen ) )
			{
				dwRet = dwTol;
				bRet = TRUE;
			}
			else
			{
				spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitEvent RegQueryValueEx fail %d !!!"), SPPREFIX, GetLastError() );
				bRet = FALSE;
			}

			RegCloseKey( hKey );
		}
		else
		{
			spLibDbgMsg( LIBMSGFLAG, TEXT("%s spLibInitEvent RegOpenKeyEx fail %d !!!!"), SPPREFIX, GetLastError() );
			bRet = FALSE;
		}
	}	
	
	if( FALSE == bRet )
		dwRet = (-1);
	
	return dwRet;
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

/*
//# Notification Thread
DWORD NotifyThread(LPVOID lpParameter)
{
        // Animation setting: Home.lock.enableAnimationOnPower

        //-- Manila Change Notify
        RegOpenKeyEx(HKEY_CURRENT_USER,__MANILAPATH,0,KEY_READ,&HKeys[0]);
        HNotf[0]=CeFindFirstRegChange(HKeys[0],FALSE,REG_NOTIFY_CHANGE_LAST_SET);

        //-- Phone State Notify
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"\\System\\State\\Phone",0,KEY_READ,&HKeys[1]);
        HNotf[1]=CeFindFirstRegChange(HKeys[1],FALSE,REG_NOTIFY_CHANGE_LAST_SET);

        //-- Terminating Event
        HNotf[2]=CreateEvent(NULL,FALSE,FALSE,L"TerminateManilaLock");

        //-- Power State Queue
        MSGQUEUEOPTIONS msgqOpt = {0};
        msgqOpt.dwSize                  = sizeof(MSGQUEUEOPTIONS);
    msgqOpt.dwFlags                     = 0;
    msgqOpt.dwMaxMessages       = QUEUE_ENTRIES;
    msgqOpt.cbMaxMessage        = sizeof(POWER_BROADCAST) + MAX_NAMELEN;
    msgqOpt.bReadAccess         = TRUE;
        HNotf[3]                = CreateMsgQueue(NULL, &msgqOpt);
        HANDLE hPwrNoti = RequestPowerNotifications(HNotf[3], PBT_TRANSITION|PBT_RESUME|PBT_POWERINFOCHANGE|PBT_POWERSTATUSCHANGE);

        while(ThreadStarted){
                DWORD RetB = WaitForMultipleObjects(4,HNotf,FALSE,INFINITE);
                if (RetB==WAIT_OBJECT_0+3){
                        //## Power Notification
                        PMNotification(HNotf[3]);
                }
                else if ((RetB>=WAIT_OBJECT_0) && (RetB<=WAIT_OBJECT_0+1)){
                        //## Phone Reg Change Signals & Manila Store Change Signals
                        DWORD NotifyNum = RetB - WAIT_OBJECT_0;
                        if (isLockEnabled!=0){
                                SendMessage(mainWnd,WM_USER+NotifyNum,0,0);
                        }
                        CeFindNextRegChange (HNotf[NotifyNum]);
                }
                else if (RetB==WAIT_OBJECT_0+2){
                        //## Terminate Signal
                        ResetEvent(HNotf[2]);
                        break;
                }
        }

        //-- Clean Up Reg&RegNotify
        for (int i=0;i<=1;i++){
                CeFindCloseRegChange(HNotf[i]);
                RegCloseKey(HKeys[i]);
        }

        //-- Clean Up Power State Handles
        StopPowerNotifications(hPwrNoti);
        CloseMsgQueue(HNotf[3]);
        CloseHandle(hPwrNoti);

        //-- Clean Up Handles
        for (int i=0;i<=3;i++)
                CloseHandle(HNotf[i]);

        return 1;
}


                LONG    status;
                HKEY    dagkey;
                DWORD   lptype;
                DWORD   lpcbdata;
                int             postype = 0;
                char    keyname[512];

                snprintf(keyname, sizeof(keyname), "%s\\CardParams\\%s",
                        "SYSTEM\\CurrentControlSet\\Services\\DAG",
                        strstr(_strlwr((char*)device), "dag"));
                do
                {
                        status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyname, 0, KEY_READ, &dagkey);
                        if(status != ERROR_SUCCESS)

						
						
						// ******************************************************************
// Local Functions
// ******************************************************************
static LONG findDrvPath (char *clsidstr,char *dllpath,int dllpathsize)
{
        HKEY                    hkEnum,hksub,hkpath;
        char                    databuf[512];
        LONG                    cr,rc = -1;
        DWORD                   datatype,datasize;
        DWORD                   index;
        OFSTRUCT                ofs;
        HFILE                   hfile;
        BOOL                    found = FALSE;

        CharLowerBuff(clsidstr,strlen(clsidstr));
        if ((cr = RegOpenKey(HKEY_CLASSES_ROOT,COM_CLSID,&hkEnum)) == ERROR_SUCCESS) {

                index = 0;
                while (cr == ERROR_SUCCESS && !found) {
                        cr = RegEnumKey(hkEnum,index++,(LPTSTR)databuf,512);
                        if (cr == ERROR_SUCCESS) {
                                CharLowerBuff(databuf,strlen(databuf));
                                if (!(strcmp(databuf,clsidstr))) {
                                        if ((cr = RegOpenKeyEx(hkEnum,(LPCTSTR)databuf,0,KEY_READ,&hksub)) == ERROR_SUCCESS) {
                                                if ((cr = RegOpenKeyEx(hksub,(LPCTSTR)INPROC_SERVER,0,KEY_READ,&hkpath)) == ERROR_SUCCESS) {
                                                        datatype = REG_SZ; datasize = (DWORD)dllpathsize;
                                                        cr = RegQueryValueEx(hkpath,0,0,&datatype,(LPBYTE)dllpath,&datasize);
                                                        if (cr == ERROR_SUCCESS) {
                                                                memset(&ofs,0,sizeof(OFSTRUCT));
                                                                ofs.cBytes = sizeof(OFSTRUCT);
                                                                hfile = OpenFile(dllpath,&ofs,OF_EXIST);
                                                                if (hfile) rc = 0;
                                                        }
                                                        RegCloseKey(hkpath);
                                                }
                                                RegCloseKey(hksub);
                                        }
                                        found = TRUE;   // break out
                                }
                        }
                }
                RegCloseKey(hkEnum);
        }
        return rc;
}

*/
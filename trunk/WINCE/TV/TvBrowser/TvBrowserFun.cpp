// TvBrowserFun.cpp: implementation of the TvBrowserFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TvBrowserFun.h"
#include "SBuffer.h"		///span1110
#include "LogClass.h"		///span1227
#include "resource.h"		///span1226
///#include "pm.h"		///span0118
#include "TvOverlayFun.h"

typedef DWORD (WINAPI * PFN_GetSystemPowerStatusEx2)( PSYSTEM_POWER_STATUS_EX2, DWORD, BOOL );

EXTERN_C DWORD GetEventData(HANDLE hEvent);
EXTERN_C BOOL SetEventData(HANDLE hEvent, DWORD dwData); 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HINSTANCE ghLibInst;
PFNSysSendMessage pSysSendMessage;
PFNSysGetMessage pSysGetMessage;

HANDLE hDTV;
DWORD dwBufOutSize = 256*1024;
DWORD dwActSize;
DWORD g_dwModBattery = 0;

extern HINSTANCE hInst;          // The current instance
extern HWND    g_hWnd;
extern HWND    g_hDlg;
extern DWORD dwProjectCode;
DWORD dwEncryptCode = 0;
static ISDB_T_SETCHANNEL ChServSave;

// APIMessage MsgIn, MsgOut;
PBYTE pBufOut;
PSvcInfo pEnsembleList[MAX_ENSEMBLE_NUM];
DWORD dwSvcListCount[MAX_ENSEMBLE_NUM];
DWORD CurrEnsembleId, CurrSubChId;
BYTE uCurrVol;
CarrierStatus CS;
SYS_ERROR nRet;

SBuffer *pFBuffer1;		///span1109
SBuffer *pFBuffer2;		///span1109
SBuffer *pFBuffer3;		///span1109
SBuffer *pFBuffer4;		///span1109
SFInFOut *pFIFO;		///span1114
SLogClass *pLog = NULL;	///span1227
MIO_ISDB_TS_FILE_HEADER ISDBTSFileHeader;

TCHAR szString[64];         	///span1226, Temperary string
BOOL bCNSPwrOn = FALSE;
BOOL bVideoDisplay = FALSE;

CHANNEL_INFO ISDBallChannelstatus[50][8];///50 channel 8 service in each channel
CHANNEL_INFO ISDBcurrChannelstatus;

ISDB_T_CH_SERVICE_INFO ChServInfo;
ISDB_T_CH_DETAIL_INFO ChDetailInfo;

DWORD dwProgramContentChanged = 0;
DWORD dwServiceContentChanged = 0;
DWORD dwProgramTOTChanged = 0;
DWORD dwServiceEWSChanged = 0;
DWORD dwServiceEPGChanged = 0;


///DTV_STATE	g_DtvStatus = PWROFF;
///DWORD g_dwDtvStatus = DVT_STATE_PWROFF;


void InitChannelListBoxISDBT(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 13");///index 0
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 14");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 15");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 16");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 17");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 18");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 19");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 20");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 21");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 22");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 23");///index 10
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 24");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 25");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 26");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 27");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 28");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 29");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 30");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 31");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 32");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 33");///index 20
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 34");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 35");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 36");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 37");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 38");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 39");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 40");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 41");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 42");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 43");///index 30
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 44");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 45");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 46");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 47");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 48");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 49");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 50");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 51");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 52");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 53");///index 40
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 54");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 55");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 56");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 57");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 58");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 59");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 60");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 61");
	SendDlgItemMessage(hDlg, IDC_CHLIST, CB_ADDSTRING, 0, (LPARAM)L"CH 62");///index 49
    SendMessage(GetDlgItem(hDlg, IDC_CHLIST), CB_SETCURSEL, (WPARAM)10, 0);	///set default 23


	SendDlgItemMessage(hDlg, IDC_SERVLIST, CB_ADDSTRING, 0, (LPARAM)L"SerV 0");///index 0
	SendDlgItemMessage(hDlg, IDC_SERVLIST, CB_ADDSTRING, 0, (LPARAM)L"SerV 1");
	SendDlgItemMessage(hDlg, IDC_SERVLIST, CB_ADDSTRING, 0, (LPARAM)L"SerV 2");
	SendDlgItemMessage(hDlg, IDC_SERVLIST, CB_ADDSTRING, 0, (LPARAM)L"SerV 3");
	SendDlgItemMessage(hDlg, IDC_SERVLIST, CB_ADDSTRING, 0, (LPARAM)L"SerV 4");
	SendDlgItemMessage(hDlg, IDC_SERVLIST, CB_ADDSTRING, 0, (LPARAM)L"SerV 5");
	SendDlgItemMessage(hDlg, IDC_SERVLIST, CB_ADDSTRING, 0, (LPARAM)L"SerV 6");
	SendDlgItemMessage(hDlg, IDC_SERVLIST, CB_ADDSTRING, 0, (LPARAM)L"SerV 7");
    SendMessage(GetDlgItem(hDlg, IDC_SERVLIST), CB_SETCURSEL, (WPARAM)0, 0);	///set default 0
    
    NKDbgPrintfW( L"TVB:: InitChannelListBoxISDBT()\r\n" );
}

BOOL ISDBallChannelOperation( DWORD dwOperation, PCHANNEL_INFO pChannelInfo )
{
	BOOL bRet = FALSE;
	PCHANNEL_INFO pAllChannelInfo;
	///DWORD dwChIndex;
	///DWORD dwServIndex;
	DWORD dwLoop = 0;
	DWORD dwFoundLoop = 0;
	DWORD dwIndex = 0;
	BOOL bFoundNext = FALSE;

	
	pAllChannelInfo = &ISDBallChannelstatus[0][0];
	
	if( Clear_allChannel == dwOperation ) {
		for( dwLoop=0; dwLoop<(50*8); dwLoop++ ) {
			pAllChannelInfo[dwLoop].ChNum.dwChannelNum = 0;
			pAllChannelInfo[dwLoop].ChNum.dwBranchNum = 0;
			pAllChannelInfo[dwLoop].dwPhyChNum = 0;
			pAllChannelInfo[dwLoop].dwServiceId = 0;
			pAllChannelInfo[dwLoop].dwAudioMode = 0;
			pAllChannelInfo[dwLoop].dwTSId = 0;
			pAllChannelInfo[dwLoop].dwCapLanguageId = 0;
		}
	} else
	if( Up_allChannel == dwOperation ) {

		///find current
		for( dwLoop=0; dwLoop<(50*8); dwLoop++ ) {
			if( 
				pAllChannelInfo[dwLoop].dwPhyChNum == ISDBcurrChannelstatus.dwPhyChNum &&
				pAllChannelInfo[dwLoop].dwServiceId == ISDBcurrChannelstatus.dwServiceId
			) {
				dwIndex = dwLoop;
				break;
			}
		}
		
		///search from current+1
		for( dwLoop=dwIndex+1; dwLoop<(50*8); dwLoop++ ) {
			if( 
				pAllChannelInfo[dwLoop].dwPhyChNum != 0
			) {
				bFoundNext = TRUE;
				dwFoundLoop = dwLoop;
				break;
			}
		}

		///search from 0 if there is no availiable after current
		if( !bFoundNext ) {
			for( dwLoop=0; dwLoop<dwIndex; dwLoop++ ) {
				if( 
					pAllChannelInfo[dwLoop].dwPhyChNum != 0
				) {
					bFoundNext = TRUE;
					dwFoundLoop = dwLoop;
					break;
				}
			}
		}

		if( bFoundNext ) {
			bRet = TRUE;

			pChannelInfo->ChNum.dwChannelNum = pAllChannelInfo[dwFoundLoop].ChNum.dwChannelNum;
			pChannelInfo->ChNum.dwBranchNum = pAllChannelInfo[dwFoundLoop].ChNum.dwBranchNum;
			pChannelInfo->dwPhyChNum = pAllChannelInfo[dwFoundLoop].dwPhyChNum;
			pChannelInfo->dwServiceId = pAllChannelInfo[dwFoundLoop].dwServiceId;
			pChannelInfo->dwAudioMode = pAllChannelInfo[dwFoundLoop].dwAudioMode;
			pChannelInfo->dwTSId = pAllChannelInfo[dwFoundLoop].dwTSId;
			pChannelInfo->dwCapLanguageId = pAllChannelInfo[dwFoundLoop].dwCapLanguageId;

		} else {
			bRet = FALSE;

			pChannelInfo->ChNum.dwChannelNum = 0;
			pChannelInfo->ChNum.dwBranchNum = 0;
			pChannelInfo->dwPhyChNum = 0;
			pChannelInfo->dwServiceId = 0;
			pChannelInfo->dwAudioMode = 0;
			pChannelInfo->dwTSId = 0;
			pChannelInfo->dwCapLanguageId = 0;

		}

       	wsprintf( szString, TEXT("NowCh=%d,%d"), ISDBcurrChannelstatus.dwPhyChNum, ISDBcurrChannelstatus.dwServiceId );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	

       	wsprintf( szString, TEXT("NextCh=%d,%d"), pChannelInfo->dwPhyChNum, pChannelInfo->dwServiceId );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	

		
	} else
	if( Down_allChannel == dwOperation ) {

		///DWORD dwIndex = 0;
		///BOOL bFoundNext = FALSE;
		
		///find current
		for( dwLoop=0; dwLoop<(50*8); dwLoop++ ) {
			if( 
				pAllChannelInfo[dwLoop].dwPhyChNum == ISDBcurrChannelstatus.dwPhyChNum &&
				pAllChannelInfo[dwLoop].dwServiceId == ISDBcurrChannelstatus.dwServiceId
			) {
				dwIndex = dwLoop;
				break;
			}
		}
	
		///search from current-1
		if( dwIndex > 0 ) {
			for( dwLoop=dwIndex-1; dwLoop>=0; dwLoop-- ) {
				if( 
					pAllChannelInfo[dwLoop].dwPhyChNum != 0
				) {
					bFoundNext = TRUE;
					dwFoundLoop = dwLoop;
					break;
				}
			}
		} else {	///dwIndex == 0
			for( dwLoop=((50*8)-1); dwLoop>=0; dwLoop-- ) {
				if( 
					pAllChannelInfo[dwLoop].dwPhyChNum != 0
				) {
					bFoundNext = TRUE;
					dwFoundLoop = dwLoop;
					break;
				}
			}
		}

		if( !bFoundNext ) {
			for( dwLoop=(50*8); dwLoop>dwIndex; dwLoop-- ) {
				if( 
					pAllChannelInfo[dwLoop].dwPhyChNum != 0
				) {
					bFoundNext = TRUE;
					dwFoundLoop = dwLoop;
					break;
				}
			}
		}

		if( bFoundNext ) {
			bRet = TRUE;

			pChannelInfo->ChNum.dwChannelNum = pAllChannelInfo[dwFoundLoop].ChNum.dwChannelNum;
			pChannelInfo->ChNum.dwBranchNum = pAllChannelInfo[dwFoundLoop].ChNum.dwBranchNum;
			pChannelInfo->dwPhyChNum = pAllChannelInfo[dwFoundLoop].dwPhyChNum;
			pChannelInfo->dwServiceId = pAllChannelInfo[dwFoundLoop].dwServiceId;
			pChannelInfo->dwAudioMode = pAllChannelInfo[dwFoundLoop].dwAudioMode;
			pChannelInfo->dwTSId = pAllChannelInfo[dwFoundLoop].dwTSId;
			pChannelInfo->dwCapLanguageId = pAllChannelInfo[dwFoundLoop].dwCapLanguageId;


		} else {
			bRet = FALSE;

			pChannelInfo->ChNum.dwChannelNum = 0;
			pChannelInfo->ChNum.dwBranchNum = 0;
			pChannelInfo->dwPhyChNum = 0;
			pChannelInfo->dwServiceId = 0;
			pChannelInfo->dwAudioMode = 0;
			pChannelInfo->dwTSId = 0;
			pChannelInfo->dwCapLanguageId = 0;

		}

       	wsprintf( szString, TEXT("NowCh=%d,%d"), ISDBcurrChannelstatus.dwPhyChNum, ISDBcurrChannelstatus.dwServiceId );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	

       	wsprintf( szString, TEXT("NextCh=%d,%d"), pChannelInfo->dwPhyChNum, pChannelInfo->dwServiceId );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	

		
	} else {
		NKDbgPrintfW( L"TVB:: ISDBallChannelOperation() error operation code!!\r\n" );
	}
	
	
	
	return bRet;
}

void ClearChannelListBox(HWND hDlg)
{
	do {
		SendDlgItemMessage(hDlg, IDC_CHLIST, CB_DELETESTRING, 0, 0);
	} while( 0 != SendDlgItemMessage(hDlg, IDC_CHLIST, CB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

	do {
		SendDlgItemMessage(hDlg, IDC_SERVLIST, CB_DELETESTRING, 0, 0);
	} while( 0 != SendDlgItemMessage(hDlg, IDC_SERVLIST, CB_GETCOUNT, (WPARAM)0 , (LPARAM)0) );

	NKDbgPrintfW( L"TVB:: ClearChannelListBox()\r\n" );
}

BOOL DumpDriverReg()
{
	BOOL bRet = TRUE;

    DWORD dwStatus = 0;
    DWORD dwSetValue = 0;
    HKEY hKey;
    TCHAR szVersion[128] = TEXT("DTV driver Ver 0.0");

    
    dwStatus = RegOpenKeyEx( HKEY_LOCAL_MACHINE, MIC_DTV_DRV_REG_KEY, 0, 0, &hKey );


	///dump registry
	if( TRUE ) {
		DWORD dwType;///, dwSize, dwVal;
	    DWORD dwIndex = 0;
	    WCHAR szValueName[MAX_PATH];
	    DWORD dwValueNameSize = MAX_PATH;
	    BYTE pValueData[256];
	    DWORD dwValueDataSize = sizeof(pValueData);	 

		wsprintf( szString, TEXT("Dump driver Reg...") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	
		while( 
			ERROR_SUCCESS == RegEnumValue( hKey, dwIndex, szValueName, &dwValueNameSize, NULL, &dwType, pValueData, &dwValueDataSize )
		)
		{
	        if(REG_SZ == dwType) {
	        	NKDbgPrintfW( L" RegEnumValue-%s=%s\r\n", szValueName, (LPWSTR)pValueData );
			    wsprintf( szString, TEXT("%s=%s"), szValueName, (LPWSTR)pValueData );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	        }
	        else
	        if(REG_DWORD == dwType) {
	            NKDbgPrintfW( L" RegEnumValue-%s=0x%08X\r\n", szValueName, *(PDWORD)pValueData );
			    wsprintf( szString, TEXT("%s=0x%08X"), szValueName, *(PDWORD)pValueData );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				if ( ((*(PDWORD)pValueData)&0xFF000000) == (MIC_DTV_DRV_PROJECT_CODE&0xFF000000) ) {
					dwProjectCode = *(PDWORD)pValueData;
					NKDbgPrintfW( L" Get project code =0x%08X\r\n", dwProjectCode );
					ISDBTSFileHeader.dwProjectCode = dwProjectCode;
					ISDBTSFileHeader.dwSubProjectCode = 0;
				}	
				if ( ((*(PDWORD)pValueData)&0xFFFFFF00) == (MIC_DTV_DRV_TS_ENCRYPTION_CODE&0xFFFFFF00) ) {
					dwEncryptCode = *(PDWORD)pValueData;
					NKDbgPrintfW( L" Get encryption code =0x%08X\r\n", dwEncryptCode );
					ISDBTSFileHeader.dwEncrypted = (dwEncryptCode&0x000000FF);
				}	
	        }
	        else
	        if(REG_MULTI_SZ == dwType) {
			    BYTE pPathValueData[256];
	    		DWORD dwPathValueDataSize = sizeof(pPathValueData);
	            PWSTR pValueTemp = (PWSTR)pValueData;
	            PWSTR pPathTemp;
	            
	            wcscpy( (PWSTR)pPathValueData, (PWSTR)pValueData );
	            pPathTemp = (PWSTR)pPathValueData;
	            
	            ///NKDbgPrintfW( L"%s:RegEnumValue-%s=", PROGNAME, szValueName );
	            while( *pValueTemp )
	            {
	            	NKDbgPrintfW( L"%s ", (LPWSTR)pValueTemp );
	                pValueTemp += (wcslen(pValueTemp) + 1);
	                pPathTemp += (wcslen(pPathTemp));
	                wcscpy( (PWSTR)pPathTemp, (PWSTR)pValueTemp );
				}
				NKDbgPrintfW( L"\r\n" );
				
				NKDbgPrintfW( L" RegEnumValue-%s=%s\r\n", szValueName, (LPWSTR)pPathValueData );        			
			    wsprintf( szString, TEXT("%s=%s"), szValueName, (LPWSTR)pPathValueData );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

	    	}	
	        else
	        	NKDbgPrintfW( L"%s:RegEnumValue-unknow type\r\n" );
	
			
			///Next ...
	        dwIndex++;
	        dwValueDataSize = sizeof(pValueData);
	        dwValueNameSize = MAX_PATH;
		}	///while( ....

		if( 1 ) {
			DWORD dwValueType = 0;
			DWORD dwValueData = 0;
			DWORD dwValueLen = 0;
			
			///check value in key
			dwValueType = REG_DWORD;
			dwValueLen = sizeof( dwValueData );
		    if( ERROR_SUCCESS == RegQueryValueEx( hKey, TEXT("MajorVer"), NULL, &dwValueType, (BYTE *)&dwValueData, &dwValueLen ) )
    		{
    			ISDBTSFileHeader.dwDrvMajorVer = dwValueData;
    		}

			///check value in key
			dwValueType = REG_DWORD;
			dwValueLen = sizeof( dwValueData );
		    if( ERROR_SUCCESS == RegQueryValueEx( hKey, TEXT("MinorVer"), NULL, &dwValueType, (BYTE *)&dwValueData, &dwValueLen ) )
    		{
    			ISDBTSFileHeader.dwDrvMinorVer = dwValueData;
    		}
		}
		
	} /// if( ) {
	
	return bRet;
}


void TvBrowserFunInit()
{
    int i;
    HANDLE hCommandThread = NULL;
    HANDLE hDrvEventThread = NULL;

    NKDbgPrintfW( L"TVB:: Init() + \r\n" );

    hDTV = CreateFile( L"DTV1:", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);

#if 0
    ghLibInst = LoadLibrary(L"dtv.dll");
    pSysSendMessage = (PFNSysSendMessage)GetProcAddress(ghLibInst, L"SysSendMessage");
    if ( pSysSendMessage == NULL )
        NKDbgPrintfW( L"TVB:: failed to get PTR of SysSendMessage\r\n" );
    pSysGetMessage = (PFNSysGetMessage)GetProcAddress(ghLibInst, L"SysGetMessage");
    if ( pSysGetMessage == NULL )
        NKDbgPrintfW( L"TVB:: failed to get PTR of SysGetMessage\r\n" );
#endif

    pBufOut = (PBYTE)LocalAlloc(LPTR, 256*1024);


	DumpDriverReg();

	if( dwProjectCode == 0x67333130 ) {	///PT310
 		pFBuffer1 = new SBuffer( 192*10*4 );		///span70418
		pFBuffer2 = new SBuffer( 192*10*4 );		///span70418
		pFBuffer3 = new SBuffer( 192*10*4 );		///span70418
		pFBuffer4 = new SBuffer( 192*10*4 );		///span70418
		pFIFO = new SFInFOut( 192*10*4 );			///span70418
		
		///init TS file header
		ISDBTSFileHeader.bFileID[0] = 'T';
		ISDBTSFileHeader.bFileID[1] = 'S';
		ISDBTSFileHeader.bFileID[2] = '_';
		ISDBTSFileHeader.bFileID[3] = '_';
		ISDBTSFileHeader.bCorpName[0] = 'M';
		ISDBTSFileHeader.bCorpName[1] = 'i';
		ISDBTSFileHeader.bCorpName[2] = 't';
		ISDBTSFileHeader.bCorpName[3] = 'a';
		ISDBTSFileHeader.bCorpName[4] = 'c';
		ISDBTSFileHeader.bCorpName[5] = '_';
		ISDBTSFileHeader.bCorpName[6] = '_';
		ISDBTSFileHeader.bCorpName[7] = '_';
		ISDBTSFileHeader.bTStype[0] = 'I';
		ISDBTSFileHeader.bTStype[1] = 'S';
		ISDBTSFileHeader.bTStype[2] = 'D';
		ISDBTSFileHeader.bTStype[3] = 'B';
		ISDBTSFileHeader.bTStype[4] = '-';
		ISDBTSFileHeader.bTStype[5] = 'T';
		ISDBTSFileHeader.bTStype[6] = '_';
		ISDBTSFileHeader.bTStype[7] = '_';
		ISDBTSFileHeader.dwHeaderSize = sizeof(MIO_ISDB_TS_FILE_HEADER);
		ISDBTSFileHeader.dwTSDataOffset = sizeof(MIO_ISDB_TS_FILE_HEADER);

		ISDBTSFileHeader.dwFileSize = 0;
		ISDBTSFileHeader.dwDuration = 0;


		ISDBTSFileHeader.bDeviceName[0] = 'P';
		ISDBTSFileHeader.bDeviceName[1] = 'T';
		ISDBTSFileHeader.bDeviceName[2] = '3';
		ISDBTSFileHeader.bDeviceName[3] = '1';
		ISDBTSFileHeader.bDeviceName[4] = '0';
		ISDBTSFileHeader.bDeviceName[5] = '_';
		ISDBTSFileHeader.bDeviceName[6] = '_';
		ISDBTSFileHeader.bDeviceName[7] = '_';

	} else {
 		pFBuffer1 = new SBuffer();		///span1109
		pFBuffer2 = new SBuffer();		///span1109
		pFBuffer3 = new SBuffer();		///span1109
		pFBuffer4 = new SBuffer();		///span1109
		pFIFO = new SFInFOut();			///span1114
	}


    for (i=0; i<MAX_ENSEMBLE_NUM; i++) {
        pEnsembleList[i] = NULL;
        dwSvcListCount[i] = 0;
    }
    
    CurrEnsembleId = 0;
    CurrSubChId = 0;

#if 0
    // SetVideoFormat656();
    SetVolume(5);
	#if 0
    NKDbgPrintfW( L"TVB:: set video size small\r\n" );
    SetVideoOutSize(240, 136);
    SetVideoPos(16, 200);
	#else
    NKDbgPrintfW( L"TVB:: set video size large\r\n" );
    SetVideoOutSize(480, 240);
    SetVideoPos(0, 0);
	#endif
#endif


	hCommandThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)TvCommandThread, NULL, 0, NULL);

	hDrvEventThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)TvDrvEventThread, NULL, 0, NULL);
	
	///DumpDriverReg();
	
    NKDbgPrintfW( L"TVB:: Init() - \r\n" );
}

void TvBrowserFunDeinit()
{
    int i;
	
	NKDbgPrintfW( L"TVB:: TvBrowserFunDeinit\r\n" );
    TvVedioOff();

    for (i=0; i<MAX_ENSEMBLE_NUM; i++) {
        if ( pEnsembleList[i] )
            LocalFree(pEnsembleList[i]);
    }

 	delete pFBuffer1;		///span1109
	delete pFBuffer2;		///span1109
	delete pFBuffer3;		///span1109	
	delete pFBuffer4;		///span1109	

    LocalFree(pBufOut);
}

DWORD SwapDword(DWORD dwVal)
{
    DWORD dwRet;
    
    dwRet = ((dwVal & 0xff000000) >> 24) |
            ((dwVal & 0x00ff0000) >> 8)  |
            ((dwVal & 0x0000ff00) << 8)  |
            ((dwVal & 0x000000ff) << 24);
    
    return dwRet;
}

WORD SwapWord(WORD wVal)
{
    WORD wRet;
    
    wRet = ((wVal & 0xff00) >> 8) |
           ((wVal & 0x00ff) << 8);
    
    return wRet;
}


BOOL ParseErrorCode( PBYTE pErrBuf )
{
    BOOL bRet = TRUE;
    PCNS_SPI_FRAME_HEADER pTempHead = NULL;
    DWORD dwErrorCode = 0;
    
    if( NULL == pErrBuf )
        pErrBuf = pBufOut;

    if( NULL == pErrBuf )
        bRet = FALSE;

    if( bRet ) {    
        pTempHead = (PCNS_SPI_FRAME_HEADER)pErrBuf;
        
        NKDbgPrintfW(  L"TVB::ParseErrorCode attribute = 0x%02x\r\n", pTempHead->aAttribute );
        NKDbgPrintfW(  L"TVB::ParseErrorCode id = 0x%02x\r\n", pTempHead->idID );
        NKDbgPrintfW(  L"TVB::ParseErrorCode payloadlen = %d\r\n", SwapWord( pTempHead->plPayLoadLength )  );
        
	    wsprintf( szString, TEXT("RSP ERR 0x%02x 0x%02x "), pTempHead->aAttribute, pTempHead->idID );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

        pTempHead++;    ///move to payload pointer
        
        dwErrorCode = SwapDword(*(PDWORD)pTempHead );
        NKDbgPrintfW(  L"TVB::ParseErrorCode payload = 0x%08x\r\n", dwErrorCode );
        
        switch( dwErrorCode )
        {
            case CNS_SPI_CMD_ERROR_INVALID_PARAMETER:
                NKDbgPrintfW(  L"TVB::ParseErrorCode ERROR_INVALID_PARAMETER\r\n" );
		    	wsprintf( szString, TEXT("ERROR_INVALID_PARAMETER") );
                break;
            case CNS_SPI_CMD_ERROR_NOT_FOUND:
                NKDbgPrintfW(  L"TVB::ParseErrorCode ERROR_NOT_FOUND\r\n" );
                wsprintf( szString, TEXT("ERROR_NOT_FOUND") );
                break;
            case CNS_SPI_CMD_ERROR_NOT_ACTIVE:
                NKDbgPrintfW(  L"TVB::ParseErrorCode ERROR_NOT_ACTIVE\r\n" );
                wsprintf( szString, TEXT("ERROR_NOT_ACTIVE") );
                break;
            case CNS_SPI_CMD_ERROR_COMMAND_FAIL:
                NKDbgPrintfW(  L"TVB::ParseErrorCode ERROR_COMMAND_FAIL\r\n" );
                wsprintf( szString, TEXT("ERROR_COMMAND_FAIL") );
                break;
            case CNS_SPI_CMD_ERROR_NOT_STREAM:
                NKDbgPrintfW(  L"TVB::ParseErrorCode ERROR_NOT_STREAM\r\n" );
                wsprintf( szString, TEXT("ERROR_NOT_STREAM") );
                break;
            case CNS_SPI_CMD_ERROR_OVERFLOW_STREAM:
                NKDbgPrintfW(  L"TVB::ParseErrorCode ERROR_OVERFLOW_STREAM\r\n" );
                wsprintf( szString, TEXT("ERROR_OVERFLOW_STREAM") );
                break;                
            case CNS_SPI_CMD_ERROR_NOT_DETECT:      ///span1118, ///add in spi comm spec 1.25
                NKDbgPrintfW(  L"TVB::ParseErrorCode ERROR_NOT_DETECT\r\n" );
                wsprintf( szString, TEXT("ERROR_NOT_DETECT") );
                break;
            default:
                NKDbgPrintfW(  L"TVB::ParseErrorCode unknow error code 0x%08x!\r\n", dwErrorCode );
                wsprintf( szString, TEXT("unknow code 0x%08x"), dwErrorCode );
                break;
        }
        SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);    
    }
    
    ///NKDbgPrintfW(  L"TVB::ParseErrorCode %s\r\n", bRet?L"Ok":L"Fail" );
    return bRet;
}


BOOL ScanServiceISDB(DWORD EnsembleIndex)
{
	BOOL bRet;
	CHANNELSCAN ScanChannel;
	CHANNEL_INFO ChannelInfo;
	
	ScanChannel.dwChannelNum = (EnsembleIndex+13);
	ScanChannel.dwScanTimeout = 2999;
	
    bRet = DeviceIoControl(hDTV, IOCTL_DTV_CHANNEL_SCAN, &ScanChannel, sizeof(ScanChannel), &ChannelInfo, sizeof(ChannelInfo), &dwActSize, NULL);
    if( bRet ) {
    	DWORD dwCH, dwSerV;
        ///NKDbgPrintfW( L"TVB:: ScanService OK\r\n" );
        dwCH = EnsembleIndex;
        dwSerV = 0;
        if( ScanChannel.dwChannelNum != ChannelInfo.dwPhyChNum ) {
        	bRet = FALSE;
        	wsprintf( szString, TEXT("Ch%d no service"), (EnsembleIndex+13) );
        	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
        	ISDBallChannelstatus[dwCH][0].dwPhyChNum = 0;	///clear each service in this physical channel
        	ISDBallChannelstatus[dwCH][1].dwPhyChNum = 0;
        	ISDBallChannelstatus[dwCH][2].dwPhyChNum = 0;
        	ISDBallChannelstatus[dwCH][3].dwPhyChNum = 0;
        	ISDBallChannelstatus[dwCH][4].dwPhyChNum = 0;
        	ISDBallChannelstatus[dwCH][5].dwPhyChNum = 0;
        	ISDBallChannelstatus[dwCH][6].dwPhyChNum = 0;
        	ISDBallChannelstatus[dwCH][7].dwPhyChNum = 0;
        	
        	DtvStatus( DVT_STATE_PWRON );
        } else {
        	wsprintf( szString, TEXT("Ch%d sync!"), (EnsembleIndex+13) );
        	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
        	
        	ISDBallChannelstatus[dwCH][0].dwPhyChNum = dwCH+13;

    		DtvStatus( DVT_STATE_LOCKED );
    		
    		wsprintf( szString, TEXT("3digit=%d"), ChannelInfo.ChNum );
    		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    		wsprintf( szString, TEXT("Ch=%d"), ChannelInfo.dwPhyChNum );
    		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    		wsprintf( szString, TEXT("ServID=%d"), ChannelInfo.dwServiceId );
    		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    		wsprintf( szString, TEXT("Audmode=%d"), ChannelInfo.dwAudioMode );
    		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    		wsprintf( szString, TEXT("TSid=%d"), ChannelInfo.dwTSId );
    		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    		wsprintf( szString, TEXT("CCid=%d"), ChannelInfo.dwCapLanguageId);
    		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

        }
        ///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
        
        if( 0 != ChannelInfo.dwPhyChNum ) {
        	///ISDB_T_CH_DETAIL_INFO ChDetailInfo;
        	GetCurrentChDetailInfo( &ChDetailInfo );
        }	
   
	} else {
		NKDbgPrintfW( L"TVB:: IOCTL_DTV_CHANNEL_SCAN Fail\r\n" );
	}	


	return bRet;
}


BOOL ScanService(DWORD EnsembleIndex)
{
    BOOL bRet = FALSE;
    DWORD ServiceId[3], DmbDabCount, ServiceTTLCount, DmbCount, DabCount, DataCount;
    PBYTE pSvcList;
    PSvcInfo pSvcPtr;
    DWORD i;

    // scan services
    ServiceId[0] = SwapDword(EnsembleIndex);
    ServiceId[1] = 0;
    ServiceId[2] = 0;

    NKDbgPrintfW( L"TVB:: ScanService() = %08x\r\n", EnsembleIndex );
    if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_ACCESS_SVC, ServiceId, 12, pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        NKDbgPrintfW( L"TVB:: ScanService OK\r\n" );

        // save service list
        if ( dwActSize > 0 ) {
            if ( pEnsembleList[EnsembleIndex] != NULL )
                LocalFree(pEnsembleList[EnsembleIndex]);
            pSvcList = (PBYTE)LocalAlloc(LPTR, dwActSize);
        
	    	NKDbgPrintfW(L"SVC_INFO:: Ensemble = %d\r\n", EnsembleIndex);
            ServiceTTLCount = (dwActSize / sizeof(SvcInfo));
            DmbDabCount = 0;
            DmbCount = 0;
            DabCount = 0;
            DataCount = 0;
            for (i=0; i<ServiceTTLCount; i++) {
                pSvcPtr = &((PSvcInfo)pBufOut)[i];
                if (
                        ( (pSvcPtr->asc_dsc==0) && (pSvcPtr->tmid==0) )
                   )
                {
                    memcpy(&((PSvcInfo)pSvcList)[DmbDabCount], &((PSvcInfo)pBufOut)[i], sizeof(SvcInfo));
                    DmbDabCount++;
                    DabCount++;
                }
                else
                if (
                        ( (pSvcPtr->asc_dsc==0x1800) && (pSvcPtr->tmid==0x0100) )
                   )
                {
                    memcpy(&((PSvcInfo)pSvcList)[DmbDabCount], &((PSvcInfo)pBufOut)[i], sizeof(SvcInfo));
                    DmbDabCount++;
                    DmbCount++;
                }
                else
                if (
                        (pSvcPtr->tmid==0x0300)
                   )
                {
                    ///memcpy(&((PSvcInfo)pSvcList)[DmbDabCount], &((PSvcInfo)pBufOut)[i], sizeof(SvcInfo));
                    DataCount++;
                }

	    	    NKDbgPrintfW( L"\r\n" );
	    	    NKDbgPrintfW( L"SVC_INFO:: index = %d\r\n", i );
	    	    NKDbgPrintfW( L"  srv_id =        %08x\r\n", SwapDword(pSvcPtr->srv_id));
	    	    NKDbgPrintfW( L"  tmid =          %04x\r\n", SwapWord(pSvcPtr->tmid));
	    	    NKDbgPrintfW( L"  asc_dsc =       %04x\r\n", SwapWord(pSvcPtr->asc_dsc));
	    	    NKDbgPrintfW( L"  sub_ch_id =     %04x\r\n", SwapWord(pSvcPtr->sub_ch_id));
	    	    NKDbgPrintfW( L"  srv_comp =      %04x\r\n", SwapWord(pSvcPtr->srv_comp));
	    	    NKDbgPrintfW( L"  packet_addr =   %04x\r\n", SwapWord(pSvcPtr->packet_addr));
	    	    NKDbgPrintfW( L"  freq_index =    %02x\r\n", pSvcPtr->freq_index);
	    	    NKDbgPrintfW( L"  dg_flag =       %02x\r\n", pSvcPtr->dg_flag);
	    	    NKDbgPrintfW( L"  bitrate =       %08x\r\n", SwapDword(pSvcPtr->bitrate));
	    	    NKDbgPrintfW( L"  sub_ch_size =   %08x\r\n", SwapDword(pSvcPtr->sub_ch_size));
	    	    NKDbgPrintfW( L"  user_app_type = %08x\r\n", SwapDword(pSvcPtr->user_app_type));
            }
            pSvcList = (PBYTE)LocalReAlloc(pSvcList, DmbDabCount*sizeof(SvcInfo), LMEM_MOVEABLE);
            pEnsembleList[EnsembleIndex] = (PSvcInfo)pSvcList;

            dwSvcListCount[EnsembleIndex] = DmbDabCount;
            if ( DmbDabCount > 0 )
                bRet = TRUE;
        
	    	NKDbgPrintfW( L"\r\n" );
	    	NKDbgPrintfW(L"SVC_INFO:: service count: TTL = %d, DMB/DAB/Data = %d/%d/%d\r\n", ServiceTTLCount, DmbCount, DabCount, DataCount );
	    	///BWS:Broadcast WebSite Service
	    	NKDbgPrintfW( L"\r\n\r\n" );

		    wsprintf( szString, TEXT("TTL/DMB/DAB/Data = %d/%d/%d/%d"), ServiceTTLCount, DmbCount, DabCount, DataCount );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

        }
    } else {
        NKDbgPrintfW( L"TVB:: ScanService Failed\r\n" );
        ParseErrorCode( pBufOut );
    }
	
    wsprintf( szString, TEXT("ScanService %s"), bRet?L"OK":L"fail" );
	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	
    return bRet;
}

BOOL SetChannel(DWORD EnsembleIndex, DWORD SvcIndex, BOOL &bDMB)
{
    BOOL bRet = FALSE;
    DWORD ServiceId[3];//, SvcIndexTemp;
    PBYTE pSvcList;
    PSvcInfo pSvcPtr;
    
    pSvcList = (PBYTE)pEnsembleList[EnsembleIndex];
    if ( dwSvcListCount[EnsembleIndex] == 0 ) {
        NKDbgPrintfW( L"TVB:: can't set channel, no service in this ensemble\r\n" );
        return FALSE;
    }
    
    pSvcPtr = &((PSvcInfo)pSvcList)[SvcIndex];
    if ( (pSvcPtr->asc_dsc==0x1800) && (pSvcPtr->tmid==0x0100) )
        bDMB = TRUE;
    else
        bDMB = FALSE;

    ServiceId[0] = SwapDword(pSvcPtr->freq_index);
    ServiceId[1] = pSvcPtr->srv_id;
    ServiceId[2] = SwapDword(SwapWord(pSvcPtr->sub_ch_id));

    NKDbgPrintfW( L"TVB:: SetChannel() = %08x, %08x (%08x, %08x, %08x)\r\n", EnsembleIndex, SvcIndex, ServiceId[0], ServiceId[1], ServiceId[2] );
    if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_ACCESS_SVC, ServiceId, 12, pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        ///NKDbgPrintfW( L"TVB:: IOCTL OK\r\n" );

       	wsprintf( szString, TEXT("SetChannel OK") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	wsprintf( szString, TEXT("EnsembleIndex=%08x"), EnsembleIndex );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	wsprintf( szString, TEXT("SvcIndex=%08x"), SvcIndex );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	wsprintf( szString, TEXT("ServiceId0=%08x"), ServiceId[0] );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	wsprintf( szString, TEXT("ServiceId1=%08x"), ServiceId[1] );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	wsprintf( szString, TEXT("ServiceId2=%08x"), ServiceId[2] );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	

       	ISDB_T_SIGNALSTATUS SignalStatus;
		GetSignalStatus( &SignalStatus );

        bRet = TRUE;
    } else {
       	wsprintf( szString, TEXT("SetChannel fail") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	wsprintf( szString, TEXT("EnsembleIndex=%08x"), EnsembleIndex );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	wsprintf( szString, TEXT("SvcIndex=%08x"), SvcIndex );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	wsprintf( szString, TEXT("ServiceId0=%08x"), ServiceId[0] );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	wsprintf( szString, TEXT("ServiceId1=%08x"), ServiceId[1] );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	wsprintf( szString, TEXT("ServiceId2=%08x"), ServiceId[2] );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	

    	ParseErrorCode( pBufOut );
        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
    
    return bRet;
}


BOOL SetISDBTAudio( BOOL bAudioOn )
{
	BOOL bRet = FALSE;
	
	DTV_AUDIO_ROUTING_CTRL AuRoCtl;
	AuRoCtl.bTVaudioRouting = bAudioOn;

    bRet = DeviceIoControl(hDTV, IOCTL_DTV_AUDIO_ROUTING_CTRL, &AuRoCtl, sizeof(DTV_AUDIO_ROUTING_CTRL), NULL, 0, &dwActSize, NULL);

	return bRet;
}

BOOL GetCurrentChDetailInfo( PISDB_T_CH_DETAIL_INFO pChDetailInfo )
{
	BOOL bRet = FALSE;
	
	bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_CURRENT_CH_DETAIL_INFO, pChDetailInfo, sizeof(ISDB_T_CH_DETAIL_INFO), NULL, 0, &dwActSize, NULL);
	
	if( bRet ) {
    	wsprintf( szString, TEXT("CH detail ok") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);	

    	wsprintf( szString, TEXT("NIT %dbytes"), pChDetailInfo->NITcontent.dwNITSize );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
		NKDbgPrintfW( L"TVB:: GetCurrentChDetailInfo() NIT size= %dbyte\r\n", pChDetailInfo->NITcontent.dwNITSize);
    	wsprintf( szString, TEXT("SDT %dbytes"), pChDetailInfo->SDTcontent.dwSDTSize );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);		
		NKDbgPrintfW( L"TVB:: GetCurrentChDetailInfo() SDT size= %dbyte\r\n", pChDetailInfo->SDTcontent.dwSDTSize);
    	wsprintf( szString, TEXT("PMT %dbytes"), pChDetailInfo->PMTcontent.dwPMTSize );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
		NKDbgPrintfW( L"TVB:: GetCurrentChDetailInfo() PMT size= %dbyte\r\n", pChDetailInfo->PMTcontent.dwPMTSize);

		if( 1 ) {
			PBYTE pByte = NULL;
			DWORD dwDataLen = 0;
			TCHAR szTempString[256];
			SLogClass *pChLog = NULL;
			pChLog = new SLogClass();
			
			wsprintf( szTempString, _T("Tv channel detail info\r\n") );
			pChLog->SetTitle( szTempString );
	
			wsprintf( szTempString, _T("This info included NIT PMT SDT\r\n") );
			pChLog->SetInfo( szTempString );
	
			wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
			pChLog->SetSeparator( szTempString );
	
			///wsprintf( szTempString, _T("\\SDMMC Card\\AutoReboot.log\0") );
			///wsprintf( szTempString, _T("\\Storage Card\\TvPwrOnTest.log") );
			wsprintf( szTempString, _T("\\My Flash Disk\\TvInfo.log") );
			pChLog->SetLogName( szTempString );
			
			
			pChLog->EnableLog( TRUE );
			///pChLog->LogCurrentTime();
			
			pChLog->WriteLog( _T("NIT data %s\r\n"), L"start" );
			pByte = &(pChDetailInfo->NITcontent.NIT[0]);
			dwDataLen = pChDetailInfo->NITcontent.dwNITSize;
			if( dwDataLen > 255 )
				dwDataLen = 0;
			while( dwDataLen > 0 ) {
				pChLog->WriteLog( _T("%02x "), *pByte );
				pByte++;
				dwDataLen--;
			}
			pChLog->WriteLog( _T("\r\nNIT data %s\r\n"), L"end" );

			pChLog->WriteLog( _T("SDT data %s\r\n"), L"start" );
			pByte = &(pChDetailInfo->SDTcontent.SDT[0]);
			dwDataLen = pChDetailInfo->SDTcontent.dwSDTSize;
			if( dwDataLen > 255 )
				dwDataLen = 0; 
			while( dwDataLen > 0 ) {
				pChLog->WriteLog( _T("%02x "), *pByte );
				pByte++;
				dwDataLen--;
			}
			pChLog->WriteLog( _T("\r\nSDT data %s\r\n"), L"end" );

			pChLog->WriteLog( _T("PMT data %s\r\n"), L"start" );
			pByte = &(pChDetailInfo->PMTcontent.PMT[0]);
			dwDataLen = pChDetailInfo->PMTcontent.dwPMTSize;
			if( dwDataLen > 255 )
				dwDataLen = 0;
			while( dwDataLen > 0 ) {
				pChLog->WriteLog( _T("%02x "), *pByte );
				pByte++;
				dwDataLen--;
			}
			pChLog->WriteLog( _T("\r\nPMT data %s\r\n"), L"end" );

			if( pChDetailInfo->NITcontent.dwNITSize > 0 ) {	///NIT exist
				ISDB_NETWORK_NAME NetworkName;
				bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_CURRENT_NETWORK_NAME, &NetworkName, sizeof(NetworkName), NULL, 0, &dwActSize, NULL);
				if( !bRet ) {
					NKDbgPrintfW( L"TVB:: IOCTL_DTV_ISDB_CURRENT_NETWORK_NAME Failed\r\n" );
					NetworkName.dwNetworkNameLength = 0;
				}
				pChLog->WriteLog( _T("NetWork Name data %s\r\n"), L"start" );
				pByte = &(NetworkName.bNetworkName[0]);
				dwDataLen = NetworkName.dwNetworkNameLength;
				if( dwDataLen > 255 )
					dwDataLen = 0;
				while( dwDataLen > 0 ) {
					pChLog->WriteLog( _T("%02x "), *pByte );
					pByte++;
					dwDataLen--;
				}
				pChLog->WriteLog( _T("\r\nNetWork Name data %s\r\n"), L"end" );
			}

			if( pChDetailInfo->NITcontent.dwNITSize > 0 ) {	///NIT exist
				ISDB_TS_NAME TSName;
				bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_CURRENT_TS_NAME, &TSName, sizeof(TSName), NULL, 0, &dwActSize, NULL);
				if( !bRet ) {
					NKDbgPrintfW( L"TVB:: IOCTL_DTV_ISDB_CURRENT_TS_NAME Failed\r\n" );
					TSName.dwTSNameLength = 0;
				}
				pChLog->WriteLog( _T("TS Name data %s\r\n"), L"start" );
				pByte = &(TSName.bTSName[0]);
				dwDataLen = TSName.dwTSNameLength;
				if( dwDataLen > 255 )
					dwDataLen = 0;
				while( dwDataLen > 0 ) {
					pChLog->WriteLog( _T("%02x "), *pByte );
					pByte++;
					dwDataLen--;
				}
				pChLog->WriteLog( _T("\r\nTS Name data %s\r\n"), L"end" );
			}

			
			pChLog->SaveLog();
		}
    		
	} else {
    	wsprintf( szString, TEXT("CH detail  Fail") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);		
	}	

	return bRet;
}

BOOL GetCurrentChServiceInfo( PISDB_T_CH_SERVICE_INFO pChServInfo )
{
	BOOL bRet = FALSE;
	
	bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_CURRENT_SERVICE_INFO, pChServInfo, sizeof(ISDB_T_CH_SERVICE_INFO), NULL, 0, &dwActSize, NULL);
	
	if( bRet ) {
    	wsprintf( szString, TEXT("dwPhyChNum=%d"), pChServInfo->dwPhyChNum );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	wsprintf( szString, TEXT("dwServiceId=%d"), pChServInfo->dwServiceId );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

    	wsprintf( szString, TEXT("3digit=%d"), pChServInfo->ChNum.dwChannelNum );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	wsprintf( szString, TEXT("dwBranchNum=%d"), pChServInfo->ChNum.dwBranchNum );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

		if( ISDB_ES_AUDIO_MODE_DUAL_MONO == pChServInfo->AudioModes.dwCurrMode )
    		wsprintf( szString, TEXT("Dual Mono") );
    	else
		if( ISDB_ES_AUDIO_MODE_MONO == pChServInfo->AudioModes.dwCurrMode )
    		wsprintf( szString, TEXT("Mono") );
    	else
		if( ISDB_ES_AUDIO_MODE_STEREO == pChServInfo->AudioModes.dwCurrMode )
    		wsprintf( szString, TEXT("Stereo") );
    	else		
			wsprintf( szString, TEXT("Unknow Audio mode") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

    	wsprintf( szString, TEXT("AudioMode=0x%x"), pChServInfo->AudioModes.dwCurrMode );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	wsprintf( szString, TEXT("AudioModes=%d"), pChServInfo->AudioModes.dwAllModes );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

    	wsprintf( szString, TEXT("AudioESnum=%d"), pChServInfo->AudioESnums.dwCurrNum );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	wsprintf( szString, TEXT("AudioESnums=%d"), pChServInfo->AudioESnums.dwTotalNum );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

		if( ISDB_VIDEO_MODE_4_3 == pChServInfo->VideoMode.dwCurrNum )
    		wsprintf( szString, TEXT("4:3") );
    	else
		if( ISDB_VIDEO_MODE_16_9 == pChServInfo->VideoMode.dwCurrNum )
    		wsprintf( szString, TEXT("16:9") );
    	else		
			wsprintf( szString, TEXT("Unknow Video mode") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

    	wsprintf( szString, TEXT("VideoMode=0x%x"), pChServInfo->VideoMode.dwCurrNum );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	////wsprintf( szString, TEXT("VideoMode=%d"), pChServInfo->AudioModes.dwAllModes );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

    	wsprintf( szString, TEXT("VideoESnum=%d"), pChServInfo->VideoESnums.dwCurrNum );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	wsprintf( szString, TEXT("VideoESnums=%d"), pChServInfo->VideoESnums.dwTotalNum );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

    	wsprintf( szString, TEXT("LanguageNum=%d"), pChServInfo->LanguageNums.dwCurrLanguageNum );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	wsprintf( szString, TEXT("LanguageNums=%d"), pChServInfo->LanguageNums.dwTotalLanguageNum );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

#if 0	///12-seg
		if( 
			( 
				(pChServInfo->CopyProtects.dwCopyCtlType == 0x01) &&
				(pChServInfo->CopyProtects.dwRecCtlData == 0x00)
			)
			||
			( 
				(pChServInfo->CopyProtects.dwCopyCtlType == 0x01) &&
				(pChServInfo->CopyProtects.dwRecCtlData == 0x02)
			)
			||
			( 
				(pChServInfo->CopyProtects.dwCopyCtlType == 0x03) &&
				(pChServInfo->CopyProtects.dwRecCtlData == 0x00)
			)
		)
		{
   			wsprintf( szString, TEXT("Copy free") );
		} else {
			wsprintf( szString, TEXT("Copy protect") );
		}
#else	///1-seg
		if( pChServInfo->CopyProtects.dwCopyCtlType == 0x02) {
			if(pChServInfo->CopyProtects.dwRecCtlData == 0x02)
				wsprintf( szString, TEXT("Copy One Generation") );
			else
			if(pChServInfo->CopyProtects.dwRecCtlData == 0x00) {
				if(pChServInfo->CopyProtects.dwEncryptMode == 0x00)
					wsprintf( szString, TEXT("Copy EPN") );
				else
					wsprintf( szString, TEXT("Copy freely") );
			} else
				wsprintf( szString, TEXT("Copy Never") );
		} else {
			wsprintf( szString, TEXT("Copy Never") );
		}	
#endif
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	wsprintf( szString, TEXT("CopyCtlType=0x%x"), pChServInfo->CopyProtects.dwCopyCtlType );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	wsprintf( szString, TEXT("RecCtldata=0x%x"), pChServInfo->CopyProtects.dwRecCtlData );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	wsprintf( szString, TEXT("EncryptMode=0x%x"), pChServInfo->CopyProtects.dwEncryptMode );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

	} else {
    	wsprintf( szString, TEXT("GetCurrentChServiceInfo Fail") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);		
	}	
	
	
	return bRet;
}


BOOL GetCurrentEPG( PISDB_EPG_INFO pEPG )
{
	BOOL bRet = FALSE;
	TCHAR szTempString[256];
	SLogClass *pChLog = NULL;
	DWORD dwLoop = 0;
	
	pChLog = new SLogClass();
	
	wsprintf( szTempString, _T("Tv EPG info\r\n") );
	pChLog->SetTitle( szTempString );

	wsprintf( szTempString, _T("This info included EPG\r\n") );
	pChLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pChLog->SetSeparator( szTempString );

	///wsprintf( szTempString, _T("\\SDMMC Card\\AutoReboot.log\0") );
	///wsprintf( szTempString, _T("\\Storage Card\\TvPwrOnTest.log") );
	wsprintf( szTempString, _T("\\My Flash Disk\\TvEPG.log") );
	pChLog->SetLogName( szTempString );
	
	pChLog->EnableLog( TRUE );
	pChLog->WriteLog( _T("EPG data %s\r\n"), L"start" );
	
	pEPG->bHasNextProgram = FALSE;
	do {
    	if ( DeviceIoControl(hDTV, IOCTL_DTV_ISDB_CURRENT_EPG, pEPG, sizeof(ISDB_EPG_INFO), NULL, 0, &dwActSize, NULL) ) {
    		NKDbgPrintfW( L"TVB:: IOCTL_DTV_ISDB_CURRENT_EPG\r\n" );
    		DWORD dwCount = 0;
			pChLog->WriteLog( _T("\r\nEPG program %d\r\n"), dwLoop );
			pChLog->WriteLog( _T("Duration=%d\r\n"), pEPG->ThisProgram.ProgTime.dwProgramDuration );
			pChLog->WriteLog( _T("Date=") );
			for( dwCount=0; dwCount<5; dwCount++ ) {
				pChLog->WriteLog( _T("%02x "), pEPG->ThisProgram.ProgTime.bProgramDate[dwCount] );
			}
			pChLog->WriteLog( _T("NameLength=%d"), pEPG->ThisProgram.ProgName.dwProgramNameLength );
			pChLog->WriteLog( _T("Name=") );
			for( dwCount=0; dwCount<pEPG->ThisProgram.ProgName.dwProgramNameLength; dwCount++ ) {
				pChLog->WriteLog( _T("%02x "), pEPG->ThisProgram.ProgName.bProgramName[dwCount] );
			}
			pChLog->WriteLog( _T("InfoLength=%d"), pEPG->ThisProgram.ProgInfo.dwProgramInfoLength );
			pChLog->WriteLog( _T("Info=") );
			for( dwCount=0; dwCount<pEPG->ThisProgram.ProgInfo.dwProgramInfoLength; dwCount++ ) {
				pChLog->WriteLog( _T("%02x "), pEPG->ThisProgram.ProgInfo.bProgramInfo[dwCount] );
			}
						
			dwLoop++;
		} else {
			NKDbgPrintfW( L"TVB:: Failed! IOCTL_DTV_ISDB_CURRENT_EPG\r\n" );
			wsprintf( szString, TEXT("EPG fail") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
		}
	}while( pEPG->bHasNextProgram );
	
	pChLog->WriteLog( _T("EPG data %s\r\n"), L"end" );
	
	pChLog->SaveLog();

	wsprintf( szString, TEXT("EPG end") );
	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

	
	return bRet;
}


BOOL SetChannelISDBT( DWORD dwChannel )
{
    BOOL bRet = FALSE;
    DWORD dwCh = dwChannel;
    
    if ( dwChannel < 13 || dwChannel > 62 ) {
        NKDbgPrintfW( L"TVB:: SetChannelISDBT invalid parameter %d\r\n",dwChannel );
        return FALSE;
    }
    
    NKDbgPrintfW( L"TVB:: SetChannelISDBT() = %d \r\n", dwCh );
    if ( DeviceIoControl(hDTV, IOCTL_DTV_CHANNEL_SET, &dwCh, sizeof(DWORD), pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_DTV_CHANNEL_SET OK\r\n" );

       	wsprintf( szString, TEXT("SetChannelISDBT OK") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	///wsprintf( szString, TEXT("EnsembleIndex=%08x"), EnsembleIndex );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	///wsprintf( szString, TEXT("SvcIndex=%08x"), SvcIndex );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	///wsprintf( szString, TEXT("ServiceId0=%08x"), ServiceId[0] );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	///wsprintf( szString, TEXT("ServiceId1=%08x"), ServiceId[1] );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	///wsprintf( szString, TEXT("ServiceId2=%08x"), ServiceId[2] );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	

        ///GetSignalStatus();
        
        ISDBcurrChannelstatus.dwPhyChNum = dwChannel;
        ISDBcurrChannelstatus.dwServiceId = 0;
        bRet = TRUE;
    } else {
       	wsprintf( szString, TEXT("SetChannelISDBT fail") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	///wsprintf( szString, TEXT("EnsembleIndex=%08x"), EnsembleIndex );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	///wsprintf( szString, TEXT("SvcIndex=%08x"), SvcIndex );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	///wsprintf( szString, TEXT("ServiceId0=%08x"), ServiceId[0] );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	///wsprintf( szString, TEXT("ServiceId1=%08x"), ServiceId[1] );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
       	///wsprintf( szString, TEXT("ServiceId2=%08x"), ServiceId[2] );
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
        ///
    	///ParseErrorCode( pBufOut );
        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
    
    return bRet;
}

BOOL SetChannelISDBT2( DWORD dwChannel, DWORD dwService )
{
    BOOL bRet = FALSE;
    ///static ISDB_T_SETCHANNEL ChServSave;
    ISDB_T_SETCHANNEL ChServ;

	if( !bCNSPwrOn ) {
       	wsprintf( szString, TEXT("Not powered!!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		return bRet;
	}

    if ( dwChannel < 13 || dwChannel > 62 ) {
        NKDbgPrintfW( L"TVB:: SetChannelISDBT2 invalid parameter %d\r\n",dwChannel );
        return FALSE;
    }
    
    if( dwChannel == ChServSave.dwPhyChNum && dwService == ChServSave.dwServiceID ) {
    	NKDbgPrintfW( L"TVB:: SetChannelISDBT2 already set %d service %d \r\n",dwChannel, dwService );
       	wsprintf( szString, TEXT("SetChannelISDBT2 already set %d service %d"),dwChannel, dwService );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
    	return TRUE;
	} else {
    	ChServSave.dwPhyChNum = dwChannel;
    	ChServSave.dwServiceID = dwService;
	} 
    
    ChServ.dwPhyChNum = dwChannel;
    ChServ.dwServiceID = dwService;
    
    NKDbgPrintfW( L"TVB:: SetChannelISDBT2() = %d %d\r\n", dwChannel, dwService );
    if ( DeviceIoControl(hDTV, IOCTL_DTV_CHANNEL_SET, &ChServ, sizeof(ChServ), pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_DTV_CHANNEL_SET OK\r\n" );
		
       	wsprintf( szString, TEXT("SetChannelISDBT2 OK") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
        ISDBcurrChannelstatus.dwPhyChNum = dwChannel;
        ISDBcurrChannelstatus.dwServiceId = dwService;
        
        DtvStatus( DVT_STATE_LOCKED );
        
        if( 0 ) {
        	///ISDB_T_CH_SERVICE_INFO ChServInfo;
        	GetCurrentChServiceInfo( &ChServInfo );
        }	
        
        if( 0 ) {
        	///ISDB_T_CH_DETAIL_INFO ChDetailInfo;
        	GetCurrentChDetailInfo( &ChDetailInfo );
        }
        bRet = TRUE;
    } else {
    	DtvStatus( DVT_STATE_PWRON );
       	wsprintf( szString, TEXT("SetChannelISDBT2 fail") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
        ISDBcurrChannelstatus.dwPhyChNum = 0;
        ISDBcurrChannelstatus.dwServiceId = 0;
    	ChServSave.dwPhyChNum = 0;
    	ChServSave.dwServiceID = 0;

        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
    
    return bRet;
}



typedef struct _APIMessageEx
{
    SYS_ERROR  nRet;
    APIMessage aMsg;
} APIMessageEx, *PAPIMessageEx;


void SetVolume(BYTE uVol)
{
    if ( uVol & 0xf0 ) uVol = 0;
    if ( (uVol & 0x0f) > 10 ) uVol = 10;

// =============================================================================================================
#if 0
    HINSTANCE ghLibInst;
    PFNDTV_Init pDTV_Init;
    PFNSysSendMessage pSysSendMessage;
    PFNSysGetMessage pSysGetMessage;
    APIMessage MsgOut;
    
    ghLibInst = LoadLibrary(L"dtv.dll");

    pSysSendMessage = (PFNSysSendMessage)GetProcAddress(ghLibInst, L"SysSendMessage");
    if ( pSysSendMessage == NULL )
        NKDbgPrintfW( L"TVB:: failed to get PTR of SysSendMessage\r\n" );

    pSysGetMessage = (PFNSysGetMessage)GetProcAddress(ghLibInst, L"SysGetMessage");
    if ( pSysGetMessage == NULL )
        NKDbgPrintfW( L"TVB:: failed to get PTR of SysGetMessage\r\n" );
        
    pDTV_Init = (PFNDTV_Init)GetProcAddress(ghLibInst, L"DTV_Init");
    if ( pDTV_Init == NULL )
        NKDbgPrintfW( L"TVB:: failed to get PTR of DTV_Init\r\n" );

    pDTV_Init(0);
    nRet = pSysGetMessage(&MsgOut);
    NKDbgPrintfW( L"TVB:: pSysGetMsg test, Ret=%d, Mmsg=%08x, Len=%d\r\n", nRet, MsgOut.msg, MsgOut.length );
#endif

// =============================================================================================================
#if 0
    APIMessageEx MsgEx;
    DWORD dwReadWrite;
    
    MsgEx.aMsg.msg = SYSTEM_INFO_SIGNAL_STATUS;
    MsgEx.aMsg.length = 0;
    MsgEx.aMsg.pdata = NULL;
    WriteFile(hDTV, &MsgEx, sizeof(APIMessageEx), &dwReadWrite, NULL);

    MsgEx.aMsg.msg = SYSTEM_INFO_SIGNAL_STATUS;
    MsgEx.aMsg.length = 0;
    MsgEx.aMsg.pdata = pBufOut;
    ReadFile(hDTV, &MsgEx, sizeof(APIMessageEx), &dwReadWrite, NULL);
    NKDbgPrintfW( L"TVB:: Signal: msg  =     %08x\r\n", MsgEx.aMsg.msg );
    NKDbgPrintfW( L"TVB:: Signal: size =     %d\r\n", MsgEx.aMsg.length );
    NKDbgPrintfW( L"TVB:: Signal: pdata =    %08x\r\n", MsgEx.aMsg.pdata );
    NKDbgPrintfW( L"TVB::         Strength = %d\r\n", ((PCarrierStatus)MsgEx.aMsg.pdata)->strength );
#endif

// =============================================================================================================
#if 1
    NKDbgPrintfW( L"TVB:: SetVolume() = %02x\r\n", uVol );
    
    if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
    	DWORD dwTempVolume = 0;
    	switch(uVol) {
    		case 0:
    			dwTempVolume = 0x00000000;
    			break;
    		case 1:
    			dwTempVolume = 0x33333333;
    			break;
    		case 2:
    			dwTempVolume = 0x66666666;
    			break;
    		case 3:
    			dwTempVolume = 0x99999999;
    			break;
    		case 4:
    			dwTempVolume = 0xCCCCCCCC;
    			break;
    		case 5:
    			dwTempVolume = 0xFFFFFFFF;
    			break;
    		default:
    			dwTempVolume = 0xFFFFFFFF;
    			break;
    	}///switch
    	
		waveOutSetVolume( 0 , dwTempVolume );
		uCurrVol = uVol;
    } else {    
    	if ( DeviceIoControl(hDTV, MSG_DMB_SET_AUDIO_VOLUME, &uVol, 1, pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
    	    ///NKDbgPrintfW( L"TVB:: IOCTL OK\r\n" );
    	    uCurrVol = uVol;
    	    if ( DeviceIoControl(hDTV, MSG_DMB_GET_AUDIO_VOLUME, NULL, 0, pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
    	    	if( dwActSize >= 8 )
    	    		NKDbgPrintfW( L"TVB:: get volume = %d\r\n", pBufOut[7] );
    		} else {
    	    	NKDbgPrintfW( L"TVB:: MSG_DMB_GET_AUDIO_VOLUME Failed\r\n" );
    	    }	
    	} else {
    	    NKDbgPrintfW( L"TVB:: MSG_DMB_SET_AUDIO_VOLUME Failed\r\n" );
    	}
    	
    }
    
#endif

// =============================================================================================================
#if 0
    DWORD i;
    
    NKDbgPrintfW( L"TVB:: ReadTunerRegister() +\r\n" );

    for (i=0; i<32; i++) {
        if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_READ_TUNER_REG, &i, 4, pBufOut, dwBufOutSize, &dwActSize, NULL) )
            NKDbgPrintfW( L"TVB:: RF Reg #%02d = %08x (Read)\r\n", i, *(DWORD *)pBufOut );
        else
            NKDbgPrintfW( L"TVB:: RF Reg #%02d = failed\r\n", i);
    }

    DWORD w[2];
    for (i=0; i<32; i++) {
        w[0] = i;
        w[1] = i+10;
        if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_WRITE_TUNER_REG, w, 8, pBufOut, dwBufOutSize, &dwActSize, NULL) )
            NKDbgPrintfW( L"TVB:: RF Reg #%02d = %08x (Write)\r\n", w[0], w[1] );
        else
            NKDbgPrintfW( L"TVB:: RF Reg #%02d = failed\r\n", i);
    }

    for (i=0; i<32; i++) {
        if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_READ_TUNER_REG, &i, 4, pBufOut, dwBufOutSize, &dwActSize, NULL) )
            NKDbgPrintfW( L"TVB:: RF Reg #%02d = %08x (Read)\r\n", i, *(DWORD *)pBufOut );
        else
            NKDbgPrintfW( L"TVB:: RF Reg #%02d = failed\r\n", i);
    }

	#if 0
    DWORD w[2];
    w[0] = 13;
    w[1] = 0xab;
    if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_WRITE_TUNER_REG, w, 8, pBufOut, dwBufOutSize, &dwActSize, NULL) )
        NKDbgPrintfW( L"TVB:: RF Reg #%02d write ok\r\n", w[0] );
    else
        NKDbgPrintfW( L"TVB:: RF Reg #%02d write failed\r\n", w[0]);

    if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_READ_TUNER_REG, w, 4, pBufOut, dwBufOutSize, &dwActSize, NULL) )
        NKDbgPrintfW( L"TVB:: RF Reg #%02d = %08x\r\n", w[0], *(DWORD *)pBufOut );
    else
        NKDbgPrintfW( L"TVB:: RF Reg #%02d = failed\r\n", w[0]);

    NKDbgPrintfW( L"TVB:: ReadTunerRegister() -\r\n" );
	#endif
#endif

}

void SetVideoPos(DWORD dwPosX, DWORD dwPosY)
{
    CifSizePos NewPos;
    
    NewPos.x = dwPosX;
    NewPos.y = dwPosY;
    
    NKDbgPrintfW( L"TVB:: SetVideoPos()\r\n" );
   	if ( DeviceIoControl(hDTV, IOCTL_DTV_SET_VIDEO_OUT_POSTION, &NewPos, sizeof(CifSizePos), pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_DTV_SET_VIDEO_OUT_POSTION OK\r\n" );
    } else 
    if ( DeviceIoControl(hDTV, IOCTL_VIDEO_OUT_POS, &NewPos, sizeof(CifSizePos), pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
   		NKDbgPrintfW( L"TVB:: IOCTL_VIDEO_OUT_POS OK\r\n" );
    } else {
        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
}

void SetVideoInSize(DWORD dwSizeX, DWORD dwSizeY)
{
    CifSizePos NewSize;
    
    NewSize.x = dwSizeX;
    NewSize.y = dwSizeY;
    
    NKDbgPrintfW( L"TVB:: SetVideoInSize()\r\n" );
    if ( DeviceIoControl(hDTV, IOCTL_DTV_SET_VIDEO_IN_SIZE, &NewSize, sizeof(CifSizePos), pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_DTV_SET_VIDEO_IN_SIZE OK\r\n" );
    } else 
    if ( DeviceIoControl(hDTV, IOCTL_VIDEO_IN_SIZE, &NewSize, sizeof(CifSizePos), pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_VIDEO_IN_SIZE OK\r\n" );
    } else {
        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
}

void SetVideoOutSize(DWORD dwSizeX, DWORD dwSizeY)
{
    CifSizePos NewSize;
    
    NewSize.x = dwSizeX;
    NewSize.y = dwSizeY;
    
    NKDbgPrintfW( L"TVB:: SetVideoOutSize()\r\n" );
    if ( DeviceIoControl(hDTV, IOCTL_DTV_SET_VIDEO_OUT_SIZE, &NewSize, sizeof(CifSizePos), pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_DTV_SET_VIDEO_OUT_SIZE OK\r\n" );
    } else
    if ( DeviceIoControl(hDTV, IOCTL_VIDEO_OUT_SIZE, &NewSize, sizeof(CifSizePos), pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_VIDEO_OUT_SIZE OK\r\n" );
    } else {
        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
}

void SetVideoFormat601()
{
    NKDbgPrintfW( L"TVB:: SetVideoFormat601()\r\n" );
    if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_SET_VIDEO_601, NULL, 0, pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        ///NKDbgPrintfW( L"TVB:: IOCTL OK\r\n" );
    } else {
        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
}

void SetVideoFormat656()
{
    NKDbgPrintfW( L"TVB:: SetVideoFormat656()\r\n" );
    if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_SET_VIDEO_656, NULL, 0, pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        ///NKDbgPrintfW( L"TVB:: IOCTL OK\r\n" );
    } else {
        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
}

BOOL GetSignalStatus( PISDB_T_SIGNALSTATUS pSignalStatus )
{
	BOOL bRet = FALSE;
    NKDbgPrintfW( L"TVB:: GetSignalStatus()\r\n" );
    if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
    	ISDB_T_SIGNALSTATUS SignalStatus;
    	SignalStatus.dwSize = sizeof(SignalStatus);
    #if 0	///query for BER
    	SignalStatus.dwBEN = 0;
    	SignalStatus.dwBEC = 999999;
    #else	///not query for BER
    	SignalStatus.dwBEN = 0;
    	SignalStatus.dwBEC = 0;    
    #endif	
	    if ( DeviceIoControl(hDTV, IOCTL_DTV_MODULE_SIGNAL_STATUS, &SignalStatus, sizeof(SignalStatus), pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
	        ///NKDbgPrintfW( L"TVB:: IOCTL OK\r\n" );
			wsprintf( szString, TEXT("OFDM sync=%d"), SignalStatus.bOFDMsync );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			wsprintf( szString, TEXT("TS sync=%d"), SignalStatus.bTSsync );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	        
			wsprintf( szString, TEXT("AGC=%d"), SignalStatus.dwAGC );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			wsprintf( szString, TEXT("CN=%lf"), SignalStatus.dbCN );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			wsprintf( szString, TEXT("BEN=%d"), SignalStatus.dwBEN );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			wsprintf( szString, TEXT("BEC=%d"), SignalStatus.dwBEC );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

			wsprintf( szString, TEXT("Signal Level=%d"), SignalStatus.dwSignalLevel );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

			swprintf( szString, L"Strength:%02x,AGC:%d,CN:%lf,BEC:", SignalStatus.dwSignalLevel, SignalStatus.dwAGC, SignalStatus.dbCN, SignalStatus.dwBEC );
			SendMessage(GetDlgItem(g_hDlg, IDC_STRENGTH), WM_SETTEXT, 0, LPARAM(szString));

			pSignalStatus->dwSize = SignalStatus.dwSize;
			pSignalStatus->bOFDMsync = SignalStatus.bOFDMsync;
			pSignalStatus->bTSsync = SignalStatus.bTSsync;
			pSignalStatus->dwAGC = SignalStatus.dwAGC;
			pSignalStatus->dbCN = SignalStatus.dbCN;
			pSignalStatus->dwBEN = SignalStatus.dwBEN;
			pSignalStatus->dwBEC = SignalStatus.dwBEC;
			pSignalStatus->dwSignalLevel = SignalStatus.dwSignalLevel;

			bRet = TRUE;
	    } else {
	        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
	        bRet = FALSE;
	    }    	
    	
    	return bRet;
	}
    
    if ( DeviceIoControl(hDTV, SYSTEM_INFO_SIGNAL_STATUS, NULL, 0, pBufOut, dwBufOutSize, &dwActSize, NULL) ) {
        ///NKDbgPrintfW( L"TVB:: IOCTL OK\r\n" );
        memcpy(&CS, pBufOut, sizeof(CarrierStatus));
        bRet = TRUE;
    } else {
        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
    
    return bRet;
}

BOOL TvScanService(DWORD EnsembleIndex)
{
    BOOL bRet = FALSE, bDMB;

	if( !bCNSPwrOn ) {
       	wsprintf( szString, TEXT("Not powered!!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		return FALSE;
	}

    if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
    	if ( ScanServiceISDB(EnsembleIndex) ) {
        	NKDbgPrintfW( L"TVB:: ScanServiceISDB, Ch%d\r\n", (EnsembleIndex+13) );
        	bRet = TRUE;
    	}
	} else {
    	if ( ScanService(EnsembleIndex) ) {
        	NKDbgPrintfW( L"TVB:: TvScanService, to set ch %d,0\r\n", EnsembleIndex );
        	bRet = SetChannel(EnsembleIndex, 0, bDMB);
    	}
    }
    NKDbgPrintfW( L"TVB:: TvScanService, Result = %d\r\n", bRet );
    
    if( bRet )
    	DtvStatus( DVT_STATE_LOCKED );
    else
    	DtvStatus( DVT_STATE_PWRON );
    
    return bRet;
}

BOOL TvChannelUp(BOOL &bDMB)
{
    BOOL bRet = FALSE;
    DWORD uLastEnId = CurrEnsembleId, uLastChId = CurrSubChId;


    if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
    	CHANNEL_INFO ChInfo;
    	if( ISDBallChannelOperation( Up_allChannel, &ChInfo ) ) {
    		SetChannelISDBT2( ChInfo.dwPhyChNum, ChInfo.dwServiceId );
    		bRet = TRUE;
    	} else {
    		bRet = FALSE;
    	}

    	return bRet;
	}

    CurrSubChId++;

    if ( CurrSubChId >= dwSvcListCount[CurrEnsembleId] ) {
        CurrSubChId = 0;
        do {
            CurrEnsembleId++;
            if ( CurrEnsembleId >= MAX_ENSEMBLE_NUM )
                CurrEnsembleId = 0;
            if ( uLastEnId == CurrEnsembleId )
                break;
        } while (dwSvcListCount[CurrEnsembleId] == 0);
    }

    if ( (uLastEnId != CurrEnsembleId) || (uLastChId != CurrSubChId) )
        bRet = SetChannel(CurrEnsembleId, CurrSubChId, bDMB);
        
    return bRet;
}

BOOL TvChannelDown(BOOL &bDMB)
{
    BOOL bRet = FALSE;
    DWORD uLastEnId = CurrEnsembleId, uLastChId = CurrSubChId;

    if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
    	CHANNEL_INFO ChInfo;
    	if( ISDBallChannelOperation( Down_allChannel, &ChInfo ) ) {
    		SetChannelISDBT2( ChInfo.dwPhyChNum, ChInfo.dwServiceId );
    		bRet = TRUE;
    	} else {
    		bRet = FALSE;
    	}

    	return bRet;
	}


    if ( CurrSubChId == 0 ) {
        do {
            if ( CurrEnsembleId == 0)
                CurrEnsembleId = MAX_ENSEMBLE_NUM;
            CurrEnsembleId--;
            CurrSubChId = dwSvcListCount[CurrEnsembleId];
            if ( uLastEnId == CurrEnsembleId )
                break;
        } while (dwSvcListCount[CurrEnsembleId] == 0);
    }

    if (CurrSubChId > 0)
        CurrSubChId--;

    if ( (uLastEnId != CurrEnsembleId) || (uLastChId != CurrSubChId) )
        bRet = SetChannel(CurrEnsembleId, CurrSubChId, bDMB);
        
    return bRet;
}

void TvVolumlUp()
{
    SetVolume(uCurrVol+1);
}

void TvVolumlDown()
{

    SetVolume(uCurrVol-1);

}


void TvChangeScreen( BOOL bPlayBack )
{
	static DWORD dwOption = 0;
	
	if( bPlayBack ) {
		HANDLE hTemp;
		hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_PLAYBACKPAUSE_EVENT );
		SetEvent( hTemp );
		Sleep(1000);
		CloseHandle( hTemp );
	}	
	
	TvVedioOff();

	if( 0 == dwOption ) {
		if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
			TvSetVideoInSize( 360, 480 );
			TvSetVideoOutSize( 240, 136 );
			TvSetVideoPos( 15, 10 );
		} else {
			TvSetVideoInSize( 640, 480 );
			TvSetVideoOutSize( 240, 136 );
			///TvSetVideoPos( 15, 105 );
			TvSetVideoPos( 15, 90 );
		}
	    wsprintf( szString, TEXT("Screen ZoomIn") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	} else if( 1 == dwOption ) {
		if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
			TvSetVideoInSize( 360, 480 );
			TvSetVideoOutSize( 320, 240 );
			TvSetVideoPos( 0, 0 );
		} else {
			TvSetVideoInSize( 640, 480 );
			TvSetVideoOutSize( 320, 240 );
			TvSetVideoPos( 0, 0 );
		}
	    wsprintf( szString, TEXT("Screen ZoomOut") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	} else if( 2 == dwOption ) {
		if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
			TvSetVideoInSize( 360, 480 );
			TvSetVideoOutSize( 480, 272 );
			TvSetVideoPos( 0, 0 );
		} else {
			TvSetVideoInSize( 640, 480 );
			TvSetVideoOutSize( 480, 272 );
			TvSetVideoPos( 0, 0 );
		}
	    wsprintf( szString, TEXT("Screen ZoomOut") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	}

	TvVedioOn();

	if( 1 ) {
		HANDLE hTemp;
		hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_CMD_REDRAW_EVENT );
		SetEvent( hTemp );
		Sleep(1000);
		CloseHandle( hTemp );
	}
	///SetEvent( CreateEvent(NULL, FALSE, FALSE, NAMED_CMD_REDRAW_EVENT ) );


	if( bPlayBack ) {
		HANDLE hTemp;
		hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_PLAYBACKPAUSE_EVENT );
		SetEvent( hTemp );
		Sleep(1000);
		CloseHandle( hTemp );
	}
	
	dwOption++;
	if( dwOption > 2 )
		dwOption = 0;

}

/*
void TvVedioOn()
{
	if( !bCNSPwrOn ) {
       	wsprintf( szString, TEXT("Not powered!!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		return;
	}	
		
    NKDbgPrintfW( L"TVB:: TvVedioOn()\r\n" );
    if ( DeviceIoControl(hDTV, IOCTL_DTV_MODULE_VIDEO_ON, NULL, 0, pBufOut, dwBufOutSize, NULL, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_DTV_MODULE_VIDEO_ON OK\r\n" );
        bVideoDisplay = TRUE;
        DtvStatus( DVT_STATE_DISPON );

       	wsprintf( szString, TEXT("IOCTL_DTV_MODULE_VIDEO_ON ok") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
    } else
    if ( DeviceIoControl(hDTV, IOCTL_VIDEO_ON, NULL, 0, pBufOut, dwBufOutSize, NULL, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_VIDEO_ON OK\r\n" );
        bVideoDisplay = TRUE;
        DtvStatus( DVT_STATE_DISPON );
       	wsprintf( szString, TEXT("IOCTL_VIDEO_ON ok") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	

    } else {
       	wsprintf( szString, TEXT("IOCTL_DTV_MODULE_VIDEO_ON fail") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	

        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
}

void TvVedioOff()
{
	if( !bCNSPwrOn ) {
       	wsprintf( szString, TEXT("Not powered!!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		return;
	}	

	
    NKDbgPrintfW( L"TVB:: TvVedioOff()!\r\n" );
    if ( DeviceIoControl(hDTV, IOCTL_DTV_MODULE_VIDEO_OFF, NULL, 0, pBufOut, dwBufOutSize, NULL, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_DTV_MODULE_VIDEO_OFF OK\r\n" );
        bVideoDisplay = FALSE;
        DtvStatus( DVT_STATE_DISPOFF );
    } else
    if ( DeviceIoControl(hDTV, IOCTL_VIDEO_OFF, NULL, 0, pBufOut, dwBufOutSize, NULL, NULL) ) {
        NKDbgPrintfW( L"TVB:: IOCTL_VIDEO_OFF OK\r\n" );
        bVideoDisplay = FALSE;
        DtvStatus( DVT_STATE_DISPOFF );
    } else {
        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
    }
}
*/
void TvVedioOn()
{
	if( !bCNSPwrOn ) {
       	wsprintf( szString, TEXT("Not powered!!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		return;
	}	

	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
///		DWORD dwCh = 0;
///		DWORD dwSerV = 0;
///		dwCh = SendDlgItemMessage( g_hDlg, IDC_CHLIST, CB_GETCURSEL, 0, 0);
///		dwSerV = SendDlgItemMessage( g_hDlg, IDC_SERVLIST, CB_GETCURSEL, 0, 0);
		
///		///SetChannelISDBT( (dwCh+13) );
///		if( !SetChannelISDBT2( (dwCh+13), dwSerV ) )
///			return;
		
		NKDbgPrintfW( L"TVB:: TvVedioOn()\r\n" );
	    if ( DeviceIoControl(hDTV, IOCTL_DTV_MODULE_VIDEO_ON, NULL, 0, pBufOut, dwBufOutSize, NULL, NULL) ) {
	        NKDbgPrintfW( L"TVB:: IOCTL_DTV_MODULE_VIDEO_ON OK\r\n" );
	        bVideoDisplay = TRUE;
	        DtvStatus( DVT_STATE_DISPON );
	
	       	wsprintf( szString, TEXT("IOCTL_DTV_MODULE_VIDEO_ON ok") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			
			///open Audio/change routing!!!
			SetISDBTAudio( TRUE );

	    } else {
	       	wsprintf( szString, TEXT("IOCTL_DTV_MODULE_VIDEO_ON fail") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
	
	        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
	    }	
	} else {
		
	    NKDbgPrintfW( L"TVB:: TvVedioOn()\r\n" );
	    if ( DeviceIoControl(hDTV, IOCTL_VIDEO_ON, NULL, 0, pBufOut, dwBufOutSize, NULL, NULL) ) {
	        NKDbgPrintfW( L"TVB:: IOCTL_VIDEO_ON OK\r\n" );
	        bVideoDisplay = TRUE;
	        DtvStatus( DVT_STATE_DISPON );
	       	wsprintf( szString, TEXT("IOCTL_VIDEO_ON ok") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		} else {
	       	wsprintf( szString, TEXT("IOCTL_VIDEO_ON fail") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
	
	        NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
	    }
	}
}

void TvVedioOff()
{
	if( !bCNSPwrOn ) {
       	wsprintf( szString, TEXT("Not powered!!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		return;
	}
	
    NKDbgPrintfW( L"TVB:: TvVedioOff()!\r\n" );

	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
	    if ( DeviceIoControl(hDTV, IOCTL_DTV_MODULE_VIDEO_OFF, NULL, 0, pBufOut, dwBufOutSize, NULL, NULL) ) {
    	    NKDbgPrintfW( L"TVB:: IOCTL_DTV_MODULE_VIDEO_OFF OK\r\n" );
        	bVideoDisplay = FALSE;
        	DtvStatus( DVT_STATE_DISPOFF );
	       	wsprintf( szString, TEXT("IOCTL_DTV_MODULE_VIDEO_OFF ok") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
    	} else {
        	NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
	       	wsprintf( szString, TEXT("IOCTL_DTV_MODULE_VIDEO_OFF fail") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
    	}
    	
	    SetISDBTAudio( FALSE );
   		ChServSave.dwPhyChNum = 0;
    	ChServSave.dwServiceID = 0;

    } else {
	    if ( DeviceIoControl(hDTV, IOCTL_VIDEO_OFF, NULL, 0, pBufOut, dwBufOutSize, NULL, NULL) ) {
    	    NKDbgPrintfW( L"TVB:: IOCTL_VIDEO_OFF OK\r\n" );
    	    bVideoDisplay = FALSE;
    	    DtvStatus( DVT_STATE_DISPOFF );
	       	wsprintf( szString, TEXT("IOCTL_VIDEO_OFF ok") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
    	} else {
        	NKDbgPrintfW( L"TVB:: IOCTL Failed\r\n" );
	       	wsprintf( szString, TEXT("IOCTL_VIDEO_OFF fail") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
    	}
    }
    
	if( 1 ) {
		HANDLE hTemp;
		hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_CMD_REDRAW_EVENT );
		SetEvent( hTemp );
		Sleep(1000);
		CloseHandle( hTemp );
	}

    
}




DWORD ByteToHex(BYTE data)
{
    DWORD dwRet = 0;
    BYTE hdata, BL, BR;
    
    hdata = data & 0x0f;
    BR = hdata > 9 ? hdata + 0x37 : hdata + 0x30;
    hdata = (data & 0xf0) >> 4;
    BL = hdata > 9 ? hdata + 0x37 : hdata + 0x30;
        
    dwRet = (BR << 16) | BL;
    
    return dwRet;
}        

void TvGetChLabel(PBYTE pBufIn)
{
    PBYTE pSvcList;
    PSvcInfo pSvcPtr;
    int i;
    
    pSvcList = (PBYTE)pEnsembleList[CurrEnsembleId];

    if ( pSvcList != NULL ) {
        pSvcPtr = &((PSvcInfo)pSvcList)[CurrSubChId];
        for (i=0; i<16; i++)
            ((PWORD)pBufIn)[i] = pSvcPtr->srv_lable[i];
    } else
        swprintf((PWORD)pBufIn, L"pSvcList is NULL?");
}

void TvGetSignal(DWORD &dwStrength, DWORD &dwSNR, DWORD &dwBER)
{
    dwStrength = CS.strength;
    dwSNR = CS.nSNR;
    dwBER = CS.nBER;
}

void TvGetVersionId(PBYTE pBufOut)
{
    BYTE testdata[128];
    WORD versionid[128];
    DWORD i;

    testdata[0] = 0x10;
    testdata[1] = 0x71;
    testdata[2] = 0x0;
    testdata[3] = 0x0;
    
    if ( DeviceIoControl(hDTV, IOCTL_DIRECT_ACCESS, testdata, 4, testdata, 128, &dwActSize, NULL) ) {
        ZeroMemory(versionid, 128*2);
        for (i=0; i<dwActSize-4; i++)
            versionid[i] = testdata[i+4];
        NKDbgPrintfW( L"TVB:: Neptune Firmware Version = %s\r\n", versionid );
        if ( pBufOut )
            swprintf((PWORD)pBufOut, L"%s", versionid);
    } else
        NKDbgPrintfW( L"TVB:: Failed to get firmware version\r\n" );
}

void TvGetChecksum(PBYTE pBufOut)
{
    BYTE testdata[128];

    if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_GET_CHECKSUM, NULL, 0, testdata, 128, &dwActSize, NULL) ) {
        NKDbgPrintfW( L"TVB:: Neptune Firmware Checksum = %08x\r\n", *(DWORD *)testdata );
        if ( pBufOut )
            swprintf((PWORD)pBufOut, L"0x%08x", *(DWORD *)testdata);
    } else
        NKDbgPrintfW( L"TVB:: Failed to get firmware checksum\r\n" );
}

DWORD wBuffer2File( HANDLE hFileWrite, SBuffer *pDataBuf )
{
	BOOL bRet = FALSE;
	DWORD dwWritten = 0;
	DWORD dwRet = 0;

	if( hFileWrite != INVALID_HANDLE_VALUE ) {
	  	///pDataBuf->DumpBuffer();
		///check if it's right data
  		bRet = WriteFile( hFileWrite, pDataBuf->GetBufferPointer(), pDataBuf->GetBufferSize(), &dwWritten, NULL);
  		if( !bRet )
  			NKDbgPrintfW( L"TVB:: wBuffer2File error! write %d=%d Err=0x%08x\r\n", pDataBuf->GetBufferSize(), dwWritten, GetLastError() );
	}
	else
		NKDbgPrintfW( L"TVB:: wBuffer2File file error! \r\n" );

	dwRet = dwWritten;
	return dwRet;
}

BOOL IsValidStreamData( SBuffer *pDataBuf )
{
	BOOL bRet = FALSE;
	PBYTE pTestBuf;

	pTestBuf = pDataBuf->GetBufferPointer();
	if( ((*(pTestBuf)&0xF0) == 0x50) && (*(pTestBuf+1) == 0x61) )
		bRet = TRUE;
	else {
		NKDbgPrintfW( L"TVB:: IsValidStreamData data not valid! \r\n" );
		///pDataBuf->DumpBuffer();
		bRet = FALSE;
	}	

	return bRet;
}


BOOL TvModulePowerOnOff( BOOL bOn, BOOL bLog )
{
    DWORD dwIoctlCode;
    DWORD dwWait = 3000;
    ///DWORD dwWait = 6000;
    BOOL bRet = FALSE;
    
    if( bCNSPwrOn == bOn ) {
    	bRet = bCNSPwrOn;
    	return bRet;
    }

	if( bOn )	
		DumpDriverReg();	///dump register before power on
	else	
		TvVedioOff();	///video off before power off
		
	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		dwWait = 0;
			
    	ChServSave.dwPhyChNum = 0;		///clear it!
    	ChServSave.dwServiceID = 0xFF;	///clear it!

	    if ( bOn )
	        dwIoctlCode = IOCTL_DTV_MODULE_POWER_ON;
	    else
	        dwIoctlCode = IOCTL_DTV_MODULE_POWER_OFF;

    } else {
    	dwWait = 3000;

	    if ( bOn )
	        dwIoctlCode = IOCTL_NEPTUNE_PWR_ON;
	    else
	        dwIoctlCode = IOCTL_NEPTUNE_PWR_OFF;
    	
    }

	if( (NULL != pLog) && bLog )
		pLog->WriteLog( _T("Power %s\r\n"), bOn?L"On":L"Off" );

	
    if( DeviceIoControl(hDTV, dwIoctlCode, NULL, 0, NULL, 0, NULL, NULL) ) {
        NKDbgPrintfW( L"TVB:: Power %s Module Ok\r\n", bOn?L"On":L"Off" );
		if( bOn ) {
			bCNSPwrOn = TRUE;
			DtvStatus( DVT_STATE_PWRON );
		} else {
			bCNSPwrOn = FALSE;
			DtvStatus( DVT_STATE_PWROFF );
		}
	       	
		wsprintf( szString, TEXT("Wait %d ms"), dwWait );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		Sleep(dwWait);
	
		wsprintf( szString, TEXT("Chip Pwr %s OK"), bOn?L"ON":L"OFF" );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	} else {
		NKDbgPrintfW( L"TVB:: Failed to power %s Module\r\n", bOn?L"On":L"Off" );
		wsprintf( szString, TEXT("Chip Pwr control fail") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	}
	

	if( bOn && 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		DWORD dwCh = 0;
		NKDbgPrintfW( L"TVB:: TvModulePowerOnOff() renew Channel list \r\n" );
		ClearChannelListBox( g_hDlg );
		InitChannelListBoxISDBT( g_hDlg );		
	} else {
		ISDBcurrChannelstatus.dwPhyChNum = 0;
		ISDBallChannelOperation( Clear_allChannel, NULL );
	}

	bRet = bCNSPwrOn;
	
	return bRet;
}


void TvNeptunePowerOnOff(BOOL bOn, BOOL bLog)
{
    DWORD dwIoctlCode;
    DWORD dwWait = 3000;
    ///DWORD dwWait = 6000;
    
    if( bCNSPwrOn == bOn )
    	return;

	if( bOn )
		DumpDriverReg();

	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		dwWait = 0;
		if( !bOn )
			TvVedioOff();
			
    	ChServSave.dwPhyChNum = 0;		///clear it!
    	ChServSave.dwServiceID = 0xFF;	///clear it!
    }

	if( (NULL != pLog) && bLog )
		pLog->WriteLog( _T("Power %s\r\n"), bOn?L"On":L"Off" );

	if( 0x67333130 != dwProjectCode ) {	///

	    if ( bOn )
	        dwIoctlCode = IOCTL_NEPTUNE_PWR_ON;
	    else
	        dwIoctlCode = IOCTL_NEPTUNE_PWR_OFF;
	
	    if ( DeviceIoControl(hDTV, dwIoctlCode, NULL, 0, NULL, 0, NULL, NULL) ) {
	        NKDbgPrintfW( L"TVB:: Power On/Off (%d) Neptune\r\n", bOn );
	        if( bOn ) {
	        	bCNSPwrOn = TRUE;
	        	///g_DtvStatus = PWRON;
	        	DtvStatus( DVT_STATE_PWRON );
	       	} else {
	       		bCNSPwrOn = FALSE;
	       		///g_DtvStatus = PWROFF;
		       	DtvStatus( DVT_STATE_PWROFF );
	       	}
	       	
	       	wsprintf( szString, TEXT("Wait %d ms"), dwWait );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
	       	Sleep(dwWait);
	
	       	
	       	wsprintf( szString, TEXT("Chip Pwr %s"), bOn?L"ON":L"OFF" );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	    } else {
	        NKDbgPrintfW( L"TVB:: Failed to power on/off Neptune\r\n" );
	       	wsprintf( szString, TEXT("Chip Pwr control fail") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	    }
	}
	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		if( bCNSPwrOn != bOn ) {
		    if ( bOn )
		        dwIoctlCode = IOCTL_DTV_MODULE_POWER_ON;
		    else
		        dwIoctlCode = IOCTL_DTV_MODULE_POWER_OFF;
		
		    if ( DeviceIoControl(hDTV, dwIoctlCode, NULL, 0, NULL, 0, NULL, NULL) ) {
		        NKDbgPrintfW( L"TVB:: Power On/Off (%d) module\r\n", bOn );
		        if( bOn ) {
		        	bCNSPwrOn = TRUE;
		        	///g_DtvStatus = PWRON;
		        	DtvStatus( DVT_STATE_PWRON );
		       	} else {
		       		bCNSPwrOn = FALSE;
		       		///g_DtvStatus = PWROFF;
			       	DtvStatus( DVT_STATE_PWROFF );
		       	}
		       	wsprintf( szString, TEXT("Wait %d ms"), dwWait );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		       	Sleep(dwWait);
		       	wsprintf( szString, TEXT("Chip1 Pwr %s"), bOn?L"ON":L"OFF" );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
		    } else {
		        NKDbgPrintfW( L"TVB:: Failed to power on/off module\r\n" );
		       	wsprintf( szString, TEXT("Chip Pwr control fail2") );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
		    }
	
		}
	}


	if( bOn && 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		DWORD dwCh = 0;
		NKDbgPrintfW( L"TVB:: TvNeptunePowerOnOff() renew Channel list \r\n" );
		ClearChannelListBox( g_hDlg );
		InitChannelListBoxISDBT( g_hDlg );		
	} else {
		ISDBcurrChannelstatus.dwPhyChNum = 0;
		ISDBallChannelOperation( Clear_allChannel, NULL );
	}

 
}


static DWORD QuerySytemMemoryStatus()
{
	DWORD dwRet = 0;
	static DWORD dwPreMemLoad = 0;
	INT32 iDiffMemLoad = 0;
	LPMEMORYSTATUS lpMemStatus;
	MEMORYSTATUS MemStatus;
	
	lpMemStatus = &MemStatus;
	
	lpMemStatus->dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus( lpMemStatus );
	
	iDiffMemLoad = lpMemStatus->dwAvailPhys-dwPreMemLoad;	
	NKDbgPrintfW( L"TVB:: MemLoad= %d offset= %d\r\n", lpMemStatus->dwAvailPhys, iDiffMemLoad );
	dwPreMemLoad = lpMemStatus->dwAvailPhys;
	
	return dwRet;
}


///#define		RECORD_INTERVAL				28	///20ms
///#define		RECORD_INTERVAL				31	///20m, if plakback 3760 in 54ms
#define		RECORD_INTERVAL				25	///20m, if plakback 3760 in 54ms, oscar320 FCS original setting

#define		STILLSHOT_INTERVAL				25	///20ms
///#define		PLAYBACK_INTERVAL			27	///20ms, PLAYBACK_BUFFER_SIZE		1880, oscar320 FCS original setting
#define		PLAYBACK_INTERVAL			20
///#define		PLAYBACK_INTERVAL			54	///20ms, PLAYBACK_BUFFER_SIZE		3760
///#define		PLAYBACK_INTERVAL			56	///20ms, PLAYBACK_BUFFER_SIZE		3760

#define		PLAYBACK_BUFFER_SIZE		1880	///
///#define		PLAYBACK_BUFFER_SIZE		3760	///

#define		ISDB_PLAYBACK_BUFFER_SIZE		(192*10*4)	///
#define 	ISDB_PLAYBACK_PRE_FILL_PACKET	18	///

#define		PLAYBACK_DAB_BUFFER_SIZE		600	///

///#define 	TV_RECOED_FILENAME		L"\\Storage Card\\TS001.ts"
///#define 	TV_RECOED_FILENAME		L"\\TS001.ts"
#define 	TV_RECOED_FILENAME		L"\\My Flash Disk\\TS001.ts"

#define 	TV_RECOED_DAB_FILENAME		L"\\Storage Card\\TS001.dab"	///move to DAB firmware

#define 	TV_SLS_FILENAME		L"\\Storage Card\\SLS.BMP"

#define 	TV_STILLSHOT_FILENAME		L"\\My Flash Disk\\TVshot.jpg"

///span1108, add for record
void TvRecordThread(void)
{
#define  REC_EVENT_NUM	3
	DWORD dwStatus = 0;
	///DWORD dwNumEvents = REC_EVENT_NUM;	
	DWORD dwTimeout = INFINITE;
	HANDLE hev[REC_EVENT_NUM];
	DWORD dwPara;
	DWORD dwParaSize = 4;
	DWORD dwRetSize = 0;
	BOOL bRet = FALSE;
	///DWORD dwDataID = 0;
    DWORD dwTickCount1 = 0;
    DWORD dwTickCount2 = 0;	
	
	hev[0] = CreateEvent(NULL, FALSE, FALSE, NAMED_RECORDSTART_EVENT );
	if( dwProjectCode == 0x67333130 ) {	///PT310
		hev[1] = CreateEvent(NULL, FALSE, FALSE, NAMED_RECORD_DTAD_AVAILABLE );
	} else {
		hev[1] = CreateEvent(NULL, FALSE, FALSE, NAMED_RECORDING_EVENT );
	}
	hev[2] = CreateEvent(NULL, FALSE, FALSE, NAMED_RECORDSTOP_EVENT );
	
	NKDbgPrintfW( L"TVB:: TvRecordThread start\r\n" );
	
	while(1)
	{
		dwStatus = WaitForMultipleObjects(REC_EVENT_NUM, hev, FALSE, dwTimeout);

		switch(dwStatus) {
        	case WAIT_TIMEOUT:
///        		if( dwProjectCode == 0x67333130 ) {	///PT310
///        			dwTimeout = INFINITE;
///        			break;
///        		}
	        case (WAIT_OBJECT_0 + 1):
	        	/// recording
	        	
	        	///check interval timing
	        	if( RECORD_INTERVAL < (GetTickCount() - dwTickCount1) )
	        		NKDbgPrintfW( L"TVB:: TvRecordThread Recording interval %d(%d) ms\r\n", GetTickCount() - dwTickCount1, dwTimeout  );
	        	
	        	dwTickCount2 = GetTickCount();	///check operation time	
    			
				///get data from driver to buffer1
    			dwPara = 3;
    			if( dwProjectCode == 0x67333130 ) {	///PT310
   					bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_RECORD_OPERATION, &dwPara, dwParaSize, pFBuffer1->GetBufferPointer(), pFBuffer1->GetMaxBufferSize(), &dwRetSize, NULL);
   				} else {
   					bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_RECORD, &dwPara, dwParaSize, pFBuffer1->GetBufferPointer(), pFBuffer1->GetMaxBufferSize(), &dwRetSize, NULL);
   				}
   				
   				if( bRet ) {
   					
   					///pFBuffer1->DumpBuffer();
#if 1	////span debug with no FIFO access
					///QuerySytemMemoryStatus();
   					///copy buffer1 data to FIFO
   					if( dwProjectCode == 0x67333130 ) {	///PT310
	   					if( 0 == dwRetSize ) {
   							///dwTimeout = INFINITE;
   							bRet = FALSE;
   						} else {
   							bRet = pFIFO->Copy2FIFO( pFBuffer1->GetBufferPointer(), dwRetSize );
   						}
   					} else {
   						bRet = pFIFO->Copy2FIFO( pFBuffer1->GetBufferPointer(), dwRetSize );
   					}
					///QuerySytemMemoryStatus();
#endif
	    			if( bRet ) {
	    				///tell file writer
						HANDLE hTemp;
						hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_GETBUFFER_DONE_EVENT );
						if( INVALID_HANDLE_VALUE != hTemp ) {
							SetEvent( hTemp );
							CloseHandle( hTemp );
						}
	    				///NKDbgPrintfW( L"TVB:: TvRecordThread Copy2FIFO %d bytes\r\n", dwRetSize );
						
						pFBuffer1->ClearBuffer();
					} else {
						if( dwProjectCode == 0x67333130 ) {
							if( 0 == dwRetSize ) {
								;
							} else {	
								NKDbgPrintfW( L"TVB:: TvRecordThread Copy2FIFO fail\r\n" );
							}
						} else {
	   	    				NKDbgPrintfW( L"TVB:: TvRecordThread Copy2FIFO fail\r\n" );
	   	    			}	
	   	    			///dwTimeout = 0;	///re-try 
	   	    		}

				} else {
					NKDbgPrintfW( L"TVB:: Recording DeviceIoControl fail %d\r\n", dwRetSize );
   	    			dwTimeout = 0;	/// got failure, re-try immediately !!!
   	    			if( dwRetSize >= 8 ) {
   	    				pFBuffer1->SetBufDataLen( 8 );
   	    				pFBuffer1->DumpBuffer();
   	    			}	
   	    		}	
					
				///all done, check next timing
				if( RECORD_INTERVAL < (GetTickCount() - dwTickCount2) )
					NKDbgPrintfW( L"TVB:: Recording buffer takes %d ms\r\n", GetTickCount() - dwTickCount2 );
#if 1
				if( bRet ) {
					DWORD dwOpTemp = (GetTickCount() - dwTickCount2);
					dwTimeout = (RECORD_INTERVAL > dwOpTemp)?(RECORD_INTERVAL - dwOpTemp):0;
				}	
#else				
				if( bRet ) {
					///check next time
					DWORD dwTemp = (GetTickCount() - dwTickCount2);	
					if( RECORD_INTERVAL > dwTemp )
						dwTimeout = RECORD_INTERVAL - dwTemp;
					else
						dwTimeout = 0;	/// operation time too long, do next immediately !!!
				} else {
					dwTimeout = 0;		/// got failure, re-try immediately !!!
				}
#endif

   				if( dwProjectCode == 0x67333130 ) {	///PT310
	   				if( 0 == dwRetSize ) {
   						dwTimeout = INFINITE;
   					} else {
   						dwTimeout = 50;	///1 ms next
   					}
   				}
				dwRetSize = 0; ///clear it
				dwTickCount1 = GetTickCount();
				break;
	        case (WAIT_OBJECT_0 + 0):
	        	/// start record
	        	NKDbgPrintfW( L"TVB:: TvRecordThread Record start\r\n" );
	        	dwTimeout = RECORD_INTERVAL;
				
				DtvStatus( DVT_STATE_RECOED );
				
    			dwPara = 0;
    			if( dwProjectCode == 0x67333130 ) {	///PT310
    				bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_RECORD_OPERATION, &dwPara, dwParaSize, NULL, 0, &dwRetSize, NULL);
    			} else {
    				bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_RECORD, &dwPara, dwParaSize, NULL, 0, &dwRetSize, NULL);
    			}
    			if( !bRet )
        			NKDbgPrintfW( L"TVB:: start Record DeviceIoControl fail\r\n" );
				
				Sleep(1000);
	            break;
	        case (WAIT_OBJECT_0 + 2):
	        	/// stop record
	        	NKDbgPrintfW( L"TVB:: TvRecordThread Record stop\r\n" );
	        	dwTimeout = INFINITE;	///wait forever

				DtvStatus( DVT_STATE_STOPFUNC );

    			dwPara = 2;
    			if( dwProjectCode == 0x67333130 ) {	///PT310
    				bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_RECORD_OPERATION, &dwPara, dwParaSize, NULL, 0, &dwRetSize, NULL);
    			} else {	
    				bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_RECORD, &dwPara, dwParaSize, NULL, 0, &dwRetSize, NULL);
    			}
    			
    			if( !bRet )
        			NKDbgPrintfW( L"TVB:: stop Record DeviceIoControl fail\r\n" );
	        	
	            break;
			default:
				NKDbgPrintfW( L"TVB:: Record error !!!\r\n" );
	        	dwTimeout = RECORD_INTERVAL;				
				break;
		}
		
	}///while(1)
	
	CloseHandle( hev[0] );
	CloseHandle( hev[1] );
	CloseHandle( hev[2] );
		
	NKDbgPrintfW( L"TVB:: TvRecordThread stop\r\n" );
}

void TvStillShotThread(void)
{
#define  STILLSHOT_EVENT_NUM	3
	DWORD dwStatus = 0;
	DWORD dwTimeout = INFINITE;
	HANDLE hev[STILLSHOT_EVENT_NUM];
	DWORD dwPara;
	DWORD dwParaSize = 4;
	DWORD dwRetSize = 0;
	BOOL bRet = FALSE;
///	DWORD dwDataID = 0;
	DWORD dwShotFrameCount = 0;
    DWORD dwTickCount1 = 0;
    DWORD dwTickCount2 = 0;	
	
	hev[0] = CreateEvent(NULL, FALSE, FALSE, NAMED_STILLSHOTSTART_EVENT );
	hev[1] = CreateEvent(NULL, FALSE, FALSE, NAMED_STILLSHOTING_EVENT );
	hev[2] = CreateEvent(NULL, FALSE, FALSE, NAMED_STILLSHOTSTOP_EVENT );
	
	NKDbgPrintfW( L"TVB:: TvStillShotThread start\r\n" );
	
	while(1)
	{
		dwStatus = WaitForMultipleObjects(STILLSHOT_EVENT_NUM, hev, FALSE, dwTimeout);

		switch(dwStatus) {
        	case WAIT_TIMEOUT:
	        case (WAIT_OBJECT_0 + 1):
	        	/// stillshoting
	        	
	        	///check interval timing
	        	if( STILLSHOT_INTERVAL < (GetTickCount() - dwTickCount1) )
	        		NKDbgPrintfW( L"TVB:: TvStillShotThread Stillshoting interval %d(%d) ms\r\n", GetTickCount() - dwTickCount1, dwTimeout  );
	        		
	        	dwTickCount2 = GetTickCount();	///check operation time	
				///dwTimeout = STILLSHOT_INTERVAL;
///				dwDataID++;

				///get data from driver to buffer4
    			dwPara = 3;
   				bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_RECORD, &dwPara, dwParaSize, pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   				
   				if( bRet ) {
   					///pFBuffer4->DumpBuffer();
   					///copy buffer4 data to FIFO
   					bRet = pFIFO->Copy2FIFO( pFBuffer4->GetBufferPointer(), dwRetSize );
   					
   					if( bRet ) {
   						///span1201 not support now, SetEvent( CreateEvent(NULL, FALSE, FALSE, NAMED_GETBUFFER_DONE_EVENT ) );
   						NKDbgPrintfW( L"TVB:: TvStillShotThread Copy2FIFO %d bytes\r\n", dwRetSize );
   						
   						pFBuffer4->ClearBuffer();
						dwShotFrameCount++;
   					} else
   						NKDbgPrintfW( L"TVB:: TvStillShotThread Copy2FIFO fail\r\n" );
   				}
				else
   	    			NKDbgPrintfW( L"TVB:: stillshoting DeviceIoControl fail\r\n" );


				///all done, check next timing
				if( STILLSHOT_INTERVAL < (GetTickCount() - dwTickCount2) )
					NKDbgPrintfW( L"TVB:: stillshoting buffer takes %d ms\r\n", GetTickCount() - dwTickCount2 );
				
				if( bRet ) {
					///check operation time
					if( STILLSHOT_INTERVAL > (GetTickCount() - dwTickCount2) )
						dwTimeout = STILLSHOT_INTERVAL - (GetTickCount() - dwTickCount2);
					else
						dwTimeout = 0;	/// operation time too long, do next immediately !!!
				} else {
					dwTimeout = 0;		/// got failure, re-try immediately !!!
				}		
				
				///check if we get enough data
				if( dwShotFrameCount >= 100 ) {
		        	/// stop record
		        	NKDbgPrintfW( L"TVB:: TvStillShotThread stillshot id=%d\r\n", dwShotFrameCount);
		        	NKDbgPrintfW( L"TVB:: TvStillShotThread stillshot stop\r\n" );
	    	    	dwTimeout = INFINITE;	///wait forever

///span we do not need to stop it,	
///	    			dwPara = 2;
///					bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_RECORD, &dwPara, dwParaSize, NULL, 0, &dwRetSize, NULL);
    			
///    				if( !bRet ) {
///        				NKDbgPrintfW( L"TVB:: stop stillshot DeviceIoControl fail\r\n" );
///        			} else {
        				dwShotFrameCount = 0; ///reset counter
///        			}
        			
        			DtvStatus( DVT_STATE_STOPFUNC );
        			
        			pFIFO->ClearFIFO();	///span1122, clear FIFO for now
        			
				} else 
					NKDbgPrintfW( L"TVB:: TvStillShotThread stillshot id=%d\r\n", dwShotFrameCount);

				dwTickCount1 = GetTickCount();
				break;
	        case (WAIT_OBJECT_0 + 0):
	        	/// start StillShot
	        	NKDbgPrintfW( L"TVB:: TvStillShotThread StillShot start\r\n" );
	        	dwTimeout = STILLSHOT_INTERVAL;
///	        	dwDataID = 0;
				DtvStatus( DVT_STATE_STILLSHOT );
///    			dwPara = 0;
				///send stillshot command
				bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_STILLSHOT, NULL, 0, NULL, 0, &dwRetSize, NULL);

    			if( !bRet )
        			NKDbgPrintfW( L"TVB:: start stillshot DeviceIoControl fail\r\n" );
				
				Sleep(1000);
	            break;
	        case (WAIT_OBJECT_0 + 2):
	        	/// stop record
	        	NKDbgPrintfW( L"TVB:: TvStillShotThread stillshot stop\r\n" );
	        	dwTimeout = INFINITE;	///wait forever
///	        	dwDataID = 0;

///    			dwPara = 2;
///    			bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_RECORD, &dwPara, dwParaSize, NULL, 0, &dwRetSize, NULL);
    			
    			if( !bRet )
        			NKDbgPrintfW( L"TVB:: stop stillshot DeviceIoControl fail\r\n" );
	        	
	        	pFIFO->ClearFIFO();	///span1122, clear FIFO for now
	        	
	            break;
			default:
				NKDbgPrintfW( L"TVB:: stillshot error\r\n" );
	        	dwTimeout = RECORD_INTERVAL;				
				break;
		}
		
	}///while(1)
	
	CloseHandle( hev[0] );
	CloseHandle( hev[1] );
	CloseHandle( hev[2] );	

	NKDbgPrintfW( L"TVB:: TvRecordThread stop\r\n" );
}


#define FILE_BULK_WRITE

void FileRecordThread(void)
{
#define  FILE_EVENT_NUM	3
	DWORD dwStatus = 0;
	BOOL bRet = FALSE;
	///DWORD dwNumEvents = FILE_EVENT_NUM;	
	DWORD dwTimeout = INFINITE;
	HANDLE hev[FILE_EVENT_NUM];

    HANDLE hFile = INVALID_HANDLE_VALUE;
    DWORD dwWritten = 0;
    DWORD dwTickCount1 = 0;
    DWORD dwTickCount2 = 0;
    DWORD dwBulkNum = 16;
    BOOL bStopRecording = FALSE;
    DWORD dwRecduration = 0;
    DWORD dwDurationCount = 0;


	if( dwProjectCode == 0x67333130 ) {	///PT310
		dwBulkNum = 4;
	} else {
		dwBulkNum = 16;
	}
	
	hev[0] = CreateEvent(NULL, FALSE, FALSE, NAMED_FILESTART_EVENT );
	hev[1] = CreateEvent(NULL, FALSE, FALSE, NAMED_GETBUFFER_DONE_EVENT );
	hev[2] = CreateEvent(NULL, FALSE, FALSE, NAMED_FILESTOP_EVENT );
	
	NKDbgPrintfW( L"TVB:: FileRecordThread start\r\n" );
	
	while(1)
	{
		dwStatus = WaitForMultipleObjects(FILE_EVENT_NUM, hev, FALSE, dwTimeout);

		switch(dwStatus) {
        	case WAIT_TIMEOUT:
        		///break;
	        case (WAIT_OBJECT_0 + 1):
	        	/// recording
	        	///NKDbgPrintfW( L"TVB:: FileRecordThread to file\r\n" );
				///break;	///span debug
#ifdef FILE_BULK_WRITE
        		dwTickCount1 = GetTickCount();
        		dwRecduration += (dwTickCount1 - dwDurationCount);
        		dwDurationCount = dwTickCount1;
				
				DWORD dwCurFIFOsize;
				///check how many data in FIFO
				dwCurFIFOsize = pFIFO->GetCurFIFODepth();	///get currently FIFO size
				//check if we are going write file
				if( 
					0 == dwCurFIFOsize ||
					(dwBulkNum > dwCurFIFOsize && !bStopRecording )	///over 20 data in FIFO
				)
				{
					///need no write to file
					///NKDbgPrintfW( L"TVB:: FileRecordThread no need to write file, %d in FIFO\r\n", dwCurFIFOsize );
					dwTimeout = INFINITE;
				} else {
					///need to write file
					DWORD dwLoop;
					SBuffer *pWriteBuffer;
					DWORD dwReturn = 0;
					pWriteBuffer = new SBuffer( (32*1024) );	///create 32K buffer for write
					
					///NKDbgPrintfW( L"TVB:: FileRecordThread write %d buffer to file \r\n", dwCurFIFOsize );
					dwLoop = (dwBulkNum<dwCurFIFOsize)?dwBulkNum:dwCurFIFOsize;	///no big then 16 buffer
					
					///prepare data for writting
					while( dwLoop > 0 ) {

	    				pFBuffer2->ClearBuffer();
    					///get data from FIFO to buffer2
    					///QuerySytemMemoryStatus();
    					dwReturn = pFIFO->FIFOCopy2( pFBuffer2->GetBufferPointer(), pFBuffer2->GetMaxBufferSize() );
						///QuerySytemMemoryStatus();
						if( dwReturn > 0 ) { 
							pFBuffer2->SetBufDataLen( dwReturn );
							
							if( dwProjectCode == 0x67333130 ) {	///PT310
								if( pWriteBuffer->Add2Buffer( (pFBuffer2->GetBufferPointer()), (pFBuffer2->GetBufferSize()) ) ) {
									dwLoop--;/// good , countdown~
								} else {
									NKDbgPrintfW( L"TVB:: FileRecordThread Add2Buffer %d fail \r\n", dwLoop );
								}
							} else
							///check if it's valid stream data
							if( IsValidStreamData( pFBuffer2 ) ) {
								///prepare data
								if( pWriteBuffer->Add2Buffer( (pFBuffer2->GetBufferPointer()+4), (pFBuffer2->GetBufferSize()-4) ) ) {
									dwLoop--;/// good , countdown~
								} else {
									NKDbgPrintfW( L"TVB:: FileRecordThread Add2Buffer %d fail \r\n", dwLoop );
								}
							} else {
								NKDbgPrintfW( L"TVB:: FileRecordThread IsValidStreamData %d fail \r\n", dwLoop );///
							}
						}
						else {
							NKDbgPrintfW( L"TVB:: FileRecordThread FIFOCopy2 %d error! \r\n", dwLoop );
							dwLoop--;		///loop out if fail over 
						}	

						///dwLoop--;
					}

#if 1		
					dwReturn = wBuffer2File( hFile, pWriteBuffer );
#else				///span debug with no write file~	
					dwReturn = pWriteBuffer->GetBufferSize();	///span debug with no write file~
#endif					
					if( dwReturn == pWriteBuffer->GetBufferSize() )
						;///NKDbgPrintfW( L"TVB:: FileRecordThread wBuffer2File %d bytes Ok \r\n", pWriteBuffer->GetBufferSize() );
					else
						NKDbgPrintfW( L"TVB:: FileRecordThread wBuffer2File %d bytes fail %d \r\n", pWriteBuffer->GetBufferSize(), dwReturn );

					delete pWriteBuffer;
					
					dwTimeout = INFINITE;
				}
					
				
				///check operation timing    					
   				///if( RECORD_INTERVAL < (GetTickCount() - dwTickCount1) )
   				if( 160 < (GetTickCount() - dwTickCount1) )
   					NKDbgPrintfW( L"TVB:: FileRecordThread write file %d buffers takes %d ms\r\n", dwCurFIFOsize, GetTickCount() - dwTickCount1 );


				///check if we are going to stop
				if( bStopRecording ) {

	   				if( 0 < pFIFO->GetCurFIFODepth() ) {
	   					NKDbgPrintfW( L"TVB:: FileRecordThread still %d data in FIFO\r\n", pFIFO->GetCurFIFODepth() );
	   					dwTimeout = 0;
	   				} else {	
	   					///NKDbgPrintfW( L"TVB:: FileRecordThread buffer all empty\r\n" );
	   					dwTimeout = INFINITE;
	   					
	   					///check if we are going to stop
	   					if( bStopRecording ) {
	        				dwRecduration += (GetTickCount() - dwDurationCount);
   						   						
   							ISDBTSFileHeader.dwDuration = dwRecduration;
   							GetLocalTime( &ISDBTSFileHeader.systemTime );
	   						
							if( (dwProjectCode == 0x67333130) && ( hFile != INVALID_HANDLE_VALUE )) {	///PT310
								BOOL bRet;
								DWORD dwWritten;
								DWORD dwRetry = 10;
								
								bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_GET_TS_STREAM_RATE, &dwWritten, sizeof(dwWritten), NULL, 0, &dwActSize, NULL);
								
								while( !bRet && dwRetry>0 ) {
									Sleep(100);
									bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_GET_TS_STREAM_RATE, &dwWritten, sizeof(dwWritten), NULL, 0, &dwActSize, NULL);
								}
								
								if( bRet )
									ISDBTSFileHeader.dwTSrate = dwWritten;
								else
									NKDbgPrintfW( L"TVB:: IOCTL_DTV_ISDB_GET_TS_STREAM_RATE fail\r\n" );
									
								SetFilePointer( hFile, 0, 0, FILE_BEGIN );
		  						bRet = WriteFile( hFile, &ISDBTSFileHeader, sizeof(ISDBTSFileHeader), &dwWritten, NULL);
  								if( !bRet )
  									NKDbgPrintfW( L"TVB:: write TS file header2 %d 0x%08x\r\n", dwWritten, GetLastError() );
							}
	   						
				        	///close file
				        	bRet = CloseHandle(hFile);
				        	
				        	///insert file size ti header
				        	if(dwProjectCode == 0x67333130) {
				        		DWORD dwFileSizeTemp;
				        		hFile = CreateFile( TV_RECOED_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

				        		dwFileSizeTemp = GetFileSize( hFile, NULL );
				        		ISDBTSFileHeader.dwFileSize = dwFileSizeTemp;

								SetFilePointer( hFile, 0, 0, FILE_BEGIN );
		  						bRet = WriteFile( hFile, &ISDBTSFileHeader, sizeof(ISDBTSFileHeader), &dwWritten, NULL);
  								if( !bRet )
  									NKDbgPrintfW( L"TVB:: write TS file header3 %d 0x%08x\r\n", dwWritten, GetLastError() );
  									
				        		bRet = CloseHandle(hFile);
				        	}
				        	
				        	NKDbgPrintfW( L"TVB:: FileRecordThread close file %s\r\n", bRet?L"Ok":L"Fail" );
				        	
				        	if( bRet ) {
				        		hFile = INVALID_HANDLE_VALUE;
				        		dwTimeout = INFINITE;
				        		bStopRecording = FALSE;
				        	}
				        	else
				        		dwTimeout = 0;
	   					}	
	   				}	
				
				}

#else       	
        		dwTickCount1 = GetTickCount();
				
				///check if any data in FIFO
    			if( 0 < pFIFO->GetCurFIFODepth() ) {
    				DWORD dwReturn = 0;
    				///get data from FIFO to buffer2
    				dwReturn = pFIFO->FIFOCopy2( pFBuffer2->GetBufferPointer(), pFBuffer2->GetMaxBufferSize() );
    				if( dwReturn > 0 ) {
    					pFBuffer2->SetBufDataLen( dwReturn );
    					
    					///put buffer data to file
	  					if( hFile != INVALID_HANDLE_VALUE ) {
	  						PBYTE pToBuf;
	  						pToBuf = pFBuffer2->GetBufferPointer();
	  						///pFBuffer2->DumpBuffer();
	  						///NKDbgPrintfW( L"TVB::  WriteFile 0x%08x %d !!!\r\n", pToBuf+4, pFBuffer2->GetBufferSize()-4 );
	  						///check if it's right data
	  						if( ((*(pToBuf)&0xF0) == 0x50) && (*(pToBuf+1) == 0x61) ) {
	  							bRet = WriteFile( hFile, pToBuf+4, pFBuffer2->GetBufferSize()-4, &dwWritten, NULL);
	  							if( !bRet )
	  								NKDbgPrintfW( L"TVB:: WriteFile error! write %d=%d Err=0x%08x\r\n", pFBuffer2->GetBufferSize()-4, dwWritten, GetLastError() );
	  						} else {
	  							NKDbgPrintfW( L"TVB:: FileRecordThread stream error! \r\n" );
	  						}
	  					}
	  					else
	  						NKDbgPrintfW( L"TVB:: FileRecordThread file error! \r\n" );
    				
    					pFBuffer2->ClearBuffer();	
    				}
    				else
    					NKDbgPrintfW( L"TVB:: FileRecordThread FIFOCopy2 error! \r\n" );
					
					///check operation timing    					
    				if( RECORD_INTERVAL < (GetTickCount() - dwTickCount1) )
    					NKDbgPrintfW( L"TVB:: write file %d bytes takes %d ms\r\n", dwReturn, GetTickCount() - dwTickCount1 );
    			}
    			else
    				NKDbgPrintfW( L"TVB:: FileRecordThread FIFO is empty\r\n" );

   				if( 0 < pFIFO->GetCurFIFODepth() ) {
   					NKDbgPrintfW( L"TVB:: FileRecordThread still %d data in buffer\r\n", pFIFO->GetCurFIFODepth() );
   					dwTimeout = 0;
   				} else {	
   					///NKDbgPrintfW( L"TVB:: FileRecordThread buffer all empty\r\n" );
   					dwTimeout = INFINITE;
   					
   					///check if we are going to stop
   					if( bStopRecording ) {

        				dwRecduration += (GetTickCount() - dwDurationCount);
   						   						
   						ISDBTSFileHeader.dwDuration = dwRecduration;
   						GetLocalTime( &ISDBTSFileHeader.systemTime );
						if( (dwProjectCode == 0x67333130) && ( hFile != INVALID_HANDLE_VALUE )) {	///PT310
							///BOOL bRet;
							///DWORD dwWritten;
							SetFilePointer( hFile, 0, 0, FILE_BEGIN );///in the head
				  			bRet = WriteFile( hFile, &ISDBTSFileHeader, sizeof(ISDBTSFileHeader), &dwWritten, NULL);
		  					if( !bRet )
		  						NKDbgPrintfW( L"TVB:: write TS file header %d 0x%08x\r\n", dwWritten, GetLastError() );
						}
		
			        	///close file
			        	bRet = CloseHandle(hFile);
			        	
			        	NKDbgPrintfW( L"TVB:: FileRecordThread close file %s\r\n", bRet?L"Ok":L"Fail" );
			        	
			        	if( bRet ) {
			        		hFile = INVALID_HANDLE_VALUE;
			        		dwTimeout = INFINITE;
			        		bStopRecording = FALSE;
			        	}
   					}	
   				}
   				
#endif	///#ifdef FILE_BULK_WRITE
				break;
	        case (WAIT_OBJECT_0 + 0):
	        	/// start record
	        	NKDbgPrintfW( L"TVB:: FileRecordThread file start\r\n" );
	        	dwTimeout = INFINITE;
	        	dwRecduration = 0;	///reset it
	        	dwDurationCount = 0;	///reset it
				if( hFile == INVALID_HANDLE_VALUE ) {
			    	hFile = CreateFile( TV_RECOED_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			    	NKDbgPrintfW( L"TVB:: CreateFile ok 0x%08x\r\n", hFile );
			    } else
			    	NKDbgPrintfW( L"TVB:: file already open\r\n" );

				if( (dwProjectCode == 0x67333130) && ( hFile != INVALID_HANDLE_VALUE )) {	///PT310
					BOOL bRet;
					DWORD dwWritten;
		  			bRet = WriteFile( hFile, &ISDBTSFileHeader, sizeof(ISDBTSFileHeader), &dwWritten, NULL);
  					if( !bRet )
  						NKDbgPrintfW( L"TVB:: write TS file header %d 0x%08x\r\n", dwWritten, GetLastError() );
				}

				
	            break;
	        case (WAIT_OBJECT_0 + 2):
	        	/// stop record
	        	///Sleep(3000);	///wait for write file done        	
///   				while( pFBuffer1->IsBufferValid() || pFBuffer2->IsBufferValid() || pFBuffer3->IsBufferValid() || pFBuffer4->IsBufferValid() ) {
///   					NKDbgPrintfW( L"TVB:: FileRecordThread still data in buffer. do not close file !\r\n" );
///   					Sleep(1000);
///   				}
			
	        	NKDbgPrintfW( L"TVB:: FileRecordThread file stop\r\n" );
	        	bStopRecording = TRUE;
	        		        	
	        	dwTimeout = 1;
				
	            break;
			default:
				NKDbgPrintfW( L"TVB:: FileRecordThread file error\r\n" );
				break;
		}
		
	}///while(1)
	
	CloseHandle( hev[0] );
	CloseHandle( hev[1] );
	CloseHandle( hev[2] );	

	NKDbgPrintfW( L"TVB:: FileRecordThread stop\r\n" );
}
///span1108, add for record

void TvPlaybackThread(void)
{
#define  FILE_EVENT_NUMP	4
	DWORD dwStatus = 0;
	BOOL bRet = FALSE;
	DWORD dwRetSize = 0;	
	///DWORD dwNumEvents = FILE_EVENT_NUM;	
	DWORD dwTimeout = INFINITE;
	HANDLE hev[FILE_EVENT_NUMP];

    HANDLE hFile = INVALID_HANDLE_VALUE;
    ///CNS_PLAYBACK_BUFFERIN PlaybackBuf;
    DTV_PLAYBACK_BUFFERIN PlaybackBuf;
    DWORD dwFileSize = 0;
    DWORD dwFileRemainSize = 0;
    DWORD dwBytesToRead = 0;
    DWORD dwTickCount1 = 0;
    DWORD dwTickCount2 = 0;
	BOOL bDataOverFlow = FALSE;
	DWORD dwPlatBackInterval = PLAYBACK_INTERVAL;
	static DWORD dwPrvTick = 0;
	static DWORD dwValidBufCount = 0;
	static BOOL bIsDABfile = FALSE;	///move to DAB firmware
	DWORD dwIOCTLcode = 0;
	DWORD dwPlaybackBufferSize = 0;
	DWORD dwPacketCount = 0;

	hev[0] = CreateEvent(NULL, FALSE, FALSE, NAMED_PLAYBACKSTART_EVENT );
	hev[1] = CreateEvent(NULL, FALSE, FALSE, NAMED_PLAYBACKPAUSE_EVENT );
	hev[2] = CreateEvent(NULL, FALSE, FALSE, NAMED_PLAYBACKSTOP_EVENT );

	if( dwProjectCode == 0x67333130 ) {	///PT310
		hev[3] = CreateEvent(NULL, FALSE, FALSE, NAMED_PLAYBACK_DTAD_QUERY );
		dwIOCTLcode = IOCTL_DTV_MODULE_PLAYBACK_OPERATION;
		dwPlaybackBufferSize = ISDB_PLAYBACK_BUFFER_SIZE;
	} else {
		///hev[3] = NULL;
		hev[3] = CreateEvent(NULL, FALSE, FALSE, (TEXT("named empty event")) );
		dwIOCTLcode = IOCTL_CNSCMD_PLAYBACK;
		dwPlaybackBufferSize = PLAYBACK_BUFFER_SIZE;
	}

	NKDbgPrintfW( L"TVB:: TvPlaybackThread start\r\n" );
	
	while(1)
	{
		
		if( dwProjectCode != 0x67333130 ) {	///PT310
			if( bIsDABfile )
				dwPlaybackBufferSize = PLAYBACK_DAB_BUFFER_SIZE;
			else
				dwPlaybackBufferSize = PLAYBACK_BUFFER_SIZE;
		}
		
		dwStatus = WaitForMultipleObjects(FILE_EVENT_NUMP, hev, FALSE, dwTimeout);

		switch(dwStatus) {
			case (WAIT_OBJECT_0 + 3):	///ISDB-T playback data
				NKDbgPrintfW( L"TVB:: TvPlaybackThread ISDB file playback stream\r\n" );
        	case WAIT_TIMEOUT:
        		///NKDbgPrintfW( L"TVB:: TvPlaybackThread file playback stream\r\n" );

	        	///check interval timing
	        	if( dwPlatBackInterval < (GetTickCount() - dwTickCount2) )
	        		NKDbgPrintfW( L"TVB:: TvPlaybackThread file playback interval %d(%d) ms\r\n", GetTickCount() - dwTickCount2, dwTimeout  );
        		
        		dwTickCount1 = GetTickCount();	///check operation timing
        		
        		if( hFile != INVALID_HANDLE_VALUE ) {	///make sure file handle is valid
///        			if( !bDataOverFlow ) {
        			if( TRUE ) {	///need no resend
        				pFBuffer3->ClearBuffer();	///preare clean buffer
        				
        				///read file to buffer3
       					bRet = ReadFile( hFile, pFBuffer3->GetBufferPointer(), (DWORD)dwPlaybackBufferSize, &dwBytesToRead, NULL );
        				if( bRet )	///if read file ok, then set the buffer length
	        				pFBuffer3->SetBufDataLen( dwBytesToRead );
						
	        			if( bRet ) {	///if read file ok, then check file status
    	    				if( dwBytesToRead == dwPlaybackBufferSize ) {	///not file tail, countdown remain size
        						dwFileRemainSize = dwFileRemainSize - dwPlaybackBufferSize;
        					} else {	///handle data tail
        						NKDbgPrintfW( L"TVB:: TvPlaybackThread file tail\r\n" );

					        	if( dwProjectCode == 0x67333130 ) {	///PT310
									dwProgramContentChanged = 0;
									dwServiceContentChanged = 0;
									dwProgramTOTChanged = 0;
									dwServiceEWSChanged = 0;
									dwServiceEPGChanged = 0;
									dwPacketCount = 0;
					        	}

#if 0
								///replay from the file head	
        						dwFileRemainSize = 0;
        						SetFilePointer( hFile, 0, 0, FILE_BEGIN );///play again
#else
								///stop ! re-start again
        						dwFileRemainSize = 0;

					        	/// stop playback
					        	NKDbgPrintfW( L"TVB:: TvPlaybackThread file stop\r\n" );
				
							    if( hFile != INVALID_HANDLE_VALUE ) {
									PlaybackBuf.Operation = 2;	///playback stop op
					        		PlaybackBuf.pBufIn = NULL;	///playback stop op
					        		PlaybackBuf.BufInSize = 0;	///playback stop op
					        		bRet = DeviceIoControl(hDTV, dwIOCTLcode, (PBYTE)&PlaybackBuf, sizeof(PlaybackBuf), NULL, 0, &dwRetSize, NULL);
					        		if( !bRet )
					        			NKDbgPrintfW( L"TVB:: playback stop DeviceIoControl fail\r\n" );
								}
				
					        	///close file
					        	bRet = CloseHandle(hFile);
					        	
					        	NKDbgPrintfW( L"TVB:: TvPlaybackThread close file %s\r\n", bRet?L"Ok":L"Fail" );
					        	
					        	if( bRet ) {
					        		hFile = INVALID_HANDLE_VALUE;
					        		DtvStatus( DVT_STATE_STOPFUNC );
	    	    					wsprintf( szString, TEXT("Stop %s file play %s"), (!bIsDABfile)?L"DAB":L"DMB", bRet?L"Ok":L"Fail" );
									SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					        	}
					        	
					        	dwFileSize = 0;
					        	
					        	if( dwProjectCode == 0x67333130 ) {	///PT310
					        		Sleep(100);
					        		///Sleep(1000);
					        		bIsDABfile = FALSE;
									dwProgramContentChanged = 0;
									dwServiceContentChanged = 0;
									dwProgramTOTChanged = 0;
									dwServiceEWSChanged = 0;
									dwServiceEPGChanged = 0;
									dwPacketCount = 0;
					        	} else {
					        		Sleep(500);	///wait for chip ready after command
					        	}
				
								dwPlatBackInterval = PLAYBACK_INTERVAL;	///reset to default interval
								
					        	/// start playback
					        	NKDbgPrintfW( L"TVB:: TvPlaybackThread file start\r\n" );
					        	DtvStatus( DVT_STATE_PLAYBACK );
				
								if( hFile == INVALID_HANDLE_VALUE ) {
									if( bIsDABfile )	///move to DAB firmware
							    		hFile = CreateFile( TV_RECOED_DAB_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
									else
										hFile = CreateFile( TV_RECOED_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
							    	NKDbgPrintfW( L"TVB:: CreateFile %s file ok 0x%08x\r\n", bIsDABfile?L"DAB":L"DMB", hFile );
					    	    	wsprintf( szString, TEXT("Open %s file Ok"), bIsDABfile?L"DAB":L"DMB" );
									SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
							    } else {
							    	NKDbgPrintfW( L"TVB:: file already open\r\n" );
					    	    	wsprintf( szString, TEXT("file already open") );
									SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);			    	
							    }	
							    
							    if( hFile != INVALID_HANDLE_VALUE ) {
							    	dwFileSize = GetFileSize( hFile, NULL );
							    	dwFileRemainSize = dwFileSize;
							    	SetFilePointer( hFile, 0, 0, FILE_BEGIN );
					        		
///move to DAB firmware	        		PlaybackBuf.Operation = 0;	///playback start op
									if( bIsDABfile )	///move to DAB firmware
										PlaybackBuf.Operation = 8;	///playback start op
									else
	        							PlaybackBuf.Operation = 7;	///playback start op
	        						if( dwProjectCode == 0x67333130 ) {	///PT310
	        							PlaybackBuf.Operation = 0;	///playback start op
	        						}
					        		PlaybackBuf.pBufIn = NULL;	///playback start op
					        		PlaybackBuf.BufInSize = 0;	///playback start op
					        		bRet = DeviceIoControl(hDTV, dwIOCTLcode, (PBYTE)&PlaybackBuf, sizeof(PlaybackBuf), NULL, 0, &dwRetSize, NULL);
					        		if( !bRet )
					        			NKDbgPrintfW( L"TVB:: playback start DeviceIoControl fail\r\n" );

									if( (dwProjectCode == 0x67333130) && ( hFile != INVALID_HANDLE_VALUE )) {	///PT310
										MIO_ISDB_TS_FILE_HEADER TempHeader;
										bRet = ReadFile( hFile, &TempHeader, sizeof(TempHeader), &dwBytesToRead, NULL );
										if( bRet ) {
										#if 0
											DWORD dwTemp;
	
							    	    	wsprintf( szString, TEXT("TS type=%c%c%c%c%c%c%c%c"), 
							    	    		TempHeader.bTStype[0], TempHeader.bTStype[1], TempHeader.bTStype[2],
							    	    		TempHeader.bTStype[3], TempHeader.bTStype[4], TempHeader.bTStype[5],
							    	    		TempHeader.bTStype[6], TempHeader.bTStype[7]
							    	    	);
											SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
							    	    	wsprintf( szString, TEXT("Device=%c%c%c%c%c%c%c%c"), 
							    	    		TempHeader.bDeviceName[0], TempHeader.bDeviceName[1], TempHeader.bDeviceName[2],
							    	    		TempHeader.bDeviceName[3], TempHeader.bDeviceName[4], TempHeader.bDeviceName[5],
							    	    		TempHeader.bDeviceName[6], TempHeader.bDeviceName[7]
							    	    	);
											SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				
							    	    	wsprintf( szString, TEXT("Dirver Ver=%d.%d"), TempHeader.dwDrvMajorVer, TempHeader.dwDrvMinorVer );
											SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
											
							    	    	wsprintf( szString, TEXT("Ts rate=%d"), TempHeader.dwTSrate );
											SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				
							    	    	wsprintf( szString, TEXT("Encrypt=%d"), TempHeader.dwEncrypted );
											SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				
											
											dwTemp = TempHeader.dwTSrate;
											bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_SET_TS_STREAM_RATE, &dwTemp, sizeof(dwTemp), NULL, 0, &dwActSize, NULL);
											if( !bRet ) {
							    	    		wsprintf( szString, TEXT("Set Ts rate fail") );
												SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
												NKDbgPrintfW( L"TVB:: playback set TS stream rate fail\r\n" );
											}
										#endif	
										} else
											NKDbgPrintfW( L"TVB:: playback read TS file header fail\r\n" );
									}
									
					        		if( dwProjectCode == 0x67333130 ) {	///PT310
										dwPacketCount = 0;
        								if( dwPacketCount < ISDB_PLAYBACK_PRE_FILL_PACKET )
        									dwTimeout = 1;
        								else
        									dwTimeout = INFINITE;
					        		} else {	
					        			Sleep(500);	///wait for chip ready after command
					        		}

								}
							
 								///get data from File to buffer3
 								if( hFile != INVALID_HANDLE_VALUE ) {
			        				///read file to buffer3
			        				bRet = ReadFile( hFile, pFBuffer3->GetBufferPointer(), (DWORD)dwPlaybackBufferSize, &dwBytesToRead, NULL);
			        				if( bRet )
				        				pFBuffer3->SetBufDataLen( dwBytesToRead );
    	    					}
    	    					if( dwBytesToRead == dwPlaybackBufferSize ) {	///not file tail, countdown remain size
        							dwFileRemainSize = dwFileRemainSize - dwPlaybackBufferSize;
        						}
#endif        						
        					}///handle data tail
        				}
        				else
        					NKDbgPrintfW( L"TVB:: TvPlaybackThread ReadFile error 0x%08x\r\n", GetLastError() );
					} else {
						bRet = TRUE;
						NKDbgPrintfW( L"TVB:: TvPlaybackThread file resend\r\n" );
					}	
					
					///data read ready, send to driver
					if( bRet ) {	
						///NKDbgPrintfW( L"TVB:: TvPlaybackThread playback 0x%08x %d bytes\r\n", pFBuffer3->GetBufferPointer(), pFBuffer3->GetBufferSize() );
						bDataOverFlow = FALSE;	///clear it
						///static DWORD dwPrvTick = 0;
						///static DWORD dwValidBufCount = 0;
						
		        		PlaybackBuf.Operation = 3;	///playback stream op
		        		PlaybackBuf.pBufIn = pFBuffer3->GetBufferPointer();	///playback stream op
	    	    		PlaybackBuf.BufInSize = pFBuffer3->GetBufferSize();	///playback stream op
	    				bRet = DeviceIoControl(hDTV, dwIOCTLcode, (PBYTE)&PlaybackBuf, sizeof(PlaybackBuf), pFBuffer3->GetBufferPointer(), pFBuffer3->GetBufferSize(), &dwRetSize, NULL);
	    				
	    				if( dwProjectCode == 0x67333130 ) {	///PT310
	    					NKDbgPrintfW( L"TVB:: TvPlaybackThread DeviceIoControl 0x%08x %s\r\n", dwIOCTLcode, bRet?L"Ok":L"Fail" );
	    					if( !bRet ) {

	    					} else {
	    						
	    					}
	    				} else {
		    				///check response
		    				if( !bRet ) {
		    					DWORD *pdwResponse = (DWORD*)pFBuffer3->GetBufferPointer();
		    					///NKDbgPrintfW( L"TVB:: TvPlaybackThread DeviceIoControl fail 0x%08x %d\r\n", *(pdwResponse+1), dwRetSize );
		    					
		    					if( 0x080000f0 == *(pdwResponse+1) ) {
		    						///static DWORD dwPrvTick = 0;
		    						bDataOverFlow = TRUE;
		    						NKDbgPrintfW( L"TVB:: TvPlaybackThread playback stream overflow !" );
		    						if( 0 != dwPrvTick ) {
		    							DWORD dwTickOffset = GetTickCount()-dwPrvTick;
		    							NKDbgPrintfW( L" overflow offset=%d", dwTickOffset );
						#if 1
		    							if( dwTickOffset < 60000 ) {	///60 sec
		    								dwPlatBackInterval++;
		    								NKDbgPrintfW( L"\r\nTVB:: change playback stream interval %d -> %d!!", dwPlatBackInterval-1, dwPlatBackInterval );
		    							}					
						#else
		    							if( dwTickOffset < 10000 ) {	///10 sec
		    								dwPlatBackInterval+=2;
		    								NKDbgPrintfW( L"\r\nTVB:: change playback stream interval %d -> %d!!", dwPlatBackInterval-2, dwPlatBackInterval );
		    							} else if( dwTickOffset < 60000 ) {	///60 sec
		    								dwPlatBackInterval++;
		    								NKDbgPrintfW( L"\r\nTVB:: change playback stream interval %d -> %d!!", dwPlatBackInterval-1, dwPlatBackInterval );
		    							}
		    			#endif
		    						}
									NKDbgPrintfW( L"\r\n" );
	    							dwPrvTick = GetTickCount();
	    							dwValidBufCount = 0;	///clear it, re-count
		    					} else {
		    						NKDbgPrintfW( L"TVB:: TvPlaybackThread DeviceIoControl fail 0x%08x %d\r\n", *(pdwResponse+1), dwRetSize );
		    					}
		    				} else {
					#if 1
								static DWORD dwStartTick;
								///count how much time pass since last playback overflow
								if( 0 == dwValidBufCount ) {
									/// we just get overflow
									dwStartTick = GetTickCount();
								}
								
								///check if over 90 sec							
								if( 90000 < (GetTickCount()-dwStartTick) )	{/// 90 sec
		    						dwPlatBackInterval--;
		    						NKDbgPrintfW( L"\r\nTVB:: change playback stream interval %d -> %d %d packet passed !!\r\n", dwPlatBackInterval+1, dwPlatBackInterval, dwValidBufCount );
									dwValidBufCount = 0;	///clear it, re-count
								} else {
									dwValidBufCount++;
								}
					#else	    					
		    					///count how many valid buffer has been pass
		    					dwValidBufCount++;
	
		    					if( ( dwValidBufCount * (dwPlatBackInterval+(GetTickCount()-dwTickCount1) ) ) > 300000 ) {	///5 min
		    						dwPlatBackInterval--;
		    						NKDbgPrintfW( L"\r\nTVB:: change playback stream interval %d -> %d!!\r\n", dwPlatBackInterval+1, dwPlatBackInterval );
		    						dwValidBufCount = 0;	///clear it, re-count
		    					}
					#endif	    					
		    				}
		    				
		    				if( !bDataOverFlow ) {
		    					DWORD dwCountOperate = 0;
		    					dwCountOperate = (GetTickCount()-dwTickCount1);
						#if 1	    					
		    					dwTimeout = (dwCountOperate>dwPlatBackInterval)?0:(dwPlatBackInterval-dwCountOperate);
						#else	    						
		    					if( dwCountOperate > dwPlatBackInterval )
			    					dwTimeout = 0;	/// operation time too long, do next immediately !!!
			    				else	
		    						dwTimeout = dwPlatBackInterval - dwCountOperate;
						#endif	    						
		    				} else {
		    					///data over flow ... slow down
		    					dwTimeout = 400;
		    					///dwTimeout = 600;
		    				}
		    			}
        			}
///        			else
///     				NKDbgPrintfW( L"TVB:: TvPlaybackThread ReadFile error 0x%08x\r\n", GetLastError() );
        				
        		} else {
        			NKDbgPrintfW( L"TVB:: TvPlaybackThread file handle error !!\r\n" );
        		}
        		
        		if( GetTickCount()-dwTickCount1 > dwPlatBackInterval )
					NKDbgPrintfW( L"TVB:: TvPlaybackThread playback takes %d ms\r\n", GetTickCount()-dwTickCount1  );
					
        		dwTickCount2 = GetTickCount();
        		if( dwProjectCode == 0x67333130 ) {	///PT310
        			dwPacketCount++;
        			if( dwPacketCount < ISDB_PLAYBACK_PRE_FILL_PACKET )
        				dwTimeout = 1;
        			else
        				dwTimeout = INFINITE;
        		}
        		break;
	        case (WAIT_OBJECT_0 + 1):
	        	/// pause
	        	NKDbgPrintfW( L"TVB:: TvPlaybackThread file playback pause\r\n" );
	        	if ( dwTimeout != INFINITE ) {
	        		dwTimeout = INFINITE;
	        		PlaybackBuf.Operation = 1;	///playback pause op
	        	} else {
	        		dwTimeout = dwPlatBackInterval;
	        		PlaybackBuf.Operation = 4;	///playback resume op
	        	}	

				if( dwProjectCode == 0x67333130 ) {	///PT310
					static BOOL bPause = FALSE;
					if( FALSE == bPause ) {
						bPause = TRUE;
						PlaybackBuf.Operation = 1;	///playback pause op
					} else {
						bPause = FALSE;
						PlaybackBuf.Operation = 4;	///playback resume op
					}
					dwTimeout = INFINITE;
				}

			    if( hFile != INVALID_HANDLE_VALUE ) {
	        		///PlaybackBuf.Operation = 0;	///playback pause/resume op
	        		PlaybackBuf.pBufIn = NULL;	///playback pause/resume op
	        		PlaybackBuf.BufInSize = 0;	///playback pause/resume op
	        		bRet = DeviceIoControl(hDTV, dwIOCTLcode, (PBYTE)&PlaybackBuf, sizeof(PlaybackBuf), NULL, 0, &dwRetSize, NULL);
	        		if( !bRet )
	        			NKDbgPrintfW( L"TVB:: playback DeviceIoControl fail\r\n" );
				}

    	    	wsprintf( szString, TEXT("Play %s file %s %s"), bIsDABfile?L"DAB":L"DMB", (dwTimeout == INFINITE)?L"pause":L"resume", bRet?L"Ok":L"Fail" );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	        	
				break;
	        case (WAIT_OBJECT_0 + 0):
	        	/// start playback
	        	NKDbgPrintfW( L"TVB:: TvPlaybackThread file start\r\n" );
	        	dwPlatBackInterval = PLAYBACK_INTERVAL;	///reset to default interval
				
				DtvStatus( DVT_STATE_PLAYBACK );

				if( hFile == INVALID_HANDLE_VALUE ) {
					if( bIsDABfile )	///move to DAB firmware
			    		hFile = CreateFile( TV_RECOED_DAB_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
					else
						hFile = CreateFile( TV_RECOED_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
			    	NKDbgPrintfW( L"TVB:: CreateFile %s file %s 0x%08x\r\n", bIsDABfile?L"DAB":L"DMB", (hFile!=INVALID_HANDLE_VALUE)?L"Ok":L"Fail", hFile );
	    	    	wsprintf( szString, TEXT("Open %s file %s"), bIsDABfile?L"DAB":L"DMB", (hFile!=INVALID_HANDLE_VALUE)?L"Ok":L"Fail" );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			    } else {
			    	NKDbgPrintfW( L"TVB:: file already open\r\n" );
	    	    	wsprintf( szString, TEXT("file already open") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);			    	
			    }	

				///move to DAB firmware
				///try again with another format
				if( hFile == INVALID_HANDLE_VALUE ) {
					if( dwProjectCode == 0x67333130 ) {	///PT310
					} else {
						bIsDABfile = !bIsDABfile;
					}
					if( bIsDABfile )	///move to DAB firmware
			    		hFile = CreateFile( TV_RECOED_DAB_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
					else
						hFile = CreateFile( TV_RECOED_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );			    		
			    	NKDbgPrintfW( L"TVB:: CreateFile %s file %s 0x%08x\r\n", bIsDABfile?L"DAB":L"DMB", (hFile!=INVALID_HANDLE_VALUE)?L"Ok":L"Fail", hFile );
	    	    	wsprintf( szString, TEXT("Open %s file %s"), bIsDABfile?L"DAB":L"DMB", (hFile!=INVALID_HANDLE_VALUE)?L"Ok":L"Fail" );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			    }	

			    
			    if( hFile != INVALID_HANDLE_VALUE ) {
			    	dwFileSize = GetFileSize( hFile, NULL );
			    	dwFileRemainSize = dwFileSize;
			    	SetFilePointer( hFile, 0, 0, FILE_BEGIN );
	        		dwTimeout = dwPlatBackInterval;
	        		
///move to DAB firmware	        		PlaybackBuf.Operation = 0;	///playback start op
					if( bIsDABfile )	///move to DAB firmware
						PlaybackBuf.Operation = 8;	///playback start op
					else
	        			PlaybackBuf.Operation = 7;	///playback start op
	        			
	        		if( dwProjectCode == 0x67333130 ) {	///PT310
	        			PlaybackBuf.Operation = 0;	///playback start op
	        		}
	        		PlaybackBuf.pBufIn = NULL;	///playback start op
	        		PlaybackBuf.BufInSize = 0;	///playback start op
	        		bRet = DeviceIoControl(hDTV, dwIOCTLcode, (PBYTE)&PlaybackBuf, sizeof(PlaybackBuf), NULL, 0, &dwRetSize, NULL);
	        		if( !bRet ) {
	        			NKDbgPrintfW( L"TVB:: playback DeviceIoControl fail\r\n" );
	        			PlaybackBuf.Operation = 0;	///playback start op, re-try for old firmware
	        			bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_PLAYBACK, (PBYTE)&PlaybackBuf, sizeof(PlaybackBuf), NULL, 0, &dwRetSize, NULL);
	        		}

					if( (dwProjectCode == 0x67333130) && ( hFile != INVALID_HANDLE_VALUE )) {	///PT310
						MIO_ISDB_TS_FILE_HEADER TempHeader;
						bRet = ReadFile( hFile, &TempHeader, sizeof(TempHeader), &dwBytesToRead, NULL );
						if( bRet ) {
							DWORD dwTemp;
							
			    	    	wsprintf( szString, TEXT("TS type=%c%c%c%c%c%c%c%c"), 
			    	    		TempHeader.bTStype[0], TempHeader.bTStype[1], TempHeader.bTStype[2],
			    	    		TempHeader.bTStype[3], TempHeader.bTStype[4], TempHeader.bTStype[5],
			    	    		TempHeader.bTStype[6], TempHeader.bTStype[7]
			    	    	);
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			    	    	wsprintf( szString, TEXT("Device=%c%c%c%c%c%c%c%c"), 
			    	    		TempHeader.bDeviceName[0], TempHeader.bDeviceName[1], TempHeader.bDeviceName[2],
			    	    		TempHeader.bDeviceName[3], TempHeader.bDeviceName[4], TempHeader.bDeviceName[5],
			    	    		TempHeader.bDeviceName[6], TempHeader.bDeviceName[7]
			    	    	);
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

			    	    	wsprintf( szString, TEXT("Dirver Ver=%d.%d"), TempHeader.dwDrvMajorVer, TempHeader.dwDrvMinorVer );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
							
			    	    	wsprintf( szString, TEXT("Ts rate=%d"), TempHeader.dwTSrate );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

			    	    	wsprintf( szString, TEXT("Duration=%dms"), TempHeader.dwDuration );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

			    	    	wsprintf( szString, TEXT("FileSize=%dKB"), TempHeader.dwFileSize );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

			    	    	wsprintf( szString, TEXT("Encrypt=%d"), TempHeader.dwEncrypted );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

							
							dwTemp = TempHeader.dwTSrate;
							bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_SET_TS_STREAM_RATE, &dwTemp, sizeof(dwTemp), NULL, 0, &dwActSize, NULL);
							if( !bRet ) {
			    	    		wsprintf( szString, TEXT("Set Ts rate fail") );
								SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
								NKDbgPrintfW( L"TVB:: playback set TS stream rate fail\r\n" );
							}
							
						} else
							NKDbgPrintfW( L"TVB:: playback read TS file header fail\r\n" );
						
					}


					if( bRet && dwProjectCode == 0x67333130 ) {
						dwPacketCount = 0;
        				if( dwPacketCount < ISDB_PLAYBACK_PRE_FILL_PACKET )
        					dwTimeout = 1;
        				else
        					dwTimeout = INFINITE;
					}

	    	    	wsprintf( szString, TEXT("Play %s file start %s"), bIsDABfile?L"DAB":L"DMB", bRet?L"Ok":L"Fail" );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					
					if( dwProjectCode != 0x67333130 ) {	///PT310
	        			Sleep(500);	///wait for chip ready after command
	        		}
				} else {
	    	    	wsprintf( szString, TEXT("Open %s file Fail"), bIsDABfile?L"DAB":L"DMB" );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				}
				
	            break;
	        case (WAIT_OBJECT_0 + 2):
	        	/// stop playback
	        	NKDbgPrintfW( L"TVB:: TvPlaybackThread file stop\r\n" );

				if( dwProjectCode == 0x67333130 ) {	///PT310
					dwProgramContentChanged = 0;
					dwServiceContentChanged = 0;
					dwProgramTOTChanged = 0;
					dwServiceEWSChanged = 0;
					dwServiceEPGChanged = 0;
					dwPacketCount = 0;
				}

			    if( hFile != INVALID_HANDLE_VALUE ) {
	        		PlaybackBuf.Operation = 2;	///playback stop op
	        		PlaybackBuf.pBufIn = NULL;	///playback stop op
	        		PlaybackBuf.BufInSize = 0;	///playback stop op
	        		bRet = DeviceIoControl(hDTV, dwIOCTLcode, (PBYTE)&PlaybackBuf, sizeof(PlaybackBuf), NULL, 0, &dwRetSize, NULL);
	        		if( !bRet )
	        			NKDbgPrintfW( L"TVB:: playback DeviceIoControl fail\r\n" );
	        		else
	        			bIsDABfile = !bIsDABfile;
	        		if( dwProjectCode == 0x67333130 ) {	///PT310
	        			bIsDABfile = FALSE;
	        		}
				}

	        	///close file
	        	bRet = CloseHandle(hFile);
	        	
	        	NKDbgPrintfW( L"TVB:: TvPlaybackThread close file %s\r\n", bRet?L"Ok":L"Fail" );
	        	
	        	if( bRet ) {
	        		hFile = INVALID_HANDLE_VALUE;
	        		DtvStatus( DVT_STATE_STOPFUNC );
	    	    	wsprintf( szString, TEXT("Stop %s file play %s"), (!bIsDABfile)?L"DAB":L"DMB", bRet?L"Ok":L"Fail" );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				}

				dwPrvTick = 0;
				dwValidBufCount = 0;

	        	dwFileSize = 0;
	        	dwTimeout = INFINITE;
				
	            break;
			default:
				NKDbgPrintfW( L"TVB:: TvPlaybackThread file error\r\n" );
				break;
		}
		
	}///while(1)
	
	CloseHandle( hev[0] );
	CloseHandle( hev[1] );
	CloseHandle( hev[2] );	
	CloseHandle( hev[3] );
	
	NKDbgPrintfW( L"TVB:: TvPlaybackThread stop\r\n" );
}


void TvDrvEventThread(void)
{
#define  DRV_EVENT_NUM	3

	DWORD	dwStatus = 0;
	BOOL	bRet = FALSE;
	DWORD	dwRetSize = 0;	
	DWORD 	dwTimeout = INFINITE;
	HANDLE 	hev[DRV_EVENT_NUM];

	DWORD dwEventData = 0xFFFF;
	DWORD dwLastTickBattery = 0;
	
	
	hev[0] = CreateEvent(NULL, FALSE, FALSE, NAMED_WARP_NOTIFY_EVENT );
	hev[1] = CreateEvent(NULL, FALSE, FALSE, (TEXT("named Drv event 1 event")) );
	hev[2] = CreateEvent(NULL, FALSE, FALSE, (TEXT("named Drv event 2 event")) );
	
	NKDbgPrintfW( L"TVB:: TvDrvEventThread start\r\n" );
	
	while(1)
	{
		
		dwStatus = WaitForMultipleObjects(DRV_EVENT_NUM, hev, FALSE, dwTimeout);

		switch(dwStatus) {
        	case WAIT_TIMEOUT:
        		break;
	        case (WAIT_OBJECT_0 + 0):
	        	NKDbgPrintfW( L"TVB:: TvDrvEventThread event 0x%08x\r\n", hev[0] );
				///parse event data
				BOOL bRet;

				dwEventData = GetEventData( hev[0] );
				NKDbgPrintfW( L"TVB:: TvDrvEventThread dwEventData 0x%08x\r\n", dwEventData );
				bRet = SetEventData( hev[0], 0 );		            ///clear
				if( !bRet ){
					NKDbgPrintfW( L"TVB:: TvDrvEventThread SetEventData fail 0x%08x\r\n", GetLastError() );
				}
				
				if( 0 == dwEventData ) {
					wsprintf( szString, TEXT("event err 0x%08x"), GetLastError() );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				} else
				if( NOTIFY_PROG_AV_ES_CHG == dwEventData ) {
					wsprintf( szString, TEXT("AV_ES_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					dwProgramContentChanged++;
				} else
				if( NOTIFY_PROG_AUDIO_MODE_CHG == dwEventData ) {
					wsprintf( szString, TEXT("AUDIO_MODE_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					dwProgramContentChanged++;
				} else
				if( NOTIFY_PROG_EIT_CHG == dwEventData ) {
					wsprintf( szString, TEXT("EIT_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					
				} else
				if( NOTIFY_PROG_COPY_INFO_CHG == dwEventData ) {
					wsprintf( szString, TEXT("COPY_INFO_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					dwProgramContentChanged++;				
				} else
				if( NOTIFY_PROG_TEXT_MODE_CHG == dwEventData ) {
					wsprintf( szString, TEXT("TEXT_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					dwProgramContentChanged++;
				} else
				if( NOTIFY_PROG_VIDEO_MODE_CHG == dwEventData ) {
					wsprintf( szString, TEXT("VIDEO_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					dwProgramContentChanged++;
				} else
				if( NOTIFY_PROG_CLOSECAPTION_CHG == dwEventData ) {
					wsprintf( szString, TEXT("CC_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					dwProgramContentChanged++;				
				} else
				if( NOTIFY_NETWORK_CHG == dwEventData ) {
					wsprintf( szString, TEXT("NETWORK_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					dwServiceContentChanged++;				
				} else
				if( NOTIFY_SDT_CHG == dwEventData ) {
					wsprintf( szString, TEXT("SDT_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					dwServiceContentChanged++;
				} else
				if( NOTIFY_SERVICE_SDTT == dwEventData ) {
					wsprintf( szString, TEXT("SERVICE_SDTT") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

				} else
				if( NOTIFY_SERVICE_EWS == dwEventData ) {
					wsprintf( szString, TEXT("SERVICE_EWS") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					dwServiceEWSChanged++;
				} else
				if( NOTIFY_SHM_ERROR == dwEventData ) {
					wsprintf( szString, TEXT("SHM_ERROR") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				} else
				if( NOTIFY_TOT_CHG == dwEventData ) {
					wsprintf( szString, TEXT("TOT") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					dwProgramTOTChanged++;
				} else
				if( NOTIFY_SHM_INIT_READY == dwEventData ) {
					wsprintf( szString, TEXT("SHM_INIT_READY") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				} else {
					wsprintf( szString, TEXT("module event 0x%08x"), dwEventData );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				}

				break;
	        case (WAIT_OBJECT_0 + 1):
	        	/// 
	            break;
	        case (WAIT_OBJECT_0 + 2):
	        	/// 
	            break;
			default:
				NKDbgPrintfW( L"TVB:: TvDrvEventThread default\r\n" );
				///dwTimeout = INFINITE;
				break;
		}
				
	}///while(1)
	
	CloseHandle( hev[0] );
	CloseHandle( hev[1] );
	CloseHandle( hev[2] );
	
	NKDbgPrintfW( L"TVB:: TvDrvEventThread stop\r\n" );
}



void TvCommandThread(void)
{
#define  COMMAND_EVENT_NUM	3
#define	 CONT_POLL_STATUS_INTERVAL	100
///#define	 CONT_POLL_BATTERY_INTERVAL	8000
///#define	 CONT_POLL_BATTERY_INTERVAL	100
#define	 CONT_POLL_BATTERY_INTERVAL	3000
#define	 CONT_POLL_RF_TEST_INTERVAL	1999
	DWORD	dwStatus = 0;
	BOOL	bRet = FALSE;
	DWORD	dwRetSize = 0;	
	///DWORD 	dwTimeout = INFINITE;
	DWORD 	dwTimeout = CONT_POLL_STATUS_INTERVAL;
	HANDLE 	hev[COMMAND_EVENT_NUM];
	HANDLE hModEvent = 0;
	DWORD dwEventData = 0xFFFF;
	DWORD dwLastTickBattery = 0;

	
	hev[0] = CreateEvent(NULL, FALSE, FALSE, NAMED_CMD_REDRAW_EVENT );
	hev[1] = CreateEvent(NULL, FALSE, FALSE, NAMED_CMD_1_EVENT );
	hev[2] = CreateEvent(NULL, FALSE, FALSE, NAMED_CMD_2_EVENT );
	
	NKDbgPrintfW( L"TVB:: TvCommandThread start\r\n" );
	
	while(1)
	{
		
		dwStatus = WaitForMultipleObjects(COMMAND_EVENT_NUM, hev, FALSE, dwTimeout);

		switch(dwStatus) {
        	case WAIT_TIMEOUT:
        		///NKDbgPrintfW( L"TVB:: TvCommandThread timeout\r\n" );
        		
				///we don't handle the event when these function is working!!
				if( 
					DtvStatus(0) & DVT_STATE_PLAYBACK ||
					DtvStatus(0) & DVT_STATE_STILLSHOT ||
					DtvStatus(0) & DVT_STATE_RECOED
				)
				{
					;
				} else {
	        		///check if we need to update the the OSD.
	        		if( 
	        			(!( DtvStatus( 0 ) & 0x0000F000 )) &&
	        			( DtvStatus( 0 ) & DVT_STATE_LOCKED ) &&
	        			( DtvStatus( 0 ) & DVT_STATE_OSD1ON ) &&
	        			( DtvStatus( 0 ) & DVT_STATE_DISPON )
	        		)
	        		{
	        			if( GetTickCount() - dwLastTickBattery > CONT_POLL_BATTERY_INTERVAL ) {
	        				TvSendBatteryInfo( TRUE );
	        				TvSendSignalInfo( TRUE );
	        				dwLastTickBattery = GetTickCount();	///store the tick
	        			} else {
	        				;
	        			}
	        		}
					
					///NKDbgPrintfW( L"TVB::DtvStatus=0x%x\r\n", DtvStatus( 0 ) );
					
					///check event
					if( dwServiceEWSChanged > 0 ) {	///EWS
						ISDB_EWS_INFO ews;
						DWORD dwLoop;
						
						NKDbgPrintfW( L"TVB:: dwServiceEWSChanged=%d\r\n", dwServiceEWSChanged );
						
						dwServiceEWSChanged = 0;
						
					    if ( DeviceIoControl(hDTV, IOCTL_DTV_ISDB_CURRENT_EWS_INFO, &ews, sizeof(ews), NULL, 0, &dwActSize, NULL) ) {
        					NKDbgPrintfW( L"TVB:: EWS.dwServiceId=0x%x\r\n", ews.dwServiceId );
							wsprintf( szString, TEXT("EWS servID=0x%x"), ews.dwServiceId );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
        					NKDbgPrintfW( L"TVB:: EWS.dwStartEndFlag=%d\r\n", ews.dwStartEndFlag );
							wsprintf( szString, TEXT("EWS StartEndFlag=%d"), ews.dwStartEndFlag );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
        					NKDbgPrintfW( L"TVB:: EWS.dwSignalLevel=%d\r\n", ews.dwSignalLevel );
							wsprintf( szString, TEXT("EWS SignalLevel=%d"), ews.dwSignalLevel );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
    	    				NKDbgPrintfW( L"TVB:: EWS.AreaCodeNumber=%d\r\n", ews.dwAreaCodeNumber );
							wsprintf( szString, TEXT("EWS dwAreaCodeNumber=%d"), ews.dwAreaCodeNumber );
							SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
							for( dwLoop = 0; dwLoop < ews.dwAreaCodeNumber; dwLoop++ ) {
    	    					NKDbgPrintfW( L"TVB:: EWS.dwAreaCode=0x%x(%d)\r\n", ews.dwAreaCode[dwLoop], ews.dwAreaCode[dwLoop] );
								wsprintf( szString, TEXT("EWS AreaCode=0x%x(%d)"), ews.dwAreaCode[dwLoop], ews.dwAreaCode[dwLoop] );
								SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
							}

    					} else {
	        				NKDbgPrintfW( L"TVB:: Failed! IOCTL_DTV_ISDB_CURRENT_EWS_INFO\r\n" );
	        				wsprintf( szString, TEXT("EWS fail") );
	        				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	        			}

        				if( 1 ) {
        					GetCurrentChDetailInfo( &ChDetailInfo );
        				}
					} else
					if( dwServiceContentChanged > 0 ) {	///service
						
        				NKDbgPrintfW( L"TVB:: dwServiceContentChanged=%d\r\n", dwServiceContentChanged );
						wsprintf( szString, TEXT("ServiceChanged=%d"), dwServiceContentChanged );
						SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

						dwServiceContentChanged = 0;
						
	        			if( 1 ) {
        					GetCurrentChDetailInfo( &ChDetailInfo );
        				}
					} else
					if( dwProgramContentChanged > 0 ) {	///program
						
        				NKDbgPrintfW( L"TVB:: dwProgramContentChanged=%d\r\n", dwProgramContentChanged );
						wsprintf( szString, TEXT("ProgramChanged=%d"), dwProgramContentChanged );
						SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

						dwProgramContentChanged = 0;
						
						if( 1 ) {
        					GetCurrentChServiceInfo( &ChServInfo );
        				}
						
					} else
					if( dwServiceEPGChanged > 0 ) {	///EPG
						ISDB_EPG_INFO epg;
						
						dwServiceEPGChanged = 0;

						GetCurrentEPG( & epg );
					} else
					if( dwProgramTOTChanged > 0 ) {	///TOT
					
						NKDbgPrintfW( L"TVB:: dwProgramTOTChanged=%d\r\n", dwProgramTOTChanged );
						dwProgramTOTChanged = 0;
						
						if( 1 ) {
							ISDB_TSTOT tot;
					   	 	if ( DeviceIoControl(hDTV, IOCTL_DTV_ISDB_LASTEST_TOT, &tot, sizeof(tot), NULL, 0, &dwActSize, NULL) ) {
        						NKDbgPrintfW( L"TVB:: TOT.high=0x%08x TOT.low=0x%08x\r\n", tot.dwHiNum, tot.dwLoNum );
								wsprintf( szString, TEXT("TOT 0x%08x:0x%08x"), tot.dwHiNum, tot.dwLoNum );
    						} else {
	        					NKDbgPrintfW( L"TVB:: Failed! IOCTL_DTV_ISDB_LASTEST_TOT\r\n" );
	        					wsprintf( szString, TEXT("TOT fail") );
	        				}
	        				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	        			}
	        			if( 1 ) {
							ISDB_PROGCLKREF pcr;
					    	if ( DeviceIoControl(hDTV, IOCTL_DTV_ISDB_LASTEST_PCR, &pcr, sizeof(pcr), NULL, 0, &dwActSize, NULL) ) {
        						NKDbgPrintfW( L"TVB:: PCR.high=0x%08x PCR.low=0x%08x\r\n", pcr.dwHiNum, pcr.dwLoNum );
								wsprintf( szString, TEXT("PCR 0x%08x:0x%08x"), pcr.dwHiNum, pcr.dwLoNum );
    						} else {
	        					NKDbgPrintfW( L"TVB:: Failed! IOCTL_DTV_ISDB_LASTEST_PCR\r\n" );
	        					wsprintf( szString, TEXT("PCR fail") );
	        				}
	        				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	        			}

					} else {
						;
					}
	

	        		///check if we are now test RF
    	    		if( dwTimeout == CONT_POLL_RF_TEST_INTERVAL ) {
        				TvRFstatusDump( 0, FALSE );
        			}
	
				}

        		break;
	        case (WAIT_OBJECT_0 + 0):
	        	/// redraw
	        	NKDbgPrintfW( L"TVB:: TvCommandThread redraw\r\n" );
	        	///dwTimeout = INFINITE;
	        	
	        	Sleep(200);
	        	
	        	///redraw
	        	
	        	///ShowWindow( g_hDlg, SW_HIDE );
				///SetWindowPos(g_hDlg, 0, 500, 500, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				///ShowWindow( g_hDlg, SW_SHOW );
				///UpdateWindow(g_hDlg);
				
				///ShowWindow( g_hDlg, SW_HIDE );
				///SetWindowPos(g_hDlg, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				///ShowWindow( g_hDlg, SW_SHOW );
				///UpdateWindow(g_hDlg);
				
				ShowWindow( g_hDlg, SW_HIDE );
				ShowWindow( g_hDlg, SW_SHOW );
				UpdateWindow(g_hDlg);
				
				NKDbgPrintfW( L"TVB:: TvCommandThread redraw done\r\n" );
				    
				break;
	        case (WAIT_OBJECT_0 + 1):
	        	NKDbgPrintfW( L"TVB:: TvCommandThread 1\r\n" );
	        	/// 
/*
	        	NKDbgPrintfW( L"TVB:: TvCommandThread module event 0x%08x\r\n", hev[1] );
				///parse event data
				BOOL bRet;

				dwEventData = GetEventData( hModEvent );
				NKDbgPrintfW( L"TVB:: TvCommandThread module dwEventData 0x%08x\r\n", dwEventData );
				bRet = SetEventData( hModEvent, 0 );		            ///clear
				if( !bRet ){
					NKDbgPrintfW( L"TVB:: TvCommandThread SetEventData fail 0x%08x\r\n", GetLastError() );
				}
				
				///we don't handle the event when these function is working!!
				if( 
					DtvStatus(0) & DVT_STATE_PLAYBACK ||
					DtvStatus(0) & DVT_STATE_STILLSHOT ||
					DtvStatus(0) & DVT_STATE_RECOED
				)
				{
					break;
				}
				
				
				if( 0 == dwEventData ) {
					wsprintf( szString, TEXT("event err 0x%08x"), GetLastError() );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				} else
				if( NOTIFY_PROG_AV_ES_CHG == dwEventData ) {
					wsprintf( szString, TEXT("AV_ES_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					if( 0 ) {
        				///ISDB_T_CH_SERVICE_INFO ChServInfo;
        				GetCurrentChServiceInfo( &ChServInfo );
        			}
				} else
				if( NOTIFY_PROG_AUDIO_MODE_CHG == dwEventData ) {
					wsprintf( szString, TEXT("AUDIO_MODE_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					if( 0 ) {
        				///ISDB_T_CH_SERVICE_INFO ChServInfo;
        				GetCurrentChServiceInfo( &ChServInfo );
        			}
				} else
				if( NOTIFY_PROG_EIT_CHG == dwEventData ) {
					wsprintf( szString, TEXT("EIT_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					if( 0 ) {
						ISDB_EPG_INFO epg;
						GetCurrentEPG( & epg );
					}
					
				} else
				if( NOTIFY_PROG_COPY_INFO_CHG == dwEventData ) {
					wsprintf( szString, TEXT("COPY_INFO_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					if( 0 ) {
        				///ISDB_T_CH_SERVICE_INFO ChServInfo;
        				GetCurrentChServiceInfo( &ChServInfo );
        			}
				} else
				if( NOTIFY_PROG_TEXT_MODE_CHG == dwEventData ) {
					wsprintf( szString, TEXT("TEXT_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					if( 0 ) {
        				///ISDB_T_CH_SERVICE_INFO ChServInfo;
        				GetCurrentChServiceInfo( &ChServInfo );
        			}
				} else
				if( NOTIFY_PROG_VIDEO_MODE_CHG == dwEventData ) {
					wsprintf( szString, TEXT("VIDEO_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					if( 0 ) {
        				///ISDB_T_CH_SERVICE_INFO ChServInfo;
        				GetCurrentChServiceInfo( &ChServInfo );
        			}
				} else
				if( NOTIFY_PROG_CLOSECAPTION_CHG == dwEventData ) {
					wsprintf( szString, TEXT("CC_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					if( 0 ) {
        				///ISDB_T_CH_SERVICE_INFO ChServInfo;
        				GetCurrentChServiceInfo( &ChServInfo );
        			}
				} else
				if( NOTIFY_NETWORK_CHG == dwEventData ) {
					wsprintf( szString, TEXT("NETWORK_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
        			if( 0 ) {
        				///ISDB_T_CH_DETAIL_INFO ChDetailInfo;
        				GetCurrentChDetailInfo( &ChDetailInfo );
        			}
				} else
				if( NOTIFY_SDT_CHG == dwEventData ) {
					wsprintf( szString, TEXT("SDT_CHG") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
        			if( 0 ) {
        				///ISDB_T_CH_DETAIL_INFO ChDetailInfo;
        				GetCurrentChDetailInfo( &ChDetailInfo );
        			}

				} else
				if( NOTIFY_SERVICE_SDTT == dwEventData ) {
					wsprintf( szString, TEXT("SERVICE_SDTT") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

				} else
				if( NOTIFY_SERVICE_EWS == dwEventData ) {
					wsprintf( szString, TEXT("SERVICE_EWS") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

					ISDB_EWS_INFO ews;
				    if ( DeviceIoControl(hDTV, IOCTL_DTV_ISDB_CURRENT_EWS_INFO, &ews, sizeof(ews), NULL, 0, &dwActSize, NULL) ) {
        				NKDbgPrintfW( L"TVB:: EWS.dwServiceId=0x%08x\r\n", ews.dwServiceId );
						wsprintf( szString, TEXT("EWS servID=0x%08x"), ews.dwServiceId );
						SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
        				NKDbgPrintfW( L"TVB:: EWS.dwStartEndFlag=0x%08x\r\n", ews.dwStartEndFlag );
						wsprintf( szString, TEXT("EWS StartEndFlag=0x%08x"), ews.dwStartEndFlag );
						SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
        				NKDbgPrintfW( L"TVB:: EWS.dwSignalLevel=0x%08x\r\n", ews.dwSignalLevel );
						wsprintf( szString, TEXT("EWS SignalLevel=0x%08x"), ews.dwSignalLevel );
						SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
        				NKDbgPrintfW( L"TVB:: EWS.dwAreaCodeLen=0x%08x\r\n", ews.dwAreaCodeLen );
						wsprintf( szString, TEXT("EWS dwAreaCodeLen=0x%08x"), ews.dwAreaCodeLen );
						SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

    				} else {
	        			NKDbgPrintfW( L"TVB:: Failed! IOCTL_DTV_ISDB_CURRENT_EWS_INFO\r\n" );
	        			wsprintf( szString, TEXT("EWS fail") );
	        			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	        		}

        			if( 0 ) {
        				///ISDB_T_CH_DETAIL_INFO ChDetailInfo;
        				GetCurrentChDetailInfo( &ChDetailInfo );
        			}

				} else
				if( NOTIFY_SHM_ERROR == dwEventData ) {
					wsprintf( szString, TEXT("SHM_ERROR") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				} else
				if( NOTIFY_TOT_CHG == dwEventData ) {
					ISDB_TSTOT tot;
					
					if( 0 ) {
						///wsprintf( szString, TEXT("TOT_CHG") );
				    	if ( DeviceIoControl(hDTV, IOCTL_DTV_ISDB_LASTEST_TOT, &tot, sizeof(tot), NULL, 0, &dwActSize, NULL) ) {
        					NKDbgPrintfW( L"TVB:: TOT.high=0x%08x TOT.low=0x%08x\r\n", tot.dwHiNum, tot.dwLoNum );
							wsprintf( szString, TEXT("TOT 0x%08x:0x%08x"), tot.dwHiNum, tot.dwLoNum );
    					} else {
	        				NKDbgPrintfW( L"TVB:: Failed! IOCTL_DTV_ISDB_LASTEST_TOT\r\n" );
	        				wsprintf( szString, TEXT("TOT fail") );
	        			}
	        			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	        			
	        			
						ISDB_PROGCLKREF pcr;
						///wsprintf( szString, TEXT("TOT_CHG") );
				    	if ( DeviceIoControl(hDTV, IOCTL_DTV_ISDB_LASTEST_PCR, &pcr, sizeof(pcr), NULL, 0, &dwActSize, NULL) ) {
        					NKDbgPrintfW( L"TVB:: PCR.high=0x%08x PCR.low=0x%08x\r\n", pcr.dwHiNum, pcr.dwLoNum );
							wsprintf( szString, TEXT("PCR 0x%08x:0x%08x"), pcr.dwHiNum, pcr.dwLoNum );
    					} else {
	        				NKDbgPrintfW( L"TVB:: Failed! IOCTL_DTV_ISDB_LASTEST_PCR\r\n" );
	        				wsprintf( szString, TEXT("PCR fail") );
	        			}
	        			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	        		}
				} else
				if( NOTIFY_SHM_INIT_READY == dwEventData ) {
					wsprintf( szString, TEXT("SHM_INIT_READY") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				} else {
					wsprintf( szString, TEXT("module event 0x%08x"), dwEventData );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				}
				///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
*/				
	            break;
	        case (WAIT_OBJECT_0 + 2):
	        	/// 
	        	NKDbgPrintfW( L"TVB:: TvCommandThread 2\r\n" );
	        	
	        	
	        	if( dwTimeout == CONT_POLL_STATUS_INTERVAL )
	        		dwTimeout = CONT_POLL_RF_TEST_INTERVAL;
				else
				if( dwTimeout == CONT_POLL_RF_TEST_INTERVAL )
					dwTimeout = CONT_POLL_STATUS_INTERVAL;
				else
					dwTimeout = CONT_POLL_STATUS_INTERVAL;
				
	            break;
			default:
				NKDbgPrintfW( L"TVB:: TvCommandThread default\r\n" );
				///dwTimeout = INFINITE;
				dwTimeout = CONT_POLL_STATUS_INTERVAL;
				break;
		}
		
	}///while(1)
	
	CloseHandle( hev[0] );
	CloseHandle( hev[1] );
	CloseHandle( hev[2] );
	
	NKDbgPrintfW( L"TVB:: TvCommandThread stop\r\n" );
}


HANDLE GetModuleEventHandel()
{
	HANDLE hTemp = 0;
	DWORD dwTemp = 0;
	DWORD dwActSize = 0;
	
    if ( DeviceIoControl(hDTV, IOCTL_DTV_MODULE_EVENT_HANDLE_QUERY, &dwTemp, sizeof(dwTemp), NULL, 0, &dwActSize, NULL) ) {
        NKDbgPrintfW( L"TVB:: GetModuleEventHandel = 0x%08x\r\n", dwTemp );
        hTemp = (HANDLE)dwTemp;
    } else
        NKDbgPrintfW( L"TVB:: Failed! GetModuleEventHandel\r\n" );

	return hTemp;
}

void GetRandomBYTE( PBYTE pByte )
{
	static DWORD dwLoop = 0;
	
	if( NULL != pByte ) {
		srand( GetTickCount() + dwLoop );	///random aseed
		*pByte = (BYTE)( rand()&0xFF );
	}
	
	dwLoop++;
}

DWORD FillBufferRandom( SBuffer *pSbuffer )
{
	DWORD dwLen = 0;
	DWORD dwLoop = 0;
	BYTE bTemp = 0;
	
	///get length
	GetRandomBYTE( &bTemp );
	dwLen = bTemp;		///0 ~ 0xFF
	if( 0 != dwLen%4 )
		dwLen -= (dwLen%4);
	if( 0 == dwLen )
		dwLen = 0xFF;
		
	dwLoop = dwLen;

	///data fill
	while( dwLoop > 0 ) {
		GetRandomBYTE( &bTemp );
		///pSbuffer->Add2Buffer( &bTemp, sizeof(bTemp) );
		pSbuffer->Add2Buffer( &bTemp, 1 );
		dwLoop--;
	}
	
	NKDbgPrintfW( L"TVB:: FillBufferRandom %dByte %dByte !!\r\n", dwLen, pSbuffer->GetBufferSize() );
	return pSbuffer->GetBufferSize();
}

DWORD DtvStatus( DWORD dwNewState )
{
	static DWORD g_dwDtvStatus = DVT_STATE_PWROFF;
	///DWORD dwState = 0;
	
	if( DVT_STATE_PWROFF == dwNewState ) {
		g_dwDtvStatus = DVT_STATE_PWROFF;
	} else
	if( DVT_STATE_PWRON == dwNewState ) {
		g_dwDtvStatus = DVT_STATE_PWRON;
	} else
	if( DVT_STATE_LOCKED == dwNewState ) {
		g_dwDtvStatus |= DVT_STATE_LOCKED;
	} else
	if( DVT_STATE_DISPON == dwNewState ) {
		g_dwDtvStatus |= DVT_STATE_DISPON;
	} else
	if( DVT_STATE_DISPOFF == dwNewState ) {
		g_dwDtvStatus &= ~DVT_STATE_DISPON;
	} else
	if( DVT_STATE_OSD1ON == dwNewState ) {
		g_dwDtvStatus |= DVT_STATE_OSD1ON;
	} else
	if( DVT_STATE_OSD1OFF == dwNewState ) {
		g_dwDtvStatus &= ~DVT_STATE_OSD1ON;
	} else
	if( DVT_STATE_RECOED == dwNewState ) {
		g_dwDtvStatus &= ~0x0000F000;
		g_dwDtvStatus |= DVT_STATE_RECOED;
	} else
	if( DVT_STATE_PLAYBACK == dwNewState ) {
		g_dwDtvStatus &= ~0x0000F000;
		g_dwDtvStatus |= DVT_STATE_PLAYBACK;
	} else
	if( DVT_STATE_STILLSHOT == dwNewState ) {
		g_dwDtvStatus &= ~0x0000F000;
		g_dwDtvStatus |= DVT_STATE_STILLSHOT;
	} else	
	if( DVT_STATE_STOPFUNC == dwNewState ) {
		g_dwDtvStatus &= ~0x0000F000;
	} else	
	if( 0 == dwNewState ) {
		;
	} else {
	}
	
	return g_dwDtvStatus;
}


BOOL _InitLog()
{
	BOOL bRet = TRUE;
	TCHAR				szTempString[256];
	
	wsprintf( szTempString, _T("Tv auto Pwr On/Off test\r\n") );
	///pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
	
	wsprintf( szTempString, _T("This test will auto test Tv module power on/off\r\n") );
	///pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );
	
	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	///pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	///wsprintf( szTempString, _T("\\SDMMC Card\\AutoReboot.log\0") );
	wsprintf( szTempString, _T("\\Storage Card\\TvPwrOnTest.log") );
	///pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}

BOOL InitLog()
{
	BOOL bRet;
	
	if( NULL == pLog )
		pLog = new SLogClass();
		
	if( NULL != pLog )
		bRet = _InitLog();
	else
		bRet = FALSE;

	///NKDbgPrintfW( TEXT("InitLog %s !\r\n"), bRet?L"Ok":L"Fail" );
	return bRet;
}


void TvPowerOnTest( DWORD dwLoop, BOOL bPwrControl )
{
	DWORD EnsembleIndex;
	BOOL bRet;
	BOOL bModuleCheck;
	
	if( InitLog() )
		pLog->EnableLog( TRUE );
	
	EnsembleIndex = SendMessage(GetDlgItem(g_hDlg, IDC_CHLIST), CB_GETCURSEL, 0, 0);
	
	///test case for power on CNS RF ready test 
	for( DWORD dwTestLoop = dwLoop; dwTestLoop > 0; dwTestLoop-- ) {
		
		if( pLog )
			pLog->WriteLog( _T("Test loop %d ------------------------------\r\n"), (dwLoop-dwTestLoop) );
		
		NKDbgPrintfW( L"TVB:: TvPowerOnTest Loop test %d/%d\r\n", (dwLoop-dwTestLoop), dwLoop );	
		
		if( bPwrControl ) {
			///power off
			TvNeptunePowerOnOff( FALSE, TRUE );
			///power on
			TvNeptunePowerOnOff( TRUE, TRUE );
		}

		///check module ready
		bModuleCheck = TvCheckModule();
		if( !bModuleCheck ) {
			///log
			if( pLog )
				pLog->WriteLog( _T("TvCheckModule1 fail @ %d\r\n"), (dwLoop-dwTestLoop) );
		} else {
			if( pLog )
				pLog->WriteLog( _T("TvCheckModule1 OK @ %d\r\n"), (dwLoop-dwTestLoop) );
		}
		///dump RF reg
		if( bModuleCheck )
			TvRFregisterDump( 0xFF, TRUE );
			
		///scan service
		if( bModuleCheck )
			bRet = TvScanService( EnsembleIndex );
		
		if( bModuleCheck ) {
			if( !bRet ) {
				if( pLog )
					pLog->WriteLog( _T("TvScanService1 fail @ %d\r\n"), (dwLoop-dwTestLoop) );
	/////////////////////////////////////////////////				
				///check module ready
				bModuleCheck = TvCheckModule();
				if( !bModuleCheck ) {
					///log
					if( pLog )
						pLog->WriteLog( _T("TvCheckModule2 fail @ %d\r\n"), (dwLoop-dwTestLoop) );
				} else {
					///log
					if( pLog )
						pLog->WriteLog( _T("TvCheckModule2 OK @ %d\r\n"), (dwLoop-dwTestLoop) );
				}
				
				///dump RF reg
				if( bModuleCheck )
					TvRFregisterDump( 0xFF, TRUE );
				
				///scan service
				if( bModuleCheck ) {
					if( !TvScanService( EnsembleIndex ) ) {
						if( pLog )
							pLog->WriteLog( _T("TvScanService2 fail @ %d\r\n"), (dwLoop-dwTestLoop) );
					} else {
						if( pLog )
							pLog->WriteLog( _T("TvScanService2 OK @ %d\r\n"), (dwLoop-dwTestLoop) );
					}
				}	
	///////////////////////////////////////////////////////////
				///check module ready
				bModuleCheck = TvCheckModule();
				if( !bModuleCheck ) {
					///log
					if( pLog )
						pLog->WriteLog( _T("TvCheckModule3 fail @ %d\r\n"), (dwLoop-dwTestLoop) );
				} else {
					///log
					if( pLog )
						pLog->WriteLog( _T("TvCheckModule3 OK @ %d\r\n"), (dwLoop-dwTestLoop) );
				}
				///dump RF reg
				if( bModuleCheck )
					TvRFregisterDump( 0xFF, TRUE );
				
				///scan service
				if( bModuleCheck ) {
					if( !TvScanService( EnsembleIndex ) ) {
						if( pLog )
							pLog->WriteLog( _T("TvScanService3 fail @ %d\r\n"), (dwLoop-dwTestLoop) );
					} else {
						if( pLog )
							pLog->WriteLog( _T("TvScanService3 OK @ %d\r\n"), (dwLoop-dwTestLoop) );
					}
				}
				///dump RF reg
				if( bModuleCheck )
					TvRFregisterDump( 0xFF, TRUE );
				
			} else {
				if( pLog )
					pLog->WriteLog( _T("TvScanService1 OK @ %d\r\n"), (dwLoop-dwTestLoop) );
					
				///dump RF reg
				TvRFregisterDump( 0xFF, TRUE );
			}
		}	///if( bModuleCheck )

    	wsprintf( szString, TEXT("TvPowerOnTest %d done"), (dwLoop-dwTestLoop) );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	}	///for
	
	if( pLog )
		pLog->SaveLog();
}

void TvCmdLoopBack()
{
	BOOL bRet = FALSE;
	DWORD dwRetSize = 0;
	DWORD dwLoop = 500;

    wsprintf( szString, TEXT("Cmd LoopBack test %ds...."), dwLoop );
	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

	if( dwProjectCode == 0x67333130 ) {	///PT310
		bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_IO_LOOPBACK_TEST, &dwLoop, sizeof(DWORD), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
		if( bRet ) {
			wsprintf( szString, TEXT("Cmd LoopBack test OK") );
		} else {
			wsprintf( szString, TEXT("Cmd LoopBack test Fail") );
		}
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

		return;
	}


	while( dwLoop > 0 ) {
		pFBuffer3->ClearBuffer();
		pFBuffer4->ClearBuffer();
    	
		bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_LOOPBACK_TEST, pFBuffer3->GetBufferPointer(), FillBufferRandom( pFBuffer3 ), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
    	
    	if( bRet ) {
    		pFBuffer4->SetBufDataLen( dwRetSize );
			if( dwRetSize != (pFBuffer3->GetBufferSize()+4) ) {
				NKDbgPrintfW( L"TVB:: TvCmdLoopBack wrong return size !!\r\n" );
			    wsprintf( szString, TEXT("Cmd LoopBack test Wrong Rsp size") );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				bRet = FALSE;
			} else {
				///compare data
#if 0				
				if( 0 != memcmp( pFBuffer3->GetBufferPointer(), pFBuffer4->GetBufferPointer()+4, pFBuffer3->GetBufferSize()) ) {
				///if( 0 != memcmp( pFBuffer3->GetBufferPointer(), pFBuffer4->GetBufferPointer()+8, pFBuffer3->GetBufferSize()) ) {
					NKDbgPrintfW( L"TVB:: TvCmdLoopBack wrong data !!\r\n" );
					bRet = FALSE;
				}
#else
				///copy buffer4 payload data to buffer2 with re-order
				pFBuffer2->ClearBuffer();
				DWORD dwLenght = 0;
				PBYTE pTar = NULL;
				PDWORD pSrc = NULL;
				
				dwLenght = pFBuffer3->GetBufferSize();
				///dwLenght = dwLenght - 4;
				dwLenght = dwLenght/4;
				pTar = pFBuffer2->GetBufferPointer();
				pSrc = (PDWORD)pFBuffer4->GetBufferPointer();
				pSrc++;
				
				while( dwLenght > 0 ) {
					*pTar = (BYTE)((*pSrc)>>24)&0xFF;
					pTar++;
					*pTar = (BYTE)((*pSrc)>>16)&0xFF;
					pTar++;
					*pTar = (BYTE)((*pSrc)>>8)&0xFF;
					pTar++;
					*pTar = (BYTE)((*pSrc)>>0)&0xFF;
					pTar++;
					
					pSrc++;
					dwLenght--;
				}
				
				pFBuffer2->SetBufDataLen( pFBuffer3->GetBufferSize() );
				
				if( 0 != memcmp( pFBuffer3->GetBufferPointer(), pFBuffer2->GetBufferPointer(), pFBuffer3->GetBufferSize()) ) {
					NKDbgPrintfW( L"TVB:: TvCmdLoopBack wrong data !!\r\n" );
				    wsprintf( szString, TEXT("Cmd LoopBack test Wrong Rsp Data") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
					bRet = FALSE;
				}
#endif				
			}
			if( !bRet ) {
				pFBuffer3->DumpBuffer();
				pFBuffer4->DumpBuffer();
				pFBuffer2->DumpBuffer();
			}	
		} else
			NKDbgPrintfW( L"TVB:: TvCmdLoopBack DeviceIoControl fail\r\n" );
		
		dwLoop--;
	}	
}


void TvCmdZoomTest( BOOL bOnPlay )
{
	BOOL bRet = FALSE;
	DWORD dwRetSize = 0;
	DWORD dwLoop = 500;

	while( dwLoop > 0 ) {
    	
    	TvChangeScreen( bOnPlay );
    	Sleep(5000);
		///NKDbgPrintfW( L"TVB:: TvCmdLoopBack DeviceIoControl fail\r\n" );
		
		dwLoop--;
	}	
}


BOOL RFregisterRead( BYTE bChipID, BYTE bIndex, PBYTE pData )
{
	BOOL bRet = FALSE;
	DWORD dwRetSize = 0;
	DTV_TUNER_REGISTER RegOp;
	
	RegOp.bRead = TRUE;
	RegOp.bChipID = bChipID;
	RegOp.dwIndex = bIndex;
	RegOp.dwRegRead = 0;
	RegOp.dwRegWrite = 0;
	RegOp.dwMask = 0;
	
	bRet = DeviceIoControl(hDTV, IOCTL_DTV_TUNER_REGISTER_ACCESS, &RegOp, sizeof(DTV_TUNER_REGISTER), NULL, 0, &dwRetSize, NULL);
	
	if( bRet ) {
		*pData = (BYTE)RegOp.dwRegRead;
	} else {
		NKDbgPrintfW( L"TVB:: RFregisterRead DeviceIoControl fail\r\n" );
		*pData = 0;
	}
	
	return bRet; 
}

BOOL RFregisterWrite( BYTE bChipID, BYTE bIndex, PBYTE pData )
{
	BOOL bRet = FALSE;
	DWORD dwRetSize = 0;
	DTV_TUNER_REGISTER RegOp;
	
	RegOp.bRead = FALSE;
	RegOp.bChipID = bChipID;
	RegOp.dwIndex = bIndex;
	RegOp.dwRegRead = 0;
	RegOp.dwRegWrite = *pData;
	RegOp.dwMask = 0;
	
	bRet = DeviceIoControl(hDTV, IOCTL_DTV_TUNER_REGISTER_ACCESS, &RegOp, sizeof(DTV_TUNER_REGISTER), NULL, 0, &dwRetSize, NULL);
	
	if( bRet ) {
		///*pData = (BYTE)RegOp.dwRegRead;
	} else {
		NKDbgPrintfW( L"TVB:: RFregisterWrite DeviceIoControl fail\r\n" );
		*pData = 0;
	}
	
	return bRet; 
}


static BYTE HDM1325Dump[] = 
{
	0x00,
	0x01,
	0x08,
	0x09,
	0x0A,
	0x10,
	0x30,
	0x33,
	0x32,
	0x36,
	0x37,
	0x38,
	0x41,
	0x42,
	0x46,
	0x47,
	0x48,
	0x4E,
	0x4F,
	0x50,
	0x51,
	0x52,
	0x53,
	0x54,
	0x56,
	0x5B,
	0x65,
	0xFF
};

DWORD GetHDM1325BER()
{
	DWORD dwRet = 0;
	BOOL bRet = FALSE;
	BOOL bBadSignal = FALSE;
	BYTE bData;
	BYTE bIndex;

	bIndex = 0x37;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	bData = bData | 0x70;	///bit[6:4] as 7
	
	bIndex = 0x37;
///		bData = 0x01;
	RFregisterWrite( 0xD0, bIndex, &bData );

	bIndex = 0x38;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	bData = bData | 0x80;	///bit[7] as 1
	
	bIndex = 0x38;
///		bData = 0x01;
	RFregisterWrite( 0xD0, bIndex, &bData );

	bIndex = 0x08;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	if( bData & 0x80 )
		dwRet = (bData & 0x7F)<<8;

	bIndex = 0x09;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	dwRet = dwRet | bData;

	bIndex = 0x0A;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	if( bData != 0 )
		bBadSignal = TRUE;

	bIndex = 0x38;
	bData = 0x00;
	RFregisterWrite( 0xD0, bIndex, &bData );

	if( bBadSignal ) {
		dwRet = 0;
		wsprintf( szString, TEXT("RF BER=Bad Signal") );
	} else {
		dwRet = dwRet/259840;
		wsprintf( szString, TEXT("RF BER=0x%08x"), dwRet );
	}

	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	
	return dwRet;
} 


DWORD GetHDM1325BERq()
{
	DWORD dwRet = 0;
	BOOL bRet = FALSE;
	BOOL bBadSignal = FALSE;
	BYTE bData;
	BYTE bIndex;

	
	bIndex = 0x38;
	bData = 0x0C;
	RFregisterWrite( 0xD0, bIndex, &bData );

	Sleep( 10 );

	bIndex = 0x38;
	bData = 0x84;
	RFregisterWrite( 0xD0, bIndex, &bData );

	bIndex = 0x37;
	///bData = 0x74;	///(1280*203*8)
	bData = 0x24;	///(40*203*8)
	RFregisterWrite( 0xD0, bIndex, &bData );

	bIndex = 0x38;
	bData = 0x04;
	RFregisterWrite( 0xD0, bIndex, &bData );

	do {
		Sleep( 200 );
		bIndex = 0x08;
		bRet = RFregisterRead( 0xD1, bIndex, &bData );
	} while( 0 == (bData&0x80) );

	bIndex = 0x38;
	bData = 0x84;
	RFregisterWrite( 0xD0, bIndex, &bData );

	bIndex = 0x08;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	dwRet = (bData & 0x7F)<<8;

	///wsprintf( szString, TEXT("RF reg[0x%02x]=0x%02x"), bIndex, bData );
	///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	
	bIndex = 0x09;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	dwRet = dwRet | bData;

	///wsprintf( szString, TEXT("RF reg[0x%02x]=0x%02x"), bIndex, bData );
	///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

	bIndex = 0x0A;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	if( bData != 0 )
		bBadSignal = TRUE;

	///wsprintf( szString, TEXT("RF reg[0x%02x]=0x%02x"), bIndex, bData );
	///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);


	if( bBadSignal ) {
		dwRet = 0;
		wsprintf( szString, TEXT("RF BER=Bad Signal 0x%02x"), bData );
	} else {
		dwRet = dwRet;
		///wsprintf( szString, TEXT("RF BER=%d/%d"), dwRet, (1280*203*8) );
		wsprintf( szString, TEXT("RF BER=%d/%d"), dwRet, (40*203*8) );
	}

	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	
	return dwRet;
} 


DWORD GetHDM1325AGC()
{
	DWORD dwRet = 0;
	BOOL bRet = FALSE;
	
	BYTE bData;
	BYTE bIndex;

	bIndex = 0x10;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	
	wsprintf( szString, TEXT("RF AGC=%d"), bData );
	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	
	dwRet = bData;
	return dwRet;
}

DWORD GetHDM1325CN()
{
	DWORD dwRet = 0;
	BOOL bRet = FALSE;
	double Y;
	
	BYTE bData;
	BYTE bIndex;

	bIndex = 0x1B;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	///wsprintf( szString, TEXT("RF reg[0x%02x]=0x%02x"), bIndex, bData );
	///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

	bIndex = 0x1D;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	///wsprintf( szString, TEXT("RF reg[0x%02x]=0x%02x"), bIndex, bData );
	///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

	bIndex = 0x1F;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );
	///wsprintf( szString, TEXT("RF reg[0x%02x]=0x%02x"), bIndex, bData );
	///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);


	bIndex = 0x15;
	bRet = RFregisterRead( 0xD1, bIndex, &bData );

	if( bData > 0 ) {
		Y = (-11.2)*log( bData ) + 66.0;	///PQSK
	///Y = (-13.9)*log( bData ) + 80.1;	///16QAM
	} else {
		Y = 0.0;
	}
	wsprintf( szString, TEXT("RF CN=%lf"), Y );
	SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	
	dwRet = bData;
	return dwRet;
}



void TvRFregisterDump( DWORD dwOption, BOOL bLog )
{
	DWORD dwRegNum = 0;
	BOOL bRet = FALSE;
	DWORD dwRetSize = 0;
	PDWORD pRsp = NULL;
///	DWORD dwReg[32];

	if( bLog && pLog )
		pLog->WriteLog( _T("Dump RF reg....\r\n") );
	
	if( dwProjectCode == 0x67333130 ) {	///PT310
		BYTE bData;
		BYTE bIndex;
		DWORD dwLoop = 0;

		bIndex = HDM1325Dump[dwLoop];
		dwLoop++;
		do {

			bRet = RFregisterRead( 0xD1, bIndex, &bData );
			wsprintf( szString, TEXT("RF reg[0x%02x]=0x%02x"), bIndex, bData );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);	
			
			bIndex = HDM1325Dump[dwLoop];
			dwLoop++;
		}while( 0xFF != bIndex );		

		GetHDM1325CN();		
		GetHDM1325AGC();
		GetHDM1325BERq();
		
		MoveList2Last();
		
///		bIndex = 0x33;
///		bData = 0x01;
///		RFregisterWrite( 0xD0, bIndex, &bData );
///		
///		bIndex = 0x33;
///		bRet = RFregisterRead( 0xD1, bIndex, &bData );
///		wsprintf( szString, TEXT("RF reg[0x%02x]=0x%02X"), bIndex, bData );
///		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);	

	} else {
			
		pFBuffer4->ClearBuffer();
		if( 0xFF == dwOption ) {	///dump all
			
			for( dwRegNum = 0; dwRegNum < 32; dwRegNum++ ) {
				bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_READ_TUNER_REG, &dwRegNum, 4, pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
				if( bRet ) {
					pRsp = (PDWORD)pFBuffer4->GetBufferPointer();
					wsprintf( szString, TEXT("RF reg[%02d]=0x%08X"), dwRegNum, *pRsp );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);	
				} else {
					wsprintf( szString, TEXT("RF reg[%02d] cmd fail"), dwRegNum );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);	
				}
				if( bLog && pLog )
					pLog->WriteLog( _T("RF reg[%02d]=0x%08X %s\r\n"), dwRegNum, *pRsp, bRet?L"OK":L"fail" );
				
///				dwReg[dwRegNum] = *pRsp;
			}
			
			MoveList2Last();
		
		} else {
		}
	}
}


void TvRFstatusDump( DWORD dwOption, BOOL bLog )
{
	DWORD dwRegNum = 0;
	BOOL bRet = FALSE;
	DWORD dwRetSize = 0;


///	if( bLog && pLog )
///		pLog->WriteLog( _T("RF status....\r\n") );
	
	if( dwProjectCode == 0x67333130 ) {	///PT310
		///BYTE bData;
		///BYTE bIndex;
		///DWORD dwLoop = 0;

		GetHDM1325CN();		
		GetHDM1325AGC();
		GetHDM1325BERq();
		
		MoveList2Last();
		
	} else {
		;	
	}
}


void MoveList2Last()
{
	///move the screen to the last message		
	DWORD dwIndex = SendDlgItemMessage(g_hDlg, IDC_MSG_LIST, LB_GETCOUNT, 0, 0);							
	SendDlgItemMessage(g_hDlg, IDC_MSG_LIST, LB_SETTOPINDEX, (dwIndex-1), 0);	
}

BOOL DoRecord( DWORD dwOption )
{
	BOOL bRet = TRUE;
	HANDLE hTemp = NULL;
	HANDLE hTempf = NULL;		

	switch( dwOption )
	{
		case 1:
			///start
			hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_RECORDSTART_EVENT );
			hTempf = CreateEvent(NULL, FALSE, FALSE, NAMED_FILESTART_EVENT );			
			break;
		case 2:
			///stop
			hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_RECORDSTOP_EVENT );
			hTempf = CreateEvent(NULL, FALSE, FALSE, NAMED_FILESTOP_EVENT );
			break;
		case 3:
			///recording
			hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_RECORDING_EVENT );
			break;

		default:
			break;	
	}
	
	if( hTemp != NULL )
		SetEvent( hTemp );

	if( hTempf != NULL )
		SetEvent( hTempf );
	
	return bRet;
}

BOOL DoStillShot( DWORD dwOption )
{
	BOOL bRet = TRUE;
	HANDLE hTemp = NULL;
	HANDLE hTempf = NULL;		

	switch( dwOption )
	{
		case 1:
			///start
			hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_STILLSHOTSTART_EVENT );
///			hTempf = CreateEvent(NULL, FALSE, FALSE, NAMED_FILESTART_EVENT );			
			break;
		case 2:
			///stop
			hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_STILLSHOTSTOP_EVENT );
///			hTempf = CreateEvent(NULL, FALSE, FALSE, NAMED_FILESTOP_EVENT );
			break;
		case 3:
			///stillshoting
			hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_STILLSHOTING_EVENT );
			break;

		default:
			break;	
	}
	
	if( hTemp != NULL )
		SetEvent( hTemp );

	if( hTempf != NULL )
		SetEvent( hTempf );
	
	return bRet;
}

DWORD DTVmoduleFWversion()
{
	DWORD dwRet = 0;
	BOOL bRet;

	if( !bCNSPwrOn ) {
       	wsprintf( szString, TEXT("Not powered!!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		bRet = FALSE;
		return bRet;
	}

	bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_FW_VERSION, &dwRet, sizeof(dwRet), NULL, 0, &dwActSize, NULL);
	
	if( bRet )
		;
	else
		dwRet = 0;
		
	NKDbgPrintfW( L"TVB:: DTVmoduleFWversion 0x%08x %s\r\n", dwRet, bRet?L"Ok":L"Fail" );	
		
	
	return dwRet;
}

#if 0
BOOL ISDBmoduleStillshot()
{
	BOOL bRet = FALSE;
	DWORD dwRetSize;
	DTV_STILLSHOT DtvStillshot;
	
	NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot start\r\n" );
	
	DtvStillshot.dwFormat = DTV_STILLSHOT_FORMAT_JPG;
	DtvStillshot.dwImageSize = 0;
	bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_VIDEO_STILLSHOT, &DtvStillshot, sizeof(DtvStillshot), NULL, 0, &dwRetSize, NULL);
	
	if( bRet ) {
		HANDLE hFile = INVALID_HANDLE_VALUE;
		DWORD dwWritten = 0;
		
		if( hFile == INVALID_HANDLE_VALUE ) {
	    	hFile = CreateFile( TV_STILLSHOT_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	    	NKDbgPrintfW( L"TVB:: SShot CreateFile ok 0x%08x\r\n", hFile );
	    } else {
	    	NKDbgPrintfW( L"TVB:: SShot file already open\r\n" );
	    }
		
		if( hFile == INVALID_HANDLE_VALUE ) {
			if( DtvStillshot.dwImageSize < (512*1024) ) {
				do {
  					bRet = WriteFile( hFile, &(DtvStillshot.bStillShotData[0]), DtvStillshot.dwImageSize, &dwWritten, NULL);
  					if( !bRet )
  						NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot WriteFile error! write %d=%d Err=0x%08x\r\n", DtvStillshot.dwImageSize, dwWritten, GetLastError() );
				}while(0);
			} else {
				NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot WriteFile error! write %d=%d Err=0x%08x\r\n", DtvStillshot.dwImageSize, dwWritten, GetLastError() );
			}
	
		} else {
			wsprintf( szString, TEXT("SShot open file fail") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);		
		}
		
		CloseHandle( hFile );
		
		wsprintf( szString, TEXT("SShot=%dbyte"), DtvStillshot.dwImageSize );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);	
		
		NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot done=%dbyte \r\n", DtvStillshot.dwImageSize );
	} else {
		wsprintf( szString, TEXT("SShot fail!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);	

		NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot fail=%dbyte \r\n", DtvStillshot.dwImageSize );
	}

	return bRet;
}
#else
BOOL ISDBmoduleStillshot()
{
	BOOL bRet = FALSE;
	DWORD dwRetSize;
	PDTV_STILLSHOT pDtvStillshot = NULL;

	if( !bCNSPwrOn ) {
       	wsprintf( szString, TEXT("Not powered!!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		return bRet;
	}
	
	NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot start\r\n" );
	
	
	pDtvStillshot = (PDTV_STILLSHOT)malloc( sizeof(DTV_STILLSHOT) );
	
	
	if( NULL != pDtvStillshot ) {
		pDtvStillshot->dwFormat = DTV_STILLSHOT_FORMAT_JPG;
		pDtvStillshot->dwImageSize = 0;
		bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_VIDEO_STILLSHOT, pDtvStillshot, sizeof(DTV_STILLSHOT), NULL, 0, &dwRetSize, NULL);
	} else {
		bRet = FALSE;
	}
	
	if( bRet ) {
		HANDLE hFile = INVALID_HANDLE_VALUE;
		DWORD dwWritten = 0;
		
		if( hFile == INVALID_HANDLE_VALUE ) {
	    	hFile = CreateFile( TV_STILLSHOT_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	    	NKDbgPrintfW( L"TVB:: SShot CreateFile ok 0x%08x\r\n", hFile );
	    } else {
	    	NKDbgPrintfW( L"TVB:: SShot file already open\r\n" );
	    }
		
		if( hFile != INVALID_HANDLE_VALUE ) {
			if( pDtvStillshot->dwImageSize < (512*1024) ) {
				do {
  					bRet = WriteFile( hFile, &(pDtvStillshot->bStillShotData[0]), pDtvStillshot->dwImageSize, &dwWritten, NULL);
  					if( !bRet )
  						NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot WriteFile error! write %d=%d Err=0x%08x\r\n", pDtvStillshot->dwImageSize, dwWritten, GetLastError() );
				}while(0);
				NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot WriteFile %d=%d\r\n", pDtvStillshot->dwImageSize, dwWritten );
			} else {
				NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot WriteFile error! write %d=%d Err=0x%08x\r\n", pDtvStillshot->dwImageSize, dwWritten, GetLastError() );
			}
	
		} else {
			wsprintf( szString, TEXT("SShot open file fail") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);		
		}
		
		CloseHandle( hFile );
		
		wsprintf( szString, TEXT("SShot=%dbyte"), pDtvStillshot->dwImageSize );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);	
		
		NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot done=%dbyte \r\n", pDtvStillshot->dwImageSize );
	} else {
		wsprintf( szString, TEXT("SShot fail!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);	

		NKDbgPrintfW( L"TVB:: ISDBmoduleStillshot fail=%dbyte \r\n", pDtvStillshot->dwImageSize );
	}

	free( pDtvStillshot );
	return bRet;
}
#endif

BOOL TVISDBchangeAVparameter( PDWORD pdwAVparameter, PDWORD pdwAVparaSwitch )
{
	BOOL bRet = FALSE;

	if( !bCNSPwrOn ) {
       	wsprintf( szString, TEXT("Not powered!!") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);       	
		return bRet;
	}

	switch( *pdwAVparameter ) {
		case 1:	///audio ES change
		{
			if( ChServInfo.AudioESnums.dwTotalNum > 0 ) {
				DWORD dwAudioESnum;
				dwAudioESnum = *pdwAVparaSwitch;
				bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_SET_AUDIO_ES_NUM, &dwAudioESnum, sizeof(dwAudioESnum), NULL, 0, &dwActSize, NULL);
				wsprintf( szString, TEXT("Set AudioESnum=%d %s"), dwAudioESnum, bRet?L"Ok":L"fail" );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				(*pdwAVparaSwitch)++;
				if( *pdwAVparaSwitch >= ChServInfo.AudioESnums.dwTotalNum )
					*pdwAVparaSwitch = 0;
			} else {
		    	wsprintf( szString, TEXT("Err!AudioESnums=%d"), ChServInfo.AudioESnums.dwTotalNum );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			}
		}
		break;
		case 2:	///audio mode change
		{
			if( ChServInfo.AudioModes.dwAllModes > 0 ) {
				DWORD dwAudioMode;
				dwAudioMode = *pdwAVparaSwitch;
				if( 0 == *pdwAVparaSwitch ) {
					dwAudioMode = ISDB_ES_AUDIO_MODE_DUAL_MONO;
					wsprintf( szString, TEXT("Set DUAL_MONO") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				} else
				if( 1 == *pdwAVparaSwitch ) {
					dwAudioMode = ISDB_ES_AUDIO_MODE_MONO;
					wsprintf( szString, TEXT("Set MONO") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				} else
				if( 2 == *pdwAVparaSwitch ) {
					dwAudioMode = ISDB_ES_AUDIO_MODE_STEREO;
					wsprintf( szString, TEXT("Set STEREO") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				} else {
					dwAudioMode = ISDB_ES_AUDIO_MODE_STEREO;
					wsprintf( szString, TEXT("Set STEREO") );
					SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				}
				bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_SET_AUDIO_MODE, &dwAudioMode, sizeof(dwAudioMode), NULL, 0, &dwActSize, NULL);
				wsprintf( szString, TEXT("Set AudioMode=%d %s"), dwAudioMode, bRet?L"Ok":L"fail" );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				(*pdwAVparaSwitch)++;
				if( *pdwAVparaSwitch >= ChServInfo.AudioModes.dwAllModes )
					*pdwAVparaSwitch = 0;
			} else {
		    	wsprintf( szString, TEXT("Err!AudioModes=%d"), ChServInfo.AudioModes.dwAllModes );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			}
		}
		break;
		case 3:	///video ES change
		{
			if( ChServInfo.VideoESnums.dwTotalNum > 0 ) {
				DWORD dwVideoESnum;
				dwVideoESnum = *pdwAVparaSwitch;
				bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_SET_VIDEO_ES_NUM, &dwVideoESnum, sizeof(dwVideoESnum), NULL, 0, &dwActSize, NULL);
				wsprintf( szString, TEXT("Set VideoESnum=%d %s"), dwVideoESnum, bRet?L"Ok":L"fail" );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				(*pdwAVparaSwitch)++;
				if( *pdwAVparaSwitch >= ChServInfo.VideoESnums.dwTotalNum )
					*pdwAVparaSwitch = 0;
			} else {
		    	wsprintf( szString, TEXT("Err!VideoESnums=%d"), ChServInfo.VideoESnums.dwTotalNum );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			}
		}
		break;
		case 4:	///Colosed Caption language
		{
			if( ChServInfo.LanguageNums.dwTotalLanguageNum > 0 ) {
				DWORD dwCCnum;
				dwCCnum = *pdwAVparaSwitch;
				bRet = DeviceIoControl(hDTV, IOCTL_DTV_ISDB_SET_CC_LANGUAGE_NUM, &dwCCnum, sizeof(dwCCnum), NULL, 0, &dwActSize, NULL);
				wsprintf( szString, TEXT("Set CCnum=%d %s"), dwCCnum, bRet?L"Ok":L"fail" );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
				(*pdwAVparaSwitch)++;
				if( *pdwAVparaSwitch >= ChServInfo.LanguageNums.dwTotalLanguageNum )
					*pdwAVparaSwitch = 0;
			} else {
		    	wsprintf( szString, TEXT("Err!CCnum=%d"), ChServInfo.LanguageNums.dwTotalLanguageNum );
				SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			}
		}
		break;
		
		default:
		{
			;
		}
		break;
	}///switch
	return bRet;
}


BOOL DoPlayBack( DWORD dwOption )
{
	BOOL bRet = TRUE;
	HANDLE hTemp = NULL;

	switch( dwOption )
	{
		case 1:
			///start
			hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_PLAYBACKSTART_EVENT );

			break;
		case 2:
			///stop
			hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_PLAYBACKSTOP_EVENT );

			break;
		case 3:
			///pause/resume
			hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_PLAYBACKPAUSE_EVENT );
			break;

		default:
			break;	
	}
	
	if( hTemp != NULL )
		SetEvent( hTemp );

	return bRet;
}

void TvStillShot(DWORD dwCmd)
{
	BOOL bRet = FALSE;
	///DWORD dwRetSize;


    static HANDLE hStillShotThread = NULL;
///    static HANDLE hRecFileThread = NULL;
    
    
    switch (dwCmd) {
        case 5:
        	if( NULL == hStillShotThread )
        		hStillShotThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)TvStillShotThread, NULL, 0, NULL);
///        	if( NULL == hStillshotThread )
///        		hRecFileThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)FileRecordThread, NULL, 0, NULL);

        	///start StillShot       	
        	if( NULL != hStillShotThread )
        		DoStillShot(1);
        	
        	break;	
        case 6:
        	///stop StillShot
        	if( NULL != hStillShotThread )
        		DoStillShot(2);

		case 7:
			///
			if( dwProjectCode == 0x67333130 ) {	///PT310
				ISDBmoduleStillshot();
			}
			
        	break;	
        default:
            return;
    }
}

void TvDataService(DWORD dwCmd)
{
	BOOL bRet = FALSE;
	DWORD dwRetSize;

#if 0	///old 
	pFBuffer4->ClearBuffer();
	///get data from driver to buffer4
   	bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_SLS_CHECK, NULL, 0, pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   				
   	if( bRet ) {
   		NKDbgPrintfW( L"TVB:: TvDataService SLS %d \r\n", dwRetSize );
   		pFBuffer4->SetBufDataLen( dwRetSize );
   		pFBuffer4->DumpBuffer();
	}
	else
		NKDbgPrintfW( L"TVB:: TvDataService DeviceIoControl SLS %d fail\r\n", dwRetSize );

	pFBuffer4->ClearBuffer();
	///get data from driver to buffer4
   	bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_DLS_CHECK, NULL, 0, pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   				
   	if( bRet ) {
   		NKDbgPrintfW( L"TVB:: TvDataService DLS %d \r\n", dwRetSize );
   		pFBuffer4->SetBufDataLen( dwRetSize );
   		pFBuffer4->DumpBuffer();
	}
	else
		NKDbgPrintfW( L"TVB:: TvDataService DeviceIoControl DLS %d fail\r\n", dwRetSize );
#else
	DataServInfo *pDServInfo;

	pDServInfo = (PDataServInfo)LocalAlloc(LPTR, sizeof(DataServInfo));
	///pDServInfo = (PDataServInfo)LocalAlloc(LPTR, 655360);

	pDServInfo->bPackHeader[0] = 0x10;

	pDServInfo->dwPackSize = sizeof(DataServInfo);

   	bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_GET_DATASERVICE, pDServInfo, sizeof(DataServInfo), NULL, 0, &dwRetSize, NULL);
   	
   	NKDbgPrintfW( L"TVB:: TvDataService bPackHeader0 = 0x%02x \r\n", pDServInfo->bPackHeader[0] );
   	NKDbgPrintfW( L"TVB:: TvDataService bPackHeader3 = 0x%02x \r\n", pDServInfo->bPackHeader[3] );   	
   	NKDbgPrintfW( L"TVB:: TvDataService dwDLSSize = %d \r\n", pDServInfo->dwDLSSize );
   	NKDbgPrintfW( L"TVB:: TvDataService dwSLSSize = %d \r\n", pDServInfo->dwSLSSize );
   	NKDbgPrintfW( L"TVB:: TvDataService SLS X Size = %d \r\n", pDServInfo->dwSLSxSize );
   	NKDbgPrintfW( L"TVB:: TvDataService SLS Y Size = %d \r\n", pDServInfo->dwSLSySize );
   	
   	///debug
   	pFBuffer4->ClearBuffer();
   	pFBuffer4->Copy2Buffer( (PBYTE)pDServInfo, 4096 );
	pFBuffer4->SetBufDataLen( 4096 );
   	pFBuffer4->DumpBuffer();
   	
   	if( pDServInfo->dwSLSSize > 0 ) {
   		///save SLS data to file
   		HANDLE hSLSFile = INVALID_HANDLE_VALUE;
   		DWORD dwWritten;
   		
   		hSLSFile = CreateFile( TV_SLS_FILENAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
   		
   		if( hSLSFile != INVALID_HANDLE_VALUE )
   		{
   			WINBMPFILEHEADER BMPFileHeader;
   			WINBMPINFOHEADER BMPInfoHeader;
   			INT32 iCount = 0;
   			
   			memset( &BMPFileHeader, 0x00, sizeof(BMPFileHeader) );
   			memset( &BMPInfoHeader, 0x00, sizeof(BMPInfoHeader) );
   			
   			///prepare BMP file header
   			///BMPheader.Signature = 0x4D42;									///must be 0x4D42 hex
   			BMPFileHeader.Signature = 0x4D424D42;								///must be 0x4D424D42 hex
   			///BMPheader.Signature0 = 0x42;										///must be 0x4D42 hex
   			///BMPheader.Signature1 = 0x4D;										///must be 0x4D42 hex
   			BMPFileHeader.FileSize = (sizeof(BMPFileHeader)-2) + sizeof(BMPInfoHeader) + pDServInfo->dwSLSSize;		///headers size + image data size
			BMPFileHeader.reserved0 = 0;										///must be zero
			BMPFileHeader.reserved1 = 0;										///must be zero
			BMPFileHeader.ImageDataOffset = (sizeof(BMPFileHeader)-2) + sizeof(BMPInfoHeader);	///usually 0x36 (54 bytes)
			
			///prepare BMP file info header
			BMPInfoHeader.BITMAPINFOHEADERSize = sizeof(BMPInfoHeader);			///must be 0x28 (40 bytes)
			BMPInfoHeader.ImageWidth = pDServInfo->dwSLSxSize;					///x resolution
			BMPInfoHeader.ImageHeight = pDServInfo->dwSLSySize;					///y resolution
			BMPInfoHeader.PlanesNumber = 1;										///must be 1
			BMPInfoHeader.BitsPerPixel = 16;									///16 for our panel,must be (1, 4, 8, or 24)
			BMPInfoHeader.CompressType = 0;										///compression type (0=none, 1=RLE-8, 2=RLE-4)
			BMPInfoHeader.ImageDataSize = pDServInfo->dwSLSSize;				///size of image data in bytes (including padding)
			BMPInfoHeader.HpixelPerMeter = 0;									///0, horizontal resolution in pixels per meter (unreliable)
			BMPInfoHeader.VpixelPerMeter = 0;									///0, vertical resolution in pixels per meter (unreliable)	
			BMPInfoHeader.ColorNumber = 0;										///0, number of colors in image, or zero
			BMPInfoHeader.VIPColorNumber = 0;									///0, number of important colors in image, or zero
   			
   			///write file header
   			PBYTE pHeader = (PBYTE)&BMPFileHeader;
   			pHeader = pHeader+2;
   			bRet = WriteFile( hSLSFile, pHeader, sizeof(BMPFileHeader)-2, &dwWritten, NULL);

			///write info header
   			pHeader = (PBYTE)&BMPInfoHeader;
   			bRet = WriteFile( hSLSFile, pHeader, sizeof(BMPInfoHeader), &dwWritten, NULL);
   			
   			///write image data
   			///bRet = WriteFile( hSLSFile, pDServInfo->bSLSBuffer, pDServInfo->dwSLSSize, &dwWritten, NULL);
   			
			SBuffer *pWriteBuffer;
			pWriteBuffer = new SBuffer( (BMPFileHeader.FileSize) );	///create buffer for write
   			
   			///copy to buffer
   			iCount = (pDServInfo->dwSLSSize / (pDServInfo->dwSLSxSize*2) ) - 1;
   			while( iCount >= 0 ) {
   				pHeader = pDServInfo->bSLSBuffer + (iCount*pDServInfo->dwSLSxSize*2);
   				///bRet = WriteFile( hSLSFile, pHeader, pDServInfo->dwSLSxSize*2, &dwWritten, NULL);
   				bRet = pWriteBuffer->Add2Buffer( pHeader, pDServInfo->dwSLSxSize*2 );
   				iCount--;
   			}

   			///write image data to file
   			bRet = WriteFile( hSLSFile, pWriteBuffer->GetBufferPointer(), pWriteBuffer->GetBufferSize(), &dwWritten, NULL);

   			delete pWriteBuffer;
   			
   			if( !bRet )
   				NKDbgPrintfW( L"TVB:: TvDataService SLS WriteFile error !!\r\n" );
   			else
   				NKDbgPrintfW( L"TVB:: TvDataService SLS WriteFile %d bytes done !!\r\n", dwWritten );
   		}		
   		
   		CloseHandle( hSLSFile );
   	}
   	
   	LocalFree(pDServInfo);
#endif

}


DWORD BatteryLevelMap( BYTE bLiftPercent )
{
	DWORD dwBattLevel = 0;
	
	if( bLiftPercent < 10 ) {
		///level 0
		dwBattLevel = 0;
	} else
	if( bLiftPercent < 30 ) {
		///level 1
		dwBattLevel = 1;
	} else
	if( bLiftPercent < 50 ) {
		///level 2
		dwBattLevel = 2;
	} else
	if( bLiftPercent < 70 ) {
		///level 3
		dwBattLevel = 3;
	} else
	if( bLiftPercent < 90 ) {
		///level 4
		dwBattLevel = 4;
	} else
	if( bLiftPercent < 120 ) {
		///level 5
		dwBattLevel = 5;
	} else {
		///level unknow
		dwBattLevel = 0;
	}

	return dwBattLevel;
}

BOOL TvSendBatteryInfo( BOOL bForceUpdate )
{
	static PFN_GetSystemPowerStatusEx2 gpfnGetSystemPowerStatusEx2 = NULL;
	static BYTE bLastBattery = 0;
	static DWORD dwModBattery = 0;
	DWORD dwIOCTLcode = 0;
	DWORD dwBattery = 0;
	DWORD dwRetSize;
	BOOL bRet;
	SYSTEM_POWER_STATUS_EX2 sysPwrStatus;

	if( NULL == gpfnGetSystemPowerStatusEx2 )
	{
		HMODULE hmCoreDll = LoadLibrary( L"coredll.dll" );
		if( hmCoreDll != NULL ) {
			gpfnGetSystemPowerStatusEx2 = (PFN_GetSystemPowerStatusEx2)GetProcAddress( hmCoreDll, L"GetSystemPowerStatusEx2" );
		}
		
		if ( NULL == gpfnGetSystemPowerStatusEx2 ) {
			bRet = FALSE;
			NKDbgPrintfW( L"TVB:: TvSendBatteryInfo gpfnGetSystemPowerStatusEx2 fail \r\n" );
			return bRet;
		} else
			bRet = TRUE;	
	}	

	if( dwModBattery != g_dwModBattery )
		bForceUpdate = TRUE;

	if( 0x67333130 == dwProjectCode )	///PT310 for ISBD-T
		dwIOCTLcode = IOCTL_DTV_SYS_BATTERY_STATUS;
	else
		dwIOCTLcode = IOCTL_CNSCMD_BATTERY_STATUS;

	///if( 0 == gpfnGetSystemPowerStatusEx2( &sysPwrStatus, sizeof(sysPwrStatus), FALSE ) ) {
	if( 0 == gpfnGetSystemPowerStatusEx2( &sysPwrStatus, sizeof(sysPwrStatus), TRUE ) ) {
		bRet = FALSE;
	} else {
		if( sysPwrStatus.BatteryFlag == BATTERY_FLAG_CHARGING )
			wsprintf( szString, TEXT("BATTERY_FLAG_CHARGING") );
		else
		if( sysPwrStatus.BatteryFlag == BATTERY_FLAG_HIGH )
			wsprintf( szString, TEXT("BATTERY_FLAG_HIGH") );
		else
		if( sysPwrStatus.BatteryFlag == BATTERY_FLAG_LOW )
			wsprintf( szString, TEXT("BATTERY_FLAG_LOW") );
		else
		if( sysPwrStatus.BatteryFlag == BATTERY_FLAG_CRITICAL )
			wsprintf( szString, TEXT("BATTERY_FLAG_CRITICAL") );
		else
		if( sysPwrStatus.BatteryFlag == BATTERY_FLAG_UNKNOWN )
			wsprintf( szString, TEXT("BATTERY_FLAG_UNKNOWN") );
		else
		if( sysPwrStatus.BatteryFlag == BATTERY_FLAG_NO_BATTERY )
			wsprintf( szString, TEXT("BATTERY_FLAG_NO_BATTERY") );
				
		///SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);

		if( sysPwrStatus.BatteryFlag == BATTERY_FLAG_CHARGING  ) {
			///update to module
			dwBattery = 0xe1;
		   	bRet = DeviceIoControl(hDTV, dwIOCTLcode, &dwBattery, sizeof(dwBattery), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
		   	if( bRet ) {
		   		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
		   		NKDbgPrintfW( L"TVB:: TvSendBatteryInfo charging %d %d \r\n", dwBattery, dwRetSize );
			} else
				NKDbgPrintfW( L"TVB:: TvSendBatteryInfo DeviceIoControl %d %d fail\r\n", dwBattery, dwRetSize );
				
			bLastBattery = sysPwrStatus.BatteryLifePercent;
		} else
		if( sysPwrStatus.BatteryLifePercent != bLastBattery || bForceUpdate ) {
			///update to module
			dwBattery = BatteryLevelMap( sysPwrStatus.BatteryLifePercent );
			
			bRet = DeviceIoControl(hDTV, dwIOCTLcode, &dwBattery, sizeof(dwBattery), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
		   	
		   	if( bRet ) {
		   		NKDbgPrintfW( L"TVB:: TvSendBatteryInfo %d %d \r\n", dwBattery, dwRetSize );
///		   		pFBuffer4->SetBufDataLen( dwRetSize );
///		   		pFBuffer4->DumpBuffer();
		   		dwModBattery = dwBattery;
		   		g_dwModBattery = dwBattery;
			}
			else
				NKDbgPrintfW( L"TVB:: TvSendBatteryInfo DeviceIoControl %d %d fail\r\n", dwBattery, dwRetSize );
				
			bLastBattery = sysPwrStatus.BatteryLifePercent;
		}
	}
	
	return bRet;
}


BOOL TvSendSignalInfo( BOOL bForceUpdate )
{

	static DWORD dwCurrSignalLevel = 0xFF;
	DWORD dwRetSize;
	DWORD dwSignal = 0;
	BOOL bRet;
	ISDB_T_SIGNALSTATUS SignalStatus;
	
	bRet = GetSignalStatus( &SignalStatus );
	
	if( bRet ) {
		if( SignalStatus.dwSignalLevel == dwCurrSignalLevel && !bForceUpdate ) {
			///same signal level, need no update
			NKDbgPrintfW( L"TVB:: TvSendSignalInfo same level need no update!!\r\n" );
		} else {
			dwCurrSignalLevel = SignalStatus.dwSignalLevel;
			dwSignal = dwCurrSignalLevel;
			bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_OSD_SIGNAL_LEVEL, &dwSignal, sizeof(dwSignal), NULL, 0, &dwRetSize, NULL);
			if( bRet )
				NKDbgPrintfW( L"TVB:: TvSendSignalInfo update %d !!\r\n", dwSignal );
			else
				NKDbgPrintfW( L"TVB:: TvSendSignalInfo DeviceIoControl Fail %d!!\r\n",dwSignal );
		}
	} else {
		NKDbgPrintfW( L"TVB:: TvSendSignalInfo GetSignalStatus fail\r\n" );
	}
	
	return bRet;
}


void TvSetBattery()
{
	static DWORD dwCurBattery = 0;
	DWORD dwBattery = 0;
	DWORD dwRetSize = 0;
	BOOL bRet;

	dwBattery = dwCurBattery;
	if( dwBattery == 6 )
		dwBattery = 0xe1;
	dwCurBattery++;
#ifdef OVERLAY_MOSQUITO
	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		bRet = DeviceIoControl(hDTV, IOCTL_DTV_SYS_BATTERY_STATUS, &dwBattery, sizeof(dwBattery), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
	} else {	
   		bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_BATTERY_STATUS, &dwBattery, sizeof(dwBattery), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   	}
#else
	#ifdef OVERLAY_DRAWTEXT
	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		bRet = DeviceIoControl(hDTV, IOCTL_DTV_SYS_BATTERY_STATUS, &dwBattery, sizeof(dwBattery), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
	} else {	
   		bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_BATTERY_STATUS, &dwBattery, sizeof(dwBattery), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   	}
	#endif
	if( bVideoDisplay ) {
	#ifndef OVERLAY_DRAWTEXT
		///DDOverlayBitmap( hInst, g_hDlg, 32, 20, 220, 10, 0, IDB_BATT3, FALSE );
		///DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 0"), IDB_BATT0, 32, 20, 210, 0, FALSE );
		DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 0"), IDB_BATT0, 30, 15, 210, 0, IDB_SIG0, 53, 15, 150, 0, FALSE );
		switch( dwBattery ) {
			case 0:
				///DDOverlayBitmap( hInst, g_hDlg, 32, 20, 220, 10, 0, IDB_BATT0, TRUE );
				///DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 0"), IDB_BATT0, 32, 20, 210, 0, TRUE );
				DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 0"), IDB_BATT0, 30, 15, 210, 0, IDB_SIG0, 53, 15, 150, 0, TRUE );
				break;
			case 1:
				///DDOverlayBitmap( hInst, g_hDlg, 32, 20, 220, 10, 0, IDB_BATT1, TRUE );
				///DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 1"), IDB_BATT1, 32, 20, 210, 0, TRUE );
				DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 1"), IDB_BATT1, 30, 15, 210, 0, IDB_SIG1, 53, 15, 150, 0, TRUE );
				break;
			case 2:
				///DDOverlayBitmap( hInst, g_hDlg, 32, 20, 220, 10, 0, IDB_BATT2, TRUE );
				///DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 2"), IDB_BATT2, 32, 20, 210, 0, TRUE );
				DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 2"), IDB_BATT2, 30, 15, 210, 0, IDB_SIG2, 53, 15, 150, 0, TRUE );
				break;
			case 3:
				///DDOverlayBitmap( hInst, g_hDlg, 32, 20, 220, 10, 0, IDB_BATT3, TRUE );
				///DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 3"), IDB_BATT3, 32, 20, 210, 0, TRUE );
				DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 3"), IDB_BATT3, 30, 15, 210, 0, IDB_SIG3, 53, 15, 150, 0, TRUE );
				break;
			case 4:
				///DDOverlayBitmap( hInst, g_hDlg, 32, 20, 220, 10, 0, IDB_BATT4, TRUE );
				///DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 4"), IDB_BATT4, 32, 20, 210, 0, TRUE );
				DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 4"), IDB_BATT4, 30, 15, 210, 0, IDB_SIG4, 53, 15, 150, 0, TRUE );
				break;
			case 0xe1:
				///DDOverlayBitmap( hInst, g_hDlg, 32, 20, 220, 10, 0, IDB_BATTAC, TRUE );
				///DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery AC"), IDB_BATTAC, 32, 20, 210, 0, TRUE );
				DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery AC"), IDB_BATTAC, 30, 15, 210, 0, IDB_SIG0, 53, 15, 150, 0, TRUE );
				break;
			default:
				///DDOverlayBitmap( hInst, g_hDlg, 32, 20, 220, 10, 0, IDB_BATT0, FALSE );
				///DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 0"), IDB_BATT0, 32, 20, 210, 0, FALSE );
				DDOverlayOSD( hInst, g_hDlg, 320, 50, 15, 10, 0, TEXT("Battery 0"), IDB_BATT0, 32, 20, 210, 0, IDB_SIG0, 32, 20, 160, 0, FALSE );
				break;

		}	///switch
	#else
		///DDOverlayText( hInst, g_hDlg, 100, 100, 10, 10, 0, IDB_BATT0, FALSE );
		///DDOverlayText( hInst, g_hDlg, 100, 100, 10, 10, 0, IDB_BATT0, TRUE );
	#endif
		bRet = TRUE;
	}
#endif   	
   	if( bRet ) {
   		NKDbgPrintfW( L"TVB:: TvSetBattery %d %d \r\n", dwBattery, dwRetSize );
   		///pFBuffer4->SetBufDataLen( dwRetSize );
   		///pFBuffer4->DumpBuffer();
   		g_dwModBattery = dwBattery;
	}
	else
		NKDbgPrintfW( L"TVB:: TvSetBattery DeviceIoControl %d %d fail\r\n", dwBattery, dwRetSize );
	
	if( dwCurBattery > 6 )
		dwCurBattery = 0;
}

void TvContrast()
{
	static DWORD dwCurContrast = 0;
	DWORD dwContrast = 0;
	DWORD dwRetSize;
	BOOL bRet;

	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		dwServiceEPGChanged++;
		return;
	}

	dwContrast = dwCurContrast;
	dwCurContrast++;	
	
   	bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_SET_CONTRAST, &dwContrast, sizeof(dwContrast), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   	if( bRet ) {
   		NKDbgPrintfW( L"TVB:: TvContrast %d %d \r\n", dwContrast, dwRetSize );
   		pFBuffer4->SetBufDataLen( dwRetSize );
   		pFBuffer4->DumpBuffer();
	}
	else
		NKDbgPrintfW( L"TVB:: TvContrast DeviceIoControl %d %d fail\r\n", dwContrast, dwRetSize );
	
	if( dwCurContrast > 9 )
		dwCurContrast = 0;
}

void TvSaturation()
{
	static DWORD dwCurSaturation = 0;
	DWORD dwSaturation = 0;
	DWORD dwRetSize;	
	BOOL bRet;

	dwSaturation = dwCurSaturation;
	dwCurSaturation++;	
	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		ISDB_T_SIGNALSTATUS SignalStatus;
		GetSignalStatus( &SignalStatus );
		bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_OSD_SIGNAL_LEVEL, &dwSaturation, sizeof(dwSaturation), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
	} else {
		bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_SET_SATURATION, &dwSaturation, sizeof(dwSaturation), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
	}
   	if( bRet ) {
   		NKDbgPrintfW( L"TVB:: TvSaturation %d %d \r\n", dwSaturation, dwRetSize );
   		pFBuffer4->SetBufDataLen( dwRetSize );
   		pFBuffer4->DumpBuffer();
	}
	else
		NKDbgPrintfW( L"TVB:: TvSaturation DeviceIoControl %d %d fail\r\n", dwSaturation, dwRetSize );


	if( dwCurSaturation > 9 )
		dwCurSaturation = 0;
	
}

void TvBrightness()
{
	static DWORD dwCurBrightness = 0;
	DWORD dwBrightness = 0;
	DWORD dwRetSize;	
	BOOL bRet;


	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		HANDLE hTemp;
		hTemp = CreateEvent(NULL, FALSE, FALSE, NAMED_CMD_2_EVENT );
		SetEvent( hTemp );
		Sleep(1000);
		CloseHandle( hTemp );
		return;
	}


	dwBrightness = dwCurBrightness;
	dwCurBrightness++;	

   	bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_SET_BRIGHTNESS, &dwBrightness, sizeof(dwBrightness), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   	if( bRet ) {
   		NKDbgPrintfW( L"TVB:: TvBrightness %d %d \r\n", dwBrightness, dwRetSize );
   		pFBuffer4->SetBufDataLen( dwRetSize );
   		pFBuffer4->DumpBuffer();
	}
	else
		NKDbgPrintfW( L"TVB:: TvBrightness DeviceIoControl %d %d fail\r\n", dwBrightness, dwRetSize );


	if( dwCurBrightness > 9 )
		dwCurBrightness = 0;

}

void TvOSD3setting( BOOL bGetSetting )
{
	BOOL bRet = FALSE;
	DWORD dwCmd = 0;
	DWORD dwRetSize = 0;
	static dwOption = 0;
	
	if( bGetSetting )
		dwCmd = IOCTL_CNSCMD_GET_FOCUS;
	else
		dwCmd = IOCTL_CNSCMD_SET_FOCUS;
	
	pFBuffer4->ClearBuffer();
	
   	bRet = DeviceIoControl(hDTV, dwCmd, &dwOption, sizeof(dwOption), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   	if( bRet ) {
   		NKDbgPrintfW( L"TVB:: TvOSD3setting %d %d \r\n", dwOption, dwRetSize );
   		pFBuffer4->SetBufDataLen( dwRetSize );
   		pFBuffer4->DumpBuffer();
	}
	else
		NKDbgPrintfW( L"TVB:: TvOSD3setting DeviceIoControl %d %d fail\r\n", dwOption, dwRetSize );

	if( bGetSetting ) {
		TvSetModuleSysInfo( 0 );
	} else {
	///if( dwCmd == IOCTL_CNSCMD_SET_FOCUS ) {
		dwOption++;
		if( dwOption > 2 ) ///only support 0,1,2
			dwOption = 0;
	}		
}

void TvSetModuleSysInfo( DWORD dwSetting )
{
	BOOL bRet = FALSE;
	DWORD dwRetSize = 0;

	if( 0 != dwSetting ) {
		///set the setting
		pFBuffer4->ClearBuffer();
			
	   	bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_SET_SYSINFO, (PBYTE)&dwSetting, sizeof(dwSetting), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
	   	if( bRet ) {
	   		NKDbgPrintfW( L"TVB:: TvSetModuleSysInfo=0x%08x %d \r\n", dwSetting, dwRetSize );
	   		pFBuffer4->SetBufDataLen( dwRetSize );
	   		pFBuffer4->DumpBuffer();
		}
		else
			NKDbgPrintfW( L"TVB:: TvSetModuleSysInfo DeviceIoControl %d fail\r\n", dwRetSize );

	}
	
	///get the setting
	pFBuffer4->ClearBuffer();
		
   	bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_GET_SYSINFO, NULL, 0, pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   	if( bRet ) {
   		NKDbgPrintfW( L"TVB:: TvGetModuleSysInfo %d \r\n", dwRetSize );
   		pFBuffer4->SetBufDataLen( dwRetSize );
   		pFBuffer4->DumpBuffer();
	}
	else
		NKDbgPrintfW( L"TVB:: TvGetModuleSysInfo DeviceIoControl %d fail\r\n", dwRetSize );
		
}

BOOL TvCheckModule()
{
	DWORD dwRetSize;	
	BOOL bRet = FALSE;

	pFBuffer4->ClearBuffer();
	
   	bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_CHECK_MODULE, NULL, 0, pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   	if( bRet ) {
   		PBYTE pRsp = NULL;

   		NKDbgPrintfW( L"TVB:: TvCheckModule %d \r\n", dwRetSize );
   		pFBuffer4->SetBufDataLen( dwRetSize );
   		pFBuffer4->DumpBuffer();

		pRsp = (PBYTE)pFBuffer4->GetBufferPointer();
		if( 0x20 == *pRsp ) {
			wsprintf( szString, TEXT("check B.B. status OK") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
			bRet = TRUE;
		} else {
			wsprintf( szString, TEXT("check B.B. status Fail") );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
		}
			
	} else {
		NKDbgPrintfW( L"TVB:: TvCheckModule DeviceIoControl %d fail\r\n", dwRetSize );

		wsprintf( szString, TEXT("check B.B. cmd fail") );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);
	}

	MoveList2Last();
	
	return bRet;
}

void TvOSD(DWORD dwCmd, BOOL bOn)
{
	BOOL bRet = FALSE;
	static DWORD dwSysVolume = 0;
	DWORD dwRetSize;

	DWORD Option[2];
	
	Option[0] = dwCmd;
	Option[1] = bOn?1:0;
		
	pFBuffer4->ClearBuffer();	///clear buffer4

	///check for OSD4 system volume display on OSD
	if( 5 == dwCmd ) {
		Option[1] = 0x00000000;		///clear

		///set volume	high word for volume setting
		Option[1] = Option[1] | ((dwSysVolume&0x0000FFFF) << 16);	
		
		if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
			///set on/off	low word for on/off control
			if( 6 == dwSysVolume )
				Option[1] = 0x00000000;///off
			else
				Option[1] = Option[1] | 0x00000001;///on, volume value

			dwSysVolume++;

			if( dwSysVolume > 6 )
				dwSysVolume = 0;
		} else {
			///set on/off	low word for on/off control
			if( 10 == dwSysVolume )
				Option[1] = 0x00000000;
			else
				Option[1] = Option[1] | 0x00000001;

			dwSysVolume++;
			
			if( dwSysVolume > 10 )
				dwSysVolume = 0;
		}
	}
	else if( 1 == dwCmd ) {
		if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T sub-title control
			
			if( bOn )
				bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_SUBTITLE_ON, NULL, 0, pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
			else
				bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_SUBTITLE_OFF, NULL, 0, pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);

			NKDbgPrintfW( L"TVB:: TvOSD IOCTL_DTV_MODULE_SUBTITLE_%s %s\r\n", bOn?L"ON":L"OFF", bRet?L"Ok":L"Fail");
	    	wsprintf( szString, TEXT("Sub-Title %s %s"), bOn?L"ON":L"OFF", bRet?L"Ok":L"Fail" );
			SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);			
			
			return;
		}
	}
	else if ( 0 == dwCmd ) {
		if( bOn )	///OSD1 on
			DtvStatus( DVT_STATE_OSD1ON );
		else	///OSD1 off
			DtvStatus( DVT_STATE_OSD1OFF );

	} 
	
	///get data from driver to buffer4
	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T
		bRet = DeviceIoControl(hDTV, IOCTL_DTV_MODULE_OSD_CTRL, Option, sizeof(Option), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
	} else {
   		bRet = DeviceIoControl(hDTV, IOCTL_CNSCMD_OSD_CTRL, Option, sizeof(Option), pFBuffer4->GetBufferPointer(), pFBuffer4->GetMaxBufferSize(), &dwRetSize, NULL);
   	}	

	if( 5 == dwCmd ) {
    	wsprintf( szString, TEXT("OSD%d %s @ %d %s"), (dwCmd+1), (Option[1]&0x01)?L"ON":L"OFF", (0 == dwSysVolume)?0:(dwSysVolume-1), bRet?L"Ok":L"Fail" );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);			
	} else {
    	wsprintf( szString, TEXT("OSD%d %s %s"), (dwCmd+1), (Option[1]&0x01)?L"ON":L"OFF", bRet?L"Ok":L"Fail" );
		SendDlgItemMessage( g_hDlg, IDC_MSG_LIST, LB_ADDSTRING, 0, (LPARAM)szString);			
	}

   				
   	if( bRet ) {
   		NKDbgPrintfW( L"TVB:: TvOSD IOCTL_CNSCMD_OSD_CONTENTS %d %d \r\n", dwCmd, dwRetSize );
   		pFBuffer4->SetBufDataLen( dwRetSize );
   		pFBuffer4->DumpBuffer();
	}
	else
		NKDbgPrintfW( L"TVB:: TvOSD DeviceIoControl CONTENTS%d %d fail\r\n", dwCmd, dwRetSize );

}

void TvRecord(DWORD dwCmd)
{
    PDWORD pPara;
    DWORD dwParaSize;
    static HANDLE hRecordThread = NULL;
    static HANDLE hRecFileThread = NULL;
    
    pPara = (PDWORD)LocalAlloc(LPTR, 512);
    dwParaSize = 4;
    
    switch (dwCmd) {
        case 0:
            pPara[0] = 0; // record
            break;
        case 1:
            pPara[0] = 1; // pause
            break;
        case 2:
            pPara[0] = 2; // stop
            break;
        
        case 5:
        	if( NULL == hRecordThread )
        		hRecordThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)TvRecordThread, NULL, 0, NULL);
        	if( NULL == hRecFileThread )
        		hRecFileThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)FileRecordThread, NULL, 0, NULL);

        	///start record       	
        	if( NULL != hRecordThread )
        		DoRecord(1);
        	
        	break;	
        case 6:
        	///stop record
        	if( NULL != hRecordThread )
        		DoRecord(2);

        	break;	
        default:
            return;
    }
	
	if( dwCmd == 0 || dwCmd == 1 || dwCmd == 2 )
	{		
    	if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_RECORD, pPara, dwParaSize, NULL, 0, NULL, NULL) )
        	NKDbgPrintfW( L"TVB:: Record (%d) \r\n", dwCmd );
    	else
	        NKDbgPrintfW( L"TVB:: Failed to Record\r\n" );
	}        
        
    LocalFree(pPara);
}

void TvPlayback(DWORD dwCmd)
{
    PDWORD pPara;
    DWORD dwParaSize;
    static HANDLE hPlaybackThread = NULL;


    pPara = (PDWORD)LocalAlloc(LPTR, 512);
    dwParaSize = 4;
    
    switch (dwCmd) {
        case 0:
            pPara[0] = 0; // playback
            break;
        case 1:
            pPara[0] = 1; // pause
            break;
        case 2:
            pPara[0] = 2; // stop
            break;

        case 5:
        	if( NULL == hPlaybackThread )
        		hPlaybackThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)TvPlaybackThread, NULL, 0, NULL);

        	///start playback
        	if( NULL != hPlaybackThread )
        		DoPlayBack(1);
        	
        	break;	
        case 6:
        	///stop playback
        	if( NULL != hPlaybackThread )
        		DoPlayBack(2);

        	break;	

        case 7:
        	///pause/resume playback
        	if( NULL != hPlaybackThread )
        		DoPlayBack(3);

        	break;	

        default:
            return;
    }

	if( dwCmd == 0 || dwCmd == 1 || dwCmd == 2 )
	{		
    	if ( DeviceIoControl(hDTV, IOCTL_CNSCMD_PLAYBACK, pPara, dwParaSize, NULL, 0, NULL, NULL) )
        	NKDbgPrintfW( L"TVB:: Playback (%d) \r\n", dwCmd );
    	else
	        NKDbgPrintfW( L"TVB:: Failed to Playback\r\n" );
	}        
        
    LocalFree(pPara);
}

void TvSetVideoInSize(DWORD dwSizeX, DWORD dwSizeY)
{
	if( 0x67333130 == dwProjectCode ) {	///PT310 for ISBD-T, fixed input video
		dwSizeX = 360;
		dwSizeY = 480;
	}	
    NKDbgPrintfW( L"TVB:: SetVInSize = %d, %d\r\n", dwSizeX, dwSizeY );
    SetVideoInSize(dwSizeX, dwSizeY);
}

void TvSetVideoOutSize(DWORD dwSizeX, DWORD dwSizeY)
{
    NKDbgPrintfW( L"TVB:: SetVOutSize = %d, %d\r\n", dwSizeX, dwSizeY );
    SetVideoOutSize(dwSizeX, dwSizeY);
}

void TvSetVideoPos(DWORD dwPosX, DWORD dwPosY)
{
    NKDbgPrintfW( L"TVB:: SetVOutPos = %d, %d\r\n", dwPosX, dwPosY );
    SetVideoPos(dwPosX, dwPosY);
}

void TvVideoPosition(DWORD dwCmd)
{
	static DWORD dwOx = 0;
	static DWORD dwOy = 0;
	///static DWORD dwOx = 15;
	///sstatic DWORD dwOy = 105;
	
	switch (dwCmd)
	{
		case 1:
			dwOx = 0;
			dwOy = 0;
			break;
		case 2:
			dwOx = dwOx + 5; 
			break;
		case 3:
			dwOy = dwOy + 5;
			break;
		default:
			NKDbgPrintfW( L"TVB:: TvVideoPosition unknow cmd = %d\r\n", dwCmd );
			break;		
	}//switch
	
	NKDbgPrintfW( L"TVB:: TvVideoPosition cmd = %d\r\n", dwCmd );
	TvVedioOff();
	
	if( dwOx >= 0 && dwOy >= 0 ) 
		TvSetVideoPos( dwOx, dwOy );
		
	TvVedioOn();
}
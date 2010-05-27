// TvBrowserFun.cpp: implementation of the TvBrowserFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TvBrowserFun.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TvBrowserFun::TvBrowserFun()
    : CurrEnsembleId(0), 
      CurrSubChId(0)
{
    int i;

    ghLibInst = LoadLibrary(L"dtv.dll");

    pSysSendMessage = (PFNSysSendMessage)GetProcAddress(ghLibInst, L"SysSendMessage");
    if ( pSysSendMessage == NULL )
        NKDbgPrintfW( L"TVB:: failed to get PTR of SysSendMessage\r\n" );

    pSysGetMessage = (PFNSysGetMessage)GetProcAddress(ghLibInst, L"SysGetMessage");
    if ( pSysGetMessage == NULL )
        NKDbgPrintfW( L"TVB:: failed to get PTR of SysGetMessage\r\n" );

    pBufOut = (PBYTE)LocalAlloc(LPTR, 256*1024);
    pCS = (CarrierStatus *)pBufOut;

    for (i=0; i<MAX_ENSEMBLE_NUM; i++) {
        pEnsembleList[i] = NULL;
        dwSvcListCount[i] = 0;
    }
    
    ScanService(0);
    SetVideoSize(320, 240);
    SetVideoPos(0, 30);
}

TvBrowserFun::~TvBrowserFun()
{
    int i;

    for (i=0; i<MAX_ENSEMBLE_NUM; i++) {
        if ( pEnsembleList[i] )
            LocalFree(pEnsembleList[i]);
    }

    LocalFree(pBufOut);
}

DWORD TvBrowserFun::SwapDword(DWORD dwVal)
{
    DWORD dwRet;
    
    dwRet = ((dwVal & 0xff000000) >> 24) |
            ((dwVal & 0x00ff0000) >> 8)  |
            ((dwVal & 0x0000ff00) << 8)  |
            ((dwVal & 0x000000ff) << 24);
    
    return dwRet;
}

DWORD TvBrowserFun::SwapWord(WORD wVal)
{
    WORD wRet;
    
    wRet = ((wVal & 0xff00) >> 8) |
           ((wVal & 0x00ff) << 8);
    
    return wRet;
}

void TvBrowserFun::ScanService(DWORD EnsembleIndex)
{
    DWORD ServiceId[3];
    PBYTE pSvcList;
    PSvcInfo pSvcPtr;
    DWORD i;

    // scan services
    MsgIn.msg = IOCTL_CNSCMD_ACCESS_SVC;
    ServiceId[0] = EnsembleIndex;
    ServiceId[1] = 0;
    ServiceId[2] = 0;
    MsgIn.length = 12;
    MsgIn.pdata = (PBYTE)ServiceId;
    nRet = pSysSendMessage(&MsgIn);
    NKDbgPrintfW( L"TVB:: pSysSendMsg IOCTL_CNSCMD_ACCESS_SVC, scan = %d\r\n", nRet );
    do {
        Sleep(1000);
        nRet = pSysGetMessage(&MsgOut);
        NKDbgPrintfW( L"TVB:: pSysGetMsg IOCTL_CNSCMD_ACCESS_SVC, scan: R=%d, M=%d, L=%d\r\n", nRet, MsgOut.msg, MsgOut.length );
    } while ( nRet == 1 );

    // save service list
    if ( MsgOut.length > 0 ) {
        if ( pEnsembleList[EnsembleIndex] != NULL )
            LocalFree(pEnsembleList[EnsembleIndex]);
        pSvcList = (PBYTE)LocalAlloc(LPTR, MsgOut.length);
        pEnsembleList[EnsembleIndex] = (PSvcInfo)pSvcList;

        memcpy(pSvcList, MsgOut.pdata, MsgOut.length);
        dwSvcListCount[EnsembleIndex] += (MsgOut.length / sizeof(SvcInfo));

		NKDbgPrintfW(L"SVC_INFO:: Ensemble = %d, total service count = %d\r\n", EnsembleIndex, dwSvcListCount[EnsembleIndex]);
        for (i=0; i<dwSvcListCount[EnsembleIndex]; i++) {
		    pSvcPtr = &((PSvcInfo)pSvcList)[i];
            
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
		NKDbgPrintfW( L"\r\n" );
		NKDbgPrintfW( L"\r\n" );
    }
}

void TvBrowserFun::SetChannel(DWORD EnsembleIndex, DWORD SvcIndex)
{
    DWORD ServiceId[3];
    PBYTE pSvcList;
    PSvcInfo pSvcPtr;
    
    pSvcList = (PBYTE)pEnsembleList[EnsembleIndex];
    pSvcPtr = &((PSvcInfo)pSvcList)[SvcIndex];

    MsgIn.msg = IOCTL_CNSCMD_ACCESS_SVC;

    ServiceId[0] = pSvcPtr->freq_index;
    ServiceId[1] = pSvcPtr->srv_id;
    ServiceId[2] = pSvcPtr->sub_ch_id;
    MsgIn.length = 12;
    MsgIn.pdata = (PBYTE)ServiceId;
    nRet = pSysSendMessage(&MsgIn);
    NKDbgPrintfW( L"TVB:: pSysSendMsg IOCTL_CNSCMD_ACCESS_SVC, SetCh = %d\r\n", nRet );
    do {
        Sleep(1000);
        nRet = pSysGetMessage(&MsgOut);
        NKDbgPrintfW( L"TVB:: pSysGetMsg IOCTL_CNSCMD_ACCESS_SVC, SetCh: R=%d, M=%d, L=%d\r\n", nRet, MsgOut.msg, MsgOut.length );
    } while ( nRet == 1 );
}

void TvBrowserFun::SetVideoPos(DWORD dwPosX, DWORD dwPosY)
{
    CifSizePos NewPos;
    
    NewPos.x = dwPosX;
    NewPos.y = dwPosY;
    
    MsgIn.msg = IOCTL_VIDEO_OUT_POS;
    MsgIn.length = sizeof(CifSizePos);
    MsgIn.pdata = (PBYTE)&NewPos;
    nRet = pSysSendMessage(&MsgIn);
    NKDbgPrintfW( L"TVB:: pSysSendMsg IOCTL_VIDEO_OUT_POS = %d\r\n", nRet );
}

void TvBrowserFun::SetVideoSize(DWORD dwSizeX, DWORD dwSizeY)
{
    CifSizePos NewSize;
    
    NewSize.x = dwSizeX;
    NewSize.y = dwSizeY;
    
    MsgIn.msg = IOCTL_VIDEO_OUT_SIZE;
    MsgIn.length = sizeof(CifSizePos);
    MsgIn.pdata = (PBYTE)&NewSize;
    nRet = pSysSendMessage(&MsgIn);
    NKDbgPrintfW( L"TVB:: pSysSendMsg IOCTL_VIDEO_OUT_SIZE = %d\r\n", nRet );
}

void TvBrowserFun::TvChannelUp()
{
    DWORD uLastEnId = CurrEnsembleId, uLastChId = CurrSubChId;

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
        SetChannel(CurrEnsembleId, CurrSubChId);
}

void TvBrowserFun::TvChannelDown()
{
    DWORD uLastEnId = CurrEnsembleId, uLastChId = CurrSubChId;

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

    CurrSubChId--;

    if ( (uLastEnId != CurrEnsembleId) || (uLastChId != CurrSubChId) )
        SetChannel(CurrEnsembleId, CurrSubChId);
}

void TvBrowserFun::TvVolumlUp()
{
}

void TvBrowserFun::TvVolumlDown()
{
}

void TvBrowserFun::TvVedioOn()
{
    MsgIn.msg = IOCTL_VIDEO_ON;
    MsgIn.length = 0;
    nRet = pSysSendMessage(&MsgIn);
    NKDbgPrintfW( L"TVB:: pSysSendMsg IOCTL_VIDEO_ON = %d\r\n", nRet );
}

void TvBrowserFun::TvVedioOff()
{
    MsgIn.msg = IOCTL_VIDEO_OFF;
    MsgIn.length = 0;
    nRet = pSysSendMessage(&MsgIn);
    NKDbgPrintfW( L"TVB:: pSysSendMsg IOCTL_VIDEO_OFF = %d\r\n", nRet );
}

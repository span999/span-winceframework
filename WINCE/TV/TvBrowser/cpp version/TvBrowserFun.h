// TvBrowserFun.h: interface for the TvBrowserFun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TVBROWSERFUN_H__C247414D_37E0_48F6_9330_765A844A5830__INCLUDED_)
#define AFX_TVBROWSERFUN_H__C247414D_37E0_48F6_9330_765A844A5830__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mnbt.h"

#define MAX_ENSEMBLE_NUM            22

typedef SYS_ERROR (WINAPI *PFNSysSendMessage)(APIMessage *msg);
typedef SYS_ERROR (WINAPI *PFNSysGetMessage)(APIMessage *msg);


class TvBrowserFun  
{

public:
	TvBrowserFun();
	virtual ~TvBrowserFun();

	void TvChannelUp();
	void TvChannelDown();
	void TvVolumlUp();
	void TvVolumlDown();
	void TvVedioOn();
	void TvVedioOff();

private:
    HINSTANCE ghLibInst;
	PFNSysSendMessage pSysSendMessage;
	PFNSysGetMessage pSysGetMessage;
    APIMessage MsgIn, MsgOut;
    PBYTE pBufOut;
    PSvcInfo pEnsembleList[MAX_ENSEMBLE_NUM];
    DWORD dwSvcListCount[MAX_ENSEMBLE_NUM];
    DWORD CurrEnsembleId, CurrSubChId;
    CarrierStatus *pCS;
    SYS_ERROR nRet;
    
    DWORD SwapDword(DWORD dwVal);
    DWORD SwapWord(WORD wVal);
    void ScanService(DWORD EnsembleIndex);
    void SetChannel(DWORD EnsembleIndex, DWORD SvcIndex);
    void SetVideoPos(DWORD dwPosX, DWORD dwPosY);
    void SetVideoSize(DWORD dwSizeX, DWORD dwSizeY);

};

#endif // !defined(AFX_TVBROWSERFUN_H__C247414D_37E0_48F6_9330_765A844A5830__INCLUDED_)

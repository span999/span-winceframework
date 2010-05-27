// TvBrowserFun.h: interface for the TvBrowserFun class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_TVBROWSERFUN_H__C247414D_37E0_48F6_9330_765A844A5830__INCLUDED_
#define AFX_TVBROWSERFUN_H__C247414D_37E0_48F6_9330_765A844A5830__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mnbt.h"
#include "dtv_def.h"
#include "cns_spi_cmd.h"
///#include "warpper.h"


///#define	OVERLAY_MOSQUITO
#define	OVERLAY_DRAWTEXT

typedef enum _DTV_STATE {
    PWROFF,
    PWRON,
    LOCKED,
    DISPON,
    RECORD,
    PLAYBACK,
    STILLSHOT,
} DTV_STATE, *PDTV_STATE;


#define		DVT_STATE_PWROFF		0x00000001
#define		DVT_STATE_PWRON			0x00000002
#define		DVT_STATE_LOCKED		0x00000010
#define		DVT_STATE_DISPON		0x00000100
#define		DVT_STATE_DISPOFF		0x00000200
#define		DVT_STATE_RECOED		0x00001000
#define		DVT_STATE_PLAYBACK		0x00002000
#define		DVT_STATE_STILLSHOT		0x00004000
#define		DVT_STATE_STOPFUNC		0x00010000
#define		DVT_STATE_OSD1ON		0x01000000
#define		DVT_STATE_OSD1OFF		0x02000000

///span1110
typedef struct T_CNS_PLAYBACK_BUFFERIN
{
    DWORD       Operation;
    PBYTE       pBufIn;
    DWORD       BufInSize;    
} CNS_PLAYBACK_BUFFERIN, *PCNS_PLAYBACK_BUFFERIN;
///span1110


#define MAX_ENSEMBLE_NUM            22

#define 	NAMED_CMD_REDRAW_EVENT		TEXT("named cmd redraw event")
#define 	NAMED_CMD_1_EVENT			TEXT("named cmd 1 event")
#define 	NAMED_CMD_2_EVENT			TEXT("named cmd 2 event")

#define 	NAMED_RECORDSTART_EVENT		TEXT("named recordstart event")
#define 	NAMED_RECORDING_EVENT		TEXT("named recording event")
#define 	NAMED_RECORDSTOP_EVENT		TEXT("named recordstop event")
#define 	NAMED_STILLSHOTSTART_EVENT	TEXT("named stillshotstart event")
#define 	NAMED_STILLSHOTING_EVENT	TEXT("named stillshoting event")
#define 	NAMED_STILLSHOTSTOP_EVENT	TEXT("named stillshotstop event")
#define 	NAMED_FILESTART_EVENT		TEXT("named file start event")
#define 	NAMED_FILESTOP_EVENT		TEXT("named file stop event")
#define 	NAMED_GETBUFFER_DONE_EVENT	TEXT("named get buffer done event")
#define 	NAMED_PLAYBACKSTART_EVENT	TEXT("named playbackstart event")
#define 	NAMED_PLAYBACKSTOP_EVENT	TEXT("named playbackstop event")
#define 	NAMED_PLAYBACKPAUSE_EVENT	TEXT("named playbackpause event")



#define 	Clear_allChannel	0x01
#define 	Up_allChannel		0x02
#define 	Down_allChannel		0x04




typedef SYS_ERROR (WINAPI *PFNSysSendMessage)(APIMessage *msg);
typedef SYS_ERROR (WINAPI *PFNSysGetMessage)(APIMessage *msg);
typedef DWORD (WINAPI *PFNDTV_Init)(DWORD dwContext);

void TvBrowserFunInit();
void TvBrowserFunDeinit();

BOOL TvChannelUp(BOOL &bDMB);
BOOL TvChannelDown(BOOL &bDMB);
void TvVolumlUp();
void TvVolumlDown();
void TvVedioOn();
void TvVedioOff();
BOOL TvScanService(DWORD EnsembleIndex);
void TvGetChLabel(PBYTE pBufIn);
void TvGetSignal(DWORD &dwStrength, DWORD &dwSNR, DWORD &dwBER);
void TvGetVersionId(PBYTE pBufOut);
void TvGetChecksum(PBYTE pBufOut);
///void TvNeptunePowerOnOff(BOOL bOn);
///void TvNeptunePowerOnOff(BOOL bOn, BOOL bLog);
DWORD DTVmoduleFWversion();
void TvRecord(DWORD dwCmd);
void TvPlayback(DWORD dwCmd);
void TvStillShot(DWORD dwCmd);
void TvDataService(DWORD dwCmd);
void TvOSD(DWORD dwOption, BOOL bOn);
void TvContrast();
void TvSaturation();
void TvBrightness();
BOOL TvSendBatteryInfo( BOOL bForceUpdate );
BOOL TvSendSignalInfo( BOOL bForceUpdate );
void TvSetBattery();
BOOL TvCheckModule();
void TvRFregisterDump( DWORD dwOption, BOOL bLog );
void TvRFstatusDump( DWORD dwOption, BOOL bLog );
void MoveList2Last();
void TvCmdLoopBack();
void TvPowerOnTest( DWORD dwLoop, BOOL bPwrControl );
void TvCmdZoomTest( BOOL bOnPlay );
void TvChangeScreen( BOOL bPlayBack );
void TvSetModuleSysInfo( DWORD dwSetting );
void TvOSD3setting( BOOL bGetSetting );
void TvVideoPosition(DWORD dwCmd);
void TvSetVideoInSize(DWORD dwSizeX, DWORD dwSizeY);
void TvSetVideoOutSize(DWORD dwSizeX, DWORD dwSizeY);
void TvSetVideoPos(DWORD dwPosX, DWORD dwPosY);
DWORD DtvStatus( DWORD dwNewState );


BOOL ISDBallChannelOperation( DWORD dwOperation, PCHANNEL_INFO pChannelInfo );
BOOL TVISDBchangeAVparameter( PDWORD pdwAVparameter, PDWORD pdwAVparaSwitch );
BOOL GetCurrentChServiceInfo( PISDB_T_CH_SERVICE_INFO pChServInfo );
BOOL GetCurrentChDetailInfo( PISDB_T_CH_DETAIL_INFO pChDetailInfo );
BOOL TvModulePowerOnOff( BOOL bOn, BOOL bLog );
HANDLE GetModuleEventHandel(void);

DWORD SwapDword(DWORD dwVal);
WORD SwapWord(WORD wVal);
BOOL ScanService(DWORD EnsembleIndex);
void SetVolume(BYTE uVol);
BOOL SetChannel(DWORD EnsembleIndex, DWORD SvcIndex, BOOL &bDMB);
BOOL SetChannelISDBT( DWORD dwChannel );
BOOL SetChannelISDBT2( DWORD dwChannel, DWORD dwService );
void SetVideoPos(DWORD dwPosX, DWORD dwPosY);
void SetVideoInSize(DWORD dwSizeX, DWORD dwSizeY);
void SetVideoOutSize(DWORD dwSizeX, DWORD dwSizeY);
BOOL GetSignalStatus( PISDB_T_SIGNALSTATUS pSignalStatus );
void SetVideoFormat601();
void SetVideoFormat656();
void TvDrvEventThread(void);
void TvCommandThread(void);
BOOL DumpDriverReg();
BOOL ParseErrorCode( PBYTE pErrBuf );

#endif // !defined(AFX_TVBROWSERFUN_H__C247414D_37E0_48F6_9330_765A844A5830__INCLUDED_)

/*
	header file for spLog class 
	spCElog.h will be main file.
*/

#ifndef __SPCELOG_H__
#define __SPCELOG_H__



#define SPCELOG_BUFFERHANDLE_EVENT_NAME	TEXT("named buffer handle event")
#define SPCELOG_LOGFILE_NAME			TEXT("spCElog.log")
#define SPCELOG_DLFFILE_NAME			TEXT("spCElog.dlf")
#define SPCELOG_STOPFILE_NAME			TEXT("Copy of spCElog.log")
#define SPCELOG_SDLOGFILE_NAME			TEXT("\\Storage Card\\spCElog.log")
#define SPCELOG_SDDLFFILE_NAME			TEXT("\\Storage Card\\spCElog.dlf")
#define SPCELOG_SDSTOPFILE_NAME			TEXT("\\Storage Card\\Copy of spCElog.log")

#define SPCELOG_EXTRAINFO_ENABLE_MASK				0x00008000
#define SPCELOG_EXTRAINFO_TIMESTAMP_ENABLE_MASK		0x00004000
#define SPCELOG_EXTRAINFO_ORDER_ENABLE_MASK			0x00002000
#define SPCELOG_EXTRAINFO_DIRECTION_ENABLE_MASK		0x00001000

#define SPCELOG_DWORD_ERROR_CODE_BASE	0x00100000
#define SPCEL_ERR_BASE					SPCELOG_DWORD_ERROR_CODE_BASE
#define SPCEL_ERR_OK					(SPCEL_ERR_BASE+1)
#define SPCEL_ERR_PARAMETER				(SPCEL_ERR_BASE+2)
#define SPCEL_ERR_POINTER				(SPCEL_ERR_BASE+3)
#define SPCEL_ERR_INIT					(SPCEL_ERR_BASE+4)
#define SPCEL_ERR_OOM					(SPCEL_ERR_BASE+5)

#define SPCELOG_LOGMODE_INIT			0
#define SPCELOG_LOGMODE_SPY				1
#define SPCELOG_LOGMODE_STANDALONE		2

typedef struct SPCELOG_STATUS_T
{
	BOOL		bInited;
	BOOL		bIsStop;
	BOOL		bStoping;
	BOOL		bUSBconnected;
	BOOL		bFakeHostenabled;
	BOOL		bSaveLogOnSD;
	BOOL		bDebugLogEnabled;
	DWORD		dwLogModes;
	CRITICAL_SECTION	cs; 
	HANDLE		hDTREvent;
	HANDLE		hSIOUSBconnectEvent;
	HANDLE		hSIOUSBdisconnectEvent;
    HANDLE		hBufferHandling;
    HANDLE		hLogCtrlHandling;
	HANDLE		hFile;
	HANDLE		hDLFFile;
	HANDLE		hBufferEvent;
	HANDLE		hCtrlEvent;
    DWORD   	dwBufferWaitTimeout;
	DWORD		dwLastCmdTime;
	DWORD		dwLastRspTime;
	DWORD		dwLastEventTime;
	DWORD		dwExtraInfo;
} spCElog_status;

BOOL __log_get_WaitForRsp();
BOOL __log_get_WaitForCmd();
BOOL __log_get_FakeHostEnabled();
BOOL __log_set_USB_connected( BOOL bConnect );
HANDLE __log_set_USB_DTR_EventHandler( HANDLE hDTR );
HANDLE __log_set_USB_Connect_EventHandler( HANDLE hDTR );
HANDLE __log_set_USB_Disconnect_EventHandler( HANDLE hDTR );
void __log_set_EventCmd_Reset_Timer();
BOOL __log_init();
DWORD __log_inp( PUCHAR pucInput, DWORD dwSize );
DWORD __log_inp1( PUCHAR pucInput, DWORD dwSize );
DWORD __log_inp2( PUCHAR pucInput, DWORD dwSize );
DWORD __log_inp3( PUCHAR pucInput, DWORD dwSize );
int __log_out( PUCHAR pucInput, int dwSize );
BOOL __log_close();

#endif	///#ifndef __SPCELOG_H__
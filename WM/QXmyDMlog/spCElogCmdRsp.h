/*
	header file for spLog2txt
	
*/

#ifndef __SPCELOGCMDRSP_H__
#define __SPCELOGCMDRSP_H__


typedef struct SPCELOG_CMD_MAPPING_T
{
	DWORD		dwIndex;
    DWORD   	dwCmdLen;
////	BYTE		bCmd[7];	
	BYTE		bCmd[10];	///add cmd length
    DWORD   	dwNameLen;
	UCHAR		ucName[128];
	BYTE		bDLFchk[2];
} spCElog_Cmd_Mapping;


typedef struct SPCELOG_RSP_MAPPING_T
{
	DWORD		dwIndex;
    DWORD   	dwRspLen;
	BYTE		bRsp[10];
    DWORD   	dwNameLen;
	UCHAR		ucName[128];
	BYTE		bDLFchk[2];
} spCElog_Rsp_Mapping;


///extern spCElog_Cmd_Mapping	Cmd_Table[];

UCHAR* Find_Cmd_Descript( BYTE* pData, DWORD dwLen );
UCHAR* Find_Rsp_Descript( BYTE* pData, DWORD dwLen );
UCHAR* Find_Cmd_Chksum( BYTE* pData, DWORD dwLen );
UCHAR* Find_Rsp_Chksum( BYTE* pData, DWORD dwLen );


BOOL Is_Polling_Cmd( BYTE* pData, DWORD dwDataLen );
BOOL Is_ExtMsgReportCfg_Cmd( BYTE* pData, DWORD dwDataLen );
BOOL Is_ExtMsgReportCfg6_Cmd( BYTE* pData, DWORD dwDataLen );
BOOL Is_Report_Control_On_Cmd( BYTE* pData, DWORD dwDataLen );
BOOL Is_TimeStamp_Request_Cmd( BYTE* pData, DWORD dwDataLen );


BOOL Is_Polling_Rsp( BYTE* pData, DWORD dwDataLen );
BOOL Is_EVENT_Rsp( BYTE* pData, DWORD dwDataLen );
BOOL Is_LOG_Rsp( BYTE* pData, DWORD dwDataLen );
BOOL Is_MSG_Rsp( BYTE* pData, DWORD dwDataLen );
BOOL Is_ExtMsgReportCfg_Rsp( BYTE* pData, DWORD dwDataLen );

#endif	///#ifndef __SPCELOGCMDRSP_H__